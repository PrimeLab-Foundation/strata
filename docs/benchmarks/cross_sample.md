# Cross-sample comparison

Machine-written by `make bench-cross` (`benchmarks/cross_sample.py`). Do not hand-edit.

Every median below is read from a validated report, at the full precision of its provenance companion where one exists. `verdict` says whether the draws may be compared: a rival is an unchanged binary, so a rival that moves between draws by more than 10% is measuring the draw, and the rows it appears in carry no standing.

Samples, in the order given:

- `p23-s1` — build/evidence/benchmark-lead/p23/ci-34681733282/ci
- `p23-s2` — build/evidence/benchmark-lead/p23/ci-34681741789/ci
- `p24-s1` — build/evidence/benchmark-lead/p24/ci-34693842452/ci
- `p24-s2` — build/evidence/benchmark-lead/p24/ci-34693855300/ci
- `now-s1` — build/evidence/benchmark-lead/p24/ci-34709994931/ci
- `now-s2` — build/evidence/benchmark-lead/p24/ci-34710005274/ci

## linux-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 0.0837 | 0.5340 | 0.0987 | 0.0887 | 0.2754 | orjson | 0.944x |
| p23-s2 | aarch64 | 0.0660 | 0.4993 | 0.0807 | 0.0669 | 0.2443 | orjson | 0.987x |
| p24-s1 | aarch64 | 0.0675 | 0.4946 | 0.0841 | 0.0699 | 0.2584 | orjson | 0.966x |
| p24-s2 | aarch64 | 0.0726 | 0.5281 | 0.0897 | 0.0736 | 0.2524 | orjson | 0.986x |
| now-s1 | aarch64 | 0.0710 | 0.5166 | 0.0891 | 0.0716 | 0.2543 | orjson | 0.991x |
| now-s2 | aarch64 | 0.0654 | 0.5012 | 0.0804 | 0.0663 | 0.2496 | orjson | 0.987x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +8%, msgspec +23%, orjson +34%, ujson +13%)

## linux-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 0.1702 | 0.6354 | 0.2008 | 0.1894 | 0.3927 | orjson | 0.898x |
| p23-s2 | aarch64 | 0.1927 | 0.6588 | 0.2317 | 0.2143 | 0.4127 | orjson | 0.899x |
| p24-s1 | aarch64 | 0.2142 | 0.6855 | 0.2436 | 0.2391 | 0.4375 | orjson | 0.896x |
| p24-s2 | aarch64 | 0.2362 | 0.7370 | 0.2927 | 0.2778 | 0.4850 | orjson | 0.850x |
| now-s1 | aarch64 | 0.2742 | 0.7767 | 0.3437 | 0.3081 | 0.5132 | orjson | 0.890x |
| now-s2 | aarch64 | 0.1823 | 0.6470 | 0.2138 | 0.2031 | 0.4075 | orjson | 0.898x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +22%, msgspec +71%, orjson +63%, ujson +31%)

## linux-arm64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 0.4141 | 1.9123 | 0.5905 | 0.4962 | 1.2036 | orjson | 0.835x |
| p23-s2 | aarch64 | 0.4370 | 1.9336 | 0.5963 | 0.5048 | 1.2272 | orjson | 0.866x |
| p24-s1 | aarch64 | 0.4260 | 1.9516 | 0.6130 | 0.5155 | 1.2377 | orjson | 0.826x |
| p24-s2 | aarch64 | 0.5523 | 2.1461 | 0.7559 | 0.6465 | 1.4076 | orjson | 0.854x |
| now-s1 | aarch64 | 0.5180 | 2.0679 | 0.7139 | 0.6032 | 1.3415 | orjson | 0.859x |
| now-s2 | aarch64 | 0.4096 | 1.9401 | 0.6048 | 0.5089 | 1.2251 | orjson | 0.805x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +12%, msgspec +28%, orjson +30%, ujson +17%)

## linux-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 0.2360 | 1.6798 | 0.3826 | 0.2935 | 0.9842 | orjson | 0.804x |
| p23-s2 | aarch64 | 0.2405 | 1.6954 | 0.3887 | 0.2979 | 0.9819 | orjson | 0.807x |
| p24-s1 | aarch64 | 0.2473 | 1.7094 | 0.4095 | 0.3121 | 0.9954 | orjson | 0.792x |
| p24-s2 | aarch64 | 0.2734 | 1.8015 | 0.4297 | 0.3286 | 1.0663 | orjson | 0.832x |
| now-s1 | aarch64 | 0.2484 | 1.7597 | 0.4092 | 0.3145 | 1.0204 | orjson | 0.790x |
| now-s2 | aarch64 | 0.2365 | 1.7142 | 0.4051 | 0.3031 | 0.9999 | orjson | 0.780x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +7%, msgspec +12%, orjson +12%, ujson +9%)

## linux-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 1.9253 | 18.9579 | 3.2962 | 2.5686 | 10.4808 | orjson | 0.750x |
| p23-s2 | aarch64 | 1.9495 | 19.1384 | 3.3288 | 2.5971 | 10.5762 | orjson | 0.751x |
| p24-s1 | aarch64 | 1.9427 | 18.9459 | 3.3422 | 2.6082 | 10.5369 | orjson | 0.745x |
| p24-s2 | aarch64 | 1.9660 | 19.2032 | 3.3648 | 2.6305 | 10.6977 | orjson | 0.747x |
| now-s1 | aarch64 | 1.9570 | 19.1575 | 3.3467 | 2.6051 | 10.6789 | orjson | 0.751x |
| now-s2 | aarch64 | 1.9376 | 19.0483 | 3.3270 | 2.6069 | 10.6090 | orjson | 0.743x |

verdict: **agree** — every shared rival within tolerance (json 1.4%, msgspec 2.1%, orjson 2.4%, ujson 2.1%)

## linux-arm64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| p23-s1 | aarch64 | 0.1891 | 0.4468 | 0.2361 | 0.2258 | 0.2958 | 0.3057 | orjson | 0.837x |
| p23-s2 | aarch64 | 0.1938 | 0.4693 | 0.2382 | 0.2175 | 0.2978 | 0.3137 | orjson | 0.891x |
| p24-s1 | aarch64 | 0.1956 | 0.4665 | 0.2431 | 0.2229 | 0.3128 | 0.3176 | orjson | 0.878x |
| p24-s2 | aarch64 | 0.2087 | 0.5048 | 0.2597 | 0.2392 | 0.3239 | 0.3468 | orjson | 0.873x |
| now-s1 | aarch64 | 0.2086 | 0.4924 | 0.2552 | 0.2323 | 0.3187 | 0.3432 | orjson | 0.898x |
| now-s2 | aarch64 | 0.1922 | 0.4767 | 0.2426 | 0.2161 | 0.3063 | 0.3310 | orjson | 0.889x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +13%, msgspec +10%, orjson +11%, pysimdjson +9%, ujson +13%)

## linux-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 0.0646 | 0.5379 | 0.0887 | 0.0710 | 0.2318 | orjson | 0.909x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.0920 | 0.5778 | 0.0962 | 0.1222 | 0.2462 | msgspec | 0.957x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 0.0502 | 0.4133 | 0.0683 | 0.0479 | 0.1789 | orjson | 1.047x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 0.0483 | 0.4082 | 0.0680 | 0.0478 | 0.1784 | orjson | 1.011x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.0821 | 0.5660 | 0.0896 | 0.0989 | 0.2407 | msgspec | 0.916x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 0.0507 | 0.4477 | 0.0724 | 0.0520 | 0.2067 | orjson | 0.976x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +42%, msgspec +41%, orjson +155%, ujson +38%)

## linux-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 0.1680 | 0.6638 | 0.1918 | 0.1769 | 0.3516 | orjson | 0.949x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2074 | 0.7317 | 0.2383 | 0.2569 | 0.4031 | msgspec | 0.870x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 0.1159 | 0.5053 | 0.1425 | 0.1251 | 0.2651 | orjson | 0.926x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 0.1161 | 0.4941 | 0.1424 | 0.1236 | 0.2646 | orjson | 0.940x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.1625 | 0.6773 | 0.1830 | 0.1811 | 0.3470 | orjson | 0.897x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 0.1089 | 0.5294 | 0.1412 | 0.1231 | 0.2876 | orjson | 0.885x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +48%, msgspec +69%, orjson +109%, ujson +52%)

## linux-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 0.4621 | 2.0518 | 0.6438 | 0.5376 | 1.2157 | orjson | 0.860x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.4634 | 2.2158 | 0.6816 | 0.5122 | 1.2498 | orjson | 0.905x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 0.3669 | 1.6260 | 0.5107 | 0.3849 | 0.9699 | orjson | 0.953x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 0.3630 | 1.5977 | 0.5008 | 0.3655 | 0.9516 | orjson | 0.993x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.4214 | 2.1818 | 0.6511 | 0.4673 | 1.2048 | orjson | 0.902x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 0.3458 | 1.6986 | 0.4911 | 0.3559 | 1.0246 | orjson | 0.972x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +39%, msgspec +39%, orjson +51%, ujson +31%)

## linux-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 0.3176 | 1.8718 | 0.4859 | 0.3769 | 1.0348 | orjson | 0.843x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2900 | 2.0158 | 0.4933 | 0.3899 | 1.0524 | orjson | 0.744x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 0.2426 | 1.4510 | 0.3799 | 0.2479 | 0.8220 | orjson | 0.979x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 0.2474 | 1.4557 | 0.3742 | 0.2481 | 0.8314 | orjson | 0.997x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2807 | 2.0110 | 0.4902 | 0.3414 | 1.0419 | orjson | 0.822x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 0.2416 | 1.5694 | 0.3805 | 0.2454 | 0.8975 | orjson | 0.984x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +39%, msgspec +32%, orjson +59%, ujson +28%)

## linux-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 2.3646 | 22.1580 | 4.1561 | 3.0923 | 11.4507 | orjson | 0.765x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 2.3232 | 22.7149 | 4.0307 | 2.5069 | 11.8257 | orjson | 0.927x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 1.8100 | 16.9400 | 3.2539 | 1.9914 | 8.9931 | orjson | 0.909x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 1.8151 | 16.6293 | 3.2294 | 1.9996 | 8.8052 | orjson | 0.908x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 2.1611 | 22.6439 | 3.9394 | 2.4240 | 11.6565 | orjson | 0.892x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 1.7005 | 17.9164 | 3.3109 | 1.9558 | 9.8780 | orjson | 0.869x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +37%, msgspec +29%, orjson +58%, ujson +34%)

## linux-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD EPYC 9V74 80-Core Processor | 0.1946 | 0.4574 | 0.2489 | 0.2419 | 0.3087 | 0.3181 | orjson | 0.804x |
| p23-s2 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2607 | 0.5367 | 0.3125 | 0.2877 | 0.3938 | 0.3706 | orjson | 0.906x |
| p24-s1 | AMD EPYC 9V74 80-Core Processor | 0.1527 | 0.3585 | 0.1898 | 0.1885 | 0.2448 | 0.2397 | orjson | 0.810x |
| p24-s2 | AMD EPYC 9V74 80-Core Processor | 0.1518 | 0.3494 | 0.1959 | 0.1856 | 0.2413 | 0.2434 | orjson | 0.818x |
| now-s1 | Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz | 0.2348 | 0.5120 | 0.2792 | 0.2653 | 0.3846 | 0.3501 | orjson | 0.885x |
| now-s2 | INTEL(R) XEON(R) PLATINUM 8573C | 0.1709 | 0.4163 | 0.2140 | 0.2038 | 0.2690 | 0.2679 | orjson | 0.839x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +54%, msgspec +65%, orjson +55%, pysimdjson +63%, ujson +55%)

## macos-arm64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 0.0352 | 0.3504 | 0.0805 | 0.0454 | 0.1664 | orjson | 0.775x |
| p23-s2 | Apple M1 (Virtual) | 0.0575 | 0.4780 | 0.0735 | 0.0653 | 0.2408 | orjson | 0.881x |
| p24-s1 | Apple M1 (Virtual) | 0.0369 | 0.3563 | 0.0548 | 0.0476 | 0.1691 | orjson | 0.774x |
| p24-s2 | Apple M1 (Virtual) | 0.0382 | 0.3559 | 0.0559 | 0.0473 | 0.1754 | orjson | 0.807x |
| now-s1 | Apple M1 (Virtual) | 0.0324 | 0.3244 | 0.0485 | 0.0910 | 0.1587 | msgspec | 0.667x |
| now-s2 | Apple M1 (Virtual) | 0.0480 | 0.3881 | 0.0663 | 0.0573 | 0.1871 | orjson | 0.837x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +47%, msgspec +66%, orjson +101%, ujson +52%)

## macos-arm64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 0.1843 | 0.5399 | 0.3144 | 0.2080 | 0.3737 | orjson | 0.886x |
| p23-s2 | Apple M1 (Virtual) | 0.2813 | 0.6992 | 0.3660 | 0.2991 | 0.5163 | orjson | 0.940x |
| p24-s1 | Apple M1 (Virtual) | 0.1569 | 0.4947 | 0.1831 | 0.1816 | 0.3017 | orjson | 0.864x |
| p24-s2 | Apple M1 (Virtual) | 0.2251 | 0.6003 | 0.2162 | 0.2170 | 0.3500 | msgspec | 1.041x |
| now-s1 | Apple M1 (Virtual) | 0.1292 | 0.4314 | 0.1536 | 0.1643 | 0.2664 | msgspec | 0.841x |
| now-s2 | Apple M1 (Virtual) | 0.2556 | 0.6617 | 0.2928 | 0.2704 | 0.4535 | orjson | 0.945x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +62%, msgspec +138%, orjson +82%, ujson +94%)

## macos-arm64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 0.4016 | 1.6169 | 0.5166 | 0.4737 | 0.9971 | orjson | 0.848x |
| p23-s2 | Apple M1 (Virtual) | 0.6612 | 1.9713 | 0.7191 | 0.7217 | 1.2339 | msgspec | 0.920x |
| p24-s1 | Apple M1 (Virtual) | 0.3309 | 1.5431 | 0.4527 | 0.3814 | 0.8813 | orjson | 0.868x |
| p24-s2 | Apple M1 (Virtual) | 0.4472 | 1.7128 | 0.5301 | 0.4931 | 1.0184 | orjson | 0.907x |
| now-s1 | Apple M1 (Virtual) | 0.3194 | 1.5095 | 0.4248 | 0.3717 | 0.8730 | orjson | 0.859x |
| now-s2 | Apple M1 (Virtual) | 0.5646 | 1.7826 | 0.6044 | 0.5196 | 1.1937 | orjson | 1.086x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +31%, msgspec +69%, orjson +94%, ujson +41%)

## macos-arm64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 0.2090 | 1.3615 | 0.3275 | 0.4618 | 0.7868 | msgspec | 0.638x |
| p23-s2 | Apple M1 (Virtual) | 0.2924 | 1.9929 | 0.3797 | 0.2902 | 0.8992 | orjson | 1.008x |
| p24-s1 | Apple M1 (Virtual) | 0.2337 | 1.5765 | 0.3400 | 0.2803 | 0.8245 | orjson | 0.834x |
| p24-s2 | Apple M1 (Virtual) | 0.2021 | 1.4400 | 0.3144 | 0.2473 | 0.7842 | orjson | 0.817x |
| now-s1 | Apple M1 (Virtual) | 0.1902 | 1.2735 | 0.2902 | 0.2338 | 0.7233 | orjson | 0.813x |
| now-s2 | Apple M1 (Virtual) | 0.2578 | 1.7154 | 0.3650 | 0.2897 | 0.8783 | orjson | 0.890x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +56%, msgspec +31%, orjson +98%, ujson +24%)

## macos-arm64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 1.8732 | 18.3230 | 3.7922 | 2.7202 | 14.0492 | orjson | 0.689x |
| p23-s2 | Apple M1 (Virtual) | 1.7750 | 17.5613 | 3.2108 | 2.6068 | 9.7466 | orjson | 0.681x |
| p24-s1 | Apple M1 (Virtual) | 1.5448 | 16.3022 | 3.1782 | 2.4291 | 9.2026 | orjson | 0.636x |
| p24-s2 | Apple M1 (Virtual) | 1.6530 | 16.9275 | 3.2160 | 2.5801 | 9.9289 | orjson | 0.641x |
| now-s1 | Apple M1 (Virtual) | 1.4327 | 16.1917 | 3.0966 | 2.4054 | 8.9359 | orjson | 0.596x |
| now-s2 | Apple M1 (Virtual) | 1.5549 | 16.1811 | 3.0152 | 2.3752 | 9.3380 | orjson | 0.655x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +13%, msgspec +26%, orjson +15%, ujson +57%)

## macos-arm64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| p23-s1 | Apple M1 (Virtual) | 0.1219 | 0.3208 | 0.1706 | 0.1615 | 2.5022 | 0.2759 | orjson | 0.754x |
| p23-s2 | Apple M1 (Virtual) | 0.1737 | 0.4135 | 0.2157 | 0.1976 | 2.8619 | 0.4388 | orjson | 0.879x |
| p24-s1 | Apple M1 (Virtual) | 0.1151 | 0.3096 | 0.1580 | 0.1489 | 2.3752 | 0.2331 | orjson | 0.773x |
| p24-s2 | Apple M1 (Virtual) | 0.1240 | 0.3295 | 0.1728 | 0.1580 | 2.4725 | 0.2145 | orjson | 0.785x |
| now-s1 | Apple M1 (Virtual) | 0.1155 | 0.3014 | 0.1596 | 0.1481 | 2.3476 | 0.2689 | orjson | 0.780x |
| now-s2 | Apple M1 (Virtual) | 0.1408 | 0.3704 | 0.1917 | 0.1999 | 2.7877 | 0.3380 | msgspec | 0.734x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +37%, msgspec +36%, orjson +35%, pysimdjson +22%, ujson +105%)

## macos-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0823 | 0.9944 | 0.1310 | 0.1000 | 0.4796 | orjson | 0.823x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1088 | 1.3851 | 0.1848 | 0.1366 | 0.6110 | orjson | 0.797x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0610 | 0.9042 | 0.1058 | 0.0747 | 0.4271 | orjson | 0.817x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0609 | 0.9067 | 0.1037 | 0.0750 | 0.4312 | orjson | 0.812x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.0671 | 1.0395 | 0.1189 | 0.0848 | 0.4963 | orjson | 0.792x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.1503 | 1.7856 | 0.2604 | 0.1622 | 0.7844 | orjson | 0.927x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +97%, msgspec +151%, orjson +117%, ujson +84%)

## macos-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.4022 | 1.3854 | 0.5273 | 0.4449 | 0.8395 | orjson | 0.904x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.5177 | 1.7126 | 0.6400 | 0.6379 | 1.2752 | orjson | 0.812x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3475 | 1.2300 | 0.3990 | 0.3636 | 0.7313 | orjson | 0.956x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3331 | 1.2385 | 0.4081 | 0.3751 | 0.7592 | orjson | 0.888x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3825 | 1.4248 | 0.4586 | 0.4051 | 0.8666 | orjson | 0.944x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6165 | 2.1763 | 0.7219 | 0.7245 | 1.2430 | msgspec | 0.854x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +77%, msgspec +81%, orjson +99%, ujson +74%)

## macos-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7254 | 4.2083 | 0.9784 | 0.8395 | 2.6638 | orjson | 0.864x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7118 | 3.9272 | 0.9046 | 0.7914 | 2.5338 | orjson | 0.899x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6556 | 3.8451 | 0.8830 | 0.7641 | 2.5133 | orjson | 0.858x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.6577 | 3.8768 | 0.8731 | 0.7446 | 2.5008 | orjson | 0.883x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7384 | 4.4027 | 0.9716 | 0.8146 | 2.9169 | orjson | 0.906x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 1.0446 | 6.8571 | 1.5514 | 1.2581 | 4.2200 | orjson | 0.830x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +78%, msgspec +78%, orjson +69%, ujson +69%)

## macos-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3349 | 3.5706 | 0.5578 | 0.4202 | 2.1631 | orjson | 0.797x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3347 | 3.6136 | 0.5444 | 0.4165 | 2.1680 | orjson | 0.804x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3102 | 3.4540 | 0.5020 | 0.3874 | 2.0806 | orjson | 0.801x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3043 | 3.4256 | 0.4969 | 0.3703 | 2.0946 | orjson | 0.822x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3482 | 4.0143 | 0.5746 | 0.4287 | 2.4362 | orjson | 0.812x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.5141 | 5.7852 | 1.0103 | 0.7191 | 3.7338 | orjson | 0.715x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +69%, msgspec +103%, orjson +94%, ujson +79%)

## macos-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.6319 | 41.5308 | 5.2860 | 3.4329 | 25.2994 | orjson | 0.767x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.9228 | 45.4115 | 5.9246 | 3.7004 | 26.5094 | orjson | 0.790x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4097 | 39.5925 | 5.0060 | 3.2094 | 23.4716 | orjson | 0.751x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.4881 | 38.9337 | 4.9301 | 3.1624 | 23.2115 | orjson | 0.787x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 2.8754 | 47.9899 | 6.0053 | 3.7782 | 28.2618 | orjson | 0.761x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 4.3822 | 63.8467 | 7.9782 | 5.3508 | 38.0820 | orjson | 0.819x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +64%, msgspec +62%, orjson +69%, ujson +64%)

## macos-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | pysimdjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|---|
| p23-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.4637 | 1.2052 | 0.6908 | 0.7435 | 4.2069 | 0.7160 | msgspec | 0.671x |
| p23-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3568 | 0.9406 | 0.4821 | 0.4565 | 3.2418 | 0.6550 | orjson | 0.782x |
| p24-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3294 | 0.8379 | 0.4444 | 0.4127 | 3.0514 | 0.5981 | orjson | 0.798x |
| p24-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3264 | 0.8312 | 0.4335 | 0.4091 | 3.0447 | 0.5913 | orjson | 0.798x |
| now-s1 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.3746 | 0.9725 | 0.4989 | 0.4695 | 3.5255 | 0.6865 | orjson | 0.798x |
| now-s2 | Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz | 0.7076 | 1.8095 | 0.9850 | 0.8504 | 5.9062 | 1.2891 | orjson | 0.832x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +118%, msgspec +127%, orjson +108%, pysimdjson +94%, ujson +118%)

## windows-x86_64 — dumps mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0755 | 0.5187 | 0.0958 | 0.0710 | 0.2691 | orjson | 1.063x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0747 | 0.5458 | 0.0913 | 0.0694 | 0.2685 | orjson | 1.076x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0787 | 0.5660 | 0.1000 | 0.0734 | 0.2780 | orjson | 1.074x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0733 | 0.5187 | 0.0936 | 0.0697 | 0.2692 | orjson | 1.052x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0763 | 0.5437 | 0.1136 | 0.0713 | 0.2691 | orjson | 1.070x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.0765 | 0.5342 | 0.1024 | 0.0721 | 0.2678 | orjson | 1.060x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +9%, msgspec +24%, orjson +6%, ujson +4%)

## windows-x86_64 — dump mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4153 | 1.0530 | 0.4345 | 0.4119 | 0.7667 | orjson | 1.008x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3857 | 1.0526 | 0.4075 | 0.3802 | 0.7581 | orjson | 1.014x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4071 | 1.0260 | 0.4179 | 0.4138 | 0.7879 | orjson | 0.984x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4515 | 1.0454 | 0.4234 | 0.3862 | 0.8010 | orjson | 1.169x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3969 | 1.0694 | 0.4373 | 0.3969 | 0.8219 | orjson | 1.000x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.4494 | 1.0969 | 0.4441 | 0.4266 | 0.8351 | orjson | 1.054x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +7%, msgspec +9%, orjson +12%, ujson +10%)

## windows-x86_64 — dump flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.7131 | 3.3434 | 0.9176 | 0.7587 | 2.8815 | orjson | 0.940x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.6947 | 3.3736 | 0.8738 | 0.7043 | 2.8440 | orjson | 0.986x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.7131 | 3.4661 | 0.8935 | 0.7575 | 2.8455 | orjson | 0.941x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.6957 | 3.5168 | 0.8957 | 0.7549 | 2.9371 | orjson | 0.922x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.8217 | 3.5223 | 0.9715 | 0.7967 | 2.9488 | orjson | 1.031x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.8545 | 3.5508 | 1.0056 | 0.8160 | 3.0740 | orjson | 1.047x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +6%, msgspec +15%, orjson +16%, ujson +8%)

## windows-x86_64 — dumps flat.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3168 | 1.9273 | 0.5073 | 0.3626 | 1.5546 | orjson | 0.874x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3122 | 1.9794 | 0.4911 | 0.3575 | 1.5624 | orjson | 0.873x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3185 | 2.1510 | 0.5149 | 0.3683 | 1.5214 | orjson | 0.865x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3285 | 2.0815 | 0.5194 | 0.3560 | 1.5446 | orjson | 0.923x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3174 | 2.0890 | 0.5470 | 0.3680 | 1.4986 | orjson | 0.862x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.3356 | 2.0194 | 0.5445 | 0.3992 | 1.4677 | orjson | 0.841x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +12%, msgspec +11%, orjson +12%, ujson +6%)

## windows-x86_64 — dumps users.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.4284 | 28.9898 | 6.0036 | 4.3675 | 16.6711 | orjson | 0.785x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2303 | 26.1338 | 5.4615 | 3.7783 | 14.4462 | orjson | 0.855x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.0680 | 24.6889 | 5.1088 | 3.5659 | 14.0480 | orjson | 0.860x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.1046 | 23.6918 | 5.2227 | 3.8659 | 14.2538 | orjson | 0.803x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.3401 | 26.5195 | 5.1446 | 4.5479 | 16.6061 | orjson | 0.734x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 3.2178 | 23.6124 | 5.2935 | 3.9101 | 14.2789 | orjson | 0.823x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +23%, msgspec +18%, orjson +28%, ujson +19%)

## windows-x86_64 — loads mixed.json

| sample | host | strata | json | msgspec | orjson | ujson | best rival | ratio |
|---|---|---|---|---|---|---|---|---|
| p23-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1919 | 0.4780 | 0.2423 | 0.2213 | 0.3855 | orjson | 0.867x |
| p23-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1847 | 0.4815 | 0.2404 | 0.2157 | 0.3535 | orjson | 0.856x |
| p24-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1903 | 0.4912 | 0.2476 | 0.2217 | 0.3691 | orjson | 0.859x |
| p24-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1847 | 0.4912 | 0.2407 | 0.2164 | 0.3774 | orjson | 0.854x |
| now-s1 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1896 | 0.4858 | 0.2480 | 0.2182 | 0.3771 | orjson | 0.869x |
| now-s2 | AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD | 0.1900 | 0.4776 | 0.2368 | 0.2155 | 0.4076 | orjson | 0.882x |

verdict: **rival-moved** — unchanged rivals differ between draws (json +3%, msgspec +5%, orjson +3%, ujson +15%)

## What the draws agree on

A rank is a within-run comparison, so a row behind on *every* draw is a deficit whatever the hosts did, a row ahead on every draw is held, and a row whose rank changes between draws is decided by the draw. `rivals moved` reports the widest spread an unchanged rival showed across these samples, which is the scale of what a single sample can resolve.

### Behind on every draw

- windows-x86_64 dumps mixed.json: 1.063x, 1.076x, 1.074x, 1.052x, 1.070x, 1.060x (rivals moved 24%)

### Rank changes between draws

- linux-x86_64 dumps mixed.json: 0.909x, 0.957x, 1.047x, 1.011x, 0.916x, 0.976x (rivals moved 155%)
- macos-arm64 dump mixed.json: 0.886x, 0.940x, 0.864x, 1.041x, 0.841x, 0.945x (rivals moved 138%)
- macos-arm64 dump flat.json: 0.848x, 0.920x, 0.868x, 0.907x, 0.859x, 1.086x (rivals moved 94%)
- macos-arm64 dumps flat.json: 0.638x, 1.008x, 0.834x, 0.817x, 0.813x, 0.890x (rivals moved 98%)
- windows-x86_64 dump mixed.json: 1.008x, 1.014x, 0.984x, 1.169x, 1.000x, 1.054x (rivals moved 12%)
- windows-x86_64 dump flat.json: 0.940x, 0.986x, 0.941x, 0.922x, 1.031x, 1.047x (rivals moved 16%)

### Ahead on every draw

- linux-arm64 dumps mixed.json: 0.944x, 0.987x, 0.966x, 0.986x, 0.991x, 0.987x (rivals moved 34%)
- linux-arm64 dump mixed.json: 0.898x, 0.899x, 0.896x, 0.850x, 0.890x, 0.898x (rivals moved 71%)
- linux-arm64 dump flat.json: 0.835x, 0.866x, 0.826x, 0.854x, 0.859x, 0.805x (rivals moved 30%)
- linux-arm64 dumps flat.json: 0.804x, 0.807x, 0.792x, 0.832x, 0.790x, 0.780x (rivals moved 12%)
- linux-arm64 dumps users.json: 0.750x, 0.751x, 0.745x, 0.747x, 0.751x, 0.743x (rivals held)
- linux-arm64 loads mixed.json: 0.837x, 0.891x, 0.878x, 0.873x, 0.898x, 0.889x (rivals moved 13%)
- linux-x86_64 dump mixed.json: 0.949x, 0.870x, 0.926x, 0.940x, 0.897x, 0.885x (rivals moved 109%)
- linux-x86_64 dump flat.json: 0.860x, 0.905x, 0.953x, 0.993x, 0.902x, 0.972x (rivals moved 51%)
- linux-x86_64 dumps flat.json: 0.843x, 0.744x, 0.979x, 0.997x, 0.822x, 0.984x (rivals moved 59%)
- linux-x86_64 dumps users.json: 0.765x, 0.927x, 0.909x, 0.908x, 0.892x, 0.869x (rivals moved 58%)
- linux-x86_64 loads mixed.json: 0.804x, 0.906x, 0.810x, 0.818x, 0.885x, 0.839x (rivals moved 65%)
- macos-arm64 dumps mixed.json: 0.775x, 0.881x, 0.774x, 0.807x, 0.667x, 0.837x (rivals moved 101%)
- macos-arm64 dumps users.json: 0.689x, 0.681x, 0.636x, 0.641x, 0.596x, 0.655x (rivals moved 57%)
- macos-arm64 loads mixed.json: 0.754x, 0.879x, 0.773x, 0.785x, 0.780x, 0.734x (rivals moved 105%)
- macos-x86_64 dumps mixed.json: 0.823x, 0.797x, 0.817x, 0.812x, 0.792x, 0.927x (rivals moved 151%)
- macos-x86_64 dump mixed.json: 0.904x, 0.812x, 0.956x, 0.888x, 0.944x, 0.854x (rivals moved 99%)
- macos-x86_64 dump flat.json: 0.864x, 0.899x, 0.858x, 0.883x, 0.906x, 0.830x (rivals moved 78%)
- macos-x86_64 dumps flat.json: 0.797x, 0.804x, 0.801x, 0.822x, 0.812x, 0.715x (rivals moved 103%)
- macos-x86_64 dumps users.json: 0.767x, 0.790x, 0.751x, 0.787x, 0.761x, 0.819x (rivals moved 69%)
- macos-x86_64 loads mixed.json: 0.671x, 0.782x, 0.798x, 0.798x, 0.798x, 0.832x (rivals moved 127%)
- windows-x86_64 dumps flat.json: 0.874x, 0.873x, 0.865x, 0.923x, 0.862x, 0.841x (rivals moved 12%)
- windows-x86_64 dumps users.json: 0.785x, 0.855x, 0.860x, 0.803x, 0.734x, 0.823x (rivals moved 28%)
- windows-x86_64 loads mixed.json: 0.867x, 0.856x, 0.859x, 0.854x, 0.869x, 0.882x (rivals moved 15%)

