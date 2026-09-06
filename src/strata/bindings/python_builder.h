#pragma once

/**
 * @file python_builder.h
 * @brief The one definition of "JSON events to Python objects".
 *
 * PythonObjectBuilder is the SAX handler behind `loads`, `load`, NDJSON and
 * the streaming-search capture path. It lives in a header because two
 * translation units instantiate the templated parser over it, and because
 * anything that builds Python values for the user -- including a captured
 * search match -- must apply the duplicate-key policy identically, or the
 * `search == query(load)` law breaks on exactly the documents nobody tests.
 *
 * The class is deliberately *not* derived from JsonSaxHandler: the parser is
 * a template, and instantiating it on this concrete type lets the compiler
 * inline every callback.
 */

#include "python_types.h"
#include "strata/json/json_parse.hpp"
#include "strata/util/fast_parse.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <vector>

// _PyDict_NewPresized is CPython-internal but exported on every supported
// version (3.10–3.14; audit on each new one — docs/bindings/SKILL.md). A dict
// created at its final size skips the resize cascade PyDict_New pays as a
// record fills in: a 21-key record otherwise reallocates and rehashes its
// table twice, once per size doubling past 5 and 10 entries.
#if PY_VERSION_HEX < 0x030F0000
extern "C" PyObject* _PyDict_NewPresized(Py_ssize_t minused);
#define STRATA_HAVE_DICT_PRESIZED 1
#endif

namespace strata::bindings {

/**
 * Reuses the `PyObject*` for a key across every record that repeats it.
 *
 * Documents are overwhelmingly arrays of same-shaped records, so the same
 * handful of key strings recur once per record. Creating each one afresh means
 * a decode, an allocation and a hash on every occurrence; here the first
 * occurrence pays for all of them.
 *
 * Open addressing over a fixed table, FNV-1a over the raw bytes, and a short
 * probe budget: a key that cannot find a slot is simply created uncached, so a
 * pathological document degrades to the previous behaviour rather than growing
 * an unbounded table.
 */
class KeyCache {
  public:
    KeyCache() = default;
    KeyCache(const KeyCache&) = delete;
    KeyCache& operator=(const KeyCache&) = delete;

    ~KeyCache() {
        for (PyObject* key : slots_)
            Py_XDECREF(key);
    }

    /// A new reference to the interned key, or nullptr with an error set.
    [[nodiscard]] PyObject* intern(std::string_view text) {
        size_t slot = hash(text) & (kSlots - 1);
        for (size_t probe = 0; probe < kMaxProbes; ++probe) {
            PyObject*& candidate = slots_[slot];
            if (candidate == nullptr) {
                PyObject* created = make(text);
                if (created == nullptr)
                    return nullptr;
                candidate = Py_NewRef(created);
                return created;
            }
            if (matches(candidate, text))
                return Py_NewRef(candidate);
            slot = (slot + 1) & (kSlots - 1);
        }
        return make(text); // table crowded here: create without caching
    }

  private:
    static constexpr size_t kSlots = 512; // power of two, so the mask works
    static constexpr size_t kMaxProbes = 8;

    [[nodiscard]] static PyObject* make(std::string_view text) {
        return PyUnicode_FromStringAndSize(text.data(), static_cast<Py_ssize_t>(text.size()));
    }

    [[nodiscard]] static bool matches(PyObject* key, std::string_view text) {
        Py_ssize_t size = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(key, &size);
        if (utf8 == nullptr) {
            PyErr_Clear();
            return false;
        }
        return static_cast<size_t>(size) == text.size() &&
               std::memcmp(utf8, text.data(), text.size()) == 0;
    }

    [[nodiscard]] static size_t hash(std::string_view text) noexcept {
        size_t value = 1469598103934665603ULL; // FNV-1a offset basis
        for (const char c : text) {
            value ^= static_cast<unsigned char>(c);
            value *= 1099511628211ULL;
        }
        return value;
    }

    PyObject* slots_[kSlots] = {};
};

/**
 * Builds Python objects directly from parser events.
 *
 * Finished values accumulate in one flat value stack; `frames_` records where
 * each open container started. An array is created once, at its closing
 * bracket, with its final size already known — so there is no repeated list
 * growth and no per-element refcount churn, because PyList_SET_ITEM steals the
 * references the stack was holding.
 *
 * The stack is three raw pointers rather than a std::vector because the parser
 * borrows `[next, limit)` into registers for the length of an array's element
 * loop (@ref ValueCursor, docs/architecture/value-cursor.md) and hands `next`
 * back at the close — a range a vector has no way to publish. Ownership is
 * unchanged: every reference in `[values_begin_, values_next_)` belongs to the
 * builder, and `reset()` is what releases them, so a cursor is closed before
 * any path that can leave the parser.
 *
 * Every callback returns false on failure with a Python exception already set,
 * which aborts the parse; loads() then reports that exception rather than
 * overwriting it with a generic parse error.
 */
class PythonObjectBuilder {
    // Declared ahead of the callbacks that name them; defined below.
    struct Frame;
    struct PredictedKey;
    struct Schema;

  public:
    ~PythonObjectBuilder() {
        reset();
        clear_predictions();
        std::free(values_begin_);
    }

    /**
     * Return to the empty state, releasing whatever an aborted parse left
     * behind. The KeyCache survives deliberately -- it is why reusing one
     * builder across calls pays -- and so does the prediction table: its
     * predictions own their bytes and outlive the buffer, and `begin_input`
     * re-arms its churn accounting for the next input.
     */
    void reset() noexcept {
        // Latched here as well as in begin_input and at construction, so no
        // entry path — loads, an NDJSON line, a fresh search capture sink —
        // can build under a stale policy.
        policy_ = get_duplicate_key_policy();
        Py_XDECREF(root_);
        root_ = nullptr;
        for (PyObject** slot = values_begin_; slot != values_next_; ++slot)
            Py_XDECREF(*slot);
        values_next_ = values_begin_; // the block itself is reused, as before
        for (const Frame& frame : frames_)
            Py_XDECREF(frame.mapping);
        frames_.clear();
        for (PyObject* key : keys_)
            Py_XDECREF(key);
        keys_.clear();
    }

    /**
     * Declare the buffer the next parse reads from.
     *
     * Only zero-copy keys are recordable, and "zero-copy" is detected by the
     * key view pointing into this range. The predictions themselves own their
     * bytes, so they survive across buffers -- an address alone could not be
     * trusted anyway, since a freed buffer's address can come back holding a
     * different document.
     */
    void begin_input(const char* begin, const char* end) noexcept {
        ascii_expected_ = true;
        input_begin_ = begin;
        input_end_ = end;
        // The prediction table's churn accounting is per input. A depth
        // retires when its objects keep disagreeing -- the guard against a
        // shape-unstable document -- but the count used to run across every
        // document a thread's builder ever saw, and a retired depth never
        // predicted again: after the other four benchmark datasets had gone
        // through, every key of mixed.json paid the miss path for the life
        // of the thread (128 -> 145 us per parse, measured, and the whole of
        // that row's deficit). Each input now starts with the count at zero
        // and the depth predicting; one that thrashes retires within itself
        // exactly as before, and re-learns on the next.
        for (Schema& schema : predicted_) {
            schema.divergences = 0;
            schema.retired = false;
        }
        // One thread-local read per parse instead of per dict member: the
        // getter is an out-of-line call, and users.json inserts ~150k keys.
        // A parse therefore runs under the policy it started with — the only
        // way to observe a difference is a Warn-policy warning filter calling
        // config.set mid-parse (docs/decisions.md).
        policy_ = get_duplicate_key_policy();
    }

    /**
     * The parser's speculative-key hook (see has_try_match_key).
     *
     * Same-schema records repeat their keys in order, so the key most likely
     * to appear next is the one that appeared at this position, at this
     * depth, in the previous record. One memcmp against its raw bytes --
     * escapes, closing quote and all -- replaces the escape scan, the decode
     * and the cache lookup.
     */
    [[nodiscard]] size_t try_match_key(const char* after_quote, size_t remaining) {
        if (frames_.empty())
            return 0;
        Frame& frame = frames_.back();
        // The resolved case first, and through the frame's own cursor: one
        // load, one probe, one increment per key.
        if (frame.next != nullptr) {
            if (frame.next == frame.end)
                return 0;
            const PredictedKey& entry = *frame.next;
            if (!prediction_hits(entry, after_quote, remaining))
                return 0;
            keys_.push_back(Py_NewRef(entry.object));
            ++frame.next;
            ++frame.cursor;
            return entry.raw_size;
        }
        if (frame.mapping == nullptr)
            return 0;
        const size_t depth = frames_.size();
        if (depth >= predicted_.size())
            return 0;
        Schema& schema = predicted_[depth];
        if (schema.retired)
            return 0;

        if (frame.way == kWayUnresolved) {
            // First key of this object: the way whose leading key matches the
            // input is the shape this record follows. Interleaved schemas
            // (kind-tagged records, event streams) alternate at one depth, so
            // a single remembered shape thrashed and retired on exactly those
            // documents; probing at most four leading keys keeps every
            // recurring shape predictable — the same design the dumps schema
            // cache settled on (docs/performance/SKILL.md, wave 6).
            for (uint8_t way_index = 0; way_index < schema.way_count; ++way_index) {
                const auto& keys = schema.ways[way_index].keys;
                if (keys.empty())
                    continue;
                const PredictedKey& entry = keys.front();
                if (prediction_hits(entry, after_quote, remaining)) {
                    keys_.push_back(Py_NewRef(entry.object));
                    frame.way = way_index;
                    frame.cursor = 1;
                    frame.next = keys.data() + 1;
                    frame.end = keys.data() + keys.size();
                    return entry.raw_size;
                }
            }
            return 0;
        }
        // A resolved way whose cursor was cleared (a miss re-recorded the
        // shape): nothing more to predict for this object.
        return 0;
    }

    /// Point @p frame's cursor at its adopted way's entries from its current
    /// slot, or clear it: called after anything that may have moved them.
    void refresh_cursor(Frame& frame, const Schema& schema) noexcept {
        if (frame.way == kWayUnresolved || schema.retired) {
            frame.next = nullptr;
            frame.end = nullptr;
            return;
        }
        const auto& keys = schema.ways[frame.way].keys;
        frame.next = keys.data() + frame.cursor;
        frame.end = keys.data() + keys.size();
    }

    bool on_null() { return push_singleton(Py_None); }

    bool on_bool(bool value) { return push_singleton(value ? Py_True : Py_False); }

    bool on_int(int64_t value) { return push(PyLong_FromLongLong(value)); }

    bool on_big_int(std::string_view text) { return push(make_big_int(text)); }

    bool on_double(double value) { return push(PyFloat_FromDouble(value)); }

    bool on_string(std::string_view value) { return push(make_string(value)); }

    // --- The value cursor (docs/architecture/value-cursor.md) --------------
    //
    // Declaring this member type is what makes ParserInline hand an array's
    // element loop the staging range to hold in registers; the `_at` callbacks
    // below are the placement half of the same events the `on_*` callbacks
    // above serve, over the identical object construction, so there is exactly
    // one definition of what a JSON scalar becomes.

    /// The staging stack's writable range, borrowed by one array's element
    /// loop. Two words, trivially copyable: it travels in registers.
    struct ValueCursor {
        PyObject** next;
        PyObject** limit;
    };

    /// Lend the writable range. Valid only until the next `close_values` --
    /// which every route into the builder's own stack is preceded by.
    [[nodiscard]] ValueCursor open_values() noexcept {
        return ValueCursor{values_next_, values_cap_};
    }

    /// Take the range back: from here the builder's own view owns every
    /// reference the loop stored, and `reset()` will release them.
    void close_values(ValueCursor cursor) noexcept { values_next_ = cursor.next; }

    ValueCursor on_null_at(ValueCursor cursor) { return store_singleton(cursor, Py_None); }

    ValueCursor on_bool_at(ValueCursor cursor, bool value) {
        return store_singleton(cursor, value ? Py_True : Py_False);
    }

    ValueCursor on_int_at(ValueCursor cursor, int64_t value) {
        return store(cursor, PyLong_FromLongLong(value));
    }

    ValueCursor on_big_int_at(ValueCursor cursor, std::string_view text) {
        return store(cursor, make_big_int(text));
    }

    ValueCursor on_double_at(ValueCursor cursor, double value) {
        return store(cursor, PyFloat_FromDouble(value));
    }

    ValueCursor on_string_at(ValueCursor cursor, std::string_view value) {
        return store(cursor, make_string(value));
    }

    /// Integers past int64 keep every digit: Python has no size limit.
    [[nodiscard]] PyObject* make_big_int(std::string_view text) {
        const std::string digits(text); // PyLong_FromString needs a terminator
        return PyLong_FromString(digits.c_str(), nullptr, 10);
    }

    [[nodiscard]] PyObject* make_string(std::string_view value) {
        // The bytes are parser-validated UTF-8 already, so for the
        // overwhelmingly common all-ASCII case the compact-ASCII object is
        // built directly: one word-wise scan plus a memcpy, instead of the
        // decoder's own re-scan and writer setup (find_first_nonascii +
        // unicode_decode_utf8 profiled at ~5% of a string-heavy parse).
        // Zero- and one-byte strings stay on the decoder: CPython hands
        // those out from its empty and latin-1 singleton caches, which a
        // fresh PyUnicode_New would replace with an allocation per value.
        const auto size = static_cast<Py_ssize_t>(value.size());
        if (size > 1) {
            if (ascii_expected_) {
                // Speculate ASCII: the compact object is allocated first and
                // the bytes are copied into it while the high bits are
                // checked -- one pass in place of the scan and the memcpy.
                // A high byte hands the string to the decoder and turns the
                // speculation off for the rest of the document, so a
                // non-ASCII document wastes one allocation, not one per
                // string.
                PyObject* object = PyUnicode_New(size, 127);
                if (object == nullptr)
                    return nullptr;
                if (copy_if_ascii(static_cast<char*>(PyUnicode_DATA(object)), value.data(),
                                  value.size()))
                    return object;
                Py_DECREF(object);
                ascii_expected_ = false;
            } else if (all_ascii(value.data(), value.size())) {
                PyObject* object = PyUnicode_New(size, 127);
                if (object == nullptr)
                    return nullptr;
                std::memcpy(PyUnicode_DATA(object), value.data(), value.size());
                return object;
            }
        }
        return PyUnicode_FromStringAndSize(value.data(), size);
    }

    bool on_start_object() {
        PyObject* mapping = new_mapping(frames_.size());
        if (mapping == nullptr)
            return false;
        frames_.push_back(Frame{mapping, values_size(), 0, kWayUnresolved, nullptr, nullptr});
        return true;
    }

    /// The prediction missed (or there was none): intern normally, then teach
    /// the table this position so the next record can predict it.
    bool on_key(std::string_view key) {
        PyObject* object = keys_cache_.intern(key);
        if (object == nullptr)
            return false;
        keys_.push_back(object);
        record_key(key, object);
        return true;
    }

    bool on_end_object() {
        if (frames_.empty())
            return false;
        PyObject* mapping = frames_.back().mapping;
        const size_t depth = frames_.size() - 1;
        frames_.pop_back();
        if (mapping != nullptr && depth < kMaxSizedDepth) {
            // Teach the depth its size, so the next object there starts life
            // at the right capacity (see new_mapping).
            depth_sizes_[depth] = static_cast<uint32_t>(PyDict_GET_SIZE(mapping));
        }
        return push(mapping);
    }

    bool on_start_array() {
        frames_.push_back(Frame{nullptr, values_size(), 0, kWayUnresolved, nullptr, nullptr});
        return true;
    }

    bool on_end_array() {
        if (frames_.empty() || frames_.back().mapping != nullptr)
            return false;
        const size_t start = frames_.back().start;
        frames_.pop_back();

        // The element loop's cursor was closed before this call, so the
        // stack's own end is the array's end (docs/architecture/value-cursor.md).
        const size_t count = values_size() - start;
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
        if (list == nullptr)
            return false; // the staged references stay owned; reset() frees them
        PyObject** const staged = values_begin_ + start;
        for (size_t index = 0; index < count; ++index) {
            // SET_ITEM steals the reference the stack was holding.
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(index), staged[index]);
        }
        values_next_ = values_begin_ + start;
        return push(list);
    }

    /// Hand the finished tree to the caller.
    [[nodiscard]] PyObject* take_root() noexcept {
        PyObject* root = root_;
        root_ = nullptr;
        return root;
    }

  private:
    /// No byte with the high bit set, a word at a time (tail via a padded
    /// word). The parser has already validated UTF-8; this only picks the
    /// construction path, so a false negative costs the decoder call, never
    /// correctness.
    [[nodiscard]] static bool all_ascii(const char* data, size_t len) noexcept {
        uint64_t accumulated = 0;
        size_t pos = 0;
        for (; pos + 8 <= len; pos += 8) {
            uint64_t word;
            std::memcpy(&word, data + pos, 8);
            accumulated |= word;
        }
        if (pos < len) {
            uint64_t word = 0;
            std::memcpy(&word, data + pos, len - pos);
            accumulated |= word;
        }
        return (accumulated & 0x8080808080808080ULL) == 0;
    }

    /// Copy @p len (at least two) bytes to @p dst while accumulating their
    /// high bits: the ASCII check and the copy in one pass. A word at a
    /// time; a tail of a string of eight bytes or more is the string's last
    /// word, loaded and stored overlapping the previous one (both ends hold
    /// those bytes already), a shorter string's tail goes byte by byte.
    /// Returns false when a byte has its high bit set -- @p dst is then
    /// partly written and the caller discards it.
    [[nodiscard]] static bool copy_if_ascii(char* dst, const char* src, size_t len) noexcept {
        uint64_t accumulated = 0;
        size_t pos = 0;
        for (; pos + 8 <= len; pos += 8) {
            uint64_t word;
            std::memcpy(&word, src + pos, 8);
            std::memcpy(dst + pos, &word, 8);
            accumulated |= word;
        }
        if (pos < len) {
            if (len >= 8) {
                uint64_t word;
                std::memcpy(&word, src + len - 8, 8);
                std::memcpy(dst + len - 8, &word, 8);
                accumulated |= word;
            } else {
                for (; pos < len; ++pos) {
                    const char byte = src[pos];
                    dst[pos] = byte;
                    accumulated |= static_cast<unsigned char>(byte);
                }
            }
        }
        return (accumulated & 0x8080808080808080ULL) == 0;
    }

    /// An open container: a dict, or an array holding from `start` in values_.
    struct Frame {
        PyObject* mapping;
        size_t start;
        uint32_t cursor; ///< next prediction slot within the adopted way
        uint8_t way;     ///< adopted prediction way, or kWayUnresolved
        /// The adopted way's remaining predictions, [next, end): the per-key
        /// probe reads one entry through these instead of re-deriving depth,
        /// schema, way and cursor through three vectors (try_match_key self
        /// time was 4.9 ns per key). Refreshed by record_key whenever it
        /// changes the way's storage (push, resize, release, retire), so a
        /// cursor never outlives the entries it points at; null when there is
        /// nothing to predict.
        const PredictedKey* next;
        const PredictedKey* end;
    };

    /// A dict frame starts with no adopted way; its first key picks one.
    static constexpr uint8_t kWayUnresolved = 0xFF;

    /// One remembered key: its raw bytes (through the closing quote) and its
    /// interned object. The bytes are owned -- typical keys sit in the small-
    /// string buffer -- so a prediction never dangles, whatever happened to
    /// the buffer it was learned from, and by construction the object's text
    /// is always those bytes minus the quote.
    ///
    /// Keys of sixteen raw bytes or fewer -- essentially all of them -- also
    /// keep a zero-padded two-word copy plus byte masks, so the per-key probe
    /// compares two registers instead of calling memcmp (profiled at ~5% of a
    /// prediction-heavy parse). Byte order note: the masks select the *first*
    /// `len` bytes on a little-endian target, which every supported platform
    /// is; the memcmp fallback remains the definition of a match.
    struct PredictedKey {
        std::string raw;
        PyObject* object;
        uint32_t raw_size = 0; ///< raw.size(), read without the SSO branch
        uint64_t word0 = 0;
        uint64_t word1 = 0;
        uint64_t mask0 = 0;
        uint64_t mask1 = 0;

        void fill_words() noexcept {
            const size_t len = raw.size();
            raw_size = static_cast<uint32_t>(len);
            if (len > 16)
                return; // masks stay zero: the probe takes the memcmp path
            unsigned char padded[16] = {};
            std::memcpy(padded, raw.data(), len);
            std::memcpy(&word0, padded, 8);
            std::memcpy(&word1, padded + 8, 8);
            const size_t low = len < 8 ? len : 8;
            mask0 = low == 8 ? ~0ULL : ((1ULL << (low * 8)) - 1);
            const size_t high = len > 8 ? len - 8 : 0;
            mask1 = high == 8 ? ~0ULL : high == 0 ? 0 : ((1ULL << (high * 8)) - 1);
        }
    };

    /// Does the raw input at @p in continue with this predicted key?
    [[nodiscard]] static bool prediction_hits(const PredictedKey& entry, const char* in,
                                              size_t remaining) noexcept {
        const size_t len = entry.raw.size();
        if (len > remaining)
            return false;
        if (entry.mask0 != 0 && remaining >= 16) {
            // Two overlapping-window loads are safe: sixteen readable bytes
            // were just checked, and the masks blank everything past the key.
            uint64_t in0;
            uint64_t in1;
            std::memcpy(&in0, in, 8);
            std::memcpy(&in1, in + 8, 8);
            return ((in0 ^ entry.word0) & entry.mask0) == 0 &&
                   ((in1 ^ entry.word1) & entry.mask1) == 0;
        }
        return std::memcmp(in, entry.raw.data(), len) == 0;
    }

    /// Depths beyond this predict nothing; documents this deep are not
    /// record-shaped and the table would only churn.
    static constexpr size_t kMaxPredictedDepth = 64;
    /// Keys per object worth remembering; wider objects stop recording.
    static constexpr size_t kMaxPredictedKeys = 64;
    /// Shape changes tolerated at one depth before it stops predicting.
    static constexpr uint32_t kMaxDivergences = 16;

    /// One remembered shape at one depth: its keys, in order.
    struct PredictionWay {
        std::vector<PredictedKey> keys;
    };

    /// The prediction state of one nesting depth: up to four shapes, because
    /// interleaved-schema documents alternate a small set of record shapes at
    /// one depth, and a single slot thrashed on exactly those (the dumps
    /// schema cache went through the same evolution — four ways, scanned in
    /// place, round-robin replacement).
    struct Schema {
        static constexpr size_t kWays = 4;

        PredictionWay ways[kWays];
        uint8_t way_count = 0;
        uint8_t next_replace = 0;
        uint32_t divergences = 0;
        /// Set once the depth has proven shape-unstable: objects there keep
        /// disagreeing, so recording would only churn allocations -- measured
        /// as a 15% regression on a mixed-shape document before this guard.
        bool retired = false;
    };

    static void release_way(PredictionWay& way) noexcept {
        for (PredictedKey& entry : way.keys)
            Py_DECREF(entry.object);
        way.keys.clear();
    }

    static void retire(Schema& schema) noexcept {
        for (PredictionWay& way : schema.ways)
            release_way(way);
        schema.way_count = 0;
        schema.retired = true;
    }

    void clear_predictions() noexcept {
        for (Schema& schema : predicted_)
            for (PredictionWay& way : schema.ways)
                release_way(way);
        predicted_.clear();
        input_begin_ = nullptr;
        input_end_ = nullptr;
    }

    void record_key(std::string_view key, PyObject* object) {
        const size_t depth = frames_.size();
        if (depth == 0 || depth >= kMaxPredictedDepth || frames_.back().mapping == nullptr)
            return;
        // Only zero-copy keys are recordable: their view points into the
        // input, so the raw span is the key plus its closing quote. A key
        // that needed decoding lives in the parser's scratch buffer and has
        // no stable raw form to compare against.
        const char* begin = key.data();
        if (begin < input_begin_ || begin + key.size() >= input_end_)
            return;

        if (predicted_.size() <= depth)
            predicted_.resize(depth + 1);
        Schema& schema = predicted_[depth];
        if (schema.retired)
            return;
        Frame& frame = frames_.back();
        if (frame.cursor >= kMaxPredictedKeys)
            return;

        if (frame.way == kWayUnresolved) {
            // try_match_key already probed every way's leading key against
            // the raw input and missed, so this first key opens a shape no
            // way holds: claim an empty way, or replace the oldest. (A first
            // key that needed decoding could still belong to a known way;
            // being zero-copy-only here just costs that record its
            // predictions, never correctness.)
            if (schema.way_count < Schema::kWays) {
                frame.way = schema.way_count++;
            } else {
                if (++schema.divergences > kMaxDivergences) {
                    retire(schema);
                    refresh_cursor(frame, schema);
                    return;
                }
                frame.way = schema.next_replace;
                schema.next_replace =
                    static_cast<uint8_t>((schema.next_replace + 1) % Schema::kWays);
                release_way(schema.ways[frame.way]);
            }
        }

        auto& keys = schema.ways[frame.way].keys;

        // The shape diverged at this position: drop the stale tail. The next
        // record either follows the new shape (and now predicts it) or pays
        // one more miss here. A depth that keeps diverging is not
        // record-shaped at all; it retires rather than churning forever.
        if (frame.cursor < keys.size()) {
            if (++schema.divergences > kMaxDivergences) {
                retire(schema);
                refresh_cursor(frame, schema);
                return;
            }
            for (size_t index = frame.cursor; index < keys.size(); ++index)
                Py_DECREF(keys[index].object);
            keys.resize(frame.cursor);
        }

        keys.push_back(PredictedKey{std::string(begin, key.size() + 1), Py_NewRef(object)});
        keys.back().fill_words();
        ++frame.cursor;
        refresh_cursor(frame, schema); // push_back may have moved the entries
    }

    /// A dict for an object opening at @p depth, presized to the size the
    /// previous object at that depth reached. Same-schema records repeat per
    /// depth, so the last size is almost always the next size; a wrong hint
    /// costs a briefly-oversized or resized-anyway table, never correctness.
    /// At five members or fewer the default 8-slot table already fits and
    /// PyDict_New is the cheaper constructor. The hints are plain integers
    /// with no lifetime — they survive reset() with the KeyCache, which is
    /// what makes them pay across the calls of a leased builder.
    [[nodiscard]] PyObject* new_mapping(size_t depth) {
#if defined(STRATA_HAVE_DICT_PRESIZED)
        if (depth < kMaxSizedDepth) {
            const uint32_t hint = depth_sizes_[depth];
            if (hint > 5)
                return _PyDict_NewPresized(static_cast<Py_ssize_t>(hint));
        }
#endif
        return PyDict_New();
    }

    /// Consume one reference to @p value, placing it where it belongs.
    bool push(PyObject* value) {
        if (value == nullptr)
            return false;

        if (frames_.empty()) {
            Py_XDECREF(root_);
            root_ = value;
            return true;
        }
        if (frames_.back().mapping == nullptr)
            return push_value(value); // the stack now owns it
        return insert_into_object(frames_.back().mapping, value);
    }

    /// Place one of the interpreter's singletons (None, True, False). From
    /// CPython 3.12 they are immortal -- a reference taken or stolen on them
    /// is a no-op -- so none is taken (measured: 2.2 ns per bool and 0.3 per
    /// null at the Python level, the reference's immortality check and the
    /// branch it was behind); earlier interpreters take one as for any value.
    bool push_singleton(PyObject* value) {
#if PY_VERSION_HEX >= 0x030C0000
        return push(value);
#else
        return push(Py_NewRef(value));
#endif
    }

    /// The singleton rule of push_singleton, through the cursor.
    ValueCursor store_singleton(ValueCursor cursor, PyObject* value) {
#if PY_VERSION_HEX >= 0x030C0000
        return store(cursor, value);
#else
        return store(cursor, Py_NewRef(value));
#endif
    }

    /**
     * Consume one reference to @p value into the borrowed range.
     *
     * The whole point of the cursor: on the common path this is a store and a
     * register increment, with no load of the stack's end and no store back to
     * it. A full range, and a failed construction, both leave through
     * `store_grown` / the null branch, which publish the cursor first -- so a
     * returned failure token always means "closed", and every reference stored
     * so far is the builder's again.
     */
    [[nodiscard]] ValueCursor store(ValueCursor cursor, PyObject* value) {
        if (value == nullptr) {
            close_values(cursor);
            return ValueCursor{};
        }
        if (cursor.next == cursor.limit)
            return store_grown(cursor, value);
        *cursor.next++ = value;
        return cursor;
    }

    /// The range is full: publish it, grow the stack, and lend a fresh range.
    /// Out of line because it happens once per doubling, not once per element.
    STRATA_NOINLINE ValueCursor store_grown(ValueCursor cursor, PyObject* value) {
        close_values(cursor);
        if (!grow_values()) {
            Py_DECREF(value);
            return ValueCursor{};
        }
        *values_next_++ = value;
        return ValueCursor{values_next_, values_cap_};
    }

    /// Consume one reference to @p value into the stack, the builder's own
    /// view. Only ever reached with no cursor outstanding: a finished nested
    /// container, or a handler that has no cursor at all.
    [[nodiscard]] bool push_value(PyObject* value) {
        if (values_next_ == values_cap_ && !grow_values()) {
            Py_DECREF(value);
            return false;
        }
        *values_next_++ = value;
        return true;
    }

    [[nodiscard]] size_t values_size() const noexcept {
        return static_cast<size_t>(values_next_ - values_begin_);
    }

    /// Double the staging block (first call: kInitialValueSlots). The slots
    /// hold plain pointers, so the move is a realloc; `Frame::start` is an
    /// index precisely so that it survives one.
    [[nodiscard]] bool grow_values() {
        const size_t used = values_size();
        const size_t old_slots = static_cast<size_t>(values_cap_ - values_begin_);
        const size_t slots = old_slots == 0 ? kInitialValueSlots : old_slots * 2;
        auto* block = static_cast<PyObject**>(
            std::realloc(values_begin_, slots * sizeof(PyObject*))); // NOLINT
        if (block == nullptr) {
            PyErr_NoMemory();
            return false;
        }
        values_begin_ = block;
        values_next_ = block + used;
        values_cap_ = block + slots;
        return true;
    }

    /// First block: 64 slots, past the doublings every small document would
    /// otherwise pay on a freshly leased builder. The block is kept across
    /// resets, so a reused builder grows it once.
    static constexpr size_t kInitialValueSlots = 64;

    bool insert_into_object(PyObject* object, PyObject* value) {
        if (keys_.empty()) {
            Py_DECREF(value);
            return false;
        }
        PyRef key(keys_.back());
        keys_.pop_back();

        // The overwhelmingly common case -- a key seen once, under the default
        // policy -- must cost exactly one dict lookup. SetDefault inserts when
        // absent and returns the existing entry when present, which is both
        // the FirstWins semantics and the duplicate probe in one operation;
        // the Contains-then-SetItem shape this replaces paid two lookups on
        // every single member.
        const DuplicateKeyPolicy policy = policy_;
        if (policy == DuplicateKeyPolicy::FirstWins) {
            PyObject* settled = PyDict_SetDefault(object, key.get(), value);
            Py_DECREF(value);
            return settled != nullptr;
        }
        if (policy == DuplicateKeyPolicy::LastWins) {
            // Overwriting *is* the policy, so no duplicate probe is needed.
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }

        // Warn and Error both need to know a duplicate happened.
        const int present = PyDict_Contains(object, key.get());
        if (present < 0) {
            Py_DECREF(value);
            return false;
        }

        if (present == 0) {
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }

        // The key is already there; the policy decides what that means.
        switch (policy) {
        case DuplicateKeyPolicy::FirstWins:
            Py_DECREF(value);
            return true;
        case DuplicateKeyPolicy::LastWins: {
            const int failed = PyDict_SetItem(object, key.get(), value);
            Py_DECREF(value);
            return failed == 0;
        }
        case DuplicateKeyPolicy::Warn: {
            Py_DECREF(value);
            // A warning filter set to "error" turns this into a raised
            // exception, which aborts the parse rather than being swallowed.
            return PyErr_WarnFormat(PyExc_RuntimeWarning, 1, "Duplicate key encountered: %U",
                                    key.get()) == 0;
        }
        case DuplicateKeyPolicy::Error:
            Py_DECREF(value);
            PyErr_SetString(PyExc_ValueError, "Invalid JSON");
            return false;
        }

        Py_DECREF(value);
        return false;
    }

    PyObject* root_ = nullptr;
    std::vector<Frame> frames_;
    /// The staging stack: owned references in [begin, next), free slots up to
    /// cap. Three raw pointers so the parser can borrow [next, cap) -- see
    /// ValueCursor.
    PyObject** values_begin_ = nullptr;
    PyObject** values_next_ = nullptr;
    PyObject** values_cap_ = nullptr;
    std::vector<PyObject*> keys_;
    /// Depths whose object sizes are remembered; deeper objects are not
    /// record-shaped and just take PyDict_New. Fixed storage: the hot path
    /// does one bounded store, never a vector grow.
    static constexpr size_t kMaxSizedDepth = 64;

    std::vector<Schema> predicted_;             ///< indexed by depth
    uint32_t depth_sizes_[kMaxSizedDepth] = {}; ///< last object size per depth
    /// Strings are built as compact ASCII speculatively until the document
    /// shows a high byte (see on_string); reset per input.
    bool ascii_expected_ = true;
    DuplicateKeyPolicy policy_ = get_duplicate_key_policy(); ///< latched per parse
    const char* input_begin_ = nullptr;
    const char* input_end_ = nullptr;
    KeyCache keys_cache_;
};

} // namespace strata::bindings
