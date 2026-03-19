/**
 * @file fuzz_bloom_filter.cpp
 * @brief libFuzzer harness for the Bloom filter subsystem.
 *
 * Splits fuzzer input into key-sized chunks, inserts them into a
 * BloomFilter, queries each, and exercises the DedupFilter with
 * random push/pop sequences derived from input bytes.
 */

#include "strata/bloom/bloom_filter.h"
#include "strata/bloom/dedup_filter.h"

#include <cstddef>
#include <cstdint>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 2) {
        return 0;
    }

    // --- Bloom filter: insert and query key-sized chunks ---
    strata::bloom::BloomFilter<256, 3> filter;

    size_t pos = 0;
    while (pos + 1 < size) {
        // Use first byte as key length (1-32).
        size_t key_len = (data[pos] % 32) + 1;
        pos++;
        if (pos + key_len > size)
            break;

        std::string_view key(reinterpret_cast<const char*>(data + pos), key_len);
        filter.insert(key);

        // Inserted key must be found (no false negatives).
        if (!filter.maybe_contains(key)) {
            __builtin_trap(); // False negative — bug.
        }
        pos += key_len;
    }

    // --- DedupFilter: random push/pop/insert sequence ---
    strata::bloom::DedupFilter dedup;

    for (size_t i = 0; i < size; ++i) {
        uint8_t cmd = data[i] % 4;
        switch (cmd) {
        case 0:
            dedup.push();
            break;
        case 1:
            dedup.pop();
            break;
        case 2:
        case 3: {
            // Insert a key derived from the byte.
            char key_buf[8];
            size_t key_len = (data[i] % 7) + 1;
            for (size_t j = 0; j < key_len && j < 8; ++j) {
                key_buf[j] = 'a' + (data[(i + j + 1) % size] % 26);
            }
            (void)dedup.check_and_insert(std::string_view(key_buf, key_len));
            break;
        }
        }
    }

    return 0;
}
