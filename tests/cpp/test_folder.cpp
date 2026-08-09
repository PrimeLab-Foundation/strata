/**
 * @file test_folder.cpp
 * @brief Contract tests for strata::util folder discovery and name rules.
 *
 * Added at M9: coverage showed folder.cpp at 0% from the C++ side. Its
 * behavior was exercised only through the Python folder tests, which breaks
 * the cross-layer parity rule (docs/context/convention.md, "Testing gates") —
 * a C++-side regression could only ever have been caught indirectly.
 *
 * Every rule pinned here is quoted from docs/context/api.md, "Folder mode".
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/util/folder.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using strata::util::Discovery;
using strata::util::DiscoveryStatus;

namespace {

/// A temporary directory that removes itself, so a failing assert cannot leave
/// litter behind that changes the next run's result.
class TempDir {
  public:
    TempDir() {
        static int counter = 0;
        root_ = fs::temp_directory_path() / ("strata_folder_test_" + std::to_string(++counter));
        fs::remove_all(root_);
        fs::create_directories(root_);
    }
    ~TempDir() {
        std::error_code ignored;
        fs::remove_all(root_, ignored);
    }
    TempDir(const TempDir&) = delete;
    TempDir& operator=(const TempDir&) = delete;

    [[nodiscard]] const fs::path& path() const noexcept { return root_; }
    [[nodiscard]] std::string str() const { return root_.string(); }

    void write(const std::string& relative, const std::string& contents = "{}") const {
        const fs::path target = root_ / relative;
        fs::create_directories(target.parent_path());
        std::ofstream out(target, std::ios::binary);
        out << contents;
    }

  private:
    fs::path root_;
};

/// Discovery returns absolute paths; compare on the part below the root.
[[nodiscard]] std::vector<std::string> relative_names(const Discovery& discovery,
                                                      const TempDir& dir) {
    std::vector<std::string> names;
    for (const std::string& file : discovery.files)
        names.push_back(fs::relative(file, dir.path()).generic_string());
    return names;
}

void test_discovery_finds_the_three_extensions() {
    TempDir dir;
    dir.write("a.json");
    dir.write("b.ndjson", "{}\n");
    dir.write("c.jsonl", "{}\n");
    dir.write("d.txt", "not json");
    dir.write("e.json.bak");
    dir.write("noextension");

    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    const std::vector<std::string> names = relative_names(discovery, dir);
    assert(names == (std::vector<std::string>{"a.json", "b.ndjson", "c.jsonl"}));
}

void test_extensions_match_case_insensitively() {
    TempDir dir;
    dir.write("upper.JSON");
    dir.write("mixed.NdJsOn", "{}\n");
    dir.write("shouty.JSONL", "{}\n");

    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    assert(discovery.files.size() == 3);
}

void test_discovery_is_recursive_and_bytewise_ordered() {
    TempDir dir;
    // Written in an order that is neither alphabetical nor depth-first, so a
    // walk that just returns iteration order would fail here.
    dir.write("zebra.json");
    dir.write("nested/deep/leaf.json");
    dir.write("alpha.json");
    dir.write("nested/beta.json");

    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    const std::vector<std::string> names = relative_names(discovery, dir);
    // Bytewise on the '/'-joined relative path: '/' (0x2f) sorts below every
    // letter, so "nested/beta.json" precedes "nested/deep/leaf.json".
    assert(names == (std::vector<std::string>{"alpha.json", "nested/beta.json",
                                              "nested/deep/leaf.json", "zebra.json"}));
}

void test_hidden_files_and_directories_are_pruned() {
    TempDir dir;
    dir.write("visible.json");
    dir.write(".hidden.json");
    dir.write(".hidden_dir/inside.json");
    dir.write(".hidden_dir/deeper/inside.json");
    dir.write("normal/.also_hidden.json");

    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    const std::vector<std::string> names = relative_names(discovery, dir);
    assert(names == (std::vector<std::string>{"visible.json"}));
}

void test_symlinks_are_not_followed() {
    TempDir dir;
    dir.write("real.json");
    dir.write("subdir/target.json");

    std::error_code ec;
    fs::create_symlink(dir.path() / "real.json", dir.path() / "link.json", ec);
    fs::create_directory_symlink(dir.path() / "subdir", dir.path() / "linkdir", ec);
    if (ec) {
        std::puts("  (skipped symlink checks: this filesystem refused to create one)");
        return;
    }

    // A self-referential link would hang a follow-the-links walk; discovery
    // must simply not descend.
    fs::create_directory_symlink(dir.path(), dir.path() / "loop", ec);

    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    const std::vector<std::string> names = relative_names(discovery, dir);
    assert(names == (std::vector<std::string>{"real.json", "subdir/target.json"}));
}

void test_empty_directory_discovers_nothing() {
    TempDir dir;
    const Discovery discovery = strata::util::discover_json_files(dir.str());
    assert(discovery.status == DiscoveryStatus::Ok);
    assert(discovery.files.empty());
}

void test_non_directories_are_reported_not_walked() {
    TempDir dir;
    dir.write("file.json");

    const Discovery on_file =
        strata::util::discover_json_files((dir.path() / "file.json").string());
    assert(on_file.status == DiscoveryStatus::NotADirectory);
    assert(!on_file.message.empty());

    const Discovery missing =
        strata::util::discover_json_files((dir.path() / "no_such_dir").string());
    assert(missing.status == DiscoveryStatus::NotADirectory);
    assert(!missing.message.empty());
}

void test_path_predicates() {
    TempDir dir;
    dir.write("file.json");

    assert(strata::util::path_exists(dir.str()));
    assert(strata::util::path_exists((dir.path() / "file.json").string()));
    assert(!strata::util::path_exists((dir.path() / "absent").string()));

    assert(strata::util::is_directory(dir.str()));
    assert(!strata::util::is_directory((dir.path() / "file.json").string()));
    assert(!strata::util::is_directory((dir.path() / "absent").string()));

    // Neither predicate may throw on a malformed path.
    assert(!strata::util::path_exists(""));
    assert(!strata::util::is_directory(""));
}

void test_safe_path_components() {
    assert(strata::util::is_safe_path_component("plain"));
    assert(strata::util::is_safe_path_component("with space"));
    assert(strata::util::is_safe_path_component("true"));
    assert(strata::util::is_safe_path_component("-42"));
    assert(strata::util::is_safe_path_component("...")); // only "." and ".." are special
    assert(strata::util::is_safe_path_component("a.b"));

    // The four documented escapes.
    assert(!strata::util::is_safe_path_component(""));
    assert(!strata::util::is_safe_path_component("."));
    assert(!strata::util::is_safe_path_component(".."));
    assert(!strata::util::is_safe_path_component("a/b"));
    assert(!strata::util::is_safe_path_component("a\\b"));
    assert(!strata::util::is_safe_path_component(std::string_view("a\0b", 3)));
    assert(!strata::util::is_safe_path_component("../escape"));
    assert(!strata::util::is_safe_path_component("/absolute"));
}

void test_make_directories() {
    TempDir dir;
    const std::string nested = (dir.path() / "one" / "two" / "three").string();
    assert(strata::util::make_directories(nested));
    assert(strata::util::is_directory(nested));

    // Idempotent: creating an existing directory succeeds.
    assert(strata::util::make_directories(nested));

    // An existing *file* is not a directory, and must not be reported as one.
    dir.write("blocker");
    assert(!strata::util::make_directories((dir.path() / "blocker").string()));
}

void test_ascii_lowercase() {
    assert(strata::util::ascii_lowercase("ABC") == "abc");
    assert(strata::util::ascii_lowercase("MiXeD 123") == "mixed 123");
    assert(strata::util::ascii_lowercase("") == "");
    // ASCII only: multi-byte UTF-8 passes through untouched, so case folding
    // never reinterprets a continuation byte. "CAF\xc3\x89" is "CAFÉ".
    const std::string folded = strata::util::ascii_lowercase("CAF\xc3\x89");
    assert(folded == "caf\xc3\x89");
    // The collision rule that uses it: two names differing only by ASCII case.
    assert(strata::util::ascii_lowercase("Group") == strata::util::ascii_lowercase("GROUP"));
}

} // namespace

int main() {
    test_discovery_finds_the_three_extensions();
    test_extensions_match_case_insensitively();
    test_discovery_is_recursive_and_bytewise_ordered();
    test_hidden_files_and_directories_are_pruned();
    test_symlinks_are_not_followed();
    test_empty_directory_discovers_nothing();
    test_non_directories_are_reported_not_walked();
    test_path_predicates();
    test_safe_path_components();
    test_make_directories();
    test_ascii_lowercase();

    std::puts("folder_tests: OK");
    return 0;
}
