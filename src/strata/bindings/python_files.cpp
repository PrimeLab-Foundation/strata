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
#include <sys/stat.h>
#endif
#include <cstdio>
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

/// Read a whole file, mapping failures onto the documented exceptions.
bool read_file_to_string(const char* path, std::string& out) {
    std::FILE* handle = std::fopen(path, "rb");
    if (handle == nullptr) {
        PyErr_SetFromErrnoWithFilename(errno == ENOENT ? PyExc_FileNotFoundError : PyExc_OSError,
                                       path);
        return false;
    }

    // One sized read: stat tells the size, so the chunked append-and-grow
    // dance (and its repeated copies) is only kept as the fallback for the
    // rare stream that will not stat.
#ifndef _WIN32
    struct stat status{};
    if (::fstat(fileno(handle), &status) == 0 && S_ISREG(status.st_mode) && status.st_size > 0) {
        out.resize(static_cast<size_t>(status.st_size));
        const size_t got = std::fread(out.data(), 1, out.size(), handle);
        out.resize(got);
        if (std::ferror(handle) != 0) {
            std::fclose(handle);
            PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
            return false;
        }
        // Anything appended between stat and read is picked up below.
    }
#endif

    char chunk[65536];
    size_t read = 0;
    while ((read = std::fread(chunk, 1, sizeof(chunk), handle)) > 0)
        out.append(chunk, read);

    const bool failed = std::ferror(handle) != 0;
    std::fclose(handle);
    if (failed) {
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

    std::FILE* handle = std::fopen(path, "wb");
    if (handle == nullptr) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }
    const bool wrote =
        std::fwrite(data, 1, static_cast<size_t>(size), handle) == static_cast<size_t>(size) &&
        std::fputc('\n', handle) != EOF; // documented trailing newline
    const bool closed = std::fclose(handle) == 0;
    if (!wrote || !closed) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }

#ifndef _WIN32
    // The documented mode is 0644; fopen would otherwise leave it to the umask.
    if (::chmod(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 0) {
        PyErr_SetFromErrnoWithFilename(PyExc_OSError, path);
        return nullptr;
    }
#endif
    Py_RETURN_NONE;
}

} // namespace strata::bindings
