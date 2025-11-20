#include "io_mmap.hpp"

#include <cerrno>
#include <cstring>

// Platform-specific headers
#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace strata {

#if defined(_WIN32) || defined(_WIN64)

// ============================================================================
// Windows Implementation
// ============================================================================

MMap::MMap(const std::string& path) noexcept
    : data_(nullptr), size_(0), file_handle_(INVALID_HANDLE_VALUE), map_handle_(nullptr) {

    // Open file for reading
    file_handle_ = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle_ == INVALID_HANDLE_VALUE) {
        error_msg_ = "Failed to open file";
        return;
    }

    // Get file size
    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file_handle_, &file_size)) {
        error_msg_ = "Failed to get file size";
        cleanup();
        return;
    }

    if (file_size.QuadPart == 0) {
        error_msg_ = "Empty file";
        cleanup();
        return;
    }

    size_ = static_cast<size_t>(file_size.QuadPart);

    // Create file mapping
    map_handle_ = CreateFileMappingA(file_handle_, nullptr, PAGE_READONLY, 0, 0, nullptr);

    if (map_handle_ == nullptr) {
        error_msg_ = "Failed to create file mapping";
        cleanup();
        return;
    }

    // Map view of file
    void* addr = MapViewOfFile(map_handle_, FILE_MAP_READ, 0, 0, 0);

    if (addr == nullptr) {
        error_msg_ = "Failed to map view of file";
        cleanup();
        return;
    }

    data_ = static_cast<const char*>(addr);
}

MMap::~MMap() noexcept { cleanup(); }

void MMap::cleanup() noexcept {
    if (data_ != nullptr) {
        UnmapViewOfFile(const_cast<char*>(data_));
        data_ = nullptr;
    }

    if (map_handle_ != nullptr) {
        CloseHandle(map_handle_);
        map_handle_ = nullptr;
    }

    if (file_handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(file_handle_);
        file_handle_ = INVALID_HANDLE_VALUE;
    }

    size_ = 0;
}

#else

// ============================================================================
// POSIX Implementation (Linux, macOS, etc.)
// ============================================================================

MMap::MMap(const std::string& path) noexcept : data_(nullptr), size_(0), fd_(-1) {

    // Open file for reading
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ < 0) {
        error_msg_ = "Failed to open file: ";
        error_msg_ += std::strerror(errno);
        return;
    }

    // Get file size
    struct stat st;
    if (fstat(fd_, &st) < 0) {
        error_msg_ = "Failed to stat file: ";
        error_msg_ += std::strerror(errno);
        cleanup();
        return;
    }

    if (st.st_size == 0) {
        error_msg_ = "Empty file";
        cleanup();
        return;
    }

    size_ = static_cast<size_t>(st.st_size);

    // Memory-map the file
    void* addr = mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0);
    if (addr == MAP_FAILED) {
        error_msg_ = "Failed to mmap file: ";
        error_msg_ += std::strerror(errno);
        cleanup();
        return;
    }

    data_ = static_cast<const char*>(addr);

    // Hint to kernel about access pattern (sequential read)
#if defined(MADV_SEQUENTIAL)
    madvise(const_cast<char*>(data_), size_, MADV_SEQUENTIAL);
#endif
}

MMap::~MMap() noexcept { cleanup(); }

void MMap::cleanup() noexcept {
    if (data_ != nullptr) {
        munmap(const_cast<char*>(data_), size_);
        data_ = nullptr;
    }

    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }

    size_ = 0;
}

#endif

// ============================================================================
// Move semantics (common for all platforms)
// ============================================================================

MMap::MMap(MMap&& other) noexcept
    : data_(other.data_), size_(other.size_), error_msg_(std::move(other.error_msg_))
#if defined(_WIN32) || defined(_WIN64)
      ,
      file_handle_(other.file_handle_), map_handle_(other.map_handle_)
#else
      ,
      fd_(other.fd_)
#endif
{
    other.data_ = nullptr;
    other.size_ = 0;
#if defined(_WIN32) || defined(_WIN64)
    other.file_handle_ = INVALID_HANDLE_VALUE;
    other.map_handle_ = nullptr;
#else
    other.fd_ = -1;
#endif
}

MMap& MMap::operator=(MMap&& other) noexcept {
    if (this != &other) {
        cleanup();

        data_ = other.data_;
        size_ = other.size_;
        error_msg_ = std::move(other.error_msg_);
#if defined(_WIN32) || defined(_WIN64)
        file_handle_ = other.file_handle_;
        map_handle_ = other.map_handle_;
#else
        fd_ = other.fd_;
#endif

        other.data_ = nullptr;
        other.size_ = 0;
#if defined(_WIN32) || defined(_WIN64)
        other.file_handle_ = INVALID_HANDLE_VALUE;
        other.map_handle_ = nullptr;
#else
        other.fd_ = -1;
#endif
    }
    return *this;
}

} // namespace strata
