"""Dumps contract: a record's opening brace, on every path that can emit one.

The prepared-slot emit of both dict writers writes `{` as its *first key's*
separator byte rather than on its own (E26-P24), so "the object opens" is now a
property of the key loop. These pin it on each path that reaches that loop and
on the paths that deliberately do not: a shape seen once (keys written the plain
way), a span too wide for an inline slot (the blob fallback), and a zero-width
dict, which has no first key to carry the brace and must reach `write_mapping`.

Every assertion is against stdlib `json` compact output, the oracle named in
docs/context/workflow.md, and every one repeats the document: the schema cache
remembers a shape on its first sighting and prepares its bytes on the second, so
the first call exercises the plain key walk and the later ones the slot loop.
"""

import json
import threading

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

    Dicts and lists share that stack, so the limit has to answer the same for a
    document nested either way, and for one that alternates.
    """

    def body():
        limit = 96
        for build in (
            lambda node: {'child': node},
            lambda node: [node],
            lambda node: {'child': [node]},
        ):
            node = 1
            for _ in range(limit):
                node = build(node)
            assert strata.dumps(node) == compact(node)

    in_fresh_cache(body)
