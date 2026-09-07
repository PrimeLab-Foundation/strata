# Strata — the single user-facing interface.
#
# Targets stay thin and forward to scripts/ (docs/context/convention.md,
# "Automation"). Docs and CI reference these targets, never ad-hoc one-liners.
# Every target forwards to a script; none of them is a one-liner in disguise.

PYTHON ?= python3
VENV ?= .venv
VPY := $(VENV)/bin/python

.PHONY: all venv dev install install-dev install-bench install-skip-tests build cpp-build \
        test test-py test-py-asan test-cpp fmt lint pre-commit-check gate \
        coverage coverage-cpp coverage-py fuzz fuzz-build fuzz-run pgo \
        bench-data bench-small bench-medium bench-large bench-all bench-baseline bench-check bench-supplementary \
        bench-ci bench-ci-summary probe-dumps-records probe-dumps-call probe-ab-builds probe-ab-rows \
        probe-ab-analyze probe-ab-floor probe-file-costs probe-schema-recovery bench-supportability \
        clean clean-venv scripts-executable help

all: test  ## Run every test suite (default target)

BENCH_SUPPLEMENTARY_TIER ?= small
BENCH_REPEAT ?= 10
BENCH_WARMUP ?= 2
FILE_COST_DATASET ?= benchmarks/data/generated/small/mixed.json
FILE_COST_OUTPUT ?= build/evidence/file-costs.json
SCHEMA_RECOVERY_OUTPUT ?= build/evidence/schema-recovery.json
probe-schema-recovery: venv  ## Diagnose the effect of unrelated schema churn on later serialization
	PYTHONPATH=. $(VPY) -m benchmarks.schema_recovery --repeat $(PROBE_REPEAT) --output $(SCHEMA_RECOVERY_OUTPUT)
probe-file-costs: venv  ## Real-file diagnostic phase controls with raw samples
	PYTHONPATH=. $(VPY) -m benchmarks.file_costs --dataset $(FILE_COST_DATASET) --output $(FILE_COST_OUTPUT) --repeat $(PROBE_REPEAT)
bench-supplementary: venv  ## NDJSON search and folder controls; separate supplementary v1 scope
	PYTHONPATH=. $(VPY) -m benchmarks.supplementary --data benchmarks/data/generated/$(BENCH_SUPPLEMENTARY_TIER) --repeat $(BENCH_REPEAT) --warmup $(BENCH_WARMUP) --output $(BENCH_REPORTS)/supplementary_v1_$(BENCH_SUPPLEMENTARY_TIER).md

bench-supportability: venv  ## Validate a complete canonical report against the 3.0x tripwire
	PYTHONPATH=. $(VPY) -m benchmarks.supportability_check $(BENCH_REPORT)

# ---------------------------------------------------------------------------
# Setup
# ---------------------------------------------------------------------------

$(VENV)/bin/python:
	$(PYTHON) -m venv $(VENV)
	$(VPY) -m pip install -U pip setuptools wheel

venv: $(VENV)/bin/python  ## Create the development virtualenv (idempotent)

dev: venv  ## venv + dev dependencies + installed pre-commit hooks
	$(VPY) -m pip install -e '.[dev]'
	$(VENV)/bin/pre-commit install

install: venv  ## Editable install; C++ tests gate the build, Python tests gate the result
	$(VPY) -m pip install -e .

install-dev: venv  ## Editable install with the dev extras
	$(VPY) -m pip install -e '.[dev]'

install-bench: venv  ## Editable install with the benchmark competitors
	$(VPY) -m pip install -e '.[dev,bench]'

install-skip-tests: venv  ## Ungated install — strongly discouraged, banned in CI and releases
	@echo "WARNING: SKIP_TESTS=1 — this build is not release-ready."
	SKIP_TESTS=1 $(VPY) -m pip install -e .

# ---------------------------------------------------------------------------
# Build
# ---------------------------------------------------------------------------

build: venv  ## Build the sdist and wheel
	$(VPY) -m pip install -U build
	$(VPY) -m build

cpp-build: venv  ## Configure and build the C++ tests without running them
	$(VPY) scripts/cpp_tests.py --build-only

# ---------------------------------------------------------------------------
# Test
# ---------------------------------------------------------------------------

test: test-cpp test-py  ## Run both layers: C++ via ctest, Python via pytest

test-cpp: venv  ## Run the C++ suite through the CMake/ctest registry
	$(VPY) scripts/cpp_tests.py

test-py: venv  ## Run tests/py (integration) and tests/unit (contract)
	$(VPY) scripts/py_tests.py

# Not part of `test`: the sanitized build is a separate, several-minute
# toolchain run in its own virtualenv, and `make test` is the fast inner loop.
# CI runs it in the corpus job, beside the sanitized C++ suites.
test-py-asan:  ## Build the extension with ASan+UBSan in .venv-asan and run both Python suites
	@bash scripts/asan_py_tests.sh

gate: venv  ## Full compliance gate: C++ tests, reinstall, Python tests, coverage
	@bash scripts/gate.sh

# ---------------------------------------------------------------------------
# Coverage
# ---------------------------------------------------------------------------

coverage: coverage-cpp coverage-py  ## Coverage for both layers

coverage-cpp: venv  ## C++ coverage via llvm-cov over the CMake registry
	@bash scripts/coverage.sh cpp

coverage-py: venv  ## Python coverage via pytest-cov
	@bash scripts/coverage.sh py

# ---------------------------------------------------------------------------
# Fuzzing
#
# The seed corpus in tests/fuzz/corpus/ is committed, and fuzz_corpus_tests
# replays it on every `make test`. libFuzzer itself needs a toolchain that
# ships the runtime — Apple's clang does not (see docs/build-and-test/SKILL.md).
# ---------------------------------------------------------------------------

fuzz-build:  ## Build the libFuzzer targets (-DFUZZ=ON)
	@bash scripts/fuzz.sh build

fuzz-run:  ## Run both fuzz targets over the committed corpus (FUZZ_TIME=120)
	@bash scripts/fuzz.sh run

fuzz: ## Build and run the fuzz targets
	@bash scripts/fuzz.sh all

# ---------------------------------------------------------------------------
# PGO
# ---------------------------------------------------------------------------

pgo: venv  ## Two-phase PGO+LTO build; gate tests run on both phases
	@bash scripts/pgo_build.sh

# ---------------------------------------------------------------------------
# Benchmarks
#
# Datasets are generated, gitignored and deterministic (seed 42). Reports are
# machine-written to docs/benchmarks/ and must never be hand-edited.
# ---------------------------------------------------------------------------

BENCH_DIR := benchmarks/data/generated
BENCH_REPORTS := docs/benchmarks
BENCH_RUN := PYTHONPATH=. $(VPY) -m benchmarks.bench_main
BENCH_GEN := PYTHONPATH=. $(VPY) -m benchmarks.data.generate_bench_data
PROBE_CALLS ?= 400
PROBE_BLOCKS ?= 3
PROBE_REPEAT ?= 60
PROBE_OUT ?= build/evidence/ab_rounds.tsv

bench-data: venv  ## Generate the small, medium and large datasets
	$(BENCH_GEN) --out-dir $(BENCH_DIR)/small  --num-users 1000 --max-orders 10 --max-items 5  --records 500
	$(BENCH_GEN) --out-dir $(BENCH_DIR)/medium --num-users 2000 --max-orders 20 --max-items 10 --records 2000
	$(BENCH_GEN) --out-dir $(BENCH_DIR)/large  --num-users 4000 --max-orders 40 --max-items 20 --records 5000

bench-small: venv  ## Benchmark the small tier
	$(BENCH_RUN) --name small --repeat 10 --warmup 2 \
		$(foreach f,users.json users.ndjson flat.json nested.json wide_arrays.json mixed.json,--dataset $(BENCH_DIR)/small/$(f)) \
		--output $(BENCH_REPORTS)/bench_results_small.md

bench-medium: venv  ## Benchmark the medium tier
	$(BENCH_RUN) --name medium --repeat 10 --warmup 2 \
		$(foreach f,users.json users.ndjson flat.json nested.json wide_arrays.json mixed.json,--dataset $(BENCH_DIR)/medium/$(f)) \
		--output $(BENCH_REPORTS)/bench_results_medium.md

bench-large: venv  ## Benchmark the large tier
	$(BENCH_RUN) --name large --repeat 10 --warmup 2 \
		$(foreach f,users.json users.ndjson flat.json nested.json wide_arrays.json mixed.json,--dataset $(BENCH_DIR)/large/$(f)) \
		--output $(BENCH_REPORTS)/bench_results_large.md

bench-all: bench-data bench-small bench-medium bench-large  ## Data plus every tier

bench-baseline: venv  ## Record BENCH_REPORT as BENCH_BASELINE (defaults to the small tier)
	PYTHONPATH=. $(VPY) -m benchmarks.regression_check \
		$(BENCH_REPORT) --baseline $(BENCH_BASELINE) --save-baseline

# The gate itself, through the one user-facing interface: >2% median/p95 or
# >5% RSS against benchmarks/results/baseline.json is fix-or-revert, and a
# comparison that covers less than the baseline's scope for this report is
# missing evidence, not a pass (docs/context/benchmarks.md).
BENCH_REPORT ?= $(BENCH_REPORTS)/bench_results_small.md
BENCH_BASELINE ?= benchmarks/results/baseline.json

bench-check: venv  ## Gate a benchmark report against the recorded baseline (BENCH_REPORT)
	PYTHONPATH=. $(VPY) -m benchmarks.regression_check $(BENCH_REPORT) --baseline $(BENCH_BASELINE)

# Flags forwarded to both halves of the CI standings pipeline. They share
# --expect, --expect-platforms and --allow-incomplete, and a deliberately
# scoped run has to say so to both: fetching a partial run and then summarizing
# it as if it were complete is exactly the false pass these gates exist to
# stop. E.g. `make bench-ci BENCH_CI_FLAGS=--allow-incomplete`.
BENCH_CI_FLAGS ?=

bench-ci: venv  ## Fetch the latest CI run's per-platform reports and rebuild the standings summary (BENCH_CI_FLAGS)
	PYTHONPATH=. $(VPY) -m benchmarks.ci_fetch $(BENCH_CI_FLAGS)
	PYTHONPATH=. $(VPY) -m benchmarks.ci_summary $(BENCH_CI_FLAGS)

bench-ci-summary: venv  ## Rebuild docs/benchmarks/ci_summary.md from the already-fetched reports (BENCH_CI_FLAGS)
	PYTHONPATH=. $(VPY) -m benchmarks.ci_summary $(BENCH_CI_FLAGS)

# ---------------------------------------------------------------------------
# Diagnostic probes
#
# Not benchmarks: these answer "why", not "how fast", and none of them writes
# to docs/benchmarks/. They exist because the official protocol -- ten repeats,
# five engines, one fixed order -- is the right instrument for a rank and the
# wrong one for a two-microsecond difference. Each keeps its raw samples.
# ---------------------------------------------------------------------------

PROBE_RUN := PYTHONPATH=. $(VPY)

probe-dumps-records: venv  ## Decompose dumps mixed's records, hot / gc / cold, with matched controls
	$(PROBE_RUN) benchmarks/decompose_dumps_records.py

probe-dumps-call: venv  ## Time the dumps call alone, preamble outside the timer, against its matched `none` arm
	@for engine in strata orjson none-strata none-orjson; do \
		$(PROBE_RUN) benchmarks/dumps_loop.py $$engine $(PROBE_CALLS) gc-call; \
	done

probe-ab-builds: venv  ## A-B-B-A rounds over two extension builds (BUILD_A, BUILD_B, PROBE_OUT)
	$(PROBE_RUN) benchmarks/ab_builds.py \
		--build A=$(BUILD_A) --build B=$(BUILD_B) \
		--target $(shell $(VPY) -c 'import strata._strata as m; print(m.__file__)') \
		--out $(PROBE_OUT) --blocks $(PROBE_BLOCKS) --repeat $(PROBE_REPEAT)

# tier:dataset:op — op is dumps, loads, load, ndload or dump.
PROBE_ROWS ?= --row small:mixed:dumps --row small:mixed:loads

probe-ab-rows: venv  ## A-B-B-A rounds over two builds on named tier:dataset:op rows (BUILD_A, BUILD_B, PROBE_ROWS, PROBE_OUT)
	$(PROBE_RUN) benchmarks/ab_rows.py \
		--build A=$(BUILD_A) --build B=$(BUILD_B) \
		--target $(shell $(VPY) -c 'import strata._strata as m; print(m.__file__)') \
		--out $(PROBE_OUT) $(PROBE_ROWS) --blocks $(PROBE_BLOCKS) --repeat $(PROBE_REPEAT)

# The one A/B analysis: effect, block-bootstrap interval and, with PROBE_AA,
# the A/A floor in the same estimator (benchmarks/ab_blocks.py).
PROBE_AA ?=

probe-ab-analyze: venv  ## Read a driver TSV: effect + interval + A/A floor (PROBE_OUT, PROBE_AA)
	$(PROBE_RUN) benchmarks/ab_blocks.py $(PROBE_OUT) $(if $(PROBE_AA),--aa $(PROBE_AA),)

probe-ab-floor: venv  ## Read an A/A TSV as the session's resolution floor (PROBE_OUT)
	$(PROBE_RUN) benchmarks/ab_floor.py $(PROBE_OUT)

# ---------------------------------------------------------------------------
# Lint / format
# ---------------------------------------------------------------------------

fmt:  ## Format everything: ruff for Python, clang-format for C/C++
	@bash scripts/fmt.sh

lint:  ## Lint the Python sources with ruff
	@bash scripts/lint.sh

pre-commit-check: venv  ## Run every pre-commit hook over the whole tree
	$(VENV)/bin/pre-commit run --all-files --show-diff-on-failure

# ---------------------------------------------------------------------------
# Misc
# ---------------------------------------------------------------------------

clean:  ## Remove build, packaging and test artefacts (keeps the virtualenv)
	rm -rf build build_* dist *.egg-info .pytest_cache .coverage htmlcov fuzz_crashes
	find . -name '__pycache__' -type d -prune -exec rm -rf {} +

clean-venv:  ## Remove the virtualenv
	rm -rf $(VENV)

scripts-executable:  ## chmod +x scripts/ so they can be run directly
	@chmod +x scripts/*.sh scripts/*.py 2>/dev/null || true
	@echo "scripts/ is executable."

help:  ## Show this help
	@echo "Strata — make targets"
	@echo ""
	@grep -hE '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
		| sort \
		| awk 'BEGIN {FS = ":.*?## "}; {printf "  %-20s %s\n", $$1, $$2}'
