"""Dumps contract: a record's opening brace, on every path that can emit one.

The prepared-slot emit of both dict writers writes `{` as its *first key's*
separator byte rather than on its own (E26-P24), so "the object opens" is now a
property of the key loop. These pin it on each path that reaches that loop and
on the paths that deliberately do not: a shape seen once (keys written the plain
way), a span too wide for an inline slot (the blob fallback), and a zero-width
dict, which has no first key to carry the brace and must reach `write_mapping`.

Every byte assertion is against stdlib `json` compact output, the oracle named in
docs/context/workflow.md, and every one repeats the document: the schema cache
remembers a shape on its first sighting and prepares its bytes on the second, so
the first call exercises the plain key walk and the later ones the slot loop.
Because an oracle comparison passes whether or not the prepared path ran, the
file also carries one *liveness* assertion, on the documented mutation rule
(`test_a_narrow_record_is_emitted_as_the_row_read_on_entry`): that one fails the
moment a narrow record stops being emitted from its staged row.
"""

import json
import sys
import threading

import pytest

import strata


def in_fresh_cache(body):
    """Run `body` on a new thread and re-raise whatever it raised.

    `SchemaCacheLease`'s slots are `thread_local`, so a new thread starts with an
    empty schema cache: a shape's first, second and third sighting are then in
    a known order rather than inherited from another test.
    """
    box = {}

    def run():
        try:
            box['value'] = body()
        except BaseException as error:  # noqa: BLE001 - re-raised below
            box['error'] = error

    thread = threading.Thread(target=run)
    thread.start()
    thread.join()
    if 'error' in box:
        raise box['error']
    return box['value']


def compact(payload):
    return json.dumps(payload, separators=(',', ':'))


def test_every_width_opens_on_its_first_key():
    """Widths 1..24 through the slot loop, on the sighting that prepares it."""

    def body():
        for width in range(1, 25):
            records = [
                {f'k{index}': index for index in range(width)},
                {f'k{index}': index for index in range(width)},
                {f'k{index}': index for index in range(width)},
            ]
            for _ in range(3):
                assert strata.dumps(records) == compact(records)
                assert strata.dumps(records, return_type='bytes') == compact(records).encode()

    in_fresh_cache(body)


def test_a_record_reached_as_a_value_opens_too():
    """The fused writer's other entry: a dict as a dict's value, and as a root."""

    def body():
        for _ in range(4):
            record = {'outer': {'inner': {'leaf': 1, 'other': 2}}, 'tail': 3}
            assert strata.dumps(record) == compact(record)
            assert strata.dumps([record, record]) == compact([record, record])

    in_fresh_cache(body)


def test_an_empty_dict_among_prepared_shapes_is_still_a_pair_of_braces():
    """A zero-width dict has no first key to carry the brace.

    It must therefore leave both slot loops -- the fused writer refuses
    `size == 0`, `write_mapping` writes `{}` itself. Interleaved with a prepared
    shape at the same depth so the schema cache is warm when it arrives.
    """

    def body():
        records = [{'a': 1}, {'a': 1}, {'a': 1}, {}, {'a': 1}, {}, {}]
        for _ in range(3):
            assert strata.dumps(records) == compact(records)
        nested = [{'a': {}}, {'a': {}}, {'a': {}}]
        for _ in range(3):
            assert strata.dumps(nested) == compact(nested)
        assert strata.dumps({}) == '{}'
        assert strata.dumps([{}, {}]) == '[{},{}]'

    in_fresh_cache(body)


def test_a_key_too_wide_for_an_inline_slot_still_opens():
    """`"key":` past 16 bytes keeps the whole schema on the blob fallback.

    That branch writes its own brace, so the fold must not have taken it away.
    The widths bracket the boundary: 13 characters fit a slot exactly (quotes
    and colon make 16), 14 do not.
    """

    def body():
        for length in (12, 13, 14, 15, 40):
            key = 'w' * length
            records = [{key: 1, 'tail': 2}] * 4
            for _ in range(3):
                assert strata.dumps(records) == compact(records)

    in_fresh_cache(body)


def test_a_shape_seen_once_opens_on_the_plain_key_walk():
    """Never-repeated shapes never prepare, so they keep the plain walk."""

    def body():
        records = [{f'unique_{index}': index} for index in range(64)]
        for _ in range(3):
            assert strata.dumps(records) == compact(records)

    in_fresh_cache(body)


def test_nesting_that_grows_the_schema_table_mid_record():
    """A record whose value is a deeper record grows the per-depth table.

    That growth can move the prepared row the *outer* record is emitting from,
    which is why the emit loops re-index it per key. Each depth is reached
    first inside a record already being emitted.
    """

    def body():
        for depth in range(1, 40):
            leaf = {'leaf': depth}
            for level in range(depth):
                leaf = {'level': level, 'child': leaf, 'after': level}
            records = [leaf, leaf, leaf]
            for _ in range(2):
                assert strata.dumps(records) == compact(records)

    in_fresh_cache(body)


def test_a_document_longer_than_the_stage_opens_every_record():
    """Enough records that the 8 KB stage flushes many times mid-document.

    The brace now shares its first key's reservation, so a flush that used to
    land between the two lands before both.
    """

    def body():
        records = [{'index': index, 'name': f'name-{index}', 'ok': True} for index in range(4000)]
        assert strata.dumps(records) == compact(records)
        assert strata.dumps(records, return_type='bytes') == compact(records).encode()

    in_fresh_cache(body)


def test_the_open_container_count_still_bounds_the_depth():
    """The depth check reads a counter now, not the open-container vector.

    So the bound itself is re-pinned here, at the real limit rather than near it:
    N open containers serialize and one more raises, where N is
    `sys.getrecursionlimit()` at the moment of the call (docs/context/api.md).
    Dicts and lists share that stack, so the answer has to be the same for a
    document nested either way and for one that alternates, and the container
    that goes one past the limit is a list in all three cases. The limit is
    lowered and restored in a try/finally, per the styleguide's deep-nesting
    rule; the exact boundary for each shape on its own is pinned in
    tests/unit/test_dumps_contract.py.
    """

    def stack_depth():
        depth = 0
        frame = sys._getframe()
        while frame is not None:
            depth += 1
            frame = frame.f_back
        return depth

    def body():
        builds = (
            lambda node: {'child': node},
            lambda node: [node],
            lambda node: {'child': [node]},
        )
        saved = sys.getrecursionlimit()
        # Room for the interpreter's own frames; the serializer's containers are
        # C++ frames, not Python ones.
        limit = max(300, stack_depth() + 120)
        try:
            sys.setrecursionlimit(limit)
            for build in builds:
                # `{'child': [node]}` opens two containers per level.
                per_level = 2 if build(1) == {'child': [1]} else 1
                node = 1
                for _ in range(limit // per_level):
                    node = build(node)
                at_limit = strata.dumps(node)
                # It really did emit that many containers (the leaf is a bare 1).
                assert at_limit.count('{') + at_limit.count('[') == limit // per_level * per_level
                for return_type in ('str', 'bytes'):
                    with pytest.raises(ValueError, match='^Maximum serialization depth exceeded$'):
                        strata.dumps([node], return_type=return_type)
        finally:
            sys.setrecursionlimit(saved)

    in_fresh_cache(body)


def test_a_narrow_record_is_emitted_as_the_row_read_on_entry():
    """The file's liveness guard: it fails if the prepared-slot path stops running.

    Every other assertion here compares against stdlib `json`, so all of them
    would still pass with the fold, the schema cache and the fused writer
    removed. This one cannot: docs/context/api.md's mutation rule says a dict of
    **at most 24** exact-`str` keys is emitted as the row the serializer read on
    entry, while a wider one is followed live -- and that difference exists only
    because the narrow record is emitted from a staged row through the prepared
    slot loop. The user code is the one api.md names for this: `__str__` of an
    `int` subclass beyond int64.
    """

    class Grower(int):
        """Adds a key to the record being emitted, from inside its own `__str__`."""

        target = None

        def __str__(self):
            if Grower.target is not None:
                Grower.target['added'] = 'late'
            return int.__str__(self)

    def emit(width, return_type):
        record = {f'k{index}': index for index in range(width - 1)}
        record['big'] = Grower(2**70)
        Grower.target = record
        try:
            out = strata.dumps(record, return_type=return_type)
        finally:
            Grower.target = None
        return out.decode() if return_type == 'bytes' else out

    def body():
        for return_type in ('str', 'bytes'):
            for width in (1, 2, 24):
                text = emit(width, return_type)
                # The row read on entry: the key user code added is not in it.
                assert '"added"' not in text
                assert text.endswith('"big":1180591620717411303424}')
            # Past kMaxSchemaKeys the same dict is followed live, and does show it.
            text = emit(25, return_type)
            assert text.endswith('"big":1180591620717411303424,"added":"late"}')

    in_fresh_cache(body)
