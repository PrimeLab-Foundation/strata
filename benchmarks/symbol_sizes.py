"""How much machine code each part of an extension is, by symbol size.

The cold half of the `dumps mixed` gap has been attributed to "the
serializer's footprint on smaller caches" without a number ever being put on
it. Code size is the measurable half of that claim: an engine whose hot walk
spans more bytes of text refaults more of it when the harness hands it a cold
cache, and an instruction cache is small (64 KB on the Neoverse-N2). This
groups every defined symbol's size.

usage: symbol_sizes.py <shared object> [<shared object> ...]

`readelf -sW` is the reader (ELF only, which is where this runs); `nm -S` is
the fallback. An object whose symbols carry no sizes, or a stripped one such
as the wheels competitors ship, says so rather than reporting a zero as if it
were a measurement.
"""

import subprocess
import sys

GROUPS = {
    "serializer walk": ("Serializer", "dumps_to_python", "StagedOutput", "SchemaCacheLease"),
    "float writer": ("format_double", "dtoa", "ragonbox", "shortest"),
    "int writer": ("format_int64", "fill_u64", "digits_word", "write_digits"),
    "escape and scan": ("copy_until_escape", "append_escaped", "escape_json", "find_next_escape"),
    "parser": ("ParserInline", "parse_json", "parse_sax", "DomBuilder", "PythonObjectBuilder"),
    "jsonpath": ("jsonpath", "JsonPath", "CompiledPath"),
}


def run(argv):
    try:
        proc = subprocess.run(argv, capture_output=True, text=True, check=False)
    except OSError:
        return ""  # the tool is not installed on this platform
    return proc.stdout if proc.returncode == 0 else ""


def from_readelf(path):
    """(size, kind, name) per defined symbol, kind 'FUNC' or 'OBJECT'."""
    text = run(["readelf", "-sWC", path]) or run(["readelf", "-sW", path])
    seen = {}
    for line in text.splitlines():
        parts = line.split(None, 7)
        if len(parts) < 8 or not parts[0].endswith(":"):
            continue
        value, size, kind, _bind, _vis, index, name = parts[1:8]
        if index == "UND" or kind not in ("FUNC", "OBJECT"):
            continue
        try:
            seen[(value, name)] = (int(size), kind, name)
        except ValueError:
            continue
    return sorted(seen.values(), reverse=True)


def from_nm(path):
    text = run(["nm", "-C", "-S", "--defined-only", path])
    rows = []
    for line in text.splitlines():
        parts = line.split(None, 3)
        if len(parts) < 4:
            continue
        try:
            size = int(parts[1], 16)
        except ValueError:
            continue
        kind = "FUNC" if parts[2] in ("t", "T") else "OBJECT"
        rows.append((size, kind, parts[3].strip()))
    return sorted(rows, reverse=True)


def main() -> int:
    for path in sys.argv[1:]:
        rows = from_readelf(path)
        reader = "readelf"
        if not any(size for size, _, _ in rows):
            rows = from_nm(path)
            reader = "nm"
        print(f"== {path}  (via {reader})")
        if not any(size for size, _, _ in rows):
            print("   no sized symbols — stripped, or this reader reports no sizes")
            continue
        text = sum(size for size, kind, _ in rows if kind == "FUNC")
        data = sum(size for size, kind, _ in rows if kind == "OBJECT")
        print(f"   code in sized symbols {text:>9d} bytes   data {data:>9d} bytes")
        print("   top 25 symbols by size:")
        for size, kind, name in rows[:25]:
            print(f"   {size:8d} {kind:6s} {name[:140]}")
        print("   grouped:")
        for label, keys in GROUPS.items():
            group = [row for row in rows if any(key in row[2] for key in keys)]
            print(
                f"   {label:18s} {sum(size for size, _, _ in group):8d} bytes "
                f"in {len(group):4d} symbols"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
