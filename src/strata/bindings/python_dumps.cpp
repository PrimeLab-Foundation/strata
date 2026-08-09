/**
 * @file python_dumps.cpp
 * @brief `strata.dumps` — Python objects to compact JSON text.
 *
 * A straightforward recursive walk. Escaping and float formatting come from
 * the core so there is exactly one implementation of each; what lives here is
 * everything that is specific to Python objects: type dispatch, the depth
 * ceiling, and cycle handling.
 *
 * The fast paths here are accelerators, never second definitions of the
 * format: the scalar-array runs re-check the element type on every element and
 * fall back mid-list, and the per-depth schema cache stores bytes produced by
 * the same escaper the general path uses. Both are measured in
 * docs/performance/SKILL.md.
 */

#include "python_types.h"
#include "strata/json/json_serialize.hpp"
#include "strata/util/dtoa.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstring>
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

/// Walks a Python object graph, appending JSON to a string.
class Serializer {
  public:
    explicit Serializer(std::string& out) : out_(out), depth_limit_(Py_GetRecursionLimit()) {}

    [[nodiscard]] bool write(PyObject* object) {
        if (object == Py_None) {
            out_.append("null");
            return true;
        }
        // bool is a subclass of int, so it has to be tested first.
        if (PyBool_Check(object)) {
            out_.append(object == Py_True ? "true" : "false");
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
            // std::to_string allocates a string per integer; to_chars writes
            // into the stack and the digits are copied once.
            char digits[24];
            const auto written = std::to_chars(digits, digits + sizeof(digits), value);
            out_.append(digits, static_cast<size_t>(written.ptr - digits));
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
        out_.append(utf8, static_cast<size_t>(size));
        return true;
    }

    void write_double(double value) {
        if (std::isnan(value) || std::isinf(value)) {
            out_.append("null"); // JSON cannot spell either
            return;
        }
        char buffer[util::kDoubleBufferSize];
        out_.append(buffer, util::format_double(value, buffer, sizeof(buffer)));
    }

    [[nodiscard]] bool write_string(PyObject* object) {
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(object, &size);
        if (utf8 == nullptr)
            return false;
        append_escaped_json_string(std::string_view(utf8, static_cast<size_t>(size)), out_);
        return true;
    }

    [[nodiscard]] bool write_sequence(PyObject* object) {
        const Frame frame(*this, object);
        if (frame.repeated())
            return frame.handle_cycle();
        if (!frame.within_depth_limit())
            return false;

        out_.push_back('[');
        const Py_ssize_t size = PySequence_Fast_GET_SIZE(object);
        PyObject** items = PySequence_Fast_ITEMS(object);

        // Arrays of one scalar type are the common shape in real payloads, and
        // for them the per-element type dispatch and the two buffer appends
        // (value, then separator) are pure overhead. The scalar runs below
        // format straight into a stack chunk and append once per chunk.
        Py_ssize_t index = write_scalar_run(items, size);

        for (; index < size; ++index) {
            if (index != 0)
                out_.push_back(',');
            if (!write(items[index]))
                return false;
        }
        out_.push_back(']');
        return true;
    }

    /// How many elements a run formats before flushing its stack chunk.
    static constexpr Py_ssize_t kChunkElements = 64;

    /**
     * Serialize the leading run of same-typed scalars, and say where it ended.
     *
     * The element type is taken from the first element and then *verified on
     * every element*: a list that turns out to be mixed simply stops the run
     * early and the caller's general loop picks up at the returned index. So
     * this is an accelerator, never a second definition of the format — every
     * byte it writes is a byte the general path would have written.
     */
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
        return 0;
    }

    [[nodiscard]] Py_ssize_t run_floats(PyObject** items, Py_ssize_t size) {
        char* const chunk = run_chunk_;
        Py_ssize_t index = 0;
        while (index < size) {
            size_t used = 0;
            const Py_ssize_t limit = std::min(size, index + kChunkElements);
            Py_ssize_t taken = index;
            for (; taken < limit; ++taken) {
                PyObject* item = items[taken];
                if (Py_TYPE(item) != &PyFloat_Type)
                    break;
                if (taken != 0)
                    chunk[used++] = ',';
                const double value = PyFloat_AS_DOUBLE(item);
                if (std::isnan(value) || std::isinf(value)) {
                    std::memcpy(chunk + used, "null", 4);
                    used += 4;
                } else {
                    used += util::format_double(value, chunk + used, util::kDoubleBufferSize);
                }
            }
            out_.append(chunk, used);
            if (taken == index)
                break;
            index = taken;
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_ints(PyObject** items, Py_ssize_t size) {
        // A long long is at most 20 characters; the separator makes 21.
        constexpr size_t kIntBytes = 24;
        char* const chunk = run_chunk_;
        Py_ssize_t index = 0;
        while (index < size) {
            size_t used = 0;
            const Py_ssize_t limit = std::min(size, index + kChunkElements);
            Py_ssize_t taken = index;
            for (; taken < limit; ++taken) {
                PyObject* item = items[taken];
                // Not PyLong_Check: bool is a subclass and must not print as
                // a number, and an int subclass may override __repr__.
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
                if (taken != 0)
                    chunk[used++] = ',';
                const auto written = std::to_chars(chunk + used, chunk + used + kIntBytes, value);
                used = static_cast<size_t>(written.ptr - chunk);
            }
            out_.append(chunk, used);
            if (taken == index)
                break;
            index = taken;
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_bools(PyObject** items, Py_ssize_t size) {
        char* const chunk = run_chunk_;
        Py_ssize_t index = 0;
        while (index < size) {
            size_t used = 0;
            const Py_ssize_t limit = std::min(size, index + kChunkElements);
            Py_ssize_t taken = index;
            for (; taken < limit; ++taken) {
                PyObject* item = items[taken];
                if (item != Py_True && item != Py_False)
                    break;
                if (taken != 0)
                    chunk[used++] = ',';
                if (item == Py_True) {
                    std::memcpy(chunk + used, "true", 4);
                    used += 4;
                } else {
                    std::memcpy(chunk + used, "false", 5);
                    used += 5;
                }
            }
            out_.append(chunk, used);
            if (taken == index)
                break;
            index = taken;
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
        // the same keys get UTF-8-fetched, escape-scanned and quoted once per
        // record. Remembering the previous object's keys turns all of that
        // into one memcpy per key. The remembered bytes are produced by the
        // very same escaper, so this is a cache, not a second format.
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
            out_.append("{}", 2);
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

        out_.push_back('{');
        for (Py_ssize_t index = 0; index < count; ++index) {
            if (index != 0)
                out_.push_back(',');
            if (prepared) {
                // `"key":` — quotes, escapes and colon, prepared once.
                //
                // Re-indexed every iteration rather than held by reference: a
                // nested object may grow `schemas_` and move its elements. The
                // slot's own bytes are never touched by a deeper level, so
                // indexing afresh is both safe and free.
                const Schema& schema = schemas_[depth];
                out_.append(schema.blob.data() + schema.offsets[static_cast<size_t>(index)],
                            schema.offsets[static_cast<size_t>(index) + 1] -
                                schema.offsets[static_cast<size_t>(index)]);
            } else {
                if (!write_string(keys[index]))
                    return false;
                out_.push_back(':');
            }
            if (!write(values[index]))
                return false;
        }
        out_.push_back('}');
        return true;
    }

    /// Prepared `"key":` bytes for one object shape, plus the keys they came from.
    struct Schema {
        std::vector<PyObject*> keys;      ///< borrowed; compared by identity only
        std::string blob;                 ///< the prepared bytes, back to back
        std::vector<uint32_t> offsets{0}; ///< blob boundaries, keys.size() + 1 of them
        bool prepared = false;            ///< whether blob/offsets are built for `keys`

        /// Note these keys without preparing anything (a first sighting).
        void remember(PyObject* const* other, Py_ssize_t count) {
            keys.assign(other, other + count);
            prepared = false;
        }

        /// Whether this slot is exactly these keys, in this order.
        [[nodiscard]] bool matches(PyObject* const* other, Py_ssize_t count) const noexcept {
            if (static_cast<Py_ssize_t>(keys.size()) != count)
                return false;
            for (Py_ssize_t index = 0; index < count; ++index) {
                // Identity, not equality: strata interns the keys it parses and
                // CPython interns identifier-like literals, so same-schema
                // records share key objects. A miss costs a rebuild, never a
                // wrong answer.
                if (keys[static_cast<size_t>(index)] != other[index])
                    return false;
            }
            return true;
        }
    };

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
        out_.push_back('{');
        Py_ssize_t position = 0;
        PyObject* key = nullptr;
        PyObject* value = nullptr;
        bool first = true;
        while (PyDict_Next(object, &position, &key, &value)) {
            if (!PyUnicode_Check(key)) {
                PyErr_Format(PyExc_TypeError, "keys must be str, not %s", Py_TYPE(key)->tp_name);
                return false;
            }
            if (!first)
                out_.push_back(',');
            first = false;
            if (!write_string(key))
                return false;
            out_.push_back(':');
            if (!write(value))
                return false;
        }
        out_.push_back('}');
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
            owner_.out_.append("null");
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

    std::string& out_;
    std::vector<PyObject*> open_;
    int depth_limit_;

    // One prepared schema per nesting depth.
    // Scratch for the scalar runs. A member, not a local: write_sequence
    // recurses, and a 2.5 KB array on every frame turns a deeply nested
    // document into a stack overflow. The runs themselves never recurse, so
    // one buffer serves them all.
    char run_chunk_[kChunkElements * (util::kDoubleBufferSize + 1)];

    std::vector<Schema> schemas_;
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

    Serializer serializer(out);
    if (!serializer.write(object))
        return nullptr;

    if (as_bytes)
        return PyBytes_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
    return PyUnicode_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
}

} // namespace strata::bindings
