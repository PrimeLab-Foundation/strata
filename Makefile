# Strata — the single user-facing interface.
#
# Targets stay thin and forward to scripts/ (docs/context/convention.md,
# "Automation"). Docs and CI reference these targets, never ad-hoc one-liners.
# Benchmark, fuzz, coverage and PGO targets arrive with their tooling in the
# milestones that build it (see docs/roadmap/SKILL.md).

PYTHON ?= python3
VENV ?= .venv
VPY := $(VENV)/bin/python

.PHONY: all venv dev install install-dev install-skip-tests build cpp-build \
        test test-py test-cpp fmt lint pre-commit-check gate \
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

gate: venv  ## Full compliance gate: C++ tests, reinstall, Python tests
	@bash scripts/gate.sh

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
	rm -rf build build_* dist *.egg-info .pytest_cache
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
