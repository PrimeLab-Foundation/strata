# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `32c5fa4` — build/evidence/benchmark-lead/history/32c5fa4/ci
- `79fa3df` — build/evidence/benchmark-lead/history/79fa3df/ci
- `75cfb42` — build/evidence/benchmark-lead/history/75cfb42/ci
- `c20ac86` — build/evidence/benchmark-lead/history/c20ac86/ci
- `ec53f93` — build/evidence/benchmark-lead/history/ec53f93/ci
- `b32d398` — build/evidence/benchmark-lead/history/b32d398/ci
- `ec04112` — build/evidence/benchmark-lead/history/ec04112/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 0.0670 | 0.4850 | 0.0790 | 0.0650 | 0.2450 | orjson | 1.031x |
| 79fa3df | aarch64 | 0.0680 | 0.4900 | 0.0800 | 0.0670 | 0.2390 | orjson | 1.015x |
| 75cfb42 | aarch64 | 0.0662 | 0.4807 | 0.0784 | 0.0647 | 0.2436 | orjson | 1.023x |
| c20ac86 | aarch64 | 0.0598 | 0.4828 | 0.0775 | 0.0635 | 0.2381 | orjson | 0.943x |
| ec53f93 | aarch64 | 0.0660 | 0.4993 | 0.0807 | 0.0669 | 0.2443 | orjson | 0.987x |
| b32d398 | aarch64 | 0.0726 | 0.5281 | 0.0897 | 0.0736 | 0.2524 | orjson | 0.986x |
| ec04112 | aarch64 | 0.0654 | 0.5012 | 0.0804 | 0.0663 | 0.2496 | orjson | 0.987x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +10%, msgspec +16%, orjson +16%, ujson +6%)

## linux-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 0.1860 | 0.6490 | 0.2150 | 0.2120 | 0.4020 | orjson | 0.877x |
| 79fa3df | aarch64 | 0.1870 | 0.6200 | 0.2240 | 0.1930 | 0.3850 | orjson | 0.969x |
| 75cfb42 | aarch64 | 0.1901 | 0.6465 | 0.2278 | 0.2072 | 0.4142 | orjson | 0.918x |
| c20ac86 | aarch64 | 0.1723 | 0.6367 | 0.2089 | 0.1970 | 0.3841 | orjson | 0.875x |
| ec53f93 | aarch64 | 0.1927 | 0.6588 | 0.2317 | 0.2143 | 0.4127 | orjson | 0.899x |
| b32d398 | aarch64 | 0.2362 | 0.7370 | 0.2927 | 0.2778 | 0.4850 | orjson | 0.850x |
| ec04112 | aarch64 | 0.1823 | 0.6470 | 0.2138 | 0.2031 | 0.4075 | orjson | 0.898x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +19%, msgspec +40%, orjson +44%, ujson +26%)

## linux-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 0.2310 | 1.7140 | 0.3880 | 0.3000 | 0.9910 | orjson | 0.770x |
| 79fa3df | aarch64 | 0.2420 | 1.7060 | 0.3890 | 0.3100 | 1.0060 | orjson | 0.781x |
| 75cfb42 | aarch64 | 0.2392 | 1.6973 | 0.3869 | 0.3063 | 1.0044 | orjson | 0.781x |
| c20ac86 | aarch64 | 0.2357 | 1.6981 | 0.3870 | 0.3024 | 0.9899 | orjson | 0.779x |
| ec53f93 | aarch64 | 0.2405 | 1.6954 | 0.3887 | 0.2979 | 0.9819 | orjson | 0.807x |
| b32d398 | aarch64 | 0.2734 | 1.8015 | 0.4297 | 0.3286 | 1.0663 | orjson | 0.832x |
| ec04112 | aarch64 | 0.2365 | 1.7142 | 0.4051 | 0.3031 | 0.9999 | orjson | 0.780x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +6%, msgspec +11%, orjson +10%, ujson +9%)

## linux-arm64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 0.4160 | 1.9450 | 0.5900 | 0.5100 | 1.2200 | orjson | 0.816x |
| 79fa3df | aarch64 | 0.4040 | 1.9140 | 0.5860 | 0.5020 | 1.2120 | orjson | 0.805x |
| 75cfb42 | aarch64 | 0.3958 | 1.9058 | 0.5975 | 0.4974 | 1.2165 | orjson | 0.796x |
| c20ac86 | aarch64 | 0.3923 | 1.9106 | 0.5810 | 0.4833 | 1.1936 | orjson | 0.812x |
| ec53f93 | aarch64 | 0.4370 | 1.9336 | 0.5963 | 0.5048 | 1.2272 | orjson | 0.866x |
| b32d398 | aarch64 | 0.5523 | 2.1461 | 0.7559 | 0.6465 | 1.4076 | orjson | 0.854x |
| ec04112 | aarch64 | 0.4096 | 1.9401 | 0.6048 | 0.5089 | 1.2251 | orjson | 0.805x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +13%, msgspec +30%, orjson +34%, ujson +18%)

## linux-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 2.0490 | 18.9950 | 3.3150 | 2.5830 | 10.5200 | orjson | 0.793x |
| 79fa3df | aarch64 | 2.0030 | 19.1490 | 3.3380 | 2.5920 | 10.5560 | orjson | 0.773x |
| 75cfb42 | aarch64 | 2.0286 | 18.9701 | 3.3098 | 2.5901 | 10.5474 | orjson | 0.783x |
| c20ac86 | aarch64 | 1.9370 | 18.9494 | 3.3126 | 2.5885 | 10.5192 | orjson | 0.748x |
| ec53f93 | aarch64 | 1.9495 | 19.1384 | 3.3288 | 2.5971 | 10.5762 | orjson | 0.751x |
| b32d398 | aarch64 | 1.9660 | 19.2032 | 3.3648 | 2.6305 | 10.6977 | orjson | 0.747x |
| ec04112 | aarch64 | 1.9376 | 19.0483 | 3.3270 | 2.6069 | 10.6090 | orjson | 0.743x |

verdict: **agree** — every shared rival within tolerance (json 1.3%, msgspec 1.7%, orjson 1.8%, ujson 1.7%)

## linux-arm64 — loads wide_arrays.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | aarch64 | 3.8450 | 9.5590 | 5.0360 | 4.0790 | 5.2570 | 6.5140 | orjson | 0.943x |
| 79fa3df | aarch64 | 3.8320 | 9.5240 | 5.0470 | 4.0710 | 5.2670 | 6.4550 | orjson | 0.941x |
| 75cfb42 | aarch64 | 3.8442 | 9.4963 | 5.0973 | 4.0966 | 5.3018 | 6.5391 | orjson | 0.938x |
| c20ac86 | aarch64 | 3.8271 | 9.4966 | 5.0817 | 4.0659 | 5.2657 | 6.4854 | orjson | 0.941x |
| ec53f93 | aarch64 | 3.9335 | 9.6372 | 5.1110 | 4.1498 | 5.3560 | 6.5743 | orjson | 0.948x |
| b32d398 | aarch64 | 4.5339 | 10.4199 | 5.8778 | 4.9651 | 6.2205 | 7.2997 | orjson | 0.913x |
| ec04112 | aarch64 | 3.9359 | 9.6177 | 5.1236 | 4.1292 | 5.3472 | 6.6131 | orjson | 0.953x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +10%, msgspec +17%, orjson +22%, pysimdjson +18%, ujson +13%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 0.0630 | 0.5170 | 0.0860 | 0.0660 | 0.2320 | orjson | 0.955x |
| 79fa3df | x86_64 | 0.0550 | 0.5020 | 0.0680 | 0.0510 | 0.2060 | orjson | 1.078x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 0.0648 | 0.5265 | 0.0892 | 0.0699 | 0.2311 | orjson | 0.927x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 0.0534 | 0.4167 | 0.0721 | 0.0501 | 0.1784 | orjson | 1.066x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.0920 | 0.5778 | 0.0962 | 0.1222 | 0.2462 | msgspec | 0.957x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 0.0483 | 0.4082 | 0.0680 | 0.0478 | 0.1784 | orjson | 1.011x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 0.0507 | 0.4477 | 0.0724 | 0.0520 | 0.2067 | orjson | 0.976x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +42%, msgspec +41%, orjson +155%, ujson +38%)

## linux-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 0.1530 | 0.6370 | 0.1910 | 0.1700 | 0.3510 | orjson | 0.900x |
| 79fa3df | x86_64 | 0.1190 | 0.5670 | 0.1400 | 0.1250 | 0.2890 | orjson | 0.952x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 0.1613 | 0.6411 | 0.2000 | 0.1862 | 0.3545 | orjson | 0.866x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 0.1244 | 0.5034 | 0.1499 | 0.1305 | 0.2741 | orjson | 0.953x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2074 | 0.7317 | 0.2383 | 0.2569 | 0.4031 | msgspec | 0.870x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 0.1161 | 0.4941 | 0.1424 | 0.1236 | 0.2646 | orjson | 0.940x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 0.1089 | 0.5294 | 0.1412 | 0.1231 | 0.2876 | orjson | 0.885x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +48%, msgspec +70%, orjson +109%, ujson +52%)

## linux-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 0.2620 | 1.8610 | 0.4370 | 0.3380 | 1.0370 | orjson | 0.775x |
| 79fa3df | x86_64 | 0.2500 | 1.6170 | 0.3840 | 0.2460 | 0.9110 | orjson | 1.016x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 0.3201 | 1.8742 | 0.4867 | 0.3681 | 1.0407 | orjson | 0.870x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 0.2601 | 1.5320 | 0.3985 | 0.2746 | 0.8362 | orjson | 0.947x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2900 | 2.0158 | 0.4933 | 0.3899 | 1.0524 | orjson | 0.744x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 0.2474 | 1.4557 | 0.3742 | 0.2481 | 0.8314 | orjson | 0.997x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 0.2416 | 1.5694 | 0.3805 | 0.2454 | 0.8975 | orjson | 0.984x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +38%, msgspec +32%, orjson +59%, ujson +27%)

## linux-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 0.4210 | 2.0400 | 0.6220 | 0.4950 | 1.2070 | orjson | 0.851x |
| 79fa3df | x86_64 | 0.3460 | 1.7200 | 0.4940 | 0.3530 | 1.0290 | orjson | 0.980x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 0.4787 | 2.0692 | 0.6591 | 0.5410 | 1.2214 | orjson | 0.885x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 0.5528 | 1.8691 | 0.7668 | 0.5559 | 1.1709 | orjson | 0.995x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.4634 | 2.2158 | 0.6816 | 0.5122 | 1.2498 | orjson | 0.905x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 0.3630 | 1.5977 | 0.5008 | 0.3655 | 0.9516 | orjson | 0.993x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 0.3458 | 1.6986 | 0.4911 | 0.3559 | 1.0246 | orjson | 0.972x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +39%, msgspec +56%, orjson +57%, ujson +31%)

## linux-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 2.4220 | 21.6430 | 3.8490 | 2.9100 | 11.3930 | orjson | 0.832x |
| 79fa3df | x86_64 | 1.7910 | 18.0070 | 3.4070 | 2.0640 | 9.8860 | orjson | 0.868x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 2.4483 | 21.5982 | 4.1418 | 3.1315 | 11.5085 | orjson | 0.782x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 2.0336 | 17.3147 | 3.3568 | 2.1176 | 9.1658 | orjson | 0.960x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 2.3232 | 22.7149 | 4.0307 | 2.5069 | 11.8257 | orjson | 0.927x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 1.8151 | 16.6293 | 3.2294 | 1.9996 | 8.8052 | orjson | 0.908x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 1.7005 | 17.9164 | 3.3109 | 1.9558 | 9.8780 | orjson | 0.869x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +37%, msgspec +28%, orjson +60%, ujson +34%)

## linux-x86_64 — loads wide_arrays.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | x86_64 | 4.0860 | 9.6930 | 5.7300 | 5.1350 | 6.1210 | 7.0750 | orjson | 0.796x |
| 79fa3df | x86_64 | 3.3170 | 8.0740 | 4.7560 | 4.4850 | 4.8060 | 5.8240 | orjson | 0.740x |
| 75cfb42 | AMD EPYC 9V74 80-Core Processor | 4.4595 | 9.9234 | 6.1992 | 5.6338 | 6.5280 | 7.6546 | orjson | 0.792x |
| c20ac86 | AMD EPYC 9V74 80-Core Processor | 3.4986 | 7.9820 | 4.8928 | 4.5093 | 5.1325 | 6.0975 | orjson | 0.776x |
| ec53f93 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 4.4404 | 10.4006 | 5.9862 | 5.6246 | 6.2368 | 7.5766 | orjson | 0.789x |
| b32d398 | AMD EPYC 9V74 80-Core Processor | 3.5031 | 7.7107 | 4.7937 | 4.3731 | 5.0854 | 5.9754 | orjson | 0.801x |
| ec04112 | INTEL(R) XEON(R) PLATINUM 8573C | 3.3929 | 8.1643 | 4.9282 | 4.7208 | 4.9355 | 6.0508 | orjson | 0.719x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +35%, msgspec +30%, orjson +29%, pysimdjson +36%, ujson +31%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 0.0450 | 0.3790 | 0.1480 | 0.0550 | 0.1820 | orjson | 0.818x |
| 79fa3df | Apple M1 (Virtual) | 0.0380 | 0.3600 | 0.0610 | 0.0460 | 0.1840 | orjson | 0.826x |
| 75cfb42 | Apple M1 (Virtual) | 0.0523 | 0.3954 | 0.1792 | 0.0602 | 0.1978 | orjson | 0.869x |
| c20ac86 | Apple M2 Pro (Virtual) | 0.0303 | 0.3186 | 0.0437 | 0.0385 | 0.1515 | orjson | 0.787x |
| ec53f93 | Apple M1 (Virtual) | 0.0575 | 0.4780 | 0.0735 | 0.0653 | 0.2408 | orjson | 0.881x |
| b32d398 | Apple M1 (Virtual) | 0.0382 | 0.3559 | 0.0559 | 0.0473 | 0.1754 | orjson | 0.807x |
| ec04112 | Apple M1 (Virtual) | 0.0480 | 0.3881 | 0.0663 | 0.0573 | 0.1871 | orjson | 0.837x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +50%, msgspec +310%, orjson +70%, ujson +59%)

## macos-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 0.3520 | 0.6640 | 0.3680 | 0.3950 | 0.4890 | msgspec | 0.957x |
| 79fa3df | Apple M1 (Virtual) | 0.1990 | 0.5720 | 0.2110 | 0.2090 | 0.3740 | orjson | 0.952x |
| 75cfb42 | Apple M1 (Virtual) | 0.2458 | 0.6008 | 0.3557 | 0.2750 | 0.4113 | orjson | 0.894x |
| c20ac86 | Apple M2 Pro (Virtual) | 0.0905 | 0.3765 | 0.1099 | 0.1054 | 0.2228 | orjson | 0.858x |
| ec53f93 | Apple M1 (Virtual) | 0.2813 | 0.6992 | 0.3660 | 0.2991 | 0.5163 | orjson | 0.940x |
| b32d398 | Apple M1 (Virtual) | 0.2251 | 0.6003 | 0.2162 | 0.2170 | 0.3500 | msgspec | 1.041x |
| ec04112 | Apple M1 (Virtual) | 0.2556 | 0.6617 | 0.2928 | 0.2704 | 0.4535 | orjson | 0.945x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +86%, msgspec +235%, orjson +275%, ujson +132%)

## macos-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 0.2900 | 1.6950 | 0.3790 | 0.3080 | 0.9070 | orjson | 0.942x |
| 79fa3df | Apple M1 (Virtual) | 0.1990 | 1.3210 | 0.2910 | 0.2340 | 0.7320 | orjson | 0.850x |
| 75cfb42 | Apple M1 (Virtual) | 0.2613 | 1.5835 | 0.3737 | 0.2967 | 0.8543 | orjson | 0.880x |
| c20ac86 | Apple M2 Pro (Virtual) | 0.1691 | 1.2014 | 0.2609 | 0.2084 | 0.6191 | orjson | 0.811x |
| ec53f93 | Apple M1 (Virtual) | 0.2924 | 1.9929 | 0.3797 | 0.2902 | 0.8992 | orjson | 1.008x |
| b32d398 | Apple M1 (Virtual) | 0.2021 | 1.4400 | 0.3144 | 0.2473 | 0.7842 | orjson | 0.817x |
| ec04112 | Apple M1 (Virtual) | 0.2578 | 1.7154 | 0.3650 | 0.2897 | 0.8783 | orjson | 0.890x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +66%, msgspec +46%, orjson +48%, ujson +46%)

## macos-arm64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 0.5720 | 2.1000 | 0.6560 | 0.5870 | 1.2020 | orjson | 0.974x |
| 79fa3df | Apple M1 (Virtual) | 0.3260 | 1.5030 | 0.4220 | 0.3540 | 0.8420 | orjson | 0.921x |
| 75cfb42 | Apple M1 (Virtual) | 0.4123 | 1.6389 | 0.5000 | 0.4388 | 0.9882 | orjson | 0.940x |
| c20ac86 | Apple M2 Pro (Virtual) | 0.2553 | 1.3283 | 0.3392 | 0.2808 | 0.7115 | orjson | 0.909x |
| ec53f93 | Apple M1 (Virtual) | 0.6612 | 1.9713 | 0.7191 | 0.7217 | 1.2339 | msgspec | 0.920x |
| b32d398 | Apple M1 (Virtual) | 0.4472 | 1.7128 | 0.5301 | 0.4931 | 1.0184 | orjson | 0.907x |
| ec04112 | Apple M1 (Virtual) | 0.5646 | 1.7826 | 0.6044 | 0.5196 | 1.1937 | orjson | 1.086x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +58%, msgspec +112%, orjson +157%, ujson +73%)

## macos-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 1.7670 | 16.4550 | 3.2150 | 2.4130 | 9.3670 | orjson | 0.732x |
| 79fa3df | Apple M1 (Virtual) | 1.4140 | 14.8900 | 2.8530 | 2.1420 | 8.3060 | orjson | 0.660x |
| 75cfb42 | Apple M1 (Virtual) | 1.6121 | 16.4371 | 3.1987 | 2.5599 | 9.3179 | orjson | 0.630x |
| c20ac86 | Apple M2 Pro (Virtual) | 1.3328 | 14.5287 | 2.4741 | 1.9353 | 7.6262 | orjson | 0.689x |
| ec53f93 | Apple M1 (Virtual) | 1.7750 | 17.5613 | 3.2108 | 2.6068 | 9.7466 | orjson | 0.681x |
| b32d398 | Apple M1 (Virtual) | 1.6530 | 16.9275 | 3.2160 | 2.5801 | 9.9289 | orjson | 0.641x |
| ec04112 | Apple M1 (Virtual) | 1.5549 | 16.1811 | 3.0152 | 2.3752 | 9.3380 | orjson | 0.655x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +21%, msgspec +30%, orjson +35%, ujson +30%)

## macos-arm64 — loads wide_arrays.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Apple M1 (Virtual) | 3.2220 | 7.3210 | 4.4420 | 4.0360 | 65.7540 | 5.7390 | orjson | 0.798x |
| 79fa3df | Apple M1 (Virtual) | 2.8050 | 6.4240 | 3.8290 | 3.3430 | 59.9260 | 5.0300 | orjson | 0.839x |
| 75cfb42 | Apple M1 (Virtual) | 3.1230 | 7.1654 | 4.2524 | 3.8555 | 65.0937 | 5.5125 | orjson | 0.810x |
| c20ac86 | Apple M2 Pro (Virtual) | 2.4998 | 5.5041 | 3.3735 | 2.8949 | 58.2108 | 4.3768 | orjson | 0.864x |
| ec53f93 | Apple M1 (Virtual) | 3.7496 | 8.0687 | 4.8491 | 4.5137 | 72.0613 | 6.3636 | orjson | 0.831x |
| b32d398 | Apple M1 (Virtual) | 3.0690 | 6.7934 | 4.1033 | 3.7024 | 63.8362 | 5.2815 | orjson | 0.829x |
| ec04112 | Apple M1 (Virtual) | 3.7660 | 8.4031 | 5.0601 | 4.7325 | 74.2813 | 6.5366 | orjson | 0.796x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +53%, msgspec +50%, orjson +63%, pysimdjson +28%, ujson +49%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0760 | 1.0740 | 0.1210 | 0.0880 | 0.4910 | orjson | 0.864x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0660 | 0.9620 | 0.1060 | 0.0770 | 0.4590 | orjson | 0.857x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0672 | 0.9888 | 0.1129 | 0.0812 | 0.4678 | orjson | 0.828x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0820 | 1.0949 | 0.1475 | 0.1151 | 0.5529 | orjson | 0.713x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1088 | 1.3851 | 0.1848 | 0.1366 | 0.6110 | orjson | 0.797x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0609 | 0.9067 | 0.1037 | 0.0750 | 0.4312 | orjson | 0.812x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1503 | 1.7856 | 0.2604 | 0.1622 | 0.7844 | orjson | 0.927x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +97%, msgspec +151%, orjson +116%, ujson +82%)

## macos-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3450 | 1.2920 | 0.4130 | 0.3940 | 0.7880 | orjson | 0.876x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3390 | 1.3150 | 0.3770 | 0.3560 | 0.7960 | orjson | 0.952x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3519 | 1.3374 | 0.4252 | 0.4049 | 0.7976 | orjson | 0.869x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3681 | 1.2733 | 0.4325 | 0.4016 | 0.7730 | orjson | 0.917x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.5177 | 1.7126 | 0.6400 | 0.6379 | 1.2752 | orjson | 0.812x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3331 | 1.2385 | 0.4081 | 0.3751 | 0.7592 | orjson | 0.888x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6165 | 2.1763 | 0.7219 | 0.7245 | 1.2430 | msgspec | 0.854x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +76%, msgspec +91%, orjson +103%, ujson +68%)

## macos-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3130 | 3.6870 | 0.5360 | 0.4060 | 2.2150 | orjson | 0.771x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3920 | 4.3680 | 0.6180 | 0.4750 | 2.6380 | orjson | 0.825x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3191 | 3.6746 | 0.5319 | 0.3967 | 2.2147 | orjson | 0.804x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3360 | 3.4960 | 0.5545 | 0.4109 | 2.1475 | orjson | 0.818x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3347 | 3.6136 | 0.5444 | 0.4165 | 2.1680 | orjson | 0.804x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3043 | 3.4256 | 0.4969 | 0.3703 | 2.0946 | orjson | 0.822x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.5141 | 5.7852 | 1.0103 | 0.7191 | 3.7338 | orjson | 0.715x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +69%, msgspec +103%, orjson +94%, ujson +78%)

## macos-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7270 | 4.2520 | 1.0080 | 0.8410 | 2.7340 | orjson | 0.864x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.8190 | 4.7650 | 1.0510 | 0.8870 | 3.0890 | orjson | 0.923x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7227 | 4.2648 | 0.9743 | 0.8239 | 2.7305 | orjson | 0.877x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6923 | 3.9933 | 0.9568 | 0.7862 | 2.5241 | orjson | 0.880x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7118 | 3.9272 | 0.9046 | 0.7914 | 2.5338 | orjson | 0.899x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6577 | 3.8768 | 0.8731 | 0.7446 | 2.5008 | orjson | 0.883x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 1.0446 | 6.8571 | 1.5514 | 1.2581 | 4.2200 | orjson | 0.830x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +77%, msgspec +78%, orjson +69%, ujson +69%)

## macos-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4400 | 41.5220 | 5.8030 | 3.2540 | 24.3410 | orjson | 0.750x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.9590 | 45.9410 | 5.5530 | 3.9520 | 26.9370 | orjson | 0.749x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.6334 | 43.1033 | 5.3087 | 3.4182 | 25.7159 | orjson | 0.770x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.6745 | 40.1802 | 5.2481 | 3.3976 | 23.6676 | orjson | 0.787x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.9228 | 45.4115 | 5.9246 | 3.7004 | 26.5094 | orjson | 0.790x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4881 | 38.9337 | 4.9301 | 3.1624 | 23.2115 | orjson | 0.787x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 4.3822 | 63.8467 | 7.9782 | 5.3508 | 38.0820 | orjson | 0.819x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +64%, msgspec +62%, orjson +69%, ujson +64%)

## macos-x86_64 — loads wide_arrays.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 7.6470 | 17.9840 | 10.6330 | 9.7220 | 84.0680 | 13.4710 | orjson | 0.787x |
| 79fa3df | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 7.8020 | 18.2920 | 10.6940 | 9.9130 | 86.1420 | 13.4980 | orjson | 0.787x |
| 75cfb42 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 7.5887 | 17.1680 | 10.5556 | 9.5102 | 80.1170 | 13.1379 | orjson | 0.798x |
| c20ac86 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 7.2286 | 16.6003 | 10.1725 | 9.5722 | 76.0135 | 12.8878 | orjson | 0.755x |
| ec53f93 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 7.8245 | 17.2153 | 10.9484 | 10.3179 | 80.4140 | 14.5441 | orjson | 0.758x |
| b32d398 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 6.9155 | 16.0463 | 9.9152 | 8.8707 | 75.0300 | 12.2478 | orjson | 0.780x |
| ec04112 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 11.7075 | 26.4256 | 15.4416 | 15.7545 | 121.7287 | 21.2250 | msgspec | 0.758x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +65%, msgspec +56%, orjson +78%, pysimdjson +62%, ujson +73%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 0.0690 | 0.5610 | 0.0960 | 0.0700 | 0.2730 | orjson | 0.986x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0790 | 0.5390 | 0.1010 | 0.0730 | 0.2740 | orjson | 1.082x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.0762 | 0.5272 | 0.1018 | 0.0751 | 0.2621 | orjson | 1.015x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0741 | 0.5258 | 0.0967 | 0.0720 | 0.2865 | orjson | 1.029x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0747 | 0.5458 | 0.0913 | 0.0694 | 0.2685 | orjson | 1.076x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0733 | 0.5187 | 0.0936 | 0.0697 | 0.2692 | orjson | 1.052x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0765 | 0.5342 | 0.1024 | 0.0721 | 0.2678 | orjson | 1.060x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +8%, msgspec +12%, orjson +8%, ujson +9%)

## windows-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 0.3830 | 0.9960 | 0.4240 | 0.3870 | 0.7110 | orjson | 0.990x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4650 | 1.0590 | 0.4680 | 0.4400 | 0.8150 | orjson | 1.057x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.4037 | 1.0599 | 0.4315 | 0.4057 | 0.7676 | orjson | 0.995x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4067 | 1.0509 | 0.4405 | 0.3954 | 0.7924 | orjson | 1.028x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3857 | 1.0526 | 0.4075 | 0.3802 | 0.7581 | orjson | 1.014x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4515 | 1.0454 | 0.4234 | 0.3862 | 0.8010 | orjson | 1.169x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4494 | 1.0969 | 0.4441 | 0.4266 | 0.8351 | orjson | 1.054x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +10%, msgspec +15%, orjson +16%, ujson +17%)

## windows-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 0.2920 | 1.8390 | 0.5330 | 0.3660 | 1.1500 | orjson | 0.798x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3200 | 2.0290 | 0.5120 | 0.3720 | 1.5800 | orjson | 0.860x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.3466 | 1.9768 | 0.5597 | 0.3832 | 1.4600 | orjson | 0.904x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3212 | 2.0070 | 0.5191 | 0.3626 | 1.4613 | orjson | 0.886x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3122 | 1.9794 | 0.4911 | 0.3575 | 1.5624 | orjson | 0.873x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3285 | 2.0815 | 0.5194 | 0.3560 | 1.5446 | orjson | 0.923x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3356 | 2.0194 | 0.5445 | 0.3992 | 1.4677 | orjson | 0.841x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +13%, msgspec +14%, orjson +12%, ujson +37%)

## windows-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 0.7180 | 3.2040 | 0.9400 | 0.7470 | 2.3600 | orjson | 0.961x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.7140 | 3.3830 | 0.9030 | 0.7370 | 2.8410 | orjson | 0.969x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 0.7228 | 3.3629 | 0.9302 | 0.7662 | 2.8654 | orjson | 0.943x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.7254 | 3.4463 | 0.9202 | 0.7913 | 2.8797 | orjson | 0.917x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.6947 | 3.3736 | 0.8738 | 0.7043 | 2.8440 | orjson | 0.986x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.6957 | 3.5168 | 0.8957 | 0.7549 | 2.9371 | orjson | 0.922x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.8545 | 3.5508 | 1.0056 | 0.8160 | 3.0740 | orjson | 1.047x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +11%, msgspec +15%, orjson +16%, ujson +30%)

## windows-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 2.6800 | 23.0970 | 5.9870 | 3.4900 | 12.9600 | orjson | 0.768x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.6730 | 28.2200 | 6.1370 | 4.8060 | 17.7720 | orjson | 0.764x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 3.2645 | 23.5123 | 5.5994 | 3.8357 | 13.1011 | orjson | 0.851x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2035 | 24.5043 | 5.1669 | 3.7000 | 14.2696 | orjson | 0.866x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2303 | 26.1338 | 5.4615 | 3.7783 | 14.4462 | orjson | 0.855x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.1046 | 23.6918 | 5.2227 | 3.8659 | 14.2538 | orjson | 0.803x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2178 | 23.6124 | 5.2935 | 3.9101 | 14.2789 | orjson | 0.823x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +22%, msgspec +19%, orjson +38%, ujson +37%)

## windows-x86_64 — loads wide_arrays.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| 32c5fa4 | Intel64 Family 6 Model 207 Stepping 2, GenuineIntel | 3.6910 | 11.4010 | 5.8040 | 6.3230 | 7.8950 | msgspec | 0.636x |
| 79fa3df | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 4.3520 | 12.3560 | 5.9780 | 6.1730 | 8.7450 | msgspec | 0.728x |
| 75cfb42 | AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD | 4.4905 | 11.5008 | 5.9312 | 6.0335 | 8.0836 | msgspec | 0.757x |
| c20ac86 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 4.4049 | 12.1573 | 6.0158 | 6.2370 | 8.6199 | msgspec | 0.732x |
| ec53f93 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 4.1963 | 11.7730 | 5.9431 | 5.6924 | 8.3111 | orjson | 0.737x |
| b32d398 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 4.2388 | 11.8334 | 5.8110 | 5.6826 | 8.2658 | orjson | 0.746x |
| ec04112 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 5.6119 | 13.6207 | 7.3479 | 7.1639 | 9.7984 | orjson | 0.783x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +19%, msgspec +27%, orjson +26%, ujson +24%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

None.

### Rank changes between draws

- linux-arm64 dumps mixed.json: 1.031x, 1.015x, 1.023x, 0.943x, 0.987x, 0.986x, 0.987x (rivals moved 16%)
- linux-x86_64 dumps mixed.json: 0.955x, 1.078x, 0.927x, 1.066x, 0.957x, 1.011x, 0.976x (rivals moved 155%)
- linux-x86_64 dumps flat.json: 0.775x, 1.016x, 0.870x, 0.947x, 0.744x, 0.997x, 0.984x (rivals moved 59%)
- macos-arm64 dump mixed.json: 0.957x, 0.952x, 0.894x, 0.858x, 0.940x, 1.041x, 0.945x (rivals moved 275%)
- macos-arm64 dumps flat.json: 0.942x, 0.850x, 0.880x, 0.811x, 1.008x, 0.817x, 0.890x (rivals moved 66%)
- macos-arm64 dump flat.json: 0.974x, 0.921x, 0.940x, 0.909x, 0.920x, 0.907x, 1.086x (rivals moved 157%)
- windows-x86_64 dumps mixed.json: 0.986x, 1.082x, 1.015x, 1.029x, 1.076x, 1.052x, 1.060x (rivals moved 12%)
- windows-x86_64 dump mixed.json: 0.990x, 1.057x, 0.995x, 1.028x, 1.014x, 1.169x, 1.054x (rivals moved 17%)
- windows-x86_64 dump flat.json: 0.961x, 0.969x, 0.943x, 0.917x, 0.986x, 0.922x, 1.047x (rivals moved 30%)

### Ahead on every draw

- linux-arm64 dump mixed.json: 0.877x, 0.969x, 0.918x, 0.875x, 0.899x, 0.850x, 0.898x (rivals moved 44%)
- linux-arm64 dumps flat.json: 0.770x, 0.781x, 0.781x, 0.779x, 0.807x, 0.832x, 0.780x (rivals moved 11%)
- linux-arm64 dump flat.json: 0.816x, 0.805x, 0.796x, 0.812x, 0.866x, 0.854x, 0.805x (rivals moved 34%)
- linux-arm64 dumps users.json: 0.793x, 0.773x, 0.783x, 0.748x, 0.751x, 0.747x, 0.743x (rivals held)
- linux-arm64 loads wide_arrays.json: 0.943x, 0.941x, 0.938x, 0.941x, 0.948x, 0.913x, 0.953x (rivals moved 22%)
- linux-x86_64 dump mixed.json: 0.900x, 0.952x, 0.866x, 0.953x, 0.870x, 0.940x, 0.885x (rivals moved 109%)
- linux-x86_64 dump flat.json: 0.851x, 0.980x, 0.885x, 0.995x, 0.905x, 0.993x, 0.972x (rivals moved 57%)
- linux-x86_64 dumps users.json: 0.832x, 0.868x, 0.782x, 0.960x, 0.927x, 0.908x, 0.869x (rivals moved 60%)
- linux-x86_64 loads wide_arrays.json: 0.796x, 0.740x, 0.792x, 0.776x, 0.789x, 0.801x, 0.719x (rivals moved 36%)
- macos-arm64 dumps mixed.json: 0.818x, 0.826x, 0.869x, 0.787x, 0.881x, 0.807x, 0.837x (rivals moved 310%)
- macos-arm64 dumps users.json: 0.732x, 0.660x, 0.630x, 0.689x, 0.681x, 0.641x, 0.655x (rivals moved 35%)
- macos-arm64 loads wide_arrays.json: 0.798x, 0.839x, 0.810x, 0.864x, 0.831x, 0.829x, 0.796x (rivals moved 63%)
- macos-x86_64 dumps mixed.json: 0.864x, 0.857x, 0.828x, 0.713x, 0.797x, 0.812x, 0.927x (rivals moved 151%)
- macos-x86_64 dump mixed.json: 0.876x, 0.952x, 0.869x, 0.917x, 0.812x, 0.888x, 0.854x (rivals moved 103%)
- macos-x86_64 dumps flat.json: 0.771x, 0.825x, 0.804x, 0.818x, 0.804x, 0.822x, 0.715x (rivals moved 103%)
- macos-x86_64 dump flat.json: 0.864x, 0.923x, 0.877x, 0.880x, 0.899x, 0.883x, 0.830x (rivals moved 78%)
- macos-x86_64 dumps users.json: 0.750x, 0.749x, 0.770x, 0.787x, 0.790x, 0.787x, 0.819x (rivals moved 69%)
- macos-x86_64 loads wide_arrays.json: 0.787x, 0.787x, 0.798x, 0.755x, 0.758x, 0.780x, 0.758x (rivals moved 78%)
- windows-x86_64 dumps flat.json: 0.798x, 0.860x, 0.904x, 0.886x, 0.873x, 0.923x, 0.841x (rivals moved 37%)
- windows-x86_64 dumps users.json: 0.768x, 0.764x, 0.851x, 0.866x, 0.855x, 0.803x, 0.823x (rivals moved 38%)
- windows-x86_64 loads wide_arrays.json: 0.636x, 0.728x, 0.757x, 0.732x, 0.737x, 0.746x, 0.783x (rivals moved 27%)

