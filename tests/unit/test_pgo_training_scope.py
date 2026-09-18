"""The PGO training workload's prohibitions, pinned inside the gate.

`docs/architecture/dumps_default_hook.md`, "The PGO trap": the training
workload must contain **no `default=` call and no unsupported-type raise**.
E26-P23 is why the `cold` attribute is not protection on its own — when the
training payload exercised a path marked cold, block placement believed the
profile over the attribute and Windows lost `dumps mixed` +4.3% (run
34665612473), which only the paired payload fixed (run 34670240916).

`tests/integrations/test_scaffold.py` asserts the same thing, but that tree is
outside `testpaths`, `make test` and `make gate` by design — so a pin that
lived only there would never run on the gate it is meant to protect. This file
is the gated copy; when one changes, change both.
"""

from __future__ import annotations

import ast
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[2]
TRAINING = PROJECT_ROOT / "scripts" / "pgo_training.py"

# Types the serializer has no writer for. A training payload containing any of
# them would reach `Serializer::write_unsupported` and train the cold tail,
# which is the half of the prohibition that is not about `default=` at all.
UNSUPPORTED_CONSTRUCTORS = frozenset(
    {
        "set",
        "frozenset",
        "bytes",
        "bytearray",
        "complex",
        "object",
        "Decimal",
        "datetime",
        "date",
        "time",
        "UUID",
        "Enum",
        "Fraction",
    },
)


def _tree() -> ast.Module:
    return ast.parse(TRAINING.read_text(encoding="utf-8"))


def test_the_training_workload_installs_no_default_hook():
    """Half one: no `default=` call anywhere in the workload.

    Read off the AST rather than the text, so the prohibition's own comment
    does not trip it.
    """
    hooked = [
        node
        for node in ast.walk(_tree())
        if isinstance(node, ast.Call) and any(keyword.arg == "default" for keyword in node.keywords)
    ]
    assert hooked == []


def test_the_training_workload_serializes_no_unsupported_type():
    """Half two: nothing the workload builds can reach the unsupported tail.

    The payload is read from disk and parsed by `json.loads`/`strata.loads`, so
    it is JSON's own closed type set; this pins that no *literal* of an
    unsupported type and no constructor of one is introduced alongside it.
    """
    tree = _tree()
    constructed = sorted(
        {
            node.func.id
            for node in ast.walk(tree)
            if isinstance(node, ast.Call)
            and isinstance(node.func, ast.Name)
            and node.func.id in UNSUPPORTED_CONSTRUCTORS
        },
    )
    assert constructed == []

    literals = [
        node
        for node in ast.walk(tree)
        if isinstance(node, (ast.Set, ast.SetComp))
        or (isinstance(node, ast.Constant) and isinstance(node.value, (bytes, complex)))
    ]
    assert literals == []


def test_the_training_workload_raises_no_unsupported_typeerror():
    """Half two, the other direction: the workload catches nothing.

    A `try`/`except TypeError` would be the shape that *hides* an
    unsupported-type raise while still training it, so the workload has no
    exception handler at all.
    """
    handlers = [node for node in ast.walk(_tree()) if isinstance(node, ast.ExceptHandler)]
    assert handlers == []


def test_the_integrations_copy_pins_the_same_thing():
    """The two copies must not drift apart (this file's docstring)."""
    scaffold = (PROJECT_ROOT / "tests" / "integrations" / "test_scaffold.py").read_text(
        encoding="utf-8",
    )
    assert "pgo_training.py" in scaffold
    assert 'keyword.arg == "default"' in scaffold
