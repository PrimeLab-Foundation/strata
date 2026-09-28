"""Both extension images compile the native type table, from one source.

docs/architecture/native_types.md, "Serializer": `python_native_types.cpp` is
compiled into `strata._strata` and into `strata._dumps_hook`, each image
keeping its own copy of the table its serializer's tail reads. It is a binding
source, so it belongs to setup.py's two source lists and never to the core
manifest (tests/unit/test_build_manifest.py pins that side for every binding
source).

This file sits in the training-ignored directory rather than beside
tests/unit/test_hook_build_scope.py: `tests/unit`'s autouse config fixture
would put its calls into the PGO profile (docs/decisions.md, 2026-09-28).
"""

import ast
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[3]
NATIVE = "src/strata/bindings/python_native_types.cpp"


def _source_lists():
    tree = ast.parse((PROJECT_ROOT / "setup.py").read_text(encoding="utf-8"))
    lists = {}
    for node in tree.body:
        if not isinstance(node, ast.Assign):
            continue
        for target in node.targets:
            if isinstance(target, ast.Name) and target.id.endswith("BINDING_SOURCES"):
                lists[target.id] = ast.literal_eval(node.value)
    return lists


def test_the_engine_image_compiles_the_native_type_table():
    assert _source_lists()["BINDING_SOURCES"].count(NATIVE) == 1


def test_the_hook_image_compiles_it_after_the_serializer():
    assert _source_lists()["HOOK_BINDING_SOURCES"] == [
        "src/strata/bindings/python_dumps_hook.cpp",
        NATIVE,
    ]


def test_it_is_not_a_core_source():
    manifest = (PROJECT_ROOT / "src" / "strata" / "core_sources.txt").read_text(encoding="utf-8")
    assert NATIVE not in manifest
    assert (PROJECT_ROOT / NATIVE).is_file()
    assert (PROJECT_ROOT / NATIVE.replace(".cpp", ".h")).is_file()
