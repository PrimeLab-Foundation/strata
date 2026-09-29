"""`strata._dumps_hook` alone compiles the native type table.

docs/architecture/native_types.md, "Flag shape (M15b)": every native writer,
the native type table (`python_native_types.cpp`) and the numpy twins'
runtime proof (`python_numpy_twins.cpp`) live only in `strata._dumps_hook`, so
`strata._strata`'s source list, order and flags stay main `38eaa9f`'s exactly.
Each is a binding source, so it belongs to setup.py's hook-only source list
and never to the core manifest (tests/unit/test_build_manifest.py pins that
side for every binding source).

This file sits in the training-ignored directory rather than beside
tests/unit/test_hook_build_scope.py: `tests/unit`'s autouse config fixture
would put its calls into the PGO profile (docs/decisions.md, 2026-09-28).
"""

import ast
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[3]
NATIVE = "src/strata/bindings/python_native_types.cpp"
#: The numpy twins' runtime proof, split out of NATIVE (the ~800 LOC rule).
NUMPY_TWINS = "src/strata/bindings/python_numpy_twins.cpp"
#: The folder/file writer halves the hook needs for `dump_native`.
FILES = "src/strata/bindings/python_files.cpp"
FOLDER = "src/strata/bindings/python_folder.cpp"
#: `parse_types`'s revival (docs/architecture/native_types.md task M15b, item
#: 5): hook-only, parsing through `_strata`'s own public entries.
PARSE_TYPES = "src/strata/bindings/python_parse_types.cpp"
PARSE_TYPES_WALK = "src/strata/bindings/python_parse_types_walk.cpp"


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


def test_the_engine_image_does_not_compile_the_native_type_table():
    sources = _source_lists()["BINDING_SOURCES"]
    for source in (NATIVE, NUMPY_TWINS, PARSE_TYPES, PARSE_TYPES_WALK):
        assert source not in sources


def test_the_hook_image_compiles_the_native_type_table_and_the_file_writers():
    assert _source_lists()["HOOK_BINDING_SOURCES"] == [
        "src/strata/bindings/python_dumps_hook.cpp",
        NATIVE,
        NUMPY_TWINS,
        PARSE_TYPES,
        PARSE_TYPES_WALK,
        FILES,
        FOLDER,
    ]


def test_it_is_not_a_core_source():
    manifest = (PROJECT_ROOT / "src" / "strata" / "core_sources.txt").read_text(encoding="utf-8")
    for source in (NATIVE, NUMPY_TWINS):
        assert source not in manifest
        assert (PROJECT_ROOT / source).is_file()
        assert (PROJECT_ROOT / source.replace(".cpp", ".h")).is_file()


def test_the_parse_side_revival_is_hook_only():
    # docs/architecture/native_types.md task M15b, item 5: the parse-side
    # revival is wired into the hook alone -- `_strata` stays main's.
    lists = _source_lists()
    for source in (PARSE_TYPES, PARSE_TYPES_WALK):
        assert source not in lists["BINDING_SOURCES"]
        assert source in lists["HOOK_BINDING_SOURCES"]


def test_the_hook_extension_defines_the_hook_macro_and_the_engine_extension_does_not():
    # P0: python_dumps_hook.cpp #defines STRATA_DUMPS_HOOK for itself (and for
    # python_dumps.cpp, which it #includes directly), but python_files.cpp and
    # python_folder.cpp are separate translation units in the same Extension --
    # without the macro at the Extension level too, their reader halves
    # compile into the hook as unresolved externals against `_strata`-only
    # symbols (loads_to_python, make_root_iterator, ...), hidden by dead-strip
    # on macOS/Linux but an MSVC link error on Windows.
    tree = ast.parse((PROJECT_ROOT / "setup.py").read_text(encoding="utf-8"))
    extensions = {}
    for node in ast.walk(tree):
        if isinstance(node, ast.Call) and getattr(node.func, "id", None) == "Extension":
            name = ast.literal_eval(node.args[0])
            macros = next(
                (kw.value for kw in node.keywords if kw.arg == "define_macros"),
                None,
            )
            extensions[name] = ast.literal_eval(macros) if macros is not None else []
    assert ("STRATA_DUMPS_HOOK", "1") in extensions["strata._dumps_hook"]
    assert extensions["strata._strata"] == []


def test_the_parse_side_revival_is_not_a_core_source():
    manifest = (PROJECT_ROOT / "src" / "strata" / "core_sources.txt").read_text(encoding="utf-8")
    for source in (PARSE_TYPES, PARSE_TYPES_WALK):
        assert source not in manifest
        assert (PROJECT_ROOT / source).is_file()
        assert (PROJECT_ROOT / source.replace(".cpp", ".h")).is_file()


def test_temporal_is_a_native_source_not_a_core_source():
    # docs/architecture/native_types.md, "Composition of the hook image":
    # src/strata/util/temporal.cpp leaves core_sources.txt (never linked into
    # `_strata`) for native_sources.txt (the C++ temporal test and the hook).
    core_manifest = (PROJECT_ROOT / "src" / "strata" / "core_sources.txt").read_text(
        encoding="utf-8"
    )
    native_manifest = (PROJECT_ROOT / "src" / "strata" / "native_sources.txt").read_text(
        encoding="utf-8"
    )
    temporal = "src/strata/util/temporal.cpp"
    assert temporal not in core_manifest
    assert temporal in native_manifest.splitlines()
