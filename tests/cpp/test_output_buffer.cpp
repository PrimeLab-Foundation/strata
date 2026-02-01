// C++ unit tests for OutputBuffer and SIMD string escaping (OutputBuffer path)

#include "strata/util/output_buffer.hpp"
#include "strata/util/simd_string.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

static std::string buffer_to_string(const strata::util::OutputBuffer& buf) {
    return std::string(buf.data(), buf.data() + buf.size());
}

static void test_output_buffer_append() {
    strata::util::OutputBuffer buf;
    buf.append("ab", 2);
    buf.push_back('c');

    assert(buf.size() == 3);
    assert(buffer_to_string(buf) == "abc");
}

static void test_output_buffer_reserve() {
    strata::util::OutputBuffer buf;
    buf.reserve(2048);
    assert(buf.capacity() >= 2048);
}

static void test_escape_clean_string() {
    strata::util::OutputBuffer buf;
    const char* text = "simple_ascii";
    bool copied = strata::util::try_copy_clean_string(text, std::strlen(text), buf);
    assert(copied);
    assert(buffer_to_string(buf) == "\"simple_ascii\"");
}

static void test_escape_with_quotes() {
    strata::util::OutputBuffer buf;
    const char* text = "he\"llo";
    strata::util::escape_json_string_simd(text, std::strlen(text), buf);
    assert(buffer_to_string(buf) == "\"he\\\"llo\"");
}

static void test_escape_control_chars() {
    strata::util::OutputBuffer buf;
    const char* text = "line1\nline2\tend";
    strata::util::escape_json_string_simd(text, std::strlen(text), buf);
    assert(buffer_to_string(buf) == "\"line1\\nline2\\tend\"");
}

int main() {
    test_output_buffer_append();
    test_output_buffer_reserve();
    test_escape_clean_string();
    test_escape_with_quotes();
    test_escape_control_chars();

    std::cout << "test_output_buffer: all assertions passed" << std::endl;
    return 0;
}
