#pragma once

/**
 * @file python_cpu_guard.h
 * @brief The import-time CPU guard of an image built for x86-64-v3.
 *
 * Shared by both images' init entries, `PyInit__strata` (python_module.cpp)
 * and `PyInit__dumps_hook` (python_dumps_hook.cpp): each is imported on its
 * own (`import strata._dumps_hook` does not need `_strata` first), so each
 * checks for itself. Design: docs/architecture/release_pipeline.md, "CPU
 * guard".
 *
 * Active where the translation unit is compiled with AVX2 on x86-64:
 * `-march=x86-64-v3` (the Linux and macOS x86_64 wheels), `/arch:AVX2` (every
 * Windows x86-64 build), and `-march=native` on a host with AVX2. Everywhere
 * else -- arm64, a universal2 build's x86_64 slice, `STRATA_MARCH=none` or an
 * explicit pre-AVX2 target -- the macros below expand to nothing and no code
 * is emitted.
 *
 * What runs before the check must hold no x86-64-v3 instruction: the init
 * entry carries @ref STRATA_CPU_GUARD_ENTRY (the baseline instruction set)
 * and does nothing but call @ref cpu_guard_passes, which is compiled the same
 * way, as is everything it calls (strata/util/cpu_features.hpp). The module's
 * real initialisation is a separate function the entry calls only when the
 * check passes; it keeps the image's instruction set, since a compiler does
 * not inline a callee whose instruction set exceeds its caller's. Static
 * initializers would run earlier still, at load: the images have none
 * (docs/architecture/release_pipeline.md, "CPU guard", has the proof). The
 * check runs once per import, never on a call path.
 *
 * Under MSVC proper (cl.exe, not a release compiler) there is no per-function
 * instruction set: the check still runs first, but the compiler may use AVX2
 * in it, so there the guard is best effort and unverified.
 */

#include "python_types.h"
#include "strata/util/cpu_features.hpp"

#if defined(STRATA_X86_64) && defined(__AVX2__)
/// 1 when the init entries check the CPU before anything else runs.
#define STRATA_CPU_GUARD 1
/// Placed between `PyMODINIT_FUNC` and the init entry's name: compiles the
/// entry for the x86-64 baseline (a GNU attribute there applies to the
/// function; before `PyMODINIT_FUNC` it would precede `extern "C"`).
#define STRATA_CPU_GUARD_ENTRY STRATA_X86_64_BASELINE
#else
#define STRATA_CPU_GUARD 0
#define STRATA_CPU_GUARD_ENTRY
#endif

#if STRATA_CPU_GUARD

namespace strata::bindings {

/// The way out, per platform. setup.py passes `/arch:AVX2` to every Windows
/// x86-64 build, so a source build does not help there; elsewhere the sdist
/// builds `-march=native`. On macOS the usual such machine is Rosetta 2, whose
/// CPUID omits AVX unless ROSETTA_ADVERTISE_AVX=1 is set, even where it
/// translates AVX2 (macOS 26 measured: unset it reports SSE4.2 at most).
#if defined(_WIN32)
#define STRATA_CPU_GUARD_REMEDY                                                                    \
    "Every Windows x86-64 build of strata targets AVX2, so a source build does not help on this "  \
    "machine."
#elif defined(__APPLE__)
#define STRATA_CPU_GUARD_REMEDY                                                                    \
    "Under Rosetta 2 on Apple silicon, install strata for an arm64 Python instead, or set "        \
    "ROSETTA_ADVERTISE_AVX=1 if this macOS's Rosetta runs AVX2. Otherwise build strata for this "  \
    "CPU from source: pip install --no-binary strata-plf strata-plf"
#else
#define STRATA_CPU_GUARD_REMEDY                                                                    \
    "Build strata for this CPU from source instead: pip install --no-binary strata-plf strata-plf"
#endif

/**
 * True when this CPU runs x86-64-v3 code; otherwise set ImportError naming
 * @p module, the requirement and the way out, and return false.
 *
 * Not `noexcept`: the compiler cannot see that `PyErr_Format` never throws,
 * and the terminate handler it would add is a function compiled with the
 * image's instruction set, reachable from the entry before the check passes.
 */
[[nodiscard]] STRATA_X86_64_BASELINE inline bool cpu_guard_passes(const char* module) {
    if (util::cpu_supports_x86_64_v3()) [[likely]]
        return true;
    PyErr_Format(PyExc_ImportError,
                 "%s was built for x86-64-v3 (an AVX2 CPU: AVX, AVX2, BMI1, BMI2, F16C, FMA, "
                 "LZCNT and MOVBE, with the OS saving AVX state), and this machine does not "
                 "provide it. " STRATA_CPU_GUARD_REMEDY,
                 module);
    return false;
}

#undef STRATA_CPU_GUARD_REMEDY

} // namespace strata::bindings

#endif // STRATA_CPU_GUARD
