import gc

import pytest

psutil = pytest.importorskip("psutil")

import strata


def _rss_mb() -> float:
    return psutil.Process().memory_info().rss / (1024 * 1024)


def test_repeated_loads_memory_stable():
    # Guard against arena or builder retaining Python objects across calls.
    sample = '{"users":[{"id":1,"name":"Alice","tags":[1,2,3],"nested":{"x":1,"y":2}}]}'
    gc.collect()
    start = _rss_mb()

    for _ in range(500):
        obj = strata.loads(sample)
        assert obj["users"][0]["id"] == 1

    gc.collect()
    end = _rss_mb()

    # Allow generous headroom for allocator/GC noise but catch runaway growth.
    assert end - start < 20, f"RSS grew too much: {end - start:.1f} MB"


def test_repeated_ndjson_batches_memory_stable():
    lines = "\n".join(
        [
            '{"a":1,"b":[1,2,3],"c":{"d":4}}',
            '{"a":2,"b":[4,5,6],"c":{"d":5}}',
            '{"a":3,"b":[7,8,9],"c":{"d":6}}',
        ]
    )
    payload = (lines + "\n") * 50  # 150 lines
    gc.collect()
    start = _rss_mb()

    total = 0
    for _ in range(120):
        for obj in strata.iter_ndjson(payload, batch_size=16):
            total += obj["a"]

    gc.collect()
    end = _rss_mb()

    # Should stay bounded; allow generous slack for platform differences.
    assert end - start < 25, f"RSS grew too much: {end - start:.1f} MB"
    assert total > 0  # sanity
