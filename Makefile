.PHONY: venv install build test test-cpp test-py bench coverage clean status

# --- Python ---
PYTHON ?= python3.14
VENV   ?= .venv/bin/$(PYTHON)
PIP    ?= .venv/bin/pip

# --- C++ ---
CXX      ?= clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Icpp/include

BUILD     := build
CPP_SRCS  := $(shell find cpp/src -name '*.cpp')
CPP_OBJS  := $(patsubst cpp/src/%.cpp,$(BUILD)/obj/%.o,$(CPP_SRCS))
TEST_SRCS := $(shell find cpp/tests -name 'test_*.cpp')
TEST_BINS := $(patsubst cpp/tests/%.cpp,$(BUILD)/%,$(TEST_SRCS))

FIND_EXCLUDE := ! -path './.venv/*' ! -path './.git/*' ! -path './.idea/*' \
                ! -path './.pytest_cache/*' ! -path './.ruff_cache/*' \
                ! -path './build/*' ! -path './*.egg-info/*'

# --- Core targets ---

build: $(TEST_BINS) test-cpp

$(BUILD)/obj/%.o: cpp/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD)/%: cpp/tests/%.cpp $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(CPP_OBJS) -o $@

test-cpp: $(TEST_BINS)
	@for t in $(TEST_BINS); do echo "--- $$t ---" && ./$$t || exit 1; done

test-py:
	$(VENV) -m pytest python/tests/ -v

test: test-cpp test-py

# --- Bench ---

BENCH_SRCS := $(shell find cpp/tests/bench -name 'bench_*.cpp' 2>/dev/null)
BENCH_BINS := $(patsubst cpp/tests/bench/%.cpp,$(BUILD)/bench/%,$(BENCH_SRCS))

$(BUILD)/bench/%: cpp/tests/bench/%.cpp $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(CPP_OBJS) -o $@

bench: $(BENCH_BINS)
	@mkdir -p docs/benchmarks/cpp_only
	@for b in $(BENCH_BINS); do ./$$b; done

# --- Coverage ---

COV_DIR     := $(BUILD)/coverage
COV_CFLAGS  := -std=c++23 -Wall -Wextra -Wpedantic -O0 -fprofile-instr-generate -fcoverage-mapping
COV_OBJS    := $(patsubst cpp/src/%.cpp,$(COV_DIR)/obj/%.o,$(CPP_SRCS))
COV_BINS    := $(patsubst cpp/tests/%.cpp,$(COV_DIR)/%,$(TEST_SRCS))

$(COV_DIR)/obj/%.o: cpp/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(COV_CFLAGS) $(INCLUDES) -c $< -o $@

$(COV_DIR)/%: cpp/tests/%.cpp $(COV_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(COV_CFLAGS) $(INCLUDES) $< $(COV_OBJS) -o $@

coverage: $(COV_BINS)
	@rm -f $(COV_DIR)/default*.profraw $(COV_DIR)/merged.profdata
	@cd $(COV_DIR) && for t in $(patsubst $(COV_DIR)/%,%,$(COV_BINS)); do \
		LLVM_PROFILE_FILE="default_$$(echo $$t | tr '/' '_').profraw" ./$$t > /dev/null || exit 1; \
	done
	@xcrun llvm-profdata merge -sparse $(COV_DIR)/default_*.profraw -o $(COV_DIR)/merged.profdata
	@echo ""
	@echo "=== C++ Test Coverage ==="
	@echo ""
	@xcrun llvm-cov report $(firstword $(COV_BINS)) \
		$(patsubst %,-object %,$(wordlist 2,$(words $(COV_BINS)),$(COV_BINS))) \
		-instr-profile=$(COV_DIR)/merged.profdata \
		$(CPP_SRCS) \
		| tail -n +2
	@echo ""

# --- Python ---

venv:
	$(PYTHON) -m venv .venv
	$(VENV) -m pip install -U pip setuptools

install:
	$(PIP) install -e .

# --- Utility ---

clean:                           ## Remove build artifacts
	rm -rf $(BUILD)

status:
	@echo "=== Strata Project Status ==="
	@echo ""
	@echo "-- Python --"
	@which $(PYTHON) 2>/dev/null && $(PYTHON) --version || echo "$(PYTHON) not found"
	@test -d .venv && echo "venv: OK" || echo "venv: missing (run make venv)"
	@echo ""
	@echo "-- Sources --"
	@printf "  .cpp: %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.cpp' 2>/dev/null | wc -l)"
	@printf "  .hpp: %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.hpp' -o -name '*.h' 2>/dev/null | wc -l)"
	@printf "  .py:  %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.py' 2>/dev/null | wc -l)"
	@echo ""
	@echo "-- Lines of code --"
	@find . $(FIND_EXCLUDE) \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.py' \) 2>/dev/null | xargs cat 2>/dev/null | wc -l | xargs echo " "
	@echo ""
	@echo "-- Tree --"
	@find . -maxdepth 4 \
		\( -name .venv -o -name .git -o -name .idea -o -name .pytest_cache \
		   -o -name .ruff_cache -o -name build -o -name '*.egg-info' \) -prune \
		-o ! -name '.' -print | sort | sed 's|[^/]*/|  |g'