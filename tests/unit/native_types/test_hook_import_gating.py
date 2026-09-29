"""Contract tests for `strata._dumps_hook` import gating.

M15b review (P... items 4a/4b): a hook image that fails to import must not be
swallowed -- the `ImportError` surfaces from every entry point that needs the
hook, on that call and every later one, while the hook-free calls keep
working (`functools.cache` on `strata.serialize._hook_module` does not cache a
raised exception, so a later call retries the import). Conversely, an entry
point whose flag routes it away from the hook (`parse_types=False`) must never
import it at all -- the same rule `tests/unit/native_types/test_native_flag.py`
pins for `dumps`/`dump` with `native=False`.
"""

import subprocess
import sys
import textwrap

import pytest

import strata


def _fresh(code):
    package_root = strata.__file__.rsplit("/python/", 1)[0] + "/python"
    result = subprocess.run(
        [sys.executable, "-c", f"import sys\nsys.path.insert(0, {package_root!r})\n" + code],
        capture_output=True,
        text=True,
        timeout=60,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    return result.stdout.split("\n")[:-1]


_BLOCK_HOOK_IMPORT = """
import sys

class _BlockHook:
    def find_spec(self, name, path, target=None):
        if name == "strata._dumps_hook":
            raise ImportError("blocked for test")
        return None

sys.meta_path.insert(0, _BlockHook())
"""


@pytest.mark.parametrize(
    ("label", "call"),
    [
        ("dumps", "strata.dumps(1, native=True)"),
        ("dump", "strata.dump(1, __import__('tempfile').mktemp(), native=True)"),
        ("loads", "strata.loads('1', parse_types=True)"),
    ],
)
def test_a_hook_that_fails_to_import_raises_on_every_call(label, call):
    out = _fresh(
        _BLOCK_HOOK_IMPORT
        + textwrap.dedent(
            f"""
            import strata

            for _ in range(2):
                try:
                    {call}
                except ImportError:
                    print("import-error")
                else:
                    print("NO-ERROR")

            print(strata.dumps({{"a": 1}}))
            print(strata.loads('{{"a": 1}}'))
            """
        )
    )
    assert out == ["import-error", "import-error", '{"a":1}', "{'a': 1}"]


def test_hook_free_calls_are_unaffected_by_a_broken_hook_image():
    out = _fresh(
        _BLOCK_HOOK_IMPORT
        + textwrap.dedent(
            """
            import strata

            try:
                strata.dumps(1, native=True)
            except ImportError:
                pass
            print("dumps-ok" if strata.dumps({"a": 1}) == '{"a":1}' else "dumps-BAD")
            print("loads-ok" if strata.loads('{"a":1}') == {"a": 1} else "loads-BAD")
            """
        )
    )
    assert out == ["dumps-ok", "loads-ok"]


# ---------------------------------------------------------------------------
# `parse_types=False` never imports the hook (load/search/query).
# ---------------------------------------------------------------------------


def test_load_with_parse_types_false_never_imports_the_hook(tmp_path):
    path = tmp_path / "a.json"
    path.write_text('{"a": 1}')
    out = _fresh(
        textwrap.dedent(
            f"""
            import sys
            import strata
            strata.load({str(path)!r})
            strata.load({str(path)!r}, parse_types=False)
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]


def test_search_with_parse_types_false_never_imports_the_hook(tmp_path):
    path = tmp_path / "a.json"
    path.write_text('{"a": 1}')
    out = _fresh(
        textwrap.dedent(
            f"""
            import sys
            import strata
            strata.search({str(path)!r}, "$.a")
            strata.search({str(path)!r}, "$.a", parse_types=False)
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]


def test_query_with_parse_types_false_never_imports_the_hook():
    out = _fresh(
        textwrap.dedent(
            """
            import sys
            import strata
            strata.query({"a": 1}, "$.a")
            strata.query({"a": 1}, "$.a", parse_types=False)
            print("imported" if "strata._dumps_hook" in sys.modules else "not-imported")
            """
        )
    )
    assert out == ["not-imported"]
