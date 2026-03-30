# Strata parse benchmark

_2026-03-30 20:26:47_

**Apple M4 Pro** | macOS | ARM64 | clang 17.0.0 | NEON

| name | ns/op | ops/sec |
|------|------:|--------:|
| null | 5.5 | 181197159 |
| bool_true | 4.5 | 224523153 |
| bool_false | 4.7 | 210949480 |
| int_small | 5.1 | 197814582 |
| int_negative | 6.3 | 158375541 |
| int_large | 13.8 | 72516806 |
| float_simple | 6.8 | 146479767 |
| float_negative | 8.4 | 119095057 |
| float_exponent | 8.6 | 116189646 |
| float_small_exp | 9.5 | 104808403 |
| string_short | 13.6 | 73389482 |
| string_medium | 52.7 | 18986171 |
| string_escaped | 71.4 | 14003569 |
| string_unicode | 22.0 | 45422889 |
| string_surrogate | 14.5 | 68937285 |
| string_empty | 8.5 | 117223508 |
| value_ws_null | 7.5 | 133889333 |
| value_ws_number | 6.7 | 149400638 |
| array_empty | 8.7 | 114628795 |
| array_3_ints | 85.3 | 11718836 |
| array_10_ints | 225.2 | 4440146 |
| array_nested | 266.5 | 3752283 |
| array_strings | 152.1 | 6575697 |
| array_mixed | 150.0 | 6666411 |
| object_empty | 9.4 | 106081862 |
| object_1_key | 42.2 | 23670907 |
| object_3_keys | 137.5 | 7274452 |
| object_nested | 139.2 | 7184327 |
| object_with_array | 194.7 | 5135945 |
| realistic_small | 190.0 | 5262526 |
| realistic_medium | 622.9 | 1605481 |
| realistic_array_of_obj | 818.3 | 1222077 |
