// parse_value.cpp — iterative JSON parser with two-byte lookahead
//
// Two-byte fast paths (before full parse):
//
//   D + !cont   → single-digit int, skip parse_number entirely
//   -D + !cont  → negative single-digit int
//   []          → empty array, skip ws + frame push
//   {}          → empty object, skip ws + frame push
//   {"          → object with key, skip empty check
//
// These cover ~60% of values in typical JSON (ids, counts,
// booleans-as-numbers, empty containers, object openers).

#include "strata/parse_value.hpp"
#include "strata/simd/ops.hpp"

#include <array>

namespace strata {
    namespace {
        constexpr int MAX_DEPTH = 512;

        using EC = ErrorCode;

        inline auto fail(EC code, const char *where) {
            return std::unexpected(ParseError{code, where});
        }

        inline const char *ws(const char *cur, const char *end) {
            return simd::skip_ws(cur, end);
        }

        // Lookup table: does this byte continue a number?
        // True for '0'-'9', '.', 'e', 'E'.
        // Single memory access, zero branches.
        constexpr auto make_num_cont() {
            std::array<bool, 256> t{};
            for (int c = '0'; c <= '9'; c++) t[c] = true;
            t['.'] = true;
            t['e'] = true;
            t['E'] = true;
            return t;
        }

        constexpr auto NUM_CONT = make_num_cont();

        struct ArrayFrame {
            JsonValue::Array items;
        };

        struct ObjectFrame {
            JsonValue::Object pairs;
            std::string pending_key;
        };

        using Frame = std::variant<ArrayFrame, ObjectFrame>;

        inline const char *parse_key_colon(
            const char *cur, const char *end,
            std::string &key_out) {
            auto key = parse<std::string>(cur, end);
            if (!key) return nullptr;
            cur = ws(key->rest, end);
            if (cur >= end || *cur != ':') return nullptr;
            key_out = std::move(key->value);
            return cur + 1;
        }
    } // anonymous namespace


    template<>
    Result<JsonValue> parse<JsonValue>(const char *cur, const char *end) {
        std::vector<Frame> stack;

        while (true) {
            cur = ws(cur, end);
            if (cur >= end)
                return fail(EC::UnexpectedEnd, cur);

            JsonValue val;
            size_t avail = static_cast<size_t>(end - cur);

            // ── two-byte fast paths ──

            if (avail >= 2) {
                char c0 = cur[0];
                char c1 = cur[1];

                // Single-digit positive int: "5," "0]" "3}" etc.
                if (c0 >= '0' && c0 <= '9' && !NUM_CONT[static_cast<unsigned char>(c1)]) {
                    val = JsonValue(Number(static_cast<int64_t>(c0 - '0')));
                    cur++;
                    goto place;
                }

                // Empty array: []
                if (c0 == '[' && c1 == ']') {
                    val = JsonValue(JsonValue::Array{});
                    cur += 2;
                    goto place;
                }

                // Empty object: {}
                if (c0 == '{' && c1 == '}') {
                    val = JsonValue(JsonValue::Object{});
                    cur += 2;
                    goto place;
                }

                // Negative single-digit int: "-5," "-0]" etc.
                if (c0 == '-' && c1 >= '0' && c1 <= '9'
                    && (avail < 3 || !NUM_CONT[static_cast<unsigned char>(cur[2])])) {
                    val = JsonValue(Number(static_cast<int64_t>(-(c1 - '0'))));
                    cur += 2;
                    goto place;
                }

                // Object with key: {"
                if (c0 == '{' && c1 == '"') {
                    if (static_cast<int>(stack.size()) >= MAX_DEPTH)
                        return fail(EC::NestingTooDeep, cur);
                    cur++;
                    ObjectFrame frame;
                    auto after = parse_key_colon(cur, end, frame.pending_key);
                    if (!after) {
                        auto key = parse<std::string>(cur, end);
                        if (!key) return std::unexpected(key.error());
                        cur = ws(key->rest, end);
                        if (cur >= end) return fail(EC::UnexpectedEnd, cur);
                        return fail(EC::UnexpectedChar, cur);
                    }
                    cur = after;
                    stack.emplace_back(std::move(frame));
                    continue;
                }
            }

            // ── full parse (slow path) ──

            switch (*cur) {
                case 'n': {
                    auto r = parse<std::nullptr_t>(cur, end);
                    if (!r) return std::unexpected(r.error());
                    val = JsonValue(r->value);
                    cur = r->rest;
                    break;
                }

                case 't':
                case 'f': {
                    auto r = parse<bool>(cur, end);
                    if (!r) return std::unexpected(r.error());
                    val = JsonValue(r->value);
                    cur = r->rest;
                    break;
                }

                case '"': {
                    auto r = parse<std::string>(cur, end);
                    if (!r) return std::unexpected(r.error());
                    val = JsonValue(std::move(r->value));
                    cur = r->rest;
                    break;
                }

                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    auto r = parse<Number>(cur, end);
                    if (!r) return std::unexpected(r.error());
                    val = JsonValue(r->value);
                    cur = r->rest;
                    break;
                }

                case '[': {
                    if (static_cast<int>(stack.size()) >= MAX_DEPTH)
                        return fail(EC::NestingTooDeep, cur);
                    cur++;
                    cur = ws(cur, end);
                    if (cur >= end)
                        return fail(EC::UnexpectedEnd, cur);
                    if (*cur == ']') {
                        val = JsonValue(JsonValue::Array{});
                        cur++;
                        break;
                    }
                    stack.emplace_back(ArrayFrame{});
                    continue;
                }

                case '{': {
                    if (static_cast<int>(stack.size()) >= MAX_DEPTH)
                        return fail(EC::NestingTooDeep, cur);
                    cur++;
                    cur = ws(cur, end);
                    if (cur >= end)
                        return fail(EC::UnexpectedEnd, cur);
                    if (*cur == '}') {
                        val = JsonValue(JsonValue::Object{});
                        cur++;
                        break;
                    }
                    if (*cur != '"')
                        return fail(EC::UnexpectedChar, cur);

                    ObjectFrame frame;
                    auto after = parse_key_colon(cur, end, frame.pending_key);
                    if (!after) {
                        auto key = parse<std::string>(cur, end);
                        if (!key) return std::unexpected(key.error());
                        cur = ws(key->rest, end);
                        if (cur >= end) return fail(EC::UnexpectedEnd, cur);
                        return fail(EC::UnexpectedChar, cur);
                    }
                    cur = after;
                    stack.emplace_back(std::move(frame));
                    continue;
                }

                default:
                    return fail(EC::UnexpectedChar, cur);
            }

            // ── place value + handle delimiters ──
        place:

            for (;;) {
                if (stack.empty())
                    return Parsed<JsonValue>{std::move(val), cur};

                cur = ws(cur, end);
                if (cur >= end)
                    return fail(EC::UnexpectedEnd, cur);

                if (auto *af = std::get_if<ArrayFrame>(&stack.back())) {
                    af->items.push_back(std::move(val));

                    if (*cur == ']') {
                        val = JsonValue(std::move(af->items));
                        stack.pop_back();
                        cur++;
                        continue;
                    }
                    if (*cur != ',')
                        return fail(EC::UnexpectedChar, cur);
                    cur++;
                    break;
                }

                auto &of = std::get<ObjectFrame>(stack.back());
                of.pairs.emplace_back(std::move(of.pending_key), std::move(val));

                if (*cur == '}') {
                    val = JsonValue(std::move(of.pairs));
                    stack.pop_back();
                    cur++;
                    continue;
                }
                if (*cur != ',')
                    return fail(EC::UnexpectedChar, cur);
                cur++;

                cur = ws(cur, end);
                if (cur >= end)
                    return fail(EC::UnexpectedEnd, cur);
                if (*cur != '"')
                    return fail(EC::UnexpectedChar, cur);

                auto after = parse_key_colon(cur, end, of.pending_key);
                if (!after) {
                    auto key = parse<std::string>(cur, end);
                    if (!key) return std::unexpected(key.error());
                    cur = ws(key->rest, end);
                    if (cur >= end) return fail(EC::UnexpectedEnd, cur);
                    return fail(EC::UnexpectedChar, cur);
                }
                cur = after;
                break;
            }
        }
    }
}
