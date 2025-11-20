#pragma once

#include <cstddef>
#include <cstdint>

#if defined(__APPLE__)
#include <mach/mach.h>
#endif

namespace strata {

/**
 * Platform-specific utilities for memory monitoring and optimization
 */

/**
 * Get peak resident set size (RSS) in bytes
 * Returns 0 if unable to determine
 */
inline size_t get_peak_rss_bytes() {
#if defined(__linux__)
    // Linux: read /proc/self/status
    FILE* fp = fopen("/proc/self/status", "r");
    if (!fp)
        return 0;

    size_t peak_rss = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmHWM:", 6) == 0) {
            // Format: "VmHWM:   123456 kB"
            char* p = line + 6;
            while (*p == ' ' || *p == '\t')
                ++p;
            peak_rss = strtoull(p, nullptr, 10) * 1024; // Convert kB to bytes
            break;
        }
    }
    fclose(fp);
    return peak_rss;

#elif defined(__APPLE__)
    // macOS: use mach_task_basic_info
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) ==
        KERN_SUCCESS) {
        return info.resident_size_max;
    }
    return 0;

#else
    // Unsupported platform
    return 0;
#endif
}

/**
 * Get current resident set size (RSS) in bytes
 */
inline size_t get_current_rss_bytes() {
#if defined(__linux__)
    FILE* fp = fopen("/proc/self/status", "r");
    if (!fp)
        return 0;

    size_t rss = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            char* p = line + 6;
            while (*p == ' ' || *p == '\t')
                ++p;
            rss = strtoull(p, nullptr, 10) * 1024;
            break;
        }
    }
    fclose(fp);
    return rss;

#elif defined(__APPLE__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) ==
        KERN_SUCCESS) {
        return info.resident_size;
    }
    return 0;

#else
    return 0;
#endif
}

/**
 * Hint to compiler for branch prediction
 */
#if defined(__GNUC__) || defined(__clang__)
#define TJ_LIKELY(x) __builtin_expect(!!(x), 1)
#define TJ_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define TJ_LIKELY(x) (x)
#define TJ_UNLIKELY(x) (x)
#endif

/**
 * Force inline
 */
#if defined(__GNUC__) || defined(__clang__)
#define TJ_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define TJ_ALWAYS_INLINE __forceinline
#else
#define TJ_ALWAYS_INLINE inline
#endif

/**
 * No inline
 */
#if defined(__GNUC__) || defined(__clang__)
#define TJ_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define TJ_NOINLINE __declspec(noinline)
#else
#define TJ_NOINLINE
#endif

/**
 * Cache line size for alignment
 */
constexpr size_t CACHE_LINE_SIZE = 64;

/**
 * Align to cache line
 */
#define TJ_CACHE_ALIGNED alignas(CACHE_LINE_SIZE)

} // namespace strata
