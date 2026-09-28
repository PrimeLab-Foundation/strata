"""Compare two builds object by object: every non-__DWARF section's bytes, the
relocations, and the symbol table (names, sections, values).

Usage: sectcmp.py <before-obj-dir> <after-obj-dir>

__DWARF is left out because -g records each tree's absolute source path, which
differs between two worktrees of the same source. Everything that becomes code
or data in the linked extension is compared byte for byte.
"""

import hashlib
import pathlib
import subprocess
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from allfuncdiff import sections  # noqa: E402


def fingerprint(path):
    data = pathlib.Path(path).read_bytes()
    parts = {}
    for seg, sect, _addr, size, off in sections(path):
        if seg == "__DWARF":
            continue
        body = data[off : off + size] if off else b"zerofill:%d" % size
        parts[f"{seg},{sect}"] = hashlib.sha256(body).hexdigest()[:16]
    relocs = subprocess.run(["otool", "-r", path], capture_output=True, text=True, check=True)
    parts["relocations"] = hashlib.sha256(relocs.stdout.split("\n", 1)[1].encode()).hexdigest()[:16]
    syms = subprocess.run(["nm", "-m", path], capture_output=True, text=True, check=True)
    parts["symbols"] = hashlib.sha256(syms.stdout.encode()).hexdigest()[:16]
    return parts


def main():
    before, after = map(pathlib.Path, sys.argv[1:3])
    same = total = 0
    for obj in sorted(before.glob("*.o")):
        other = after / obj.name
        if not other.exists():
            print(f"only-before {obj.name}")
            continue
        total += 1
        fb, fa = fingerprint(obj), fingerprint(other)
        if fb == fa:
            same += 1
            print(f"IDENTICAL  {obj.name}  ({len(fb) - 2} sections + relocations + symbols)")
        else:
            keys = sorted(k for k in set(fb) | set(fa) if fb.get(k) != fa.get(k))
            print(f"DIFFERS    {obj.name}  {' '.join(keys)}")
    for obj in sorted(after.glob("*.o")):
        if not (before / obj.name).exists():
            print(f"only-after {obj.name}")
    print(f"# {same} of {total} common objects identical")


if __name__ == "__main__":
    main()
