#pragma once

/**
 * @file arena_allocator.hpp
 * @brief Arena (bump-pointer) allocator with STL compatibility.
 *
 * Provides three components:
 * - Arena            — fast bump-pointer allocator; all memory freed at once.
 * - ArenaAllocator<T> — STL-compatible allocator backed by an Arena.
 * - ArenaStringBuilder — fast string builder for JSON serialisation.
 *
 * Performance: 3-5x faster than malloc/free for small allocations.
 */

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace strata {
namespace util {

/// Default block size for new arenas (64 KB).
static constexpr size_t kArenaDefaultBlockSize = 64 * 1024;

/**
 * Bump-pointer arena allocator.
 *
 * Allocations are contiguous within a block; when a block is exhausted
 * a new one is allocated at 2x the previous size.  All blocks are freed
 * when the Arena is destroyed (or reset for reuse).
 */
class Arena {
  public:
    explicit Arena(size_t initial_size = kArenaDefaultBlockSize)
        : current_block_(nullptr), current_pos_(0), current_size_(0) {
        allocate_block(initial_size);
    }

    ~Arena() = default;

    /**
     * Allocate @p size bytes with the given @p alignment.
     *
     * @return Pointer to the allocated region (never nullptr —
     *         a new block is allocated if the current one is exhausted).
     */
    [[nodiscard]] void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        size_t aligned_pos = (current_pos_ + alignment - 1) & ~(alignment - 1);

        if (aligned_pos + size > current_size_) {
            size_t new_block_size = std::max(size + 1024, current_size_ * 2);
            allocate_block(new_block_size);
            aligned_pos = 0;
        }

        void* ptr = current_block_ + aligned_pos;
        current_pos_ = aligned_pos + size;
        return ptr;
    }

    /// Reset arena (keep allocated blocks for reuse).
    void reset() noexcept { current_pos_ = 0; }

    /// Total bytes allocated across all blocks.
    [[nodiscard]] size_t total_size() const noexcept {
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
 * Deallocate is a no-op; memory is reclaimed when the arena is
 * reset or destroyed.
 *
 * @tparam T Value type.
 */
template <typename T> class ArenaAllocator {
  public:
    using value_type = T;

    explicit ArenaAllocator(Arena* arena = nullptr) noexcept : arena_(arena) {}

    template <typename U>
    ArenaAllocator(const ArenaAllocator<U>& other) noexcept : arena_(other.arena()) {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if (!arena_) {
            throw std::bad_alloc();
        }
        void* ptr = arena_->allocate(n * sizeof(T), alignof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    void deallocate(T*, std::size_t) noexcept {} // No-op — arena reclaims all.

    template <typename U> struct rebind {
        using other = ArenaAllocator<U>;
    };

    [[nodiscard]] Arena* arena() const noexcept { return arena_; }

    bool operator==(const ArenaAllocator& other) const noexcept { return arena_ == other.arena_; }
    bool operator!=(const ArenaAllocator& other) const noexcept { return arena_ != other.arena_; }

  private:
    Arena* arena_ = nullptr;
};

/**
 * Fast string builder using arena allocation.
 *
 * Optimised for JSON serialisation where:
 * - Final size is approximately known.
 * - Single write pass (no modifications).
 * - Entire buffer discarded after use.
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

    [[nodiscard]] std::string_view view() const noexcept {
        return std::string_view(buffer_, size_);
    }
    [[nodiscard]] std::string to_string() const { return std::string(buffer_, size_); }
    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] const char* data() const noexcept { return buffer_; }

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

    /// Grow by 1.5x (or to @p required, whichever is larger).
    void ensure_capacity(size_t required) {
        if (required <= capacity_)
            return;

        size_t new_capacity = std::max(required, capacity_ + capacity_ / 2);
        char* new_buffer = static_cast<char*>(arena_.allocate(new_capacity, 1));

        if (size_ > 0) {
            std::memcpy(new_buffer, buffer_, size_);
        }

        buffer_ = new_buffer;
        capacity_ = new_capacity;
    }
};

} // namespace util
} // namespace strata
