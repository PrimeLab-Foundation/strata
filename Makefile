.PHONY: venv install status

PYTHON ?= python3.14
VENV ?= .venv/bin/$(PYTHON)
PIP ?= .venv/bin/pip


venv:
	$(PYTHON) -m venv .venv
	$(VENV) -m pip install -U pip setuptools

install:
	$(PIP) install -e .

status:
	@echo "=== Strata Project Status ==="
	@echo ""
	@echo "-- Python --"
	@which $(PYTHON) 2>/dev/null && $(PYTHON) --version || echo "$(PYTHON) not found"
	@test -d .venv && echo "venv: OK" || echo "venv: missing (run make venv)"
	@echo ""
	@echo "-- Sources --"
	@printf "  .cpp: %s\n" "$$(find . -name '*.cpp' ! -path './.venv/*' 2>/dev/null | wc -l)"
	@printf "  .hpp: %s\n" "$$(find . -name '*.hpp' ! -path './.venv/*' 2>/dev/null | wc -l)"
	@printf "  .py:  %s\n" "$$(find . -name '*.py'  ! -path './.venv/*' 2>/dev/null | wc -l)"
	@echo ""
	@echo "-- Lines of code --"
	@find . \( -name '*.cpp' -o -name '*.hpp' -o -name '*.py' \) ! -path './.venv/*' 2>/dev/null | xargs cat 2>/dev/null | wc -l || echo "0"
	@echo ""
	@echo "-- Tree (depth 3) --"
	@find . -maxdepth 3 ! -path './.venv/*' ! -path './.git/*' ! -path './build/*' ! -name '.' | sort | sed 's|[^/]*/|  |g'