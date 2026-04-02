# Strata parser comparison

_2026-04-02 02:33:03_

| input | strata | nlohmann | speedup | rapidjson | ratio | simdjson | ratio |
|-------|-------:|--------:|--------:|---------:|------:|--------:|------:|
| null | 5.3 ns | 63.4 ns | 11.9x | 74.0 ns | 0.07x | 141.8 ns | 0.04x |
| bool_true | 4.8 ns | 65.0 ns | 13.6x | 74.3 ns | 0.06x | 138.8 ns | 0.03x |
| bool_false | 4.5 ns | 76.9 ns | 17.0x | 75.5 ns | 0.06x | 137.6 ns | 0.03x |
| int_small | 5.5 ns | 62.8 ns | 11.5x | 78.9 ns | 0.07x | 155.7 ns | 0.04x |
| int_negative | 7.7 ns | 103.6 ns | 13.4x | 81.4 ns | 0.10x | 153.4 ns | 0.05x |
| int_large | 8.6 ns | 167.2 ns | 19.4x | 85.6 ns | 0.10x | 160.4 ns | 0.05x |
| float_simple | 7.4 ns | 84.5 ns | 11.4x | 78.9 ns | 0.09x | 157.1 ns | 0.05x |
| float_negative | 9.0 ns | 105.2 ns | 11.7x | 79.0 ns | 0.11x | 161.3 ns | 0.06x |
| float_exponent | 9.6 ns | 104.0 ns | 10.8x | 82.2 ns | 0.12x | 157.0 ns | 0.06x |
| float_small_exp | 11.3 ns | 120.8 ns | 10.7x | 81.4 ns | 0.14x | 157.1 ns | 0.07x |
| string_short | 13.9 ns | 102.4 ns | 7.3x | 111.2 ns | 0.13x | 157.5 ns | 0.09x |
| string_medium | 42.8 ns | 289.3 ns | 6.8x | 727.5 ns | 0.06x | 138.4 ns | 0.31x |
| string_escaped | 68.4 ns | 252.7 ns | 3.7x | 651.3 ns | 0.11x | 163.1 ns | 0.42x |
| string_unicode | 21.4 ns | 173.2 ns | 8.1x | 123.6 ns | 0.17x | 172.4 ns | 0.12x |
| string_surrogate | 14.4 ns | 123.1 ns | 8.6x | 114.5 ns | 0.13x | 156.4 ns | 0.09x |
| string_empty | 8.7 ns | 65.8 ns | 7.6x | 107.9 ns | 0.08x | 149.3 ns | 0.06x |
| value_ws_null | 7.2 ns | 100.1 ns | 13.9x | 76.7 ns | 0.09x | 158.7 ns | 0.05x |
| value_ws_number | 7.1 ns | 97.0 ns | 13.7x | 82.3 ns | 0.09x | 153.0 ns | 0.05x |
| array_empty | 8.8 ns | 75.3 ns | 8.6x | 76.9 ns | 0.11x | 171.4 ns | 0.05x |
| array_3_ints | 90.2 ns | 244.7 ns | 2.7x | 578.4 ns | 0.16x | 168.8 ns | 0.53x |
| array_10_ints | 236.3 ns | 511.5 ns | 2.2x | 627.4 ns | 0.38x | 192.2 ns | 1.23x |
| array_nested | 271.7 ns | 569.6 ns | 2.1x | 614.0 ns | 0.44x | 181.0 ns | 1.50x |
| array_strings | 145.4 ns | 362.0 ns | 2.5x | 668.0 ns | 0.22x | 175.4 ns | 0.83x |
| array_mixed | 149.5 ns | 380.3 ns | 2.5x | 615.8 ns | 0.24x | 168.0 ns | 0.89x |
| object_empty | 9.4 ns | 76.8 ns | 8.1x | 76.2 ns | 0.12x | 164.8 ns | 0.06x |
| object_1_key | 42.6 ns | 173.5 ns | 4.1x | 599.0 ns | 0.07x | 161.8 ns | 0.26x |
| object_3_keys | 136.3 ns | 318.9 ns | 2.3x | 636.5 ns | 0.21x | 162.6 ns | 0.84x |
| object_nested | 141.7 ns | 343.7 ns | 2.4x | 643.2 ns | 0.22x | 167.1 ns | 0.85x |
| object_with_array | 201.7 ns | 442.3 ns | 2.2x | 768.9 ns | 0.26x | 172.3 ns | 1.17x |
| realistic_small | 184.8 ns | 376.2 ns | 2.0x | 670.2 ns | 0.28x | 156.5 ns | 1.18x |
| realistic_medium | 640.6 ns | 1286.8 ns | 2.0x | 937.1 ns | 0.68x | 215.0 ns | 2.98x |
| realistic_array_of_obj | 847.8 ns | 1106.9 ns | 1.3x | 812.3 ns | 1.04x | 206.2 ns | 4.11x |
