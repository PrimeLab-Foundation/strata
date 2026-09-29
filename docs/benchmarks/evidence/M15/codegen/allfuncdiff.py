"""Per-function symbolized instruction comparison of two Mach-O objects.

Usage: allfuncdiff.py <before.o> <after.o> [--show SUBSTR ...]

Every __text symbol of both objects is disassembled (llvm-objdump -d -r), and
each function's instruction stream is normalized so that only codegen, not
object layout, can make two streams differ:
  - addresses dropped; in-function branch targets kept as <function+offset>;
  - relocations attached to the instruction they patch (@target);
  - linker-private labels resolved to what they label: l_.str.N / lCPI / other
    local data symbols -> their bytes (a C string up to NUL, else 16 bytes);
    ltmpN (section-start labels) -> the section name;
  - _OUTLINED_FUNCTION_N (arm64 machine outliner) -> a hash of that outlined
    body, so renumbering alone is no difference;
  - x86 '## 0xADDR <...>' target comments -> the bytes at ADDR when it lies in
    a data section, dropped when it is the instruction's own relocation slot.
Prints: functions only in before, only in after, and those present in both
whose normalized streams differ (instruction counts and -/+ line counts).
"""

import difflib
import hashlib
import re
import subprocess
import sys

OBJDUMP = subprocess.run(
    ["xcrun", "--find", "llvm-objdump"], check=True, capture_output=True, text=True
).stdout.strip()


def sections(path):
    """[(segname, sectname, addr, size, offset)] from otool -l."""
    out = subprocess.run(["otool", "-l", path], check=True, capture_output=True, text=True).stdout
    result, cur = [], {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[0] in ("sectname", "segname", "addr", "size", "offset"):
            cur[parts[0]] = parts[1]
            if parts[0] == "offset":
                result.append(
                    (
                        cur["segname"],
                        cur["sectname"],
                        int(cur["addr"], 16),
                        int(cur["size"], 16),
                        int(cur["offset"]),
                    )
                )
    return result


class Obj:
    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        self.sects = sections(path)
        self.syms = {}  # name -> (addr, section)
        out = subprocess.run(["nm", "-m", path], check=True, capture_output=True, text=True).stdout
        for line in out.splitlines():
            m = re.match(r"([0-9a-f]+) \((\w+),(\w+)\) .* (\S+)$", line)
            if m:
                self.syms[m.group(4)] = (int(m.group(1), 16), m.group(3))

    def bytes_at(self, addr):
        for seg, sect, base, size, off in self.sects:
            if base <= addr < base + size:
                if sect == "__text":
                    return None
                if "zerofill" in sect or sect in ("__bss", "__common") or off == 0:
                    return f"{sect}:zero"
                raw = self.data[off + addr - base : off + min(size, addr - base + 64)]
                if sect in ("__cstring",) or b"\0" in raw[:48]:
                    s = raw.split(b"\0", 1)[0]
                    if s and all(32 <= c < 127 for c in s):
                        return f'{sect}:"{s.decode()}"'
                return f"{sect}:{raw[:16].hex()}"
        return None

    def label(self, name):
        if name.startswith("_OUTLINED_FUNCTION_"):
            return "OUTLINED[" + OUTLINED.get((self.path, name), name) + "]"
        if re.match(r"ltmp\d+$", name) and name in self.syms:
            return "ltmp<" + self.syms[name][1] + ">"
        if (
            name.startswith("l_") or name.startswith("lCPI") or name.startswith("L")
        ) and name in self.syms:
            content = self.bytes_at(self.syms[name][0])
            if content:
                return "<" + content + ">"
        return name


OUTLINED = {}


def functions(obj):
    out = subprocess.run(
        [OBJDUMP, "-d", "-r", "--no-show-raw-insn", obj.path],
        check=True,
        capture_output=True,
        text=True,
    ).stdout
    funcs, cur, name = {}, None, None
    for line in out.splitlines():
        h = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if h:
            name = h.group(1)
            cur = funcs.setdefault(name, [])
            continue
        if cur is None:
            continue
        reloc = re.match(r"\s*[0-9a-f]+:\s+(\w*RELOC\w*|X86_64_\w+|ARM64_\w+)\s+(\S+)", line)
        if reloc and cur:
            cur[-1] += "  @" + reloc.group(2)
            continue
        m = re.match(r"\s*([0-9a-f]+):\s+(.*)", line)
        if not m:
            continue
        insn = m.group(2).strip()
        cm = re.search(r"##\s*0x([0-9a-f]+)\s*<([^>]*)>", insn)
        if cm:
            content = obj.bytes_at(int(cm.group(1), 16))
            insn = insn[: cm.start()].rstrip()
            if content:
                insn = re.sub(r"-?0x[0-9a-f]+\(%rip\)", "(%rip)", insn) + "  =" + content
        insn = re.sub(r"\s*##.*$", "", insn)
        insn = re.sub(r"0x[0-9a-f]+ <([^>]*)>", r"<\1>", insn)
        # arm64 adrp in an object: the printed target is the instruction's own
        # page (immediate 0, patched by the relocation) -- layout, not codegen.
        insn = re.sub(r"^(adrp\s+\w+,\s*)(0x[0-9a-f]+\s*)?<[^>]*>", r"\1<page>", insn)
        insn = re.sub(r"\s*;.*$", "", insn)
        insn = re.sub(r"\s+", " ", insn)
        cur.append(insn)
    return funcs


def normalize(obj, funcs):
    def fix(insn):
        return re.sub(r"(?<=[@<])([_A-Za-z.$][\w.$]*)", lambda m: obj.label(m.group(1)), insn)

    # outlined bodies first (they call nothing outlined), then everything else
    for name, body in funcs.items():
        if name.startswith("_OUTLINED_FUNCTION_"):
            text = "\n".join(fix(re.sub(r"<_OUTLINED_FUNCTION_\d+\+", "<SELF+", i)) for i in body)
            OUTLINED[(obj.path, name)] = hashlib.sha1(text.encode()).hexdigest()[:10]
    result = {}
    for name, body in funcs.items():
        if name.startswith("_OUTLINED_FUNCTION_"):
            continue
        result[name] = [re.sub(re.escape("<" + name + "+"), "<SELF+", fix(i)) for i in body]
    outlined = sorted(v for (p, _), v in OUTLINED.items() if p == obj.path)
    return result, outlined


def main():
    before_path, after_path = sys.argv[1:3]
    show = sys.argv[sys.argv.index("--show") + 1 :] if "--show" in sys.argv else []
    ob, oa = Obj(before_path), Obj(after_path)
    fb, outb = normalize(ob, functions(ob))
    fa, outa = normalize(oa, functions(oa))
    only_b = sorted(set(fb) - set(fa))
    only_a = sorted(set(fa) - set(fb))
    both = sorted(set(fb) & set(fa))
    differ = []
    for name in both:
        if fb[name] != fa[name]:
            diff = list(difflib.unified_diff(fb[name], fa[name], lineterm="", n=2))
            rem = sum(1 for d in diff if d.startswith("-") and not d.startswith("---"))
            add = sum(1 for d in diff if d.startswith("+") and not d.startswith("+++"))
            differ.append((name, len(fb[name]), len(fa[name]), rem, add, diff))
    print(f"# {before_path} -> {after_path}")
    print(
        f"# functions: before {len(fb)}, after {len(fa)}, common {len(both)}, "
        f"identical {len(both) - len(differ)}, differ {len(differ)}"
    )
    ob_set, oa_set = sorted(set(outb)), sorted(set(outa))
    print(
        f"# outlined bodies: before {len(outb)}, after {len(outa)}; "
        f"bodies only before {len(set(outb) - set(outa))}, only after {len(set(outa) - set(outb))}"
    )
    for n in only_b:
        print(f"only-before {len(fb[n]):5d}  {n}")
    for n in only_a:
        print(f"only-after  {len(fa[n]):5d}  {n}")
    for name, lb, la, rem, add, diff in differ:
        print(f"DIFFERS  insns {lb} -> {la}  (-{rem} +{add})  {name}")
    for name, lb, la, rem, add, diff in differ:
        if any(s in name for s in show):
            print(f"\n## {name}")
            for d in diff:
                print(d)


if __name__ == "__main__":
    main()
