.PHONY: all venv install build test test-cpp test-py bench bench-compare coverage vendor vendor-nlohmann vendor-rapidjson vendor-simdjson vendor-clean clean status

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
                ! -path './build/*' ! -path './*.egg-info/*' ! -path '*/vendor/*'

# --- Core targets ---
all: clean build

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

BENCH_INC := -Icpp/tests/bench

$(BUILD)/bench/bench_parse: cpp/tests/bench/bench_parse.cpp $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BENCH_INC) $< $(CPP_OBJS) -o $@

bench: $(BUILD)/bench/bench_parse
	@mkdir -p docs/benchmarks/cpp_only
	@./$(BUILD)/bench/bench_parse

VENDOR_FLAGS :=
ifneq (,$(wildcard cpp/vendor/nlohmann/json.hpp))
    VENDOR_FLAGS += -DHAS_NLOHMANN -Icpp/vendor
endif
ifneq (,$(wildcard cpp/vendor/rapidjson/include/rapidjson/document.h))
    VENDOR_FLAGS += -DHAS_RAPIDJSON -Icpp/vendor/rapidjson/include
endif
ifneq (,$(wildcard cpp/vendor/simdjson/singleheader/simdjson.h))
    VENDOR_FLAGS += -DHAS_SIMDJSON -Icpp/vendor/simdjson/singleheader
endif

SIMDJSON_OBJ :=
ifneq (,$(wildcard cpp/vendor/simdjson/singleheader/simdjson.cpp))
    SIMDJSON_OBJ := $(BUILD)/obj/vendor/simdjson.o
endif

$(BUILD)/obj/vendor/simdjson.o: cpp/vendor/simdjson/singleheader/simdjson.cpp
	@mkdir -p $(dir $@)
	$(CXX) -std=c++23 -O2 -Icpp/vendor/simdjson/singleheader -c $< -o $@

$(BUILD)/bench/bench_compare: cpp/tests/bench/bench_compare.cpp $(CPP_OBJS) $(SIMDJSON_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BENCH_INC) $(VENDOR_FLAGS) $< $(CPP_OBJS) $(SIMDJSON_OBJ) -o $@

bench-compare: $(BUILD)/bench/bench_compare
	@./$(BUILD)/bench/bench_compare

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

# --- Vendor ---

vendor: vendor-clean vendor-nlohmann vendor-rapidjson vendor-simdjson

vendor-nlohmann:
	@mkdir -p cpp/vendor/nlohmann
	@if [ ! -f cpp/vendor/nlohmann/json.hpp ]; then \
		echo "downloading nlohmann/json..."; \
		curl -sL https://github.com/nlohmann/json/releases/latest/download/json.hpp \
			-o cpp/vendor/nlohmann/json.hpp; \
		echo "  done"; \
	else \
		echo "nlohmann/json: already vendored"; \
	fi

vendor-rapidjson:
	@if [ ! -d cpp/vendor/rapidjson ]; then \
		echo "downloading rapidjson..."; \
		git clone --depth 1 -q https://github.com/Tencent/rapidjson.git cpp/vendor/rapidjson; \
		echo "  done"; \
	else \
		echo "rapidjson: already vendored"; \
	fi

vendor-simdjson:
	@if [ ! -d cpp/vendor/simdjson ]; then \
		echo "downloading simdjson..."; \
		git clone --depth 1 -q https://github.com/simdjson/simdjson.git cpp/vendor/simdjson; \
		echo "  done"; \
	else \
		echo "simdjson: already vendored"; \
	fi

vendor-clean:
	rm -rf cpp/vendor

# --- Python ---

venv:
	$(PYTHON) -m venv .venv
	$(VENV) -m pip install -U pip setuptools

install:
	$(PIP) install -e .

# --- Utility ---

clean:                           ## Remove build artifacts
	rm -rf $(BUILD)

lint:
	clang-tidy $(CPP_SRCS) -- $(CXXFLAGS) $(INCLUDES)

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
		   -o -name .ruff_cache -o -name build -o -name '*.egg-info' -o -name vendor \) -prune \
		-o ! -name '.' -print | sort | sed 's|[^/]*/|  |g'