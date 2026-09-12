#pragma once

/**
 * @file python_dumps_output.h
 * @brief The serializer's output machinery: staging and the schema cache.
 *
 * Split from python_dumps.cpp for the ~800-line rule; included only there.
 * Two pieces live here:
 *
 *  - StagedOutput: an 8KB stage flushed into a std::string (the str path),
 *    or a PyBytes written directly, sized to the previous document, with
 *    the stage holding the tail that its last reservation could not fit
 *    (the bytes path -- one document copy at most, usually none).
 *  - SchemaCacheLease: the per-thread lease of everything a dumps() call
 *    keeps per nesting level -- the prepared-key cache with owned key
 *    references, and the staged key/value rows the dict writers emit from.
 *
 * The third, the raw dict walk, outgrew the same rule and moved to
 * python_rawdict.h; this file includes it because a lease holds the
 * compaction scratch that walk fills.
 */

#include "python_rawdict.h"
#include "python_types.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include <vector>

namespace strata::bindings {

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
    /// The widest single reservation any writer makes (python_dumps.cpp
    /// guards its string paths with it). An exact repeat never grows its
    /// block only while the stage can hold the tail one such reservation
    /// leaves behind plus the next one — pinned below.
    static constexpr size_t kMaxReservation = 4096;
    static constexpr size_t kStageBytes = 8192;
    static_assert(2 * kMaxReservation <= kStageBytes,
                  "the stage must hold two of the widest reservation, or exact repeats resize");

    explicit StagedOutput(std::string& sink)
        : str_sink_(&sink), base_(stage_), capacity_(sizeof(stage_)) {}

    /**
     * Bytes mode: the serializer writes *directly into the PyBytes buffer* —
     * generated once, copied never. Str mode stages through the stack and
     * flushes into the string.
     *
     * Both modes share one branch-free hot path: `base_`/`capacity_` always
     * describe the current write region, so put/write/cursor are a load and
     * a store with no mode test. (A per-write `direct_` select was measured
     * as a 6–8% whole-serializer regression — the mode check lives only in
     * the cold overflow path.)
     *
     * The block is sized to the previous document exactly (see
     * dumps_to_python), so on a repeated document the *last* reservation —
     * every writer reserves a small constant before it writes — no longer
     * fits the block although the content does. That reservation, and
     * everything after it, goes to the stage; take_bytes() copies the tail
     * into the block when the content fits, and only a document that really
     * outgrew the block pays a resize. The alternative, a block with
     * headroom shrunk in place, was measured to cost 13 µs per call on the
     * Windows runners and on macOS against 2 µs for an exact-fit block
     * (docs/decisions.md 2026-09-03).
     */
    StagedOutput() = default;

    ~StagedOutput() { Py_XDECREF(bytes_); }

    [[nodiscard]] bool init_bytes(size_t initial) {
        if (initial == 0)
            initial = 1; // never the shared empty singleton, which cannot be resized
        bytes_ = PyBytes_FromStringAndSize(nullptr, static_cast<Py_ssize_t>(initial));
        if (bytes_ == nullptr)
            return false;
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = initial;
        block_capacity_ = initial;
        block_used_ = 0;
        staged_ = false;
        used_ = 0;
        return true;
    }

    /// Finish bytes mode: land the staged tail, exact-size the object, hand
    /// it over. A block that already has the document's size is not resized.
    [[nodiscard]] PyObject* take_bytes() {
        if (failed_ || bytes_ == nullptr)
            return PyErr_NoMemory();
        size_t total = used_;
        if (staged_) {
            total = block_used_ + used_;
            if (total > block_capacity_) {
                if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(total)) != 0)
                    return nullptr; // bytes_ already cleared by the resize
                block_capacity_ = total;
            }
            std::memcpy(PyBytes_AS_STRING(bytes_) + block_used_, stage_, used_);
            staged_ = false;
        }
        if (total != block_capacity_ &&
            _PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(total)) != 0)
            return nullptr;
        PyObject* result = bytes_;
        bytes_ = nullptr;
        return result;
    }

    StagedOutput(const StagedOutput&) = delete;
    StagedOutput& operator=(const StagedOutput&) = delete;

    /// Make room for @p bytes of upcoming raw writes. Str mode: @p bytes must
    /// fit the stage (every caller asks for a small constant). Bytes mode:
    /// any size.
    void ensure(size_t bytes) {
        if (used_ + bytes > capacity_)
            overflow(bytes);
    }

    [[nodiscard]] char* cursor() noexcept { return base_ + used_; }
    void advance(size_t bytes) noexcept { used_ += bytes; }

    /// Take back @p bytes just written (never past an overflow).
    void rewind(size_t bytes) noexcept { used_ -= bytes; }

    /// One byte, after a covering ensure().
    void put(char c) noexcept { base_[used_++] = c; }

    /// @p len bytes, after a covering ensure().
    void write(const char* data, size_t len) noexcept {
        std::memcpy(base_ + used_, data, len);
        used_ += len;
    }

    /// Any length, no prior ensure needed.
    void write_spanning(const char* data, size_t len) {
        if (used_ + len <= capacity_) {
            std::memcpy(base_ + used_, data, len);
            used_ += len;
            return;
        }
        if (str_sink_ != nullptr) {
            flush_str();
            if (len >= sizeof(stage_) / 2) {
                str_sink_->append(data, len);
                return;
            }
            std::memcpy(stage_, data, len);
            used_ = len;
            return;
        }
        overflow(len);
        if (used_ + len > capacity_)
            return; // a failed grow: absorb the write, take_bytes() reports
        std::memcpy(base_ + used_, data, len);
        used_ += len;
    }

    /// A string sink for the rare escaped-string path, flushed first. In
    /// bytes mode this is a per-thread scratch the caller then spans in.
    [[nodiscard]] std::string& direct_sink() {
        if (str_sink_ != nullptr) {
            flush_str();
            return *str_sink_;
        }
        static thread_local std::string scratch;
        scratch.clear();
        return scratch;
    }

    /// Bytes mode only: true when direct_sink() handed out the scratch that
    /// still has to be written through write_spanning.
    [[nodiscard]] bool sink_is_scratch() const noexcept { return str_sink_ == nullptr; }

    /// Str mode only: land the stage in the string. In bytes mode the stage
    /// holds live document bytes that only take_bytes() may move.
    void flush_str() {
        if (str_sink_ != nullptr && used_ != 0) {
            str_sink_->append(stage_, used_);
            used_ = 0;
        }
    }

  private:
    /// The cold path: str mode flushes the stage; bytes mode stages the
    /// reservation that did not fit the block, and grows the block only
    /// once the stage is full too (or for a reservation wider than it).
    void overflow(size_t needed) {
        if (str_sink_ != nullptr) {
            flush_str();
            return;
        }
        if (failed_) {
            used_ = 0; // absorb writes safely in the scratch region
            return;
        }
        if (!staged_) {
            if (needed <= sizeof(stage_)) {
                block_used_ = used_;
                base_ = stage_;
                capacity_ = sizeof(stage_);
                used_ = 0;
                staged_ = true;
                return;
            }
            grow_block(used_ + needed);
            return;
        }
        // The stage is full: the document outgrew the block. Grow it, move
        // the stage in, and resume direct writes.
        const size_t total = block_used_ + used_;
        const size_t staged = used_;
        if (!grow_block(total + needed))
            return; // the stage is scratch now; nothing to move
        std::memcpy(base_ + block_used_, stage_, staged);
        used_ = total;
        staged_ = false;
    }

    /// Grow the block by half until it holds @p minimum (the block starts
    /// at the previous document's exact size, so doubling would spike a
    /// document that grew by a few kilobytes to twice its footprint); on
    /// failure the stage becomes scratch so raw stores stay in bounds and
    /// take_bytes() reports.
    bool grow_block(size_t minimum) {
        size_t grown = block_capacity_ + block_capacity_ / 2 + 1;
        while (grown < minimum)
            grown += grown / 2 + 1;
        if (_PyBytes_Resize(&bytes_, static_cast<Py_ssize_t>(grown)) != 0) {
            PyErr_Clear();
            failed_ = true;
            // bytes_ was cleared by the failed resize.
            base_ = stage_;
            capacity_ = sizeof(stage_);
            used_ = 0;
            staged_ = false;
            return false;
        }
        base_ = PyBytes_AS_STRING(bytes_);
        capacity_ = grown;
        block_capacity_ = grown;
        return true;
    }

    std::string* str_sink_ = nullptr;
    PyObject* bytes_ = nullptr;
    char* base_ = nullptr;
    size_t capacity_ = 0;
    size_t block_capacity_ = 0; ///< bytes mode: the block's size, whichever region is live
    size_t block_used_ = 0;     ///< bytes mode: bytes committed to the block before staging
    bool staged_ = false;       ///< bytes mode: the live region is the stage
    bool failed_ = false;
    size_t used_ = 0;
    char stage_[kStageBytes];
};

/**
 * The per-thread state a dumps() call leases: prepared keys and staged rows.
 *
 * Sharing is what lets repeated serialization of same-shaped payloads skip
 * key preparation entirely after the first call. The keys are *owned*
 * references: identity comparison is only sound while the objects live, and
 * a borrowed pointer could be freed between calls and reincarnated as a
 * different key. The busy flag covers re-entrancy -- a cycle warning can run
 * arbitrary Python, which can call dumps() again mid-walk; the nested call
 * pays for a private, empty state instead, so neither the outer walk's
 * schemas nor its staged rows are touched.
 */
class SchemaCacheLease {
  public:
    /// Keys per schema slot row; object width past this takes the plain walk.
    static constexpr size_t kSchemaSlots = 24;
    /// Bytes per inline `"key":` slot; a wider span falls back to heap spans.
    static constexpr size_t kSlotBytes = 16;
    /// Nesting levels with a schema slot and a staged row of their own;
    /// deeper objects take the plain walk (python_dumps.cpp kMaxCachedDepth).
    static constexpr size_t kMaxDepth = 64;

    /// Prepared `"key":` bytes for one object shape at one depth.
    ///
    /// The prepared bytes live *inline*, one fixed 16-byte slot per key, so
    /// the emit loop's whole read stream is this contiguous struct. The
    /// benchmark harness runs a full gc.collect() before every timed call,
    /// and the collector's traversal re-warms every dict's own internals —
    /// the memory a direct-walk serializer reads — while evicting side
    /// structures; with blob/offsets as separate heap allocations the emit
    /// path paid three cold hops per record under exactly that condition
    /// (parity isolated, behind under the harness, on every x86 leg).
    /// Spans wider than a slot keep the heap blob as the fallback.
    struct Schema {
        std::vector<PyObject*> keys;                ///< owned references
        std::string blob;                           ///< fallback bytes for wide spans only
        std::vector<uint32_t> offsets{0};           ///< fallback boundaries, keys.size() + 1
        uint8_t spans[kSchemaSlots] = {};           ///< inline `"key":` length per slot
        char slots[kSchemaSlots * kSlotBytes] = {}; ///< the inline bytes
        PyObject* key_row[kSchemaSlots] = {};       ///< borrowed copies for the verify scan
        bool prepared = false;
        bool wide = false; ///< some span exceeded a slot: emit from the blob

        /// Move-only, deliberately. `keys` holds *owned* references, so the
        /// one relocation this type sees -- `State::schemas` growing when a
        /// deeper level is first written -- must transfer them. A copy would
        /// duplicate the raw pointers into a second slot and hand
        /// `release_keys` two releases for one reference; deleting it makes
        /// that the compiler's rule rather than a comment's. No destructor is
        /// declared here on purpose: one would suppress the implicit moves and
        /// silently turn that relocation back into a copy.
        ///
        /// The moves are defaulted *without* an explicit `noexcept`: written
        /// out, the specification would be believed rather than computed
        /// (C++20 P1286R2), and a future throwing member would call
        /// `std::terminate` mid-relocation instead of failing the
        /// `is_nothrow_move_constructible_v` assertion below. Deduced, that
        /// assertion is a real guard.
        Schema() = default;
        Schema(Schema&&) = default;
        Schema& operator=(Schema&&) = default;
        Schema(const Schema&) = delete;
        Schema& operator=(const Schema&) = delete;

        void remember(PyObject* const* other, Py_ssize_t count) {
            // The one step that can allocate, taken before anything is
            // mutated: a throw here leaves the way exactly as it was, rather
            // than with its old references already dropped and a vector the
            // next remember() would release a second time. `assign` cannot
            // then allocate, so the rest of this runs to completion.
            keys.reserve(static_cast<size_t>(count));
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.assign(other, other + count);
            for (PyObject* key : keys)
                Py_INCREF(key);
            // Borrowed duplicates of the owned vector, inline: the per-record
            // verify scan reads this row instead of chasing the vector's heap
            // storage (cold after the harness's per-call gc.collect()).
            for (Py_ssize_t index = 0; index < count; ++index)
                key_row[static_cast<size_t>(index)] = other[index];
            prepared = false;
            wide = false;
        }

        /// Drop the remembered shape: release the owned keys and blank the
        /// borrowed row, leaving nothing an identity compare could hit. The
        /// prepared bytes are not cleared -- `prepared` is false, and
        /// `build_schema` resets `blob` and `offsets` before it writes them.
        void forget() {
            prepared = false;
            wide = false;
            for (PyObject* key : keys)
                Py_DECREF(key);
            keys.clear();
            for (PyObject*& remembered : key_row)
                remembered = nullptr;
        }

        /// Keys past the first, by identity. `select` has already matched the
        /// count and the first key from its inline arrays; only a plausible
        /// hit pays for touching this vector's heap storage. Identity, not
        /// equality: strata interns the keys it parses and CPython interns
        /// identifier-like literals, so same-schema records share key
        /// objects. A miss costs a rebuild, never a wrong answer.
        [[nodiscard]] bool matches_tail(PyObject* const* other, Py_ssize_t count) const noexcept {
            for (Py_ssize_t index = 1; index < count; ++index) {
                if (key_row[static_cast<size_t>(index)] != other[index])
                    return false;
            }
            return true;
        }
    };

    /**
     * One depth's schema set: four ways, scanned in place.
     *
     * A single slot per depth has a 0% hit rate on documents that rotate a
     * few record shapes -- mixed.json cycles four schemas round-robin and
     * missed every single object. Four ways cover that; the ways are *not*
     * reordered on a hit, because round-robin access is LRU's worst case:
     * move-to-front put every hit at the deepest way and paid a full
     * three-schema shuffle per object (a measured 45 ns regression before
     * this was flattened). A scan of up to four count-compares is near-free;
     * replacement just walks a round-robin cursor.
     *
     * `select` returns the matching way's index, or `kMiss` after
     * remembering the shape in the next victim way.
     */
    struct DepthSchemas {
        static constexpr size_t kWays = 4;
        static constexpr size_t kMiss = kWays;

        /// Scanned in place of the ways' own storage: each way's key count
        /// and first key, inline. A rejected way costs two loads from this
        /// cache line instead of a pointer chase into its `keys` vector --
        /// the scan is the per-object cost on shape-rotating documents, and
        /// with four ways it ran once per record on mixed.json.
        Py_ssize_t counts[kWays] = {-1, -1, -1, -1};
        PyObject* first_keys[kWays] = {nullptr, nullptr, nullptr, nullptr};
        Schema ways[kWays];
        uint8_t victim = 0;
        uint32_t misses = 0;
        /// True churn -- shapes that never repeat -- retires the depth so it
        /// stops paying remember()'s reference traffic.
        bool retired = false;

        /// Take one way out of service: it stops matching *before* its owned
        /// keys are released, so no select() can reach a slot whose shape is
        /// half gone. `counts` back at -1 is what un-matches it -- callers
        /// guarantee count >= 1, so -1 can never be asked for.
        void invalidate(size_t way) {
            counts[way] = -1;
            first_keys[way] = nullptr;
            ways[way].forget();
        }

        [[nodiscard]] size_t select(PyObject* const* keys, Py_ssize_t count) {
            if (retired)
                return kMiss;
            PyObject* const first = keys[0]; // callers guarantee count >= 1
            for (size_t way = 0; way < kWays; ++way) {
                if (counts[way] == count && first_keys[way] == first &&
                    ways[way].matches_tail(keys, count))
                    return way;
            }
            if (++misses > 64) {
                for (size_t way = 0; way < kWays; ++way)
                    invalidate(way);
                retired = true;
                return kMiss;
            }
            ways[victim].remember(keys, count);
            counts[victim] = count;
            first_keys[victim] = first;
            victim = static_cast<uint8_t>((victim + 1) % kWays);
            return kMiss;
        }
    };

    /**
     * The staged key and value rows of one dict nesting level.
     *
     * The dict writers read a record's keys and values once -- a walk that
     * runs no user code -- and then emit from that row, so the record that
     * reaches the output is the one the serializer read. The row is *leased*
     * rather than declared as a local array, because a local array in
     * write_mapping or write_record_fused is a local array in every function
     * they are inlined into: under `-fprofile-use` the whole dict writer
     * folds into the per-value dispatcher, and a 192-byte row there becomes a
     * 448-byte frame plus a stack-protector canary on `write` and
     * `write_sequence` -- paid by every value of every document, arrays of
     * scalars included (build/evidence/E26-P2/BUILDS.md). Leasing it keeps
     * the hot walk free of stack arrays under any inlining decision the
     * profile makes.
     *
     * One row per nesting level is exactly what the recursion can have live
     * at once: `map_depth_` is unique along the walk's path, so a level's row
     * belongs to the single write_mapping / write_record_fused frame at that
     * level, and a sibling record reuses it only after that frame -- and the
     * RowLock registered over it -- has gone.
     */
    struct StagedRow {
        PyObject* keys[kSchemaSlots];
        PyObject* values[kSchemaSlots];
    };

    /// One registration of a staged row on the serializer's row list.
    ///
    /// Leased for the second half of the same reason the rows are: an
    /// intrusive list node *is* an address-taken local, and a function with
    /// one gets a stack-protector canary under `-fstack-protector-strong`
    /// (the Linux legs' default) whatever its frame contains. Holding the
    /// node here leaves the frame guard two pointers the optimizer keeps in
    /// registers. Two nodes per level is what the recursion can have live:
    /// write_mapping_body registers its values row and, on the branch that
    /// still reads staged keys, its keys row; the fused writer registers one.
    struct RowNode {
        RowNode* next;
        PyObject* const* row;
        uint8_t first;
        uint8_t last;
        bool latched;
    };

    /// Everything one dumps() call leases: a schema set and a staged row per
    /// nesting level. Heap-resident and never moved, so a row pointer stays
    /// valid for the whole walk (`schemas` may reallocate; `rows` may not).
    struct State {
        std::vector<DepthSchemas> schemas;
        /// Indexed by dict nesting level. Index `kMaxDepth` is the scratch
        /// every deeper level stages into and *none of them reads*:
        /// write_mapping_body hands a level that deep to
        /// write_mapping_uncached, which walks the dict itself, before it
        /// touches a row. Levels never overlap in a row of their own, so
        /// only the levels past the cache share.
        StagedRow rows[kMaxDepth + 1];
        /// Two registration nodes per level, addressed rather than pushed:
        /// only levels inside the schema cache ever register one.
        RowNode locks[2 * (kMaxDepth + 1)];
#if defined(STRATA_RAW_DICT_WALK)
        /// Compaction scratch for general-kind tables: one per lease, not one
        /// per level. A compacted array is fully consumed -- into the staged
        /// row by the fused writer, into the keys/values row by
        /// write_mapping -- before any step that can recurse or run user
        /// code, which is the same point the raw `entries` pointer already
        /// stopped being readable (python_dumps.cpp, the fused writer's
        /// "never touches `entries` again"). So no two levels hold one, and
        /// the copy is in fact safer than the pointer: user code that frees
        /// the table cannot reach it.
        ///
        /// Leased for the reason StagedRow is: a stack array here is a stack
        /// array in every function the profile inlines its holder into, and
        /// it drags a stack-protector canary.
        rawdict::Entry general[rawdict::kScratchEntries];
#endif
    };

    SchemaCacheLease() {
        if (!busy_) {
            State* const state = shared();
            if (state != nullptr) {
                busy_ = true;
                state_ = state;
                owns_flag_ = true;
                return;
            }
        }
        // Re-entrant, or the thread's own state could not be allocated: a
        // private state, so the outer walk keeps its schemas and its rows.
        // A failed allocation is reported by ok(), not swallowed.
        fallback_.reset(new (std::nothrow) State);
        state_ = fallback_.get();
    }

    /// Ends the lease. The private state dies with the call, and its schemas
    /// own the keys they remembered, so they are released here -- the shared
    /// state's are not, because it outlives every call by design (see
    /// shared()).
    ///
    /// Safe on the exceptional exit too, and this is the last local of
    /// dumps_to_python to be destroyed, so every Frame, RowLock and the
    /// Serializer itself have already gone: nothing borrows these keys any
    /// more. The release cannot run user code or disturb a pending exception
    /// either -- only exact `str` keys are ever remembered (python_dumps.cpp
    /// `exact_keys`), and a `str` has neither `__del__` nor weakrefs.
    ~SchemaCacheLease() {
        if (fallback_)
            release_keys(*fallback_);
        if (owns_flag_)
            busy_ = false;
    }

    SchemaCacheLease(const SchemaCacheLease&) = delete;
    SchemaCacheLease& operator=(const SchemaCacheLease&) = delete;

    /// False when even the private state could not be allocated.
    [[nodiscard]] bool ok() const noexcept { return state_ != nullptr; }

    [[nodiscard]] State& state() noexcept { return *state_; }

  private:
    /// Release every owned key a private state remembered, way by way.
    ///
    /// `invalidate` un-matches the way before it releases, so the state is
    /// left consistent rather than half torn down -- it is about to be freed,
    /// but a slot that still matched a shape whose keys are gone is exactly
    /// the hazard E26-FIX2 closed, and there is no second rule here.
    static void release_keys(State& state) noexcept {
        for (DepthSchemas& depth : state.schemas) {
            for (size_t way = 0; way < DepthSchemas::kWays; ++way)
                depth.invalidate(way);
        }
    }

    [[nodiscard]] static State* shared() {
        // Deliberately leaked: a destructor after interpreter shutdown could
        // not legally Py_DECREF the owned keys anyway. That is the whole of
        // the shutdown policy -- the *private* state a re-entrant lease takes
        // is released instead, but inside the call that made it, with the
        // interpreter running (see ~SchemaCacheLease). Default-initialized,
        // not value-initialized: the staged rows are scratch that is always
        // written before it is read, and zeroing 25 KB per thread would buy
        // nothing.
        static thread_local State* instance = new (std::nothrow) State;
        return instance;
    }

    static thread_local bool busy_;

    std::unique_ptr<State> fallback_;
    State* state_ = nullptr;
    bool owns_flag_ = false;
};

inline thread_local bool SchemaCacheLease::busy_ = false;

// The keys a schema slot remembers are owned references, released one by one
// when a private lease ends. The only relocation such a slot ever sees is
// `State::schemas` growing as a deeper level is first written, and it has to
// move: a copy would leave two slots holding the same raw pointers, and the
// release would run twice on one reference.
static_assert(std::is_nothrow_move_constructible_v<SchemaCacheLease::DepthSchemas>,
              "schema slots must relocate by move, never by copy");
static_assert(!std::is_copy_constructible_v<SchemaCacheLease::DepthSchemas>,
              "schema slots own key references and must not be copyable");

} // namespace strata::bindings
