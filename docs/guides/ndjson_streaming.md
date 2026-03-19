# NDJSON Streaming Guide

NDJSON (Newline Delimited JSON) is a format where each line is a complete, valid JSON object. Strata provides `NdjsonStream` for efficient line-by-line processing with SIMD-accelerated line splitting, speculative parsing with online learning, and optional selective key filtering.

______________________________________________________________________

## NdjsonStream API Overview

```cpp
#include <strata/json/ndjson_stream.hpp>
```

`NdjsonStream` operates on a `std::string_view` of the entire NDJSON data. It does not copy the data -- the caller must ensure the underlying buffer outlives the stream.

### Construction

```cpp
strata::NdjsonStream stream(ndjson_data);
```

- `ndjson_data` is a `std::string_view` pointing to the NDJSON content.

______________________________________________________________________

## Line-by-Line Processing

Use `next()` for one-at-a-time processing:

```cpp
#include <strata/json/ndjson_stream.hpp>
#include <cstdio>

int main() {
    std::string_view data = R"({"name":"Alice","age":30}
{"name":"Bob","age":25}
{"name":"Charlie","age":35})";

    strata::NdjsonStream stream(data);

    while (stream.has_next()) {
        auto result = stream.next();
        if (result.ok()) {
            // Process result.value() -- a JsonValue
            std::printf("Line %zu parsed successfully\n", stream.line_number());
        } else {
            std::printf("Line %zu: parse error\n", stream.line_number());
        }
    }

    std::printf("Total lines: %zu, Errors: %zu\n",
                stream.lines_processed(), stream.error_count());
    return 0;
}
```

### Status Codes

- `Status::Ok` -- Line parsed successfully.
- `Status::ParseError` -- Line contains invalid JSON.
- `Status::KeyNotFound` -- No more lines (end of stream).

______________________________________________________________________

## Batch Processing

### parse_all

Parse all lines into a vector:

```cpp
auto results = stream.parse_all(/*skip_errors=*/false);
// skip_errors=false: stop on first error
// skip_errors=true:  skip malformed lines, continue parsing
```

### parse_all_fast

SIMD-accelerated batch mode with precise pre-allocation. Uses SIMD line counting to determine the exact number of lines before allocating:

```cpp
auto results = stream.parse_all_fast(/*skip_errors=*/true);
```

This is faster than `parse_all()` for large datasets because it avoids vector reallocation during parsing.

### next_batch

Process a fixed number of lines at a time. Useful for reducing Python/C++ boundary crossings or processing in chunks:

```cpp
while (stream.has_next()) {
    auto batch = stream.next_batch(1000, /*skip_errors=*/true);
    // Process up to 1000 records per batch
    for (const auto& doc : batch) {
        // ...
    }
}
```

______________________________________________________________________

## Speculative Parsing with Online Learning

`NdjsonStream` internally uses the speculative parsing engine. A `TransitionModel` persists across lines, so the Markov model learns the structural patterns of your data as it processes more records. This means prediction accuracy improves over time.

### How Learning Improves Across Lines

Consider an NDJSON file where every record has the same schema:

```json
{"id": 1, "name": "Alice", "active": true, "score": 95.5}
{"id": 2, "name": "Bob", "active": false, "score": 87.2}
...
```

The speculative model observes:

- After key `"id"`: always INTEGER
- After key `"name"`: always SHORT_STRING
- After key `"active"`: always BOOL_TRUE or BOOL_FALSE
- After key `"score"`: always FLOAT

After the warmup period (default: 100 values), the model reaches high confidence and begins speculating. By the second or third record, it is already predicting most value types correctly.

### Monitoring Prediction Accuracy

Access the speculative model's statistics to verify that learning is effective:

```cpp
strata::NdjsonStream stream(data);
auto results = stream.parse_all_fast(true);

auto stats = stream.speculative_model().stats();
std::printf("Total predictions:  %llu\n", stats.total_predictions);
std::printf("Correct predictions: %llu\n", stats.correct_predictions);
std::printf("Hit rate:           %.1f%%\n", stats.hit_rate() * 100);
std::printf("Speculation rate:   %.1f%%\n", stats.speculation_rate() * 100);
```

Typical hit rates for homogeneous NDJSON (same schema on every line) reach 95-99% after the warmup period.

### Warmup Behavior

During warmup, the parser uses the generic (non-speculative) path for all values while the model accumulates observations. The default warmup is 100 values (configurable via `SpeculativeParser::Config::online_learning_warmup`). For a record with 10 fields, warmup completes after approximately 10 lines.

______________________________________________________________________

## Selective Parsing in NDJSON

Combine `NdjsonStream` with `KeyFilter` to parse only desired fields from each record:

```cpp
#include <strata/json/ndjson_stream.hpp>
#include <strata/bloom/key_filter.h>
#include <memory>

int main() {
    // Assume each line has: id, name, email, department, salary, ...
    std::string_view data = /* large NDJSON data */;

    strata::NdjsonStream stream(data);

    // Only parse "name" and "salary"
    std::array<std::string_view, 2> desired = {"name", "salary"};
    stream.set_key_filter(
        std::make_unique<strata::bloom::KeyFilter>(desired));

    auto results = stream.parse_all_fast(true);
    // Each result contains only {"name": "...", "salary": N}

    return 0;
}
```

When a key filter is set, the speculative parser skips values for non-matching keys using structural index jumps. The combination of speculation + selective parsing provides both prediction-based fast-path dispatch for wanted values and zero-cost skipping for unwanted values.

______________________________________________________________________

## Raw Line Access

For advanced use cases (e.g., when the Python bindings call `parse_sax` directly), you can read raw lines:

```cpp
while (stream.has_next()) {
    std::string_view line = stream.read_raw_line();
    if (line.empty()) break;

    // Process the raw line yourself
    // Call stream.record_error() if your external parsing fails
}
```

`read_raw_line()` returns a `string_view` into the original data buffer. It skips blank and whitespace-only lines and increments the internal `lines_processed_` counter.

______________________________________________________________________

## Error Handling Strategies

### Strict Mode (stop on first error)

```cpp
auto results = stream.parse_all(/*skip_errors=*/false);
if (stream.error_count() > 0) {
    std::fprintf(stderr, "Parse error at line %zu\n", stream.line_number());
}
```

### Lenient Mode (skip bad lines)

```cpp
auto results = stream.parse_all(/*skip_errors=*/true);
std::printf("Parsed %zu records, skipped %zu errors\n",
            results.size(), stream.error_count());
```

### Batch Mode with Per-batch Error Handling

```cpp
size_t total_ok = 0;
while (stream.has_next()) {
    auto batch = stream.next_batch(5000, /*skip_errors=*/true);
    total_ok += batch.size();
    // Process batch...
}
std::printf("Total OK: %zu, Errors: %zu\n", total_ok, stream.error_count());
```

______________________________________________________________________

## Performance Considerations

### parse_all vs. parse_all_fast

| Method           | Pre-allocation    | Line Splitting   | Best For                     |
| ---------------- | ----------------- | ---------------- | ---------------------------- |
| `parse_all`      | Grows dynamically | Sequential scan  | Small files, unknown format  |
| `parse_all_fast` | SIMD line count   | SIMD-accelerated | Large files, known good data |

`parse_all_fast` counts newlines with SIMD before allocating the result vector, avoiding reallocation overhead. For large NDJSON files (millions of lines), this can be significantly faster.

### Batch Size Selection

For `next_batch()`, the batch size controls the tradeoff between per-call overhead and memory usage:

- **Small batches (100-1000):** Lower peak memory, more function call overhead.
- **Large batches (10000+):** Higher peak memory, better amortization of per-call costs.
- **Recommended:** Start with 1000-5000 and tune based on your memory constraints.

### Memory Efficiency

`NdjsonStream` does not copy the input data. The `string_view` constructor means the original buffer must remain valid for the lifetime of the stream. For file-backed data, consider memory-mapping the file to avoid loading the entire contents into a `std::string`.

______________________________________________________________________

## Complete Example: NDJSON Analytics Pipeline

```cpp
#include <strata/json/ndjson_stream.hpp>
#include <strata/bloom/key_filter.h>
#include <cstdio>
#include <memory>

int main() {
    // Simulated NDJSON log data
    std::string_view logs = R"({"ts":"2025-01-15T10:00:00Z","level":"INFO","msg":"started","latency_ms":0}
{"ts":"2025-01-15T10:00:01Z","level":"WARN","msg":"slow query","latency_ms":450}
{"ts":"2025-01-15T10:00:02Z","level":"ERROR","msg":"timeout","latency_ms":5000}
{"ts":"2025-01-15T10:00:03Z","level":"INFO","msg":"recovered","latency_ms":12})";

    // Only extract level and latency
    strata::NdjsonStream stream(logs);
    std::array<std::string_view, 2> desired = {"level", "latency_ms"};
    stream.set_key_filter(
        std::make_unique<strata::bloom::KeyFilter>(desired));

    auto records = stream.parse_all_fast(true);

    // Compute average latency
    double total_latency = 0;
    for (const auto& rec : records) {
        strata::JsonCursor cursor(&rec);
        total_latency += cursor.field("latency_ms").get_float();
    }

    std::printf("Records: %zu\n", records.size());
    std::printf("Avg latency: %.1f ms\n", total_latency / records.size());

    // Check speculation effectiveness
    auto model_stats = stream.speculative_model().stats();
    std::printf("Prediction hit rate: %.1f%%\n", model_stats.hit_rate() * 100);

    return 0;
}
```
