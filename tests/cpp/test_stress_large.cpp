/**
 * test_stress_large.cpp - Stress tests for large JSON payloads.
 *
 * Mirrors Python tests/py/test_stress_large.py (large parse/serialize round-trip).
 */

#include "strata/json/json_parse.hpp"
#include "strata/json/json_serialize.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace strata;

void test_large_parse_roundtrip_5mb() {
    // Build ~5MB JSON: array of objects [{"id":0,"data":"xxx"}, ...]
    const size_t target_bytes = 5 * 1024 * 1024;
    const std::string chunk(80, 'x');
    std::ostringstream oss;
    oss << "[";
    size_t count = 0;
    while (oss.str().size() < target_bytes) {
        if (count > 0)
            oss << ",";
        oss << "{\"id\":" << count << ",\"data\":\"" << chunk << "\"}";
        ++count;
    }
    oss << "]";
    std::string json_str = oss.str();
    assert(json_str.size() >= 4 * 1024 * 1024);

    auto result = parse_json(json_str);
    assert(result.ok());
    const auto& arr = result.value.as_array();
    assert(arr.size() == count);
    assert(arr[0].as_object().at("id").as_number() == 0.0);

    std::string back = serialize_json(result.value);
    auto result2 = parse_json(back);
    assert(result2.ok());
    assert(result2.value.as_array().size() == count);

    std::cout << "✓ test_large_parse_roundtrip_5mb passed (" << (json_str.size() / 1024 / 1024)
              << " MB)\n";
}

int main() {
    std::cout << "Running large-payload stress tests...\n\n";
    test_large_parse_roundtrip_5mb();
    std::cout << "\n✅ All large-payload stress tests passed!\n";
    return 0;
}
