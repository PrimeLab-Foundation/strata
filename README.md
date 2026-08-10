# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

**Version 2026.8.10.** The ground-up rebuild is complete: `loads`, `dumps`,
`load`, `dump`, cursor mode, JSONPath (`query`, `search`, `compile` — with a
streaming SAX search evaluator) and `config`, over single files, NDJSON and
whole directories, backed by fuzzing, two-layer coverage, and a PGO+LTO
release build. On the benchmark suite (vs orjson, msgspec, ujson, stdlib —
evidence under [docs/benchmarks/](docs/benchmarks/)) strata is **#1 in 63 of
81 rows**: every `query` and `search` row (3–100× ahead of
jmespath/jsonpath-ng pipelines), every NDJSON-loading row (~1.4×), file
loading on 13 of 15 rows, and both parsing and serialization on the headline
dataset at effectively every tier. The rows still behind — small-document
parsing and a few 1.0–1.2× serialization ties — are enumerated honestly in
[docs/benchmarking/SKILL.md](docs/benchmarking/SKILL.md). The docs under [docs/](docs/) are the complete specification:
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
