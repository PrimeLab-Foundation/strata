"""Supplementary scopes preserve operation semantics and the canonical denominator."""

import json

import pytest

import strata
from benchmarks.bench_main import _load_competitors, _load_query_libraries
from benchmarks.harness import workload_rows
from benchmarks.supplementary import (
    ROWS,
    equivalent,
    file_bytes,
    loop_dump,
    loop_load,
    loop_search,
    ndjson_calls,
)


def test_scope_does_not_change_canonical_rows():
    assert len(workload_rows()) == 27
    assert len(ROWS) == 6


@pytest.mark.parametrize("expression", ["$", "$[0]", "$[*].id", "$..total"])
def test_ndjson_search_uses_complete_list(tmp_path, expression):
    records = [{"id": 1, "total": 3}, {"id": 1, "total": 3}]
    path = tmp_path / "users.ndjson"
    path.write_text("\n".join(json.dumps(r) for r in records) + "\n")
    libraries, excluded = _load_competitors()
    engines = _load_query_libraries(excluded)
    query = {"strata": expression, "jmespath": None, "jsonpath_ng": expression}
    calls = ndjson_calls(libraries, engines, path, query)
    assert calls["strata"]() == strata.query(records, expression)
    equivalent(calls)


def test_equivalence_preserves_order_and_duplicates():
    for wrong in ([2, 1, 1], [1, 2]):
        with pytest.raises(ValueError, match="differ"):
            equivalent({"strata": lambda: [1, 1, 2], "rival": lambda: wrong})


def test_folder_controls_include_discovery_and_per_file_search(tmp_path):
    (tmp_path / "sub").mkdir()
    (tmp_path / ".hidden").mkdir()
    (tmp_path / "a.JSON").write_text('[{"id": 1}, {"id": 2}]')
    (tmp_path / "sub" / "b.ndjson").write_text('{"id": 3}\n')
    (tmp_path / "z.json").write_text('{"id": 4}')
    (tmp_path / ".hidden" / "ignore.json").write_text("invalid")
    (tmp_path / "ignore.txt").write_text("invalid")
    assert loop_load(strata, tmp_path) == strata.load(tmp_path)
    for expression in ("$", "$[*].id"):
        compiled = strata.compile(expression)
        assert loop_search(strata, tmp_path, compiled) == strata.search(tmp_path, compiled)


def test_folder_dump_bytes_and_shorter_overwrite(tmp_path):
    records = [{"id": i, "bench_group": f"group-{i % 2}"} for i in range(8)]
    native, loop = tmp_path / "native", tmp_path / "loop"
    for values in (records, records[:2]):
        strata.dump(values, native, split_by="bench_group")
        loop_dump(strata, values, loop)
        assert file_bytes(native) == file_bytes(loop)


def test_file_phase_control_handles_short_writes(tmp_path, monkeypatch):
    import os

    from benchmarks.file_costs import phases

    original = os.write
    monkeypatch.setattr(os, "write", lambda fd, data: original(fd, data[:3]))
    target = tmp_path / "control.json"
    value = [{"text": "long enough for several writes"}]
    measured = phases(value, target)
    assert target.read_bytes() == strata.dumps(value, return_type="bytes") + b"\n"
    assert set(measured) == {"serialize", "open_truncate", "write_and_newline", "metadata", "close"}


def test_file_phase_control_closes_after_write_failure(tmp_path, monkeypatch):
    import os

    from benchmarks.file_costs import phases

    closed = []
    close = os.close

    def record_close(fd):
        closed.append(fd)
        close(fd)

    monkeypatch.setattr(os, "close", record_close)
    monkeypatch.setattr(os, "write", lambda fd, data: 0)
    with pytest.raises(OSError, match="no progress"):
        phases([1], tmp_path / "control.json")
    assert len(closed) == 1


@pytest.mark.parametrize("missing_library", ["strata", "strata-loop"])
def test_supplementary_cli_rejects_missing_required_rows(tmp_path, monkeypatch, missing_library):
    from benchmarks import supplementary
    from benchmarks.harness import Report, summarize

    report = Report("supplementary-v1")
    for section, dataset in ROWS:
        for library in ("strata", "strata-loop") if section.startswith("folder") else ("strata",):
            if section == "folder load" and library == missing_library:
                continue
            report.measurements.append(summarize(section, dataset, library, [1, 2, 3], 10))
    monkeypatch.setattr(supplementary, "run", lambda *args, **kwargs: report)
    monkeypatch.setattr(supplementary, "write_report", lambda *args: None)
    assert supplementary.main(["--data", str(tmp_path), "--output", str(tmp_path / "out.md")]) == 1


def test_ndjson_rivals_follow_the_canonical_disagreement_rule(tmp_path):
    """A rival composition that computes a different result set is excluded, not timed."""
    from benchmarks.bench_main import _drop_disagreeing
    from benchmarks.harness import Report

    records = [
        {"id": 1, "orders": [{"total": 2}, {"total": 3}]},
        {"id": 4, "orders": [{"total": 5}]},
    ]
    path = tmp_path / "users.ndjson"
    path.write_text("\n".join(json.dumps(r) for r in records) + "\n")
    libraries, excluded = _load_competitors()
    engines = _load_query_libraries(excluded)
    if "orjson" not in libraries or "jmespath" not in engines:
        pytest.skip("the composed rivals need orjson and jmespath")
    flattening = {
        "strata": "$[*].orders[*].total",
        "jmespath": "[].orders[].total",
        "jsonpath_ng": "$[*].orders[*].total",
    }
    report = Report("supplementary-v1")
    kept = _drop_disagreeing(ndjson_calls(libraries, engines, path, flattening), report, "totals")
    assert "orjson+jmespath" in kept
    assert kept["orjson+jmespath"]() == [2, 3, 5] == kept["strata"]()

    projecting = dict(flattening, jmespath="[*].orders[*].total")
    kept = _drop_disagreeing(ndjson_calls(libraries, engines, path, projecting), report, "totals")
    assert "orjson+jmespath" not in kept
    assert "orjson+jmespath (totals)" in report.excluded
