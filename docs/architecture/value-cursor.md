# Decision record: the value cursor (wave 24)

Opened 2026-09-05. Status: **measured on the Neoverse-N2; see the verdict**.

## Problem, with its evidence file

`loads wide_arrays` and `load wide_arrays` sit 1.04–1.10x behind orjson on
the Linux arm64 leg (a Neoverse-N2, 4 vCPU, Ubuntu clang 18, Python 3.12) on
every one of four five-platform CI samples — the one persistent deficit in
the standings, everything else on that leg and every other leg being either
#1 or shared-runner band (docs/decisions.md, 2026-09-05).

The leg was profiled down to the instruction. `parse_value` is 35.6% of
strata's process on that workload and `parse_number` 18.0%; orjson's own
parse never rises above `PyObject_Malloc` at 13.9% (profile runs 33948756816
and 33955161161). Inside `parse_value`, three groups own the self time:

| group                                      | share of `parse_value` |
| ------------------------------------------ | ---------------------- |
| the whitespace loop                        | ~15%                   |
| **the staging vector's push**              | **~11%**               |
| the instruction after object-creation calls | 6%                     |
| the string scan's NEON compare + index math | ~8%                    |

The staging push breaks down as the capacity compare `cmp x8, x10` at 6.7%,
the element store at 2.2%, and the end-pointer load and shift for the rest —
"an end pointer that round-trips through memory on every element", in the
words of that entry. Nothing in the function is a branch miss (0.7% of
branches): it is dependent arithmetic and memory round trips that an
eight-wide core overlaps and a five-wide one does not.

The whitespace loop is closed as a negative (a single-space fast path reads
−14% on `wide_arrays` on the M1). This record is about the second group.

## Why it is a round trip at all

`PythonObjectBuilder::push` places an array element by

```
ldp  x9, x8, [x0, #0x8]    ; frames_.__begin_, frames_.__end_   (empty check)
ldur x1, [x8, #-0x28]      ; frames_.back().mapping             (array or dict)
ldp  x9, x8, [x0, #0x28]   ; values_.__end_, values_.__end_cap_
cmp  x9, x8
str  x19, [x9], #0x8       ; *end++ = value
str  x9, [x0, #0x28]       ; values_.__end_ = end
```

— four loads and two stores per element, all through the builder object.
They cannot be hoisted out of the loop: every element is produced by an
opaque call (`PyLong_FromLongLong`, `PyUnicode_New`, `PyFloat_FromDouble`)
that the compiler must assume can reach the builder, so the end pointer is
reloaded after each one. That is not a missed optimisation; it is what the
language guarantees, and it is why **the already-recorded negative "a cached
pointer to the innermost frame in the builder's `push`" failed** — the
cached pointer had to be reloaded after every object-creating call anyway
and cost one-digit integers +3–4% on the M1 (docs/performance/SKILL.md,
wave 20 follow-up).

The conclusion that follows is the design: caching the pointer *inside the
builder* cannot work; the pointer has to live somewhere the opaque calls
provably cannot reach — a local of the parser.

## Decision

Add an **optional handler capability, the value cursor**, detected by
`strata::value_cursor_of<Handler>` on the presence of a member type
`Handler::ValueCursor`:

```cpp
struct ValueCursor { PyObject** next; PyObject** limit; };  // two words

ValueCursor open_values();                     // lend [next, limit)
void        close_values(ValueCursor);         // publish `next` back
ValueCursor on_null_at(ValueCursor);           // ... one per scalar event
ValueCursor on_bool_at(ValueCursor, bool);
ValueCursor on_int_at(ValueCursor, int64_t);
ValueCursor on_big_int_at(ValueCursor, std::string_view);
ValueCursor on_double_at(ValueCursor, double);
ValueCursor on_string_at(ValueCursor, std::string_view);
```

`ParserInline::parse_array` opens a cursor after `on_start_array`, threads it
through the element loop **by value** — `parse_value` and `parse_number` take
it in registers and return it in registers, so the two words survive even the
deliberately non-inlined number head — and closes it once, at the loop's
single exit. The per-element store becomes

```
cmp x1, x2                 ; next == limit, both in registers
str x8, [x1], #0x8         ; *next++ = value
```

`PythonObjectBuilder`'s staging vector becomes three raw pointers
(`values_begin_ / values_next_ / values_cap_`) rather than a `std::vector`,
because a vector has no way to publish a writable range past its own end.
`Frame::start` was already an index, which is what makes the block
relocatable under growth.

### The invariant that makes it safe

**The cursor is live only inside one array's element loop, and every route
that can reach the builder's stack by any other means is preceded by
`close_values`.** Concretely:

- a nested `[` or `{` closes the cursor, runs the nested parse (which opens
  its own cursor, or inserts into a dict), and re-opens afterwards — so
  `on_start_array`/`on_start_object` read a current `values_size()`, and
  `on_end_array`/`on_end_object` push into a current stack;
- every failure — syntax error, refused element, failed object construction,
  failed growth — closes the cursor before returning the failure token, so
  the value-initialised cursor a caller sees always means "already closed";
- `on_end_array` runs after the loop's single `close_values`;
- growth publishes the cursor, reallocates, and hands back a fresh one
  (`store_grown`, out of line: once per doubling, not once per element).

Ownership therefore never changes hands: every reference in
`[values_begin_, values_next_)` belongs to the builder at every point where
control can leave the parser, and `reset()` is what releases them, exactly as
before.

Re-entrancy is unchanged and unaffected: the only way arbitrary Python runs
mid-parse is a duplicate-key warning filter, which is reached from
`insert_into_object` — inside an object, i.e. with the enclosing array's
cursor already closed — and a re-entrant `loads` leases its own builder
(`BuilderLease`), so it cannot see the outer stack at all.

### Handlers without the capability

`value_cursor_of` falls back to `strata::PlainCursor`, a single `bool`
carrying "no failure yet". `parse_value(PlainCursor)` and
`parse_number(PlainCursor)` compile to the boolean dispatcher they replaced —
verified instruction for instruction on the null path — and every
`if constexpr` cursor operation erases. The DOM builder, the NDJSON stream,
the streaming-search handler and every `JsonSaxHandler` subclass keep exactly
the code they had. The no-argument `parse_value()` / `parse_number()`
overloads remain for the root, for object members and for the number suites.

## Alternatives considered

1. **Cache the innermost frame pointer in the builder.** Already measured,
   already a recorded negative: the pointer must be reloaded after every
   object-creating call, and it cost +3–4% on one-digit integers.
2. **Fill the list in place** — presize a `PyList` from the depth's last
   array and `PyList_SET_ITEM` as elements arrive. Already measured, already
   a recorded negative: every scalar class +2–4 ns per element with
   `PyList_Append` past the hint, +0.2–0.3 ns and users.json +8% with the
   overflow staged; "the staging vector with one exact list at the close is
   the cheapest strategy measured" (2026-09-04). This record keeps that
   strategy and changes only where the end pointer lives.
3. **Keep `std::vector` and reserve a span.** A vector cannot be told "the
   end is now here" without `resize`, which value-initialises the slots the
   parser just wrote. Rejected on correctness, not on speed.
4. **A separate cursor-carrying copy of `parse_value` / `parse_number`.**
   Rejected: two definitions of number classification is exactly the
   duplication the styleguide's one-obvious-way rule forbids, and the classes
   are already this delicate. Templating the one definition on the cursor
   type keeps a single source; it does cost a second instantiation in the
   binary for the Python builder (see consequences).
5. **Pass the cursor by reference.** Rejected on codegen: taking its address
   puts it back in memory the moment it reaches the deliberately non-inlined
   `parse_number`, which is where the target workload's elements are made.

## Consequences

- **Two instantiations** of `parse_value` and `parse_number` in the binding
  translation units (one on `ValueCursor`, one on `PlainCursor`). The M11
  L1I analysis (docs/decisions.md, 2026-08-15) is the standing warning about
  code growth on 32 KB-L1I x86 runners; the two copies are used in disjoint
  contexts (array elements vs object members and the root), but this is the
  change's main portability risk and the x86 legs are where it would show.
- One extra `close_values` + `open_values` pair per **nested container inside
  an array** — two register-to-memory moves per record on array-of-records
  documents (users.json, mixed.json), against one `push` before. Small, but
  in the wrong direction for exactly those datasets.
- The staging block is now `std::realloc`-managed and starts at 64 slots
  instead of growing from one, and is kept across `reset()` as the vector's
  capacity was.
- The SAX contract in `docs/context/api.md` and
  `include/strata/json/json_sax_handler.hpp` is unchanged: the capability is
  additive and invisible to every handler that does not declare it.

## The measurements this rests on

- The N2 instruction annotation naming the staging push at ~11% of
  `parse_value`, which is 35.6% of the process (profile runs 33948756816,
  33955161161).
- The two recorded negatives above, which between them rule out every
  builder-side placement of the same pointer.
- Generated-code proof that the round trip is gone (arm64, clang -O3, the
  `python_loads.cpp` translation unit): baseline `push` does
  `ldp/cmp/str/str` through the builder per element; the cursor build does
  `cmp x1, x2` + `str x8, [x1], #0x8`, with `parse_array` loading
  `values_next_ / values_cap_` once per array (`ldp x8, x2, [x9, #0x28]`)
  and keeping them in x8/x2 across every `parse_value` call of the loop.

## The measurements it needs

- **The N2 per-element table** (nulls, bools, ints by width, floats) and
  `perf stat` on 400 `loads wide_arrays`, against the same-day main baseline:
  nulls 7.89, bools 10.72, ints 0–9 10.10, ints 4-digit 28.59, floats 6dp
  41.13 ns/element; 1.849 s, 20.86 G instructions, 4.14 G branches.
  Instructions per parse is the direct test of this change: four loads and
  two stores per element removed should be visible in the instruction count
  before it is visible in the time.
- **The M1 quiet-machine roll**, which is the gate every per-token lever in
  this area has failed: the compact array loop (−6%), the single-space
  whitespace path (−14%), the cached frame pointer (+3–4% on small ints).
  A change that helps the N2 and loses on the M1 does not merge.
- **The x86 legs**, for the code-growth risk of the second instantiation.

## Kill criterion

No improvement in instructions per `loads wide_arrays` parse on the N2, or
any regression on the M1 per-element classes or on the small-tier rows —
then this record closes as a negative beside its two predecessors, and the
`push`-through-the-builder placement stands as the cheapest one measured.
