"""Dumps contract: schema hits preserve order and survive deleted entries."""

import json

import strata


def test_fused_records_preserve_deleted_and_reinserted_entries():
    records = []
    for width in (1, 4, 24):
        keys = [f'field_{index}' for index in range(width)]
        for deleted in range(width):
            full = dict.fromkeys(keys, None)
            hole = full.copy()
            del hole[keys[deleted]]
            reinserted = hole.copy()
            reinserted[keys[deleted]] = [deleted, True]
            records.extend([full.copy() for _ in range(8)])
            records.extend([hole, reinserted, full.copy()])
    for _ in range(3):
        assert strata.dumps(records) == json.dumps(records, separators=(',', ':'))


def test_fused_same_first_key_different_tail_preserves_all_keys():
    records = []
    for index in range(32):
        records.extend([{'first': None, 'left': index}] * 4)
        records.append({'first': None, 'right': index})
        records.append({'first': None})
    for _ in range(3):
        assert strata.dumps(records) == json.dumps(records, separators=(',', ':'))
