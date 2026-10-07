#!/usr/bin/env python3
"""Post-release verification glue: the PyPI wheel, installed and proven, before it is measured.

``.github/workflows/post-release.yml`` runs these on every release leg after a
PyPI publish (docs/architecture/release_pipeline.md, "Post-release
verification"). They live beside ``scripts/release.py`` rather than in it
because that file is already past the ~800-line rule and its tests patch its
module globals, so moving code out of it would silently unpatch them.

- ``install VERSION``: ``pip install --only-binary=strata-plf strata-plf==VERSION``
  from PyPI, retried with backoff while the new version propagates through the
  index. VERSION must be a final ``YYYY.M.D[.N]`` (release.py's grammar; an
  ``rcK`` never reaches PyPI). pip's cache is bypassed so that a retry cannot
  read a cached index page from before the upload.
- ``check-installed VERSION``: prints ``strata.__version__`` and where
  ``strata``, ``strata._strata`` and ``strata._dumps_hook`` import from, then
  fails unless the version is VERSION and every file resolves into
  site-packages, outside this checkout. Run it with the benchmark's own
  ``PYTHONPATH`` and working directory, so that a checkout tree shadowing the
  install would be caught here rather than measured.
- ``bench-requirements``: the ``bench`` extra as ``pyproject.toml`` declares it,
  one requirement per line: the competitor set ``make install-bench`` installs,
  without installing strata from the checkout.
- ``standings-check REPORT... [--json PATH]``: the post-release standings gate,
  a within-run rank gate over each leg's canonical report (thresholds and their
  sources below, at ``MAX_BEHIND_ROWS`` and ``MAX_BEHIND_RATIO``). Prints a
  per-leg table and writes a machine-readable JSON summary.
- ``standings-combine SUMMARY... [--output PATH]``: the five legs' JSON
  summaries as one table; fails unless every declared leg is present and passed.

The two standings commands read the checkout's ``benchmarks`` package, so they
run as ``PYTHONPATH=. python -m scripts.release_post``, like ``check-installed``.
"""

from __future__ import annotations

import argparse
import importlib
import json
import runpy
import subprocess
import sys
import sysconfig
import time
from pathlib import Path
from typing import NoReturn

PROJECT_ROOT = Path(__file__).resolve().parents[1]
RELEASE_SCRIPT = PROJECT_ROOT / "scripts" / "release.py"
INDEX_URL = "https://pypi.org/simple/"
MODULES = ("strata", "strata._strata", "strata._dumps_hook")
# 15 + 30 + 60 + 120 * 4 s: about ten minutes of propagation before giving up.
ATTEMPTS = 8
FIRST_DELAY = 15
MAX_DELAY = 120

# The post-release standings gate. Every number it reads is a within-run
# ratio: strata's median over the best rival's median in one row of one leg's
# report, all measured in the same job. It never compares a time with another
# run's, a baseline or another platform (docs/context/benchmarks.md: ranks are
# within-run comparisons only; the 2% gate is same-machine evidence only).
#
# (a) The per-leg coin band: at most 3 of the 27 declared rows behind. Source:
#     the 28 distinct five-leg CI samples archived in September 2026
#     (docs/decisions.md, 2026-10-07) read 0-3 rows behind on 137 of 140
#     leg-draws, windows-x86_64 reading 3 on four draws of shipped source. The
#     three above it were linux-x86_64: 4 behind on 79fa3df (run 34064174240,
#     the x86 serializer regression E26-P6 later found in the code) and 5 and 6
#     behind on c16eaa6 (runs 35305165291 and 35301268133, the source of the
#     135/135 sweep, on an AMD EPYC 9V45 host): a known-good build's false alarms.
MAX_BEHIND_ROWS = 3
# (b) The ratio bound: no row past 1.25x its best rival. Derivation: the worst
#     behind ratio on any leg-draw inside the coin band in those samples,
#     1.169x (windows-x86_64 `dump mixed`, b32d398), times the release ISA's
#     largest resolved cost against -march=native, +6.79% (linux-x86_64
#     `search users $[*].id`, docs/performance/experiment-ledger.md R1):
#     1.169 * 1.0679 = 1.248, rounded to 1.25. The supportability tripwire's
#     3.0x (benchmarks/supportability_check.py) stays the hard backstop.
MAX_BEHIND_RATIO = 1.25
STANDINGS_WORKLOAD = "ci"
STANDINGS_SCHEMA = "post-release-standings/1"
STANDINGS_GATE = (
    "A within-run rank gate. Per leg, strata is ranked in each of the 27 declared rows of "
    "that leg's own canonical report (its full-precision JSON companion when present) "
    "against the rivals measured in the same run. It never compares a time with another "
    "run's, a baseline or another platform. A leg fails when more than "
    f"{MAX_BEHIND_ROWS} rows are behind (the per-leg coin band of the archived CI samples) "
    f"or any row is past {MAX_BEHIND_RATIO:.2f}x its best rival (the worst in-band behind "
    "ratio of those samples, 1.169x, times R1's release-ISA cost, 1.0679). The "
    "supportability tripwire's 3.0x stays the hard backstop."
)
STANDINGS_EXIT_CODES = (
    "Exit codes: 0 every leg passes; 1 a leg is behind past the gate; 2 a report is not "
    "gateable evidence (missing file, unreadable or ERROR rows, unusable numbers, short of "
    "the declared workload, a row with no rival, or a leg that cannot be told or is "
    "reported twice)."
)


def _fail(message: str) -> NoReturn:
    raise SystemExit(f"release_post: {message}")


def _release() -> dict:
    return runpy.run_path(str(RELEASE_SCRIPT))


def require_final(version: str) -> None:
    """Exit unless VERSION is a final release in release.py's grammar."""
    if _release()["version_key"](version)[4] == 0:
        _fail(f"{version} is a release candidate; PyPI holds final versions only")


def install(version: str) -> int:
    require_final(version)
    project = _release()["PROJECT_NAME"]
    cmd = [
        sys.executable,
        "-m",
        "pip",
        "install",
        "--no-cache-dir",
        "--index-url",
        INDEX_URL,
        f"--only-binary={project}",
        f"{project}=={version}",
    ]
    delay = FIRST_DELAY
    for attempt in range(1, ATTEMPTS + 1):
        print(f"+ {' '.join(cmd)}  (attempt {attempt}/{ATTEMPTS})", flush=True)
        if subprocess.run(cmd, check=False).returncode == 0:
            return 0
        if attempt < ATTEMPTS:
            print(f"release_post: not installable yet; retrying in {delay} s", flush=True)
            time.sleep(delay)
            delay = min(delay * 2, MAX_DELAY)
    _fail(f"{project}=={version} did not install from {INDEX_URL} in {ATTEMPTS} attempts")


def check_installed(version: str) -> int:
    paths = sysconfig.get_paths()
    site = sorted({Path(paths[key]).resolve() for key in ("purelib", "platlib") if key in paths})
    strata = importlib.import_module("strata")
    print(f"+ strata.__version__ = {strata.__version__!r}", flush=True)
    # The package first: a shadowing tree is named here, before a submodule it
    # lacks can fail to import.
    for name in MODULES:
        path = Path(importlib.import_module(name).__file__).resolve()
        print(f"+ {name}.__file__ = {path}", flush=True)
        if path.is_relative_to(PROJECT_ROOT) or not any(path.is_relative_to(s) for s in site):
            _fail(
                f"{name} imports from {path}, not from the installed wheel in "
                f"{', '.join(map(str, site))} (the checkout {PROJECT_ROOT} must not shadow it)",
            )
    if strata.__version__ != version:
        _fail(f"strata.__version__ is {strata.__version__!r}, not the released {version!r}")
    print(f"release_post: strata {version} is the installed wheel", flush=True)
    return 0


def bench_requirements(pyproject: Path | None = None) -> list[str]:
    """The ``bench`` extra as ``pyproject.toml`` declares it (tomli below Python 3.11)."""
    try:
        import tomllib
    except ModuleNotFoundError:
        try:
            import tomli as tomllib
        except ModuleNotFoundError:
            _fail(f"reading pyproject.toml needs tomllib (Python 3.11+) or tomli: {sys.executable}")
    with (pyproject or PROJECT_ROOT / "pyproject.toml").open("rb") as fh:
        project = tomllib.load(fh)["project"]
    requirements = project.get("optional-dependencies", {}).get("bench")
    if not requirements:
        _fail("pyproject.toml declares no 'bench' extra")
    return requirements


def _leg_of(path: Path, environment: dict[str, str]) -> tuple[str, str | None]:
    """The leg a report measured, and the problem if it cannot be told or contradicts its name."""
    from benchmarks.ci_fetch import platform_key
    from benchmarks.harness import CI_PLATFORMS

    named = next((leg for leg in CI_PLATFORMS if path.stem.endswith(leg)), None)
    try:
        measured = platform_key(environment)
    except ValueError as error:
        return named or path.name, f"cannot tell the leg: {error}"
    if named is not None and named != measured:
        return measured, f"the report is named for {named} but was measured on {measured}"
    return measured, None


def _leg_record(leg: str, report: str, rows: int, ranked: list, problems: list[str]) -> dict:
    behind = sorted((row for row in ranked if row.rank > 1), key=lambda row: -row.ratio)
    failures = []
    if len(behind) > MAX_BEHIND_ROWS:
        failures.append(
            f"{len(behind)} rows behind, past the per-leg coin band of {MAX_BEHIND_ROWS}",
        )
    failures += [
        f"{row.section} {row.dataset}: {row.ratio:.3f}x {row.best_rival}, "
        f"past the {MAX_BEHIND_RATIO:.2f}x bound"
        for row in behind
        if row.ratio > MAX_BEHIND_RATIO
    ]
    return {
        "leg": leg,
        "report": report,
        "verdict": "INVALID" if problems else "FAIL" if failures else "pass",
        "rows": rows,
        "won": len(ranked) - len(behind),
        "behind": [
            {
                "section": row.section,
                "dataset": row.dataset,
                "ratio": row.ratio,
                "best_rival": row.best_rival,
                "tied": row.tied,
            }
            for row in behind
        ],
        "failures": failures,
        "problems": problems,
    }


def standings_leg(path: Path) -> dict:
    """One leg's standings in its own report, and the gate's verdict: pass, FAIL or INVALID."""
    from benchmarks.ci_summary import standings
    from benchmarks.harness import read_report, resolve_workload, validate_report

    declared = resolve_workload(STANDINGS_WORKLOAD)
    if not path.is_file():
        return _leg_record(
            _leg_of(path, {})[0],
            path.name,
            len(declared),
            [],
            [
                f"no such report: {path}",
            ],
        )
    report = read_report(path)
    leg, problem = _leg_of(path, report.environment)
    problems = [problem] if problem else []
    problems += [
        str(item) for item in validate_report(report, expected=declared).problems if item.fatal
    ]
    rows = set(declared)
    ranked = [row for row in standings(report) if (row.section, row.dataset) in rows]
    if not problems:
        problems = [
            f"{section}|{dataset}: no rival measured to rank strata against"
            for section, dataset in sorted(rows - {(row.section, row.dataset) for row in ranked})
        ]
    return _leg_record(leg, path.name, len(declared), ranked, problems)


def _flag_duplicates(legs: list[dict]) -> None:
    names = [leg["leg"] for leg in legs]
    for leg in legs:
        if names.count(leg["leg"]) > 1:
            leg["problems"].append(f"{leg['leg']} is reported more than once")
            leg["verdict"] = "INVALID"


def render_standings(legs: list[dict]) -> str:
    """The per-leg table: leg, rows won, each behind row with its ratio, verdict."""
    lines = [
        "| leg | rows won | behind (strata / best rival) | verdict |",
        "|---|---|---|---|",
    ]
    for leg in legs:
        behind = ", ".join(
            f"{row['section']} {row['dataset']} {row['ratio']:.3f}x ({row['best_rival']})"
            for row in leg["behind"]
        )
        lines.append(
            f"| {leg['leg']} | {leg['won']}/{leg['rows']} | {behind or '-'} | {leg['verdict']} |"
        )
    lines += [
        "",
        f"Gate: a leg fails past {MAX_BEHIND_ROWS} rows behind or any row past "
        f"{MAX_BEHIND_RATIO:.2f}x its best rival. Within-run ranks only: no time is "
        "compared with another run's.",
    ]
    notes = [
        f"- {leg['leg']}: {text}" for leg in legs for text in leg["problems"] + leg["failures"]
    ]
    return "\n".join(lines + ([""] + notes if notes else [])) + "\n"


def standings_check(paths: list[Path], summary: Path | None) -> int:
    legs = [standings_leg(path) for path in paths]
    _flag_duplicates(legs)
    sys.stdout.write(render_standings(legs))
    if summary is not None:
        gate = {
            "max_behind_rows": MAX_BEHIND_ROWS,
            "max_behind_ratio": MAX_BEHIND_RATIO,
            "workload": STANDINGS_WORKLOAD,
        }
        document = {"schema": STANDINGS_SCHEMA, "gate": gate, "legs": legs}
        summary.write_text(json.dumps(document, indent=2) + "\n", encoding="utf-8")
    verdicts = {leg["verdict"] for leg in legs}
    return 2 if "INVALID" in verdicts else 1 if "FAIL" in verdicts else 0


def standings_combine(paths: list[Path], output: Path | None) -> int:
    """Every declared leg's summary as one table; 1 unless all five are present and passed."""
    from benchmarks.harness import CI_PLATFORMS, resolve_workload

    legs: list[dict] = []
    unreadable: list[str] = []
    for path in paths:
        try:
            document = json.loads(path.read_text(encoding="utf-8"))
            if document.get("schema") != STANDINGS_SCHEMA:
                raise ValueError(f"schema is not {STANDINGS_SCHEMA}")
            legs += document["legs"]
        except (OSError, ValueError, KeyError, AttributeError, TypeError) as error:
            unreadable.append(f"{path}: unreadable standings summary ({error})")
    _flag_duplicates(legs)
    rows = len(resolve_workload(STANDINGS_WORKLOAD))
    present = {leg["leg"] for leg in legs}
    missing = [
        {**_leg_record(name, "", rows, [], ["no standings summary"]), "verdict": "MISSING"}
        for name in CI_PLATFORMS
        if name not in present
    ]
    order = {name: index for index, name in enumerate(CI_PLATFORMS)}
    combined = sorted(legs + missing, key=lambda leg: order.get(leg["leg"], len(order)))
    text = render_standings(combined) + "".join(f"- {note}\n" for note in unreadable)
    sys.stdout.write(text)
    if output is not None:
        output.write_text(f"# Post-release standings\n\n{text}", encoding="utf-8")
    passed = all(leg["verdict"] == "pass" for leg in combined)
    return 0 if passed and not unreadable else 1


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    commands = parser.add_subparsers(dest="command", required=True)
    for name, text in (
        ("install", "install strata-plf==VERSION from PyPI, retried while the index propagates"),
        ("check-installed", "prove the importable strata is the installed VERSION"),
    ):
        commands.add_parser(name, help=text).add_argument("version", help="final YYYY.M.D[.N]")
    commands.add_parser("bench-requirements", help="print the bench extra, one per line")
    check = commands.add_parser(
        "standings-check",
        help="the within-run rank gate on each leg's canonical report",
        description=STANDINGS_GATE,
        epilog=STANDINGS_EXIT_CODES,
    )
    check.add_argument("reports", nargs="+", type=Path, help="reports written by bench_main")
    check.add_argument("--json", type=Path, help="write the machine-readable summary here")
    combine = commands.add_parser(
        "standings-combine",
        help="every leg's standings summary as one table",
        description=STANDINGS_GATE,
        epilog="Exit codes: 0 every declared leg is present and passed; 1 otherwise.",
    )
    combine.add_argument("summaries", nargs="*", type=Path, help="standings-check --json files")
    combine.add_argument("--output", type=Path, help="write the combined table here")
    args = parser.parse_args(argv)
    if args.command == "install":
        return install(args.version)
    if args.command == "check-installed":
        return check_installed(args.version)
    if args.command == "standings-check":
        return standings_check(args.reports, args.json)
    if args.command == "standings-combine":
        return standings_combine(args.summaries, args.output)
    print("\n".join(bench_requirements()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
