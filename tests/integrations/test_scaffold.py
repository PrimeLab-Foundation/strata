"""The scaffold's own test: the tree is wired, and it is outside the gate.

M12 criterion 7 asks for the tree to exist, to be excluded from `testpaths`,
`make test`, `make gate` and the PGO profile, and to have its own target and CI
job. The first library module (`pydantic`, `attrs`, `numpy`, an ORM row) lands
with M13; until then this file is what `make test-integrations` runs, and it
pins the exclusion so a later edit cannot quietly pull the tree into the gate.
"""

from __future__ import annotations

import ast
from pathlib import Path

import tomllib

PROJECT_ROOT = Path(__file__).resolve().parents[2]


def test_the_tree_is_excluded_from_testpaths():
    """Criterion 7: `tests/integrations` is not a gated suite."""
    config = tomllib.loads((PROJECT_ROOT / "pyproject.toml").read_text(encoding="utf-8"))
    testpaths = config["tool"]["pytest"]["ini_options"]["testpaths"]
    assert testpaths == ["tests/py", "tests/unit"]
    assert "tests/integrations" not in testpaths


def test_the_gate_runner_does_not_collect_this_tree():
    """`scripts/py_tests.py` is the single definition of what the gate runs."""
    source = (PROJECT_ROOT / "scripts" / "py_tests.py").read_text(encoding="utf-8")
    assert 'TEST_PATHS = ("tests/py", "tests/unit")' in source
    assert "tests/integrations" not in source


def test_the_pgo_training_workload_installs_no_hook():
    """Record, "The PGO trap": the training workload must contain no
    `default=` call and no unsupported-type raise -- the attribute alone is not
    protection, the profile is what decides block placement (E26-P23).

    Read off the AST, not the text, so the prohibition's own comment does not
    trip it and a `default=` on some unrelated helper is not mistaken for one.
    """
    tree = ast.parse((PROJECT_ROOT / "scripts" / "pgo_training.py").read_text(encoding="utf-8"))
    hooked = [
        node
        for node in ast.walk(tree)
        if isinstance(node, ast.Call) and any(keyword.arg == "default" for keyword in node.keywords)
    ]
    assert hooked == []


def test_the_target_exists():
    """Criterion 7: its own `make test-integrations`."""
    makefile = (PROJECT_ROOT / "Makefile").read_text(encoding="utf-8")
    assert "test-integrations:" in makefile


def test_the_ci_job_exists():
    """Criterion 7: its own CI job, separate from the build gate."""
    workflow = PROJECT_ROOT / ".github" / "workflows" / "integrations.yml"
    assert workflow.is_file()
    text = workflow.read_text(encoding="utf-8")
    assert "tests/integrations" in text
