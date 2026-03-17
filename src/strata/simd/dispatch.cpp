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
// Runtime backend detection
// ============================================================================

#ifdef STRATA_X86

static Backend detect_backend_impl() noexcept {
    // cpuid EAX=7, ECX=0 → EBX:
    //   bit 5:  AVX2
    //   bit 8:  BMI2
    //   bit 16: AVX512F
    //   bit 30: AVX512BW
    //
    // cpuid EAX=1 → ECX:
    //   bit 1:  PCLMULQDQ

    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;

#if defined(__GNUC__) || defined(__clang__)
    __cpuid(0, eax, ebx, ecx, edx);
    unsigned int max_leaf = eax;
    if (max_leaf < 7)
        return Backend::SCALAR;

    __cpuid(1, eax, ebx, ecx, edx);
    bool has_pclmul = (ecx >> 1) & 1;

    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    bool has_avx2 = (ebx >> 5) & 1;
    bool has_bmi2 = (ebx >> 8) & 1;
    bool has_avx512f = (ebx >> 16) & 1;
    bool has_avx512bw = (ebx >> 30) & 1;

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
    bool has_avx512f = (info[1] >> 16) & 1;
    bool has_avx512bw = (info[1] >> 30) & 1;
#else
    bool has_avx2 = false, has_bmi2 = false, has_pclmul = false;
    bool has_avx512f = false, has_avx512bw = false;
#endif

    // AVX-512 requires F + BW (byte-level operations) + PCLMUL for prefix-XOR.
    if (has_avx512f && has_avx512bw && has_pclmul)
        return Backend::AVX512;

    if (!has_avx2 || !has_pclmul)
        return Backend::SCALAR;

    return has_bmi2 ? Backend::AVX2_BMI2 : Backend::AVX2;
}

#elif defined(__aarch64__) || defined(__ARM_NEON)

static Backend detect_backend_impl() noexcept {
    // SVE2 is detected at compile time (__ARM_FEATURE_SVE2).
    // At runtime on Linux we could check /proc/cpuinfo or HWCAP2,
    // but compile-time detection is sufficient for statically-linked paths.
#if defined(__ARM_FEATURE_SVE2)
    return Backend::SVE2;
#else
    return Backend::NEON;
#endif
}

#elif defined(__wasm_simd128__)

static Backend detect_backend_impl() noexcept { return Backend::WASM_SIMD; }

#elif defined(__riscv_v) || defined(__riscv_vector)

static Backend detect_backend_impl() noexcept { return Backend::RVV; }

#else

static Backend detect_backend_impl() noexcept { return Backend::SCALAR; }

#endif // platform detection

Backend detect_backend() noexcept {
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

IndexBuilder::StructuralIndex index_document(const uint8_t* data, size_t len) {
    IndexBuilder builder;
    return builder.build(data, len);
}

} // namespace simd
} // namespace strata
