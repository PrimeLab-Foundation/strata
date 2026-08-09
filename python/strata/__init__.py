"""Strata — fast JSON for Python.

Parsing, serialization, and JSONPath querying powered by a dependency-free
C++20 engine with hand-written CPython C-API bindings.

The public surface contracted in ``docs/context/api.md`` is added milestone by
milestone; this build exposes ``__version__`` only. Importing the package pulls
in the native engine, so a package that imports is a package whose extension
was built.
"""

# The engine is imported for its side effect: `import strata` must fail loudly
# when the extension is missing rather than degrade to a Python fallback
# (docs/context/convention.md, "No dependency fallbacks").
from . import _strata as _native  # noqa: F401

# Single source of truth for the version (docs/context/api.md § Versioning).
# pyproject.toml reads this literal dynamically — never add a second copy.
__version__ = "2026.8.9"

__all__ = ["__version__"]
