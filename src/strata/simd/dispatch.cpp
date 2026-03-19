// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
//
// This file implements runtime SIMD backend detection and selection.
// At startup (on first call to detect_backend()), the library queries the
// CPU for supported instruction sets and selects the highest-performance
// backend that the hardware supports.  The result is cached in a
// function-local static so that the CPUID overhead is paid only once.
//
// Supported backends and their priority (highest first):
//
//   ┌──────────┬──────────────────────────────────────────────────────┐
//   │ Backend  │ Requires                                            │
//   ├──────────┼──────────────────────────────────────────────────────┤
//   │ AVX-512  │ AVX-512F + AVX-512BW + PCLMULQDQ                   │
//   │ AVX2+BMI2│ AVX2 + BMI2 + PCLMULQDQ  (adds PEXT/PDEP/BLSR)    │
//   │ AVX2     │ AVX2 + PCLMULQDQ                                   │
//   │ NEON     │ ARM NEON (aarch64 baseline)                         │
//   │ SVE2     │ ARM SVE2 (detected at compile time)                 │
//   │ WASM SIMD│ WebAssembly SIMD 128-bit                            │
//   │ RVV      │ RISC-V Vector Extension                             │
//   │ SCALAR   │ Always available (portable C++ fallback)            │
//   └──────────┴──────────────────────────────────────────────────────┘
//
// The priority chain for x86 is:
//   AVX-512 > AVX2+BMI2 > AVX2 > SCALAR
//
// Why this order:
//   - AVX-512 processes 64 bytes per instruction vs 32 for AVX2, roughly
//     doubling classification throughput per cycle.
//   - AVX2+BMI2 is preferred over plain AVX2 because BMI2 provides PEXT
//     (parallel bit extract) and BLSR (clear lowest set bit) which accelerate
//     the bit-extraction loop in index_builder.cpp.
//   - Plain AVX2 still provides 32-byte SIMD classification, far faster than
//     the scalar byte-at-a-time fallback.
//   - SCALAR is the universal fallback when no SIMD is available.

#include "strata/simd/dispatch.h"

#include <cstring>

// ── x86 CPUID detection headers ──
// CPUID is the x86 instruction for querying CPU feature flags at runtime.
// GCC/Clang expose it via <cpuid.h>; MSVC uses <intrin.h>.
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define STRATA_X86 1
#if defined(__GNUC__) || defined(__clang__)
#include <cpuid.h> // __cpuid(), __cpuid_count()
#elif defined(_MSC_VER)
#include <intrin.h> // __cpuid(), __cpuidex()
#endif
#endif

namespace strata {
namespace simd {

// ============================================================================
// Runtime backend detection
// ============================================================================
//
// detect_backend_impl() performs the actual hardware query.  It is called
// exactly once (see detect_backend() below) and its result is cached for the
// lifetime of the process.  Each platform has its own implementation:
//   - x86: uses CPUID leaves 0, 1, and 7 to probe feature bits
//   - ARM: uses compile-time feature macros (__ARM_NEON, __ARM_FEATURE_SVE2)
//   - WASM/RISC-V: compile-time detection only
//   - Force-scalar: compile-time override via STRATA_FORCE_SCALAR

#if defined(STRATA_FORCE_SCALAR)

/// Forced scalar mode — used for testing or platforms without SIMD support.
static Backend detect_backend_impl() noexcept { return Backend::SCALAR; }

#elif defined(STRATA_X86)

/// @brief Query x86 CPUID to determine the best available SIMD backend.
///
/// Uses CPUID leaves to detect instruction set extensions:
///   - Leaf 0:  Maximum supported CPUID leaf number.
///   - Leaf 1:  Processor feature flags (ECX/EDX).
///   - Leaf 7:  Extended feature flags (EBX/ECX/EDX, sub-leaf 0).
///
/// The specific register bits checked are:
///
///   Leaf 1, ECX:
///     Bit  1 → PCLMULQDQ  (carry-less multiplication, used for fast string
///                           boundary detection in the SIMD classifier)
///
///   Leaf 7 sub-leaf 0, EBX:
///     Bit  5 → AVX2       (256-bit integer SIMD — core classification engine)
///     Bit  8 → BMI2       (PEXT/PDEP/BLSR — accelerates bit extraction)
///     Bit 16 → AVX-512F   (512-bit foundation — doubles throughput vs AVX2)
///     Bit 30 → AVX-512BW  (512-bit byte/word ops — needed for byte shuffles)
///
/// Selection priority (first match wins):
///   1. AVX-512F + AVX-512BW + PCLMULQDQ  →  Backend::AVX512
///   2. AVX2 + PCLMULQDQ + BMI2           →  Backend::AVX2_BMI2
///   3. AVX2 + PCLMULQDQ                  →  Backend::AVX2
///   4. Otherwise                          →  Backend::SCALAR
static Backend detect_backend_impl() noexcept {
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;

#if defined(__GNUC__) || defined(__clang__)
    // ── Leaf 0: query maximum supported CPUID leaf ──
    // We need at least leaf 7 for AVX2/AVX-512/BMI2 detection.
    __cpuid(0, eax, ebx, ecx, edx);
    unsigned int max_leaf = eax; // highest CPUID leaf the CPU supports
    if (max_leaf < 7)
        return Backend::SCALAR; // CPU too old for extended feature detection

    // ── Leaf 1: basic processor features ──
    // ECX bit 1 = PCLMULQDQ (carry-less multiply, needed by the classifier
    // for fast string-escape processing).
    __cpuid(1, eax, ebx, ecx, edx);
    bool has_pclmul = (ecx >> 1) & 1; // ECX bit 1: PCLMULQDQ support

    // ── Leaf 7, sub-leaf 0: extended feature flags ──
    // This leaf contains the AVX2, BMI2, and AVX-512 feature bits.
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    bool has_avx2 = (ebx >> 5) & 1;      // EBX bit  5: AVX2 (256-bit int SIMD)
    bool has_bmi2 = (ebx >> 8) & 1;      // EBX bit  8: BMI2 (PEXT/PDEP/BLSR)
    bool has_avx512f = (ebx >> 16) & 1;  // EBX bit 16: AVX-512 Foundation
    bool has_avx512bw = (ebx >> 30) & 1; // EBX bit 30: AVX-512 Byte & Word
#elif defined(_MSC_VER)
    // MSVC CPUID intrinsics — same logic, different API.
    int info[4];

    // ── Leaf 0: maximum CPUID leaf ──
    __cpuid(info, 0);
    unsigned int max_leaf = static_cast<unsigned int>(info[0]);
    if (max_leaf < 7)
        return Backend::SCALAR;

    // ── Leaf 1: basic features (info[2] = ECX) ──
    __cpuid(info, 1);
    bool has_pclmul = (info[2] >> 1) & 1; // ECX bit 1: PCLMULQDQ

    // ── Leaf 7, sub-leaf 0: extended features (info[1] = EBX) ──
    __cpuidex(info, 7, 0);
    bool has_avx2 = (info[1] >> 5) & 1;      // EBX bit  5: AVX2
    bool has_bmi2 = (info[1] >> 8) & 1;      // EBX bit  8: BMI2
    bool has_avx512f = (info[1] >> 16) & 1;  // EBX bit 16: AVX-512F
    bool has_avx512bw = (info[1] >> 30) & 1; // EBX bit 30: AVX-512BW
#else
    // Non-x86 compiler or unknown platform — fall through to SCALAR.
    bool has_avx2 = false, has_bmi2 = false, has_pclmul = false;
    bool has_avx512f = false, has_avx512bw = false;
#endif

    // ── Backend selection: highest-performance match wins ──

    // Priority 1: AVX-512 (64 bytes/instruction, ~2x throughput over AVX2).
    // Requires both Foundation (F) and Byte/Word (BW) for byte-level shuffles,
    // plus PCLMULQDQ for the string-escape classifier.
    if (has_avx512f && has_avx512bw && has_pclmul)
        return Backend::AVX512;

    // Gate: AVX2 + PCLMULQDQ are the minimum for any SIMD path.
    // Without both, fall back to scalar.
    if (!has_avx2 || !has_pclmul)
        return Backend::SCALAR;

    // Priority 2/3: AVX2 with or without BMI2.
    // BMI2 adds PEXT (parallel bit extract) and BLSR (clear lowest set bit),
    // which accelerate extract_positions() in index_builder.cpp by ~15-20%.
    return has_bmi2 ? Backend::AVX2_BMI2 : Backend::AVX2;
}

// ── ARM backend detection (compile-time) ──
// ARM NEON is baseline on aarch64, so no runtime detection is needed.
// SVE2 support is checked via a compiler-defined feature macro.
#elif defined(__aarch64__) || defined(__ARM_NEON)

static Backend detect_backend_impl() noexcept {
#if defined(__ARM_FEATURE_SVE2)
    return Backend::SVE2; // Scalable Vector Extension 2 (variable-width SIMD)
#else
    return Backend::NEON; // 128-bit fixed-width SIMD (aarch64 baseline)
#endif
}

// ── WebAssembly SIMD detection (compile-time) ──
#elif defined(__wasm_simd128__)
static Backend detect_backend_impl() noexcept { return Backend::WASM_SIMD; }

// ── RISC-V Vector Extension detection (compile-time) ──
#elif defined(__riscv_v) || defined(__riscv_vector)
static Backend detect_backend_impl() noexcept { return Backend::RVV; }

// ── No SIMD detected — universal scalar fallback ──
#else
static Backend detect_backend_impl() noexcept { return Backend::SCALAR; }
#endif

/// @brief Return the detected SIMD backend, caching the result.
///
/// Uses the "Meyers singleton" pattern: the function-local static variable
/// `cached` is initialized exactly once on the first call (thread-safe in
/// C++11 and later, guaranteed by [stmt.dcl] p4).  Subsequent calls simply
/// return the cached value with no synchronization overhead — just a load
/// from an already-initialized static.
///
/// This avoids both:
///   1. Repeated CPUID overhead (CPUID serializes the pipeline on x86).
///   2. Global-constructor ordering issues (no file-scope static needed).
Backend detect_backend() noexcept {
    static Backend cached = detect_backend_impl(); // initialized once, thread-safe
    return cached;
}

// ============================================================================
// Backend naming (string <-> enum)
// ============================================================================

/// @brief Return a human-readable string for the given backend enum value.
///
/// Used in logging, diagnostics, and the Python-facing `strata.simd_backend()`
/// introspection function.  Returns a static string literal (no allocation).
const char* backend_name(Backend b) noexcept {
    switch (b) {
    case Backend::SCALAR:
        return "scalar";
    case Backend::AVX2:
        return "avx2";
    case Backend::AVX2_BMI2:
        return "avx2+bmi2";
    case Backend::AVX512:
        return "avx512";
    case Backend::NEON:
        return "neon";
    case Backend::SVE2:
        return "sve2";
    case Backend::WASM_SIMD:
        return "wasm-simd";
    case Backend::RVV:
        return "rvv";
    }
    return "unknown";
}

// ============================================================================
// Convenience entry point
// ============================================================================

/// @brief One-shot helper: create a temporary IndexBuilder and index the
///        entire document in a single call.
///
/// This is the simplest API for callers that don't need to reuse the builder
/// across multiple documents.  For batch processing, prefer constructing an
/// IndexBuilder once and calling build() repeatedly to amortize the
/// constructor cost and benefit from internal allocator reuse.
///
/// @param data  Pointer to the raw JSON bytes.
/// @param len   Number of bytes to index.
/// @return StructuralIndex with all structural character positions.
IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len) {
    IndexBuilder builder;
    return builder.build(data, len);
}

} // namespace simd
} // namespace strata
