#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace strata {
namespace util {

/**
 * Arena allocator for JSON serialization.
 *
 * Fast bump-pointer allocation with minimal overhead.
 * All allocations freed at once when arena is destroyed.
 *
 * Performance: 3-5x faster than malloc/free for small allocations.
 */
class Arena {
  public:
    explicit Arena(size_t initial_size = 64 * 1024) // 64KB default
        : current_block_(nullptr), current_pos_(0), current_size_(0) {
        allocate_block(initial_size);
    }

    ~Arena() {
        // All blocks freed automatically
    }

    // Allocate memory from arena (returns nullptr if allocation fails)
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        // Align current position
        size_t aligned_pos = (current_pos_ + alignment - 1) & ~(alignment - 1);

        // Check if we have space in current block
        if (aligned_pos + size > current_size_) {
            // Need new block
            size_t new_block_size = std::max(size + 1024, current_size_ * 2);
            allocate_block(new_block_size);
            aligned_pos = 0;
        }

        void* ptr = current_block_ + aligned_pos;
        current_pos_ = aligned_pos + size;
        return ptr;
    }

    // Reset arena (keep memory for reuse)
    void reset() {
        if (blocks_.empty()) return;

        // Keep only the first block to prevent unbounded memory growth.
        // The first block is typically small (64KB) or the initial size.
        if (blocks_.size() > 1) {
            blocks_.resize(1);
        }

        current_block_ = blocks_[0].data.get();
        current_size_ = blocks_[0].size;
        current_pos_ = 0;
    }

    // Get total allocated memory
    size_t total_size() const {
        size_t total = 0;
        for (const auto& block : blocks_) {
            total += block.size;
        }
        return total;
    }

  private:
    struct Block {
        std::unique_ptr<char[]> data;
        size_t size;
    };

    std::vector<Block> blocks_;
    char* current_block_;
    size_t current_pos_;
    size_t current_size_;

    void allocate_block(size_t size) {
        Block block;
        block.data = std::make_unique<char[]>(size);
        block.size = size;
        current_block_ = block.data.get();
        current_size_ = size;
        current_pos_ = 0;
        blocks_.push_back(std::move(block));
    }
};

/**
 * STL-compatible allocator backed by an Arena.
 *
 * Deallocate is a no-op; memory is reclaimed on arena reset/destruction.
 */
template <typename T> class ArenaAllocator {
  public:
    using value_type = T;

    explicit ArenaAllocator(Arena* arena = nullptr) noexcept : arena_(arena) {}

    template <typename U>
    ArenaAllocator(const ArenaAllocator<U>& other) noexcept : arena_(other.arena()) {}

    T* allocate(std::size_t n) {
        if (!arena_) {
            throw std::bad_alloc();
        }
        void* ptr = arena_->allocate(n * sizeof(T), alignof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    void deallocate(T*, std::size_t) noexcept {}

    template <typename U> struct rebind {
        using other = ArenaAllocator<U>;
    };

    Arena* arena() const noexcept { return arena_; }

    bool operator==(const ArenaAllocator& other) const noexcept { return arena_ == other.arena_; }

    bool operator!=(const ArenaAllocator& other) const noexcept { return arena_ != other.arena_; }

  private:
    Arena* arena_ = nullptr;
};

/**
 * Fast string builder using arena allocation.
 *
 * Optimized for JSON serialization where:
 * - Final size is approximately known
 * - Single write pass (no modifications)
 * - Entire buffer discarded after use
 */
class ArenaStringBuilder {
  public:
    explicit ArenaStringBuilder(Arena& arena, size_t initial_reserve = 4096)
        : arena_(arena), capacity_(initial_reserve), size_(0) {
        buffer_ = static_cast<char*>(arena_.allocate(capacity_, 1));
    }

    void push_back(char c) {
        ensure_capacity(size_ + 1);
        buffer_[size_++] = c;
    }

    void append(const char* str, size_t len) {
        ensure_capacity(size_ + len);
        std::memcpy(buffer_ + size_, str, len);
        size_ += len;
    }

    void append(const char* str) { append(str, std::strlen(str)); }

    std::string_view view() const { return std::string_view(buffer_, size_); }

    std::string to_string() const { return std::string(buffer_, size_); }

    size_t size() const { return size_; }
    const char* data() const { return buffer_; }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ensure_capacity(new_capacity);
        }
    }

  private:
    Arena& arena_;
    char* buffer_;
    size_t capacity_;
    size_t size_;

    void ensure_capacity(size_t required) {
        if (required <= capacity_)
            return;

        // Grow by 1.5x or to required size
        size_t new_capacity = std::max(required, capacity_ + capacity_ / 2);
        char* new_buffer = static_cast<char*>(arena_.allocate(new_capacity, 1));

        // Copy existing data
        if (size_ > 0) {
            std::memcpy(new_buffer, buffer_, size_);
        }

        buffer_ = new_buffer;
        capacity_ = new_capacity;
    }
};

} // namespace util
} // namespace strata
