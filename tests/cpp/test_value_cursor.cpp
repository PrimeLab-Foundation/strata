/**
 * @file test_value_cursor.cpp
 * @brief Contract suite for the parser's optional value-cursor capability.
 *
 * The capability (docs/architecture/value-cursor.md) lets a handler that
 * stages array elements lend the parser its writable range for the length of
 * one element loop, so the per-element store needs no round trip through the
 * handler. Everything the parser gains there it must pay for in ownership
 * discipline, and that is what this suite pins:
 *
 * - **Equivalence.** A cursor-capable handler and a plain twin that forwards
 *   only the JsonSaxHandler-style callbacks must produce the same tree, the
 *   same accept/reject verdict and the same stop position on every document.
 *   The twin is the same shape as the production streaming-search sink, which
 *   is exactly how a handler opts *out* of the capability.
 * - **Ownership.** Every value the handler created must be released, on every
 *   exit: a clean parse, a syntax error mid-array, a handler that refuses an
 *   element, and an allocation that fails inside the cursor's own store. The
 *   live-object counter standing at zero after the handler resets is the
 *   core-side analogue of the builder's reference invariant.
 * - **Growth.** The staging block starts small here on purpose, so arrays
 *   cross the reallocation boundary many times and every cursor handed back
 *   after a growth is proven to point into the new block.
 *
 * Style: plain `assert` + `main()`, no framework (docs/context/styleguide.md).
 */

#include "strata/json/json_parser_inline.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

namespace {

/// A built value. `live` counts the ones in existence: a handler that loses
/// track of a reference leaves it above zero, which is the failure this suite
/// is built to catch.
struct Cell {
    std::string text;         ///< scalar rendering, empty for containers
    std::vector<Cell*> items; ///< children, for both array and object cells
    bool container = false;

    static int live;

    [[nodiscard]] static Cell* make(std::string rendering) {
        if (budget == 0)
            return nullptr; // the simulated allocation failure
        if (budget > 0)
            --budget;
        ++live;
        return new Cell{std::move(rendering), {}, false};
    }

    [[nodiscard]] static Cell* make_container() {
        ++live;
        return new Cell{std::string(), {}, true};
    }

    static void drop(Cell* cell) {
        if (cell == nullptr)
            return;
        for (Cell* child : cell->items)
            drop(child);
        --live;
        delete cell;
    }

    /// Creations remaining before make() starts failing; negative = unlimited.
    static int budget;
};

int Cell::live = 0;
int Cell::budget = -1;

[[nodiscard]] std::string render(const Cell* cell) {
    if (cell == nullptr)
        return "<none>";
    if (!cell->container)
        return cell->text;
    std::string out = "(";
    for (size_t index = 0; index < cell->items.size(); ++index) {
        if (index != 0)
            out += ' ';
        out += render(cell->items[index]);
    }
    return out + ')';
}

/**
 * A handler with the capability: it stages array elements on its own stack
 * exactly the way PythonObjectBuilder does, including the raw three-pointer
 * layout that makes a borrowed range expressible.
 */
class TreeHandler {
  public:
    /// Declaring this member type is the whole opt-in.
    struct ValueCursor {
        Cell** next;
        Cell** limit;
    };

    ~TreeHandler() {
        reset();
        std::free(values_begin_);
    }

    void reset() noexcept {
        Cell::drop(root_);
        root_ = nullptr;
        for (Cell** slot = values_begin_; slot != values_next_; ++slot)
            Cell::drop(*slot);
        values_next_ = values_begin_;
        for (const Frame& frame : frames_)
            Cell::drop(frame.mapping);
        frames_.clear();
        keys_.clear();
    }

    [[nodiscard]] Cell* take_root() noexcept {
        Cell* root = root_;
        root_ = nullptr;
        return root;
    }

    /// Refuse the element at this index (counted over every scalar the parse
    /// produces); -1 disables the injection.
    void refuse_at(int index) noexcept { refuse_at_ = index; }

    // --- plain callbacks ---------------------------------------------------

    bool on_null() { return push(scalar("null")); }
    bool on_bool(bool value) { return push(scalar(value ? "true" : "false")); }
    bool on_int(int64_t value) { return push(scalar("i" + std::to_string(value))); }
    bool on_big_int(std::string_view text) { return push(scalar("B" + std::string(text))); }
    bool on_double(double value) { return push(scalar("d" + std::to_string(value))); }
    bool on_string(std::string_view value) { return push(scalar("s" + std::string(value))); }

    bool on_start_object() {
        Cell* mapping = Cell::make_container();
        frames_.push_back(Frame{mapping, values_size()});
        return true;
    }

    bool on_key(std::string_view key) {
        keys_.emplace_back(key);
        return true;
    }

    bool on_end_object() {
        if (frames_.empty())
            return false;
        Cell* mapping = frames_.back().mapping;
        frames_.pop_back();
        return push(mapping);
    }

    bool on_start_array() {
        frames_.push_back(Frame{nullptr, values_size()});
        return true;
    }

    bool on_end_array() {
        if (frames_.empty() || frames_.back().mapping != nullptr)
            return false;
        const size_t start = frames_.back().start;
        frames_.pop_back();
        Cell* list = Cell::make_container();
        for (Cell** slot = values_begin_ + start; slot != values_next_; ++slot)
            list->items.push_back(*slot);
        values_next_ = values_begin_ + start;
        return push(list);
    }

    // --- the cursor half ---------------------------------------------------

    [[nodiscard]] ValueCursor open_values() noexcept {
        return ValueCursor{values_next_, values_cap_};
    }

    void close_values(ValueCursor cursor) noexcept { values_next_ = cursor.next; }

    ValueCursor on_null_at(ValueCursor cursor) { return store(cursor, scalar("null")); }
    ValueCursor on_bool_at(ValueCursor cursor, bool value) {
        return store(cursor, scalar(value ? "true" : "false"));
    }
    ValueCursor on_int_at(ValueCursor cursor, int64_t value) {
        return store(cursor, scalar("i" + std::to_string(value)));
    }
    ValueCursor on_big_int_at(ValueCursor cursor, std::string_view text) {
        return store(cursor, scalar("B" + std::string(text)));
    }
    ValueCursor on_double_at(ValueCursor cursor, double value) {
        return store(cursor, scalar("d" + std::to_string(value)));
    }
    ValueCursor on_string_at(ValueCursor cursor, std::string_view value) {
        return store(cursor, scalar("s" + std::string(value)));
    }

  private:
    struct Frame {
        Cell* mapping;
        size_t start;
    };

    /// Build one scalar, honouring the refusal injection: a null return is a
    /// failed construction, which both placement paths must handle.
    [[nodiscard]] Cell* scalar(std::string rendering) {
        const bool refused = scalars_ == refuse_at_;
        ++scalars_;
        if (refused)
            return nullptr;
        return Cell::make(std::move(rendering));
    }

    [[nodiscard]] ValueCursor store(ValueCursor cursor, Cell* value) {
        if (value == nullptr) {
            close_values(cursor);
            return ValueCursor{};
        }
        if (cursor.next == cursor.limit)
            return store_grown(cursor, value);
        *cursor.next++ = value;
        return cursor;
    }

    ValueCursor store_grown(ValueCursor cursor, Cell* value) {
        close_values(cursor);
        if (!grow()) {
            Cell::drop(value);
            return ValueCursor{};
        }
        *values_next_++ = value;
        return ValueCursor{values_next_, values_cap_};
    }

    bool push(Cell* value) {
        if (value == nullptr)
            return false;
        if (frames_.empty()) {
            Cell::drop(root_);
            root_ = value;
            return true;
        }
        if (frames_.back().mapping == nullptr) {
            if (values_next_ == values_cap_ && !grow()) {
                Cell::drop(value);
                return false;
            }
            *values_next_++ = value;
            return true;
        }
        if (keys_.empty()) {
            Cell::drop(value);
            return false;
        }
        Cell* keyed = Cell::make_container();
        keyed->text = keys_.back();
        keys_.pop_back();
        keyed->items.push_back(value);
        frames_.back().mapping->items.push_back(keyed);
        return true;
    }

    [[nodiscard]] size_t values_size() const noexcept {
        return static_cast<size_t>(values_next_ - values_begin_);
    }

    [[nodiscard]] bool grow() {
        const size_t used = values_size();
        const size_t old_slots = static_cast<size_t>(values_cap_ - values_begin_);
        // Deliberately tiny, so the suite's arrays cross many reallocations.
        const size_t slots = old_slots == 0 ? 2 : old_slots * 2;
        auto* block = static_cast<Cell**>(std::realloc(values_begin_, slots * sizeof(Cell*)));
        if (block == nullptr)
            return false;
        values_begin_ = block;
        values_next_ = block + used;
        values_cap_ = block + slots;
        return true;
    }

    Cell* root_ = nullptr;
    Cell** values_begin_ = nullptr;
    Cell** values_next_ = nullptr;
    Cell** values_cap_ = nullptr;
    std::vector<Frame> frames_;
    std::vector<std::string> keys_;
    int scalars_ = 0;
    int refuse_at_ = -1;
};

/**
 * The plain twin: the same tree, built through the JsonSaxHandler-style
 * callbacks only. It declares no ValueCursor, so ParserInline selects
 * PlainCursor for it — the same way the streaming-search handler and the DOM
 * builder do in production.
 */
class PlainTreeHandler {
  public:
    bool on_null() { return inner_.on_null(); }
    bool on_bool(bool value) { return inner_.on_bool(value); }
    bool on_int(int64_t value) { return inner_.on_int(value); }
    bool on_big_int(std::string_view text) { return inner_.on_big_int(text); }
    bool on_double(double value) { return inner_.on_double(value); }
    bool on_string(std::string_view value) { return inner_.on_string(value); }
    bool on_start_object() { return inner_.on_start_object(); }
    bool on_key(std::string_view key) { return inner_.on_key(key); }
    bool on_end_object() { return inner_.on_end_object(); }
    bool on_start_array() { return inner_.on_start_array(); }
    bool on_end_array() { return inner_.on_end_array(); }

    void reset() noexcept { inner_.reset(); }
    [[nodiscard]] Cell* take_root() noexcept { return inner_.take_root(); }
    void refuse_at(int index) noexcept { inner_.refuse_at(index); }

  private:
    TreeHandler inner_;
};

static_assert(std::is_same_v<strata::value_cursor_of<TreeHandler>::type, TreeHandler::ValueCursor>,
              "a handler declaring ValueCursor must parse on it");
static_assert(std::is_same_v<strata::value_cursor_of<PlainTreeHandler>::type, strata::PlainCursor>,
              "a handler without ValueCursor must keep the plain dispatcher");

/// One parse outcome, comparable across the two handlers.
struct Outcome {
    bool ok;
    size_t consumed;
    std::string tree;
};

template <typename Handler> [[nodiscard]] Outcome run(std::string_view text, int refuse_at = -1) {
    Handler handler;
    handler.refuse_at(refuse_at);
    strata::ParserInline<Handler> parser{text.data(), text.size(), handler};
    const bool ok = parser.parse_value();
    Outcome outcome{ok, parser.i, std::string()};
    Cell* root = handler.take_root();
    outcome.tree = ok ? render(root) : std::string("<aborted>");
    Cell::drop(root);
    handler.reset();
    return outcome;
}

const char* const kDocuments[] = {
    // Scalars at the root: no array, so no cursor is ever opened.
    "null",
    "true",
    "false",
    "0",
    "-17",
    "12345678901234567890",
    "3.5",
    "1.7976931348623157e308",
    "\"plain\"",
    "\"esc\\u00e9\\n\"",
    // Flat arrays of every scalar class, long enough to cross the staging
    // block's growth boundary many times over.
    "[]",
    "[1]",
    "[null,null,null,null,null,null,null,null,null]",
    "[true,false,true,false,true,false,true]",
    "[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]",
    "[1000,20000,300000,4000000,50000000,600000000,7000000000]",
    "[0.5,1.25,3.14159,2.718281828459045,1e-9,-0.0,6.02e23]",
    "[99999999999999999999,-99999999999999999999,1]",
    "[\"a\",\"bb\",\"ccc\",\"dddd\",\"eeeee\",\"ffffff\",\"ggggggg\"]",
    "[\"\\u00e9\",\"\\t\",\"\\\"\",\"plain\"]",
    // Nested containers: every one of these closes the outer cursor, runs a
    // nested parse and re-opens it.
    "[[1,2],[3,4],[5,6]]",
    "[[[[[1]]]]]",
    "[[],[],[]]",
    "[{\"a\":1},{\"a\":2},{\"a\":3}]",
    "[1,[2,[3,[4,[5]]]],6]",
    "[{\"k\":[1,2,3]},[{\"k\":4}],5]",
    "{\"a\":[1,2,3],\"b\":{\"c\":[4,[5,6]]},\"d\":7}",
    "[null,{\"x\":[true,\"y\",{\"z\":[]}]},1.5,\"end\"]",
    // Rejections, at every position an array loop can fail at.
    "[",
    "[1",
    "[1,",
    "[1,]",
    "[,1]",
    "[1 2]",
    "[1,2",
    "[01]",
    "[1,,2]",
    "[[1],",
    "[{\"a\":1},",
    "[1,[2,]",
    "[\"unterminated]",
    "[tru]",
    "[nul]",
    "[1.]",
    "[1e]",
    "[\"a\"\"b\"]",
    "[}",
    "{\"a\":[1,}",
    "[1]]",
    "[1] junk",
};

/// Equivalence and ownership over the whole corpus.
void test_plain_and_cursor_agree() {
    for (const char* document : kDocuments) {
        const Outcome plain = run<PlainTreeHandler>(document);
        assert(Cell::live == 0);
        const Outcome cursor = run<TreeHandler>(document);
        assert(Cell::live == 0);
        if (plain.ok != cursor.ok || plain.consumed != cursor.consumed ||
            plain.tree != cursor.tree) {
            std::fprintf(stderr, "divergence on %s: plain(%d,%zu,%s) cursor(%d,%zu,%s)\n", document,
                         static_cast<int>(plain.ok), plain.consumed, plain.tree.c_str(),
                         static_cast<int>(cursor.ok), cursor.consumed, cursor.tree.c_str());
            assert(false);
        }
    }
}

/// A handler refusing element N: the parse aborts, and every element already
/// stored through the cursor is still the handler's to release.
void test_refused_element_releases_everything() {
    for (const char* document : kDocuments) {
        for (int refuse = 0; refuse < 12; ++refuse) {
            const Outcome plain = run<PlainTreeHandler>(document, refuse);
            assert(Cell::live == 0);
            const Outcome cursor = run<TreeHandler>(document, refuse);
            assert(Cell::live == 0);
            assert(plain.ok == cursor.ok);
            assert(plain.consumed == cursor.consumed);
            assert(plain.tree == cursor.tree);
        }
    }
}

/// The same, driven from the other side: construction starts failing after a
/// budget, which is the path a real MemoryError takes through store().
void test_failed_construction_releases_everything() {
    const char* const documents[] = {
        "[1,2,3,4,5,6,7,8,9,10,11,12]",
        "[[1,2,3],[4,5,6],[7,8,9]]",
        "{\"a\":[1,2,3],\"b\":[4,5,6]}",
        "[{\"a\":[1,2]},{\"b\":[3,4]},5]",
    };
    for (const char* document : documents) {
        for (int budget = 0; budget < 14; ++budget) {
            Cell::budget = budget;
            const Outcome plain = run<PlainTreeHandler>(document);
            assert(Cell::live == 0);
            Cell::budget = budget;
            const Outcome cursor = run<TreeHandler>(document);
            assert(Cell::live == 0);
            assert(plain.ok == cursor.ok);
            assert(plain.tree == cursor.tree);
        }
    }
    Cell::budget = -1;
}

/// A long array proves the cursor survives repeated reallocation of the
/// staging block: with a two-slot initial block this document grows it eleven
/// times, and every element must still land in order.
void test_growth_keeps_order() {
    std::string document = "[";
    std::string expected = "(";
    for (int index = 0; index < 2000; ++index) {
        if (index != 0) {
            document += ',';
            expected += ' ';
        }
        document += std::to_string(index);
        expected += 'i' + std::to_string(index);
    }
    document += ']';
    expected += ')';

    const Outcome cursor = run<TreeHandler>(document);
    assert(Cell::live == 0);
    assert(cursor.ok);
    assert(cursor.tree == expected);

    const Outcome plain = run<PlainTreeHandler>(document);
    assert(Cell::live == 0);
    assert(plain.tree == expected);
}

/// Nesting deeper than the cursor's own lifetime: each level opens, closes and
/// re-opens a cursor, and the innermost values must still reach the right one.
void test_deep_nesting_places_every_value() {
    std::string document;
    std::string closing;
    for (int depth = 0; depth < 60; ++depth) {
        document += "[1,";
        closing = "]" + closing;
    }
    document += "2";
    document += closing;

    const Outcome cursor = run<TreeHandler>(document);
    assert(Cell::live == 0);
    assert(cursor.ok);
    const Outcome plain = run<PlainTreeHandler>(document);
    assert(Cell::live == 0);
    assert(plain.tree == cursor.tree);
}

} // namespace

int main() {
    test_plain_and_cursor_agree();
    test_refused_element_releases_everything();
    test_failed_construction_releases_everything();
    test_growth_keeps_order();
    test_deep_nesting_places_every_value();
    std::puts("value cursor tests passed");
    return 0;
}
