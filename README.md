# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

**Rebuild in progress.** The implementation is being rebuilt from scratch, one
milestone at a time. Working today: `loads`, `dumps`, `load`, `dump`, cursor
mode and `config`, over JSON and NDJSON. Folder mode and JSONPath are still to
come, and the performance layer is only partly rebuilt, so this is not yet a
drop-in replacement for the previous release. The docs under [docs/](docs/) are the complete
specification: conventions, style, public API contract, architecture,
benchmarking methodology, the optimization playbook (including negative
results), and project history.

```python
import strata

strata.loads('{"n": 12345678901234567890}')   # {'n': 12345678901234567890} - exact
strata.dumps({"a": [1, 2.5, None]})           # '{"a":[1,2.5,null]}'
strata.config.set("duplicate_key_policy", "last")

strata.load("records.ndjson", skip_errors=True)     # one document per line
cursor = strata.load("big.json", return_type="cursor")
cursor.field("users").at(0).field("name").get_str()  # nothing else parsed
```

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
