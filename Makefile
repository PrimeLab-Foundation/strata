.PHONY: dev fmt lint typecheck test build bench-data bench-small bench-all clean

PYTHON ?= python3.14
VENV ?= .venv


venv:
	$(PYTHON) -m venv $(VENV)
	@echo "Virtual environment created in $(VENV)"

dev: venv
	$(VENV)/bin/$(PYTHON) -m pip install -U pip setuptools pre-commit
	$(VENV)/bin/pre-commit install
	@echo "Installed pip setuptools pre-commit"

install: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e .

install-dev: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[dev]'

install-bench: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[bench]'

install-all: venv
	$(VENV)/bin/$(PYTHON) -m pip install -e '.[dev,bench]'

fmt:
	$(VENV)/bin/ruff format .
	$(VENV)/bin/clang-format -i $(shell git ls-files '*.cpp' '*.hpp' '*.h' '*.cc' '*.cxx' 2>/dev/null)

lint:
	$(VENV)/bin/ruff check .

typecheck:
	$(VENV)/bin/mypy src/strata

pre-commit-check:
	$(VENV)/bin/pre-commit run --all-files --show-diff-on-failure

test:
	pytest -q

build:
	$(PYTHON) -m pip install -U build
	$(PYTHON) -m build

clean:
	rm -rf build dist *.egg-info _skbuild benchmarks/artifacts __pycache__ */__pycache__ .venv


# BENCH DATA GENERATION
bench-data:
	$(PYTHON) -m benchmarks.data.generate_bench_data \
		--out-dir benchmarks/data/generated \
		--num-users 5000 \
		--max-orders 10 \
		--max-items 5

bench-small:
	$(MAKE) bench-data
	$(PYTHON) -m benchmarks.bench_main \
		--dataset benchmarks/data/generated/users.json \
		--dataset benchmarks/data/generated/users.ndjson \
		--limit 500 --repeat 2 \
		--mode auto --materialize auto \
		--html

bench-all:
	$(MAKE) bench-data
	$(PYTHON) -m benchmarks.bench_main \
		--dataset benchmarks/data/generated/users.json \
		--dataset benchmarks/data/generated/users.ndjson \
		--repeat 1 --mode auto --materialize auto --strict-missing off \
		--html


