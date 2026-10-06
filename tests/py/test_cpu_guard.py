"""The import-time CPU guard of the x86-64-v3 images.

docs/architecture/release_pipeline.md, "CPU guard": where an image is compiled
with AVX2 on x86-64 (`-march=x86-64-v3`, `/arch:AVX2`), its init entry is
compiled for the x86-64 baseline and checks for x86-64-v3 before anything else
runs, raising ImportError on a CPU without it; everywhere else the guard is
compiled away. Each image checks for itself, since either can be loaded first.

The failure branch needs a CPU without x86-64-v3 and is exercised by running an
image on one -- Rosetta 2 without ROSETTA_ADVERTISE_AVX, Intel SDE's `-nhm` --
not here: this suite has to pass on the machine that built the image. What it
pins is that each image initialises on its own, and that the guard is present
exactly where the build's instruction set says it must be.
"""

import json
import platform
import subprocess
import sys
from pathlib import Path

import pytest
import strata._dumps_hook
import strata._strata

# The guard's ImportError text, present in an image only where the guard is
# compiled in (src/strata/bindings/python_cpu_guard.h).
_GUARD_TEXT = b"was built for x86-64-v3"

_IMAGES = {
    "strata._strata": strata._strata.__file__,
    "strata._dumps_hook": strata._dumps_hook.__file__,
}

# Loads one image by file in a fresh interpreter. The `strata` package is a
# stand-in whose path is the images' directory, so no package code runs first
# and the image's own init entry is the first of its code to execute; the hook
# image's init then imports the real `strata._strata` from that directory.
_LOAD_ALONE = """
import importlib.machinery, importlib.util, sys, types
package = types.ModuleType("strata")
package.__path__ = [{directory!r}]
sys.modules["strata"] = package
loader = importlib.machinery.ExtensionFileLoader({name!r}, {path!r})
spec = importlib.util.spec_from_loader({name!r}, loader)
module = importlib.util.module_from_spec(spec)
loader.exec_module(module)
print(sorted(name for name in sys.modules if name.startswith("strata.")))
"""

# Instruction-set flags as setup.py spells them, by whether they enable AVX2
# (and with it the guard). `-march=native` depends on the build host and is
# not classified.
_AVX2_FLAGS = ("-march=x86-64-v3", "-march=x86-64-v4", "/arch:AVX2", "/arch:AVX512")
_PRE_AVX2_FLAGS = ("-march=x86-64", "-march=x86-64-v2")


def _load_alone(name):
    path = _IMAGES[name]
    code = _LOAD_ALONE.format(name=name, path=path, directory=str(Path(path).parent))
    completed = subprocess.run(
        [sys.executable, "-I", "-c", code],
        capture_output=True,
        text=True,
        check=False,
    )
    assert completed.returncode == 0, completed.stderr
    return completed.stdout.strip()


def _isa_flags(path):
    """The instruction-set flags of every compile command in the image's
    `.build.json`, or None when the image has no build record."""
    record = Path(path + ".build.json")
    if not record.is_file():
        return None
    commands = json.loads(record.read_text(encoding="utf-8")).get("commands") or []
    flags = set()
    for command in commands:
        flags.update(arg for arg in command if arg.startswith(("-march=", "/arch:")))
    return flags


def test_strata_initialises_on_its_own():
    # Its init entry is the first of its code to run, guard included.
    assert _load_alone("strata._strata") == "['strata._strata']"


def test_the_hook_initialises_on_its_own():
    # Loaded before `strata._strata`, the hook's own init entry runs first and
    # its guard passes; only then does it import `_strata`.
    assert _load_alone("strata._dumps_hook") == "['strata._dumps_hook', 'strata._strata']"


@pytest.mark.parametrize("name", sorted(_IMAGES))
def test_the_guard_is_compiled_in_exactly_where_the_build_targets_avx2(name):
    path = _IMAGES[name]
    present = _GUARD_TEXT in Path(path).read_bytes()
    if platform.machine().lower() not in ("x86_64", "amd64"):
        # arm64 (and a universal2 image's x86_64 slice, built without -march):
        # compiled away entirely.
        assert not present
        return
    flags = _isa_flags(path)
    if flags is None:
        pytest.skip("no .build.json beside the image")
    if flags & set(_AVX2_FLAGS):
        assert present, sorted(flags)
    elif not flags or flags <= set(_PRE_AVX2_FLAGS):
        assert not present, sorted(flags)
    else:
        pytest.skip(f"instruction set depends on the build host: {sorted(flags)}")
