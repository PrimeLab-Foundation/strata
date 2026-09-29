"""Hot-symbol table for the boundary arm, in attr/hot_symbols_table.txt's format.

offset from the first __text symbol (hex) / size in bytes (distance to the next __text symbol) /
address mod 64, per arm, using attr/layout.py's nm + otool measure. '-' = no out-of-line symbol.
usage: python3 hot_symbols.py <label>=<_strata .so> ...  -> stdout
"""

import subprocess
import sys

sys.path.insert(0, "/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/attr")
from layout import demangle, short, text_symbols  # noqa: E402

WANT = [
    "Serializer::write(_object*)",
    "Serializer::write_record_fused(_object*)",
    "Serializer::write_mapping_body(_object*,",
    "Serializer::write_mapping(_object*)",
    "Serializer::write_string(_object*)",
    "Serializer::write_string_bytes(char cons",
    "Serializer::write_scalar_run(_object**,",
    "util::format_double(double, char*, unsig",
    "b::dumps_to_python(_object*, bool)",
    "b::dump_to_file(_object*, char const*)",
    "Serializer::write_key_cold(_object*)",
    "Serializer::push_open_cold(_object*)",
    "Serializer::write_native(_object*)",
    "Serializer::write_set_table(_object*)",
    "Serializer::write_enum(_object*)",
    "Serializer::write_dataclass(_object*)",
    "Serializer::write_decimal(_object*)",
    "Serializer::write_native_text(char const",
    "Serializer::write_set(_object*)",
    "util::copy_until_escape_scalar(char cons",
    "util::find_next_escape(char const*, unsi",
    "util::find_next_escape_scalar(char const",
]


def arm(path):
    syms = text_symbols(path)
    dem = demangle([n for _, _, n in syms])
    first = syms[0][0]
    table = {}
    for addr, size, name in syms:
        s = short(dem[name])[:40]
        if s not in table:
            table[s] = (addr - first, size, addr % 64)
    sec = subprocess.run(["size", "-m", path], capture_output=True, text=True).stdout
    text = next(line.split(":")[1].strip() for line in sec.splitlines() if "Section __text" in line)
    return table, text


def main():
    arms = [a.split("=", 1) for a in sys.argv[1:]]
    data = [(label, *arm(path)) for label, path in arms]
    print("offset from first __text symbol (hex) / size B / address mod 64; '-' = no out-of-line symbol")
    print(f"{'Section __text (size -m)':48}" + "".join(f"{label + ' ' + text:>28}" for label, _, text in data))
    print(f"{'symbol':48}" + "".join(f"{label + ' off/size/mod64':>28}" for label, _, _ in data))
    for w in WANT:
        cells = []
        for _, table, _ in data:
            v = next((table[k] for k in table if k.startswith(w)), None)
            cells.append(f"{v[0]:x}/{v[1]}/{v[2]}" if v else "-")
        print(f"{w:48}" + "".join(f"{c:>28}" for c in cells))


if __name__ == "__main__":
    main()
