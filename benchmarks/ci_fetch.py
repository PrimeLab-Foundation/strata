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

Placement is all-or-nothing: every report is downloaded and validated in a
temporary directory first, and only then replaces the previous fetch, together
with ``run_info.json`` recording which run the files came from. The content is
byte-identical up to newline normalization to LF (the Windows leg writes CRLF).

Requires the authenticated GitHub CLI (``gh``).

Exit codes: 0 fetched, 1 gh failure (missing, unauthenticated, no completed
run, no artifacts), 2 report error (unrecognized platform, duplicate leg).
"""

from __future__ import annotations

import argparse
import json
import subprocess
import sys
import tempfile
from pathlib import Path

from benchmarks.harness import parse_report

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_DEST = PROJECT_ROOT / "docs" / "benchmarks" / "ci"
DEFAULT_WORKFLOW = "benchmark.yml"
ARTIFACT_PATTERN = "benchmark-*"
RUN_INFO_NAME = "run_info.json"

RUN_FIELDS = "databaseId,workflowName,url,event,status,conclusion,headBranch,headSha,createdAt"

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
        report = parse_report(path.read_text(encoding="utf-8"), name=path.name)
        key = platform_key(report.environment)
        if key in reports:
            raise ValueError(f"two artifacts claim {key}: {reports[key].name} and {path.name}")
        reports[key] = path
    return reports


def _place(found: dict[str, Path], run: dict, dest: Path, scratch: Path) -> None:
    """Replace the previous fetch: the directory is one run's evidence, never a mix."""
    dest.mkdir(parents=True, exist_ok=True)
    for stale in dest.glob("bench_results_*.md"):
        stale.unlink()
    info_path = dest / RUN_INFO_NAME
    info_path.unlink(missing_ok=True)

    sources: dict[str, str] = {}
    for key in sorted(found):
        source = found[key]
        target = dest / f"bench_results_{key}.md"
        text = source.read_text(encoding="utf-8")
        target.write_text("\n".join(text.splitlines()) + "\n", encoding="utf-8", newline="\n")
        sources[key] = source.relative_to(scratch).as_posix()
        print(f"fetched {key} <- {sources[key]}")

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
    }
    info_path.write_text(json.dumps(info, indent=2) + "\n", encoding="utf-8", newline="\n")
    print(f"wrote {len(sources)} report(s) + {RUN_INFO_NAME} -> {dest}")


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--run", type=int, default=None, help="run id; default: the latest completed run"
    )
    parser.add_argument("--workflow", default=DEFAULT_WORKFLOW)
    parser.add_argument("--dest", type=Path, default=DEFAULT_DEST)
    args = parser.parse_args(argv)

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
            _place(found, run, args.dest, scratch_path)
    except FileNotFoundError:
        sys.stderr.write("error: the GitHub CLI (gh) is required and was not found on PATH\n")
        return 1
    except (RuntimeError, json.JSONDecodeError) as error:
        sys.stderr.write(f"error: {error}\n")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
