"""Compare two builds of the extension by swapping the `.so` between processes.

A build comparison on a desktop that is also being used has one honest shape,
and this is it:

* **Same virtualenv, same interpreter, same rival binaries.** Only the strata
  extension changes between launches — with its Python facade, when the arms
  carry theirs (`<build>.facade/`) — so nothing else can explain a difference.
* **Fresh process per launch.** A loaded shared library cannot be replaced in
  place; a driver that pretends otherwise is measuring the first build twice.
* **A-B-B-A blocks.** Linear drift over a block cancels inside the block: the
  mean of the two A launches and the mean of the two B launches sit at the same
  point in time. The order is predeclared on the command line and every launch
  is kept, including the ones that disagree.
* **A repeated baseline at the end.** The trailing A against the leading A is
  the session's own drift, measured rather than assumed.
* **A/A first.** Run this with two builds of *unchanged* source before running
  it with a candidate. Whatever spread A/A shows is the floor; an effect
  smaller than the floor is not an effect, and this tool cannot make it one.

Safety, all four rules learned the hard way (the 7 September review, finding 5):

* The driver overwrites the extension of the checkout it lives in and refuses
  any other target. Point it at another tree's `.so` and it exits.
* The extension that was installed when the run started is **copied aside
  first and restored in a `finally`**, then re-hashed. That covers a run that
  finishes and a run that raises — including a failing launch. It cannot
  cover a signal: a `kill -9` (or a lost machine) leaves an arm installed and
  the saved copy behind as `<target>.ab_original`. The recovery is the *next*
  run, which finds that copy and puts the original back before taking one of
  its own; the original is never overwritten by an arm.
* Every completed launch's samples are **appended to the TSV and flushed
  immediately**, so a failure at launch 19 keeps launches 0-18 instead of
  discarding the whole session.
* The driver process must never have imported the extension it is about to
  overwrite: a loaded shared library cannot be swapped, and a driver holding
  one would measure it in every launch. It refuses to start if `strata` is
  already imported.

usage:
  ab_builds.py --build A=<so> --build B=<so> --target <so> --out <tsv>
               [--order ABBA] [--blocks 3] [--tail A] [--repeat 60]
  ab_builds.py --analyze <tsv> [--baseline-build A] [--min-samples N]

The `--min-samples` default is `ab_blocks.DEFAULT_MIN_SAMPLES`, shared with
`ab_blocks.py` and `ab_floor.py`: the three views of a packet agree on whether
it is valid.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import statistics
import subprocess
import sys
from collections.abc import Callable
from pathlib import Path

from benchmarks import ab_blocks, ab_rounds

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def _digest(path: Path) -> str:
    return hashlib.md5(path.read_bytes()).hexdigest()  # noqa: S324


def _refuse_if_target_imported(target: Path) -> None:
    """A driver that has loaded the target cannot swap the target.

    `shutil.copy2` over a mapped `.so` either fails or leaves the running
    process on the old image; either way every launch after the first would be
    measuring whatever the driver itself loaded. The check is against the file
    the extension modules were loaded from, so it names the actual conflict
    rather than any import called `strata`.
    """
    resolved = target.resolve()
    loaded = sorted(
        name
        for name, module in list(sys.modules.items())
        if getattr(module, "__file__", None) and Path(module.__file__).resolve() == resolved
    )
    if loaded:
        raise SystemExit(
            f"refusing to run: this process has already imported {resolved} as {loaded}; "
            "the driver must not import the extension it replaces"
        )


class SampleWriter:
    """The TSV, written as the session goes rather than at the end.

    Every completed launch reaches the file before the next one starts, so a
    campaign that dies at launch 19 leaves nineteen usable launches behind
    instead of nothing. `flush` + `fsync` because the failure this guards
    against includes the driver being killed.
    """

    def __init__(self, path: Path) -> None:
        self.path = Path(path)
        self.path.parent.mkdir(parents=True, exist_ok=True)
        self._handle = open(self.path, "w", encoding="utf-8")  # noqa: SIM115
        self._handle.write("\t".join(ab_rounds.TSV_HEADER) + "\n")
        self._sync()
        self.launches = 0

    def _sync(self) -> None:
        self._handle.flush()
        os.fsync(self._handle.fileno())

    def add(self, lines: list[str]) -> None:
        for line in lines:
            if line.strip():
                self._handle.write(line + "\n")
        self._sync()
        self.launches += 1

    def close(self) -> None:
        if not self._handle.closed:
            self._sync()
            self._handle.close()


class InstalledExtension:
    """The extension in the checkout: saved, swapped, restored, verified.

    The restore is unconditional (`finally` in `drive`) and is followed by a
    hash check, because "the run finished, so the tree is clean" is exactly the
    assumption that left arm A installed after a successful campaign.

    A `<target>.ab_original` that is already there when this object is built
    is a previous campaign's saved product — the state a signal leaves, since
    a killed driver never reaches its `finally`. Copying the target over it,
    which this class used to do unconditionally, destroys the only copy of the
    product and then "restores" an arm over it (the 7 September review's T3
    follow-up, defect 1). So the leftover copy decides what happens: identical
    to the installed file, the previous run did restore and the copy is stale
    scrap this run reuses as its own backup; different, the installed file is
    that run's arm and the copy is the product, put back before anything else.
    """

    def __init__(self, target: Path) -> None:
        self.target = _check_target(target)
        if not self.target.exists():
            raise SystemExit(f"no extension to swap at {self.target}")
        self.backup = self.target.with_name(self.target.name + ".ab_original")
        # The build identity beside the extension describes *that* binary;
        # every swap moves it too, or provenance capture refuses the arm.
        self.sidecar = self.target.with_name(self.target.name + ".build.json")
        self.sidecar_backup = self.sidecar.with_name(self.sidecar.name + ".ab_original")
        if self.backup.exists():
            self._recover()
        else:
            shutil.copy2(self.target, self.backup)
            if self.sidecar.exists():
                shutil.copy2(self.sidecar, self.sidecar_backup)
        self.digest = _digest(self.target)

    def _recover(self) -> None:
        """Read the leftover backup before writing anything."""
        saved = _digest(self.backup)
        if saved == _digest(self.target):
            print(
                f"# {self.backup} matches the installed extension: a stale backup of an "
                "intact original, reused as this run's backup",
                file=sys.stderr,
                flush=True,
            )
            return
        print(
            f"# {self.backup} (md5={saved}) is not the installed extension "
            f"(md5={_digest(self.target)}): a previous run was killed before it could "
            f"restore. Putting {self.target} back from it before this run starts.",
            file=sys.stderr,
            flush=True,
        )
        shutil.copy2(self.backup, self.target)
        self._restore_sidecar()
        restored = _digest(self.target)
        if restored != saved:
            raise SystemExit(
                f"recovery failed: {self.target} is {restored}, the saved original was "
                f"{saved}; the saved copy is kept at {self.backup}"
            )

    def _restore_sidecar(self) -> None:
        if self.sidecar_backup.exists():
            shutil.copy2(self.sidecar_backup, self.sidecar)
        elif self.sidecar.exists():
            self.sidecar.unlink()

    def install(self, source: Path) -> str:
        shutil.copy2(source, self.target)
        source_sidecar = source.with_name(source.name + ".build.json")
        if source_sidecar.exists():
            shutil.copy2(source_sidecar, self.sidecar)
        elif self.sidecar.exists():
            self.sidecar.unlink()
        digest = _digest(self.target)
        if digest != _digest(source):
            raise SystemExit(f"copy of {source} to {self.target} did not land: {digest}")
        return digest

    def restore(self) -> None:
        shutil.copy2(self.backup, self.target)
        self._restore_sidecar()
        digest = _digest(self.target)
        if digest != self.digest:
            raise SystemExit(
                f"restore failed: {self.target} is {digest}, the original was {self.digest}; "
                f"the saved copy is kept at {self.backup}"
            )
        self.backup.unlink()
        if self.sidecar_backup.exists():
            self.sidecar_backup.unlink()


FACADE_SUFFIX = ".facade"


def arm_facade(build: Path) -> Path | None:
    """The Python facade collected beside @p build (`<build>.facade/`), if any."""
    facade = build.with_name(build.name + FACADE_SUFFIX)
    return facade if facade.is_dir() else None


def arm_facades(builds: dict[str, Path]) -> dict[str, Path] | None:
    """Each arm's facade, or None when no arm carries one; a mixed set is refused.

    An extension run under another revision's facade either fails to launch
    (run 36497513720: the candidate's `loads` passes `parse_types=` to main's
    `_strata`) or times Python code its arm does not ship. With no facade beside
    any arm, every launch runs the checkout's own, as before; with some but not
    all, which facade an arm runs would depend on the order, so nothing runs.
    """
    facades = {tag: arm_facade(path) for tag, path in builds.items()}
    missing = sorted(tag for tag, facade in facades.items() if facade is None)
    if len(missing) == len(facades):
        return None
    if missing:
        raise SystemExit(
            f"builds {missing} carry no <build>{FACADE_SUFFIX}/ while the others do; "
            "every arm needs its own facade or none may have one"
        )
    return facades


def facade_files(root: Path) -> list[Path]:
    """The facade: every `*.py` under @p root, as paths relative to it."""
    return sorted(
        path.relative_to(root)
        for path in root.rglob("*.py")
        if "__pycache__" not in path.relative_to(root).parts
    )


def facade_digest(root: Path) -> str:
    digest = hashlib.md5()  # noqa: S324
    for relative in facade_files(root):
        data = (root / relative).read_bytes()
        digest.update(f"{relative.as_posix()}\0{len(data)}\0".encode())
        digest.update(data)
    return digest.hexdigest()


def _replace_facade(destination: Path, source: Path) -> None:
    """Make @p destination's facade exactly @p source's; other files stay.

    The bytecode caches go too: `copy2` keeps the source's mtime, and a cache
    validated by mtime and size could otherwise serve the other arm's code.
    """
    if destination.exists():
        for relative in facade_files(destination):
            (destination / relative).unlink()
        for cache in list(destination.rglob("__pycache__")):
            shutil.rmtree(cache)
    for relative in facade_files(source):
        (destination / relative).parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source / relative, destination / relative)


class InstalledFacade:
    """The checkout's Python facade: saved, swapped per launch, restored, verified.

    The same contract as `InstalledExtension`, for the `*.py` files of the
    package directory the extension sits in. The saved copy is a sibling
    directory, `<package>.ab_original_facade`, outside the package (a dotted
    name is never a package). It is written under a `.partial` name and renamed
    once complete, so a leftover backup is always a whole facade and follows the
    extension's recovery rule: equal to the installed facade, it is reused;
    different, it is put back before anything else.
    """

    def __init__(self, package: Path) -> None:
        self.package = package
        self.backup = package.with_name(package.name + ".ab_original_facade")
        if self.backup.exists():
            self._recover()
        else:
            partial = self.backup.with_name(self.backup.name + ".partial")
            shutil.rmtree(partial, ignore_errors=True)
            partial.mkdir()
            _replace_facade(partial, package)
            partial.rename(self.backup)
        self.digest = facade_digest(self.package)

    def _recover(self) -> None:
        saved = facade_digest(self.backup)
        if saved == facade_digest(self.package):
            print(
                f"# {self.backup} matches the installed facade: reused as this run's backup",
                file=sys.stderr,
                flush=True,
            )
            return
        print(
            f"# {self.backup} (md5={saved}) is not the installed facade "
            f"(md5={facade_digest(self.package)}): a previous run was killed before it "
            f"could restore. Putting {self.package} back from it before this run starts.",
            file=sys.stderr,
            flush=True,
        )
        _replace_facade(self.package, self.backup)
        if facade_digest(self.package) != saved:
            raise SystemExit(
                f"facade recovery failed: {self.package} does not match the saved "
                f"original; the saved copy is kept at {self.backup}"
            )

    def install(self, source: Path) -> str:
        _replace_facade(self.package, source)
        digest = facade_digest(self.package)
        if digest != facade_digest(source):
            raise SystemExit(f"facade {source} did not land in {self.package}: {digest}")
        return digest

    def restore(self) -> None:
        _replace_facade(self.package, self.backup)
        digest = facade_digest(self.package)
        if digest != self.digest:
            raise SystemExit(
                f"facade restore failed: {self.package} is {digest}, the original was "
                f"{self.digest}; the saved copy is kept at {self.backup}"
            )
        shutil.rmtree(self.backup)


def drive(
    order: list[str],
    builds: dict[str, Path],
    target: Path,
    out: Path,
    launch: Callable[[int, str], list[str]],
) -> SampleWriter:
    """Run the launch sequence, persisting as it goes and restoring at the end.

    `launch(index, tag)` returns the TSV lines that launch produced; it is a
    parameter so the drivers' subprocess machinery and this file's recovery
    guarantees can be tested apart from each other. When the arms carry their
    facades (`arm_facades`), each launch installs its arm's beside the
    extension, and the checkout's is put back in the same `finally`.
    """
    _refuse_if_target_imported(target)
    facades = arm_facades(builds)
    extension = InstalledExtension(target)
    facade = None
    try:
        if facades is None:
            print("# facade   not swapped: no build carries one", file=sys.stderr, flush=True)
        else:
            facade = InstalledFacade(extension.target.parent)
            print(f"# facade   {facade.package} swapped per launch", file=sys.stderr, flush=True)
        writer = SampleWriter(out)
        try:
            for index, tag in enumerate(order):
                digest = extension.install(builds[tag])
                note = "" if facade is None else f" facade_md5={facade.install(facades[tag])}"
                print(
                    f"# launch {index:02d} build={tag} md5={digest}{note}",
                    file=sys.stderr,
                    flush=True,
                )
                writer.add(launch(index, tag))
        finally:
            writer.close()
            print(
                f"# wrote {out} ({writer.launches} of {len(order)} launches)",
                file=sys.stderr,
                flush=True,
            )
    finally:
        try:
            extension.restore()
            print(f"# restored {target} to md5={extension.digest}", file=sys.stderr, flush=True)
        finally:
            if facade is not None:
                facade.restore()
                print(
                    f"# restored facade {facade.package} to md5={facade.digest}",
                    file=sys.stderr,
                    flush=True,
                )
    return writer


def _check_target(target: Path) -> Path:
    resolved = target.resolve()
    try:
        resolved.relative_to(PROJECT_ROOT)
    except ValueError:
        raise SystemExit(
            f"refusing to write outside this checkout: {resolved} is not under {PROJECT_ROOT}"
        ) from None
    return resolved


def _check_build_identity(path: Path) -> None:
    """Refuse unverified or known-incompatible arms before swapping anything."""
    sidecar = path.with_name(path.name + ".build.json")
    try:
        metadata = json.loads(sidecar.read_text())
    except (OSError, ValueError) as exc:
        raise SystemExit(f"missing or invalid build identity for {path}: {exc}") from exc
    if metadata.get("extension_sha256") != hashlib.sha256(path.read_bytes()).hexdigest():
        raise SystemExit(f"build identity hash mismatch: {path}")
    # Older identities have no explicit ABI field. Their compiler include
    # paths still identify versioned CPython headers on POSIX. Unversioned
    # Windows include paths cannot establish ABI compatibility this way.
    versions = set(
        re.findall(r"python(3\.\d+)(?:/|\\|\")", json.dumps(metadata.get("commands", [])))
    )
    current = f"{sys.version_info.major}.{sys.version_info.minor}"
    if versions and versions != {current}:
        raise SystemExit(
            f"incompatible Python headers for {path}: {sorted(versions)}; running {current}"
        )


def parse_builds(entries: list[str]) -> dict[str, Path]:
    builds: dict[str, Path] = {}
    for entry in entries:
        tag, _, path = entry.partition("=")
        if not path:
            raise SystemExit(f"--build wants TAG=PATH, got {entry!r}")
        builds[tag] = Path(path).resolve()
        if not builds[tag].exists():
            raise SystemExit(f"no such build: {builds[tag]}")
        _check_build_identity(builds[tag])
    return builds


def plan_order(order: str, blocks: int, tail: str, builds: dict[str, Path]) -> list[str]:
    planned = list(order) * blocks + list(tail)
    unknown = sorted(set(planned) - set(builds))
    if unknown:
        raise SystemExit(f"order names builds that were not given: {unknown}")
    return planned


def subprocess_launch(command_for: Callable[[int, str], list[str]], env: dict[str, str]):
    """A launcher that runs one child per launch and returns its stdout lines.

    A child that fails still reaches the caller's `finally`, so the samples
    already written stay written and the original extension is restored; the
    child's stderr is printed first, because that is where the probe says which
    extension it actually loaded.
    """

    def launch(index: int, tag: str) -> list[str]:
        command = command_for(index, tag)
        result = subprocess.run(  # noqa: S603
            command,
            cwd=PROJECT_ROOT,
            capture_output=True,
            text=True,
            env={**env, "PYTHONPATH": str(PROJECT_ROOT)},
            check=False,
        )
        sys.stderr.write(result.stderr)
        sys.stderr.flush()
        if result.returncode != 0:
            raise SystemExit(f"launch {index:02d} ({tag}) failed with {result.returncode}")
        return [line for line in result.stdout.splitlines() if line.strip()]

    return launch


def run(args: argparse.Namespace) -> int:
    builds = parse_builds(args.build)
    target = _check_target(Path(args.target))
    order = plan_order(args.order, args.blocks, args.tail, builds)

    print(f"# target   {target}", file=sys.stderr)
    for tag, path in builds.items():
        print(f"# build {tag}  {path}  md5={_digest(path)}", file=sys.stderr)
    print(f"# order    {''.join(order)} ({len(order)} launches)", file=sys.stderr)

    def command_for(index: int, tag: str) -> list[str]:
        command = [
            sys.executable,
            "benchmarks/dumps_rows_probe.py",
            "--build",
            tag,
            "--tag",
            f"L{index:02d}",
            "--tier",
            args.tier,
            "--repeat",
            str(args.repeat),
            "--identify",
        ]
        for dataset in args.dataset or []:
            command += ["--dataset", dataset]
        return command

    drive(order, builds, target, Path(args.out), subprocess_launch(command_for, args.env))
    return analyze(
        argparse.Namespace(analyze=args.out, baseline_build=order[0], min_samples=args.min_samples)
    )


def analyze(args: argparse.Namespace) -> int:
    """The launch-median table, then the one estimator.

    Every statistic printed below the table comes from `benchmarks/ab_blocks`,
    which is also what `ab_floor.py` prints and what the reading rule in the
    ledger refers to. This file contributes the per-launch view and nothing
    else, so `--analyze` and `ab_blocks.py` cannot report two different effects
    for one packet (the 7 September review, finding 4). The minimum sample
    count comes from there too: a packet the other views refuse as too short
    is refused here as well, rather than read at a laxer default.
    """
    path = Path(args.analyze)
    min_samples = getattr(args, "min_samples", None)
    if min_samples is None:
        min_samples = ab_blocks.DEFAULT_MIN_SAMPLES
    launches = ab_blocks.read_launches(path, min_samples=min_samples)
    engines = sorted({engine for launch in launches for engine, _ in launch.samples})
    datasets = sorted({row for launch in launches for _, row in launch.samples})

    print("== launch medians (ms)")
    header = "row/engine".ljust(30) + "".join(
        f"{launch.tag}:{launch.build:<8}" for launch in launches
    )
    print(header)
    for dataset in datasets:
        for engine in engines:
            cells = []
            for launch in launches:
                value = launch.median(engine, dataset)
                cells.append(f"{value:<11.4f}" if value is not None else f"{'-':<11}")
            print(f"{dataset + ' ' + engine:<30}" + "".join(cells))
    print()
    analysis = ab_blocks.analyze(
        path,
        baseline=args.baseline_build,
        min_samples=min_samples,
    )
    print(ab_blocks.render(analysis))
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--analyze", help="read a TSV this tool wrote and report, without measuring"
    )
    parser.add_argument("--baseline-build", default=None)
    parser.add_argument("--build", action="append", default=[], help="TAG=PATH")
    parser.add_argument("--target", help="the .so to overwrite (must be in this checkout)")
    parser.add_argument("--out", help="TSV of every sample")
    parser.add_argument("--order", default="ABBA")
    parser.add_argument("--blocks", type=int, default=3)
    parser.add_argument("--tail", default="A", help="launches appended after the blocks")
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--tier", default="small")
    parser.add_argument("--dataset", action="append", default=None)
    parser.add_argument("--min-samples", type=int, default=ab_blocks.DEFAULT_MIN_SAMPLES)
    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    args.env = dict(os.environ)

    if args.analyze:
        return analyze(args)
    missing = [name for name in ("target", "out") if not getattr(args, name)]
    if missing or len(args.build) < 2:
        parser.error("measuring needs --build TAG=PATH twice, --target and --out")
    return run(args)


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except ab_blocks.AnalysisError as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(2) from None
