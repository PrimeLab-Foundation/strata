.PHONY: dev fmt lint typecheck test build bench-data bench-small bench-all clean

PYTHON ?= python3.14
VENV ?= .venv
VERSION ?= 0.1.0

tag-create:
	@git tag $(VERSION)
	@git push --tags
	sed -i.bak -E "s/^version = \"[^\"]+\"/version = \"${VERSION}\"/" pyproject.toml


tag-delete:
	@git tag -d $(VERSION)
	@git push origin --delete $(VERSION)

tag-update: tag-delete tag-create

venv:
	$(PYTHON) -m venv $(VENV)
	@echo "Virtual environment created in $(VENV)"

dev: venv
	$(VENV)/bin/$(PYTHON) -m pip install -U pip setuptools wheel pybind11 pre-commit
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

.PHONY: cmake-check cpp-build cpp-test

cmake-check:
	@command -v cmake >/dev/null 2>&1 || { \
		echo "Error: cmake not found in PATH."; \
		echo ""; \
		echo "Install CMake and re-run this command."; \
		if [ "$(UNAME_S)" = "Darwin" ]; then \
			echo "  macOS (Homebrew):   brew install cmake"; \
			echo "  macOS (installer):  https://cmake.org/download/"; \
		elif [ "$(UNAME_S)" = "Linux" ]; then \
			echo "  Debian/Ubuntu:      sudo apt-get install cmake"; \
			echo "  Fedora:             sudo dnf install cmake"; \
			echo "  Arch:               sudo pacman -S cmake"; \
		else \
			echo "  Other platforms:    see https://cmake.org/download/"; \
		fi; \
		exit 1; \
	}


cpp-build: cmake-check
	mkdir -p build
	cd build && cmake .. && cmake --build .

cpp-test: cpp-build
	cd build && ctest --output-on-failure

build:
	$(PYTHON) -m pip install -U build
	$(PYTHON) -m build

clean:
	rm -rf build dist *.egg-info _skbuild benchmarks/artifacts __pycache__ */__pycache__

clean-venv:
	rm -rf .venv


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


