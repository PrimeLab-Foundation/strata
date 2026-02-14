#!/usr/bin/env python3
"""
Profile strata.loads using macOS native profiling tools (sample/xctrace).

This script runs strata.loads under profiling and generates data that can be
converted to flamegraphs showing C++ level hotspots.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
import time
from pathlib import Path

import strata


def run_simple_benchmark(data_path: Path, iterations: int = 100) -> None:
    """Run strata.loads repeatedly for profiling."""
    print(f"Loading data from {data_path}...")
    data = data_path.read_bytes()
    size_mb = len(data) / 1024 / 1024
    print(f"Data size: {size_mb:.2f} MB")
    print(f"Running {iterations} iterations...")

    start = time.perf_counter()
    for i in range(iterations):
        if i % 10 == 0:
            print(f"  Iteration {i}/{iterations}...")
        result = strata.loads(data)

    elapsed = time.perf_counter() - start
    print(f"\nCompleted {iterations} iterations in {elapsed:.2f}s")
    print(f"Average: {elapsed/iterations*1000:.2f}ms per iteration")
    print(f"Throughput: {size_mb * iterations / elapsed:.2f} MB/s")


def profile_with_sample(data_path: Path, iterations: int, output_path: Path) -> bool:
    """Profile using macOS 'sample' tool."""
    print("\n" + "="*80)
    print("Profiling with macOS 'sample' tool")
    print("="*80)

    # Create a temporary script that runs the benchmark
    script_path = output_path.parent / "profile_temp.py"
    script_path.write_text(f"""
import sys
sys.path.insert(0, '{Path.cwd()}')
from benchmarks.profile_cpp import run_simple_benchmark
from pathlib import Path

run_simple_benchmark(Path('{data_path}'), {iterations})
""")

    try:
        # Start the Python process
        print(f"Starting profiling session...")
        cmd = [
            sys.executable,
            str(script_path),
        ]

        proc = subprocess.Popen(cmd)
        time.sleep(0.5)  # Give it a moment to start

        # Run sample on the process
        sample_output = output_path.with_suffix('.sample.txt')
        sample_cmd = [
            'sample',
            str(proc.pid),
            '30',  # Sample for 30 seconds
            '-file', str(sample_output),
        ]

        print(f"Sampling PID {proc.pid} for 30 seconds...")
        subprocess.run(sample_cmd)

        proc.wait()

        print(f"\nProfile data saved to: {sample_output}")
        return sample_output.exists()

    finally:
        script_path.unlink(missing_ok=True)


def analyze_sample_output(sample_file: Path, top_n: int = 20) -> list[tuple[str, int, float]]:
    """Parse sample output and extract hotspots."""
    if not sample_file.exists():
        print(f"Sample file not found: {sample_file}")
        return []

    print("\n" + "="*80)
    print("Analyzing Sample Output - Top Hotspots")
    print("="*80)

    content = sample_file.read_text()

    # Find the "Heaviest stack" section or symbol counts
    lines = content.split('\n')

    # Look for symbol counts or call tree
    hotspots = []
    in_symbol_section = False
    total_samples = 0

    for line in lines:
        # Parse total samples
        if 'Total number in stack' in line or 'samples' in line.lower():
            parts = line.split()
            for i, part in enumerate(parts):
                if part.isdigit() and int(part) > 100:
                    total_samples = max(total_samples, int(part))

        # Look for symbol counts (format: "123 symbol_name")
        stripped = line.strip()
        if stripped and len(stripped.split()) >= 2:
            parts = stripped.split(None, 1)
            if parts[0].isdigit():
                count = int(parts[0])
                symbol = parts[1] if len(parts) > 1 else "unknown"
                if count > 5:  # Filter noise
                    hotspots.append((symbol, count, 0.0))

    # Calculate percentages
    if total_samples > 0:
        hotspots = [(sym, count, count/total_samples*100) for sym, count, _ in hotspots]

    # Sort by count
    hotspots.sort(key=lambda x: x[1], reverse=True)
    hotspots = hotspots[:top_n]

    # Print results
    print(f"{'Rank':<6} {'Samples':<10} {'% Total':<10} {'Function/Symbol'}")
    print("-" * 80)

    for i, (symbol, count, pct) in enumerate(hotspots, 1):
        # Clean up symbol name
        symbol_short = symbol[:60] + "..." if len(symbol) > 60 else symbol
        print(f"{i:<6} {count:<10} {pct:<10.2f} {symbol_short}")

    print("-" * 80)
    print(f"Total samples analyzed: {total_samples}")
    print()

    return hotspots


def generate_flamegraph_data(sample_file: Path, output_path: Path) -> bool:
    """Convert sample output to flamegraph format (folded stacks)."""
    if not sample_file.exists():
        return False

    print("\n" + "="*80)
    print("Generating Flamegraph Data")
    print("="*80)

    content = sample_file.read_text()
    lines = content.split('\n')

    # Parse call stacks from sample output
    # Sample output format varies, so we'll do a best-effort parse
    stacks = []
    current_stack = []

    for line in lines:
        stripped = line.strip()

        # Look for stack frames (usually indented or have specific patterns)
        if stripped and (
            stripped.startswith('0x') or
            '::' in stripped or
            'strata' in stripped.lower() or
            any(word in stripped for word in ['parse', 'loads', 'PyObject', 'PyDict', 'PyList'])
        ):
            # This looks like a stack frame
            # Clean it up
            frame = stripped.split()[0] if stripped.split() else stripped
            current_stack.append(frame)
        elif current_stack and not stripped:
            # End of stack
            if current_stack:
                stacks.append(current_stack[:])
            current_stack = []

    # Write folded stacks format: "frame1;frame2;frame3 count"
    folded_path = output_path.with_suffix('.folded')

    if stacks:
        stack_counts = {}
        for stack in stacks:
            key = ';'.join(reversed(stack))  # Flamegraph wants root at the start
            stack_counts[key] = stack_counts.get(key, 0) + 1

        with folded_path.open('w') as f:
            for stack, count in sorted(stack_counts.items(), key=lambda x: x[1], reverse=True):
                f.write(f"{stack} {count}\n")

        print(f"Flamegraph data (folded stacks) saved to: {folded_path}")
        print(f"Total unique stacks: {len(stack_counts)}")
        print(f"\nTo generate flamegraph SVG, use:")
        print(f"  flamegraph.pl {folded_path} > {output_path.with_suffix('.svg')}")
        print(f"  (Install: git clone https://github.com/brendangregg/FlameGraph)")
        return True
    else:
        print("Warning: Could not extract stack traces from sample output")
        print("Sample output may need manual processing for flamegraph generation")
        return False


def main() -> int:
    parser = argparse.ArgumentParser(description="Profile strata.loads with C++ profiler")
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("benchmarks/data/generated/large/users.json"),
        help="Path to JSON file",
    )
    parser.add_argument(
        "--iterations",
        type=int,
        default=100,
        help="Number of iterations (should run ~30+ seconds for good profiling)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("docs/benchmarks/strata_cpp_profile"),
        help="Output path prefix for profile data",
    )
    parser.add_argument(
        "--top-n",
        type=int,
        default=20,
        help="Show top N hotspots",
    )
    parser.add_argument(
        "--simple",
        action="store_true",
        help="Just run benchmark without profiling (for manual profiling)",
    )
    args = parser.parse_args()

    if not args.data.exists():
        print(f"Error: Data file not found: {args.data}")
        return 1

    # Ensure output directory exists
    args.output.parent.mkdir(parents=True, exist_ok=True)

    if args.simple:
        # Just run the benchmark for manual profiling
        run_simple_benchmark(args.data, args.iterations)
        return 0

    # Profile with sample
    success = profile_with_sample(args.data, args.iterations, args.output)

    if success:
        sample_file = args.output.with_suffix('.sample.txt')

        # Analyze the output
        hotspots = analyze_sample_output(sample_file, args.top_n)

        # Try to generate flamegraph data
        generate_flamegraph_data(sample_file, args.output)

    return 0


if __name__ == "__main__":
    sys.exit(main())
