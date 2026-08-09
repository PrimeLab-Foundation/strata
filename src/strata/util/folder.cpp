/**
 * @file folder.cpp
 * @brief Directory discovery and split-name rules.
 */

#include "strata/util/folder.hpp"

#include <algorithm>
#include <filesystem>
#include <system_error>

namespace strata::util {

namespace {

namespace fs = std::filesystem;

[[nodiscard]] bool is_hidden(const std::string& name) noexcept {
    return !name.empty() && name.front() == '.';
}

/// True for `.json`, `.ndjson` and `.jsonl`, whatever the case.
[[nodiscard]] bool has_json_extension(const std::string& name) {
    const size_t dot = name.rfind('.');
    if (dot == std::string::npos)
        return false;
    const std::string extension = ascii_lowercase(std::string_view(name).substr(dot));
    return extension == ".json" || extension == ".ndjson" || extension == ".jsonl";
}

/// Walk @p directory, collecting (relative, absolute) pairs.
void walk(const fs::path& root, const fs::path& current, const std::string& prefix,
          std::vector<std::pair<std::string, std::string>>& found, std::error_code& failure) {
    std::error_code error;
    fs::directory_iterator entries(current, fs::directory_options::none, error);
    if (error) {
        failure = error;
        return;
    }

    // Collect first, then sort: directory iteration order is not defined, and
    // the documented ordering must not depend on it.
    std::vector<fs::directory_entry> children;
    for (const fs::directory_entry& entry : entries)
        children.push_back(entry);

    for (const fs::directory_entry& entry : children) {
        const std::string name = entry.path().filename().string();
        if (is_hidden(name))
            continue; // hidden files and hidden directories are both pruned

        // is_symlink() is the non-following query, so a symlinked file or
        // directory is skipped rather than traversed. That also makes a
        // symlink loop impossible.
        std::error_code link_error;
        if (entry.is_symlink(link_error) || link_error)
            continue;

        std::error_code kind_error;
        const std::string relative = prefix.empty() ? name : prefix + "/" + name;
        if (entry.is_directory(kind_error) && !kind_error) {
            walk(root, entry.path(), relative, found, failure);
            if (failure)
                return;
        } else if (entry.is_regular_file(kind_error) && !kind_error && has_json_extension(name)) {
            found.emplace_back(relative, entry.path().string());
        }
    }
}

} // namespace

bool path_exists(std::string_view path) noexcept {
    std::error_code error;
    return fs::exists(fs::path{std::string(path)}, error) && !error;
}

bool is_directory(std::string_view path) noexcept {
    std::error_code error;
    return fs::is_directory(fs::path(std::string(path)), error) && !error;
}

Discovery discover_json_files(std::string_view directory) {
    Discovery result;
    const fs::path root{std::string(directory)};

    std::error_code error;
    if (!fs::is_directory(root, error) || error) {
        result.status = DiscoveryStatus::NotADirectory;
        result.message = std::string(directory);
        return result;
    }

    std::vector<std::pair<std::string, std::string>> found;
    std::error_code failure;
    walk(root, root, "", found, failure);
    if (failure) {
        result.status = DiscoveryStatus::WalkFailed;
        result.message = failure.message();
        return result;
    }

    // Bytewise on the relative path: stable across platforms and filesystems.
    std::sort(found.begin(), found.end(),
              [](const auto& left, const auto& right) { return left.first < right.first; });

    result.files.reserve(found.size());
    for (auto& [relative, absolute] : found) {
        (void)relative;
        result.files.push_back(std::move(absolute));
    }
    return result;
}

bool make_directories(std::string_view path) noexcept {
    std::error_code error;
    const fs::path target{std::string(path)};
    fs::create_directories(target, error);
    // create_directories reports "false, no error" when the directory already
    // existed, which is a success for our purposes.
    return fs::is_directory(target, error) && !error;
}

bool is_safe_path_component(std::string_view name) noexcept {
    if (name.empty() || name == "." || name == "..")
        return false;
    for (const char c : name) {
        if (c == '/' || c == '\\' || c == '\0')
            return false;
    }
    return true;
}

std::string ascii_lowercase(std::string_view text) {
    std::string out(text);
    for (char& c : out) {
        if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
    }
    return out;
}

} // namespace strata::util
