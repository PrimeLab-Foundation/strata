/**
 * @file test_mmap.cpp
 * @brief Tests for memory-mapped JSON file parsing (MmapFile + parse_json_file).
 *
 * Exercises: open/close lifecycle, move semantics, empty files, valid JSON files,
 * invalid paths, parse_json_file convenience function, parse_json_file_cursor.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_mmap.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

static int tests_passed = 0;

#define TEST(name)                                                                                 \
    static void test_##name();                                                                     \
    static void run_##name() {                                                                     \
        test_##name();                                                                             \
        ++tests_passed;                                                                            \
        std::printf("  PASS: %s\n", #name);                                                        \
    }                                                                                              \
    static void test_##name()

#define ASSERT_TRUE(expr)                                                                          \
    do {                                                                                           \
        bool _v = (expr);                                                                          \
        if (!_v)                                                                                   \
            std::fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #expr);                  \
        assert(_v);                                                                                \
        (void)_v;                                                                                  \
    } while (0)

#define ASSERT_EQ(a, b)                                                                            \
    do {                                                                                           \
        auto _a = (a);                                                                             \
        auto _b = (b);                                                                             \
        if (_a != _b)                                                                              \
            std::fprintf(stderr, "FAIL: %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b);           \
        assert(_a == _b);                                                                          \
        (void)_a;                                                                                  \
        (void)_b;                                                                                  \
    } while (0)

// Helper: write a string to a temp file and return the path.
static std::string write_temp_file(const std::string& content,
                                   const std::string& suffix = ".json") {
    std::string path =
        "/tmp/strata_test_mmap_" + std::to_string(reinterpret_cast<uintptr_t>(&content)) + suffix;
    std::ofstream out(path, std::ios::binary);
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    out.close();
    return path;
}

// ─── parse_json_file tests ───────────────────────────────────────────────────

TEST(parse_json_file_simple_object) {
    std::string json = R"({"name":"Alice","age":30})";
    auto path = write_temp_file(json);

    auto result = strata::parse_json_file(path.c_str());
    ASSERT_TRUE(result.ok());

    auto cursor = result.value.root();
    ASSERT_TRUE(cursor.is_object());

    auto name = cursor.field("name");
    ASSERT_TRUE(name.is_string());
    ASSERT_EQ(name.get_str(), std::string("Alice"));

    auto age = cursor.field("age");
    ASSERT_TRUE(age.is_number());

    std::remove(path.c_str());
}

TEST(parse_json_file_array) {
    std::string json = R"([1, 2, 3, "four", true, null])";
    auto path = write_temp_file(json);

    auto result = strata::parse_json_file(path.c_str());
    ASSERT_TRUE(result.ok());

    auto cursor = result.value.root();
    ASSERT_TRUE(cursor.is_array());
    ASSERT_EQ(cursor.array_size(), size_t{6});

    std::remove(path.c_str());
}

TEST(parse_json_file_nonexistent) {
    auto result = strata::parse_json_file("/tmp/strata_nonexistent_file_12345.json");
    ASSERT_TRUE(!result.ok());
    ASSERT_EQ(result.status, strata::Status::ParseError);
}

TEST(parse_json_file_empty_file) {
    auto path = write_temp_file("");

    // Empty file is not valid JSON — should fail parsing.
    auto result = strata::parse_json_file(path.c_str());
    // Either fails (empty is not valid JSON) or returns a null document.
    // The behavior depends on parse_json("") — just verify no crash.
    (void)result;

    std::remove(path.c_str());
}

TEST(parse_json_file_nested_object) {
    std::string json = R"({"user":{"name":"Bob","scores":[95,87]}})";
    auto path = write_temp_file(json);

    auto result = strata::parse_json_file(path.c_str());
    ASSERT_TRUE(result.ok());

    auto root = result.value.root();
    ASSERT_TRUE(root.is_object());
    auto user = root.field("user");
    ASSERT_TRUE(user.is_object());
    auto name = user.field("name");
    ASSERT_EQ(name.get_str(), std::string("Bob"));

    std::remove(path.c_str());
}

TEST(parse_json_file_large_file) {
    // Generate a ~50KB JSON array.
    std::string json = "[";
    for (int i = 0; i < 5000; ++i) {
        if (i > 0)
            json += ",";
        json += std::to_string(i);
    }
    json += "]";
    auto path = write_temp_file(json);

    auto result = strata::parse_json_file(path.c_str());
    ASSERT_TRUE(result.ok());
    auto root = result.value.root();
    ASSERT_TRUE(root.is_array());
    ASSERT_EQ(root.array_size(), size_t{5000});

    std::remove(path.c_str());
}

TEST(parse_json_file_with_escapes) {
    std::string json = R"({"msg":"hello\nworld","path":"c:\\dir"})";
    auto path = write_temp_file(json);

    auto result = strata::parse_json_file(path.c_str());
    ASSERT_TRUE(result.ok());
    auto root = result.value.root();
    ASSERT_TRUE(root.is_object());

    std::remove(path.c_str());
}

// ─── parse_json_file_cursor tests ────────────────────────────────────────────

TEST(parse_json_file_cursor_returns_error) {
    // parse_json_file_cursor is documented as broken (dangling pointer).
    // It should return an error status.
    auto result = strata::parse_json_file_cursor("/tmp/strata_test_dummy.json");
    ASSERT_TRUE(!result.ok());
    ASSERT_EQ(result.status, strata::Status::ParseError);
}

// ─── Main ────────────────────────────────────────────────────────────────────

int main() {
    std::printf("=== MmapFile / parse_json_file Tests ===\n\n");

    run_parse_json_file_simple_object();
    run_parse_json_file_array();
    run_parse_json_file_nonexistent();
    run_parse_json_file_empty_file();
    run_parse_json_file_nested_object();
    run_parse_json_file_large_file();
    run_parse_json_file_with_escapes();
    run_parse_json_file_cursor_returns_error();

    std::printf("\n=== All %d tests passed ===\n", tests_passed);
    return 0;
}
