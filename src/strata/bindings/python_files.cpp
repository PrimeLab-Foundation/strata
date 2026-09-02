/**
 * @file python_files.cpp
 * @brief `strata.load` and `strata.dump` in file mode, and NDJSON.
 *
 * Dispatch is by extension: `.ndjson` and `.jsonl` are line-delimited records,
 * anything else is one document (docs/context/api.md, § File & folder I/O).
 *
 * The invalid-line contract is the load-bearing part here: **a malformed
 * NDJSON line raises**, and only `skip_errors=True` silences it, identically
 * whether the caller reads eagerly or through the iterator. The previous
 * implementation skipped bad lines silently and returned a short list, which
 * the docs call out as not to be reproduced.
 */

#include "python_types.h"
#include "strata/json/json_document.hpp"
#include "strata/json/json_parse.hpp"

#include <cerrno>
#ifndef _WIN32
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#else
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#endif
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace strata::bindings {

namespace {

/// Case-insensitive suffix test, so `.NDJSON` reads like `.ndjson`.
[[nodiscard]] bool has_suffix_impl(std::string_view path, std::string_view suffix) {
    if (path.size() < suffix.size())
        return false;
    const size_t offset = path.size() - suffix.size();
    for (size_t index = 0; index < suffix.size(); ++index) {
        char c = path[offset + index];
        if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
        if (c != suffix[index])
            return false;
    }
    return true;
}

} // namespace

bool file_is_ndjson(const char* path) {
    return has_suffix_impl(path, ".ndjson") || has_suffix_impl(path, ".jsonl");
}

namespace {

/// One read(2) into @p buffer, retried on EINTR; -1 with errno on failure.
/// Capped at 1 GB per call: past that the kernels disagree (Linux clamps,
/// macOS rejects with EINVAL, `_read` takes an unsigned int), so the caller
/// loops in slices instead.
[[nodiscard]] std::ptrdiff_t read_some(int fd, char* buffer, size_t count) noexcept {
    constexpr size_t kMaxSlice = size_t{1} << 30;
    const size_t slice = count > kMaxSlice ? kMaxSlice : count;
#ifndef _WIN32
    for (;;) {
        const ssize_t got = ::read(fd, buffer, slice);
        if (got < 0 && errno == EINTR)
            continue;
        return got;
    }
#else
    return ::_read(fd, buffer, static_cast<unsigned int>(slice));
#endif
}

[[nodiscard]] int close_descriptor(int fd) noexcept {
#ifndef _WIN32
    return ::close(fd);
#else
    return ::_close(fd);
#endif
}

} // namespace

/// Read a whole file, mapping failures onto the documented exceptions.
///
/// Raw descriptor I/O on both platforms, the same shape as dump_to_file:
/// one open, one fstat for the size, one sized read straight into the
/// string, one close. The stdio route paid a FILE allocation with its own
/// buffer and a lock per call — and on Windows the sized read was missing
/// altogether, so every file there went through the 64 KB append-and-grow
/// loop, copying itself on each capacity doubling: the one leg whose `load`
/// rows trailed their `loads` rows by more than the read itself. Anything
/// past the stat'd size (a file growing under us, or a stream that will not
/// stat) still arrives through the chunk loop.
bool read_file_to_string(const char* path, std::string& out) {
#ifndef _WIN32
    const int fd = ::open(path, O_RDONLY | O_CLOEXEC);
#else
    const int fd = ::_open(path, _O_RDONLY | _O_BINARY | _O_NOINHERIT);
#endif
    if (fd < 0) {
        PyErr_SetFromErrnoWithFilename(errno == ENOENT ? PyExc_FileNotFoundError : PyExc_OSError,
                                       path);
        return false;
    }

    size_t expected = 0;
#ifndef _WIN32
    struct stat status{};
    if (::fstat(fd, &status) == 0 && S_ISREG(status.st_mode) && status.st_size > 0)
        expected = static_cast<size_t>(status.st_size);
#else
    struct _stat64 status{};
    if (::_fstat64(fd, &status) == 0 && (status.st_mode & _S_IFREG) != 0 && status.st_size > 0)
        expected = static_cast<size_t>(status.st_size);
#endif

    bool failed = false;
    if (expected > 0) {
        out.resize(expected);
        size_t filled = 0;
        while (filled < expected) {
            const std::ptrdiff_t got = read_some(fd, out.data() + filled, expected - filled);
            if (got < 0) {
                failed = true;
                break;
            }
            if (got == 0)
                break;
            filled += static_cast<size_t>(got);
        }
        out.resize(filled);
    }

    if (!failed) {
        char chunk[65536];
        for (;;) {
            const std::ptrdiff_t got = read_some(fd, chunk, sizeof(chunk));
            if (got < 0) {
                failed = true;
                break;
            }
            if (got == 0)
                break;
            out.append(chunk, static_cast<size_t>(got));
        }
    }

    if (failed) {
        const int saved = errno;
        (void)close_descriptor(fd);
        errno = saved;
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return false;
    }
    if (close_descriptor(fd) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return false;
    }
    return true;
}

PyObject* load_from_file(const char* path, const char* return_type, bool iterator,
                         bool skip_errors) {
    const bool is_ndjson = file_is_ndjson(path);
    const bool want_cursor = std::string_view(return_type) == "cursor";

    if (is_ndjson && want_cursor) {
        PyErr_SetString(PyExc_ValueError, "return_type=\"cursor\" is not supported for NDJSON");
        return nullptr;
    }
    if (!want_cursor && std::string_view(return_type) != "dict") {
        PyErr_Format(PyExc_ValueError, "invalid return_type: %s", return_type);
        return nullptr;
    }

    std::string text;
    if (!read_file_to_string(path, text))
        return nullptr;

    if (is_ndjson) {
        // Lazily: the file is read up front, but each line is parsed only when
        // it is reached, so a malformed line raises at that point rather than
        // before iteration begins (docs/context/api.md).
        if (iterator)
            return make_ndjson_iterator(std::move(text), skip_errors);
        return ndjson_to_list(text, skip_errors);
    }

    if (text.empty()) {
        PyErr_SetString(PyExc_ValueError, "Empty file");
        return nullptr;
    }

    if (want_cursor) {
        auto document = JsonDocument::from_string(text);
        if (!document.ok()) {
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return nullptr;
        }
        return make_root_cursor(document.value.share());
    }

    PyRef value(loads_to_python(text, /*validate_utf8=*/true));
    if (!value)
        return nullptr;
    if (!iterator)
        return value.release();
    return make_root_iterator(value.get());
}

PyObject* dump_to_file(PyObject* object, const char* path) {
    PyRef text(dumps_to_python(object, /*as_bytes=*/true));
    if (!text)
        return nullptr;

    char* data = nullptr;
    Py_ssize_t size = 0;
    if (PyBytes_AsStringAndSize(text.get(), &data, &size) != 0)
        return nullptr;

#ifndef _WIN32
    // Raw descriptor I/O: the stdio route paid a buffer copy, a separate
    // flush write for the newline, and an unconditional path-resolving chmod
    // on every call — fixed per-call costs that decide the small-document
    // dump rows (and folder dump multiplies them per group file). The
    // documented exact 0644 (api.md) survives as an fd-based fstat that
    // escalates to fchmod only when the mode actually differs: open()'s mode
    // argument applies on creation only and is filtered by the umask, so
    // neither it nor creation alone can be trusted.
    constexpr mode_t kDocumentedMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    const int fd = ::open(path, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, kDocumentedMode);
    if (fd < 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }

    // One vectored write covers the document and the documented trailing
    // newline: the newline cannot be appended to the exact-sized immutable
    // bytes object, and a second write() syscall for one byte was a
    // measurable share of the per-call floor that decides the small-document
    // dump rows (folder dump repeats it per group file).
    char newline = '\n';
    struct iovec parts[2];
    parts[0].iov_base = data;
    parts[0].iov_len = static_cast<size_t>(size);
    parts[1].iov_base = &newline;
    parts[1].iov_len = 1;
    int active = 0;
    while (active < 2) {
        const ssize_t wrote = ::writev(fd, parts + active, 2 - active);
        if (wrote <= 0) {
            if (wrote < 0 && errno == EINTR)
                continue;
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
            ::close(fd);
            return nullptr;
        }
        auto consumed = static_cast<size_t>(wrote);
        while (active < 2 && consumed >= parts[active].iov_len) {
            consumed -= parts[active].iov_len;
            ++active;
        }
        if (active < 2 && consumed != 0) {
            parts[active].iov_base = static_cast<char*>(parts[active].iov_base) + consumed;
            parts[active].iov_len -= consumed;
        }
    }

    struct stat status;
    if (::fstat(fd, &status) != 0 || (status.st_mode & 07777) != kDocumentedMode) {
        if (::fchmod(fd, kDocumentedMode) != 0) {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
            ::close(fd);
            return nullptr;
        }
    }
    if (::close(fd) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }
#else
    // Same raw-descriptor shape as the POSIX branch, in CRT spelling: stdio's
    // fwrite pays a buffer copy and a FILE lock per call, and dropping it was
    // the dominant share of the small-document dump win on the POSIX side
    // (dump mixed 1.25x -> 1.03x there). Windows has no writev, so the
    // documented trailing newline is a second _write; there is no mode
    // enforcement here — the exact-0644 contract is POSIX-only (api.md).
    const int fd = ::_open(path, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY | _O_NOINHERIT,
                           _S_IREAD | _S_IWRITE);
    if (fd < 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }
    const char* cursor = data;
    size_t remaining = static_cast<size_t>(size);
    while (remaining > 0) {
        const unsigned int chunk =
            remaining > (1u << 30) ? (1u << 30) : static_cast<unsigned int>(remaining);
        const int wrote = ::_write(fd, cursor, chunk);
        if (wrote <= 0) {
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
            ::_close(fd);
            return nullptr;
        }
        cursor += wrote;
        remaining -= static_cast<size_t>(wrote);
    }
    if (::_write(fd, "\n", 1) != 1) { // documented trailing newline
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        ::_close(fd);
        return nullptr;
    }
    if (::_close(fd) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }
#endif
    Py_RETURN_NONE;
}

} // namespace strata::bindings
