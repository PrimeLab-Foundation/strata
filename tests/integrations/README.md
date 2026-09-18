# Third-party integration tests

The third test tree. `tests/py` is integration against the project's own
surface and `tests/unit` is the clause-by-clause contract mirror; **this tree
holds tests that import third-party libraries**, to show that the `default=`
hook and (from M13) the framework adapters compose with the packages callers
actually have — `pydantic`, `attrs`, `numpy`, dataclass-heavy ORM rows.

## It is deliberately outside every gate

It is excluded from `pyproject.toml`'s `testpaths` (`tests/py`, `tests/unit`),
from `make test`, from `make gate`, and from the PGO training run. Two reasons,
both measured:

- Under the gate-inclusive PGO recipe the test suite is 47.5–55% of the
  profile's counts, so **every test addition is a several-percent move on some
  row** (E26-P7b priced a tests-only commit at linux-x86_64 `dumps flat` +3.4%
  and the Neoverse-N2's flat/users rows −2%, run 34594271053; E26-P8 attributed
  the counts per process). Tests whose cost is dominated by heavy third-party
  imports must not enter that profile.
- A green gate must not depend on packages the project otherwise has none of
  (`docs/context/convention.md`, "No dependency fallbacks"). strata's engine
  depends on nothing; its *gate* must not either.

## Running it

```bash
make test-integrations            # installs nothing; skips what is missing
pip install -e ".[integrations]"  # the optional extra that supplies the libraries
```

Every module here imports its library through `pytest.importorskip`, so the
target is green on a machine with none of them installed and meaningful on one
with all of them. CI runs it in its own job (`.github/workflows/integrations.yml`),
never as part of the build gate.

## Adding a file

One module per third-party library, named `test_<library>.py`. It must:

1. start with `pytest.importorskip("<library>")`;
2. assert against an oracle, not against strata's own output — stdlib `json`
   with the same `default=` for the hook tests, the framework's own serializer
   for an adapter;
3. state the library's supported version floor in a comment, matching the
   extra in `pyproject.toml`.
