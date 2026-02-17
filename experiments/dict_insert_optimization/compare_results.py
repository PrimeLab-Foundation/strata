"""Compare baseline vs optimized results."""

# Baseline (before changes):
baseline = {
    "1000x3": {"min": 0.305, "median": 0.315, "p95": 0.348, "tp": 223.0},
    "1000x10": {"min": 0.715, "median": 0.749, "p95": 0.845, "tp": 306.8},
    "1000x25": {"min": 1.781, "median": 1.867, "p95": 1.951, "tp": 321.5},
    "500x50": {"min": 1.593, "median": 1.696, "p95": 1.944, "tp": 357.4},
    "nested": {"min": 0.821, "median": 0.880, "p95": 1.013, "tp": 159.5},
    "small": {"min": 9.803, "median": 10.393, "p95": 11.699, "tp": 92.5},
    "medium": {"min": 37.832, "median": 39.226, "p95": 41.098, "tp": 159.3},
    "large": {"min": 349.811, "median": 355.380, "p95": 364.318, "tp": 123.4},
}

# Optimized (known-hash insertion):
optimized = {
    "1000x3": {"min": 0.319, "median": 0.333, "p95": 0.403, "tp": 211.0},
    "1000x10": {"min": 0.701, "median": 0.735, "p95": 0.869, "tp": 312.6},
    "1000x25": {"min": 1.734, "median": 1.832, "p95": 2.097, "tp": 327.5},
    "500x50": {"min": 1.598, "median": 1.701, "p95": 1.961, "tp": 356.4},
    "nested": {"min": 0.804, "median": 0.834, "p95": 0.983, "tp": 168.4},
    "small": {"min": 9.396, "median": 9.889, "p95": 12.640, "tp": 97.2},
    "medium": {"min": 36.997, "median": 38.180, "p95": 40.926, "tp": 163.6},
    "large": {"min": 338.083, "median": 342.607, "p95": 347.009, "tp": 128.0},
}

print(f"{'Dataset':<20} {'Baseline med':>12} {'Optimized med':>14} {'Delta':>8} {'TP delta':>10}")
print("-" * 70)
for key in baseline:
    b = baseline[key]
    o = optimized[key]
    delta_pct = (o["median"] - b["median"]) / b["median"] * 100
    tp_delta = (o["tp"] - b["tp"]) / b["tp"] * 100
    print(f"{key:<20} {b['median']:>10.3f}ms {o['median']:>12.3f}ms {delta_pct:>+7.1f}% {tp_delta:>+8.1f}%")

if __name__ == '__main__':
    pass
