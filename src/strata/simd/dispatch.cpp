// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.

#include "strata/simd/dispatch.h"

// cpuid detection on x86
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define STRATA_X86 1
#if defined(__GNUC__) || defined(__clang__)
#include <cpuid.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#endif
#endif

namespace strata {
namespace simd {

// ============================================================================
// Runtime backend detection via cpuid
// ============================================================================

#ifdef STRATA_X86

static Backend detect_backend_impl() noexcept {
    // EAX=7, ECX=0 gives extended feature flags.
    //   EBX bit 5:  AVX2
    //   EBX bit 8:  BMI2
    //
    // EAX=1 gives basic feature flags.
    //   ECX bit 1:  PCLMULQDQ

    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;

#if defined(__GNUC__) || defined(__clang__)
    // Check maximum supported cpuid leaf.
    __cpuid(0, eax, ebx, ecx, edx);
    unsigned int max_leaf = eax;

    if (max_leaf < 7)
        return Backend::SCALAR;

    // EAX=1: basic feature flags (PCLMUL in ECX bit 1).
    __cpuid(1, eax, ebx, ecx, edx);
    bool has_pclmul = (ecx >> 1) & 1;

    // EAX=7, ECX=0: extended features (AVX2, BMI2 in EBX).
    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    bool has_avx2 = (ebx >> 5) & 1;
    bool has_bmi2 = (ebx >> 8) & 1;

#elif defined(_MSC_VER)
    int info[4];
    __cpuid(info, 0);
    unsigned int max_leaf = static_cast<unsigned int>(info[0]);
    if (max_leaf < 7)
        return Backend::SCALAR;

    __cpuid(info, 1);
    bool has_pclmul = (info[2] >> 1) & 1;

    __cpuidex(info, 7, 0);
    bool has_avx2 = (info[1] >> 5) & 1;
    bool has_bmi2 = (info[1] >> 8) & 1;
#else
    bool has_avx2 = false;
    bool has_bmi2 = false;
    bool has_pclmul = false;
#endif

    // We require at least AVX2 + PCLMUL for the SIMD path.
    if (!has_avx2 || !has_pclmul)
        return Backend::SCALAR;

    return has_bmi2 ? Backend::AVX2_BMI2 : Backend::AVX2;
}

#elif defined(__ARM_NEON) || defined(__aarch64__)

static Backend detect_backend_impl() noexcept {
    // NEON structural indexer is not yet implemented — the classify() path
    // falls through to the scalar #else branch.  Report SCALAR until a real
    // NEON implementation exists so callers get an accurate label.
    return Backend::SCALAR;
}

#else

static Backend detect_backend_impl() noexcept { return Backend::SCALAR; }

#endif // STRATA_X86

Backend detect_backend() noexcept {
    // Cache the result in a function-local static (thread-safe in C++11+).
    static Backend cached = detect_backend_impl();
    return cached;
}

const char* backend_name(Backend b) noexcept {
    switch (b) {
    case Backend::SCALAR:
        return "scalar";
    case Backend::AVX2:
        return "avx2";
    case Backend::AVX2_BMI2:
        return "avx2+bmi2";
    case Backend::NEON:
        return "neon";
    }
    return "unknown";
}

// ============================================================================
// Convenience entry point
// ============================================================================

IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len) {
    // The IndexBuilder uses StructuralIndexer internally, which already
    // selects the AVX2 or scalar path at compile time.  Runtime dispatch
    // here is informational — the actual SIMD path is determined by
    // compile flags (-mavx2, -mpclmul, -mbmi2).
    //
    // A future enhancement could use ifunc or dlsym-based dispatch to
    // select between separately compiled object files at load time.
    IndexBuilder builder;
    return builder.build(data, len);
}

} // namespace simd
} // namespace strata
