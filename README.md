# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by
a dependency-free C++20 engine with hand-written CPython C-API bindings.

The ground-up rebuild is complete: `loads`, `dumps`,
`load`, `dump`, cursor mode, JSONPath (`query`, `search`, `compile` — with a
streaming SAX search evaluator) and `config`, over single files, NDJSON and
whole directories, backed by fuzzing, two-layer coverage, and a PGO+LTO
release build. On the CI benchmark suite (27 rows on each of five platforms,
against orjson, msgspec, ujson and stdlib json, and jmespath and jsonpath-ng
for JSONPath), the complete sweep of 2026-09-18 read strata **#1 in all 135
rows**. Other draws typically read 131–134 of 135, the misses rotating among
cells inside the measurement noise (evidence under
[docs/benchmarks/](https://github.com/PrimeLab-Foundation/strata/tree/main/docs/benchmarks)).
Those benchmark builds compile for each runner's own CPU (`-march=native`,
Windows apart), while the Linux and macOS wheels target portable baselines
(x86-64-v3 on x86_64), so a wheel's speed can differ from the published
figures; on Linux and macOS, `make pgo` in a checkout reproduces the
benchmarked build. The docs under [docs/](https://github.com/PrimeLab-Foundation/strata/tree/main/docs) are the complete specification:
conventions, style, public API contract, architecture, benchmarking
methodology, the optimization playbook (including negative results), and
project history.

Install it with `pip install strata-plf`; the package you import is `strata`
(requirements under Installation below).

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
supported JSONPath subset — is [docs/context/api.md](https://github.com/PrimeLab-Foundation/strata/blob/main/docs/context/api.md).

## Installation

```bash
pip install strata-plf
```

The distribution is `strata-plf`; the package it installs is imported as
`strata`. The project named `strata` on PyPI is an unrelated package.

Binary wheels cover CPython 3.10–3.14 on:

- Linux x86_64 and aarch64 with glibc 2.28 or newer (`manylinux_2_28`)
- macOS x86_64 13.0 or newer, and macOS arm64 11.0 or newer
- Windows x86_64

All 25 wheels are built with profile-guided optimization (PGO) and link-time
optimization (LTO), except the Windows wheels, which use PGO without LTO. They
are built by
[`.github/workflows/release.yml`](https://github.com/PrimeLab-Foundation/strata/blob/main/.github/workflows/release.yml)
with cibuildwheel 4.3.0; each wheel is compiled through both test suites and
runs the Python suite again once installed.

**The x86_64 wheels need a CPU with AVX2.** The Linux and macOS wheels target
x86-64-v3 (Intel Haswell, 2013, and newer; AMD Excavator and newer), and the
Windows wheel is compiled with `/arch:AVX2`. On Linux or macOS with an older
x86_64 CPU, build from the source distribution instead:

```bash
pip install --no-binary strata-plf strata-plf
```

The source build compiles for the CPU that builds it (`-march=native`; under a
universal2 macOS interpreter, for each architecture's baseline instead),
without PGO or LTO. It
needs a C++20 compiler, and it runs the C++ and Python test suites before
installing; pip fetches CMake and pytest for it. On Windows the source build
also compiles with `/arch:AVX2`, so it does not help on a CPU without AVX2.
There are no wheels for musl-based Linux (Alpine), free-threaded CPython builds
or 32-bit platforms; pip then attempts the source build, which is not tested
there.

Versions are calendar dates of release, `YYYY.M.D`; a same-day re-release
appends `.N` and a release candidate `rcK`, so versions sort correctly under
PEP 440.

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

The public API contract is
[docs/context/api.md](https://github.com/PrimeLab-Foundation/strata/blob/main/docs/context/api.md).

## License

MIT — see [LICENSE](https://github.com/PrimeLab-Foundation/strata/blob/main/LICENSE).
