# Strata parser comparison

_2026-03-30 20:37:41_

| input | strata | nlohmann | speedup | rapidjson | ratio | simdjson | ratio |
|-------|-------:|--------:|--------:|---------:|------:|--------:|------:|
| null | 5.4 ns | 62.3 ns | 11.6x | 74.8 ns | 0.07x | 139.4 ns | 0.04x |
| bool_true | 4.3 ns | 66.5 ns | 15.4x | 74.4 ns | 0.06x | 135.9 ns | 0.03x |
| bool_false | 4.3 ns | 77.6 ns | 18.1x | 73.5 ns | 0.06x | 135.6 ns | 0.03x |
| int_small | 4.9 ns | 60.4 ns | 12.3x | 75.7 ns | 0.07x | 151.7 ns | 0.03x |
| int_negative | 6.1 ns | 99.1 ns | 16.1x | 78.6 ns | 0.08x | 157.6 ns | 0.04x |
| int_large | 13.4 ns | 168.8 ns | 12.6x | 86.1 ns | 0.16x | 157.4 ns | 0.08x |
| float_simple | 6.9 ns | 84.3 ns | 12.2x | 74.4 ns | 0.09x | 159.3 ns | 0.04x |
| float_negative | 8.0 ns | 104.5 ns | 13.0x | 74.0 ns | 0.11x | 160.7 ns | 0.05x |
| float_exponent | 8.5 ns | 102.1 ns | 12.1x | 77.5 ns | 0.11x | 160.5 ns | 0.05x |
| float_small_exp | 9.3 ns | 125.0 ns | 13.5x | 76.2 ns | 0.12x | 158.3 ns | 0.06x |
| string_short | 13.8 ns | 99.2 ns | 7.2x | 109.0 ns | 0.13x | 154.0 ns | 0.09x |
| string_medium | 42.6 ns | 283.6 ns | 6.7x | 709.8 ns | 0.06x | 133.0 ns | 0.32x |
| string_escaped | 66.7 ns | 240.8 ns | 3.6x | 651.8 ns | 0.10x | 157.9 ns | 0.42x |
| string_unicode | 21.0 ns | 172.7 ns | 8.2x | 120.9 ns | 0.17x | 167.0 ns | 0.13x |
| string_surrogate | 14.4 ns | 123.8 ns | 8.6x | 113.7 ns | 0.13x | 160.7 ns | 0.09x |
| string_empty | 8.8 ns | 64.1 ns | 7.3x | 101.4 ns | 0.09x | 151.1 ns | 0.06x |
| value_ws_null | 7.3 ns | 98.9 ns | 13.6x | 74.6 ns | 0.10x | 153.3 ns | 0.05x |
| value_ws_number | 6.5 ns | 95.8 ns | 14.8x | 74.7 ns | 0.09x | 157.3 ns | 0.04x |
| array_empty | 8.7 ns | 73.7 ns | 8.5x | 73.0 ns | 0.12x | 148.0 ns | 0.06x |
| array_3_ints | 89.4 ns | 246.3 ns | 2.8x | 578.0 ns | 0.15x | 151.8 ns | 0.59x |
| array_10_ints | 233.7 ns | 510.8 ns | 2.2x | 637.6 ns | 0.37x | 184.6 ns | 1.27x |
| array_nested | 271.6 ns | 596.8 ns | 2.2x | 649.4 ns | 0.42x | 179.2 ns | 1.52x |
| array_strings | 151.9 ns | 369.0 ns | 2.4x | 677.7 ns | 0.22x | 158.8 ns | 0.96x |
| array_mixed | 151.6 ns | 391.7 ns | 2.6x | 646.1 ns | 0.23x | 163.6 ns | 0.93x |
| object_empty | 9.6 ns | 77.3 ns | 8.1x | 77.1 ns | 0.12x | 149.1 ns | 0.06x |
| object_1_key | 43.2 ns | 179.7 ns | 4.2x | 638.1 ns | 0.07x | 157.9 ns | 0.27x |
| object_3_keys | 135.1 ns | 323.7 ns | 2.4x | 647.4 ns | 0.21x | 163.7 ns | 0.83x |
| object_nested | 133.7 ns | 352.5 ns | 2.6x | 662.2 ns | 0.20x | 163.4 ns | 0.82x |
| object_with_array | 191.6 ns | 446.8 ns | 2.3x | 665.4 ns | 0.29x | 167.8 ns | 1.14x |
| realistic_small | 182.4 ns | 375.2 ns | 2.1x | 690.1 ns | 0.26x | 154.7 ns | 1.18x |
| realistic_medium | 617.4 ns | 1307.4 ns | 2.1x | 882.7 ns | 0.70x | 221.5 ns | 2.79x |
| realistic_array_of_obj | 800.0 ns | 1128.4 ns | 1.4x | 823.6 ns | 0.97x | 214.7 ns | 3.73x |
