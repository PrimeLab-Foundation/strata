"""Agreement/exclusion logic for the native-v1 benchmark scope (benchmarks/native_v1.py).

`strata.dumps(obj, native=...)` is not implemented in this checkout yet (it
lands separately, docs/architecture/native_types.md "Flag shape (M15b)"), so
these tests fake the `strata` rival rather than importing the real package,
per the module's own agreement rule: a rival is timed only if its output
parses back to the same JSON as strata's, decimals compared by value.
"""

import decimal

from benchmarks.harness import Report
from benchmarks.native_v1 import (
    _as_comparable,
    _dump_comparable,
    _flag_calls,
    _native_dump_calls,
    _native_dumps_calls,
    agreeing_calls,
)


def test_as_comparable_accepts_bytes_and_str_and_compares_decimals_by_value():
    from_bytes = _as_comparable(b'{"amount": 1.50}')
    from_str = _as_comparable('{"amount": 1.5}')
    assert from_bytes == from_str
    assert from_bytes["amount"] == decimal.Decimal("1.50")


def test_agreeing_calls_keeps_agreement_drops_disagreement_and_keeps_errors():
    report = Report("t")
    calls = {
        "strata": lambda: b'{"a": 1.50}',
        "agrees_as_float": lambda: '{"a": 1.5}',
        "disagrees": lambda: '{"a": 2}',
        "raises": lambda: (_ for _ in ()).throw(ValueError("boom")),
    }
    kept = agreeing_calls(calls, report, "label")
    assert set(kept) == {"strata", "agrees_as_float", "raises"}
    assert report.excluded == {"disagrees (label)": "output disagrees as parsed JSON"}


def test_agreeing_calls_is_a_no_op_without_a_strata_call():
    report = Report("t")
    calls = {"orjson": lambda: b"{}"}
    assert agreeing_calls(calls, report, "label") == calls
    assert report.excluded == {}


def test_agreeing_calls_keeps_everything_if_strata_itself_fails():
    report = Report("t")
    calls = {
        "strata": lambda: (_ for _ in ()).throw(TypeError("no native kwarg yet")),
        "orjson": lambda: b"{}",
    }
    assert agreeing_calls(calls, report, "label") == calls


class _FakeStrata:
    """A fake standing in for `strata` until `native=` lands."""

    def dumps(self, obj, *, return_type="str", native=False):
        assert return_type == "bytes"
        payload = obj if isinstance(obj, list) else [obj]
        return f'"native={native}:{len(payload)}"'.encode()

    def dump(self, obj, path, *, native=False):
        with open(path, "wb") as handle:
            handle.write(self.dumps(obj, return_type="bytes", native=native))


def test_native_dumps_calls_route_through_the_native_true_arm(tmp_path):
    rivals = {"strata": _FakeStrata()}
    calls = _native_dumps_calls(rivals, [1, 2, 3])
    assert set(calls) == {"strata"}
    assert calls["strata"]() == b'"native=True:3"'


def test_native_dump_calls_write_a_file_through_the_native_true_arm(tmp_path):
    rivals = {"strata": _FakeStrata()}
    calls = _native_dump_calls(rivals, [1, 2], tmp_path)
    calls["strata"]()
    assert (tmp_path / "strata.json").read_bytes() == b'"native=True:2"'


def test_flag_calls_measure_both_native_arms_under_distinct_library_names():
    rivals = {"strata": _FakeStrata()}
    calls = _flag_calls(rivals, {"k": "v"})
    assert set(calls) == {"strata (native=False)", "strata (native=True)"}
    assert calls["strata (native=False)"]() == b'"native=False:1"'
    assert calls["strata (native=True)"]() == b'"native=True:1"'


def test_native_dumps_calls_excludes_ujson_and_records_the_reason():
    # ujson is out of scope for the native dataset (no native type support),
    # even when it is installed -- the exclusion is unconditional.
    calls = _native_dumps_calls({"strata": _FakeStrata(), "ujson": object()}, [1])
    assert "ujson" not in calls


def _write(path, text: str) -> None:
    path.write_text(text, encoding="utf-8")


def test_agreeing_calls_dump_rows_are_not_vacuous_on_a_none_return(tmp_path):
    # dump()-style calls write a file and return None; comparing None against
    # None would make every rival "agree" regardless of what it wrote.
    report = Report("t")
    calls = {
        "strata": lambda: _write(tmp_path / "strata.json", '{"a": 1.50}'),
        "agrees": lambda: _write(tmp_path / "agrees.json", '{"a": 1.5}'),
        "disagrees": lambda: _write(tmp_path / "disagrees.json", '{"a": 2}'),
    }
    kept = agreeing_calls(calls, report, "label", comparable=_dump_comparable(tmp_path))
    assert set(kept) == {"strata", "agrees"}
    assert report.excluded == {"disagrees (label)": "output disagrees as parsed JSON"}


def test_agreeing_calls_dump_keeps_a_rival_whose_own_dump_agrees(tmp_path):
    # A rival's dump agreement is judged on its own written file, independent
    # of whatever happened on the in-memory `dumps` comparison: excluded on
    # `dumps` does not, by itself, exclude it from `dump`.
    report = Report("t")
    calls = {
        "strata": lambda: _write(tmp_path / "strata.json", '{"a": 1}'),
        "disagreed_on_dumps": lambda: _write(tmp_path / "disagreed_on_dumps.json", '{"a": 1}'),
    }
    kept = agreeing_calls(calls, report, "label (dump)", comparable=_dump_comparable(tmp_path))
    assert set(kept) == {"strata", "disagreed_on_dumps"}
    assert report.excluded == {}


def test_native_dump_calls_agreement_reads_the_written_files(tmp_path):
    # End-to-end through the module's own dump-call builder, not a hand-rolled
    # calls dict: strata's native writer (via _native_dump_calls, naming its
    # file out_dir/strata.json) beside a disagreeing rival written the same way.
    calls = _native_dump_calls({"strata": _FakeStrata()}, [1], tmp_path)
    calls["orjson"] = lambda: _write(tmp_path / "orjson.json", '{"a": 999}')
    report = Report("t")
    kept = agreeing_calls(
        calls, report, "native.small (dump)", comparable=_dump_comparable(tmp_path)
    )
    assert set(kept) == {"strata"}
    assert report.excluded == {"orjson (native.small (dump))": "output disagrees as parsed JSON"}
