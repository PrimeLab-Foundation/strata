#pragma once

/**
 * @file output_buffer.hpp
 * @brief Growable and fixed-size output buffers for JSON serialisation.
 *
 * OutputBuffer is a realloc-based growable buffer with unsafe fast-path
 * variants (unsafe_push_back, unsafe_append) that skip capacity checks
 * when the caller has already called reserve().
 *
 * FixedOutputBuffer writes into a caller-owned region and sets an
 * overflow flag instead of growing.
 */

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif
#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

namespace strata {
namespace util {

/// Default initial capacity for OutputBuffer (bytes).
static constexpr size_t kOutputBufferDefaultCapacity = 1024;

/**
 * Growable output buffer backed by malloc/realloc.
 *
 * Move-only. Memory is freed in the destructor.
 */
class OutputBuffer {
  public:
    OutputBuffer() = default;
    ~OutputBuffer() { std::free(data_); }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer& operator=(const OutputBuffer&) = delete;

    void clear() noexcept { size_ = 0; }

    [[nodiscard]] const char* data() const noexcept { return data_; }
    [[nodiscard]] char* data() noexcept { return data_; }
    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }

    void reserve(size_t min_capacity) {
        if (min_capacity <= capacity_) {
            return;
        }
        grow(min_capacity);
    }

    void append(const char* src, size_t len) {
        if (UNLIKELY(len == 0)) {
            return;
        }
        size_t new_size = size_ + len;
        if (UNLIKELY(new_size > capacity_)) {
            grow(new_size);
        }
        std::memcpy(data_ + size_, src, len);
        size_ = new_size;
    }

    void push_back(char c) {
        if (UNLIKELY(size_ == capacity_)) {
            grow(size_ + 1);
        }
        data_[size_++] = c;
    }

    /// Skip capacity check. Caller must have reserved sufficient space.
    void unsafe_push_back(char c) noexcept { data_[size_++] = c; }
    /// Skip capacity check. Caller must have reserved sufficient space.
    void unsafe_append(const char* src, size_t len) noexcept {
        std::memcpy(data_ + size_, src, len);
        size_ += len;
    }
    /// Advance write position without writing. Caller must have reserved.
    void unsafe_advance(size_t n) noexcept { size_ += n; }

  private:
    void ensure(size_t needed) {
        if (needed > capacity_) {
            grow(needed);
        }
    }

    /// Grow by 1.5x (or to @p needed, whichever is larger).
    __attribute__((noinline)) void grow(size_t needed) {
        size_t new_capacity = capacity_ == 0 ? kOutputBufferDefaultCapacity : capacity_;
        while (new_capacity < needed) {
            new_capacity = new_capacity + (new_capacity / 2);
        }

        void* new_data = std::realloc(data_, new_capacity);
        if (!new_data) {
            throw std::bad_alloc();
        }
        data_ = static_cast<char*>(new_data);
        capacity_ = new_capacity;
    }

    char* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

/**
 * Fixed-size output buffer (no dynamic growth).
 *
 * Writes into a caller-owned region. Sets an overflow flag when
 * the capacity is exceeded instead of throwing.
 */
class FixedOutputBuffer {
  public:
    FixedOutputBuffer(char* data, size_t capacity) : data_(data), capacity_(capacity) {}

    void clear() noexcept {
        size_ = 0;
        overflowed_ = false;
    }

    [[nodiscard]] const char* data() const noexcept { return data_; }
    [[nodiscard]] char* data() noexcept { return data_; }
    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }
    [[nodiscard]] bool overflowed() const noexcept { return overflowed_; }

    void reserve(size_t) {} // No-op for fixed buffers.

    void append(const char* src, size_t len) {
        if (len == 0 || overflowed_) {
            return;
        }
        if (size_ + len > capacity_) {
            overflowed_ = true;
            return;
        }
        std::memcpy(data_ + size_, src, len);
        size_ += len;
    }

    void push_back(char c) {
        if (overflowed_) {
            return;
        }
        if (size_ + 1 > capacity_) {
            overflowed_ = true;
            return;
        }
        data_[size_++] = c;
    }

    // For FixedOutputBuffer, unsafe variants still check bounds (no dynamic growth possible).
    void unsafe_push_back(char c) { push_back(c); }
    void unsafe_append(const char* src, size_t len) { append(src, len); }
    void unsafe_advance(size_t n) {
        if (size_ + n <= capacity_)
            size_ += n;
        else
            overflowed_ = true;
    }

  private:
    char* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    bool overflowed_ = false;
};

} // namespace util
} // namespace strata
