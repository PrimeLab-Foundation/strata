"""Compare one benchmark row across several archived CI samples.

A rank inside one sample is a within-run comparison and nothing else
(`docs/context/benchmarks.md`). So when two samples of one revision disagree,
or two revisions are read across different draws, the only way to separate a
code effect from a host effect is to look at every library's own absolute
time in each draw: a rival is an unchanged binary, so a rival that moves
between draws is measuring the draw.

This tool writes that comparison instead of leaving it to prose. Per platform
and row it prints each sample's per-library median from the validated report
(full precision whenever a provenance companion is present), the ratio of
strata to the best rival, the host processor, and a verdict on whether the
draws may be compared at all: `agree` when every rival shared by the samples
moves by no more than `--rival-tolerance`, `rival-moved` otherwise. A row
whose draws do not agree carries no standing.

usage:
  python -m benchmarks.cross_sample --sample LABEL=<ci dir> [--sample ...]
      [--platform linux-x86_64] [--row dumps:mixed.json]
      [--rival-tolerance 0.10] [--output docs/benchmarks/cross_sample.md]

Each `<ci dir>` holds `bench_results_<platform>.md` beside its JSON
companion -- what `benchmarks/ci_fetch.py` places in `docs/benchmarks/ci/`
and what `build/evidence/**/ci-<run>/ci/` archives.
"""

from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass, field
from pathlib import Path

from benchmarks.harness import CI_PLATFORMS, Report, read_report

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = PROJECT_ROOT / "docs" / "benchmarks" / "cross_sample.md"
DEFAULT_TOLERANCE = 0.10
STRATA = "strata"


@dataclass(frozen=True)
class Cell:
    """One sample's reading of one row on one platform."""

    label: str
    host: str
    strata_ms: float | None = None
    rivals: dict[str, float] = field(default_factory=dict)
    missing: str = ""

    @property
    def best_rival(self) -> tuple[str, float] | None:
        live = {name: ms for name, ms in self.rivals.items() if ms}
        if not live:
            return None
        name = min(live, key=lambda key: live[key])
        return name, live[name]

    @property
    def ratio(self) -> float | None:
        best = self.best_rival
        if best is None or not self.strata_ms:
            return None
        return self.strata_ms / best[1]


def host_of(report: Report) -> str:
    """The environment line that identifies the machine, or "unknown"."""
    for key in ("processor", "cpu", "machine", "platform"):
        value = report.environment.get(key)
        if value:
            return value.strip()
    return "unknown"


def read_cell(label: str, directory: Path, platform: str, section: str, dataset: str) -> Cell:
    path = directory / f"bench_results_{platform}.md"
    if not path.exists():
        return Cell(label=label, host="unknown", missing=f"no report at {path}")
    report = read_report(path)
    host = host_of(report)
    strata_ms: float | None = None
    rivals: dict[str, float] = {}
    for measurement in report.measurements:
        if measurement.section != section or measurement.dataset != dataset:
            continue
        if measurement.failed or measurement.median_ms is None:
            continue
        if measurement.library == STRATA:
            strata_ms = measurement.median_ms
        else:
            rivals[measurement.library] = measurement.median_ms
    if strata_ms is None and not rivals:
        return Cell(label=label, host=host, missing=f"{section}/{dataset} absent")
    return Cell(label=label, host=host, strata_ms=strata_ms, rivals=rivals)


def rival_drift(cells: list[Cell]) -> dict[str, float]:
    """Per rival, its own spread across the samples: max/min - 1."""
    by_rival: dict[str, list[float]] = {}
    for cell in cells:
        for name, value in cell.rivals.items():
            if value:
                by_rival.setdefault(name, []).append(value)
    return {
        name: max(values) / min(values) - 1.0
        for name, values in by_rival.items()
        if len(values) > 1 and min(values) > 0
    }


def verdict(cells: list[Cell], tolerance: float) -> tuple[str, str]:
    """Whether these samples may be compared, and why."""
    usable = [cell for cell in cells if not cell.missing]
    if len(usable) < 2:
        return "incomparable", "fewer than two samples carry this row"
    drift = rival_drift(usable)
    if not drift:
        return "incomparable", "no rival appears in more than one sample"
    worst = max(drift, key=lambda key: drift[key])
    if drift[worst] > tolerance:
        moved = ", ".join(f"{name} {drift[name] * 100:+.0f}%" for name in sorted(drift))
        return "rival-moved", f"unchanged rivals differ between draws ({moved})"
    held = ", ".join(f"{name} {drift[name] * 100:.1f}%" for name in sorted(drift))
    return "agree", f"every shared rival within tolerance ({held})"


def format_ms(value: float | None) -> str:
    return "-" if value is None else f"{value:.4f}"


def render(
    samples: list[tuple[str, Path]],
    platforms: list[str],
    rows: list[tuple[str, str]],
    tolerance: float,
) -> str:
    lines = [
        "# Cross-sample comparison",
        "",
        "Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.",
        "",
        "Every median below is read from a validated report, at the full "
        "precision of its provenance companion where one exists. `verdict` "
        "says whether the draws may be compared: a rival is an unchanged "
        "binary, so a rival that moves between draws by more than "
        f"{tolerance * 100:.0f}% is measuring the draw, and the rows it "
        "appears in carry no standing.",
        "",
        "Samples, in the order given:",
        "",
    ]
    for label, directory in samples:
        lines.append(f"- `{label}` — {directory}")
    lines.append("")

    behind: list[str] = []
    ahead: list[str] = []
    flipping: list[str] = []
    for platform in platforms:
        for section, dataset in rows:
            cells = [read_cell(label, path, platform, section, dataset) for label, path in samples]
            if all(cell.missing for cell in cells):
                continue
            state, why = verdict(cells, tolerance)
            lines.append(f"## {platform} — {section} {dataset}")
            lines.append("")
            libraries = sorted({name for cell in cells for name in cell.rivals})
            header = ["sample", "host", "strata"] + libraries + ["best rival", "ratio"]
            lines.append("| " + " | ".join(header) + " |")
            lines.append("|" + "---|" * len(header))
            for cell in cells:
                if cell.missing:
                    lines.append(
                        "| "
                        + " | ".join(
                            [cell.label, cell.host, cell.missing] + ["-"] * (len(libraries) + 2)
                        )
                        + " |"
                    )
                    continue
                best = cell.best_rival
                cells_text = (
                    [cell.label, cell.host, format_ms(cell.strata_ms)]
                    + [format_ms(cell.rivals.get(name)) for name in libraries]
                    + [
                        best[0] if best else "-",
                        f"{cell.ratio:.3f}x" if cell.ratio else "-",
                    ]
                )
                lines.append("| " + " | ".join(cells_text) + " |")
            lines.append("")
            lines.append(f"verdict: **{state}** — {why}")
            lines.append("")
            ratios = [cell.ratio for cell in cells if cell.ratio]
            if ratios:
                shown = ", ".join(f"{ratio:.3f}x" for ratio in ratios)
                drift = rival_drift([cell for cell in cells if not cell.missing])
                worst = max(drift.values(), default=0.0)
                aside = f"rivals moved {worst * 100:.0f}%" if state != "agree" else "rivals held"
                entry = f"{platform} {section} {dataset}: {shown} ({aside})"
                if all(ratio > 1.0 for ratio in ratios):
                    behind.append(entry)
                elif all(ratio < 1.0 for ratio in ratios):
                    ahead.append(entry)
                else:
                    flipping.append(entry)

    lines.append("## What the draws agree on")
    lines.append("")
    lines.append(
        "A rank is a within-run comparison, so a row behind on *every* draw "
        "is a deficit whatever the hosts did, a row ahead on every draw is "
        "held, and a row whose rank changes between draws is decided by the "
        "draw. `rivals moved` reports the widest spread an unchanged rival "
        "showed across these samples, which is the scale of what a single "
        "sample can resolve."
    )
    lines.append("")
    for title, entries in (
        ("Behind on every draw", behind),
        ("Rank changes between draws", flipping),
        ("Ahead on every draw", ahead),
    ):
        lines.append(f"### {title}")
        lines.append("")
        if entries:
            lines.extend(f"- {entry}" for entry in entries)
        else:
            lines.append("None.")
        lines.append("")
    return "\n".join(lines) + "\n"


def parse_sample(text: str) -> tuple[str, Path]:
    label, _, path = text.partition("=")
    if not label or not path:
        raise argparse.ArgumentTypeError("expected LABEL=path")
    return label, Path(path)


def parse_row(text: str) -> tuple[str, str]:
    section, _, dataset = text.partition(":")
    if not section or not dataset:
        raise argparse.ArgumentTypeError("expected SECTION:DATASET, e.g. dumps:mixed.json")
    return section, dataset


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sample", action="append", type=parse_sample, required=True)
    parser.add_argument("--platform", action="append", choices=CI_PLATFORMS)
    parser.add_argument("--row", action="append", type=parse_row)
    parser.add_argument("--rival-tolerance", type=float, default=DEFAULT_TOLERANCE)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--stdout", action="store_true", help="print instead of writing")
    args = parser.parse_args(argv)

    platforms = list(args.platform or CI_PLATFORMS)
    rows = list(args.row or [("dumps", "mixed.json")])
    text = render(args.sample, platforms, rows, args.rival_tolerance)
    if args.stdout:
        print(text, end="")
        return 0
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(text, encoding="utf-8")
    print(f"wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
