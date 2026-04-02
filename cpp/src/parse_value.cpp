// parse_value.cpp — iterative JSON parser with explicit stack
//
// Recursive logic, iterative implementation.
// Call stack replaced by std::vector<Frame> on heap.
//
//   Outer loop: parse one value (primitive or open container)
//   Inner loop: place value, handle delimiters, close containers
//
// Benefits over recursive version:
//   - No stack overflow on deep nesting (heap-allocated)
//   - Controllable depth limit (MAX_DEPTH)
//   - Better cache locality (frames in contiguous vector)
//   - Arena-friendly (future: replace vector with arena)

#include "strata/parse_value.hpp"
#include "strata/simd/ops.hpp"

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

        struct ArrayFrame {
            JsonValue::Array items;
        };

        struct ObjectFrame {
            JsonValue::Object pairs;
            std::string pending_key;
        };

        using Frame = std::variant<ArrayFrame, ObjectFrame>;

        // Parse object key + colon, store key in frame.
        // Caller guarantees *cur == '"'.
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

            // ── parse one value ──

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
                        // Re-parse to get the exact error
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
