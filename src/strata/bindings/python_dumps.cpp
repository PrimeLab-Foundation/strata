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
 *
 * ## Re-entrancy: what the walk may borrow, and for how long
 *
 * The serializer runs user code at exactly four steps, all of them rare and
 * all of them named here:
 *
 *   1. a cycle placeholder's `PyErr_WarnEx` under the default
 *      `cycle_policy="warn"` (a warnings filter, a `showwarning` hook);
 *   2. `write_int`'s `PyObject_Str` on an `int` *subclass* beyond int64 --
 *      its `__str__`;
 *   3. `write_int`'s `PyObject_Str` on an **exact** `int` beyond int64 whose
 *      decimal conversion CPython 3.12+ delegates to the `_pylong` *Python*
 *      module (above roughly 10 000 digits, which
 *      `sys.set_int_max_str_digits` must permit): that step imports modules
 *      and runs bytecode, so a collection can run there too;
 *   4. the serializer's own release of a reference it took at one of those --
 *      a `__del__` or a weakref callback firing out of a `Py_DECREF` in
 *      `Frame`, `DeferredOpen` or `RowLock`.
 *
 * Everything else the walk executes runs none: it calls nothing the user
 * wrote and allocates nothing the collector tracks (the output buffer is
 * `bytes`/`std::string`, the schema blob is `std::string`, and the staged
 * rows and the schema cache are leased before the walk starts), so no
 * collection and therefore no finalizer can be triggered either. That is the
 * same fact frame elision already rests on -- which is why step 3 is not just
 * a latch: `is_plain_scalar` refuses to call a large `int` a plain scalar, so
 * a container holding one is framed and its row registered like a container
 * holding an `int` subclass. Steps 2 and 3 additionally hold a strong
 * reference to the value being converted, which the latch does not cover: the
 * `_pylong` import runs before the value is handed over and can orphan it.
 *
 * The enumeration is only true because nothing here resolves lazily. The
 * raw-dict layout proof used to: it is a function-local static whose
 * initialiser allocates two dicts, and its first use is *in the middle of the
 * walk* of a process's first dumps() of a document containing a dict, where a
 * collection triggered by that allocation ran finalizers under borrowed rows
 * and elided containers (a deterministic segfault; see
 * build/evidence/FIX1-REVIEW). `prepare_dumps_runtime()` resolves it at
 * module init instead. A lazily-resolved static, or any GC-tracked
 * allocation, or any conversion an interpreter version hands back to Python
 * (step 3 is exactly that, and was missed once) added below is a fifth step
 * and breaks this contract.
 *
 * Because that user code can mutate the very container being written, the
 * walk obeys two rules, and a change here has to keep both:
 *
 *  - **Ownership, latched.** The walk borrows; it takes strong references
 *    only when it is about to run user code. `latch()` increfs every entry of
 *    `open_` and every registered `RowLock` that is not already latched, and
 *    each reference is released when its frame or row goes out of scope.
 *    Three facts make that sufficient, and all three have to survive a change
 *    here:
 *      1. a container is on `open_` *before* any user code can run beneath it
 *         -- `Frame` pushes on entry, and the deferred push of the sequence
 *         and record loops arms at the first element `is_plain_scalar`
 *         refuses, which is the first element that can run anything (a large
 *         `int` included: see step 3 above);
 *      2. the latch is prefix-closed -- `open_` is a stack whose latched
 *         entries are a prefix from the root, and the row list's latched
 *         nodes are a suffix from the root, so latching stops at the first
 *         node already latched;
 *      3. a `Py_DECREF` of the serializer's own can only reach zero from a
 *         *latched* guard, and a latched guard implies every guard still live
 *         at that moment is latched too -- so the `__del__` it fires sees the
 *         same fully-owned walk the other two steps do.
 *    Ordinary documents run no user code and so pay no reference counting at
 *    all; a document that does pays O(depth) once per event.
 *  - **Freshness.** No borrowed pointer *into* a container's storage survives
 *    a step that can run user code. A list's `ob_item`/`ob_size` are re-read
 *    per element, so the loop follows the live list exactly as stdlib json's
 *    encoder does. A dict's row is read once and *registered* from the first
 *    non-scalar value on, so a record emits the row the serializer read --
 *    which is what makes the general and the fused writer agree byte for byte
 *    even here. Dicts too wide for that row take `PyDict_Next`, which
 *    re-validates against the dict on every call. The row itself is *leased*,
 *    one per nesting level, not a local array: see
 *    SchemaCacheLease::StagedRow for why no function on this walk may carry
 *    one in its frame.
 *
 * The schema cache is part of the same contract: it *owns* the keys it
 * remembers, so it only ever remembers exact `str` objects, whose release
 * cannot run a `__del__` or a weakref callback while another record's row is
 * staged. A dict with a `str` subclass key takes the plain walk instead.
 */

#include "python_dumps_output.h"
#include "python_types.h"
#include "strata/json/json_serialize.hpp"
#include "strata/util/dtoa.hpp"
#include "strata/util/scan.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <string_view>
#include <vector>

namespace strata::bindings {

namespace {

// Cold-path annotation: keeps rare paths out of the serializer's hot text.
// The benchmark condition interleaves five engines per round; on x86's small
// L1I every byte of hot footprint refaults per call, and these functions run
// once per schema, once per cycle, or never.
#if defined(__clang__) || defined(__GNUC__)
#define STRATA_COLD_FN __attribute__((noinline, cold))
#define STRATA_NOINLINE_HOT __attribute__((noinline))
#elif defined(_MSC_VER)
#define STRATA_COLD_FN __declspec(noinline)
#define STRATA_NOINLINE_HOT __declspec(noinline)
#else
#define STRATA_COLD_FN
#define STRATA_NOINLINE_HOT
#endif

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

/// Walks a Python object graph, appending JSON through the staged buffer.
class Serializer {
    using Schema = SchemaCacheLease::Schema;

  public:
    Serializer(StagedOutput& out, SchemaCacheLease::State& state)
        : out_(out), depth_limit_(Py_GetRecursionLimit()), schemas_(state.schemas),
          staged_rows_(state.rows), lock_nodes_(state.locks) {}

    [[nodiscard]] bool write(PyObject* object) {
        // Exact types first: one pointer compare instead of a flag load per
        // candidate, and real data is exact types essentially always.
        // Subclasses fall to the flag-check chain below, whose one semantic
        // ordering is bool before int (bool subclasses int and must never
        // print as a number).
        PyTypeObject* const type = Py_TYPE(object);
        if (type == &PyUnicode_Type)
            return write_string(object);
        if (type == &PyFloat_Type) {
            write_double(PyFloat_AS_DOUBLE(object));
            return true;
        }
        if (type == &PyLong_Type)
            return write_int(object);
        if (type == &PyDict_Type)
            return write_mapping(object);
        if (type == &PyList_Type)
            return write_sequence(object);
        if (object == Py_None) {
            out_.ensure(4);
            out_.write("null", 4);
            return true;
        }
        if (object == Py_True || object == Py_False) {
            out_.ensure(5);
            if (object == Py_True)
                out_.write("true", 4);
            else
                out_.write("false", 5);
            return true;
        }

        if (PyUnicode_Check(object))
            return write_string(object);
        if (PyFloat_Check(object)) {
            write_double(PyFloat_AS_DOUBLE(object));
            return true;
        }
        if (PyLong_Check(object)) // bools were handled above by identity
            return write_int(object);
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
#if PY_VERSION_HEX >= 0x030C0000
        // A compact int is one machine word inside the object; reading it
        // directly skips the exported conversion's call and overflow dance.
        if (PyUnstable_Long_IsCompact(reinterpret_cast<PyLongObject*>(object))) {
            const Py_ssize_t value =
                PyUnstable_Long_CompactValue(reinterpret_cast<PyLongObject*>(object));
            out_.ensure(util::kInt64BufferSize);
            out_.advance(util::format_int64(static_cast<int64_t>(value), out_.cursor()));
            return true;
        }
#endif
        int overflow = 0;
        const long long value = PyLong_AsLongLongAndOverflow(object, &overflow);
        if (overflow == 0) {
            if (value == -1 && PyErr_Occurred())
                return false;
            out_.ensure(util::kInt64BufferSize);
            out_.advance(util::format_int64(value, out_.cursor()));
            return true;
        }

        // Beyond int64 the digits come from Python itself, so nothing is lost
        // -- and this call is user code whatever the type is. For an `int`
        // *subclass* it is `__str__`; for an **exact** int on CPython 3.12+ it
        // is the `_pylong` module, which `long_to_decimal_string` imports and
        // executes above roughly 10 000 digits. Either can empty the
        // containers this walk is inside, and so can the release of a `str`
        // subclass returned by the first, so the walk takes its references
        // first -- unconditionally. The condition that used to stand here
        // ("exact ints run nothing") was false on 3.12+ and cost a
        // heap-use-after-free; is_plain_scalar keeps the other half of the
        // bargain by refusing to call such an int plain, so the container is
        // armed and the row registered before this is reached.
        //
        // The strong reference on the value itself covers the one window the
        // latch cannot. `latch()` owns the containers and the staged rows, but
        // not the entry being written -- and `long_to_decimal_string` imports
        // `_pylong` *before* it hands the value over, so a collection during
        // that import can run a `__del__` that empties the container and drops
        // the last reference to the very int being converted. Measured: with
        // the value held elsewhere the reproducer is 24/24 clean, orphaned it
        // is 4/24 SIGSEGV without this line (build/evidence/E26-FIX1/v4).
        // Both fast paths above return before here, so only big ints pay it.
        latch();
        const PyRef value_guard(Py_NewRef(object));
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
        // Copy while scanning: the clean case reads each byte once. The
        // stage is over-reserved by a block so the vectorized copy may store
        // whole blocks before checking them; on a hit nothing is advanced,
        // so the partial copy is scratch and the core escaper takes over --
        // it remains the one definition of escaping.
        // The cap bounds stage usage in str mode (the stage is 8 KB); it was
        // 512, which sent longer strings to the spanning path — measured
        // catastrophic under MSVC (600-char strings 7.5x behind orjson while
        // 200-char ones, inside the cap, ran 1.1x). Four kilobytes keeps the
        // same safety margin and keeps real-world long values on the proven
        // copy-while-scanning path.
        if (size + 2 + 16 <= StagedOutput::kMaxReservation) {
            out_.ensure(size + 2 + 16);
            out_.put('"');
            const size_t clean = util::copy_until_escape(data, size, out_.cursor());
            if (clean == size) {
                out_.advance(size);
                out_.put('"');
                return true;
            }
            // Roll back the opening quote; the escaper writes the whole
            // string, quotes included.
            out_.rewind(1);
        }

        const size_t clean = util::find_next_escape(data, size);
        if (clean == size) {
            out_.ensure(1);
            out_.put('"');
            out_.write_spanning(data, size);
            out_.ensure(1);
            out_.put('"');
            return true;
        }
        std::string& sink = out_.direct_sink();
        if (!out_.sink_is_scratch()) {
            append_escaped_json_string(std::string_view(data, size), sink);
            return true;
        }
        append_escaped_json_string(std::string_view(data, size), sink);
        out_.write_spanning(sink.data(), sink.size());
        return true;
    }

    [[nodiscard]] bool write_sequence(PyObject* object) {
        // The frame answers two questions -- "is this list already open above
        // me?" and "is the nesting too deep?" -- and only descending into a
        // container child can make either matter for the *next* level. So the
        // cycle probe runs up front, the boundary case keeps the framed path
        // (its depth error must stay byte-identical), and the push is deferred
        // until the first element that is not an exact scalar -- which is also
        // the first element that can run user code, so this list is on open_
        // whenever `latch()` could need it. All-scalar lists -- the
        // overwhelming majority -- never touch the open_ vector at all.
        if (std::find(open_.begin(), open_.end(), object) != open_.end())
            return emit_cycle_placeholder();
        if (open_.size() >= static_cast<size_t>(depth_limit_)) {
            const Frame frame(*this, object);
            if (frame.repeated())
                return frame.handle_cycle();
            if (!frame.within_depth_limit())
                return false;
            return write_sequence_body(object);
        }

        out_.ensure(1);
        out_.put('[');
        // A list's ob_item and ob_size are re-read per element instead of
        // being hoisted: user code running below this loop can resize the
        // list, which moves the item array out from under a hoisted pointer
        // (that read freed memory, and crashed on a reallocation). stdlib
        // json's encoder re-reads for the same reason, and this loop follows
        // the list the same way -- a shrunk list ends here, an appended
        // element is written. A tuple cannot change, so it is read once.
        const bool from_list = PyList_Check(object) != 0;
        Py_ssize_t size = PySequence_Fast_GET_SIZE(object);
        PyObject** items = PySequence_Fast_ITEMS(object);

        // Arrays of one scalar type are the common shape in real payloads, and
        // for them the per-element dispatch through write() is pure overhead.
        // Each run formats straight into the stage; the element type is
        // re-checked per element, so a mixed list just ends the run early and
        // the general loop below picks up exactly where it stopped. The runs
        // handle exact scalars only, so none of them can run user code and the
        // snapshot they are handed stays valid for their whole length.
        Py_ssize_t index = write_scalar_run(items, size);

        DeferredOpen open_container(*this);
        for (; index < size; ++index) {
            if (index != 0) {
                out_.ensure(1);
                out_.put(',');
            }
            PyObject* const item = items[index];
            if (!open_container.armed() && !is_plain_scalar(item))
                open_container.arm(object);
            // Array-of-records is the shape the certified rows are made of
            // (docs/architecture/fused_record_writer.md): exact dicts take
            // the one-pass emit, everything else the general dispatch.
            const bool ok = Py_TYPE(item) == &PyDict_Type ? write_record_fused(item) : write(item);
            if (!ok)
                return false;
            if (from_list) {
                items = reinterpret_cast<PyListObject*>(object)->ob_item;
                size = Py_SIZE(object);
            }
        }
        out_.ensure(1);
        out_.put(']');
        return true;
    }

    /// The framed emit loop, for sequences at the depth boundary.
    [[nodiscard]] bool write_sequence_body(PyObject* object) {
        out_.ensure(1);
        out_.put('[');
        Py_ssize_t size = PySequence_Fast_GET_SIZE(object);
        PyObject** items = PySequence_Fast_ITEMS(object);
        Py_ssize_t index = write_scalar_run(items, size);
        for (; index < size; ++index) {
            if (index != 0) {
                out_.ensure(1);
                out_.put(',');
            }
            if (!write(items[index]))
                return false;
            // Re-read for the reason the main loop does: the Frame keeps the
            // sequence alive, but user code below can still have moved its
            // items. This path is the depth boundary, so it pays the macro's
            // type test rather than hoisting it.
            size = PySequence_Fast_GET_SIZE(object);
            items = PySequence_Fast_ITEMS(object);
        }
        out_.ensure(1);
        out_.put(']');
        return true;
    }

    /// Counts dict nesting for the schema cache, framed or not.
    class MappingDepth {
      public:
        explicit MappingDepth(size_t& counter) : counter_(counter) { ++counter_; }
        ~MappingDepth() { --counter_; }
        MappingDepth(const MappingDepth&) = delete;
        MappingDepth& operator=(const MappingDepth&) = delete;

      private:
        size_t& counter_;
    };

    /// The cycle placeholder the policy calls for, outside any frame.
    [[nodiscard]] STRATA_COLD_FN bool emit_cycle_placeholder() {
        if (g_cycle_policy == CyclePolicyValue::Error) {
            PyErr_SetString(PyExc_ValueError, "Circular reference detected");
            return false;
        }
        out_.ensure(4);
        out_.write("null", 4);
        if (g_cycle_policy == CyclePolicyValue::Warn) {
            // The warning runs a user handler, which can empty the containers
            // this walk is inside: take the references before it runs.
            // A warning filter set to "error" raises here, which stops the
            // serialization rather than being swallowed.
            latch();
            return PyErr_WarnEx(PyExc_RuntimeWarning, "Circular reference detected", 1) == 0;
        }
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
        if (items[0] == Py_None)
            return run_nones(items, size);
        return 0;
    }

    /// Elements per covering reserve in the scalar runs: one capacity branch
    /// per block instead of one per element, at a worst-case cost of a few
    /// kilobytes of slack in the stage.
    static constexpr Py_ssize_t kScalarRunBlock = 64;
    static_assert(kScalarRunBlock * (util::kDoubleBufferSize + 1) <= StagedOutput::kMaxReservation,
                  "a scalar-run block must fit one reservation");

    [[nodiscard]] Py_ssize_t run_floats(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        while (index < size) {
            const Py_ssize_t block_end = std::min<Py_ssize_t>(size, index + kScalarRunBlock);
            out_.ensure(static_cast<size_t>(block_end - index) * (util::kDoubleBufferSize + 1));
            for (; index < block_end; ++index) {
                PyObject* item = items[index];
                if (Py_TYPE(item) != &PyFloat_Type)
                    return index;
                // Fused separator, as in the int run: the comma is stored
                // unconditionally and the payload lands one byte past it,
                // overwriting it for the very first element.
                char* cursor = out_.cursor();
                *cursor = ',';
                const auto skip = static_cast<size_t>(index != 0);
                const double value = PyFloat_AS_DOUBLE(item);
                if (!std::isfinite(value)) { // NaN or ±Inf: JSON cannot spell either
                    std::memcpy(cursor + skip, "null", 4);
                    out_.advance(skip + 4);
                } else {
                    out_.advance(
                        skip + util::format_double(value, cursor + skip, util::kDoubleBufferSize));
                }
            }
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_ints(PyObject** items, Py_ssize_t size) {
        Py_ssize_t index = 0;
        while (index < size) {
            const Py_ssize_t block_end = std::min<Py_ssize_t>(size, index + kScalarRunBlock);
            out_.ensure(static_cast<size_t>(block_end - index) * (util::kInt64BufferSize + 1));
            for (; index < block_end; ++index) {
                PyObject* item = items[index];
                // Not PyLong_Check: bool is a subclass and must not print as a
                // number, and an int subclass may override __repr__.
                if (Py_TYPE(item) != &PyLong_Type)
                    return index;
                int64_t value;
#if PY_VERSION_HEX >= 0x030C0000
                if (PyUnstable_Long_IsCompact(reinterpret_cast<PyLongObject*>(item))) {
                    value = static_cast<int64_t>(
                        PyUnstable_Long_CompactValue(reinterpret_cast<PyLongObject*>(item)));
                } else
#endif
                {
                    int overflow = 0;
                    const long long wide = PyLong_AsLongLongAndOverflow(item, &overflow);
                    if (overflow != 0)
                        return index; // the general path renders big ints via their str
                    if (wide == -1 && PyErr_Occurred()) {
                        PyErr_Clear();
                        return index;
                    }
                    value = static_cast<int64_t>(wide);
                }
                // Fused separator: the comma is stored unconditionally and the
                // digits land one byte past it — except for the very first
                // element, whose digits simply overwrite the comma. One store
                // and one size update per element, no separator branch.
                char* cursor = out_.cursor();
                *cursor = ',';
                const auto skip = static_cast<size_t>(index != 0);
                out_.advance(skip + util::format_int64(value, cursor + skip));
            }
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_bools(PyObject** items, Py_ssize_t size) {
        if (size == 0 || (items[0] != Py_True && items[0] != Py_False))
            return 0;
        // First element bare, the rest as one eight-byte constant store each
        // (",true" / ",false" plus slack the next write overwrites). The
        // store is *selected, not branched*: real flag data is close to
        // random, so a taken/not-taken branch per element mispredicts about
        // half the time — the two literal words sit in registers and a
        // conditional move picks one.
        uint64_t true_word;
        uint64_t false_word;
        std::memcpy(&true_word, ",true\0\0", 8);
        std::memcpy(&false_word, ",false\0", 8);
        out_.ensure(8);
        if (items[0] == Py_True) {
            out_.write("true", 4);
        } else {
            out_.write("false", 5);
        }
        Py_ssize_t index = 1;
        while (index < size) {
            // Eight bytes per element covers the widest store window: the
            // last element's 8-byte store reaches at most (block-1)*6+8
            // bytes past the reserve base, which 8*block always contains.
            const Py_ssize_t block_end = std::min<Py_ssize_t>(size, index + kScalarRunBlock);
            out_.ensure(static_cast<size_t>(block_end - index) * 8);
            for (; index < block_end; ++index) {
                PyObject* item = items[index];
                const bool truth = item == Py_True;
                if (!truth && item != Py_False)
                    return index;
                const uint64_t word = truth ? true_word : false_word;
                std::memcpy(out_.cursor(), &word, 8);
                out_.advance(6 - static_cast<size_t>(truth));
            }
        }
        return index;
    }

    [[nodiscard]] Py_ssize_t run_nones(PyObject** items, Py_ssize_t size) {
        // First element bare, then one eight-byte constant store per element
        // (",null" plus slack the next write overwrites). Same store-window
        // proof as the bool run: the last store reaches (block-1)*5+8 bytes
        // past the reserve base, which 8*block always contains.
        out_.ensure(8);
        out_.write("null", 4);
        Py_ssize_t index = 1;
        while (index < size) {
            const Py_ssize_t block_end = std::min<Py_ssize_t>(size, index + kScalarRunBlock);
            out_.ensure(static_cast<size_t>(block_end - index) * 8);
            for (; index < block_end; ++index) {
                if (items[index] != Py_None)
                    return index;
                std::memcpy(out_.cursor(), ",null\0\0", 8);
                out_.advance(5);
            }
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
            if (length + 3 + 16 > StagedOutput::kMaxReservation)
                break;
            out_.ensure(length + 3 + 16); // block-rounded room for the fused copy
            const size_t before = index != 0 ? 2u : 1u;
            // Separator and opening quote as one store.
            if (index != 0) {
                std::memcpy(out_.cursor(), ",\"", 2);
                out_.advance(2);
            } else {
                out_.put('"');
            }
            if (util::copy_until_escape(data, length, out_.cursor()) != length) {
                // An escape: undo the separator and quote, end the run, and
                // let the general path (and the one escaper) handle this one.
                out_.rewind(before);
                break;
            }
            out_.advance(length);
            out_.put('"');
        }
        return index;
    }

    /// Objects with more keys than this serialize their keys the plain way.
    static constexpr Py_ssize_t kMaxSchemaKeys = 24;
    static_assert(kMaxSchemaKeys <= 255, "RowLock packs its row bounds into bytes");
    /// Nesting levels that get a schema slot; deeper objects take the plain walk.
    static constexpr size_t kMaxCachedDepth = SchemaCacheLease::kMaxDepth;
    static_assert(static_cast<size_t>(kMaxSchemaKeys) == SchemaCacheLease::kSchemaSlots,
                  "the staged rows and the schema slot rows are the same width");

    /**
     * True while an exact `int` prints without running Python.
     *
     * CPython 3.12+ hands `long_to_decimal_string` a conversion above roughly
     * 10 000 decimal digits to the **`_pylong` Python module** -- it imports
     * four modules and runs bytecode, so such an int is a user-code step like
     * any other (`sys.set_int_max_str_digits` is what lets the conversion be
     * attempted at all). The test used here is the interpreter's own
     * compactness bound -- one digit, |v| < 2^30 -- which is public API, far
     * below the `_pylong` threshold, and one tag load. Earlier versions have
     * no `_pylong`, but the same bound is applied there from the digit count,
     * so the classifier's answer does not depend on the interpreter version.
     */
    [[nodiscard]] static bool is_compact_int(PyObject* value) noexcept {
#if PY_VERSION_HEX >= 0x030C0000
        return PyUnstable_Long_IsCompact(reinterpret_cast<PyLongObject*>(value)) != 0;
#else
        const Py_ssize_t digits = Py_SIZE(value);
        return digits >= -1 && digits <= 1;
#endif
    }

    /// Exact scalar types cannot recurse, so a dict of them cannot contain
    /// itself and needs no cycle frame -- and, just as load-bearing, none of
    /// them can run user code, so the row staged for such a dict is emitted
    /// without a frame or a registration at all. A large `int` is therefore
    /// *not* plain: its decimal conversion runs Python (is_compact_int), and
    /// a value that can run Python has to arm the container and register the
    /// row before it is written.
    [[nodiscard]] static bool is_plain_scalar(PyObject* value) noexcept {
        PyTypeObject* type = Py_TYPE(value);
        if (type == &PyUnicode_Type || type == &PyFloat_Type)
            return true;
        if (type == &PyLong_Type)
            return is_compact_int(value);
        return type == &PyBool_Type || value == Py_None;
    }

    /**
     * One-pass emit for a record inside an array-of-records
     * (docs/architecture/fused_record_writer.md): the entry array is walked
     * once, the schema way resolves from the first key, keys emit from the
     * inline slot row and values dispatch as visited — no second walk of the
     * dict, and the only staging is the stack copy of the value pointers the
     * verification pass already loads. Verification of the whole key row
     * completes before any
     * output byte, so every deviation — split table, holes, width, way miss,
     * unprepared or wide schema, retired depth, the depth boundary — falls
     * back to write_mapping with nothing to undo. The general path stays the
     * single definition of behavior; a miss here also lets it remember the
     * shape, which is what makes the next sibling hit.
     *
     * Out of line, like write_mapping_body and for the same reason: a
     * document with no array-of-records — `flat`, `wide_arrays` — must not
     * fetch this body at all, and write_sequence must stay the small loop
     * every array in every document runs (inlined here it costs write_sequence
     * +545 instructions and +80 frame bytes under the profile). What it no
     * longer carries either way is a staging array: the row is the leased row
     * of this nesting level (SchemaCacheLease::StagedRow), so neither this
     * function nor any function it is folded into declares one.
     */
    [[nodiscard]] STRATA_NOINLINE_HOT bool write_record_fused(PyObject* object) {
#if defined(STRATA_RAW_DICT_WALK)
        if (!rawdict::available())
            return write_mapping(object);
        Py_ssize_t entry_count = 0;
        const rawdict::UnicodeEntry* entries = rawdict::entry_array(object, &entry_count);
        if (entries == nullptr)
            return write_mapping(object);
        const Py_ssize_t size = PyDict_GET_SIZE(object);
        if (size == 0 || size > kMaxSchemaKeys || entry_count != size)
            return write_mapping(object);
        const size_t depth = map_depth_ + 1; // what MappingDepth will make it
        if (depth >= kMaxCachedDepth || open_.size() >= static_cast<size_t>(depth_limit_))
            return write_mapping(object);
        if (schemas_.size() <= depth)
            schemas_.resize(depth + 1);
        auto& depth_schemas = schemas_[depth];
        if (depth_schemas.retired || entries[0].me_value == nullptr)
            return write_mapping(object);

        PyObject* const first_key = entries[0].me_key;
        size_t way = SchemaCacheLease::DepthSchemas::kMiss;
        for (size_t candidate = 0; candidate < SchemaCacheLease::DepthSchemas::kWays; ++candidate) {
            if (depth_schemas.counts[candidate] == size &&
                depth_schemas.first_keys[candidate] == first_key) {
                way = candidate;
                break;
            }
        }
        if (way == SchemaCacheLease::DepthSchemas::kMiss)
            return write_mapping(object);
        Schema& schema = depth_schemas.ways[way];
        if (!schema.prepared || schema.wide)
            return write_mapping(object);
        // The verification pass keeps the value pointers it already loads to
        // check each slot is occupied. The emit loop then reads this level's
        // staged row instead of re-striding the entry array -- and,
        // load-bearing, never touches `entries` again: user code below a value
        // can resize the dict and free that table. Still one walk of the entry
        // array and no second walk of the dict; the row is a copy of pointers
        // this pass has in hand, in storage the call leases rather than in
        // this frame (docs/architecture/fused_record_writer.md).
        PyObject** const row = staged_row(depth).values;
        for (Py_ssize_t index = 0; index < size; ++index) {
            PyObject* const value = entries[index].me_value;
            if (value == nullptr ||
                schema.key_row[static_cast<size_t>(index)] != entries[index].me_key)
                return write_mapping(object);
            row[static_cast<size_t>(index)] = value;
        }

        const MappingDepth level(map_depth_);
        out_.ensure(1);
        out_.put('{');
        DeferredOpen open_container(*this);
        RowLock values_lock;
        for (Py_ssize_t index = 0; index < size; ++index) {
            // Re-indexed per iteration: a nested object may grow schemas_
            // and move its elements (the write_mapping_body hazard).
            const Schema& slot_row = schemas_[depth].ways[way];
            out_.ensure(17);
            char* cursor = out_.cursor();
            *cursor = ',';
            const auto skip = static_cast<size_t>(index != 0);
            std::memcpy(cursor + skip,
                        slot_row.slots + static_cast<size_t>(index) * SchemaCacheLease::kSlotBytes,
                        SchemaCacheLease::kSlotBytes);
            out_.advance(skip + slot_row.spans[static_cast<size_t>(index)]);
            PyObject* const value = row[static_cast<size_t>(index)];
            if (!open_container.armed() && !is_plain_scalar(value)) {
                // The one moment two things become true at once: this dict
                // must be findable in open_ while a container child walks, and
                // the row behind it stops being safe to borrow, because the
                // child can run user code that empties the dict. Same deferred
                // frame as the sequence loop, same row registration as
                // write_mapping_body -- so both emit the same record here.
                open_container.arm(object);
                values_lock.own(*this, lock_node(depth, 0), row, index + 1, size);
            }
            if (!write(value))
                return false;
        }
        out_.ensure(1);
        out_.put('}');
        return true;
#else
        return write_mapping(object);
#endif
    }

    [[nodiscard]] bool write_mapping(PyObject* object) {
        // The schema cache is keyed by *dict nesting level*, counted here
        // explicitly. open_.size() cannot be the key: frame elision keeps
        // all-scalar dicts off that stack, so a child could share its
        // parent's depth — and a same-depth select() reorders the ways the
        // parent is mid-way through emitting from. That was a latent
        // silent-corruption bug even with one way per depth (a rebuilt blob
        // under the parent's offsets); the four-way swap turned it into a
        // crash, which is how it was found.
        const MappingDepth level(map_depth_);

        // One walk collects everything the decisions below need: the keys and
        // values, whether every value is a plain scalar, and whether the
        // object is too wide for the schema cache. The row it stages is this
        // level's leased one, not a local array -- see
        // SchemaCacheLease::StagedRow for why that difference is hot.
        SchemaCacheLease::StagedRow& staged = staged_row(map_depth_);
        PyObject** const keys = staged.keys;
        PyObject** const values = staged.values;
        Py_ssize_t count = 0;
        bool too_many = false;
        bool all_scalar = true;
        // The schema cache *owns* the keys it remembers, and releasing a `str`
        // subclass can run a `__del__` or a weakref callback -- while another
        // record's row is staged, which would be exactly the hazard the row
        // lock below exists to close. Such keys keep out of the cache: the
        // plain walk emits identical bytes.
        bool exact_keys = true;
        /// Index of the first value that can run user code; only read when
        /// `all_scalar` is false, in which case the walk below set it.
        Py_ssize_t first_container = 0;

#if defined(STRATA_RAW_DICT_WALK)
        Py_ssize_t entry_count = 0;
        const rawdict::UnicodeEntry* entries =
            rawdict::available() ? rawdict::entry_array(object, &entry_count) : nullptr;
        if (entries != nullptr) {
            for (Py_ssize_t index = 0; index < entry_count; ++index) {
                PyObject* value = entries[index].me_value;
                if (value == nullptr)
                    continue; // deleted slot
                PyObject* key = entries[index].me_key;
                if (Py_TYPE(key) != &PyUnicode_Type) {
                    if (!PyUnicode_Check(key)) {
                        PyErr_Format(PyExc_TypeError, "keys must be str, not %s",
                                     Py_TYPE(key)->tp_name);
                        return false;
                    }
                    exact_keys = false;
                }
                if (count == kMaxSchemaKeys) {
                    too_many = true;
                    break;
                }
                if (all_scalar && !is_plain_scalar(value)) {
                    all_scalar = false;
                    first_container = count;
                }
                keys[count] = key;
                values[count] = value;
                ++count;
            }
            if (!too_many && count != PyDict_GET_SIZE(object)) {
                // The table disagreed with the size — walk the safe way.
                count = 0;
                all_scalar = true;
                exact_keys = true;
                first_container = 0;
                entries = nullptr;
            }
        }
        if (entries == nullptr)
#endif
        {
            Py_ssize_t position = 0;
            PyObject* key = nullptr;
            PyObject* value = nullptr;
            while (PyDict_Next(object, &position, &key, &value)) {
                if (Py_TYPE(key) != &PyUnicode_Type) {
                    if (!PyUnicode_Check(key)) {
                        PyErr_Format(PyExc_TypeError, "keys must be str, not %s",
                                     Py_TYPE(key)->tp_name);
                        return false;
                    }
                    exact_keys = false;
                }
                if (count == kMaxSchemaKeys) {
                    too_many = true;
                    break;
                }
                if (all_scalar && !is_plain_scalar(value)) {
                    all_scalar = false;
                    first_container = count;
                }
                keys[count] = key;
                values[count] = value;
                ++count;
            }
        }
        // Neither walk above can run user code, so the row it staged is
        // whole; from here on it is only borrowed, and write_mapping_body
        // registers the part of it that outlives a step which can run user
        // code.
        const bool cacheable = !too_many && exact_keys;
        const Py_ssize_t own_from = all_scalar ? count : first_container + 1;

        // The cycle frame exists to recognise this dict if it reappears below
        // itself -- which can only happen through a container value. A dict
        // of plain scalars cannot recurse, so the all-scalar majority of
        // record data skips the frame entirely. The depth guard keeps the
        // depth-limit contract byte-identical: at the boundary the framed
        // path runs and raises exactly as before.
        if (cacheable && count > 0 && all_scalar &&
            open_.size() < static_cast<size_t>(depth_limit_))
            return write_mapping_body(object, keys, values, count, own_from);

        const Frame frame(*this, object);
        if (frame.repeated())
            return frame.handle_cycle();
        if (!frame.within_depth_limit())
            return false;

        if (!cacheable)
            return write_mapping_uncached(object);

        // An empty object has no keys to prepare, and letting it reach the
        // schema path would have it "match" a never-built cache.
        if (count == 0) {
            out_.ensure(2);
            out_.write("{}", 2);
            return true;
        }
        return write_mapping_body(object, keys, values, count, own_from);
    }

    // One out-of-line copy, deliberately. It was force-inlined into both call
    // sites (measured worth ~9 ns per object hot on shallow-dict documents),
    // but the cold-state probe (docs/decisions.md, 2026-09-03) showed the
    // x86 legs pay for the serializer's *entry footprint*: entering dumps cold
    // on one record costs 18.6 µs on Windows against orjson's 10.6, and the
    // interleaved harness hands every call a cold cache. Two inlined copies
    // of this body plus the fused writer's own loop were three copies of the
    // key-emit machinery in hot text; this keeps one.
    ///
    /// @param own_from The first index of the staged row that outlives a step
    ///        that can run user code: the index just past the first value that
    ///        can run any, or @p count for an all-scalar record, whose empty
    ///        range registers nothing.
    [[nodiscard]] STRATA_NOINLINE_HOT bool
    write_mapping_body(PyObject* object, PyObject* const* keys, PyObject* const* values,
                       Py_ssize_t count, Py_ssize_t own_from) {
        // Documents are overwhelmingly made of records that share a schema, so
        // the same keys get escape-scanned and quoted once per record.
        // Remembering the previous object's keys turns all of that into one
        // memcpy per key. The remembered bytes are produced by the very same
        // escaper, so this is a cache, not a second format.

        // One cache slot per nesting depth. A single slot thrashes on real
        // documents: a user holds orders which hold items, so by the time the
        // walk returns to the next user the slot describes an item. Keyed by
        // depth, each level keeps its own schema and sibling records — which
        // is where the repetition actually is — hit every time.
        const size_t depth = map_depth_;
        if (depth >= kMaxCachedDepth)
            return write_mapping_uncached(object);

        // From the first value that can run user code on, the staged row is
        // registered: that code can empty the dict these pointers are borrowed
        // from, and everything after it is still to be emitted. Everything
        // before it is already written, so what reaches the output is the row
        // the serializer read -- which is the row the fused writer emits too.
        RowLock values_lock(*this, lock_node(depth, 0), values, own_from, count);
        if (schemas_.size() <= depth)
            schemas_.resize(depth + 1);

        // Preparing the bytes costs about what writing them costs, so a schema
        // seen once would pay for a cache it never uses — measurably so on
        // documents of one-off shapes. A miss therefore only *remembers the
        // keys* (a pointer copy); the bytes are prepared on the second
        // sighting, when repetition is established. Four ways per depth
        // absorb documents that rotate several record shapes; true churn
        // retires the depth.
        bool prepared = false;
        const size_t way = schemas_[depth].select(keys, count);
        if (way != SchemaCacheLease::DepthSchemas::kMiss) {
            Schema& selected = schemas_[depth].ways[way];
            if (!selected.prepared && !build_schema(selected))
                return false;
            prepared = true;
        }

        out_.ensure(1);
        out_.put('{');
        if (prepared && !schemas_[depth].ways[way].wide) {
            for (Py_ssize_t index = 0; index < count; ++index) {
                // `"key":` — quotes, escapes and colon, prepared once, read
                // from the schema's *inline* slot row: fused comma plus one
                // fixed 16-byte copy at a fixed stride, so the whole key
                // stream for a record is one contiguous struct. That keeps
                // the emit warm under the harness's per-call gc.collect(),
                // whose traversal re-warms every dict's own internals while
                // evicting heap side-structures (a blob/offsets layout paid
                // cold hops per record under exactly that condition).
                //
                // Re-indexed every iteration rather than held by reference: a
                // nested object may grow `schemas_` and move its elements.
                const Schema& schema = schemas_[depth].ways[way];
                out_.ensure(17);
                char* cursor = out_.cursor();
                *cursor = ',';
                const auto skip = static_cast<size_t>(index != 0);
                std::memcpy(cursor + skip,
                            schema.slots +
                                static_cast<size_t>(index) * SchemaCacheLease::kSlotBytes,
                            SchemaCacheLease::kSlotBytes);
                out_.advance(skip + schema.spans[static_cast<size_t>(index)]);
                if (!write(values[index]))
                    return false;
            }
        } else if (prepared) {
            // Some span exceeded its slot: emit every key from the blob.
            for (Py_ssize_t index = 0; index < count; ++index) {
                const Schema& schema = schemas_[depth].ways[way];
                const uint32_t offset = schema.offsets[static_cast<size_t>(index)];
                const uint32_t span = schema.offsets[static_cast<size_t>(index) + 1] - offset;
                if (index != 0) {
                    out_.ensure(1);
                    out_.put(',');
                }
                out_.write_spanning(schema.blob.data() + offset, span);
                if (!write(values[index]))
                    return false;
            }
        } else {
            // The only branch that reads a staged *key* after the first value
            // has been written, so the only one that has to register them too.
            RowLock keys_lock(*this, lock_node(depth, 1), keys, own_from, count);
            for (Py_ssize_t index = 0; index < count; ++index) {
                if (index != 0) {
                    out_.ensure(1);
                    out_.put(',');
                }
                if (!write_string(keys[index]))
                    return false;
                out_.ensure(1);
                out_.put(':');
                if (!write(values[index]))
                    return false;
            }
        }
        out_.ensure(1);
        out_.put('}');
        return true;
    }

    /// Prepare the `"key":` bytes for a schema whose keys are already recorded.
    [[nodiscard]] STRATA_COLD_FN bool build_schema(Schema& schema) {
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
        // Inline each span into its fixed slot; one oversized span keeps the
        // whole schema on the blob fallback (schema.wide).
        for (size_t index = 0; index + 1 < schema.offsets.size(); ++index) {
            const uint32_t span = schema.offsets[index + 1] - schema.offsets[index];
            if (span > SchemaCacheLease::kSlotBytes) {
                schema.wide = true;
                break;
            }
            schema.spans[index] = static_cast<uint8_t>(span);
            std::memcpy(schema.slots + index * SchemaCacheLease::kSlotBytes,
                        schema.blob.data() + schema.offsets[index], span);
        }
        schema.prepared = true;
        return true;
    }

    /// The plain walk, for objects too wide to be worth remembering (and for
    /// the ones whose keys must stay out of the schema cache). It stages
    /// nothing: `PyDict_Next` re-validates its position against the dict on
    /// every call, so this loop follows a mutated dict without ever reading
    /// freed memory -- its caller's Frame is what keeps the dict itself alive.
    [[nodiscard]] STRATA_COLD_FN bool write_mapping_uncached(PyObject* object) {
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
        Frame(Serializer& owner, PyObject* container) : owner_(owner), container_(container) {
            const auto& open = owner_.open_;
            repeated_ = std::find(open.begin(), open.end(), container) != open.end();
            // Pushed, not increfed: `latch()` is what makes every entry of
            // open_ strong, and it runs before any user code that could drop
            // the container out of its parent's slot. That is also what makes
            // every pointer this scan compares safe to compare against.
            if (!repeated_)
                owner_.open_.push_back(container);
        }

        ~Frame() {
            if (!repeated_)
                owner_.close_container(container_);
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
        [[nodiscard]] STRATA_COLD_FN bool handle_cycle() const {
            if (g_cycle_policy == CyclePolicyValue::Error) {
                PyErr_SetString(PyExc_ValueError, "Circular reference detected");
                return false;
            }
            owner_.out_.ensure(4);
            owner_.out_.write("null", 4);
            if (g_cycle_policy == CyclePolicyValue::Warn) {
                // The warning runs a user handler, which can empty the
                // containers this walk is inside -- including the one this
                // frame found repeated, which this frame deliberately does not
                // push and whose only keeper is the ancestor that did.
                // A warning filter set to "error" raises here, which stops the
                // serialization rather than being swallowed.
                owner_.latch();
                return PyErr_WarnEx(PyExc_RuntimeWarning, "Circular reference detected", 1) == 0;
            }
            return true;
        }

      private:
        Serializer& owner_;
        PyObject* container_;
        bool repeated_ = false;
    };

    /**
     * The deferred frame of the sequence and record loops: the container goes
     * on `open_` the first time the loop is about to walk a child that could
     * run user code -- which is exactly the first element that is not an exact
     * scalar. All-scalar containers never arm it and pay nothing, which is
     * what makes frame elision free.
     */
    class DeferredOpen {
      public:
        explicit DeferredOpen(Serializer& owner) noexcept : owner_(owner) {}

        ~DeferredOpen() {
            if (container_ != nullptr)
                owner_.close_container(container_);
        }

        DeferredOpen(const DeferredOpen&) = delete;
        DeferredOpen& operator=(const DeferredOpen&) = delete;

        void arm(PyObject* container) {
            owner_.open_.push_back(container);
            container_ = container;
        }

        [[nodiscard]] bool armed() const noexcept { return container_ != nullptr; }

      private:
        Serializer& owner_;
        PyObject* container_ = nullptr;
    };

    /**
     * A staged row of borrowed pointers, registered so `latch()` can find it.
     *
     * A row read out of a dict is only borrowed for as long as the dict holds
     * it, and user code running under a container value can empty the dict.
     * From that value on the row registers itself on the serializer's row
     * stack -- three stores, no reference counting -- and pays a reference per
     * entry only if user code actually runs. An all-scalar record's range is
     * empty and registers nothing at all.
     */
    class RowLock {
      public:
        RowLock() noexcept = default;
        RowLock(Serializer& owner, SchemaCacheLease::RowNode& node, PyObject* const* row,
                Py_ssize_t first, Py_ssize_t last) noexcept {
            own(owner, node, row, first, last);
        }

        ~RowLock() {
            if (owner_ == nullptr)
                return;
            // Always the head: registrations nest with the frames that make
            // them, so the innermost is the first destroyed. A row registered
            // outside that discipline would unlink the wrong node.
            owner_->rows_ = node_->next;
            if (!node_->latched)
                return;
            for (unsigned index = node_->first; index < node_->last; ++index)
                Py_DECREF(node_->row[index]);
        }

        RowLock(const RowLock&) = delete;
        RowLock& operator=(const RowLock&) = delete;

        /// Register `row[first .. last)` on @p node. An empty range
        /// registers nothing: there is no borrowed pointer left for user code
        /// to invalidate. Called at most once per object -- both call sites
        /// are guarded by the same condition that arms the container's
        /// deferred push -- and the node is this level's, so no two live
        /// registrations can share one.
        void own(Serializer& owner, SchemaCacheLease::RowNode& node, PyObject* const* row,
                 Py_ssize_t first, Py_ssize_t last) noexcept {
            if (first >= last)
                return;
            node.row = row;
            // Both are indices into a row of at most kMaxSchemaKeys entries,
            // so a byte holds either.
            node.first = static_cast<uint8_t>(first);
            node.last = static_cast<uint8_t>(last);
            // The node is reused by the next record at this level: a stale
            // flag would release references this row never took.
            node.latched = false;
            node.next = owner.rows_;
            owner_ = &owner;
            node_ = &node;
            owner.rows_ = &node;
        }

      private:
        Serializer* owner_ = nullptr;
        SchemaCacheLease::RowNode* node_ = nullptr;
    };

    /**
     * Make every borrowed pointer the walk still needs a strong one.
     *
     * Called immediately before each of the two steps that run user code, and
     * so also before any `__del__` the serializer's own releases can fire (see
     * the rule at the top of this file). Latched entries stay latched until
     * their frame or row goes out of scope, so a second event only pays for
     * what has been opened since the first: `open_`'s latched entries are a
     * prefix from the root and the row list's are a suffix from the root,
     * which is why both loops may stop at the first one already latched.
     */
    STRATA_COLD_FN void latch() noexcept {
        while (owned_ < open_.size()) {
            Py_INCREF(open_[owned_]);
            ++owned_;
        }
        for (SchemaCacheLease::RowNode* row = rows_; row != nullptr && !row->latched;
             row = row->next) {
            for (unsigned index = row->first; index < row->last; ++index)
                Py_INCREF(row->row[index]);
            row->latched = true;
        }
    }

    /// Pop a container off `open_`, releasing it if the walk had latched it.
    void close_container(PyObject* container) {
        open_.pop_back();
        if (owned_ > open_.size()) {
            owned_ = open_.size();
            Py_DECREF(container);
        }
    }

    /// The staged row of dict nesting level @p depth. Levels past the schema
    /// cache share the one scratch row at `kMaxCachedDepth`, which nothing
    /// ever reads back -- see SchemaCacheLease::State.
    [[nodiscard]] SchemaCacheLease::StagedRow& staged_row(size_t depth) const noexcept {
        return staged_rows_[depth < kMaxCachedDepth ? depth : kMaxCachedDepth];
    }

    /// The registration node of one of a level's two staged rows. Only levels
    /// inside the schema cache register: both call sites have already sent a
    /// deeper level to write_mapping_uncached, which stages nothing.
    [[nodiscard]] SchemaCacheLease::RowNode& lock_node(size_t depth, size_t which) const noexcept {
        return lock_nodes_[2 * depth + which];
    }

    StagedOutput& out_;
    std::vector<PyObject*> open_;
    /// `open_[0 … owned_)` hold a strong reference; the rest are borrowed.
    size_t owned_ = 0;
    /// Innermost registered staged row; see RowLock.
    SchemaCacheLease::RowNode* rows_ = nullptr;
    size_t map_depth_ = 0; ///< dict nesting, independent of frame elision
    int depth_limit_;

    // One prepared schema per nesting depth; leased, so it survives the call.
    std::vector<SchemaCacheLease::DepthSchemas>& schemas_;
    /// One staged key/value row per nesting depth, leased from the same
    /// state. Leased rather than declared here or in the dict writers'
    /// frames: a stack row is a stack row in every function the profile
    /// inlines them into (SchemaCacheLease::StagedRow).
    SchemaCacheLease::StagedRow* staged_rows_;
    /// One registration node per staged row, leased for the same reason.
    SchemaCacheLease::RowNode* lock_nodes_;
};

} // namespace

void prepare_dumps_runtime() noexcept {
#if defined(STRATA_RAW_DICT_WALK)
    // The layout proof builds two dicts, and a dict is GC-tracked: allocating
    // it can run a collection, and a collection runs __del__ and weakref
    // callbacks -- user code. Inside a walk that would be a fourth user-code
    // step, at a point that takes no latch and beneath rows and elided
    // containers nothing protects. Resolving it here, once, before any walk,
    // is what makes this file's three-step enumeration true as written.
    if (!rawdict::available())
        PyErr_Clear(); // a refused layout is not an import failure
#endif
}

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
    SchemaCacheLease lease;
    if (!lease.ok())
        return PyErr_NoMemory();

    if (as_bytes) {
        // Sized to the previous document on this thread, exactly: growth
        // doubling would otherwise copy roughly one document's worth of
        // bytes on the way up, cancelling the copy this mode exists to
        // save, and a repeated document -- the workload that matters --
        // then allocates once, needs no resize, and asks the allocator for
        // the very block the previous call freed. Its last reservation,
        // which the exact block cannot take, lands on StagedOutput's stage
        // and is copied in at the end. A hint with headroom (previously
        // +12.5%) put the block in a larger size class than the document
        // and shrank it in place; measured in the harness's window
        // (gc.collect() before the call, the result freed inside it) that
        // round trip cost 13 us on the Windows runners and 13 us on macOS
        // against 2 us for an exact-fit block (benchmarks/
        // alloc_roundtrip_probe.py; docs/decisions.md 2026-09-03).
        static thread_local size_t size_hint = kDumpsInitialCapacity;
        StagedOutput staged;
        if (!staged.init_bytes(size_hint))
            return PyErr_NoMemory();
        Serializer serializer(staged, lease.state());
        if (!serializer.write(object))
            return nullptr;
        PyObject* result = staged.take_bytes();
        if (result != nullptr)
            size_hint = static_cast<size_t>(PyBytes_GET_SIZE(result));
        return result;
    }

    // Reused across calls on this thread: after the first few documents the
    // buffer has grown to size and stops allocating entirely. Thread-local
    // rather than global because the GIL is held but the buffer outlives the
    // call, and other threads must not share it. A nested call -- a cycle
    // warning or a big int's __str__ can run arbitrary Python mid-walk --
    // must not clear the outer call's buffer, so it serializes into a
    // private one while the shared buffer is busy.
    static thread_local std::string shared_out;
    static thread_local bool shared_busy = false;
    std::string private_out;
    const bool owns_shared = !shared_busy;
    std::string& out = owns_shared ? shared_out : private_out;
    if (owns_shared)
        shared_busy = true;
    struct Release {
        bool owns;
        ~Release() {
            if (owns)
                shared_busy = false;
        }
    } release{owns_shared};
    out.clear();
    if (out.capacity() < kDumpsInitialCapacity)
        out.reserve(kDumpsInitialCapacity);

    StagedOutput staged(out);
    Serializer serializer(staged, lease.state());
    if (!serializer.write(object))
        return nullptr;
    staged.flush_str();
    return PyUnicode_FromStringAndSize(out.data(), static_cast<Py_ssize_t>(out.size()));
}

} // namespace strata::bindings
