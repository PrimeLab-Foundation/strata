# Decision record: the value cursor (wave 25)

Opened 2026-09-05, adopted 2026-09-06. Status: **measured on the Neoverse-N2 twice; a nesting cap added after the change was refuted on stack safety; adopted on the M1 same-venv A/B, its in-process re-measure and a PGO-against-PGO A/B against HEAD's own build (docs/decisions.md, 2026-09-06), all taken on a loaded machine — a quiet-machine roll with `regression_check` and the five-platform CI sample are still owed.**

## Problem, with its evidence file

`loads wide_arrays` and `load wide_arrays` sit 1.04–1.10x behind orjson on
the Linux arm64 leg (a Neoverse-N2, 4 vCPU, Ubuntu clang 18, Python 3.12) on
every one of four five-platform CI samples — the one persistent deficit in
the standings, everything else on that leg and every other leg being either
at #1 or inside the shared-runner band (docs/decisions.md, 2026-09-05).

The leg was profiled down to the instruction. `parse_value` is 35.6% of
strata's process on that workload and `parse_number` 18.0%; orjson's own
parse never rises above `PyObject_Malloc` at 13.9% (profile runs 33948756816
and 33955161161). Inside `parse_value`, three groups own the self time:

| group                                       | share of `parse_value` |
| ------------------------------------------- | ---------------------- |
| the whitespace loop                         | ~15%                   |
| **the staging vector's push**               | **~11%**               |
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
carrying "no failure yet"; every `if constexpr` cursor operation erases, and
`parse_value(PlainCursor)` is the boolean dispatcher it replaced. The DOM
builder, the NDJSON stream, the streaming-search handler and every
`JsonSaxHandler` subclass therefore keep the behaviour they had — pinned, not
assumed: `tests/cpp/test_value_cursor.cpp` runs a cursor-capable handler and
a plain twin over one corpus and requires the same tree, the same
accept/reject verdict and the same stop position on every document. The
no-argument `parse_value()` / `parse_number()` overloads remain for the root,
for object members and for the number suites.

The generated code on that path is close but **not** identical, and the
first version of this record said it was — which is how the change nearly
shipped a halved stack budget on a path it claimed not to touch. What is
actually true, `clang -O3 -march=native` on the development host (macOS
arm64) over the `json_parse.cpp` translation unit, i.e. the DOM builder's
instantiation:

| function (DOM path)     | main     | cursor    | note                                |
| ----------------------- | -------- | --------- | ----------------------------------- |
| `parse_value` insns     | 412      | 454       | +10%                                |
| `parse_value` **frame** | **96 B** | **144 B** | +50%: three more callee-saved pairs |
| `parse_string` insns    | 310      | —         | folded away                         |
| `scan_string` insns     | —        | 217       | the new out-of-line half            |
| `parse_number` insns    | 259      | 243       |                                     |
| plain trio, total       | 981      | 914       | −7%                                 |

`parse_array` and `parse_object` are inlined into `parse_value` on both
sides, so that frame *is* one level of nesting. The growth is where the
string case went: `parse_string` used to be a tail call out of the
dispatcher and is now `scan_string` plus a dispatch step inside it, and the
register pressure that follows spills x23–x28 as well. Two attempts to give
the plain path its old shape back — an out-of-line `parse_string_plain`, and
the original early-return element loop under `if constexpr` — each left the
frame at 144 bytes, so the cause is the dispatcher's live range, not either
of those shapes; both were reverted rather than kept as unmeasurable
duplication.

Behaviour on that path is unchanged and pinned (`test_value_cursor.cpp`);
the stack cost is not, and it is what the next section is about.

### How the invariant is checked, not asserted

The invariant above is the whole risk of the change: a staged reference the
builder loses on an abort is a leak, and one it releases twice is a crash.
Four gates cover it, all green on `exp/cursor`:

- `tests/cpp/test_value_cursor.cpp` — a cursor-capable handler and a plain
  twin over one corpus (same tree, same accept/reject, same stop position),
  plus a refused element, a failed object construction, growth mid-array and
  deep nesting, each asserting every staged reference is accounted for.
- `tests/unit/test_loads.py` — aborts at every level of a nested document
  release every staged element; the singleton refcounts (`None`, `True`,
  `False`, small ints) return to their pre-parse values after an abort inside
  an array; nested containers inside arrays match the stdlib oracle; a
  re-entrant `loads` from a duplicate-key warning filter builds both trees.
- The whole ctest registry rebuilt with `-fsanitize=address,undefined -fno-sanitize-recover=all`: 15/15 pass, `test_value_cursor` included.
- A differential harness over 400 nested documents, 13 578 mutated documents
  (each rejected exactly where stdlib `json` rejects it) and 200 re-entrant
  parses, reading `sys.getallocatedblocks()` across repeats.

`make test-py-asan` — the sanitized *Python* gate — **passes on this branch
on the development host**: 1794 tests in 56 s, bindings clean under
ASan + UBSan. The earlier claim in this record, that it "cannot run on the
development host at all", was a property of one interpreter and not of the
platform: macOS refuses to preload the Apple-clang sanitizer runtime into a
python.org framework interpreter ("Sanitizer load violates platform policy"),
and the Homebrew one takes it (`BASE_PYTHON=/opt/homebrew/bin/python3.14`,
which is what the repository's own `.venv` is built from). CI's `corpus` and
`bindings-asan-macos` jobs run it as well.

The differential's re-entrant section reads an allocated-block drift of ~350
over its first 200 parses, which looks like a small leak and is not: running
the identical loop four times in one process reads 354, 1, 1, 1. It is the
staging block being warmed once — the design keeps it across `reset()`, as
the vector's capacity was kept — and nothing accumulates per parse, per abort
or per re-entry.

## The nesting cap this change forced

### What the refuter found

The parser recurses, one frame per open container, and nothing capped it —
`json_parser_inline.hpp` said so in as many words ("Recursion is the depth
limit"), and `docs/architecture/SKILL.md` carried it as contributor
invariant 1. So the frame growth above is not a footnote: it is a **33%
reduction in the depth `loads()` survives**, and past that point strata does
not raise, it dies with the process. Measured by binary search over a
subprocess (`depth_probe.py` / `depth_search.py`), macOS arm64, one sweep
per row (a repeat of the 1 MB rows reproduced them exactly, and the three
stack sizes are collinear at 96 and 144 bytes per level, which is the
cross-check that matters — those are the disassembled frames):

| stack                               | main    | cursor  | ratio |
| ----------------------------------- | ------- | ------- | ----- |
| 1 MB thread, arrays                 | 11 026  | 7 351   | 0.67  |
| 1 MB thread, objects                | 11 024  | 8 268   | 0.75  |
| 2 MB thread, arrays                 | 21 949  | 14 632  | 0.67  |
| 8 MB thread, arrays                 | 87 485  | 58 323  | 0.67  |
| 8 MB main thread                    | 174 647 | 116 432 | 0.67  |
| 1 MB thread, `return_type="cursor"` | 11 022  | 7 347   | 0.67  |

(The refuter that found this called it "roughly halves"; measured, the
surviving depth drops by a third — 0.67x, consistently, on every stack size
and both paths. The conclusion is the same, and the cap is sized from the
frame, not from the ratio.)

The slope across stack sizes is 96 bytes per level on main and 144 on the
cursor build — exactly the frames disassembled above, and identical on the
**no-capability** path, which is the second thing the header got wrong.
Nothing in either test suite saw it: the deep-nesting tests use depth 100,
four orders of magnitude short of the cliff.

### Decision

**Cap nesting at `strata::kMaxNestingDepth` = 1024 open containers**, in the
one place every path goes through — `ParserInline::parse_array` and
`parse_object`, before the handler is told the container started, so a
refused document never leaves a handler with an unbalanced start event. The
level is held by an RAII `Level` (increment, decrement on scope exit, one
`too_deep` flag for the way out), `parse_sax_inline` turns that flag into
`Status::DepthExceeded`, and the binding layer turns *that* into
`ValueError("Maximum nesting depth exceeded")` — or, per NDJSON line,
`"Maximum nesting depth exceeded on line N"`. Because the cap lives in the
parser and not in a builder, `loads` (str and bytes), `load`,
`return_type="cursor"` (the C++ `JsonDocument`), every NDJSON line and the
streaming-search leg inherit it from one definition.

It is a *refusal*, kept distinct from `"Invalid JSON"`: a caller can tell
"your document is too deep" from "your document is broken", and a document
that is both reports the depth, because the cap stops the parse first.

### Why 1024

Sized from the smallest stack strata must run on — Windows' 1 MB main
thread — against the largest measured frame, then rounded down hard:

- 144 bytes per level on the deepest path (nested arrays, Python builder),
  so a 1 MB stack holds ~7 300 levels; 1024 is a **7.1x** margin on that
  arithmetic.
- Measured end to end rather than extrapolated: the smallest thread stack on
  which a `loads` of a document at exactly 1024 still completes — CPython's
  own frames and the probe included — is **160 KB** for nested arrays,
  144 KB for the mixed and cursor paths, 128 KB for objects. Against 1 MB
  that is a **6.4x** margin, which is the number to quote: it tolerates a
  build whose frames are six times clang's.
- Within the market: orjson caps at the same 1024 and refuses the 1025th; stdlib `json` is bounded by the interpreter's recursion guard, which varies by version (depth 100 000 parses on 3.14).
- Nothing in the repository comes near it: the deepest committed documents
  are the depth-100 stress tests and a depth-60 fuzz seed (`deep_arrays`),
  both measured by counting brackets.

The cost is one compare, one increment and one decrement per **container** —
not per element, which is where this record's gains are. On the N2 the
per-element table is the direct test that it did not eat them.

### What it is pinned by

- `tests/cpp/test_stress_deep.cpp` — depth 1024 parses on arrays, objects
  and alternating nesting, and the tree is walked and destroyed at that
  depth; 1025 is `DepthExceeded`; so are 4100 levels, an unbalanced 1025 and
  a document that is both too deep and malformed; NDJSON reports the failing
  line, and `skip_errors` skips it.
- `tests/cpp/test_value_cursor.cpp` — the capable handler and the plain twin
  refuse at the same depth, at the same byte, with everything staged
  released (`Cell::live == 0`).
- `tests/cpp/test_jsonpath_stream.cpp` — the streaming leg refuses what
  `parse_json` refuses, and accepts what it accepts, at the boundary.
- `tests/unit/test_loads.py`, `tests/unit/test_load_dump.py` — the message,
  on `str` and `bytes`, on arrays/objects/mixed, through `loads`, `load`,
  cursor mode, NDJSON (eager, lazy, `skip_errors`), folder `load` and
  `search`; that a refusal leaves nothing behind that lowers the next
  document's ceiling; and that 200 000 levels raise rather than crash.

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
- The SAX contract in `include/strata/json/json_sax_handler.hpp` is
  unchanged: the capability is additive and invisible to every handler that
  does not declare it. `docs/context/api.md` *does* change, for the nesting
  cap — a new refusal on every parsing entry point, and the one
  user-visible behaviour difference in this change.
- **The plain path costs 48 more bytes of stack per level** (96 → 144) and
  ~10% more instructions in `parse_value`, on the DOM/cursor, NDJSON-C++ and
  streaming-search handlers, which the N2 profile did not measure because it
  measured `loads`. The cap removes the safety consequence; the speed
  consequence on `search` is unmeasured and is this change's second
  portability risk after the two instantiations.

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
  41.13 ns/element; 1.850 s, 20.86 G instructions, 4.14 G branches.
  Instructions per parse is the direct test of this change: four loads and
  two stores per element removed should be visible in the instruction count
  before it is visible in the time.
- **The M1 roll**, which is the gate every per-token lever in this area has failed: the compact array loop (−6%), the single-space whitespace path (−14%), the cached frame pointer (+3–4% on small ints). A change that helps the N2 and loses on the M1 does not merge. Taken 2026-09-06 with drift-free instruments on a loaded machine ("What the M1 measured", below); the quiet-machine `regression_check` roll is still owed.
- **The x86 legs**, for the code-growth risk of the second instantiation.

## Kill criterion

No improvement in instructions per `loads wide_arrays` parse on the N2, or
any regression on the M1 per-element classes or on the small-tier rows —
then this record closes as a negative beside its two predecessors, and the
`push`-through-the-builder placement stands as the cheapest one measured.

**Disposition, 2026-09-06: it did not fire.** The N2 read −9.8% instructions and −14.5% elapsed on 400 `loads wide_arrays` against code-current main; no M1 per-element class regressed. The tier roll put two parse medians past +2% — large `loads nested` +2.7% and small `load mixed` +4.0% on a 0.17 ms row — and both read −0.3% and −1.8% in a 40-repeat, three-round in-process re-measure whose controls read −2.9%, −0.9% and −0.4% (docs/decisions.md, 2026-09-06).

## What the N2 measured

Three profile runs on `exp/cursor` (33970232962 on the parser change, 33970851049 on the delivered tree, 33985005769 on the same code with this record added — cursor r1, r2 and r3 in the tables below) and two on the capped `exp/cursor2` (33987499471 and 33988508233, +cap r1 and r2), against the same-day main baseline
(33947097598 / 33955161161). PGO+LTO, Python 3.12, Ubuntu clang 18.

Per element, `benchmarks/parse_elements_probe.py`, ns per element (a list of
2000 of one kind, median of 30 rounds):

| class        | main  | cursor r1 | cursor r2 | change | orjson (3 runs)       |
| ------------ | ----- | --------- | --------- | ------ | --------------------- |
| nulls        | 7.89  | 4.34      | 4.50      | −44%   | 3.63 / 3.61 / 3.62    |
| bools        | 10.72 | 4.94      | 5.06      | −53%   | 6.63 / 6.32 / 6.65    |
| ints 0–9     | 10.10 | 7.48      | 7.68      | −25%   | 6.79 / 6.79 / 6.80    |
| ints 100–999 | 24.56 | 21.38     | 21.13     | −14%   | 18.82 / 18.96 / 18.85 |
| ints 4-digit | 28.59 | 25.24     | 25.24     | −12%   | 18.81 / 18.81 / 18.86 |
| ints 7-digit | 28.90 | 25.18     | 25.32     | −13%   | 20.02 / 19.92 / 20.21 |
| ints 9–10dig | 37.05 | 30.92     | 31.20     | −16%   | 23.22 / 23.21 / 23.13 |
| floats 6dp   | 41.13 | 36.37     | 36.48     | −11%   | 25.25 / 25.30 / 25.27 |
| floats 17dig | 45.71 | 40.92     | 41.02     | −10%   | 33.36 / 33.34 / 33.36 |

orjson's own column moves by at most 0.4% across the three runs, which is
what makes the strata column readable as the change and not the draw. Bools
cross over: 1.62x behind before, 0.76x ahead after.

`perf stat`, 400 `loads` of small `wide_arrays.json`. The main column that
counts is the code-current one — f4bb217, run 33955161161, the tree after
wave 23's bytes path; the earlier main draw (1cc7049, run 33947097598,
before that change) is kept because the first version of this record
compared against it alone:

| counter             | main 1cc7049 | main f4bb217 | cursor r1 | cursor r2 | cursor r3 | **+cap r1** | **+cap r2** |
| ------------------- | ------------ | ------------ | --------- | --------- | --------- | ----------- | ----------- |
| strata elapsed      | 1.850 s      | 1.912 s      | 1.673 s   | 1.667 s   | 1.596 s   | 1.635 s     | 1.616 s     |
| strata instructions | 20.86 G      | 21.22 G      | 19.22 G   | 19.26 G   | 18.63 G   | 19.15 G     | 19.13 G     |
| strata / orjson     | 1.026x       | 1.118x       | 0.938x    | 0.976x    | 0.896x    | 0.932x      | 0.940x      |

Against current main the capped build is −9.8% instructions and −14.5%
elapsed (−8.2% / −11.6% on the worst pairing across every draw); every main
draw sits above 1.02x and every cursor-family draw below 0.98x, and orjson's
own instruction count is flat across all of them (19.02–19.50 G). The capped
build sits *inside* the uncapped spread — cursor r3, byte-identical code to
r2, drew fewer instructions and less time than either capped draw — so the
cap's cost is below this instrument's resolution here, which is also what
the arithmetic says: a `wide_arrays` parse opens 2 501 containers for
128 500 scalars, and 2 501 × 400 × three to ten instructions is 0.016–0.05% of the count. That is why this file cannot answer the cost question for
container-dense data (users.json 2.1 scalars per container, mixed.json 2.2);
the M1 same-venv A/B over the three tiers is the reading for those rows ("What the M1 measured", below).
The same-code spread is the discount on any CI prediction: identical
builds span 3.4% instructions, 4.4% elapsed and 8.9% in the ratio across
dispatches (`make pgo` retrains per run), comparable to the deficit closed.

Per element (`benchmarks/parse_elements_probe.py`, ns):

| class        | main  | cursor r1 | cursor r2 | cursor r3 | **+cap r1** | **+cap r2** | orjson |
| ------------ | ----- | --------- | --------- | --------- | ----------- | ----------- | ------ |
| nulls        | 7.89  | 4.34      | 4.50      | 4.47      | 4.62        | 4.61        | 3.63   |
| bools        | 10.72 | 4.94      | 5.06      | 5.03      | **6.18**    | **6.17**    | 6.62   |
| ints 0–9     | 10.10 | 7.48      | 7.68      | 7.63      | 7.58        | 7.56        | 6.77   |
| ints 100–999 | 24.56 | 21.38     | 21.13     | 21.15     | 21.12       | 21.16       | 18.98  |
| ints 4-digit | 28.59 | 25.24     | 25.24     | 25.42     | 25.37       | 25.38       | 18.93  |
| ints 7-digit | 28.90 | 25.18     | 25.32     | 25.48     | 25.54       | 25.40       | 20.03  |
| ints 9–10dig | 37.05 | 30.92     | 31.20     | 30.95     | 31.07       | 31.18       | 22.99  |
| floats 6dp   | 41.13 | 36.37     | 36.48     | 36.40     | 36.40       | 36.48       | 25.15  |
| floats 17dig | 45.71 | 40.92     | 41.02     | 41.06     | 41.32       | 41.25       | 33.34  |

Outside the `exp/cursor` range on both capped draws: **bools**, +22–25%
(6.18 and 6.17 against 4.94–5.06), **nulls**, +2–6%, and **floats 17dig**,
+0.5–0.6% (41.32 and 41.25 against 40.92–41.06); **ints 7-digit** is outside
on the first capped draw only (25.54 against 25.18–25.48) and **ints
4-digit** on neither once r3 is counted (25.37/25.38 against 25.24–25.42).
The probe's own same-code spread across the three cursor draws is 0.3–3.7%
by class, which is the discount on every sub-1% figure here; bools' +22–25%
is far outside it, which is why that one is layout and not noise. They are not a per-element cost of the cap: the probe parses one
list of 2000 elements, so the cap's three instructions run **once** for the
whole measurement. It is code layout — one branch and two parser-struct
words more in `parse_array` shift what PGO+LTO inlines around the cheapest
classes. Both classes remain far ahead of main (bools −42%, nulls −41%) and
bools stays ahead of orjson at 0.93x. `decompose_loads_mixed` shows the
same shape and a little more: `list bools` 0.0209 against the cursor runs'
0.0195/0.0199 and main's 0.0277/0.0285, and the rows list meta dicts, shape
label/items and the interleaved aggregate each worse than every uncapped
draw by more than the probe's same-code repeatability (0.2–1.8%) on both
capped draws — about +0.5% on the interleaved `loads mixed` aggregate
(0.881x/0.883x against 0.874–0.879x), isolated 0.887x against
0.881x/0.893x. No disassembly of the capped build was taken; the frame and
instruction figures above describe `exp/cursor`.

`perf record` self shares are unchanged in kind: the two `parse_value`
instantiations hold 51.4% (32.5% plain, 18.9% cursor) and the builder's
`push` is still absent from the list. `dumps mixed`, untouched, reads 1.023x
isolated and 1.106x then 1.081x interleaved on the two capped draws against
1.05–1.06x before — the leg's other deficit, where it was, inside the
interleave phase's drift.

## What the M1 measured

Same-venv tier A/B on the M1 Max, the `.so` swapped in one venv, two
alternating rounds of `make bench-small/medium/large` per build, the wave-24
build against the same plus the capped cursor, machine at load 1.4–3.9 (the
user's experiment running), so deltas and ranks rather than headline times:
`loads wide_arrays` −1.6/−3.3/−3.0% raw on small/medium/large, `load wide_arrays` −2.3/−2.2/−2.4%, `loads flat` −3.5/−2.4/−0.1%, `loads users`
−0.5/−2.9/−0.9%, `loads mixed` 0.0/−0.8/−0.9%, `loads nested`
−0.8/−1.1/+2.7%, `load mixed` +4.0/−0.4/−0.7%. The two medians past the +2%
gate — large `loads nested` and small `load mixed` — were re-measured
in-process over 40 repeats and three alternating rounds: −0.3% and −1.8%,
with controls at −2.9% (large `loads wide_arrays`), −0.9% (medium `loads users`) and −0.4% (small `load nested`). Then PGO against PGO: HEAD's own
PGO+LTO build, made in a worktree of 3758c97 with the same interpreter and
the same `make pgo`, against the final tree's, three alternating rounds of
40 repeats at load 6–9: `loads users` −2.9%, `flat` −0.3%, `nested` −2.0%,
`wide_arrays` +0.4%, `mixed` −4.2%, `load users` −2.5%, `users.ndjson`
−1.5%, `dumps mixed` −7.0%, the streaming `search $[*].id` control +0.0%,
`query $..total` −0.1%, and `query $[*].id` −9.9% on a 0.10 ms row whose
HEAD draws alone span 17%. Peak memory per build (`ru_maxrss`, fresh
process, twelve harness-like rounds) is unchanged within ±2 MB. Full
numbers and the reading of the harness's regression gate on that machine:
docs/decisions.md, 2026-09-06.

## What it still needs

A genuinely quiet-machine `PGO_MODE=use STRATA_ENABLE_LTO=1 make bench-small`
with `regression_check` before any baseline refresh (docs/decisions.md,
2026-09-06), an x86 sample for the second instantiation's code-growth risk,
and a `search` row on any leg — the PGO-against-PGO A/B's streaming `search`
at +0.0% is the one reading of the plain-path question above so far.
