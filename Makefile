# Strata — the single user-facing interface.
#
# Targets stay thin and forward to scripts/ (docs/context/convention.md,
# "Automation"). Docs and CI reference these targets, never ad-hoc one-liners.
# Every target forwards to a script; none of them is a one-liner in disguise.

PYTHON ?= python3
VENV ?= .venv
VPY := $(VENV)/bin/python

.PHONY: all venv dev install install-dev install-bench install-skip-tests build cpp-build \
        test test-py test-cpp fmt lint pre-commit-check gate \
        coverage coverage-cpp coverage-py fuzz fuzz-build fuzz-run pgo \
        bench-data bench-small bench-medium bench-large bench-all bench-baseline \
        clean clean-venv scripts-executable help

all: test  ## Run every test suite (default target)

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

bench-baseline: venv  ## Record the small tier as the regression baseline
	PYTHONPATH=. $(VPY) -m benchmarks.regression_check \
		$(BENCH_REPORTS)/bench_results_small.md --save-baseline

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
