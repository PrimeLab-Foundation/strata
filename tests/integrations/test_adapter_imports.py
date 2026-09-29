"""The framework adapters are opt-in modules (api.md, Framework adapters).

`import strata` — and `import strata.integrations` — imports no adapter and no
framework, and the package's `__all__` does not grow. Checked in a fresh
interpreter, since this process has imported every adapter by now.
"""

import importlib
import subprocess
import sys

import pytest

import strata

FRAMEWORKS = ("flask", "django", "aiohttp", "falcon", "fastapi", "pydantic", "structlog")
DEPENDENCIES = ("starlette", "pydantic_core")

_PROBE = """
import sys
import strata
import strata.integrations
frameworks = {frameworks!r}
loaded = sorted(
    name for name in sys.modules
    if name.split(".")[0] in frameworks or name.startswith("strata.integrations.")
)
print(",".join(loaded))
"""


def test_import_strata_imports_no_adapter_and_no_framework():
    probe = _PROBE.format(frameworks=FRAMEWORKS + DEPENDENCIES)
    completed = subprocess.run(
        [sys.executable, "-c", probe],
        capture_output=True,
        text=True,
        check=True,
    )
    assert completed.stdout.strip() == ""


def test_all_does_not_grow():
    assert sorted(strata.__all__) == sorted(
        [
            "loads",
            "dumps",
            "dumps_with_default",
            "load",
            "dump",
            "search",
            "query",
            "compile",
            "JsonCursor",
            "CompiledPath",
            "config",
            "__version__",
        ],
    )


@pytest.mark.parametrize("framework", FRAMEWORKS)
def test_each_adapter_is_importable_by_its_own_name(framework):
    module = importlib.import_module(f"strata.integrations.{framework}")
    assert module.__all__
    for name in module.__all__:
        assert callable(getattr(module, name))
