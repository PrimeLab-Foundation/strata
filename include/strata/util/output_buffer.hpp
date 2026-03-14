#pragma once

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

class OutputBuffer {
  public:
    OutputBuffer() = default;
    ~OutputBuffer() { std::free(data_); }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer& operator=(const OutputBuffer&) = delete;

    void clear() { size_ = 0; }

    const char* data() const { return data_; }
    char* data() { return data_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

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

    // Unsafe variants: skip capacity check. Call only after reserve(size + n).
    void unsafe_push_back(char c) noexcept { data_[size_++] = c; }
    void unsafe_append(const char* src, size_t len) noexcept {
        std::memcpy(data_ + size_, src, len);
        size_ += len;
    }
    void unsafe_advance(size_t n) noexcept { size_ += n; }

  private:
    void ensure(size_t needed) {
        if (needed > capacity_) {
            grow(needed);
        }
    }

    __attribute__((noinline)) void grow(size_t needed) {
        size_t new_capacity = capacity_ == 0 ? 1024 : capacity_;
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

class FixedOutputBuffer {
  public:
    FixedOutputBuffer(char* data, size_t capacity) : data_(data), capacity_(capacity) {}

    void clear() {
        size_ = 0;
        overflowed_ = false;
    }

    const char* data() const { return data_; }
    char* data() { return data_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool overflowed() const { return overflowed_; }

    void reserve(size_t) {}

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
