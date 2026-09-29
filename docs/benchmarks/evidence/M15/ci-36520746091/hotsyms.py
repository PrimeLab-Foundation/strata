"""Hot-writer symbol sizes of a run's A/A2/B arms per leg (ELF / Mach-O via llvm-nm; PE via
llvm-objdump's COFF symbol table if present). Prints only the serializer rows + any
function whose size differs between A and A2 vs A and B."""
import re
import subprocess
import sys
from pathlib import Path

NM = "/opt/homebrew/opt/llvm/bin/llvm-nm"
HOT = re.compile(
    r"Serializer::(write\(|write_string\(|write_string_bytes|write_mapping_body|write_mapping\(|"
    r"write_record_fused\(|write_record_fused_value|write_sequence|write_scalar_run|write_int|"
    r"write_double|Frame::Frame|push_open|close_container|latch|write_mapping_uncached|"
    r"write_native|write_key_cold|write_dataclass|write_set|write_enum|write_decimal)"
)


def sizes(path: Path) -> dict[str, int]:
    out = subprocess.run(
        [NM, "-S", "-C", "--defined-only", str(path)], capture_output=True, text=True
    ).stdout
    table: dict[str, int] = {}
    for line in out.splitlines():
        parts = line.split(None, 3)
        if len(parts) < 4 or parts[2].lower() not in ("t", "w"):
            continue
        name = parts[3].replace("strata::bindings::(anonymous namespace)::", "")
        name = re.sub(r" \[clone \.llvm\.\d+\]", "", name)
        try:
            table[name] = table.get(name, 0) + int(parts[1], 16)
        except ValueError:
            continue
    return table


for leg in sys.argv[1:]:
    arms = Path(leg) / "arms"
    ext = ".pyd" if (arms / "A.pyd").exists() else ".so"
    tabs = {a: sizes(arms / f"{a}{ext}") for a in ("A", "A2", "B")}
    print(f"== {Path(leg).name}: symbols A={len(tabs['A'])} A2={len(tabs['A2'])} B={len(tabs['B'])}")
    if len(tabs["A"]) < 20:
        print("   (no usable symbol table in this image)")
        continue
    names = sorted({n for t in tabs.values() for n in t if HOT.search(n)})
    for n in names:
        a, a2, b = (tabs[x].get(n, 0) for x in ("A", "A2", "B"))
        flag = "" if a == b else ("  <- B differs" + ("" if a == a2 else " (A2 differs too)"))
        print(f"   {a:6d} {a2:6d} {b:6d}  {n[:80]}{flag}")
    shared = [n for n in tabs["A"] if n in tabs["B"] and not HOT.search(n)]
    moved = [n for n in shared if tabs["A"][n] != tabs["B"][n] and tabs["A"][n] == tabs["A2"].get(n)]
    print(f"   other functions: {len(shared)} shared, {len(moved)} differ A vs B while A == A2")
    for n in sorted(moved, key=lambda n: -abs(tabs['B'][n] - tabs['A'][n]))[:8]:
        print(f"     {tabs['A'][n]:6d} -> {tabs['B'][n]:6d}  {n[:90]}")
    gone = [n for n in tabs["A"] if n not in tabs["B"] and n in tabs["A2"]]
    new_hot = [n for n in tabs["B"] if n not in tabs["A"] and not re.search(r"native|Native|numpy|parse_types|temporal|write_key_cold|push_open_cold|write_dataclass|write_set|write_enum|write_decimal|revive|Revival|scan_temporal|format_", n)]
    print(f"   in A (and A2) but not B: {gone[:8]}")
    print(f"   new in B (non-native): {[n[:70] for n in new_hot[:12]]}")
