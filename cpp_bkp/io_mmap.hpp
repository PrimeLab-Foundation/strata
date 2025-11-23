#pragma once

#include <cstddef>
#include <string>

namespace strata {

/**
 * Cross-platform memory-mapped file wrapper (PR #4)
 *
 * Provides RAII-based memory mapping for large files to reduce I/O overhead.
 * Supports Linux, macOS, and Windows with automatic fallback on failure.
 *
 * Features:
 * - Zero-copy file access
 * - Automatic cleanup on destruction
 * - Platform-specific implementations
 * - Safe fallback semantics
 *
 * Usage:
 *   MMap mmap(path);
 *   if (mmap.valid()) {
 *       std::string_view data(mmap.data(), mmap.size());
 *       // Use data...
 *   }
 */
class MMap {
  public:
    /**
     * Memory-map a file for reading
     *
     * @param path Path to file
     * @throws Nothing; check valid() after construction
     */
    explicit MMap(const std::string& path) noexcept;

    /**
     * Destructor unmaps and closes handles
     */
    ~MMap() noexcept;

    // Non-copyable
    MMap(const MMap&) = delete;
    MMap& operator=(const MMap&) = delete;

    // Movable
    MMap(MMap&& other) noexcept;
    MMap& operator=(MMap&& other) noexcept;

    /**
     * Check if mapping succeeded
     */
    bool valid() const noexcept { return data_ != nullptr && size_ > 0; }

    /**
     * Get pointer to mapped data
     */
    const char* data() const noexcept { return data_; }

    /**
     * Get size of mapped region
     */
    size_t size() const noexcept { return size_; }

    /**
     * Get error message if mapping failed
     */
    const std::string& error() const noexcept { return error_msg_; }

  private:
    void cleanup() noexcept;

    const char* data_ = nullptr;
    size_t size_ = 0;
    std::string error_msg_;

#if defined(_WIN32) || defined(_WIN64)
    void* file_handle_ = nullptr;
    void* map_handle_ = nullptr;
#else
    int fd_ = -1;
#endif
};

} // namespace strata
