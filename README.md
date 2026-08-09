# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

**Rebuild in progress.** The implementation is being rebuilt from scratch, one
milestone at a time. The build, both test layers, the style gates, a CI skeleton
and the C++ engine's value model and JSON parser are in place; nothing is
exposed to Python yet, so there is no working JSON API. The docs under [docs/](docs/) are the complete
specification: conventions, style, public API contract, architecture,
benchmarking methodology, the optimization playbook (including negative
results), and project history.

```bash
make dev      # virtualenv, dev dependencies, pre-commit hooks
make install  # editable install — C++ tests gate the build, Python tests gate the result
make test     # both layers: ctest + pytest
make fmt lint # ruff format + clang-format; ruff check
```

- Start here: [CLAUDE.md](CLAUDE.md)
- Public API contract: [docs/context/api.md](docs/context/api.md)
- Previous implementation (v0.2.0, all tests green, #1 in most benchmark
  categories vs orjson/msgspec/ujson): branch `backup/pre-reset-main` and the
  repos under `../archive/`

## License

MIT — see [LICENSE](LICENSE).
