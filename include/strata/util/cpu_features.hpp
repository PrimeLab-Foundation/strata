#pragma once

/**
 * @file cpu_features.hpp
 * @brief The x86-64-v3 check behind the release wheels' import-time CPU guard.
 *
 * The x86-64 release wheels compile every image for x86-64-v3
 * (`-march=x86-64-v3`, `/arch:AVX2` on Windows), so on an older CPU the first
 * such instruction would kill the interpreter with SIGILL. Each image's init
 * entry runs this check first, compiled for the x86-64 baseline, and raises
 * ImportError when it fails (src/strata/bindings/python_cpu_guard.h;
 * docs/architecture/release_pipeline.md, "CPU guard").
 *
 * Two halves. @ref meets_x86_64_v3 is a pure predicate over the CPUID and XCR0
 * words, compiled and tested on every platform (tests/cpp/test_cpu_features.cpp).
 * @ref read_x86_feature_words reads those words, on x86-64 only. Nothing in this
 * file knows about CPython.
 *
 * Every function here carries @ref STRATA_X86_64_BASELINE. That is a
 * correctness requirement, not tidiness: a callee whose instruction set is a
 * superset of its caller's is never inlined into it, so an unmarked helper
 * called from the baseline init entry would be emitted out of line with the
 * translation unit's own (x86-64-v3) instructions and run before the check.
 */

#include <cstdint>

#if defined(__x86_64__) || (defined(_M_X64) && !defined(_M_ARM64EC))
#define STRATA_X86_64 1
#if defined(_MSC_VER) && !defined(__clang__)
#include <immintrin.h> // _xgetbv
#include <intrin.h>    // __cpuidex
#endif
#endif

/// Compiles one function for the x86-64 baseline (SSE2), whatever `-march` or
/// `/arch` its translation unit has. `arch=x86-64` resets the CPU and the
/// features it implies, but clang keeps a feature named by its own flag
/// (`-mavx2`, `-mbmi2`, `-mlzcnt` in CFLAGS survive `arch=` alone), so the
/// `no-` list removes those too. Removing SSE3 removes everything built on it
/// (SSSE3, SSE4.x, AVX, AVX2, FMA, F16C); POPCNT, LZCNT, BMI1/2 and MOVBE stand
/// alone and are named. GCC, clang and clang-cl take the attribute; MSVC
/// proper has no per-function instruction set, so there the macro is empty.
#if defined(STRATA_X86_64) && (defined(__clang__) || defined(__GNUC__))
#define STRATA_X86_64_BASELINE                                                                     \
    __attribute__((target("arch=x86-64,no-sse3,no-popcnt,no-lzcnt,no-bmi,no-bmi2,no-movbe")))
#else
#define STRATA_X86_64_BASELINE
#endif

namespace strata::util {

/// The CPUID and XCR0 words x86-64-v3 is decided from. A word whose leaf the
/// CPU does not report stays zero, and so does `xcr0` when the OS has not
/// enabled XGETBV (CPUID.1:ECX.OSXSAVE clear).
///
/// A plain aggregate, value-initialized where it is made (`X86FeatureWords
/// words{}`): default member initializers would give it a constructor, an
/// ordinary function compiled with the translation unit's instruction set,
/// which an -O0 build calls out of line from the baseline reader.
struct X86FeatureWords {
    uint32_t max_leaf;     ///< CPUID.0:EAX, the highest basic leaf.
    uint32_t leaf1_ecx;    ///< CPUID.1:ECX.
    uint32_t leaf7_ebx;    ///< CPUID.(EAX=7,ECX=0):EBX.
    uint32_t max_ext_leaf; ///< CPUID.80000000h:EAX, the highest extended leaf.
    uint32_t ext1_ecx;     ///< CPUID.80000001h:ECX.
    uint64_t xcr0;         ///< XGETBV with ECX=0: the state components the OS saves.
};

// The x86-64 psABI's levels: x86-64-v2 adds CMPXCHG16B, LAHF-SAHF, POPCNT,
// SSE3, SSE4.1, SSE4.2 and SSSE3 to the baseline; x86-64-v3 adds AVX, AVX2,
// BMI1, BMI2, F16C, FMA, LZCNT, MOVBE and OSXSAVE. `-march=x86-64-v3` may emit
// any of them, so all of them are required.

/// CPUID.1:ECX bits: SSE3 (0), SSSE3 (9), FMA (12), CMPXCHG16B (13),
/// SSE4.1 (19), SSE4.2 (20), MOVBE (22), POPCNT (23), OSXSAVE (27), AVX (28),
/// F16C (29).
inline constexpr uint32_t kV3Leaf1Ecx = (1u << 0) | (1u << 9) | (1u << 12) | (1u << 13) |
                                        (1u << 19) | (1u << 20) | (1u << 22) | (1u << 23) |
                                        (1u << 27) | (1u << 28) | (1u << 29);
/// CPUID.1:ECX.OSXSAVE: the OS has enabled XGETBV, and XCR0 can be read.
inline constexpr uint32_t kLeaf1EcxOsxsave = 1u << 27;
/// CPUID.(7,0):EBX bits: BMI1 (3), AVX2 (5), BMI2 (8).
inline constexpr uint32_t kV3Leaf7Ebx = (1u << 3) | (1u << 5) | (1u << 8);
/// CPUID.80000001h:ECX bits: LAHF-SAHF in 64-bit mode (0), LZCNT (5).
inline constexpr uint32_t kV3Ext1Ecx = (1u << 0) | (1u << 5);
/// XCR0 bits: XMM state (1) and YMM state (2). AVX instructions are usable
/// only when the OS saves both across context switches; a CPU can report AVX
/// under an OS (or hypervisor) that does not.
inline constexpr uint64_t kV3Xcr0 = (1u << 1) | (1u << 2);
/// The first extended leaf, which carries LZCNT and LAHF-SAHF.
inline constexpr uint32_t kExtLeaf1 = 0x80000001u;

/// True when @p words show every x86-64-v3 feature and the OS state that AVX
/// needs.
[[nodiscard]] STRATA_X86_64_BASELINE constexpr bool
meets_x86_64_v3(const X86FeatureWords& words) noexcept {
    return words.max_leaf >= 7 && (words.leaf1_ecx & kV3Leaf1Ecx) == kV3Leaf1Ecx &&
           (words.leaf7_ebx & kV3Leaf7Ebx) == kV3Leaf7Ebx && words.max_ext_leaf >= kExtLeaf1 &&
           (words.ext1_ecx & kV3Ext1Ecx) == kV3Ext1Ecx && (words.xcr0 & kV3Xcr0) == kV3Xcr0;
}

#if defined(STRATA_X86_64)

namespace detail {

/// The four registers one CPUID leaf returns.
struct CpuidRegisters {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

/// CPUID with EAX=@p leaf and ECX=@p subleaf. The instruction exists on every
/// x86-64 CPU.
[[nodiscard]] STRATA_X86_64_BASELINE inline CpuidRegisters cpuid(uint32_t leaf,
                                                                 uint32_t subleaf) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    int regs[4];
    __cpuidex(regs, static_cast<int>(leaf), static_cast<int>(subleaf));
    return {static_cast<uint32_t>(regs[0]), static_cast<uint32_t>(regs[1]),
            static_cast<uint32_t>(regs[2]), static_cast<uint32_t>(regs[3])};
#else
    // Inline assembly rather than <cpuid.h>: clang-cl lacks that header's GNU
    // macros, and an intrinsic would bring its own target requirements.
    CpuidRegisters regs;
    __asm__ volatile("cpuid"
                     : "=a"(regs.eax), "=b"(regs.ebx), "=c"(regs.ecx), "=d"(regs.edx)
                     : "a"(leaf), "c"(subleaf));
    return regs;
#endif
}

/// XGETBV with ECX=0. Faults unless CPUID.1:ECX.OSXSAVE is set: call it only
/// then.
[[nodiscard]] STRATA_X86_64_BASELINE inline uint64_t xgetbv0() noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    return _xgetbv(0);
#else
    // Inline assembly: the `_xgetbv` intrinsic requires the `xsave` target
    // feature, which the baseline function deliberately does not have.
    uint32_t low;
    uint32_t high;
    __asm__ volatile("xgetbv" : "=a"(low), "=d"(high) : "c"(0u));
    return (static_cast<uint64_t>(high) << 32) | low;
#endif
}

} // namespace detail

/// Read the words @ref meets_x86_64_v3 decides from, querying each leaf only
/// where the CPU reports it.
[[nodiscard]] STRATA_X86_64_BASELINE inline X86FeatureWords read_x86_feature_words() noexcept {
    X86FeatureWords words{};
    words.max_leaf = detail::cpuid(0, 0).eax;
    if (words.max_leaf >= 1)
        words.leaf1_ecx = detail::cpuid(1, 0).ecx;
    if (words.max_leaf >= 7)
        words.leaf7_ebx = detail::cpuid(7, 0).ebx;
    words.max_ext_leaf = detail::cpuid(0x80000000u, 0).eax;
    if (words.max_ext_leaf >= kExtLeaf1)
        words.ext1_ecx = detail::cpuid(kExtLeaf1, 0).ecx;
    if ((words.leaf1_ecx & kLeaf1EcxOsxsave) != 0)
        words.xcr0 = detail::xgetbv0();
    return words;
}

/// True when this CPU, under this OS, runs x86-64-v3 code.
[[nodiscard]] STRATA_X86_64_BASELINE inline bool cpu_supports_x86_64_v3() noexcept {
    return meets_x86_64_v3(read_x86_feature_words());
}

#endif // STRATA_X86_64

} // namespace strata::util
