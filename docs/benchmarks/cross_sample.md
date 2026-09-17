# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `sample1` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35265975969/ci
- `sample2` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35267792012/ci
- `sample3` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35275497426/ci
- `sample4` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35277206429/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | aarch64 | 0.0615 | 0.4930 | 0.0779 | 0.0642 | 0.2441 | orjson | 0.958x |
| sample2 | aarch64 | 0.0741 | 0.5199 | 0.0923 | 0.0741 | 0.2652 | orjson | 1.000x |
| sample3 | aarch64 | 0.0622 | 0.4940 | 0.0772 | 0.0650 | 0.2417 | orjson | 0.957x |
| sample4 | aarch64 | 0.0643 | 0.5006 | 0.0820 | 0.0663 | 0.2451 | orjson | 0.970x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +5%, msgspec +20%, orjson +15%, ujson +10%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD EPYC 7763 64-Core Processor | 0.0668 | 0.5404 | 0.0965 | 0.0706 | 0.2438 | orjson | 0.946x |
| sample2 | AMD EPYC 9V74 80-Core Processor | 0.0639 | 0.5129 | 0.0882 | 0.0687 | 0.2268 | orjson | 0.931x |
| sample3 | AMD EPYC 7763 64-Core Processor | 0.0613 | 0.5167 | 0.0850 | 0.0656 | 0.2384 | orjson | 0.935x |
| sample4 | AMD EPYC 7763 64-Core Processor | 0.0616 | 0.5172 | 0.0847 | 0.0657 | 0.2326 | orjson | 0.937x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +5%, msgspec +14%, orjson +8%, ujson +7%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Apple M1 (Virtual) | 0.0449 | 0.4015 | 0.0685 | 0.1285 | 0.1912 | msgspec | 0.656x |
| sample2 | Apple M1 (Virtual) | 0.0615 | 0.4441 | 0.0819 | 0.0715 | 0.2327 | orjson | 0.859x |
| sample3 | Apple M1 (Virtual) | 0.0319 | 0.3257 | 0.0481 | 0.0655 | 0.1596 | msgspec | 0.663x |
| sample4 | Apple M1 (Virtual) | 0.0405 | 0.3688 | 0.0566 | 0.0495 | 0.1770 | orjson | 0.818x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +36%, msgspec +70%, orjson +159%, ujson +46%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1199 | 1.3446 | 0.1900 | 0.1414 | 0.6614 | orjson | 0.848x |
| sample2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0601 | 0.9037 | 0.1095 | 0.0749 | 0.4302 | orjson | 0.803x |
| sample3 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0767 | 1.0100 | 0.1291 | 0.0926 | 0.4800 | orjson | 0.829x |
| sample4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1696 | 2.1451 | 0.2504 | 0.1824 | 0.9759 | orjson | 0.929x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +137%, msgspec +129%, orjson +144%, ujson +127%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0784 | 0.5435 | 0.1168 | 0.0805 | 0.2666 | orjson | 0.974x |
| sample2 | Intel64 Family 6 Model 173 Stepping 1, GenuineIntel | 0.0553 | 0.4087 | 0.0822 | 0.0590 | 0.1819 | orjson | 0.937x |
| sample3 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0735 | 0.5802 | 0.1124 | 0.0761 | 0.2645 | orjson | 0.965x |
| sample4 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0700 | 0.5147 | 0.0948 | 0.0693 | 0.2650 | orjson | 1.009x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +42%, msgspec +42%, orjson +36%, ujson +47%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

None.

### Rank changes between draws

- linux-arm64 dumps mixed.json: 0.958x, 1.000x, 0.957x, 0.970x (rivals moved 20%)
- windows-x86_64 dumps mixed.json: 0.974x, 0.937x, 0.965x, 1.009x (rivals moved 47%)

### Ahead on every draw

- linux-x86_64 dumps mixed.json: 0.946x, 0.931x, 0.935x, 0.937x (rivals moved 14%)
- macos-arm64 dumps mixed.json: 0.656x, 0.859x, 0.663x, 0.818x (rivals moved 159%)
- macos-x86_64 dumps mixed.json: 0.848x, 0.803x, 0.829x, 0.929x (rivals moved 144%)

