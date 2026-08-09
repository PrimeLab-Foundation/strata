/**
 * @file test_smoke.cpp
 * @brief Scaffolding smoke suite.
 *
 * Proves the C++20 toolchain compiles and that CMake/ctest — the single C++
 * test registry (docs/build-and-test/SKILL.md) — actually runs a binary and
 * reports its exit status. Real subsystem suites join it from M1 onward.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 * CMake compiles test targets with `-UNDEBUG` so these assertions stay live in
 * every build type.
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <vector>

static_assert(__cplusplus >= 202002L, "strata requires C++20");

/// A failing assert must fail the suite — verify assertions were not compiled out.
static void test_assertions_are_enabled() {
#ifdef NDEBUG
    // Reaching this means the test binary was built with assertions disabled,
    // which would silently turn every suite into a no-op.
    std::fputs("NDEBUG is defined: assertions are compiled out\n", stderr);
    std::abort();
#endif
    assert(true);
}

/// Minimal exercise of the C++20 library features the core is built on.
static void test_cxx20_toolchain() {
    constexpr std::string_view kName = "strata";
    assert(kName.size() == 6);
    assert(kName.starts_with("str"));

    std::vector<int> values{3, 1, 2};
    values.push_back(4);
    assert(values.size() == 4);
    assert(values.back() == 4);
}

int main() {
    test_assertions_are_enabled();
    test_cxx20_toolchain();
    std::puts("smoke_tests: OK");
    return 0;
}
