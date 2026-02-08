#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace strata {
namespace util {

class OutputBuffer {
  public:
    OutputBuffer() = default;
    ~OutputBuffer() { std::free(data_); }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer& operator=(const OutputBuffer&) = delete;

    void clear() {
        size_ = 0;
        reserved_extra_ = 0;
    }

    const char* data() const { return data_; }
    char* data() { return data_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

    void reserve(size_t min_capacity) {
        size_t needed = min_capacity;
        if (reserved_extra_ > 0 && size_ + reserved_extra_ > needed) {
            needed = size_ + reserved_extra_;
        }
        if (needed <= capacity_) {
            return;
        }
        grow(needed);
    }

    void ensure_extra(size_t n) {
        if (n == 0) {
            return;
        }
        reserved_extra_ += n;
        ensure(size_ + reserved_extra_);
    }

    void append(const char* src, size_t len) {
        if (len == 0) {
            return;
        }
        ensure(size_ + len + reserved_extra_);
        std::memcpy(data_ + size_, src, len);
        size_ += len;
    }

    void push_back(char c) {
        ensure(size_ + 1 + reserved_extra_);
        data_[size_++] = c;
    }

    void push_back_unchecked(char c) {
        assert(reserved_extra_ > 0);
        data_[size_++] = c;
        if (reserved_extra_ > 0) {
            --reserved_extra_;
        }
    }

  private:
    void ensure(size_t needed) {
        if (needed > capacity_) {
            grow(needed);
        }
    }

    void grow(size_t needed) {
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
    size_t reserved_extra_ = 0; // Remaining structural bytes reserved for unchecked writes.
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

    void ensure_extra(size_t) {}

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

    void push_back_unchecked(char c) { push_back(c); }

  private:
    char* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    bool overflowed_ = false;
};

} // namespace util
} // namespace strata
