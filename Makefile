PYTHON ?= python3.14
VENV ?= .venv/bin/$(PYTHON)
PIP ?= .venv/bin/pip


venv:
	$(PYTHON) -m venv .venv
	$(VENV) -m pip install -U pip setuptools

install-py:
	$(PIP) install -e .