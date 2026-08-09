/**
 * @file python_dumps.cpp
 * @brief `strata.dumps` — Python objects to compact JSON text.
 *
 * A recursive walk over the object graph, writing through a staged buffer:
 * every token lands in a stack-resident stage with raw stores, and the
 * thread-local output string sees one append per stageful instead of several
 * per token. `std::string`'s per-append capacity check and size update were
 * a measured double-digit share of serialization time on record-shaped data.
 *
 * The fast paths here are accelerators, never second definitions of the
 * format: the scalar-array runs re-check the element type on every element
 * and fall back mid-list, the string fast path emits exactly the bytes the
 * core escaper would (and defers to it the moment a string needs escaping),
 * and the per-depth schema cache stores bytes produced by that same escaper.
 * All of them are measured in docs/performance/SKILL.md.
 */

#include "python_types.h"
#include "strata/json/json_serialize.hpp"
#include "strata/util/dtoa.hpp"
#include "strata/util/scan.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

namespace {

/**
 * What to do when a container contains itself.
 *
 * Process-global, and seeded to Warn at module import so that the reported
 * setting and the actual behaviour agree from the very first call. The
 * previous implementation seeded the config map with "warn" while this
 * variable started at Ignore, so the two disagreed until the first
 * `config.set` (docs/bindings/SKILL.md: "do not reproduce").
 */
CyclePolicyValue g_cycle_policy = CyclePolicyValue::Warn;

/**
 * A staging buffer between the serializer and the output string.
 *
 * `ensure(n)` guarantees `n` writable bytes at `cursor()`; everything the
 * serializer emits is then raw stores and memcpys, and the output string is
 * touched once per stageful. Reservations must fit the stage — every caller
 * here reserves a small constant — while `write_spanning` takes any size and
 * routes oversized payloads straight to the string.
 */
class StagedOutput {
  public:
    explicit StagedOutput(std::string& sink) : sink_(sink) {}

    StagedOutput(const StagedOutput&) = delete;
    StagedOutput& operator=(const StagedOutput&) = delete;

    /// Make room for @p bytes of upcoming raw writes. @p bytes ≤ stage size.
    void ensure(size_t bytes) {
        if (used_ + bytes > sizeof(stage_))
            flush();
    }

    [[nodiscard]] char* cursor() noexcept { return stage_ + used_; }
    void advance(size_t bytes) noexcept { used_ += bytes; }

    /// One byte, after a covering ensure().
    void put(char c) noexcept { stage_[used_++] = c; }

    /// @p len bytes, after a covering ensure().
    void write(const char* data, size_t len) noexcept {
        std::memcpy(stage_ + used_, data, len);
        used_ += len;
    }

    /// Any length; spills to the sink when the stage cannot hold it.
    void write_spanning(const char* data, size_t len) {
        if (used_ + len <= sizeof(stage_)) {
            std::memcpy(stage_ + used_, data, len);
            used_ += len;
            return;
        }
        flush();
        if (len >= sizeof(stage_) / 2) {
            sink_.append(data, len);
            return;
        }
        std::memcpy(stage_, data, len);
        used_ = len;
    }

    /// The sink, flushed first — for the rare paths that append directly.
    [[nodiscard]] std::string& direct_sink() {
        flush();
        return sink_;
    }

    void flush() {
        if (used_ != 0) {
            sink_.append(stage_, used_);
            used_ = 0;
        }
    }

  private:
    std::string& sink_;
    size_t used_ = 0;
    char stage_[8192];
};

/**
 * The per-thread schema cache, shared across dumps() calls.
 *
 * Sharing is what lets repeated serialization of same-shaped payloads skip
 * key preparation entirely after the first call. The keys are *owned*
 * references: identity comparison is only sound while the objects live, and
 * a borrowed pointer could be freed between calls and reincarnated as a
 * different key. The busy flag covers re-entrancy -- a cycle warning can run
 * arbitrary Python, which can call dumps() again mid-walk; the nested call
 * pays for a private, empty cache instead.
 */
class SchemaCacheLease {
  public:
    /// Prepared `"key":` bytes for one object shape at one depth.
    struct Schema {
        std::vector<PyObject*> keys;      ///< owned references
        std::string blob;                 ///< the prepared bytes, back to back
        std::vector<uint32_t> offsets{0}; ///< blob boundaries, keys.size() + 1
        bool prepared = false;

        void remember(PyObject* const* other, Py_ssize_t count) {
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.assign(other, other + count);
            for (PyObject* key : keys)
                Py_INCREF(key);
            prepared = false;
        }

        [[nodiscard]] bool matches(PyObject* const* other, Py_ssize_t count) const noexcept {
            if (static_cast<Py_ssize_t>(keys.size()) != count)
                return false;
            for (Py_ssize_t index = 0; index < count; ++index) {
                // Identity, not equality: strata interns the keys it parses
                // and CPython interns identifier-like literals, so same-schema
                // records share key objects. A miss costs a rebuild, never a
                // wrong answer.
                if (keys[static_cast<size_t>(index)] != other[index])
                    return false;
            }
            return true;
        }
    };

    SchemaCacheLease() {
        if (!busy_ && shared() != nullptr) {
            busy_ = true;
            slots_ = shared();
            owns_flag_ = true;
        } else {
            slots_ = &fallback_;
        }
    }

    ~SchemaCacheLease() {
        if (owns_flag_)
            busy_ = false;
    }

    SchemaCacheLease(const SchemaCacheLease&) = delete;
    SchemaCacheLease& operator=(const SchemaCacheLease&) = delete;

    [[nodiscard]] std::vector<Schema>& slots() noexcept { return *slots_; }

  private:
    [[nodiscard]] static std::vector<Schema>* shared() {
        // Deliberately leaked: a destructor after interpreter shutdown could
        // not legally Py_DECREF the owned keys anyway.
        static thread_local std::vector<Schema>* instance =
            new (std::nothrow) std::vector<Schema>();
        return instance;
    }

    static thread_local bool busy_;

    std::vector<Schema>* slots_;
    std::vector<Schema> fallback_;
    bool owns_flag_ = false;
};

thread_local bool SchemaCacheLease::busy_ = false;

/// Walks a Python object graph, appending JSON through the staged buffer.
class Serializer {
  public:
    Serializer(StagedOutput& out, std::vector<SchemaCacheLease::Schema>& schemas)
        : out_(out), depth_limit_(Py_GetRecursionLimit()), schemas_(schemas) {}

    [[nodiscard]] bool write(PyObject* object) {
        if (object == Py_None) {
            out_.ensure(4);
            out_.write("null", 4);
            return true;
        }
        // bool is a subclass of int, so it has to be tested first.
        if (PyBool_Check(object)) {
            out_.ensure(5);
            if (object == Py_True)
                out_.write("true", 4);
            else
                out_.write("false", 5);
            return true;
        }
        if (PyLong_Check(object))
            return write_int(object);
        if (PyFloat_Check(object)) {
            write_double(PyFloat_AS_DOUBLE(object));
            return true;
        }
        if (PyUnicode_Check(object))
            return write_string(object);
        if (PyList_Check(object) || PyTuple_Check(object))
            return write_sequence(object);
        if (PyDict_Check(object))
            return write_mapping(object);

        PyErr_Format(PyExc_TypeError, "Object of type %s is not JSON serializable",
                     Py_TYPE(object)->tp_name);
        return false;
    }

  private:
    [[nodiscard]] bool write_int(PyObject* object) {
        int overflow = 0;
        const long long value = PyLong_AsLongLongAndOverflow(object, &overflow);
        if (overflow == 0) {
            if (value == -1 && PyErr_Occurred())
                return false;
            out_.ensure(24);
            const auto written = std::to_chars(out_.cursor(), out_.cursor() + 24, value);
            out_.advance(static_cast<size_t>(written.ptr - out_.cursor()));
            return true;
        }

        // Beyond int64 the digits come from Python itself, so nothing is lost.
        PyRef text(PyObject_Str(object));
        if (!text)
            return false;
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(text.get(), &size);
        if (utf8 == nullptr)
            return false;
        out_.write_spanning(utf8, static_cast<size_t>(size));
        return true;
    }

    void write_double(double value) {
        out_.ensure(util::kDoubleBufferSize);
        if (std::isnan(value) || std::isinf(value)) {
            out_.write("null", 4); // JSON cannot spell either
            return;
        }
        out_.advance(util::format_double(value, out_.cursor(), util::kDoubleBufferSize));
    }

    [[nodiscard]] bool write_string(PyObject* object) {
        // Compact ASCII is the overwhelmingly common shape and its bytes are
        // the UTF-8, sitting right after the header.
        if (PyUnicode_IS_COMPACT_ASCII(object)) {
            const char* data = static_cast<const char*>(PyUnicode_DATA(object));
            const auto size = static_cast<size_t>(PyUnicode_GET_LENGTH(object));
            return write_string_bytes(data, size);
        }
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(object, &size);
        if (utf8 == nullptr)
            return false;
        return write_string_bytes(utf8, static_cast<size_t>(size));
    }

    [[nodiscard]] bool write_string_bytes(const char* data, size_t size) {
        const size_t clean = util::find_next_escape(data, size);
        if (clean == size) {
            if (size + 2 <= 512) {
                out_.ensure(size + 2);
                out_.put('"');
                out_.write(data, size);
                out_.put('"');
            } else {
                out_.ensure(1);
                out_.put('"');
                out_.write_spanning(data, size);
                out_.ensure(1);
                out_.put('"');
            }
            return true;
        }
        // Escaping is the rare path; the core escaper is the one definition
        // of it, and it appends to the sink directly.
        append_escaped_json_string(std::string_view(data, size), out_.direct_sink());
        return true;
    }

    [[nodiscard]] bool write_sequence(PyObject* object) {
        const Frame frame(*this, object);
        if (frame.repeated())
            return frame.handle_cycle();
        if (!frame.within_depth_limit())
            return false;

        out_.ensure(1);
        out_.put('[');
        const Py_ssize_t size = PySequence_Fast_GET_SIZE(object);
        PyObject** items = PySequence_Fast_ITEMS(object);

        // Arrays of one scalar type are the common shape in real payloads, and
        // for them the per-element dispatch through write() is pure overhead.
        // Each run formats straight into the stage; the element type is
        // re-checked per element, so a mixed list just ends the run early and
        // the general loop below picks up exactly where it stopped.
        Py_ssize_t index = write_scalar_run(items, size);

        for (; index < size; ++index) {
            if (index != 0) {
                out_.ensure(1);
                out_.put(',');
            }
            if (!write(items[index]))
                return false;
        }
        out_.ensure(1);
        out_.put(']');
        return true;
    }

    [[nodiscard]] Py_ssize_t write_scalar_run(PyObject** items, Py_ssize_t size) {
        if (size == 0)
            return 0;
        PyTypeObject* type = Py_TYPE(items[0]);
        if (type == &PyFloat_Type)
            return run_floats(items, size);
        if (type == &PyLong_Type)
            return run_ints(items, size);
        if (type == &PyBool_Type)
            return run_bools(items, size);
        if (type == &PyUnicode_Type)
            return run_strings(items, size);
        return 0;
    }

    [[nodiscard]] Py_ssize_t run_floats(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        for (; index < size; ++index) {
            PyObject* item = items[index];
            if (Py_TYPE(item) != &PyFloat_Type)
                break;
            out_.ensure(util::kDoubleBufferSize + 1);
            if (index != 0)
                out_.put(',');
            const double value = PyFloat_AS_DOUBLE(item);
            if (std::isnan(value) || std::isinf(value)) {
                out_.write("null", 4);
            } else {
                out_.advance(util::format_double(value, out_.cursor(), util::kDoubleBufferSize));
            }
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_ints(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        for (; index < size; ++index) {
            PyObject* item = items[index];
            // Not PyLong_Check: bool is a subclass and must not print as a
            // number, and an int subclass may override __repr__.
            if (Py_TYPE(item) != &PyLong_Type)
                break;
            int overflow = 0;
            const long long value = PyLong_AsLongLongAndOverflow(item, &overflow);
            if (overflow != 0)
                break; // the general path renders big ints via their str
            if (value == -1 && PyErr_Occurred()) {
                PyErr_Clear();
                break;
            }
            out_.ensure(25);
            if (index != 0)
                out_.put(',');
            const auto written = std::to_chars(out_.cursor(), out_.cursor() + 24, value);
            out_.advance(static_cast<size_t>(written.ptr - out_.cursor()));
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_bools(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        for (; index < size; ++index) {
            PyObject* item = items[index];
            if (item != Py_True && item != Py_False)
                break;
            out_.ensure(6);
            if (index != 0)
                out_.put(',');
            if (item == Py_True)
                out_.write("true", 4);
            else
                out_.write("false", 5);
        }
        return index;
    }

    /// Clean short ASCII strings, quoted straight into the stage. The first
    /// element needing escapes, non-ASCII text, or real length ends the run.
    [[nodiscard]] Py_ssize_t run_strings(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        for (; index < size; ++index) {
            PyObject* item = items[index];
            if (Py_TYPE(item) != &PyUnicode_Type || !PyUnicode_IS_COMPACT_ASCII(item))
                break;
            const char* data = static_cast<const char*>(PyUnicode_DATA(item));
            const auto length = static_cast<size_t>(PyUnicode_GET_LENGTH(item));
            if (length > 500 || util::find_next_escape(data, length) != length)
                break;
            out_.ensure(length + 3);
            if (index != 0)
                out_.put(',');
            out_.put('"');
            out_.write(data, length);
            out_.put('"');
        }
        return index;
    }

    /// Objects with more keys than this serialize their keys the plain way.
    static constexpr Py_ssize_t kMaxSchemaKeys = 24;
    /// Nesting levels that get a schema slot; deeper objects take the plain walk.
    static constexpr size_t kMaxCachedDepth = 64;

    [[nodiscard]] bool write_mapping(PyObject* object) {
        const Frame frame(*this, object);
        if (frame.repeated())
            return frame.handle_cycle();
        if (!frame.within_depth_limit())
            return false;

        // Documents are overwhelmingly made of records that share a schema, so
        // the same keys get escape-scanned and quoted once per record.
        // Remembering the previous object's keys turns all of that into one
        // memcpy per key. The remembered bytes are produced by the very same
        // escaper, so this is a cache, not a second format.
        PyObject* keys[kMaxSchemaKeys];
        PyObject* values[kMaxSchemaKeys];
        Py_ssize_t count = 0;
        Py_ssize_t position = 0;
        PyObject* key = nullptr;
        PyObject* value = nullptr;
        bool too_many = false;

        while (PyDict_Next(object, &position, &key, &value)) {
            if (!PyUnicode_Check(key)) {
                PyErr_Format(PyExc_TypeError, "keys must be str, not %s", Py_TYPE(key)->tp_name);
                return false;
            }
            if (count == kMaxSchemaKeys) {
                too_many = true;
                break;
            }
            keys[count] = key;
            values[count] = value;
            ++count;
        }

        if (too_many)
            return write_mapping_uncached(object);

        // An empty object has no keys to prepare, and letting it reach the
        // schema path would have it "match" a never-built cache.
        if (count == 0) {
            out_.ensure(2);
            out_.write("{}", 2);
            return true;
        }

        // One cache slot per nesting depth. A single slot thrashes on real
        // documents: a user holds orders which hold items, so by the time the
        // walk returns to the next user the slot describes an item. Keyed by
        // depth, each level keeps its own schema and sibling records — which
        // is where the repetition actually is — hit every time.
        const size_t depth = open_.size();
        if (depth >= kMaxCachedDepth)
            return write_mapping_uncached(object);
        if (schemas_.size() <= depth)
            schemas_.resize(depth + 1);

        // Preparing the bytes costs about what writing them costs, so a schema
        // seen once would pay for a cache it never uses — measurably so on
        // documents of one-off shapes. A miss therefore only *remembers the
        // keys* (a pointer copy); the bytes are prepared on the second
        // sighting, when repetition is established.
        bool prepared = false;
        if (schemas_[depth].matches(keys, count)) {
            if (!schemas_[depth].prepared && !build_schema(schemas_[depth]))
                return false;
            prepared = true;
        } else {
            schemas_[depth].remember(keys, count);
        }

        out_.ensure(1);
        out_.put('{');
        for (Py_ssize_t index = 0; index < count; ++index) {
            if (index != 0) {
                out_.ensure(1);
                out_.put(',');
            }
            if (prepared) {
                // `"key":` — quotes, escapes and colon, prepared once.
                //
                // Re-indexed every iteration rather than held by reference: a
                // nested object may grow `schemas_` and move its elements. The
                // slot's own bytes are never touched by a deeper level, so
                // indexing afresh is both safe and free.
                const Schema& schema = schemas_[depth];
                out_.write_spanning(schema.blob.data() + schema.offsets[static_cast<size_t>(index)],
                                    schema.offsets[static_cast<size_t>(index) + 1] -
                                        schema.offsets[static_cast<size_t>(index)]);
            } else {
                if (!write_string(keys[index]))
                    return false;
                out_.ensure(1);
                out_.put(':');
            }
            if (!write(values[index]))
                return false;
        }
        out_.ensure(1);
        out_.put('}');
        return true;
    }

    using Schema = SchemaCacheLease::Schema;

    /// Prepare the `"key":` bytes for a schema whose keys are already recorded.
    [[nodiscard]] bool build_schema(Schema& schema) {
        schema.blob.clear();
        schema.offsets.assign(1, 0);
        for (PyObject* key : schema.keys) {
            Py_ssize_t size = 0;
            const char* utf8 = PyUnicode_AsUTF8AndSize(key, &size);
            if (utf8 == nullptr) {
                schema.keys.clear(); // never leave a half-built slot cached
                return false;
            }
            append_escaped_json_string(std::string_view(utf8, static_cast<size_t>(size)),
                                       schema.blob);
            schema.blob.push_back(':');
            schema.offsets.push_back(static_cast<uint32_t>(schema.blob.size()));
        }
        schema.prepared = true;
        return true;
    }

    /// The plain walk, for objects too wide to be worth remembering.
    [[nodiscard]] bool write_mapping_uncached(PyObject* object) {
        out_.ensure(1);
        out_.put('{');
        Py_ssize_t position = 0;
        PyObject* key = nullptr;
        PyObject* value = nullptr;
        bool first = true;
        while (PyDict_Next(object, &position, &key, &value)) {
            if (!PyUnicode_Check(key)) {
                PyErr_Format(PyExc_TypeError, "keys must be str, not %s", Py_TYPE(key)->tp_name);
                return false;
            }
            if (!first) {
                out_.ensure(1);
                out_.put(',');
            }
            first = false;
            if (!write_string(key))
                return false;
            out_.ensure(1);
            out_.put(':');
            if (!write(value))
                return false;
        }
        out_.ensure(1);
        out_.put('}');
        return true;
    }

    /**
     * One level of nesting: records the container while it is open, so a
     * container that reappears below itself is recognised as a cycle.
     *
     * The scan is linear in the current depth. A memo keyed by identity was
     * measured slower on the previous implementation — open frames are few,
     * and the scan touches memory that is already hot.
     */
    class Frame {
      public:
        Frame(Serializer& owner, PyObject* container) : owner_(owner) {
            const auto& open = owner_.open_;
            repeated_ = std::find(open.begin(), open.end(), container) != open.end();
            if (!repeated_)
                owner_.open_.push_back(container);
        }

        ~Frame() {
            if (!repeated_)
                owner_.open_.pop_back();
        }

        Frame(const Frame&) = delete;
        Frame& operator=(const Frame&) = delete;

        [[nodiscard]] bool repeated() const noexcept { return repeated_; }

        [[nodiscard]] bool within_depth_limit() const {
            if (static_cast<int>(owner_.open_.size()) <= owner_.depth_limit_)
                return true;
            PyErr_SetString(PyExc_ValueError, "Maximum serialization depth exceeded");
            return false;
        }

        /// Emit the placeholder the policy calls for, and say whether to go on.
        [[nodiscard]] bool handle_cycle() const {
            if (g_cycle_policy == CyclePolicyValue::Error) {
                PyErr_SetString(PyExc_ValueError, "Circular reference detected");
                return false;
            }
            owner_.out_.ensure(4);
            owner_.out_.write("null", 4);
            if (g_cycle_policy == CyclePolicyValue::Warn) {
                // A warning filter set to "error" raises here, which stops the
                // serialization rather than being swallowed.
                return PyErr_WarnEx(PyExc_RuntimeWarning, "Circular reference detected", 1) == 0;
            }
            return true;
        }

      private:
        Serializer& owner_;
        bool repeated_ = false;
    };

    StagedOutput& out_;
    std::vector<PyObject*> open_;
    int depth_limit_;

    // One prepared schema per nesting depth; leased, so it survives the call.
    std::vector<Schema>& schemas_;
};

} // namespace

CyclePolicyValue get_cycle_policy() noexcept { return g_cycle_policy; }

bool set_cycle_policy(std::string_view name) noexcept {
    if (name == "warn") {
        g_cycle_policy = CyclePolicyValue::Warn;
    } else if (name == "error") {
        g_cycle_policy = CyclePolicyValue::Error;
    } else if (name == "ignore") {
        g_cycle_policy = CyclePolicyValue::Ignore;
    } else {
        return false;
    }
    return true;
}

PyObject* dumps_to_python(PyObject* object, bool as_bytes) {
    // Reused across calls on this thread: after the first few documents the
    // buffer has grown to size and stops allocating entirely. Thread-local
    // rather than global because the GIL is held but the buffer outlives the
    // call, and other threads must not share it.
    static thread_local std::string out;
    out.clear();
    if (out.capacity() < kDumpsInitialCapacity)
        out.reserve(kDumpsInitialCapacity);

    StagedOutput staged(out);
    SchemaCacheLease schemas;
    Serializer serializer(staged, schemas.slots());
    if (!serializer.write(object))
        return nullptr;
    staged.flush();

    if (as_bytes)
        return PyBytes_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
    return PyUnicode_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
}

} // namespace strata::bindings
