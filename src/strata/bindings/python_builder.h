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

#include <cstdint>
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
 * Finished values accumulate in one flat `values_` vector; `frames_` records
 * where each open container started. An array is created once, at its closing
 * bracket, with its final size already known — so there is no repeated list
 * growth and no per-element refcount churn, because PyList_SET_ITEM steals the
 * references the vector was holding.
 *
 * Every callback returns false on failure with a Python exception already set,
 * which aborts the parse; loads() then reports that exception rather than
 * overwriting it with a generic parse error.
 */
class PythonObjectBuilder {
  public:
    ~PythonObjectBuilder() {
        reset();
        clear_predictions();
    }

    /**
     * Return to the empty state, releasing whatever an aborted parse left
     * behind. The KeyCache survives deliberately -- it is why reusing one
     * builder across calls pays -- and so does the prediction table, which
     * `begin_input` invalidates the moment the input buffer changes.
     */
    void reset() noexcept {
        // Latched here as well as in begin_input and at construction, so no
        // entry path — loads, an NDJSON line, a fresh search capture sink —
        // can build under a stale policy.
        policy_ = get_duplicate_key_policy();
        Py_XDECREF(root_);
        root_ = nullptr;
        for (PyObject* value : values_)
            Py_XDECREF(value);
        values_.clear();
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
        input_begin_ = begin;
        input_end_ = end;
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
        if (frames_.empty() || frames_.back().mapping == nullptr)
            return 0;
        Frame& frame = frames_.back();
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
                    return entry.raw.size();
                }
            }
            return 0;
        }

        const auto& keys = schema.ways[frame.way].keys;
        if (frame.cursor >= keys.size())
            return 0;
        const PredictedKey& entry = keys[frame.cursor];
        if (!prediction_hits(entry, after_quote, remaining))
            return 0;
        keys_.push_back(Py_NewRef(entry.object));
        ++frame.cursor;
        return entry.raw.size();
    }

    bool on_null() { return push(Py_NewRef(Py_None)); }

    bool on_bool(bool value) { return push(Py_NewRef(value ? Py_True : Py_False)); }

    bool on_int(int64_t value) { return push(PyLong_FromLongLong(value)); }

    /// Integers past int64 keep every digit: Python has no size limit.
    bool on_big_int(std::string_view text) {
        const std::string digits(text); // PyLong_FromString needs a terminator
        return push(PyLong_FromString(digits.c_str(), nullptr, 10));
    }

    bool on_double(double value) { return push(PyFloat_FromDouble(value)); }

    bool on_string(std::string_view value) {
        // The bytes are parser-validated UTF-8 already, so for the
        // overwhelmingly common all-ASCII case the compact-ASCII object is
        // built directly: one word-wise scan plus a memcpy, instead of the
        // decoder's own re-scan and writer setup (find_first_nonascii +
        // unicode_decode_utf8 profiled at ~5% of a string-heavy parse).
        // Zero- and one-byte strings stay on the decoder: CPython hands
        // those out from its empty and latin-1 singleton caches, which a
        // fresh PyUnicode_New would replace with an allocation per value.
        const auto size = static_cast<Py_ssize_t>(value.size());
        if (size > 1 && all_ascii(value.data(), value.size())) {
            PyObject* object = PyUnicode_New(size, 127);
            if (object == nullptr)
                return push(nullptr);
            std::memcpy(PyUnicode_DATA(object), value.data(), value.size());
            return push(object);
        }
        return push(PyUnicode_FromStringAndSize(value.data(), size));
    }

    bool on_start_object() {
        PyObject* mapping = new_mapping(frames_.size());
        if (mapping == nullptr)
            return false;
        frames_.push_back(Frame{mapping, values_.size(), 0, kWayUnresolved});
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
        frames_.push_back(Frame{nullptr, values_.size(), 0, kWayUnresolved});
        return true;
    }

    bool on_end_array() {
        if (frames_.empty() || frames_.back().mapping != nullptr)
            return false;
        const size_t start = frames_.back().start;
        frames_.pop_back();

        const size_t count = values_.size() - start;
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
        if (list == nullptr)
            return false;
        for (size_t index = 0; index < count; ++index) {
            // SET_ITEM steals the reference the vector was holding.
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(index), values_[start + index]);
        }
        values_.resize(start);
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

    /// An open container: a dict, or an array holding from `start` in values_.
    struct Frame {
        PyObject* mapping;
        size_t start;
        uint32_t cursor; ///< next prediction slot within the adopted way
        uint8_t way;     ///< adopted prediction way, or kWayUnresolved
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
        uint64_t word0 = 0;
        uint64_t word1 = 0;
        uint64_t mask0 = 0;
        uint64_t mask1 = 0;

        void fill_words() noexcept {
            const size_t len = raw.size();
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
                return;
            }
            for (size_t index = frame.cursor; index < keys.size(); ++index)
                Py_DECREF(keys[index].object);
            keys.resize(frame.cursor);
        }

        keys.push_back(PredictedKey{std::string(begin, key.size() + 1), Py_NewRef(object)});
        keys.back().fill_words();
        ++frame.cursor;
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
        if (frames_.back().mapping == nullptr) {
            values_.push_back(value); // the vector now owns it
            return true;
        }
        return insert_into_object(frames_.back().mapping, value);
    }

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
    std::vector<PyObject*> values_;
    std::vector<PyObject*> keys_;
    /// Depths whose object sizes are remembered; deeper objects are not
    /// record-shaped and just take PyDict_New. Fixed storage: the hot path
    /// does one bounded store, never a vector grow.
    static constexpr size_t kMaxSizedDepth = 64;

    std::vector<Schema> predicted_;                          ///< indexed by depth
    uint32_t depth_sizes_[kMaxSizedDepth] = {};              ///< last object size per depth
    DuplicateKeyPolicy policy_ = get_duplicate_key_policy(); ///< latched per parse
    const char* input_begin_ = nullptr;
    const char* input_end_ = nullptr;
    KeyCache keys_cache_;
};

} // namespace strata::bindings
