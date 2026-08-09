# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

**Fresh start.** This branch intentionally contains documentation only — the
implementation is being rebuilt from scratch. The docs under [docs/](docs/) are
the complete specification: conventions, style, public API contract,
architecture, benchmarking methodology, the optimization playbook (including
negative results), and project history.

- Start here: [CLAUDE.md](CLAUDE.md)
- Public API contract: [docs/context/api.md](docs/context/api.md)
- Previous implementation (v0.2.0, all tests green, #1 in most benchmark
  categories vs orjson/msgspec/ujson): branch `backup/pre-reset-main` and the
  repos under `../archive/`

## License

MIT — see [LICENSE](LICENSE).
