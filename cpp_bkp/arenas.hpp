#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace strata {

/**
 * Bump allocator for fast, arena-style allocations
 * No individual deallocations; reset() clears entire arena
 * Thread-local usage (not thread-safe)
 */
class Arena {
  public:
    static constexpr size_t DEFAULT_BLOCK_SIZE = 64 * 1024; // 64 KB blocks

    explicit Arena(size_t block_size = DEFAULT_BLOCK_SIZE)
        : block_size_(block_size), current_(nullptr), remaining_(0) {
        allocate_block();
    }

    ~Arena() {
        for (auto* block : blocks_) {
            ::operator delete(block);
        }
    }

    // Non-copyable, movable
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;
    Arena(Arena&&) noexcept = default;
    Arena& operator=(Arena&&) noexcept = default;

    /**
     * Allocate raw bytes (aligned to 8 bytes)
     */
    void* allocate(size_t bytes) {
        // Align to 8 bytes
        bytes = (bytes + 7) & ~7;

        if (bytes > remaining_) {
            allocate_block(bytes);
        }

        void* ptr = current_;
        current_ += bytes;
        remaining_ -= bytes;
        return ptr;
    }

    /**
     * Allocate and construct object
     */
    template <typename T, typename... Args> T* create(Args&&... args) {
        void* mem = allocate(sizeof(T));
        return new (mem) T(std::forward<Args>(args)...);
    }

    /**
     * Allocate array of objects (default-constructed)
     */
    template <typename T> T* create_array(size_t count) {
        void* mem = allocate(sizeof(T) * count);
        T* arr = static_cast<T*>(mem);
        for (size_t i = 0; i < count; ++i) {
            new (&arr[i]) T();
        }
        return arr;
    }

    /**
     * Reset arena for reuse (doesn't free memory)
     */
    void reset() {
        if (!blocks_.empty()) {
            current_ = static_cast<char*>(blocks_[0]);
            remaining_ = block_size_;
        }
        // Keep all blocks allocated for reuse
    }

    /**
     * Get total bytes allocated (across all blocks)
     */
    size_t bytes_allocated() const { return blocks_.size() * block_size_ - remaining_; }

  private:
    void allocate_block(size_t min_size = 0) {
        size_t alloc_size = std::max(block_size_, min_size);
        void* block = ::operator new(alloc_size);
        blocks_.push_back(block);
        current_ = static_cast<char*>(block);
        remaining_ = alloc_size;
    }

    size_t block_size_;
    char* current_;
    size_t remaining_;
    std::vector<void*> blocks_;
};

/**
 * Small-buffer-optimized vector (SBO)
 * Stores up to N elements inline; heap-allocates for larger sizes
 */
template <typename T, size_t N = 32> class small_vector {
  public:
    small_vector() : size_(0), capacity_(N), data_(inline_storage()) {}

    ~small_vector() {
        clear();
        if (!is_inline()) {
            ::operator delete(data_);
        }
    }

    // Copyable (deep copy)
    small_vector(const small_vector& other) : size_(0), capacity_(N), data_(inline_storage()) {
        reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            push_back(other[i]);
        }
    }

    small_vector& operator=(const small_vector& other) {
        if (this != &other) {
            clear();
            reserve(other.size_);
            for (size_t i = 0; i < other.size_; ++i) {
                push_back(other[i]);
            }
        }
        return *this;
    }

    // Movable
    small_vector(small_vector&& other) noexcept : size_(other.size_), capacity_(other.capacity_) {
        if (other.is_inline()) {
            data_ = inline_storage();
            for (size_t i = 0; i < size_; ++i) {
                new (&data_[i]) T(std::move(other.data_[i]));
            }
        } else {
            data_ = other.data_;
            other.data_ = other.inline_storage();
            other.capacity_ = N;
        }
        other.size_ = 0;
    }

    small_vector& operator=(small_vector&& other) noexcept {
        if (this != &other) {
            clear();
            if (!is_inline()) {
                ::operator delete(data_);
            }

            size_ = other.size_;
            capacity_ = other.capacity_;

            if (other.is_inline()) {
                data_ = inline_storage();
                for (size_t i = 0; i < size_; ++i) {
                    new (&data_[i]) T(std::move(other.data_[i]));
                }
            } else {
                data_ = other.data_;
                other.data_ = other.inline_storage();
                other.capacity_ = N;
            }
            other.size_ = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        new (&data_[size_]) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            grow();
        }
        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    template <typename... Args> void emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            grow();
        }
        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_)
            return;

        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }

        if (!is_inline()) {
            ::operator delete(data_);
        }

        data_ = new_data;
        capacity_ = new_capacity;
    }

    // Accessors
    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    T* data() { return data_; }
    const T* data() const { return data_; }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

  private:
    bool is_inline() const { return data_ == inline_storage(); }

    T* inline_storage() { return reinterpret_cast<T*>(&storage_); }

    const T* inline_storage() const { return reinterpret_cast<const T*>(&storage_); }

    void grow() {
        size_t new_capacity = capacity_ * 2;
        reserve(new_capacity);
    }

    size_t size_;
    size_t capacity_;
    T* data_;
    alignas(T) char storage_[N * sizeof(T)];
};

} // namespace strata
