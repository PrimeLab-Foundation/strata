"""Run benchmarks/symbol_sizes.py's report on a Mach-O extension.

Usage: symbol_sizes_macho.py <tree>/benchmarks/symbol_sizes.py <so> [<so> ...]

symbol_sizes.py reads sizes with readelf (ELF only) or `nm -S`; on Mach-O, nm
prints "sizes with --print-size for Mach-O files are always zero", so the
script reports "no sized symbols". This wrapper substitutes its readers with
the M12 measure (symsizes.py: a __text symbol's size is the distance to the
next symbol, alignment padding included, so the sizes sum to Section __text),
names demangled with c++filt, and then runs the script's own main(): same top
25, same GROUPS.
"""

import importlib.util
import pathlib
import subprocess
import sys

HERE = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import symsizes  # noqa: E402


def load(path):
    spec = importlib.util.spec_from_file_location("symbol_sizes", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def macho_rows(path):
    sizes = symsizes.sizes(path)
    names = list(sizes)
    demangled = subprocess.run(
        ["xcrun", "c++filt", "-_"],
        input="\n".join(names),
        capture_output=True,
        text=True,
        check=True,
    ).stdout.splitlines()
    rows = [(sizes[n], "FUNC", d) for n, d in zip(names, demangled)]
    return sorted(rows, reverse=True)


def main():
    module = load(sys.argv[1])
    module.from_readelf = macho_rows
    module.from_nm = macho_rows
    sys.argv = [sys.argv[0], *sys.argv[2:]]
    return module.main()


if __name__ == "__main__":
    raise SystemExit(main())
