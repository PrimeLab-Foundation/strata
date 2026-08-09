#pragma once

/**
 * @file folder.hpp
 * @brief Directory discovery and split-name rules for folder mode.
 *
 * Folder mode is new in the target API — there is no previous implementation to
 * follow here, so every rule below comes from docs/context/api.md and is
 * pinned by test.
 *
 * Discovery is shared by `load` and `search`, which is what makes the folder
 * search law (`search(dir) == concat of search(file)`) hold: both walk the same
 * list in the same order.
 */

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace strata::util {

/// Why a directory walk failed, when it did.
enum class DiscoveryStatus { Ok, NotADirectory, WalkFailed };

struct Discovery {
    DiscoveryStatus status = DiscoveryStatus::Ok;
    /// Absolute paths, ordered bytewise by their '/'-joined relative path.
    std::vector<std::string> files;
    /// The failure detail, when status is not Ok.
    std::string message;
};

/**
 * Every `.json`, `.ndjson` and `.jsonl` file under @p directory.
 *
 * Recursive. Extensions match case-insensitively. Hidden files *and* hidden
 * directories are pruned, so nothing under a dot-directory is discovered.
 * Symlinks are not followed — neither symlinked files nor symlinked
 * directories — which also means the walk cannot loop.
 *
 * Ordering is bytewise on the relative path with '/' separators, so it is the
 * same on every platform and does not depend on directory iteration order.
 */
[[nodiscard]] Discovery discover_json_files(std::string_view directory);

/// Whether @p path names anything at all.
[[nodiscard]] bool path_exists(std::string_view path) noexcept;

/// Whether @p path names an existing directory.
[[nodiscard]] bool is_directory(std::string_view path) noexcept;

/**
 * Whether @p name is safe to use as a single path component.
 *
 * Rejects the empty string, `.` and `..`, and anything containing `/`, `\` or
 * a NUL — the four ways a group name could escape the directory it is meant to
 * live in.
 */
[[nodiscard]] bool is_safe_path_component(std::string_view name) noexcept;

/// Create @p path and any missing parents. True if it exists afterwards.
[[nodiscard]] bool make_directories(std::string_view path) noexcept;

/// Lowercase ASCII, for detecting names that differ only by case.
[[nodiscard]] std::string ascii_lowercase(std::string_view text);

} // namespace strata::util
