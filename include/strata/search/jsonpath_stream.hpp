#pragma once

/**
 * @file jsonpath_stream.hpp
 * @brief Streaming JSONPath evaluation over SAX events.
 *
 * The headline throughput feature: `search()` on a plain path evaluates the
 * expression *during* the parse and materializes nothing but the matches. A
 * 4 MB document queried for a thousand small values costs one validating scan
 * plus a thousand small objects — not a full tree.
 *
 * Scope is deliberately the fixed-depth subset: Field, Wildcard, and
 * non-negative Index selectors (`is_streamable`). Those paths have a length,
 * so a match's subtree sits strictly *below* the full-match depth and no new
 * match can begin inside a capture. That is the property the previous
 * implementation's streaming path lacked for recursive descent — it swallowed
 * an `a` nested inside a matched `a`, and `search()` disagreed with
 * `query()`. Excluding descent keeps the law true by construction; a slice or
 * filter needs sibling knowledge a stream does not have.
 *
 * The law also fixes the duplicate-key story: `query(load(f))` sees objects
 * *after* the duplicate-key policy collapsed them. Under the default
 * FirstWins policy that means a Field selector may only match the first
 * occurrence of its name per object, and a Wildcard over an object must skip
 * re-occurring keys — both handled here. Other policies cannot be replayed
 * from a stream without buffering whole objects, so callers gate on the
 * policy and fall back to load-then-query (the binding does exactly that).
 *
 * The Sink receives a match as ordinary SAX callbacks followed by
 * `finish_value()`, so a builder that already speaks SAX — the Python object
 * builder, the DOM builder — captures matches without new conversion code.
 */

#include "strata/search/jsonpath.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

/**
 * SAX handler that evaluates a streamable path and forwards matches to Sink.
 *
 * @tparam Sink Receives the standard value callbacks (`on_null` … `on_end_object`)
 *         while a match is being captured, then `finish_value()` once the
 *         match is complete. Must return false to abort.
 */
template <typename Sink> class StreamSearchHandler {
  public:
    /// @p path must satisfy is_streamable() and contain at least one selector.
    StreamSearchHandler(const CompiledPath& path, Sink& sink) : sink_(sink) {
        for (const PathStep& step : path) {
            if (step.op != PathOp::Root)
                selectors_.push_back(&step);
        }
    }

    bool on_null() {
        return value([&] { return sink_.on_null(); });
    }
    bool on_bool(bool v) {
        return value([&] { return sink_.on_bool(v); });
    }
    bool on_int(int64_t v) {
        return value([&] { return sink_.on_int(v); });
    }
    bool on_big_int(std::string_view text) {
        return value([&] { return sink_.on_big_int(text); });
    }
    bool on_double(double v) {
        return value([&] { return sink_.on_double(v); });
    }
    bool on_string(std::string_view v) {
        return value([&] { return sink_.on_string(v); });
    }

    bool on_key(std::string_view key) {
        if (capture_depth_ > 0)
            return sink_.on_key(key);
        if (levels_.empty())
            return true;
        Level& top = levels_.back();
        top.child_selected = false;
        if (!top.on_track || !top.is_object)
            return true;

        const PathStep& selector = *selectors_[active_depth_ - 1];
        switch (selector.op) {
        case PathOp::Field:
            // FirstWins: the first occurrence of the name is the one the
            // built dict would hold; later duplicates must not match.
            if (!top.field_done && key == selector.name) {
                top.child_selected = true;
                top.field_done = true;
            }
            break;
        case PathOp::Wildcard: {
            // FirstWins again: a wildcard over the built dict yields one
            // value per distinct key, so a re-occurring key is skipped.
            bool seen = false;
            for (const std::string& previous : top.seen_keys) {
                if (previous == key) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                top.seen_keys.emplace_back(key);
                top.child_selected = true;
            }
            break;
        }
        default:
            break; // an Index selects nothing from an object
        }
        return true;
    }

    bool on_start_object() { return open(/*is_object=*/true); }
    bool on_start_array() { return open(/*is_object=*/false); }

    bool on_end_object() { return close(/*is_object=*/true); }
    bool on_end_array() { return close(/*is_object=*/false); }

  private:
    /// One open container in the document.
    struct Level {
        bool on_track; ///< its children are tested against a selector
        bool is_object;
        bool child_selected;                ///< object child: decided by the preceding key
        bool field_done;                    ///< Field selector already matched in this object
        int64_t next_index;                 ///< array child position, counted at value starts
        std::vector<std::string> seen_keys; ///< Wildcard-over-object dedup
    };

    /// Whether the value now beginning is selected by its parent's selector.
    [[nodiscard]] bool child_selected() {
        if (levels_.empty())
            return false; // a scalar root has no children to select
        Level& top = levels_.back();
        if (!top.on_track)
            return false;
        if (top.is_object)
            return top.child_selected;

        const PathStep& selector = *selectors_[active_depth_ - 1];
        const int64_t position = top.next_index;
        switch (selector.op) {
        case PathOp::Wildcard:
            return true;
        case PathOp::Index:
            return position == selector.index;
        default:
            return false; // a Field selects nothing from an array
        }
    }

    /// Bookkeeping every value start shares: array positions advance, and an
    /// object's key decision is consumed exactly once.
    void consume_child_slot() {
        if (levels_.empty())
            return;
        Level& top = levels_.back();
        ++top.next_index;
        if (top.is_object)
            top.child_selected = false;
    }

    template <typename Forward> [[nodiscard]] bool value(Forward&& forward) {
        if (capture_depth_ > 0)
            return forward();
        const bool selected = child_selected();
        consume_child_slot();
        if (selected && active_depth_ == selectors_.size()) {
            // A whole scalar match: deliver and finish in one breath.
            if (!forward())
                return false;
            return sink_.finish_value();
        }
        return true;
    }

    [[nodiscard]] bool open(bool is_object) {
        if (capture_depth_ > 0) {
            ++capture_depth_;
            return is_object ? sink_.on_start_object() : sink_.on_start_array();
        }

        bool on_track = false;
        if (levels_.empty()) {
            // The root container is what the first selector applies into.
            on_track = !selectors_.empty();
        } else {
            const bool selected = child_selected();
            consume_child_slot();
            if (selected) {
                if (active_depth_ == selectors_.size()) {
                    // The selected value is a container: capture its subtree.
                    capture_depth_ = 1;
                    return is_object ? sink_.on_start_object() : sink_.on_start_array();
                }
                on_track = true;
            }
        }

        levels_.push_back(Level{on_track, is_object, false, false, 0, {}});
        if (on_track)
            ++active_depth_;
        return true;
    }

    [[nodiscard]] bool close(bool is_object) {
        if (capture_depth_ > 0) {
            // The close event belongs to the captured subtree; once the
            // depth returns to zero the sink takes the finished value.
            if (!(is_object ? sink_.on_end_object() : sink_.on_end_array()))
                return false;
            if (--capture_depth_ == 0)
                return sink_.finish_value();
            return true;
        }
        if (levels_.empty())
            return false;
        if (levels_.back().on_track)
            --active_depth_;
        levels_.pop_back();
        return true;
    }

    Sink& sink_;
    std::vector<const PathStep*> selectors_;
    std::vector<Level> levels_;
    size_t active_depth_ = 0;  ///< number of on-track containers open
    size_t capture_depth_ = 0; ///< >0 while forwarding a matched subtree
};

} // namespace strata
