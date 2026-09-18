# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `sample1` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35265975969/ci
- `sample2` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35267792012/ci
- `sample3` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35275497426/ci
- `sample4` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35277206429/ci
- `sample5` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35287410197/ci
- `sample6` — /Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/benchmark-lead/p28/ci-35299852958/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | aarch64 | 0.0615 | 0.4930 | 0.0779 | 0.0642 | 0.2441 | orjson | 0.958x |
| sample2 | aarch64 | 0.0741 | 0.5199 | 0.0923 | 0.0741 | 0.2652 | orjson | 1.000x |
| sample3 | aarch64 | 0.0622 | 0.4940 | 0.0772 | 0.0650 | 0.2417 | orjson | 0.957x |
| sample4 | aarch64 | 0.0643 | 0.5006 | 0.0820 | 0.0663 | 0.2451 | orjson | 0.970x |
| sample5 | aarch64 | 0.0654 | 0.4962 | 0.0808 | 0.0689 | 0.2411 | orjson | 0.950x |
| sample6 | aarch64 | 0.0614 | 0.4749 | 0.0788 | 0.0646 | 0.2342 | orjson | 0.951x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +9%, msgspec +20%, orjson +15%, ujson +13%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD EPYC 7763 64-Core Processor | 0.0668 | 0.5404 | 0.0965 | 0.0706 | 0.2438 | orjson | 0.946x |
| sample2 | AMD EPYC 9V74 80-Core Processor | 0.0639 | 0.5129 | 0.0882 | 0.0687 | 0.2268 | orjson | 0.931x |
| sample3 | AMD EPYC 7763 64-Core Processor | 0.0613 | 0.5167 | 0.0850 | 0.0656 | 0.2384 | orjson | 0.935x |
| sample4 | AMD EPYC 7763 64-Core Processor | 0.0616 | 0.5172 | 0.0847 | 0.0657 | 0.2326 | orjson | 0.937x |
| sample5 | INTEL(R) XEON(R) PLATINUM 8573C | 0.0549 | 0.5011 | 0.0751 | 0.0530 | 0.2044 | orjson | 1.035x |
| sample6 | AMD EPYC 7763 64-Core Processor | 0.0625 | 0.5307 | 0.0877 | 0.0663 | 0.2368 | orjson | 0.942x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +8%, msgspec +28%, orjson +33%, ujson +19%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Apple M1 (Virtual) | 0.0449 | 0.4015 | 0.0685 | 0.1285 | 0.1912 | msgspec | 0.656x |
| sample2 | Apple M1 (Virtual) | 0.0615 | 0.4441 | 0.0819 | 0.0715 | 0.2327 | orjson | 0.859x |
| sample3 | Apple M1 (Virtual) | 0.0319 | 0.3257 | 0.0481 | 0.0655 | 0.1596 | msgspec | 0.663x |
| sample4 | Apple M1 (Virtual) | 0.0405 | 0.3688 | 0.0566 | 0.0495 | 0.1770 | orjson | 0.818x |
| sample5 | Apple M1 (Virtual) | 0.0316 | 0.3256 | 0.0474 | 0.0399 | 0.1578 | orjson | 0.794x |
| sample6 | Apple M1 (Virtual) | 0.0455 | 0.4286 | 0.0965 | 0.0602 | 0.2071 | orjson | 0.756x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +36%, msgspec +104%, orjson +222%, ujson +47%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1199 | 1.3446 | 0.1900 | 0.1414 | 0.6614 | orjson | 0.848x |
| sample2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0601 | 0.9037 | 0.1095 | 0.0749 | 0.4302 | orjson | 0.803x |
| sample3 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0767 | 1.0100 | 0.1291 | 0.0926 | 0.4800 | orjson | 0.829x |
| sample4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1696 | 2.1451 | 0.2504 | 0.1824 | 0.9759 | orjson | 0.929x |
| sample5 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0605 | 0.8979 | 0.1089 | 0.0783 | 0.4260 | orjson | 0.773x |
| sample6 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1579 | 1.5742 | 0.2416 | 0.1811 | 0.8368 | orjson | 0.872x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +139%, msgspec +130%, orjson +144%, ujson +129%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| sample1 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0784 | 0.5435 | 0.1168 | 0.0805 | 0.2666 | orjson | 0.974x |
| sample2 | Intel64 Family 6 Model 173 Stepping 1, GenuineIntel | 0.0553 | 0.4087 | 0.0822 | 0.0590 | 0.1819 | orjson | 0.937x |
| sample3 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0735 | 0.5802 | 0.1124 | 0.0761 | 0.2645 | orjson | 0.965x |
| sample4 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0700 | 0.5147 | 0.0948 | 0.0693 | 0.2650 | orjson | 1.009x |
| sample5 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0704 | 0.5530 | 0.1006 | 0.0752 | 0.2585 | orjson | 0.936x |
| sample6 | AMD64 Family 26 Model 2 Stepping 1, AuthenticAMD | 0.0420 | 0.2983 | 0.0647 | 0.0372 | 0.1312 | orjson | 1.128x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +95%, msgspec +81%, orjson +116%, ujson +103%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

None.

### Rank changes between draws

- linux-arm64 dumps mixed.json: 0.958x, 1.000x, 0.957x, 0.970x, 0.950x, 0.951x (rivals moved 20%)
- linux-x86_64 dumps mixed.json: 0.946x, 0.931x, 0.935x, 0.937x, 1.035x, 0.942x (rivals moved 33%)
- windows-x86_64 dumps mixed.json: 0.974x, 0.937x, 0.965x, 1.009x, 0.936x, 1.128x (rivals moved 116%)

### Ahead on every draw

- macos-arm64 dumps mixed.json: 0.656x, 0.859x, 0.663x, 0.818x, 0.794x, 0.756x (rivals moved 222%)
- macos-x86_64 dumps mixed.json: 0.848x, 0.803x, 0.829x, 0.929x, 0.773x, 0.872x (rivals moved 144%)

