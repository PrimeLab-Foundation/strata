/**
 * @file python_dumps.cpp
 * @brief `strata.dumps` — Python objects to compact JSON text.
 *
 * A straightforward recursive walk. Escaping and float formatting come from
 * the core so there is exactly one implementation of each; what lives here is
 * everything that is specific to Python objects: type dispatch, the depth
 * ceiling, and cycle handling.
 *
 * This is the correctness-first build. The thread-local output buffer,
 * homogeneous-list fast paths and batched dict serialization described in
 * docs/bindings/SKILL.md are performance work and land with that milestone.
 */

#include "python_types.h"
#include "strata/json/json_serialize.hpp"
#include "strata/util/dtoa.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
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
            out_.append(std::to_string(value));
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
        for (Py_ssize_t index = 0; index < size; ++index) {
            if (index != 0)
                out_.push_back(',');
            if (!write(items[index]))
                return false;
        }
        out_.push_back(']');
        return true;
    }

    [[nodiscard]] bool write_mapping(PyObject* object) {
        const Frame frame(*this, object);
        if (frame.repeated())
            return frame.handle_cycle();
        if (!frame.within_depth_limit())
            return false;

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
    std::string out;
    out.reserve(kDumpsInitialCapacity);

    Serializer serializer(out);
    if (!serializer.write(object))
        return nullptr;

    if (as_bytes)
        return PyBytes_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
    return PyUnicode_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
}

} // namespace strata::bindings
