# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `sample1` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35265975969/ci
- `sample2` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35267792012/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | aarch64 | 0.0615 | 0.4930 | 0.0779 | 0.0642 | 0.2441 | orjson | 0.958x |
| sample2 | aarch64 | 0.0741 | 0.5199 | 0.0923 | 0.0741 | 0.2652 | orjson | 1.000x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +5%, msgspec +18%, orjson +15%, ujson +9%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD EPYC 7763 64-Core Processor | 0.0668 | 0.5404 | 0.0965 | 0.0706 | 0.2438 | orjson | 0.946x |
| sample2 | AMD EPYC 9V74 80-Core Processor | 0.0639 | 0.5129 | 0.0882 | 0.0687 | 0.2268 | orjson | 0.931x |

verdict: **agree** — every shared rival within tolerance (json 5.4%, msgspec 9.4%, orjson 2.8%, ujson 7.5%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Apple M1 (Virtual) | 0.0449 | 0.4015 | 0.0685 | 0.1285 | 0.1912 | msgspec | 0.656x |
| sample2 | Apple M1 (Virtual) | 0.0615 | 0.4441 | 0.0819 | 0.0715 | 0.2327 | orjson | 0.859x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +11%, msgspec +20%, orjson +80%, ujson +22%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1199 | 1.3446 | 0.1900 | 0.1414 | 0.6614 | orjson | 0.848x |
| sample2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0601 | 0.9037 | 0.1095 | 0.0749 | 0.4302 | orjson | 0.803x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +49%, msgspec +74%, orjson +89%, ujson +54%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0784 | 0.5435 | 0.1168 | 0.0805 | 0.2666 | orjson | 0.974x |
| sample2 | Intel64 Family 6 Model 173 Stepping 1, GenuineIntel | 0.0553 | 0.4087 | 0.0822 | 0.0590 | 0.1819 | orjson | 0.937x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +33%, msgspec +42%, orjson +36%, ujson +47%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

None.

### Rank changes between draws

- linux-arm64 dumps mixed.json: 0.958x, 1.000x (rivals moved 18%)

### Ahead on every draw

- linux-x86_64 dumps mixed.json: 0.946x, 0.931x (rivals held)
- macos-arm64 dumps mixed.json: 0.656x, 0.859x (rivals moved 80%)
- macos-x86_64 dumps mixed.json: 0.848x, 0.803x (rivals moved 89%)
- windows-x86_64 dumps mixed.json: 0.974x, 0.937x (rivals moved 47%)

