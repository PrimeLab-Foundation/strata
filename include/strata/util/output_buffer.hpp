#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stdexcept>

#include "strata/util/arena_allocator.hpp"

namespace strata {
namespace util {

class OutputBuffer {
  public:
    OutputBuffer() = default;
    explicit OutputBuffer(Arena* arena) { reset_with_arena(arena); }
    ~OutputBuffer() { release_heap(); }

    OutputBuffer(const OutputBuffer&) = delete;
    OutputBuffer& operator=(const OutputBuffer&) = delete;

    void clear() {
        size_ = 0;
        reserved_extra_ = 0;
    }

    void reset_with_arena(Arena* arena) {
        release_heap();
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        reserved_extra_ = 0;
        using_arena_ = false;
        arena_ = arena;
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
        size_t new_capacity = capacity_ == 0
                                   ? (arena_ ? kArenaInitialCapacity : kHeapInitialCapacity)
                                   : capacity_;
        if (arena_ && new_capacity < kArenaInitialCapacity) {
            new_capacity = kArenaInitialCapacity;
        }
        while (new_capacity < needed) {
            if (arena_) {
                new_capacity *= 2;
            } else {
                new_capacity = new_capacity + (new_capacity / 2);
            }
        }

        if (arena_ && try_grow_arena(new_capacity)) {
            return;
        }

        arena_ = nullptr;
        grow_heap(new_capacity);
    }

    bool try_grow_arena(size_t new_capacity) {
        if (!arena_) {
            return false;
        }
        try {
            char* new_data = static_cast<char*>(arena_->allocate(new_capacity, 1));
            if (size_ > 0) {
                std::memcpy(new_data, data_, size_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
            using_arena_ = true;
            return true;
        } catch (const std::bad_alloc&) {
            return false;
        }
    }

    void grow_heap(size_t new_capacity) {
        if (!using_arena_) {
            void* new_data = std::realloc(data_, new_capacity);
            if (!new_data) {
                throw std::bad_alloc();
            }
            data_ = static_cast<char*>(new_data);
        } else {
            void* new_data = std::malloc(new_capacity);
            if (!new_data) {
                throw std::bad_alloc();
            }
            if (size_ > 0) {
                std::memcpy(new_data, data_, size_);
            }
            data_ = static_cast<char*>(new_data);
        }
        capacity_ = new_capacity;
        using_arena_ = false;
    }

    void release_heap() {
        if (data_ && !using_arena_) {
            std::free(data_);
        }
    }

    static constexpr size_t kHeapInitialCapacity = 1024;
    static constexpr size_t kArenaInitialCapacity = 64 * 1024;

    char* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    size_t reserved_extra_ = 0; // Remaining structural bytes reserved for unchecked writes.
    Arena* arena_ = nullptr;
    bool using_arena_ = false;
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
