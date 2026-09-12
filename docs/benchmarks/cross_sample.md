# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `ec53f93-s1` — build/evidence/benchmark-lead/p23/ci-34681733282/ci
- `ec53f93-s2` — build/evidence/benchmark-lead/p23/ci-34681741789/ci
- `b32d398-s1` — build/evidence/benchmark-lead/p24/ci-34693842452/ci
- `b32d398-s2` — build/evidence/benchmark-lead/p24/ci-34693855300/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | aarch64 | 0.0837 | 0.5340 | 0.0987 | 0.0887 | 0.2754 | orjson | 0.944x |
| ec53f93-s2 | aarch64 | 0.0660 | 0.4993 | 0.0807 | 0.0669 | 0.2443 | orjson | 0.987x |
| b32d398-s1 | aarch64 | 0.0675 | 0.4946 | 0.0841 | 0.0699 | 0.2584 | orjson | 0.966x |
| b32d398-s2 | aarch64 | 0.0726 | 0.5281 | 0.0897 | 0.0736 | 0.2524 | orjson | 0.986x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +8%, msgspec +22%, orjson +33%, ujson +13%)

## linux-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | aarch64 | 0.1702 | 0.6354 | 0.2008 | 0.1894 | 0.3927 | orjson | 0.898x |
| ec53f93-s2 | aarch64 | 0.1927 | 0.6588 | 0.2317 | 0.2143 | 0.4127 | orjson | 0.899x |
| b32d398-s1 | aarch64 | 0.2142 | 0.6855 | 0.2436 | 0.2391 | 0.4375 | orjson | 0.896x |
| b32d398-s2 | aarch64 | 0.2362 | 0.7370 | 0.2927 | 0.2778 | 0.4850 | orjson | 0.850x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +16%, msgspec +46%, orjson +47%, ujson +23%)

## linux-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | aarch64 | 0.2360 | 1.6798 | 0.3826 | 0.2935 | 0.9842 | orjson | 0.804x |
| ec53f93-s2 | aarch64 | 0.2405 | 1.6954 | 0.3887 | 0.2979 | 0.9819 | orjson | 0.807x |
| b32d398-s1 | aarch64 | 0.2473 | 1.7094 | 0.4095 | 0.3121 | 0.9954 | orjson | 0.792x |
| b32d398-s2 | aarch64 | 0.2734 | 1.8015 | 0.4297 | 0.3286 | 1.0663 | orjson | 0.832x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +7%, msgspec +12%, orjson +12%, ujson +9%)

## linux-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | aarch64 | 1.9253 | 18.9579 | 3.2962 | 2.5686 | 10.4808 | orjson | 0.750x |
| ec53f93-s2 | aarch64 | 1.9495 | 19.1384 | 3.3288 | 2.5971 | 10.5762 | orjson | 0.751x |
| b32d398-s1 | aarch64 | 1.9427 | 18.9459 | 3.3422 | 2.6082 | 10.5369 | orjson | 0.745x |
| b32d398-s2 | aarch64 | 1.9660 | 19.2032 | 3.3648 | 2.6305 | 10.6977 | orjson | 0.747x |

verdict: **agree** — every shared rival within tolerance (json 1.4%, msgspec 2.1%, orjson 2.4%, ujson 2.1%)

## linux-arm64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | aarch64 | 0.1891 | 0.4468 | 0.2361 | 0.2258 | 0.2958 | 0.3057 | orjson | 0.837x |
| ec53f93-s2 | aarch64 | 0.1938 | 0.4693 | 0.2382 | 0.2175 | 0.2978 | 0.3137 | orjson | 0.891x |
| b32d398-s1 | aarch64 | 0.1956 | 0.4665 | 0.2431 | 0.2229 | 0.3128 | 0.3176 | orjson | 0.878x |
| b32d398-s2 | aarch64 | 0.2087 | 0.5048 | 0.2597 | 0.2392 | 0.3239 | 0.3468 | orjson | 0.873x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +13%, msgspec +10%, orjson +10%, pysimdjson +9%, ujson +13%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD EPYC 9V74 80-Core Processor | 0.0646 | 0.5379 | 0.0887 | 0.0710 | 0.2318 | orjson | 0.909x |
| ec53f93-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.0920 | 0.5778 | 0.0962 | 0.1222 | 0.2462 | msgspec | 0.957x |
| b32d398-s1 | AMD EPYC 9V74 80-Core Processor | 0.0502 | 0.4133 | 0.0683 | 0.0479 | 0.1789 | orjson | 1.047x |
| b32d398-s2 | AMD EPYC 9V74 80-Core Processor | 0.0483 | 0.4082 | 0.0680 | 0.0478 | 0.1784 | orjson | 1.011x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +42%, msgspec +41%, orjson +155%, ujson +38%)

## linux-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD EPYC 9V74 80-Core Processor | 0.1680 | 0.6638 | 0.1918 | 0.1769 | 0.3516 | orjson | 0.949x |
| ec53f93-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2074 | 0.7317 | 0.2383 | 0.2569 | 0.4031 | msgspec | 0.870x |
| b32d398-s1 | AMD EPYC 9V74 80-Core Processor | 0.1159 | 0.5053 | 0.1425 | 0.1251 | 0.2651 | orjson | 0.926x |
| b32d398-s2 | AMD EPYC 9V74 80-Core Processor | 0.1161 | 0.4941 | 0.1424 | 0.1236 | 0.2646 | orjson | 0.940x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +48%, msgspec +67%, orjson +108%, ujson +52%)

## linux-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD EPYC 9V74 80-Core Processor | 0.3176 | 1.8718 | 0.4859 | 0.3769 | 1.0348 | orjson | 0.843x |
| ec53f93-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2900 | 2.0158 | 0.4933 | 0.3899 | 1.0524 | orjson | 0.744x |
| b32d398-s1 | AMD EPYC 9V74 80-Core Processor | 0.2426 | 1.4510 | 0.3799 | 0.2479 | 0.8220 | orjson | 0.979x |
| b32d398-s2 | AMD EPYC 9V74 80-Core Processor | 0.2474 | 1.4557 | 0.3742 | 0.2481 | 0.8314 | orjson | 0.997x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +39%, msgspec +32%, orjson +57%, ujson +28%)

## linux-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD EPYC 9V74 80-Core Processor | 2.3646 | 22.1580 | 4.1561 | 3.0923 | 11.4507 | orjson | 0.765x |
| ec53f93-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 2.3232 | 22.7149 | 4.0307 | 2.5069 | 11.8257 | orjson | 0.927x |
| b32d398-s1 | AMD EPYC 9V74 80-Core Processor | 1.8100 | 16.9400 | 3.2539 | 1.9914 | 8.9931 | orjson | 0.909x |
| b32d398-s2 | AMD EPYC 9V74 80-Core Processor | 1.8151 | 16.6293 | 3.2294 | 1.9996 | 8.8052 | orjson | 0.908x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +37%, msgspec +29%, orjson +55%, ujson +34%)

## linux-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD EPYC 9V74 80-Core Processor | 0.1946 | 0.4574 | 0.2489 | 0.2419 | 0.3087 | 0.3181 | orjson | 0.804x |
| ec53f93-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2607 | 0.5367 | 0.3125 | 0.2877 | 0.3938 | 0.3706 | orjson | 0.906x |
| b32d398-s1 | AMD EPYC 9V74 80-Core Processor | 0.1527 | 0.3585 | 0.1898 | 0.1885 | 0.2448 | 0.2397 | orjson | 0.810x |
| b32d398-s2 | AMD EPYC 9V74 80-Core Processor | 0.1518 | 0.3494 | 0.1959 | 0.1856 | 0.2413 | 0.2434 | orjson | 0.818x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +54%, msgspec +65%, orjson +55%, pysimdjson +63%, ujson +55%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Apple M1 (Virtual) | 0.0352 | 0.3504 | 0.0805 | 0.0454 | 0.1664 | orjson | 0.775x |
| ec53f93-s2 | Apple M1 (Virtual) | 0.0575 | 0.4780 | 0.0735 | 0.0653 | 0.2408 | orjson | 0.881x |
| b32d398-s1 | Apple M1 (Virtual) | 0.0369 | 0.3563 | 0.0548 | 0.0476 | 0.1691 | orjson | 0.774x |
| b32d398-s2 | Apple M1 (Virtual) | 0.0382 | 0.3559 | 0.0559 | 0.0473 | 0.1754 | orjson | 0.807x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +36%, msgspec +47%, orjson +44%, ujson +45%)

## macos-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Apple M1 (Virtual) | 0.1843 | 0.5399 | 0.3144 | 0.2080 | 0.3737 | orjson | 0.886x |
| ec53f93-s2 | Apple M1 (Virtual) | 0.2813 | 0.6992 | 0.3660 | 0.2991 | 0.5163 | orjson | 0.940x |
| b32d398-s1 | Apple M1 (Virtual) | 0.1569 | 0.4947 | 0.1831 | 0.1816 | 0.3017 | orjson | 0.864x |
| b32d398-s2 | Apple M1 (Virtual) | 0.2251 | 0.6003 | 0.2162 | 0.2170 | 0.3500 | msgspec | 1.041x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +41%, msgspec +100%, orjson +65%, ujson +71%)

## macos-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Apple M1 (Virtual) | 0.2090 | 1.3615 | 0.3275 | 0.4618 | 0.7868 | msgspec | 0.638x |
| ec53f93-s2 | Apple M1 (Virtual) | 0.2924 | 1.9929 | 0.3797 | 0.2902 | 0.8992 | orjson | 1.008x |
| b32d398-s1 | Apple M1 (Virtual) | 0.2337 | 1.5765 | 0.3400 | 0.2803 | 0.8245 | orjson | 0.834x |
| b32d398-s2 | Apple M1 (Virtual) | 0.2021 | 1.4400 | 0.3144 | 0.2473 | 0.7842 | orjson | 0.817x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +46%, msgspec +21%, orjson +87%, ujson +15%)

## macos-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Apple M1 (Virtual) | 1.8732 | 18.3230 | 3.7922 | 2.7202 | 14.0492 | orjson | 0.689x |
| ec53f93-s2 | Apple M1 (Virtual) | 1.7750 | 17.5613 | 3.2108 | 2.6068 | 9.7466 | orjson | 0.681x |
| b32d398-s1 | Apple M1 (Virtual) | 1.5448 | 16.3022 | 3.1782 | 2.4291 | 9.2026 | orjson | 0.636x |
| b32d398-s2 | Apple M1 (Virtual) | 1.6530 | 16.9275 | 3.2160 | 2.5801 | 9.9289 | orjson | 0.641x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +12%, msgspec +19%, orjson +12%, ujson +53%)

## macos-arm64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Apple M1 (Virtual) | 0.1219 | 0.3208 | 0.1706 | 0.1615 | 2.5022 | 0.2759 | orjson | 0.754x |
| ec53f93-s2 | Apple M1 (Virtual) | 0.1737 | 0.4135 | 0.2157 | 0.1976 | 2.8619 | 0.4388 | orjson | 0.879x |
| b32d398-s1 | Apple M1 (Virtual) | 0.1151 | 0.3096 | 0.1580 | 0.1489 | 2.3752 | 0.2331 | orjson | 0.773x |
| b32d398-s2 | Apple M1 (Virtual) | 0.1240 | 0.3295 | 0.1728 | 0.1580 | 2.4725 | 0.2145 | orjson | 0.785x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +34%, msgspec +36%, orjson +33%, pysimdjson +20%, ujson +105%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0823 | 0.9944 | 0.1310 | 0.1000 | 0.4796 | orjson | 0.823x |
| ec53f93-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1088 | 1.3851 | 0.1848 | 0.1366 | 0.6110 | orjson | 0.797x |
| b32d398-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0610 | 0.9042 | 0.1058 | 0.0747 | 0.4271 | orjson | 0.817x |
| b32d398-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0609 | 0.9067 | 0.1037 | 0.0750 | 0.4312 | orjson | 0.812x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +53%, msgspec +78%, orjson +83%, ujson +43%)

## macos-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.4022 | 1.3854 | 0.5273 | 0.4449 | 0.8395 | orjson | 0.904x |
| ec53f93-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.5177 | 1.7126 | 0.6400 | 0.6379 | 1.2752 | orjson | 0.812x |
| b32d398-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3475 | 1.2300 | 0.3990 | 0.3636 | 0.7313 | orjson | 0.956x |
| b32d398-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3331 | 1.2385 | 0.4081 | 0.3751 | 0.7592 | orjson | 0.888x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +39%, msgspec +60%, orjson +75%, ujson +74%)

## macos-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3349 | 3.5706 | 0.5578 | 0.4202 | 2.1631 | orjson | 0.797x |
| ec53f93-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3347 | 3.6136 | 0.5444 | 0.4165 | 2.1680 | orjson | 0.804x |
| b32d398-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3102 | 3.4540 | 0.5020 | 0.3874 | 2.0806 | orjson | 0.801x |
| b32d398-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3043 | 3.4256 | 0.4969 | 0.3703 | 2.0946 | orjson | 0.822x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +5%, msgspec +12%, orjson +13%, ujson +4%)

## macos-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.6319 | 41.5308 | 5.2860 | 3.4329 | 25.2994 | orjson | 0.767x |
| ec53f93-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.9228 | 45.4115 | 5.9246 | 3.7004 | 26.5094 | orjson | 0.790x |
| b32d398-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4097 | 39.5925 | 5.0060 | 3.2094 | 23.4716 | orjson | 0.751x |
| b32d398-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4881 | 38.9337 | 4.9301 | 3.1624 | 23.2115 | orjson | 0.787x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +17%, msgspec +20%, orjson +17%, ujson +14%)

## macos-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.4637 | 1.2052 | 0.6908 | 0.7435 | 4.2069 | 0.7160 | msgspec | 0.671x |
| ec53f93-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3568 | 0.9406 | 0.4821 | 0.4565 | 3.2418 | 0.6550 | orjson | 0.782x |
| b32d398-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3294 | 0.8379 | 0.4444 | 0.4127 | 3.0514 | 0.5981 | orjson | 0.798x |
| b32d398-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3264 | 0.8312 | 0.4335 | 0.4091 | 3.0447 | 0.5913 | orjson | 0.798x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +45%, msgspec +59%, orjson +82%, pysimdjson +38%, ujson +21%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0755 | 0.5187 | 0.0958 | 0.0710 | 0.2691 | orjson | 1.063x |
| ec53f93-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0747 | 0.5458 | 0.0913 | 0.0694 | 0.2685 | orjson | 1.076x |
| b32d398-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0787 | 0.5660 | 0.1000 | 0.0734 | 0.2780 | orjson | 1.074x |
| b32d398-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0733 | 0.5187 | 0.0936 | 0.0697 | 0.2692 | orjson | 1.052x |

verdict: **agree** — every shared rival within tolerance (json 9.1%, msgspec 9.6%, orjson 5.7%, ujson 3.5%)

## windows-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4153 | 1.0530 | 0.4345 | 0.4119 | 0.7667 | orjson | 1.008x |
| ec53f93-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3857 | 1.0526 | 0.4075 | 0.3802 | 0.7581 | orjson | 1.014x |
| b32d398-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4071 | 1.0260 | 0.4179 | 0.4138 | 0.7879 | orjson | 0.984x |
| b32d398-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4515 | 1.0454 | 0.4234 | 0.3862 | 0.8010 | orjson | 1.169x |

verdict: **agree** — every shared rival within tolerance (json 2.6%, msgspec 6.6%, orjson 8.8%, ujson 5.7%)

## windows-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3168 | 1.9273 | 0.5073 | 0.3626 | 1.5546 | orjson | 0.874x |
| ec53f93-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3122 | 1.9794 | 0.4911 | 0.3575 | 1.5624 | orjson | 0.873x |
| b32d398-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3185 | 2.1510 | 0.5149 | 0.3683 | 1.5214 | orjson | 0.865x |
| b32d398-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3285 | 2.0815 | 0.5194 | 0.3560 | 1.5446 | orjson | 0.923x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +12%, msgspec +6%, orjson +3%, ujson +3%)

## windows-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.4284 | 28.9898 | 6.0036 | 4.3675 | 16.6711 | orjson | 0.785x |
| ec53f93-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2303 | 26.1338 | 5.4615 | 3.7783 | 14.4462 | orjson | 0.855x |
| b32d398-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.0680 | 24.6889 | 5.1088 | 3.5659 | 14.0480 | orjson | 0.860x |
| b32d398-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.1046 | 23.6918 | 5.2227 | 3.8659 | 14.2538 | orjson | 0.803x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +22%, msgspec +18%, orjson +22%, ujson +19%)

## windows-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| ec53f93-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1919 | 0.4780 | 0.2423 | 0.2213 | 0.3855 | orjson | 0.867x |
| ec53f93-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1847 | 0.4815 | 0.2404 | 0.2157 | 0.3535 | orjson | 0.856x |
| b32d398-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1903 | 0.4912 | 0.2476 | 0.2217 | 0.3691 | orjson | 0.859x |
| b32d398-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1847 | 0.4912 | 0.2407 | 0.2164 | 0.3774 | orjson | 0.854x |

verdict: **agree** — every shared rival within tolerance (json 2.8%, msgspec 3.0%, orjson 2.7%, ujson 9.1%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

- windows-x86_64 dumps mixed.json: 1.063x, 1.076x, 1.074x, 1.052x (rivals held)

### Rank changes between draws

- linux-x86_64 dumps mixed.json: 0.909x, 0.957x, 1.047x, 1.011x (rivals moved 155%)
- macos-arm64 dump mixed.json: 0.886x, 0.940x, 0.864x, 1.041x (rivals moved 100%)
- macos-arm64 dumps flat.json: 0.638x, 1.008x, 0.834x, 0.817x (rivals moved 87%)
- windows-x86_64 dump mixed.json: 1.008x, 1.014x, 0.984x, 1.169x (rivals held)

### Ahead on every draw

- linux-arm64 dumps mixed.json: 0.944x, 0.987x, 0.966x, 0.986x (rivals moved 33%)
- linux-arm64 dump mixed.json: 0.898x, 0.899x, 0.896x, 0.850x (rivals moved 47%)
- linux-arm64 dumps flat.json: 0.804x, 0.807x, 0.792x, 0.832x (rivals moved 12%)
- linux-arm64 dumps users.json: 0.750x, 0.751x, 0.745x, 0.747x (rivals held)
- linux-arm64 loads mixed.json: 0.837x, 0.891x, 0.878x, 0.873x (rivals moved 13%)
- linux-x86_64 dump mixed.json: 0.949x, 0.870x, 0.926x, 0.940x (rivals moved 108%)
- linux-x86_64 dumps flat.json: 0.843x, 0.744x, 0.979x, 0.997x (rivals moved 57%)
- linux-x86_64 dumps users.json: 0.765x, 0.927x, 0.909x, 0.908x (rivals moved 55%)
- linux-x86_64 loads mixed.json: 0.804x, 0.906x, 0.810x, 0.818x (rivals moved 65%)
- macos-arm64 dumps mixed.json: 0.775x, 0.881x, 0.774x, 0.807x (rivals moved 47%)
- macos-arm64 dumps users.json: 0.689x, 0.681x, 0.636x, 0.641x (rivals moved 53%)
- macos-arm64 loads mixed.json: 0.754x, 0.879x, 0.773x, 0.785x (rivals moved 105%)
- macos-x86_64 dumps mixed.json: 0.823x, 0.797x, 0.817x, 0.812x (rivals moved 83%)
- macos-x86_64 dump mixed.json: 0.904x, 0.812x, 0.956x, 0.888x (rivals moved 75%)
- macos-x86_64 dumps flat.json: 0.797x, 0.804x, 0.801x, 0.822x (rivals moved 13%)
- macos-x86_64 dumps users.json: 0.767x, 0.790x, 0.751x, 0.787x (rivals moved 20%)
- macos-x86_64 loads mixed.json: 0.671x, 0.782x, 0.798x, 0.798x (rivals moved 82%)
- windows-x86_64 dumps flat.json: 0.874x, 0.873x, 0.865x, 0.923x (rivals moved 12%)
- windows-x86_64 dumps users.json: 0.785x, 0.855x, 0.860x, 0.803x (rivals moved 22%)
- windows-x86_64 loads mixed.json: 0.867x, 0.856x, 0.859x, 0.854x (rivals held)

