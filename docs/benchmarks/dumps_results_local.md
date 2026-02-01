# Benchmarks

## Dumps benchmark (users.json)

Command: `python3.14 -m benchmarks.bench_dumps --data benchmarks/data/generated/users.json --repeat 50 --warmup 3`

| Run    | Min (ms) | Median (ms) | P95 (ms) | Size (bytes) | RSS (MB) |
| ------ | -------- | ----------- | -------- | ------------ | -------- |
| Before | 7.52     | 8.19        | 8.99     | 4706770      | 72.0     |
| After  | 7.46     | 7.94        | 8.67     | 4706770      | 71.7     |
