#pragma once

#include <array>
#include <cstdio>
#include <cstring>
#include <string>

#ifdef __APPLE__
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#ifdef __linux__
#include <fstream>
#endif

namespace bench {

inline std::string get_os() {
#if defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(_WIN32)
    return "Windows";
#else
    return "Unknown";
#endif
}

inline std::string get_arch() {
#if defined(__aarch64__) || defined(_M_ARM64)
    return "ARM64";
#elif defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86";
#else
    return "Unknown";
#endif
}

inline std::string get_compiler() {
    char buf[128];
#if defined(__clang__)
    std::snprintf(buf, sizeof(buf), "clang %d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    std::snprintf(buf, sizeof(buf), "gcc %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    std::snprintf(buf, sizeof(buf), "MSVC %d", _MSC_VER);
#else
    std::snprintf(buf, sizeof(buf), "unknown");
#endif
    return buf;
}

inline std::string get_simd() {
#if defined(__AVX2__)
    return "AVX2";
#elif defined(__SSE2__)
    return "SSE2";
#elif defined(__ARM_NEON)
    return "NEON";
#else
    return "scalar";
#endif
}

inline std::string get_cpu() {
#ifdef __APPLE__
    char buf[256] = {};
    size_t len = sizeof(buf);
    if (sysctlbyname("machdep.cpu.brand_string", buf, &len, nullptr, 0) == 0)
        return buf;
#endif

#ifdef __linux__
    std::ifstream f("/proc/cpuinfo");
    std::string line;
    while (std::getline(f, line)) {
        if (line.find("model name") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                auto name = line.substr(pos + 1);
                while (!name.empty() && name[0] == ' ') name.erase(0, 1);
                return name;
            }
        }
    }
#endif

    return "unknown";
}

inline void print_platform() {
    printf("  platform:\n");
    printf("    cpu:       %s\n", get_cpu().c_str());
    printf("    os:        %s\n", get_os().c_str());
    printf("    arch:      %s\n", get_arch().c_str());
    printf("    compiler:  %s\n", get_compiler().c_str());
    printf("    simd:      %s\n", get_simd().c_str());
    printf("\n");
}

}