/**
 * @file test_fuzz_corpus.cpp
 * @brief Replay the committed fuzz corpus through the engine.
 *
 * The corpus exists so libFuzzer starts from meaningful inputs instead of
 * random bytes — the previous implementation documented one and never
 * committed it, so its fuzz runs failed at startup every time
 * (docs/build-and-test/SKILL.md).
 *
 * Replaying it here does two jobs a corpus directory alone cannot:
 *
 * 1. It keeps the corpus honest. A seed that stops parsing, or a directory
 *    that goes missing, fails `make test` rather than being noticed months
 *    later by a scheduled job.
 * 2. It gives the corpus value on toolchains with no libFuzzer runtime —
 *    Apple's clang, for one. Built with `-fsanitize=address,undefined` this
 *    suite is a real memory-safety check over every seed.
 *
 * Every seed must be *handled*: parsed or cleanly rejected, never a crash and
 * never a hang. Which of the two is expected is encoded in the file name, so a
 * seed that silently changes meaning is caught.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"
#include "strata/json/ndjson_stream.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

/// ctest runs suites from the source directory, so these resolve.
constexpr const char* kLoadsCorpus = "tests/fuzz/corpus/loads";
constexpr const char* kNdjsonCorpus = "tests/fuzz/corpus/ndjson";

[[nodiscard]] std::string read_file(const fs::path& path) {
    std::ifstream input(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
}

/// Seeds whose name begins with `bad_` must be rejected; the rest must parse.
[[nodiscard]] bool expects_rejection(const std::string& name) {
    return name.rfind("bad_", 0) == 0 || name == "empty";
}

[[nodiscard]] std::vector<fs::path> corpus_files(const char* directory) {
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file())
            files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    return files;
}

void test_the_corpus_is_present() {
    // A vacuous pass here would defeat the whole point of committing it.
    assert(fs::is_directory(kLoadsCorpus));
    assert(fs::is_directory(kNdjsonCorpus));
    assert(corpus_files(kLoadsCorpus).size() >= 30);
    assert(corpus_files(kNdjsonCorpus).size() >= 10);
}

void test_every_loads_seed_is_handled() {
    for (const fs::path& path : corpus_files(kLoadsCorpus)) {
        const std::string data = read_file(path);
        const auto parsed = strata::parse_json(data);

        const std::string name = path.filename().string();
        if (expects_rejection(name)) {
            if (parsed.ok()) {
                std::printf("seed %s was accepted but its name says otherwise\n", name.c_str());
                assert(false);
            }
            continue;
        }
        if (!parsed.ok()) {
            std::printf("seed %s no longer parses\n", name.c_str());
            assert(false);
        }

        // Round-trip what parsed, so the serializer sees the corpus too.
        const std::string text = strata::serialize_json(parsed.value);
        assert(strata::parse_json(text).ok());
    }
}

void test_every_ndjson_seed_is_handled() {
    for (const fs::path& path : corpus_files(kNdjsonCorpus)) {
        const std::string data = read_file(path);

        // Line at a time, to the documented end-of-stream marker.
        strata::NdjsonStream stream(data);
        for (;;) {
            const auto record = stream.next();
            if (record.status == strata::Status::KeyNotFound)
                break;
        }

        // Both sides of the skip_errors contract.
        strata::NdjsonStream lenient(data);
        const auto all = lenient.parse_all(/*skip_errors=*/true);
        assert(all.ok());

        strata::NdjsonStream strict(data);
        size_t failed_line = 0;
        const auto stopped = strict.parse_all(/*skip_errors=*/false, &failed_line);
        const std::string name = path.filename().string();
        if (expects_rejection(name) && name != "empty") {
            assert(!stopped.ok());
            assert(failed_line > 0); // the caller is told which line
        }
    }
}

/// Truncating a seed anywhere must still be handled, never crash.
void test_truncated_seeds_are_handled() {
    for (const fs::path& path : corpus_files(kLoadsCorpus)) {
        const std::string data = read_file(path);
        for (size_t length = 0; length < data.size(); ++length) {
            const auto parsed = strata::parse_json(std::string_view(data.data(), length));
            if (parsed.ok())
                (void)strata::serialize_json(parsed.value);
        }
    }
}

/// So must flipping any single byte.
void test_single_byte_mutations_are_handled() {
    for (const fs::path& path : corpus_files(kLoadsCorpus)) {
        std::string data = read_file(path);
        if (data.size() > 64)
            continue; // the long seeds are covered by truncation above
        for (size_t index = 0; index < data.size(); ++index) {
            const char original = data[index];
            for (const char replacement : {'\0', '"', '\\', '{', '[', ',', ':', '9', '\x80'}) {
                data[index] = replacement;
                const auto parsed = strata::parse_json(data);
                if (parsed.ok())
                    (void)strata::serialize_json(parsed.value);
            }
            data[index] = original;
        }
    }
}

} // namespace

int main() {
    test_the_corpus_is_present();
    test_every_loads_seed_is_handled();
    test_every_ndjson_seed_is_handled();
    test_truncated_seeds_are_handled();
    test_single_byte_mutations_are_handled();

    std::puts("fuzz_corpus_tests: OK");
    return 0;
}
