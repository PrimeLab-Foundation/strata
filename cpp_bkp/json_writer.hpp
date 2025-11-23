/**
 * json_writer.hpp - Compact JSON writer with reusable buffers (PR #4)
 *
 * SmallJsonWriter provides a per-worker buffer for efficient JSON materialization
 * with minimal allocations. The buffer grows geometrically up to a hard cap.
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

/**
 * SmallJsonWriter - Compact JSON writer with reusable buffer
 *
 * Provides a growable buffer for JSON output with the following features:
 * - Initial capacity set by configuration
 * - Geometric growth (2x) up to hard cap
 * - Reset between uses to reuse buffer
 * - Direct simdjson integration
 *
 * Usage:
 *   SmallJsonWriter writer(256 * 1024, 8 * 1024 * 1024);
 *   writer.reset_segment();
 *   char* ptr = writer.reserve(100);
 *   // Write data to ptr
 *   writer.commit(actual_size);
 *   std::string_view result = writer.finish_segment();
 */
class SmallJsonWriter {
  public:
    /**
     * Constructor
     *
     * @param initial_capacity Initial buffer size in bytes
     * @param max_capacity Maximum buffer size in bytes
     */
    explicit SmallJsonWriter(size_t initial_capacity = 256 * 1024,
                             size_t max_capacity = 8 * 1024 * 1024)
        : max_cap_(max_capacity), segment_start_(0), segment_end_(0) {
        buffer_.reserve(initial_capacity);
    }

    /**
     * Reset segment for next write
     *
     * Marks the start of a new segment. Buffer is not cleared, just position reset.
     */
    void reset_segment() {
        segment_start_ = 0;
        segment_end_ = 0;
        buffer_.clear();
    }

    /**
     * Reserve space for writing
     *
     * @param n Number of bytes to reserve
     * @return Pointer to reserved space
     * @throws std::runtime_error if reservation exceeds max capacity
     */
    char* reserve(size_t n) {
        size_t required = segment_end_ + n;

        if (required > max_cap_) {
            throw std::runtime_error("SmallJsonWriter: requested size " + std::to_string(required) +
                                     " exceeds max capacity " + std::to_string(max_cap_));
        }

        // Grow geometrically if needed
        if (required > buffer_.capacity()) {
            size_t new_cap = buffer_.capacity() * 2;
            if (new_cap == 0)
                new_cap = 4096;
            while (new_cap < required) {
                new_cap *= 2;
            }
            if (new_cap > max_cap_) {
                new_cap = max_cap_;
            }
            buffer_.reserve(new_cap);
        }

        // Resize buffer to accommodate
        if (required > buffer_.size()) {
            buffer_.resize(required);
        }

        return buffer_.data() + segment_end_;
    }

    /**
     * Commit written bytes
     *
     * @param n Number of bytes actually written (must be <= last reserve())
     */
    void commit(size_t n) {
        segment_end_ += n;
        if (segment_end_ > buffer_.size()) {
            buffer_.resize(segment_end_);
        }
    }

    /**
     * Finish segment and return view
     *
     * @return string_view of the written segment
     */
    std::string_view finish_segment() {
        return std::string_view(buffer_.data() + segment_start_, segment_end_ - segment_start_);
    }

    /**
     * Append raw data
     *
     * @param data Data to append
     * @param len Length of data
     */
    void append(const char* data, size_t len) {
        char* dest = reserve(len);
        std::memcpy(dest, data, len);
        commit(len);
    }

    /**
     * Append string_view
     *
     * @param sv String view to append
     */
    void append(std::string_view sv) { append(sv.data(), sv.size()); }

    /**
     * Get current buffer capacity
     *
     * @return Current buffer capacity in bytes
     */
    size_t capacity() const { return buffer_.capacity(); }

    /**
     * Get current segment size
     *
     * @return Current segment size in bytes
     */
    size_t size() const { return segment_end_ - segment_start_; }

  private:
    std::vector<char> buffer_;
    size_t max_cap_;
    size_t segment_start_;
    size_t segment_end_;
};

} // namespace strata
