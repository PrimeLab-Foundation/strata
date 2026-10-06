/**
 * @file test_cpu_features.cpp
 * @brief The x86-64-v3 check behind the release wheels' CPU guard
 *        (include/strata/util/cpu_features.hpp).
 *
 * The predicate is pure and runs on every platform: each feature the x86-64
 * psABI puts in x86-64-v2 or x86-64-v3 is spelled here a second time, by name
 * and CPUID position (Intel SDM vol. 2A, CPUID; AMD APM vol. 3, appendix E),
 * and clearing any one of them must fail the check. On x86-64 the reader runs
 * against the real CPU and, where the compiler runtime offers
 * `__builtin_cpu_supports`, each feature it can name is diffed against it.
 * The guard's failure branch itself is exercised by running an image under an
 * emulated pre-AVX2 CPU (Intel SDE `-nhm`), not here: this binary has to pass
 * on every CPU the source build supports.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/util/cpu_features.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <type_traits>

using strata::util::X86FeatureWords;

namespace {

/// Which word of @ref X86FeatureWords a feature bit lives in.
enum class Word { Leaf1Ecx, Leaf7Ebx, Ext1Ecx, Xcr0 };

struct Feature {
    const char* name;
    Word word;
    int bit;
};

// The psABI's x86-64-v2 and x86-64-v3 additions, plus the OS state AVX needs.
constexpr Feature kRequired[] = {
    {"SSE3", Word::Leaf1Ecx, 0},     {"SSSE3", Word::Leaf1Ecx, 9},
    {"FMA", Word::Leaf1Ecx, 12},     {"CMPXCHG16B", Word::Leaf1Ecx, 13},
    {"SSE4.1", Word::Leaf1Ecx, 19},  {"SSE4.2", Word::Leaf1Ecx, 20},
    {"MOVBE", Word::Leaf1Ecx, 22},   {"POPCNT", Word::Leaf1Ecx, 23},
    {"OSXSAVE", Word::Leaf1Ecx, 27}, {"AVX", Word::Leaf1Ecx, 28},
    {"F16C", Word::Leaf1Ecx, 29},    {"BMI1", Word::Leaf7Ebx, 3},
    {"AVX2", Word::Leaf7Ebx, 5},     {"BMI2", Word::Leaf7Ebx, 8},
    {"LAHF-SAHF", Word::Ext1Ecx, 0}, {"LZCNT", Word::Ext1Ecx, 5},
    {"XMM state", Word::Xcr0, 1},    {"YMM state", Word::Xcr0, 2},
};

/// A CPU with exactly the required features and leaves, nothing else.
constexpr X86FeatureWords kMinimalV3 = {7,
                                        strata::util::kV3Leaf1Ecx,
                                        strata::util::kV3Leaf7Ebx,
                                        strata::util::kExtLeaf1,
                                        strata::util::kV3Ext1Ecx,
                                        strata::util::kV3Xcr0};

static_assert(strata::util::meets_x86_64_v3(kMinimalV3));
static_assert(!strata::util::meets_x86_64_v3(X86FeatureWords{}));

X86FeatureWords with_bit(X86FeatureWords words, const Feature& feature, bool set) {
    auto apply = [&](auto& word) {
        using T = std::remove_reference_t<decltype(word)>;
        const T mask = static_cast<T>(T{1} << feature.bit);
        word = set ? static_cast<T>(word | mask) : static_cast<T>(word & ~mask);
    };
    switch (feature.word) {
    case Word::Leaf1Ecx:
        apply(words.leaf1_ecx);
        break;
    case Word::Leaf7Ebx:
        apply(words.leaf7_ebx);
        break;
    case Word::Ext1Ecx:
        apply(words.ext1_ecx);
        break;
    case Word::Xcr0:
        apply(words.xcr0);
        break;
    }
    return words;
}

/// The masks are exactly the named features: built up from nothing, the list
/// reproduces them bit for bit, so a typo on either side fails.
void test_masks_are_the_named_features() {
    X86FeatureWords built{};
    for (const Feature& feature : kRequired)
        built = with_bit(built, feature, true);
    assert(built.leaf1_ecx == strata::util::kV3Leaf1Ecx);
    assert(built.leaf7_ebx == strata::util::kV3Leaf7Ebx);
    assert(built.ext1_ecx == strata::util::kV3Ext1Ecx);
    assert(built.xcr0 == strata::util::kV3Xcr0);
    assert((strata::util::kV3Leaf1Ecx & strata::util::kLeaf1EcxOsxsave) != 0);
}

/// Every required feature is necessary: clearing any one fails the check,
/// from the minimal CPU and from one that reports every other bit too.
void test_each_feature_is_required() {
    X86FeatureWords everything = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                  0xFFFFFFFFu, 0xFFFFFFFFu, ~uint64_t{0}};
    assert(strata::util::meets_x86_64_v3(kMinimalV3));
    assert(strata::util::meets_x86_64_v3(everything));
    for (const Feature& feature : kRequired) {
        if (strata::util::meets_x86_64_v3(with_bit(kMinimalV3, feature, false)) ||
            strata::util::meets_x86_64_v3(with_bit(everything, feature, false))) {
            std::fprintf(stderr, "x86-64-v3 check passes without %s\n", feature.name);
            assert(false);
        }
    }
}

/// A leaf the CPU does not report cannot vouch for its features, whatever the
/// word holds: CPUID beyond the highest leaf returns unrelated data.
void test_unreported_leaves_fail() {
    X86FeatureWords words = kMinimalV3;
    words.max_leaf = 6;
    assert(!strata::util::meets_x86_64_v3(words));
    words = kMinimalV3;
    words.max_ext_leaf = 0x80000000u;
    assert(!strata::util::meets_x86_64_v3(words));
    words = kMinimalV3;
    words.max_leaf = 0x0000000Du; // a later CPU: more leaves are fine
    words.max_ext_leaf = 0x80000008u;
    assert(strata::util::meets_x86_64_v3(words));
}

#if defined(STRATA_X86_64)

bool bit(uint64_t word, int position) { return ((word >> position) & 1u) != 0; }

/// The reader against the real CPU: the decision is the predicate's over what
/// it read, XCR0 is read only when the OS allows it, and each feature the
/// compiler runtime can name agrees with the words.
void test_reader_on_this_cpu() {
    const X86FeatureWords words = strata::util::read_x86_feature_words();
    assert(words.max_leaf >= 1); // every x86-64 CPU reports leaf 1
    assert(strata::util::cpu_supports_x86_64_v3() == strata::util::meets_x86_64_v3(words));
    if (!bit(words.leaf1_ecx, 27))
        assert(words.xcr0 == 0);
    else
        assert(bit(words.xcr0, 0)); // x87 state is always enabled

#if (defined(__GNUC__) || defined(__clang__)) && !defined(_WIN32)
    // libgcc's and compiler-rt's own CPUID readers, as an independent oracle.
    // Both count AVX, AVX2 and FMA only when the OS saves YMM state.
    const bool os_avx = bit(words.leaf1_ecx, 27) && (words.xcr0 & 6u) == 6u;
    __builtin_cpu_init();
    assert(bit(words.leaf1_ecx, 0) == (__builtin_cpu_supports("sse3") != 0));
    assert(bit(words.leaf1_ecx, 9) == (__builtin_cpu_supports("ssse3") != 0));
    assert(bit(words.leaf1_ecx, 19) == (__builtin_cpu_supports("sse4.1") != 0));
    assert(bit(words.leaf1_ecx, 20) == (__builtin_cpu_supports("sse4.2") != 0));
    assert(bit(words.leaf1_ecx, 23) == (__builtin_cpu_supports("popcnt") != 0));
    assert((bit(words.leaf1_ecx, 28) && os_avx) == (__builtin_cpu_supports("avx") != 0));
    assert((bit(words.leaf1_ecx, 12) && os_avx) == (__builtin_cpu_supports("fma") != 0));
    assert((bit(words.leaf7_ebx, 5) && os_avx) == (__builtin_cpu_supports("avx2") != 0));
    assert(bit(words.leaf7_ebx, 3) == (__builtin_cpu_supports("bmi") != 0));
    assert(bit(words.leaf7_ebx, 8) == (__builtin_cpu_supports("bmi2") != 0));
#endif
    std::printf("cpu_features_tests: this CPU %s x86-64-v3\n",
                strata::util::cpu_supports_x86_64_v3() ? "runs" : "does not run");
}

#endif // STRATA_X86_64

} // namespace

int main() {
    test_masks_are_the_named_features();
    test_each_feature_is_required();
    test_unreported_leaves_fail();
#if defined(STRATA_X86_64)
    test_reader_on_this_cpu();
#endif
    std::puts("cpu_features_tests: OK");
    return 0;
}
