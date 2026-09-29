"""Byte identity of two builds' loaded code: the static step of M12b's criterion 5.

Usage: python benchmarks/image_identity.py <A-image> <B-image> [--json <path>]

Reads the section table of each image with the standard library alone -- ELF64
(Linux), Mach-O 64 (macOS) and PE32+ (Windows) -- and compares section bytes:

- the code section (`.text`, `__TEXT,__text`) is the gate: a difference exits 1,
  and the A/B workflow stops that leg before any timing, because an arm whose
  `_strata` code differs is not the pair M12b's design promises
  (docs/architecture/dumps_with_default.md, criterion 4/5); an image this
  script cannot read exits 2 (identity unverified, recorded, not a stop);
- every other loaded section is compared and reported, not gated: some carry a
  build-path or build-id fingerprint by construction (`.note.gnu.build-id`,
  PE `.rdata`'s debug directory), and the two arms are built in different
  directories.

Debug sections are never read (they carry checkout paths).
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

CODE = {".text", "__TEXT,__text"}


class UnreadableImage(Exception):
    """The section table could not be read: identity is unverified, not refuted."""


def _elf(data: bytes) -> dict[str, tuple[int, int]]:
    (shoff,) = struct.unpack_from("<Q", data, 0x28)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x3A)
    headers = [struct.unpack_from("<IIQQQQIIQQ", data, shoff + i * shentsize) for i in range(shnum)]
    strtab = headers[shstrndx]
    names = data[strtab[4] : strtab[4] + strtab[5]]
    result = {}
    for name_off, kind, flags, _addr, offset, size, *_ in headers:
        name = names[name_off : names.index(b"\0", name_off)].decode()
        if flags & 0x2 and kind != 8:  # SHF_ALLOC, not SHT_NOBITS
            result[name] = (offset, size)
    return result


def _macho(data: bytes) -> dict[str, tuple[int, int]]:
    ncmds, _sizeofcmds = struct.unpack_from("<II", data, 16)
    pos, result = 32, {}
    for _ in range(ncmds):
        cmd, cmdsize = struct.unpack_from("<II", data, pos)
        if cmd == 0x19:  # LC_SEGMENT_64
            nsects = struct.unpack_from("<I", data, pos + 64)[0]
            for i in range(nsects):
                sect = pos + 72 + i * 80
                sectname = data[sect : sect + 16].rstrip(b"\0").decode()
                segname = data[sect + 16 : sect + 32].rstrip(b"\0").decode()
                size, offset = struct.unpack_from("<QI", data, sect + 40)
                flags = struct.unpack_from("<I", data, sect + 64)[0]
                zerofill = (flags & 0xFF) in (0x1, 0xC, 0x12)
                if segname.startswith(("__TEXT", "__DATA")) and not zerofill:
                    result[f"{segname},{sectname}"] = (offset, size)
        pos += cmdsize
    return result


def _pe(data: bytes) -> dict[str, tuple[int, int]]:
    (pe,) = struct.unpack_from("<I", data, 0x3C)
    nsections, _ts, _sym, _nsym, optsize = struct.unpack_from("<HIIIH", data, pe + 6)
    table, result = pe + 24 + optsize, {}
    for i in range(nsections):
        entry = table + i * 40
        name = data[entry : entry + 8].rstrip(b"\0").decode(errors="replace")
        virtual_size, _va, raw_size, raw_offset = struct.unpack_from("<IIII", data, entry + 8)
        result[name] = (raw_offset, min(virtual_size, raw_size) if virtual_size else raw_size)
    return result


def sections(path: Path) -> dict[str, tuple[int, int]]:
    data = path.read_bytes()
    if data[:4] == b"\x7fELF":
        return _elf(data)
    if data[:4] == b"\xcf\xfa\xed\xfe":
        return _macho(data)
    if data[:2] == b"MZ":
        return _pe(data)
    raise UnreadableImage(f"{path}: not an ELF64, Mach-O 64 or PE image this script reads")


def digests(path: Path) -> dict[str, str]:
    data = path.read_bytes()
    return {
        name: hashlib.sha256(data[offset : offset + size]).hexdigest()
        for name, (offset, size) in sections(path).items()
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("a", type=Path)
    parser.add_argument("b", type=Path)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args(argv)
    try:
        da, db = digests(args.a), digests(args.b)
    except (UnreadableImage, struct.error, UnicodeDecodeError, ValueError, IndexError) as error:
        # Exit 2, not 1: a reader failure leaves identity unverified, which the
        # workflow records against the leg without stopping its timing.
        print(f"IDENTITY UNVERIFIED: {error}")
        return 2
    rows, code_identical = [], True
    for name in sorted(set(da) | set(db)):
        same = da.get(name) == db.get(name)
        if name in CODE:
            code_identical &= same
        rows.append({"section": name, "a": da.get(name), "b": db.get(name), "identical": same})
        tag = "identical" if same else ("DIFFERS (gate)" if name in CODE else "differs")
        print(f"{name:32} {(da.get(name) or '-')[:16]:16} {(db.get(name) or '-')[:16]:16} {tag}")
    if not any(name in CODE for name in da):
        print("no code section found", file=sys.stderr)
        code_identical = False
    print("CODE IDENTICAL" if code_identical else "CODE DIFFERS: stop before timing")
    if args.json:
        args.json.write_text(
            json.dumps({"code_identical": code_identical, "sections": rows}, indent=1)
        )
    return 0 if code_identical else 1


if __name__ == "__main__":
    raise SystemExit(main())
