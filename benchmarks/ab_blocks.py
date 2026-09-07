"""The one A/B estimator: complete ABBA blocks, one effect, one interval.

Every A/B claim in this repository is read through this module. It exists
because the campaign briefly had two: `ab_blocks` computed a median of
normalised ABBA block effects while `ab_floor` bootstrapped *individual*
launches and reported a ratio of medians, so the number and the interval
printed beside it were not statistics of the same quantity (the 7 September
review, finding 4). `ab_floor.py` and `ab_builds.py --analyze` are now views
over the functions here; they cannot disagree with this file or with each
other because they do not compute anything themselves.

The estimator, stated once:

* **A launch** is one process: one extension build, one `rows_probe.py` run,
  every sample it took. Its value for a row is the median of its samples.
* **A normalised launch** is `median(strata) / median(rival)` where the rival
  ran *in the same process*, on the same row, in alternating order
  (`ab_rounds.alternating_rounds`). The rival binary never changes, so what it
  moved by is the machine. The rival is chosen per operation: `dumps` rows are
  read against `orjson-bytes` because the canonical row measures bytes,
  `loads` against `orjson-loads`, and the file operations against orjson's
  documented composition (`RIVAL_BY_ENGINE`). A row whose rival is missing
  from a launch is a rejected packet, not a raw-only reading.
* **A block** is four consecutive launches `A B B A`. Linear drift across the
  block cancels: the mean of its two A launches and the mean of its two B
  launches sit at the same point in time. A block effect is
  `mean(B) / mean(A) - 1`.
* **The effect** is the median of the block effects, and **the interval** is a
  bootstrap that resamples *whole blocks* — the same unit the point estimate
  is made of. Resampling launches instead would report an interval for a
  different statistic.
* **The A/A control** is the identical estimator over two launches of the same
  binary, with a comparable block count. Its floor is `max(|ci low|, |ci
  high|)` — the further of the interval's two ends from zero — and an effect
  inside it is not resolvable in that session. A control of fewer than
  `MIN_CONTROL_BLOCKS` blocks is reported with a warning: a bootstrap can only
  resample the block effects it has, and two of them do not make a 95%
  interval.
* **Raw strata, raw rival and normalised** effects are printed from the same
  packet, side by side. A rival that moved as much as strata did is drift; a
  rival that moved on its own is a shared-process effect, and the reader is
  entitled to see which one happened.

What it refuses, rather than average over: a launch out of ABBA order, a
trailing launch that is not the baseline build, a third build label without an
explicit `--pair` (three source arms are three pairwise comparisons, never a
pooled "everything that is not A"), a launch missing its rival or its strata
row, a non-finite or non-positive sample, and a launch with fewer samples than
`--min-samples`.

usage:
  ab_blocks.py <tsv> [<tsv> ...] [--baseline A] [--pair A:B]
               [--aa <tsv>] [--min-samples N] [--json <path>]
"""

from __future__ import annotations

import argparse
import json
import math
import random
import statistics
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

BOOTSTRAP_SEED = 42
BOOTSTRAP_RESAMPLES = 2000
BLOCK = 4

# The one minimum every front end applies. `ab_floor.py`, `ab_builds.py
# --analyze` and `ab_rows.py` all take their default from here, so the three
# views of one packet cannot disagree about whether it is valid: a packet with
# short launches is refused by all of them or by none (the 7 September review's
# T3 follow-up, defect 2).
DEFAULT_MIN_SAMPLES = 10

# Below this many blocks the bootstrap interval is not a 95% interval: it can
# only resample the block effects that exist, so a two-block control's interval
# spans its two block effects and nothing else. A simulation of the estimator
# (4000 synthetic sessions) measures ~51% coverage at two blocks against ~94%
# at six, which is why a floor from a short control is announced as one.
MIN_CONTROL_BLOCKS = 4

# The rival for a strata engine, per operation. `rows_probe.py` names its
# engines after the call it timed, and the canonical row's composition
# (benchmarks/bench_main.py) decides which orjson call is the fair partner:
# bytes against bytes for `dumps`, read+parse for `load`, per-line parse for
# NDJSON, serialize+write for `dump`. `strata-str` is the text form of the
# same serialization and is read against the same bytes rival, which is what
# makes its number comparable with the canonical row's.
RIVAL_BY_ENGINE = {
    "strata-bytes": "orjson-bytes",
    "strata-str": "orjson-bytes",
    "strata-loads": "orjson-loads",
    "strata-load": "orjson-load",
    "strata-ndload": "orjson-ndload",
    "strata-dump": "orjson-dump",
    # The oldest probes named the two engines without an operation suffix.
    "strata": "orjson",
}

TSV_COLUMNS = ("tag", "build", "engine", "row", "round", "ms")


class AnalysisError(Exception):
    """The packet cannot be read as a valid A/B experiment.

    Raised rather than returning a degraded result: every condition that
    reaches this exception invalidates the comparison, and a printed number
    with a footnote is how the campaign lost a week.
    """


# ---------------------------------------------------------------------------
# Reading and validating a packet
# ---------------------------------------------------------------------------


@dataclass
class Launch:
    """One process: its order in the session, its build, its samples."""

    index: int
    tag: str
    build: str
    samples: dict[tuple[str, str], list[float]] = field(default_factory=dict)

    def median(self, engine: str, row: str) -> float | None:
        values = self.samples.get((engine, row))
        return statistics.median(values) if values else None


def _tag_key(tag: str) -> tuple[str, int, str]:
    """Sort `L9` before `L10`: the drivers zero-pad, but a longer session must
    not silently reorder itself if one ever stops."""
    digits = ""
    while tag and tag[-1].isdigit():
        digits = tag[-1] + digits
        tag = tag[:-1]
    return (tag, int(digits) if digits else -1, digits)


def read_launches(path: Path, *, min_samples: int = DEFAULT_MIN_SAMPLES) -> list[Launch]:
    """Parse a driver TSV into launches, in the order they ran.

    Order comes from the `tag` column (`L00`, `L01`, ...), which the drivers
    write from the launch index, so it is the session's chronology and not the
    order rows happen to appear in the file.
    """
    rows: list[dict[str, str]] = []
    with open(path, encoding="utf-8") as handle:
        first = handle.readline().rstrip("\n")
        header = first.split("\t")
        if tuple(header) != TSV_COLUMNS:
            raise AnalysisError(f"{path}: header is {header}, expected {list(TSV_COLUMNS)}")
        for number, line in enumerate(handle, start=2):
            if not line.strip() or line.startswith("#"):
                continue
            fields = line.rstrip("\n").split("\t")
            if len(fields) != len(TSV_COLUMNS):
                raise AnalysisError(f"{path}:{number}: {len(fields)} fields, expected 6")
            row = dict(zip(header, fields, strict=True))
            try:
                value = float(row["ms"])
            except ValueError:
                raise AnalysisError(f"{path}:{number}: ms is not a number: {row['ms']!r}") from None
            if not math.isfinite(value) or value <= 0.0:
                raise AnalysisError(f"{path}:{number}: ms is not a positive finite time: {value!r}")
            row["value"] = value  # type: ignore[assignment]
            rows.append(row)
    if not rows:
        raise AnalysisError(f"{path}: no samples")

    order: list[str] = []
    builds: dict[str, str] = {}
    samples: dict[str, dict[tuple[str, str], list[float]]] = defaultdict(lambda: defaultdict(list))
    for row in rows:
        tag = row["tag"]
        if tag not in builds:
            builds[tag] = row["build"]
            order.append(tag)
        elif builds[tag] != row["build"]:
            raise AnalysisError(
                f"{path}: launch {tag} carries two build labels: {builds[tag]} and {row['build']}"
            )
        samples[tag][(row["engine"], row["row"])].append(row["value"])  # type: ignore[index]

    launches = [
        Launch(index=index, tag=tag, build=builds[tag], samples=dict(samples[tag]))
        for index, tag in enumerate(sorted(order, key=_tag_key))
    ]
    for launch in launches:
        for (engine, row), values in launch.samples.items():
            if len(values) < min_samples:
                raise AnalysisError(
                    f"{path}: launch {launch.tag} has {len(values)} samples for "
                    f"{row}/{engine}, fewer than the required {min_samples}"
                )
    return launches


# ---------------------------------------------------------------------------
# Blocks
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class Block:
    """One `A B B A` window of four consecutive launches."""

    number: int
    a: tuple[Launch, Launch]
    b: tuple[Launch, Launch]

    def label(self) -> str:
        return "".join(launch.tag for launch in (self.a[0], self.b[0], self.b[1], self.a[1]))


@dataclass(frozen=True)
class Structure:
    """The block decomposition of one session, plus what it left over."""

    base: str
    other: str
    blocks: list[Block]
    leading: list[Launch]
    trailing: list[Launch]
    launches: list[Launch]
    dropped: list[Launch]


def build_structure(
    launches: list[Launch], *, baseline: str | None = None, pair: tuple[str, str] | None = None
) -> Structure:
    """Split the launch order into complete ABBA blocks, or refuse.

    `pair` selects two build labels out of a session that holds more; the
    launches of the other arms are removed, and a block is still refused if
    one of them ran *inside* it, because a foreign launch between the A and
    the B breaks exactly the drift cancellation the block is for.
    """
    if not launches:
        raise AnalysisError("no launches")
    labels = list(dict.fromkeys(launch.build for launch in launches))
    selected = launches
    if pair is not None:
        missing = [label for label in pair if label not in labels]
        if missing:
            raise AnalysisError(f"--pair names builds not in the packet: {missing}")
        selected = [launch for launch in launches if launch.build in pair]
        labels = [label for label in labels if label in pair]
    elif len(labels) != 2:
        raise AnalysisError(
            f"the packet holds build labels {labels}; two are required. "
            "Three source arms are three pairwise comparisons: name one with --pair A:B."
        )

    base = baseline or selected[0].build
    if base not in labels:
        raise AnalysisError(f"baseline build {base!r} is not in the packet: {labels}")
    others = [label for label in labels if label != base]
    if len(others) != 1:
        raise AnalysisError(f"expected exactly one non-baseline build, got {others}")
    other = others[0]

    by_index = {launch.index: launch for launch in launches}
    kept = {launch.index for launch in selected}
    blocks: list[Block] = []
    position = 0
    # Baseline launches before the first block: the mirror of the trailing
    # baseline, and the only way `--pair` can reach the second campaign in a
    # three-arm session. Only the baseline build may be skipped, so nothing
    # that could carry a candidate effect is ever dropped silently.
    while (
        position + BLOCK <= len(selected)
        and selected[position].build == base
        and [launch.build for launch in selected[position : position + BLOCK]]
        != [base, other, other, base]
    ):
        position += 1
    leading = selected[:position]
    while position + BLOCK <= len(selected):
        window = selected[position : position + BLOCK]
        pattern = [launch.build for launch in window]
        if pattern != [base, other, other, base]:
            break
        span = range(window[0].index, window[-1].index + 1)
        foreign = [
            by_index[index].tag
            for index in span
            if index in by_index and by_index[index].build not in (base, other)
        ]
        if foreign:
            raise AnalysisError(
                f"block {len(blocks)} ({window[0].tag}..{window[-1].tag}) is interrupted by "
                f"launches of another build: {foreign}"
            )
        blocks.append(Block(number=len(blocks), a=(window[0], window[3]), b=(window[1], window[2])))
        position += BLOCK

    trailing = selected[position:]
    stray = [launch.tag for launch in trailing if launch.build != base]
    if stray:
        raise AnalysisError(
            f"launches out of {base}{other}{other}{base} order at {stray[0]}: "
            f"{''.join(launch.build for launch in selected)}"
        )
    if not blocks:
        raise AnalysisError(
            f"no complete {base}{other}{other}{base} block in "
            f"{''.join(launch.build for launch in selected)}"
        )
    return Structure(
        base=base,
        other=other,
        blocks=blocks,
        leading=leading,
        trailing=trailing,
        launches=selected,
        dropped=[launch for launch in launches if launch.index not in kept],
    )


# ---------------------------------------------------------------------------
# The estimator
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class Interval:
    """A block-bootstrap interval on the median of block effects."""

    low: float
    high: float

    @property
    def floor(self) -> float:
        """`max(|low|, |high|)`: the interval end furthest from zero.

        An effect smaller than this is inside the interval an A/A session —
        two builds known to be identical — produced, so the session cannot
        distinguish it from nothing.
        """
        return max(abs(self.low), abs(self.high))


def bootstrap_blocks(effects: list[float], *, seed: int = BOOTSTRAP_SEED) -> Interval:
    """Resample whole blocks and re-take the median of block effects.

    The resampling unit is the block because the point estimate is a statistic
    *of* blocks. Deterministic: the same effects give the same interval.
    """
    if not effects:
        raise AnalysisError("no block effects to resample")
    if len(effects) == 1:
        return Interval(low=effects[0], high=effects[0])
    rng = random.Random(seed)
    draws = []
    for _ in range(BOOTSTRAP_RESAMPLES):
        drawn = [effects[rng.randrange(len(effects))] for _ in range(len(effects))]
        draws.append(statistics.median(drawn))
    draws.sort()
    return Interval(
        low=draws[int(0.025 * (len(draws) - 1))],
        high=draws[int(0.975 * (len(draws) - 1))],
    )


def block_effects(values: dict[str, float], structure: Structure) -> list[float]:
    """`mean(B) / mean(A) - 1` per block, from per-launch values keyed by tag."""
    effects = []
    for block in structure.blocks:
        a = statistics.fmean(values[launch.tag] for launch in block.a)
        b = statistics.fmean(values[launch.tag] for launch in block.b)
        effects.append(b / a - 1.0)
    return effects


@dataclass
class Series:
    """One row, one strata engine: the three effects and the interval."""

    row: str
    engine: str
    rival: str
    normalised: list[float]
    raw_strata: list[float]
    raw_rival: list[float]
    interval: Interval
    drift_strata: float | None
    drift_rival: float | None
    drift_normalised: float | None

    @property
    def effect(self) -> float:
        return statistics.median(self.normalised)

    @property
    def strata_change(self) -> float:
        return statistics.median(self.raw_strata)

    @property
    def rival_change(self) -> float:
        return statistics.median(self.raw_rival)

    @property
    def positive(self) -> int:
        return sum(1 for value in self.normalised if value > 0)

    @property
    def blocks(self) -> int:
        return len(self.normalised)


@dataclass
class Analysis:
    """Everything one packet supports, and nothing it does not."""

    path: Path
    structure: Structure
    series: list[Series]
    min_samples: int

    def find(self, row: str, engine: str) -> Series | None:
        for series in self.series:
            if series.row == row and series.engine == engine:
                return series
        return None


def _launch_values(
    launches: list[Launch], engine: str, row: str, rival: str, path: Path
) -> tuple[dict[str, float], dict[str, float], dict[str, float]]:
    strata: dict[str, float] = {}
    rival_values: dict[str, float] = {}
    normalised: dict[str, float] = {}
    for launch in launches:
        own = launch.median(engine, row)
        other = launch.median(rival, row)
        if own is None:
            raise AnalysisError(f"{path}: launch {launch.tag} has no {row}/{engine} samples")
        if other is None:
            raise AnalysisError(
                f"{path}: launch {launch.tag} has {row}/{engine} but no {row}/{rival} — "
                "the in-process drift control is missing, so the row cannot be normalised"
            )
        strata[launch.tag] = own
        rival_values[launch.tag] = other
        normalised[launch.tag] = own / other
    return strata, rival_values, normalised


def analyze(
    path: Path,
    *,
    baseline: str | None = None,
    pair: tuple[str, str] | None = None,
    min_samples: int = DEFAULT_MIN_SAMPLES,
) -> Analysis:
    """Read one driver TSV and produce the packet's single analysis."""
    launches = read_launches(path, min_samples=min_samples)
    structure = build_structure(launches, baseline=baseline, pair=pair)
    used = [launch for block in structure.blocks for launch in (*block.a, *block.b)]

    present = {key for launch in used for key in launch.samples}
    unknown = sorted(
        engine
        for engine, _ in present
        if engine not in RIVAL_BY_ENGINE and engine not in set(RIVAL_BY_ENGINE.values())
    )
    if unknown:
        raise AnalysisError(f"{path}: no rival is defined for engines {unknown}")

    series: list[Series] = []
    for row in sorted({row for _, row in present}):
        for engine in sorted({engine for engine, name in present if name == row}):
            rival = RIVAL_BY_ENGINE.get(engine)
            if rival is None:
                continue  # a rival engine; it is the denominator, not a subject
            strata, rival_values, normalised = _launch_values(used, engine, row, rival, path)
            drift_strata = drift_rival = drift_normalised = None
            if structure.trailing:
                head, tail = structure.blocks[0].a[0], structure.trailing[-1]
                tail_own = tail.median(engine, row)
                tail_rival = tail.median(rival, row)
                if tail_own is not None and tail_rival is not None:
                    drift_strata = tail_own / strata[head.tag] - 1.0
                    drift_rival = tail_rival / rival_values[head.tag] - 1.0
                    drift_normalised = (tail_own / tail_rival) / normalised[head.tag] - 1.0
            effects = block_effects(normalised, structure)
            series.append(
                Series(
                    row=row,
                    engine=engine,
                    rival=rival,
                    normalised=effects,
                    raw_strata=block_effects(strata, structure),
                    raw_rival=block_effects(rival_values, structure),
                    interval=bootstrap_blocks(effects),
                    drift_strata=drift_strata,
                    drift_rival=drift_rival,
                    drift_normalised=drift_normalised,
                )
            )
    return Analysis(path=path, structure=structure, series=series, min_samples=min_samples)


# ---------------------------------------------------------------------------
# Rendering
# ---------------------------------------------------------------------------


def _percent(value: float | None) -> str:
    return "     -" if value is None else f"{value * 100:+.2f}%"


def control_warning(control: Analysis) -> str | None:
    """The sentence a short A/A control must be printed with, or None.

    A floor is quoted as if it were a 95% bound; from two blocks it is not
    one, and a reader comparing a small effect against it deserves to be told
    so on the same page (the 7 September review's T3 follow-up, defect 3).
    """
    count = len(control.structure.blocks)
    if count >= MIN_CONTROL_BLOCKS:
        return None
    return (
        f"WARNING: the A/A control has {count} block"
        f"{'' if count == 1 else 's'}; a bootstrap over fewer than "
        f"{MIN_CONTROL_BLOCKS} blocks resamples too few values to be a 95% interval "
        "(~51% coverage at two blocks in simulation, ~94% at six). Read this "
        "floor as a lower bound on the session's true floor, and re-run the "
        "control with the candidate's block count before certifying an effect "
        "on the floor alone."
    )


def render(analysis: Analysis, *, control: Analysis | None = None, detail: bool = True) -> str:
    """The packet as text: identity, structure, effects, then drift."""
    structure = analysis.structure
    selected = {launch.tag for launch in structure.launches}
    lines = [
        f"file: {analysis.path}",
        # Every launch in the file, including the ones `--pair` dropped: a
        # gap in the tag sequence is not a readable account of what was left
        # out (the 7 September review's T3 follow-up, defect 5).
        "launches: "
        + " ".join(
            f"{launch.tag}:{launch.build}" + ("" if launch.tag in selected else "(dropped)")
            for launch in sorted(
                structure.launches + structure.dropped, key=lambda launch: launch.index
            )
        ),
        f"baseline: {structure.base}   candidate: {structure.other}   "
        f"blocks: {len(structure.blocks)}   "
        f"leading: {' '.join(launch.tag for launch in structure.leading) or 'none'}   "
        f"trailing: {' '.join(launch.tag for launch in structure.trailing) or 'none'}   "
        f"min samples/launch: {analysis.min_samples}",
        f"blocks: {'  '.join(block.label() for block in structure.blocks)}",
    ]
    if control is not None:
        lines.append(f"A/A control: {control.path} ({len(control.structure.blocks)} blocks)")
        warning = control_warning(control)
        if warning is not None:
            lines.append(warning)
    lines.append("")
    header = (
        f"{'row':<22}{'engine':<14}{'rival':<14}{'blk':>4}"
        f"{'raw strata':>12}{'raw rival':>11}{'normalised':>12}"
        f"{'ci low':>10}{'ci high':>10}{'pos':>6}"
    )
    if control is not None:
        header += f"{'A/A':>10}{'floor':>9}"
    lines.append(header)
    for series in analysis.series:
        line = (
            f"{series.row:<22}{series.engine:<14}{series.rival:<14}{series.blocks:>4}"
            f"{_percent(series.strata_change):>12}{_percent(series.rival_change):>11}"
            f"{_percent(series.effect):>12}"
            f"{_percent(series.interval.low):>10}{_percent(series.interval.high):>10}"
            f"{series.positive:>3}/{series.blocks}"
        )
        if control is not None:
            match = control.find(series.row, series.engine)
            if match is None:
                line += f"{'-':>10}{'-':>9}"
            else:
                line += f"{_percent(match.effect):>10}{match.interval.floor * 100:>8.2f}%"
        lines.append(line)
        if detail:
            blocks = " ".join(f"{value * 100:+.2f}" for value in series.normalised)
            lines.append(f"{'':<22}{'':<14}blocks (normalised): {blocks}")
    lines.append("")
    lines.append("raw strata / raw rival: the same block estimator on the unnormalised medians.")
    lines.append("normalised: strata/rival measured in the same process, then blocked.")
    lines.append("ci: bootstrap over whole blocks of the median block effect (2000 resamples).")
    if control is not None:
        lines.append(
            "A/A: the identical estimator on two launches of one binary; "
            "floor = max(|ci low|, |ci high|)."
        )
    if structure.trailing:
        lines.append("")
        lines.append("== drift: the trailing baseline launch against the leading one")
        for series in analysis.series:
            lines.append(
                f"{series.row:<22}{series.engine:<14}"
                f"strata {_percent(series.drift_strata)}   "
                f"rival {_percent(series.drift_rival)}   "
                f"normalised {_percent(series.drift_normalised)}"
            )
    return "\n".join(lines)


def to_json(analysis: Analysis, control: Analysis | None = None) -> dict:
    """The same numbers, for a report that wants to tabulate them."""
    payload = {
        "file": str(analysis.path),
        "baseline": analysis.structure.base,
        "candidate": analysis.structure.other,
        "blocks": len(analysis.structure.blocks),
        "launches": [
            {"tag": launch.tag, "build": launch.build} for launch in analysis.structure.launches
        ],
        "dropped": [
            {"tag": launch.tag, "build": launch.build} for launch in analysis.structure.dropped
        ],
        "rows": [],
    }
    for series in analysis.series:
        entry = {
            "row": series.row,
            "engine": series.engine,
            "rival": series.rival,
            "raw_strata": series.strata_change,
            "raw_rival": series.rival_change,
            "normalised": series.effect,
            "ci_low": series.interval.low,
            "ci_high": series.interval.high,
            "block_effects": series.normalised,
            "positive": series.positive,
        }
        if control is not None:
            match = control.find(series.row, series.engine)
            if match is not None:
                entry["aa_effect"] = match.effect
                entry["aa_floor"] = match.interval.floor
                entry["aa_blocks"] = len(control.structure.blocks)
        payload["rows"].append(entry)
    return payload


def _pair(text: str) -> tuple[str, str]:
    first, _, second = text.partition(":")
    if not first or not second:
        raise argparse.ArgumentTypeError(f"--pair wants A:B, got {text!r}")
    return (first, second)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tsv", nargs="+", type=Path)
    parser.add_argument("--baseline", default=None, help="the build label of arm A")
    parser.add_argument(
        "--pair", type=_pair, default=None, help="A:B, when the packet has three arms"
    )
    parser.add_argument("--aa", type=Path, default=None, help="an A/A packet to read as the floor")
    parser.add_argument("--min-samples", type=int, default=DEFAULT_MIN_SAMPLES)
    parser.add_argument("--json", type=Path, default=None, help="write the analysis as JSON too")
    parser.add_argument("--no-detail", action="store_true", help="omit the per-block effects")
    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)

    control = None
    if args.aa is not None:
        control = analyze(
            args.aa, baseline=args.baseline, pair=args.pair, min_samples=args.min_samples
        )
        warning = control_warning(control)
        if warning is not None:
            print(warning, file=sys.stderr)

    payloads = []
    for index, path in enumerate(args.tsv):
        analysis = analyze(
            path, baseline=args.baseline, pair=args.pair, min_samples=args.min_samples
        )
        if index:
            print()
        print(render(analysis, control=control, detail=not args.no_detail))
        payloads.append(to_json(analysis, control))
    if args.json is not None:
        args.json.write_text(json.dumps(payloads, indent=2) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AnalysisError as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(2) from None
