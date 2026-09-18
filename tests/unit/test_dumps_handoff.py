"""Dumps contract: the record writer's hand-off, forced at every key index.

A record whose values are all plain scalars is emitted in one pass; the moment a
value is a container the record writer has to hand that value off to the general
walk and then resume its own key loop where it left off. Where that hand-off
happens inside the record — first key, last key, or anywhere between — must not
change a single output byte.

This is proof obligation 3 of docs/architecture/record_emit_handoff.md: for every
width 1..24 (the schema cache's range) and every position in turn, under both
return types. The oracle is stdlib `json` compact output, as
docs/context/workflow.md requires, and every document is dumped more than once:
the schema cache remembers a shape on its first sighting and prepares its bytes
on the second, so the repeats reach the prepared path with the hand-off already
in the middle of it.
"""

import json
import threading

import strata

SCALARS = ('text', 7, 1.5, True, None, -3, '', False, 0.25, 'tail')

CONTAINERS = (list, lambda: {'z': 1})


def in_fresh_cache(body):
    """Run `body` on a new thread and re-raise whatever it raised.

    The schema cache is thread-local, so a new thread starts empty: a shape's
    first and second sighting are then in a known order rather than inherited
    from another test in the same process.
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


def scalar(index):
    return SCALARS[index % len(SCALARS)]


def record(width, position, container):
    """`width` exact-`str` keys, a container at `position`, scalars elsewhere."""
    return {
        f'k{index}': (container() if index == position else scalar(index)) for index in range(width)
    }


def both_return_types(payload):
    assert strata.dumps(payload) == compact(payload)
    assert strata.dumps(payload, return_type='bytes') == compact(payload).encode()


def test_the_hand_off_at_every_index():
    """Widths 1..24 x every position x both container kinds, both return types.

    The test the design exists to need. Each document is dumped three times, so
    the hand-off is exercised on the plain key walk (first sighting) and on the
    prepared-slot loop (later ones).
    """

    def body():
        for width in range(1, 25):
            for position in range(width):
                for container in CONTAINERS:
                    one = record(width, position, container)
                    documents = (one, [one, one, one], {'held': one})
                    for _ in range(3):
                        for document in documents:
                            both_return_types(document)

    in_fresh_cache(body)


def test_the_hand_off_at_the_first_key():
    """Position 0, including width 1, where the record is nothing but a hand-off."""

    def body():
        for container in CONTAINERS:
            for width in range(1, 25):
                one = record(width, 0, container)
                for _ in range(3):
                    both_return_types(one)
                    both_return_types([one, one])

    in_fresh_cache(body)


def test_the_hand_off_at_the_last_key():
    """Position width - 1: the record resumes only to close itself."""

    def body():
        for container in CONTAINERS:
            for width in range(1, 25):
                one = record(width, width - 1, container)
                for _ in range(3):
                    both_return_types(one)
                    both_return_types([one, one])

    in_fresh_cache(body)


def test_a_record_whose_every_value_is_a_container():
    """No plain run at all: the key loop hands off on each key in turn."""

    def body():
        for container in CONTAINERS:
            for width in range(1, 25):
                one = {f'k{index}': container() for index in range(width)}
                mixed = {f'k{index}': ([] if index % 2 else {'z': 1}) for index in range(width)}
                for _ in range(3):
                    both_return_types(one)
                    both_return_types(mixed)
                    both_return_types([one, mixed, one])

    in_fresh_cache(body)


def test_a_record_with_no_container_value():
    """The control: the same widths with no hand-off to make."""

    def body():
        for width in range(1, 25):
            one = {f'k{index}': scalar(index) for index in range(width)}
            for _ in range(3):
                both_return_types(one)
                both_return_types([one, one, one])

    in_fresh_cache(body)


def test_a_hand_off_record_repeated_at_one_depth():
    """The same shape twice in one document, and two shapes alternating at a depth.

    The second occurrence reads the prepared row the first one populated, and the
    alternation makes the cache re-decide at a depth it already holds.
    """

    def body():
        for width in range(1, 25):
            for position in range(width):
                first = record(width, position, list)
                second = record(width, position, CONTAINERS[1])
                documents = (
                    [first, first],
                    [first, second, first, second],
                    {'a': first, 'b': second, 'c': first},
                )
                for _ in range(3):
                    for document in documents:
                        both_return_types(document)

    in_fresh_cache(body)


def test_nesting_two_hand_off_records():
    """A hand-off record whose handed-off value is itself a hand-off record.

    The outer record must resume its own key loop after a nested record ran the
    same loop underneath it, at every combination of the two positions.
    """

    def body():
        for width in (1, 2, 5, 23, 24):
            for outer in range(width):
                for inner in (0, width - 1):
                    leaf = record(width, inner, CONTAINERS[1])
                    one = record(width, outer, lambda: dict(leaf))
                    deeper = record(width, outer, lambda: {'mid': dict(leaf)})
                    for _ in range(3):
                        both_return_types(one)
                        both_return_types(deeper)
                        both_return_types([one, deeper, one])

    in_fresh_cache(body)
