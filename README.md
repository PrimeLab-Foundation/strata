# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

**Rebuild in progress.** The implementation is being rebuilt from scratch, one
milestone at a time. Working today: `loads`, `dumps`, `load`, `dump`, cursor
mode, JSONPath (`query`, `search`, `compile` — including the streaming SAX
search evaluator) and `config`, over single files, NDJSON and whole
directories, with fuzzing, coverage and a PGO+LTO build in place. On the
benchmark suite strata leads every `query` and `search` row (3–100× against
jmespath/jsonpath-ng pipelines), NDJSON loading (~1.3× ahead of msgspec), and
file/in-memory parsing on most datasets including the headline one;
serialization still trails orjson by ~1.1–1.6×, which is the remaining gap
before release. The docs under [docs/](docs/) are the complete specification:
conventions, style, public API contract, architecture, benchmarking
methodology, the optimization playbook (including negative results), and
project history.

```python
import strata

strata.loads('{"n": 12345678901234567890}')   # {'n': 12345678901234567890} - exact
strata.dumps({"a": [1, 2.5, None]})           # '{"a":[1,2.5,null]}'
strata.config.set("duplicate_key_policy", "last")

strata.load("records.ndjson", skip_errors=True)     # one document per line
cursor = strata.load("big.json", return_type="cursor")
cursor.field("users").at(0).field("name").get_str()  # nothing else parsed

strata.query(data, "$.users[?(@.age > 30)].name")   # JSONPath over objects
strata.search("data.json", "$..price")              # ...or over a file

strata.dump(records, "out/", split_by=["region", "team"])  # out/eu/red.json, ...
strata.load("out/")                                        # every record back
strata.search("out/", "$..price")                          # across the tree
```

Every entry point normalizes `Path` and `str` alike, raises `ValueError` on
invalid JSON and `TypeError` on unsupported types, and parses integers exactly
at any size. The full contract — including the folder round-trip law and the
supported JSONPath subset — is [docs/context/api.md](docs/context/api.md).

## Development

```bash
make dev        # virtualenv, dev dependencies, pre-commit hooks
make install    # editable install — C++ tests gate the build, Python tests gate the result
make test       # both layers: ctest + pytest
make fmt lint   # ruff format + clang-format; ruff check
make gate       # full compliance pass: C++ tests, reinstall, Python tests, coverage
```

```bash
make coverage   # llvm-cov over the C++ suites + pytest-cov over the facade
make fuzz       # libFuzzer over the committed seed corpus (FUZZ_TIME=120)
make bench-all  # datasets + every tier vs orjson/msgspec/ujson → docs/benchmarks/
make pgo        # two-phase PGO+LTO build; the gate runs on both phases
```

`make fuzz` needs a toolchain that ships the libFuzzer runtime — Apple's clang
does not, so on macOS install LLVM (`brew install llvm`) or rely on
`make test`, which replays the same corpus through the engine on every run.

- Start here: [CLAUDE.md](CLAUDE.md)
- Public API contract: [docs/context/api.md](docs/context/api.md)
- Previous implementation (v0.2.0, all tests green, #1 in most benchmark
  categories vs orjson/msgspec/ujson): branch `backup/pre-reset-main` and the
  repos under `../archive/`

## License

MIT — see [LICENSE](LICENSE).
