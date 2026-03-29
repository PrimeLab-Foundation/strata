.PHONY: venv install status

PYTHON ?= python3.14
VENV ?= .venv/bin/$(PYTHON)
PIP ?= .venv/bin/pip

FIND_EXCLUDE := ! -path './.venv/*' ! -path './.git/*' ! -path './.idea/*' \
                ! -path './.pytest_cache/*' ! -path './.ruff_cache/*' \
                ! -path './build/*' ! -path './*.egg-info/*'

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
	@printf "  .cpp: %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.cpp' 2>/dev/null | wc -l)"
	@printf "  .hpp: %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.hpp' -o -name '*.h' 2>/dev/null | wc -l)"
	@printf "  .py:  %s\n" "$$(find . $(FIND_EXCLUDE) -name '*.py' 2>/dev/null | wc -l)"
	@echo ""
	@echo "-- Lines of code --"
	@find . $(FIND_EXCLUDE) \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.py' \) 2>/dev/null | xargs cat 2>/dev/null | wc -l | xargs echo " "
	@echo ""
	@echo "-- Tree --"
	@find . -maxdepth 3 \
		\( -name .venv -o -name .git -o -name .idea -o -name .pytest_cache \
		   -o -name .ruff_cache -o -name build -o -name '*.egg-info' \) -prune \
		-o ! -name '.' -print | sort | sed 's|[^/]*/|  |g'