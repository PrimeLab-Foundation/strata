"""Fetch the cross-platform CI benchmark reports into the tree.

The weekly benchmark workflow (.github/workflows/benchmark.yml) runs the same
suite on every supported platform/architecture leg and uploads one report per
leg. This tool pulls one completed run's reports into
``docs/benchmarks/ci/bench_results_<os>-<arch>.md`` so the tree carries the
evidence behind the cross-platform goal -- strata #1 on every platform and
architecture -- and `ci_summary` can rank it.

Reports are named by what they *are*, not by which runner produced them: os
and arch come from each report's own environment header (the ``platform`` /
``machine`` lines `harness.describe_environment` writes), so artifacts from
before the per-platform artifact names -- and after any future runner-label
change -- fetch identically.

Placement is all-or-nothing, and now actually is: every report is downloaded
and validated first, the complete replacement is written into a staging
directory beside the destination, and only a successful staging is swapped in
(the previous set moves aside and is removed after the swap succeeds). The
reviewed version deleted the previous reports and ``run_info.json`` *before*
writing the new ones, so an error on the second write left one new report and
no prior evidence at all (docs/performance/ci-review-2026-09-07.md, finding
10). The content is byte-identical up to newline normalization to LF (the
Windows leg writes CRLF).

What is verified before anything is placed: each report names a platform this
tool can identify, no two legs claim the same platform, each report actually
contains measurements, each report's own commit matches the run's head SHA,
and together they cover the declared platforms and the declared workload
(`harness.WORKLOADS` / `harness.CI_PLATFORMS`). Identity defects are refused
outright. An incomplete run -- a leg that did not upload, a dataset that did
not run, ERROR rows -- is refused too, unless ``--allow-incomplete`` says to
fetch it anyway, which records the problems in ``run_info.json`` so the
summary discloses them.

A fetch that is *killed* mid-swap cannot restore anything itself, so the next
one does: on start-up the leftover ``.<dest>.previous-*`` / ``.<dest>.staging-*``
siblings are looked for, an unambiguously interrupted swap (the destination
gone, one previous set beside it) is put back, and anything else is named on
stderr and refused rather than left for nobody to find
(build/evidence/T2-REVIEW/REVIEW.md, defect 3).

Requires the authenticated GitHub CLI (``gh``).

Exit codes:

* 0 -- fetched (or placed deliberately with ``--allow-incomplete``).
* 1 -- gh failure (missing, unauthenticated, no completed run, no artifacts),
  or evidence refused as short of the declared workload.
* 2 -- report error (unrecognized platform, duplicate leg, empty report,
  commit mismatch), or debris from an interrupted fetch that this tool must
  not resolve on its own.
* 3 -- the reports could not be placed: a filesystem error during the staged
  install. Nothing was replaced; if the previous set could not be put back,
  the directory holding it is named on stderr.
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Sequence
from pathlib import Path

from benchmarks.harness import (
    CI_PLATFORMS,
    WORKLOADS,
    read_report,
    resolve_workload,
    validate_report,
)

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_DEST = PROJECT_ROOT / "docs" / "benchmarks" / "ci"
DEFAULT_WORKFLOW = "benchmark.yml"
ARTIFACT_PATTERN = "benchmark-*"
RUN_INFO_NAME = "run_info.json"

RUN_FIELDS = "databaseId,workflowName,url,event,status,conclusion,headBranch,headSha,createdAt"

DEFAULT_WORKLOAD = "ci"

EXIT_CODES = (
    "Exit codes: 0 fetched; 1 gh failure or evidence short of the declared "
    "workload; 2 report error (unrecognized platform, duplicate leg, empty "
    "report, commit mismatch) or unresolvable debris from an interrupted "
    "fetch; 3 the reports could not be placed (filesystem error) -- nothing "
    "was replaced."
)

# platform.platform() leads with the OS name; platform.machine() spells the
# architecture differently per OS (AMD64 on Windows, aarch64 on arm64 Linux).
_OS_NAMES = {"linux": "linux", "macos": "macos", "windows": "windows"}
_ARCH_NAMES = {"x86_64": "x86_64", "amd64": "x86_64", "arm64": "arm64", "aarch64": "arm64"}


def platform_key(environment: dict[str, str]) -> str:
    """``<os>-<arch>`` from a report's environment header, e.g. ``macos-arm64``."""
    raw_os = environment.get("platform", "").split("-", 1)[0].lower()
    raw_arch = environment.get("machine", "").lower()
    os_name = _OS_NAMES.get(raw_os)
    arch = _ARCH_NAMES.get(raw_arch)
    if os_name is None or arch is None:
        raise ValueError(
            f"unrecognized os/arch: platform={environment.get('platform')!r} "
            f"machine={environment.get('machine')!r}"
        )
    return f"{os_name}-{arch}"


def _run_gh(args: list[str]) -> subprocess.CompletedProcess[str]:
    """The one place ``gh`` is invoked; tests substitute this boundary."""
    return subprocess.run(["gh", *args], capture_output=True, text=True, check=False)


def _gh_json(args: list[str]):
    completed = _run_gh(args)
    if completed.returncode != 0:
        raise RuntimeError(completed.stderr.strip() or f"gh {' '.join(args)} failed")
    return json.loads(completed.stdout)


def _resolve_run(workflow: str, run_id: int | None) -> dict:
    """The requested run, or the newest completed one of the workflow."""
    if run_id is not None:
        return _gh_json(["run", "view", str(run_id), "--json", RUN_FIELDS])
    runs = _gh_json(["run", "list", "--workflow", workflow, "--limit", "20", "--json", RUN_FIELDS])
    for run in runs:
        if run.get("status") == "completed":
            return run
    raise RuntimeError(f"no completed run of {workflow} among the last {len(runs)} runs")


def _download(run_id: int, into: Path) -> None:
    completed = _run_gh(
        ["run", "download", str(run_id), "--dir", str(into), "--pattern", ARTIFACT_PATTERN]
    )
    if completed.returncode != 0:
        raise RuntimeError(
            completed.stderr.strip() or f"gh run download {run_id} failed with no artifacts"
        )


def _collect_reports(download_dir: Path) -> dict[str, Path]:
    """Map platform-arch -> downloaded report, refusing colliding legs."""
    reports: dict[str, Path] = {}
    for path in sorted(download_dir.rglob("*.md")):
        report = read_report(path)
        key = platform_key(report.environment)
        if key in reports:
            raise ValueError(f"two artifacts claim {key}: {reports[key].name} and {path.name}")
        reports[key] = path
    return reports


def _verify(
    found: dict[str, Path],
    run: dict,
    *,
    expected_rows: tuple[tuple[str, str], ...] | None,
    expected_platforms: tuple[str, ...],
) -> tuple[list[str], list[str]]:
    """Check identity and coverage of a downloaded set before it is placed.

    Returns (identity defects, coverage defects). An identity defect means the
    files are not what they claim to be and can never be placed; a coverage
    defect means the run is short of the declared evidence, which
    ``--allow-incomplete`` may accept.
    """
    identity: list[str] = []
    coverage: list[str] = []
    head = str(run.get("headSha") or "").strip()

    for key in sorted(found):
        report = read_report(found[key])
        if not report.measurements:
            identity.append(f"{key}: {found[key].name} contains no measurements")
            continue
        commit = (report.environment.get("commit") or "").strip()
        if report.provenance:
            build = report.provenance.get("extension", {}).get("build")
            source = build.get("source") if build else None
            if not source or not source.get("commit") or source.get("dirty") is not False:
                identity.append(f"{key}: measured binary has dirty or unknown build source")
        if not commit or commit == "unknown":
            identity.append(f"{key}: the report records no commit, so it cannot be attributed")
        elif head and not (head.startswith(commit) or commit.startswith(head)):
            identity.append(
                f"{key}: report commit {commit} is not run {run.get('databaseId')}'s {head[:12]}"
            )
        validation = validate_report(report, expected=expected_rows)
        coverage.extend(f"{key}: {problem}" for problem in validation.problems if problem.fatal)

    for key in expected_platforms:
        if key not in found:
            coverage.append(f"{key}: the declared platform uploaded no report")
    return identity, coverage


def _orphans(dest: Path) -> tuple[list[Path], list[Path]]:
    """The (previous, staging) directories an interrupted `_install` left."""
    if not dest.parent.is_dir():
        return [], []
    return (
        sorted(dest.parent.glob(f".{dest.name}.previous-*")),
        sorted(dest.parent.glob(f".{dest.name}.staging-*")),
    )


def recover_interrupted_install(dest: Path) -> tuple[bool, list[str]]:
    """Deal with the debris of a fetch that was killed mid-swap.

    `_install` moves the previous set to ``.<dest>.previous-<rand>`` and only
    then renames staging into place. Its exception paths restore; a signal
    between the two steps cannot, and the reviewed tool then neither adopted,
    reported nor removed the orphan -- the previous evidence sat in a directory
    nothing named (build/evidence/T2-REVIEW/REVIEW.md, defect 3).

    One case is unambiguous and is repaired: the destination is gone and
    exactly one ``.previous-*`` holds it. Anything else -- two previous sets,
    or one beside a destination that already exists -- is a state this tool
    must not resolve by guessing, so it names every orphan and refuses. A
    leftover ``.staging-*`` is only ever half-written *new* evidence, never the
    last copy of anything: it is reported and left alone (removing it could
    pull the floor out from under a concurrent fetch).

    Returns (ok, messages); ok=False means the caller must refuse.
    """
    previous, staging = _orphans(dest)
    messages = [f"an unfinished staging directory is beside {dest}: {p.name}" for p in staging]
    if not previous:
        return True, messages
    if len(previous) == 1 and not dest.exists():
        os.replace(previous[0], dest)
        return True, [
            f"an earlier fetch was interrupted mid-swap; recovered the previous "
            f"reports from {previous[0].name} into {dest}",
            *messages,
        ]
    named = ", ".join(path.name for path in (*previous, *staging))
    return False, [
        f"an earlier fetch into {dest} did not finish and left: {named}",
        "the previous reports are in there; move or remove them by hand. Nothing was fetched.",
    ]


def _write_text(path: Path, text: str) -> None:
    """The one place a replacement file is written; tests substitute this seam."""
    path.write_text(text, encoding="utf-8", newline="\n")


def _install(dest: Path, files: dict[str, str]) -> None:
    """Swap a complete, already-rendered file set into `dest`, or change nothing.

    Every write lands in a staging directory beside the destination first, so a
    failure while writing leaves the previous fetch untouched. The swap itself
    moves the old directory aside and only deletes it once the new one is in
    place; a failure there puts the old one back.

    A *signal* between the move-aside and the rename runs none of that, which
    is why `recover_interrupted_install` looks for the debris on the next
    start-up: the two halves are one contract.
    """
    dest.parent.mkdir(parents=True, exist_ok=True)
    staging = Path(tempfile.mkdtemp(prefix=f".{dest.name}.staging-", dir=dest.parent))
    moved_aside: Path | None = None
    try:
        for name, text in sorted(files.items()):
            _write_text(staging / name, text)
        if dest.exists():
            # Files the fetch does not own stay: deleting them is not its call.
            # A stale `bench_results_*.md` of a platform this run did not
            # produce does go, though -- the directory is one run's evidence.
            for path in sorted(dest.iterdir()):
                owned = path.name.startswith("bench_results_") or path.name == RUN_INFO_NAME
                if path.is_file() and not owned:
                    shutil.copy2(path, staging / path.name)
            moved_aside = Path(tempfile.mkdtemp(prefix=f".{dest.name}.previous-", dir=dest.parent))
            moved_aside.rmdir()
            os.replace(dest, moved_aside)
        os.replace(staging, dest)
    except BaseException:
        shutil.rmtree(staging, ignore_errors=True)
        if moved_aside is not None and not dest.exists():
            try:
                os.replace(moved_aside, dest)
                moved_aside = None
            except OSError:
                # Never silently: the only copy of the previous evidence is
                # named here rather than removed by the cleanup below.
                sys.stderr.write(
                    f"error: could not restore the previous reports; they are in {moved_aside}\n"
                )
        raise
    finally:
        if moved_aside is not None and moved_aside.exists() and dest.exists():
            shutil.rmtree(moved_aside, ignore_errors=True)


def _place(
    found: dict[str, Path],
    run: dict,
    dest: Path,
    scratch: Path,
    *,
    problems: Sequence[str] = (),
    expected_platforms: tuple[str, ...] = CI_PLATFORMS,
) -> None:
    """Render the whole replacement, then install it: one run's evidence, never a mix."""
    files: dict[str, str] = {}
    sources: dict[str, str] = {}
    sidecars: dict[str, str] = {}
    for key in sorted(found):
        source = found[key]
        text = source.read_text(encoding="utf-8")
        files[f"bench_results_{key}.md"] = "\n".join(text.splitlines()) + "\n"
        sources[key] = source.relative_to(scratch).as_posix()
        from benchmarks.provenance import companion, validate_companion

        if validate_companion(source, text) is not None:
            sidecar = companion(source)
            files[f"bench_results_{key}.json"] = sidecar.read_text(encoding="utf-8")
            sidecars[key] = sidecar.relative_to(scratch).as_posix()

    info = {
        "workflow": run.get("workflowName"),
        "run_id": run.get("databaseId"),
        "url": run.get("url"),
        "event": run.get("event"),
        "conclusion": run.get("conclusion"),
        "head_branch": run.get("headBranch"),
        "head_sha": run.get("headSha"),
        "created_at": run.get("createdAt"),
        "reports": sources,
        "sidecars": sidecars,
        "expected_platforms": list(expected_platforms),
        "complete": not problems,
    }
    if problems:
        info["problems"] = list(problems)
    files[RUN_INFO_NAME] = json.dumps(info, indent=2) + "\n"

    _install(dest, files)
    for key in sorted(sources):
        print(f"fetched {key} <- {sources[key]}")
    print(f"wrote {len(sources)} report(s) + {RUN_INFO_NAME} -> {dest}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, epilog=EXIT_CODES)
    parser.add_argument(
        "--run", type=int, default=None, help="run id; default: the latest completed run"
    )
    parser.add_argument("--workflow", default=DEFAULT_WORKFLOW)
    parser.add_argument("--dest", type=Path, default=DEFAULT_DEST)
    parser.add_argument(
        "--expect",
        default=DEFAULT_WORKLOAD,
        choices=sorted(WORKLOADS),
        help="the declared workload each fetched report must contain in full",
    )
    parser.add_argument(
        "--expect-platforms",
        default=",".join(CI_PLATFORMS),
        help="comma-separated platform-arch keys the run is expected to cover",
    )
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="place a run that is short of the declared evidence; the gaps are "
        "recorded in run_info.json and the summary reports them",
    )
    args = parser.parse_args(argv)

    expected_platforms = tuple(
        key.strip() for key in args.expect_platforms.split(",") if key.strip()
    )

    # Before anything is downloaded: a fetch that was killed mid-swap left the
    # previous evidence in a directory nothing names.
    try:
        recovered, notes = recover_interrupted_install(args.dest)
    except OSError as error:
        sys.stderr.write(f"error: could not recover the previous fetch: {error}\n")
        return 3
    for note in notes:
        sys.stderr.write(f"{'error' if not recovered else 'warning'}: {note}\n")
    if not recovered:
        return 2

    try:
        run = _resolve_run(args.workflow, args.run)
        run_id = run.get("databaseId")
        if run_id is None:
            raise RuntimeError("gh returned a run without a databaseId")

        if run.get("conclusion") != "success":
            sys.stderr.write(
                f"warning: run {run_id} concluded {run.get('conclusion')!r} -- a leg failed "
                "or the tripwire fired; fetching what it uploaded, the summary will say so.\n"
            )

        with tempfile.TemporaryDirectory() as scratch:
            scratch_path = Path(scratch)
            _download(run_id, scratch_path)
            try:
                found = _collect_reports(scratch_path)
            except ValueError as error:
                sys.stderr.write(f"error: {error}\n")
                return 2
            if not found:
                sys.stderr.write(f"error: run {run_id} uploaded no benchmark reports\n")
                return 1

            identity, coverage = _verify(
                found,
                run,
                expected_rows=resolve_workload(args.expect),
                expected_platforms=expected_platforms,
            )
            if identity:
                sys.stderr.write(
                    "error: the downloaded reports are not this run's evidence; "
                    f"nothing was placed in {args.dest}\n"
                )
                for defect in identity:
                    sys.stderr.write(f"  {defect}\n")
                return 2
            if coverage and not args.allow_incomplete:
                sys.stderr.write(
                    f"error: run {run_id} is short of the declared evidence; "
                    f"nothing was placed in {args.dest}. Pass --allow-incomplete to "
                    "fetch it anyway (the gaps are recorded and reported).\n"
                )
                for defect in coverage:
                    sys.stderr.write(f"  {defect}\n")
                return 1

            try:
                _place(
                    found,
                    run,
                    args.dest,
                    scratch_path,
                    problems=coverage,
                    expected_platforms=expected_platforms,
                )
            except (OSError, ValueError) as error:
                # Documented as exit 3: the evidence was fine, the filesystem
                # was not. `_install` has already restored the previous set or
                # said where it is; a traceback here would be neither.
                sys.stderr.write(
                    f"error: the reports could not be placed in {args.dest}: {error}\n"
                    "Nothing was replaced.\n"
                )
                return 3
            if coverage:
                sys.stderr.write(f"warning: placed incomplete evidence for run {run_id}:\n")
                for defect in coverage:
                    sys.stderr.write(f"  {defect}\n")
    except FileNotFoundError:
        sys.stderr.write("error: the GitHub CLI (gh) is required and was not found on PATH\n")
        return 1
    except (RuntimeError, json.JSONDecodeError) as error:
        sys.stderr.write(f"error: {error}\n")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
