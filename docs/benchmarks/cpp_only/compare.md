# Strata parser comparison

_2026-04-02 03:56:43_

| input | strata | nlohmann | speedup | rapidjson | ratio | simdjson | ratio |
|-------|-------:|--------:|--------:|---------:|------:|--------:|------:|
| null | 8.6 ns | 68.2 ns | 7.9x | 73.1 ns | 0.12x | 139.0 ns | 0.06x |
| bool_true | 9.2 ns | 64.2 ns | 7.0x | 73.9 ns | 0.12x | 140.6 ns | 0.07x |
| bool_false | 9.8 ns | 77.3 ns | 7.9x | 79.5 ns | 0.12x | 144.3 ns | 0.07x |
| int_small | 10.7 ns | 63.3 ns | 5.9x | 80.8 ns | 0.13x | 160.9 ns | 0.07x |
| int_negative | 12.9 ns | 101.0 ns | 7.8x | 80.5 ns | 0.16x | 154.8 ns | 0.08x |
| int_large | 13.9 ns | 171.6 ns | 12.4x | 85.4 ns | 0.16x | 159.1 ns | 0.09x |
| float_simple | 12.7 ns | 85.1 ns | 6.7x | 77.8 ns | 0.16x | 161.4 ns | 0.08x |
| float_negative | 14.9 ns | 105.0 ns | 7.0x | 77.8 ns | 0.19x | 175.8 ns | 0.09x |
| float_exponent | 14.8 ns | 109.7 ns | 7.4x | 82.0 ns | 0.18x | 162.0 ns | 0.09x |
| float_small_exp | 15.7 ns | 126.7 ns | 8.1x | 83.6 ns | 0.19x | 162.2 ns | 0.10x |
| string_short | 18.6 ns | 103.7 ns | 5.6x | 110.4 ns | 0.17x | 150.5 ns | 0.12x |
| string_medium | 47.1 ns | 306.6 ns | 6.5x | 756.0 ns | 0.06x | 138.6 ns | 0.34x |
| string_escaped | 70.9 ns | 246.9 ns | 3.5x | 667.8 ns | 0.11x | 161.0 ns | 0.44x |
| string_unicode | 25.6 ns | 178.8 ns | 7.0x | 128.6 ns | 0.20x | 170.5 ns | 0.15x |
| string_surrogate | 18.9 ns | 128.2 ns | 6.8x | 115.9 ns | 0.16x | 153.5 ns | 0.12x |
| string_empty | 12.7 ns | 63.8 ns | 5.0x | 103.5 ns | 0.12x | 152.0 ns | 0.08x |
| value_ws_null | 10.7 ns | 102.4 ns | 9.6x | 81.0 ns | 0.13x | 156.0 ns | 0.07x |
| value_ws_number | 12.2 ns | 97.5 ns | 8.0x | 81.0 ns | 0.15x | 156.8 ns | 0.08x |
| array_empty | 11.9 ns | 77.1 ns | 6.5x | 78.7 ns | 0.15x | 158.9 ns | 0.08x |
| array_3_ints | 93.9 ns | 243.9 ns | 2.6x | 584.8 ns | 0.16x | 152.5 ns | 0.62x |
| array_10_ints | 210.2 ns | 518.0 ns | 2.5x | 614.9 ns | 0.34x | 183.6 ns | 1.14x |
| array_nested | 265.4 ns | 574.8 ns | 2.2x | 621.5 ns | 0.43x | 167.6 ns | 1.58x |
| array_strings | 148.7 ns | 362.9 ns | 2.4x | 668.2 ns | 0.22x | 149.0 ns | 1.00x |
| array_mixed | 168.0 ns | 377.2 ns | 2.2x | 633.2 ns | 0.27x | 154.6 ns | 1.09x |
| object_empty | 9.0 ns | 77.7 ns | 8.6x | 78.9 ns | 0.11x | 153.7 ns | 0.06x |
| object_1_key | 54.8 ns | 176.1 ns | 3.2x | 607.8 ns | 0.09x | 152.3 ns | 0.36x |
| object_3_keys | 115.4 ns | 322.0 ns | 2.8x | 639.3 ns | 0.18x | 156.6 ns | 0.74x |
| object_nested | 162.8 ns | 346.5 ns | 2.1x | 637.8 ns | 0.26x | 153.4 ns | 1.06x |
| object_with_array | 179.9 ns | 455.4 ns | 2.5x | 646.8 ns | 0.28x | 159.1 ns | 1.13x |
| realistic_small | 157.7 ns | 377.1 ns | 2.4x | 681.3 ns | 0.23x | 166.1 ns | 0.95x |
| realistic_medium | 554.5 ns | 1272.7 ns | 2.3x | 871.4 ns | 0.64x | 213.7 ns | 2.59x |
| realistic_array_of_obj | 704.8 ns | 1101.3 ns | 1.6x | 819.6 ns | 0.86x | 204.3 ns | 3.45x |
