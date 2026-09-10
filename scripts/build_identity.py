"""Build-time identities shared by setuptools and benchmark provenance.

Commands are captured from the compiler's spawn boundary, after setuptools
has added its platform flags. No report-time environment is treated as proof
of how an existing extension was built.
"""

from __future__ import annotations

import hashlib
import json
import subprocess
from pathlib import Path


def file_hash(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def command_output(
    command: list[str], *, cwd: Path | None = None, allow_failure: bool = False
) -> str | None:
    try:
        result = subprocess.run(command, cwd=cwd, capture_output=True, timeout=15, check=False)
    except (OSError, subprocess.TimeoutExpired):
        return None
    if result.returncode != 0 and not allow_failure:
        return None
    # An empty result is a result ("nothing differs"); only a failure is None.
    return (result.stdout + result.stderr).decode("utf-8", errors="replace").strip()


def source_identity(root: Path) -> dict:
    top = command_output(["git", "rev-parse", "--show-toplevel"], cwd=root)
    if top is None or Path(top).resolve() != root.resolve():
        return {
            "commit": None,
            "dirty": None,
            "patch_sha256": None,
            "untracked_sha256": {},
            "limitation": "source archive without its own Git metadata",
        }
    commit = command_output(["git", "rev-parse", "HEAD"], cwd=root)
    if commit is None:
        return {
            "commit": None,
            "dirty": None,
            "patch_sha256": None,
            "untracked_sha256": {},
            "limitation": "source archive without Git metadata",
        }
    patch = command_output(["git", "diff", "--binary", "HEAD", "--", "."], cwd=root)
    # Untracked source is part of a dirty checkout too. Ignored build outputs
    # are deliberately excluded, as are the bytes of any external files.
    names = command_output(["git", "ls-files", "--others", "--exclude-standard"], cwd=root)
    if patch is None or names is None:
        # A diff that failed or timed out is not a clean tree: consumers treat
        # anything but `dirty: False` as unverified, which is the honest state.
        return {
            "commit": commit,
            "dirty": None,
            "patch_sha256": None,
            "untracked_sha256": {},
            "limitation": "git diff or ls-files failed; the checkout's cleanliness is unknown",
        }
    untracked = {
        name: file_hash(root / name) for name in names.splitlines() if (root / name).is_file()
    }
    return {
        "commit": commit,
        "dirty": bool(patch or untracked),
        "patch_sha256": hashlib.sha256(patch.encode()).hexdigest(),
        "untracked_sha256": untracked,
    }


def profile_identity(path: str | None) -> dict | None:
    if not path:
        return None
    target = Path(path)
    files = [target] if target.is_file() else sorted(p for p in target.rglob("*") if p.is_file())
    result = {"path": str(target.resolve()), "files": {str(p): file_hash(p) for p in files}}
    composition = target.with_name(target.name + ".inputs.json")
    result["composition"] = json.loads(composition.read_text()) if composition.is_file() else None
    return result


def write_identity(
    extension: Path,
    *,
    root: Path,
    commands: list,
    profile: str | None,
    source: dict | None = None,
    required_sources: list[str] | None = None,
) -> None:
    destination = extension.with_name(extension.name + ".build.json")
    binary_hash = file_hash(extension)
    if not commands and destination.is_file():
        existing = json.loads(destination.read_text())
        if existing.get("extension_sha256") == binary_hash:
            return
    tools = sorted({str(command[0]) for command in commands if command})
    compiled = set()
    for command in commands:
        for argument in command:
            part = str(argument).strip('"')
            if part.startswith(("/Tp", "/Tc")):
                part = part[3:].strip('"')
            if part.endswith((".cpp", ".cc", ".c", ".cxx")):
                compiled.add(str((root / part).resolve()))
    complete = bool(commands) and all(
        str((root / name).resolve()) in compiled for name in (required_sources or [])
    )
    linker = None
    for tool in tools:
        if Path(tool).name.lower() not in {"cl.exe", "link.exe", "clang-cl.exe", "clang-cl"}:
            resolved = command_output([tool, "-print-prog-name=ld"])
            if resolved and "\n" not in resolved:
                linker = {
                    "path": resolved,
                    "version": command_output([resolved, "-v"], allow_failure=True),
                }
                break
    packet = {
        "schema_version": 1,
        "extension_sha256": binary_hash,
        "source": (source or source_identity(root)) if complete else None,
        "complete_compilation": complete,
        "commands": commands,
        "linker": linker,
        "tool_versions": {
            tool: command_output(
                [tool, "/?"]
                if Path(tool).name.lower() in {"cl.exe", "link.exe"}
                else [tool, "--version"],
                allow_failure=True,
            )
            for tool in tools
        },
        "pgo_profile": profile_identity(profile),
        "limitation": None
        if complete
        else "incremental build: full source compilation not observed",
    }
    destination.write_text(json.dumps(packet, indent=2, allow_nan=False) + "\n", encoding="utf-8")


def record_profile(profile: Path, raw: Path, root: Path, recipe: str) -> None:
    inputs = sorted(
        p
        for p in raw.rglob("*")
        if p.is_file() and p.suffix in {".profraw", ".gcda", ".pgc", ".pgd"}
    )
    source_files = [
        *sorted((root / "tests").rglob("*.py")),
        root / "scripts/pgo_training.py",
        root / "scripts/pgo_training_data.py",
    ]
    data = {
        "schema_version": 1,
        "recipe": recipe,
        "raw_inputs": {str(p): file_hash(p) for p in inputs},
        "workload_sources": {str(p.relative_to(root)): file_hash(p) for p in source_files},
        "training_data": {p.name: file_hash(p) for p in profile.parent.glob("train.*")},
        "limitation": "raw profiles include build/test gates and dedicated training; counts are not separated by phase",
    }
    profile.with_name(profile.name + ".inputs.json").write_text(
        json.dumps(data, indent=2) + "\n", encoding="utf-8"
    )


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Record the inputs of a merged PGO profile")
    parser.add_argument("--profile", type=Path, required=True)
    parser.add_argument("--raw", type=Path, required=True)
    parser.add_argument("--recipe", required=True)
    args = parser.parse_args()
    record_profile(args.profile, args.raw, Path(__file__).resolve().parents[1], args.recipe)
