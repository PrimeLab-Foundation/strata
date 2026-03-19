/**
 * @file json_mmap.cpp
 * @brief Memory-mapped JSON file parsing (POSIX).
 *
 * MmapFile is a move-only RAII wrapper around mmap/munmap.  The file
 * is opened read-only, mapped with PROT_READ, and advised as
 * MADV_SEQUENTIAL for optimal kernel page prefetching.
 *
 * After mapping, the data is parsed via parse_json() into a full
 * JsonValue tree (the mapping is unmapped once parsing completes).
 */

#include "strata/json/json_mmap.hpp"

#include "strata/json/json_parse.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace strata {

/**
 * RAII wrapper for memory-mapped file.
 */
class MmapFile {
  public:
    MmapFile() : data_(nullptr), size_(0), fd_(-1) {}

    ~MmapFile() { close(); }

    // No copy
    MmapFile(const MmapFile&) = delete;
    MmapFile& operator=(const MmapFile&) = delete;

    // Move allowed
    MmapFile(MmapFile&& other) noexcept : data_(other.data_), size_(other.size_), fd_(other.fd_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.fd_ = -1;
    }

    MmapFile& operator=(MmapFile&& other) noexcept {
        if (this != &other) {
            close();
            data_ = other.data_;
            size_ = other.size_;
            fd_ = other.fd_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.fd_ = -1;
        }
        return *this;
    }

    bool open(const char* filepath) {
        // Open file
        fd_ = ::open(filepath, O_RDONLY);
        if (fd_ < 0) {
            return false;
        }

        // Get file size
        struct stat st;
        if (fstat(fd_, &st) < 0) {
            ::close(fd_);
            fd_ = -1;
            return false;
        }

        size_ = static_cast<size_t>(st.st_size);

        // Handle empty file — use empty string instead of nullptr
        // to avoid undefined behavior in std::string_view(nullptr, 0).
        if (size_ == 0) {
            data_ = "";
            return true;
        }

        // Memory-map the file
        void* addr = mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0);
        if (addr == MAP_FAILED) {
            ::close(fd_);
            fd_ = -1;
            size_ = 0;
            return false;
        }

        data_ = static_cast<const char*>(addr);

        // Hint to OS: we'll read sequentially
        madvise(const_cast<char*>(data_), size_, MADV_SEQUENTIAL);

        return true;
    }

    void close() {
        if (data_ && size_ > 0) {
            munmap(const_cast<char*>(data_), size_);
        }
        if (fd_ >= 0) {
            ::close(fd_);
        }
        data_ = nullptr;
        size_ = 0;
        fd_ = -1;
    }

    std::string_view view() const { return std::string_view(data_, size_); }

    bool is_open() const { return fd_ >= 0; }

  private:
    const char* data_;
    size_t size_;
    int fd_;
};

Result<JsonDocument> parse_json_file(const char* filepath) {
    // Open and mmap the file
    MmapFile mmap_file;
    if (!mmap_file.open(filepath)) {
        return {Status::ParseError, JsonDocument(JsonValue())};
    }

    // Get view of file contents
    std::string_view file_contents = mmap_file.view();

    // Parse JSON from memory-mapped region
    // NOTE: This creates a copy of the data (JsonValue owns its strings)
    // For true zero-copy, we'd need to keep the mmap alive and use string_views
    // That's a future optimization
    auto result = parse_json(file_contents);

    if (!result.ok()) {
        return {result.status, JsonDocument(JsonValue())};
    }

    // Create document from parsed value
    return {Status::Ok, JsonDocument(std::move(result.value))};
}

Result<JsonCursor> parse_json_file_cursor(const char* filepath) {
    // NOTE: This function is fundamentally broken — the JsonDocument is destroyed
    // at the end of this function, leaving the cursor pointing to freed memory.
    // Callers should use parse_json_file() and call .root() on the document instead.
    // Kept for ABI compatibility but marked as deprecated.
    (void)filepath;
    return {Status::ParseError, JsonCursor()};
}

} // namespace strata
