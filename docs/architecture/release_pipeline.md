# Decision record: the release pipeline — PGO wheels to TestPyPI, promoted byte for byte to PyPI

Status: **implemented, not yet run** (2026-10-06): the configuration, the
scripts and both workflows exist, but no Release workflow run has executed yet.
The release ISA is priced by R1 (below), and T7's CPU guard is in place and
verified (see "CPU guard" below).

Area: packaging and CI only — `pyproject.toml` (`[project]`,
`[tool.cibuildwheel]`), `setup.py`'s build knobs, `scripts/release.py`,
`.github/workflows/release.yml` and `publish-pypi.yml`, `tests/release/`.
Nothing in `include/`, `src/` or the Python facade changes. The runbook is in
`docs/build-and-test/SKILL.md`, "Release".

## Names

The **distribution** is `strata-plf`, and the **import** stays `strata`
(`pip install strata-plf`, then `import strata`). The name `strata` on PyPI
belongs to an unrelated project. `strata-plf` was confirmed free on
2026-10-06. Files are named `strata_plf-<version>…`. The two extension images
(`strata._strata` and `strata._dumps_hook`) keep their module names.

## What a release ships

25 wheels and 1 sdist, nothing else (`scripts/release.py verify-dist`):

| Leg            | Platform tag                        | Runner             | CPython        |
| -------------- | ----------------------------------- | ------------------ | -------------- |
| Linux x86_64   | `manylinux_*_x86_64`, glibc ≤ 2.28  | `ubuntu-latest`    | cp310 to cp314 |
| Linux aarch64  | `manylinux_*_aarch64`, glibc ≤ 2.28 | `ubuntu-24.04-arm` | cp310 to cp314 |
| macOS x86_64   | `macosx_13_0_x86_64`                | `macos-15-intel`   | cp310 to cp314 |
| macOS arm64    | `macosx_11_0_arm64`                 | `macos-latest`     | cp310 to cp314 |
| Windows x86_64 | `win_amd64`                         | `windows-latest`   | cp310 to cp314 |

**Excluded:** musllinux, the free-threaded `cp3Xt` builds, PyPy, win-arm64,
universal2, i686 and win32 (`skip = "cp3??t-* *-musllinux_* *_i686 *-win32"`).
None of them has a CI leg, so none of them meets the platform-support rule in
`docs/context/convention.md`. On those platforms, pip falls back to the sdist.

## Toolchain per leg

| Leg                 | Compiler and runtime                                                                                                                                                                                                              |
| ------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Linux (both arches) | `manylinux_2_28` image. `before-all` runs `dnf install -y clang llvm lld perl-Digest-SHA`. `CC`/`CXX` are `/usr/bin/clang(++)`, using `--gcc-toolchain=$DEVTOOLSET_ROOTPATH/usr` (the gcc-toolset's libstdc++) and `-fuse-ld=lld` |
| macOS x86_64        | Apple clang, `MACOSX_DEPLOYMENT_TARGET=13.0`                                                                                                                                                                                      |
| macOS arm64         | Apple clang, `MACOSX_DEPLOYMENT_TARGET=11.0`                                                                                                                                                                                      |
| Windows             | clang-cl (`STRATA_WIN_COMPILER=clang-cl`), PGO **without** LTO: clang-cl's LTO needs lld-link, and setuptools does not drive it. This is the same build the Windows benchmark leg measures                                        |

POSIX wheels build with ThinLTO (`STRATA_ENABLE_LTO=1`). `release.py profile`
asserts the toolchain and prints it before it trains anything. On Linux it
checks that clang is used for `CC` and `CXX` and has the same major version as
`llvm-profdata`. On POSIX it checks that `STRATA_MARCH` is set explicitly. On
macOS it checks the deployment target and that `ARCHFLAGS` names exactly the
host architecture. On Windows it checks for clang-cl.

## PGO: per version, per leg, in `before-build`

Each matrix job (one CPython tag on one leg) trains its own profiles. The
cibuildwheel `before-build` hook runs `python scripts/release.py profile`,
which does the following:

1. It creates a private virtualenv from that job's interpreter. It then runs
   the project's own PGO script (`scripts/pgo_build.sh`, or
   `scripts/pgo_build_clang_cl.py` on Windows) into `build/release-pgo` with
   `PGO_VERIFY_BENCH=0`, so the script's verification benchmarks and their
   data generation are skipped. The wheel build's own PGO variables are
   stripped from the script's environment, because the script sets them phase
   by phase itself.
2. The script makes **four test-gated builds**: phase 1 (instrumented), phase
   2 (`_strata` on its profile, plus LTO), phase 3a (the hook instrumented
   alone) and phase 3b (the hook on its own profile). Each is a
   `pip install -e .` that runs setup.py's C++ gate before compiling and its
   Python gate after. The wheel build that follows goes through the same two
   gates against the finished profiles. `SKIP_TESTS` is refused.
3. **The hook invariant:** the hook never carries `_strata`'s profile.
   `profile` fails if `hook.profdata` and `strata.profdata` have the same
   SHA-256, and records both in `build/release-pgo/profiles.json`. In each
   job's test step, `check-install --identity` proves that each installed
   image was built against its own profile alone
   (`build_identity.check_profiled`, with the other profile as the foreign
   one).
4. It removes everything the training builds left behind: all of `build/`
   except the profiles, plus the in-place extensions and the egg-info. The
   wheel then compiles every image from scratch.

`profile` deletes `build/`, **`build/evidence` included**, and the in-tree
extensions. It therefore refuses to run unless `CIBUILDWHEEL` is set, which
only cibuildwheel's build environment does. `make release-wheel-linux` builds
from a copy of the tree in `build/release-src` for the same reason.

On macOS, cibuildwheel exports `MACOSX_DEPLOYMENT_TARGET`, and CMake builds
the C++ gate's suites for that target. `CMakeLists.txt` therefore defines
`_LIBCPP_DISABLE_AVAILABILITY` on Apple, as setup.py does for the extensions.
Without it, the float `to_chars` oracle in `tests/cpp/test_float_precision.cpp`
does not compile below macOS 13.3. That failure stopped every macOS job of
rehearsal run 37445278333 in `before-build`. The suites run on the build host,
which is newer than libc++'s annotation floor.

## Repair: the image the identity hashed

`check-install --identity` fails when the installed image's SHA-256 differs
from the `extension_sha256` in its `.build.json`
(`build_identity.profiled_problems`, "describes another binary"). The identity
is written at link time, before cibuildwheel's repair step. That step must
therefore leave every image byte for byte as it was linked.

| Leg     | Repair (cibuildwheel 4.3.0)                   | Effect on the images                                                                                                                                                                                                                                                                                     |
| ------- | --------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Linux   | `auditwheel repair` (default)                 | None. The images need only libraries the manylinux policy allows (libstdc++, libm, libc, libgcc_s). auditwheel grafts nothing, so it runs no `patchelf` and only retags the wheel. Every Linux job of run 37445278333 passed the identity check.                                                         |
| macOS   | `delocate-wheel --require-archs …` (default)  | None. The images link only `/usr/lib/libc++.1.dylib` and `/usr/lib/libSystem.B.dylib`. delocate copies nothing from system paths, so it runs no `install_name_tool` and no re-signing. Checked locally with delocate 0.13.0 on both arm64 images: the SHA-256 after repair matches the one before.       |
| Windows | `delvewheel repair --no-mangle-all -w … -v …` | None. The images import one DLL that Python does not ship: `msvcp140.dll`. It is still vendored into `strata_plf.libs`, and `strata/__init__.py` gets delvewheel's `os.add_dll_directory` patch. The default mangling renames the DLL and rewrites each `.pyd`'s import table, which broke the identity. |

Mangling is off because it is what rewrites the image. delvewheel 1.13.1
renames dependencies only when it mangles (`_wheel_repair.py`). With
`--no-mangle-all`, the one remaining write to a `.pyd` clears a non-zero
`DependentLoadFlags`, and strata links no `/DEPENDENTLOADFLAG`.

The trade-off is the DLL's name. `msvcp140.dll` keeps its stable name, so a
process loads one copy: the first one loaded, wherever it came from. Microsoft
supports a runtime that is newer than the toolset. It does not support one that
is older, so strata relies on the vendored copy, or any copy loaded before it,
being at least as new as the build toolset. Mangling gave strata a private copy
instead. Rejected: hashing the pre-repair image in `check-install`. The
identity would then describe a binary the wheel does not ship.

**Open, for the next rehearsal to confirm on every Windows job:**

- `check-install --identity` passes.
- The `-v` log prints `skip mangling DLL names` and no `clearing DependentLoadFlags`.
- The vendored `msvcp140.dll` comes from a VC++ redistributable at least as new
  as the toolset. Run 37445278333 copied
  `C:\hostedtoolcache\windows\Java_Temurin-Hotspot_jdk\17.0.20-101\x64\bin\msvcp140.dll`,
  the first copy on `PATH`. delvewheel warned that the images were built with a
  newer platform toolset (14.51) than that DLL. This holds with or without
  mangling. The wheels job now puts the newest VC++ redistributable's x64 CRT
  directory (`VC\Redist\MSVC\<version>\x64\Microsoft.VC14*.CRT`, found by
  `vswhere`) first on `PATH` before cibuildwheel runs, so delvewheel vendors
  that `msvcp140.dll`, and the step fails the job, printing what `vswhere`
  found, when no such directory holds one; the step's log names the copy and
  its file version for the rehearsal to check against the toolset.

## Release ISA

| Leg           | Flag                                                           | Checked by `check-install --identity`    |
| ------------- | -------------------------------------------------------------- | ---------------------------------------- |
| Linux x86_64  | `-march=x86-64-v3`                                             | exactly that `-march`, plus `-flto=thin` |
| macOS x86_64  | `-march=x86-64-v3`                                             | exactly that `-march`, plus `-flto=thin` |
| Linux aarch64 | `-march=armv8-a`                                               | exactly that `-march`, plus `-flto=thin` |
| macOS arm64   | none (`STRATA_MARCH=none`): Apple clang's default arm64 target | no `-march` at all, plus `-flto=thin`    |
| Windows       | `/arch:AVX2`                                                   | `/arch:AVX2` present                     |

On every leg, `-march=native` is a failure. `STRATA_MARCH` is the setup.py
knob: a target name is passed as `-march=<target>`, `none` emits no `-march`,
and unset keeps `-march=native`.

**The ISA is priced (R1, run 37447108196 on `work/t8-isa-ab` at 09233f3;
ledger entry R1, packets under `docs/benchmarks/evidence/R1/`).** Three arms
per x86 leg — `-march=native`, `x86-64-v3`, `x86-64-v2` — each built at the
same path with its own PGO profile, 27 canonical small-tier rows, 6 ABBA
blocks of repeat 60 against an A/A floor. The decision rule was: v2 only if
it lands inside the floor on every row on both legs. It does not, so both
x86 legs ship **x86-64-v3**:

| Leg                      | v3 vs native, rows resolved of 27                                   | v2 vs v3, rows resolved of 27                                                             | Choice    |
| ------------------------ | ------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | --------- |
| linux-x86_64 (EPYC 9V45) | 2, both slower (+6.79% `search users $[*].id`, +4.67% `load users`) | 3: one loss (`dumps wide_arrays` +1.13%), two gains (streaming `search` −15.09%, −11.65%) | x86-64-v3 |
| macos-x86_64 (i7-8700B)  | 1, slower (`load flat` +4.38%)                                      | 2: one loss (`dump nested` +4.31%), one gain (`search` −5.25%)                            | x86-64-v3 |

Caveats recorded with R1: one draw per leg on one host each; the release
toolchain itself (manylinux clang, deployment target 13.0) is not priced;
the six JSONPath rows use orjson parsing the same document as their drift
control. An open observation, not acted on: v2 beats v3 on the streaming
`search` rows on both legs.

**T7, the CPU guard, is closed** (see "CPU guard" below): its Linux
full-image disassembly and its SDE run are recorded against the release
wheels of rehearsal run 37451214168.

**The sdist keeps `-march=native` as the fallback.** It sets no
`STRATA_MARCH`, so `pip install --no-binary strata-plf strata-plf` builds for
the host CPU, through both gates (`cmake` and `pytest` are build
requirements). That is also the route for every excluded platform.

**`-D_LIBCPP_DISABLE_AVAILABILITY` stays** in setup.py's POSIX flags,
unconditionally. `src/strata/util/folder.cpp` uses `std::filesystem`, which
libc++ marks unavailable below macOS 10.15, and ci.yml's macOS jobs build
universal2 slices at deployment targets 10.9 and 10.13, which need the define
to compile. It changes no release wheel: both images compile at deployment
targets 11.0 and 13.0 without it, and at a fixed path with `ZERO_AR_DATE=1` the
build is byte-identical with and without it.

## CPU guard

Where an image is compiled with AVX2 on x86-64 (`-march=x86-64-v3`,
`/arch:AVX2`, or `-march=native` on an AVX2 host), both init entries,
`PyInit__strata` and `PyInit__dumps_hook`, are compiled for the x86-64
baseline (`STRATA_CPU_GUARD_ENTRY`, `src/strata/bindings/python_cpu_guard.h`)
and first check the full x86-64-v3 set — CPUID leaves 1, 7 and 80000001h
plus XCR0's XMM and YMM bits through XGETBV
(`include/strata/util/cpu_features.hpp`). On failure they raise ImportError
naming the requirement and the way out: `pip install --no-binary strata-plf strata-plf` on Linux, the same or an arm64 Python or `ROSETTA_ADVERTISE_AVX=1`
on macOS, and on Windows the statement that every build there targets AVX2.
On success the module's unchanged initialisation runs. The hook image has its
own guard, from the same header: it can be loaded before `_strata`. On arm64
and every non-AVX2 build the guard compiles away. GCC, clang and clang-cl
take the baseline attribute; under MSVC proper (not a release compiler) the
check runs but its own code is not held to the baseline.

Evidence (`docs/benchmarks/evidence/T7/`): an x86-64-v3 macOS build's
entries hold no instruction above the baseline and the images have no static
initializers (`macos_x86_64_v3_scan.txt`); under Rosetta 2, whose CPUID omits
AVX unless `ROSETTA_ADVERTISE_AVX=1`, each image loaded alone raises the
ImportError, and imports with the variable set (`rosetta_import.txt`). On the
release cp312 Linux x86_64 wheel of run 37451214168 (source f17efb5, PGO,
ThinLTO, `-march=x86-64-v3`), `scripts/check_guard_isa.py` passes both
images: `PyInit__strata`, its `.cold` part (where PGO placed the check),
`PyInit__dumps_hook` and the toolchain's `_init`/`frame_dummy` initializers
hold no instruction above the baseline, and each entry reaches only
`PyErr_Format@plt` and its module body (`linux_release_wheel_scan.txt`). Under
Intel SDE 10.13.1 on a hosted Linux runner (run 37466325074), the same wheel
raises the ImportError under `-nhm` and imports and serializes under `-hsw`
and natively (`sde_nhm_ci.txt`); SDE cannot run under Rosetta
(`sde_local_rosetta_attempt.txt`).

`check-install --identity` repeats that scan on every wheel it tests on x86-64
POSIX (Linux x86_64, macOS x86_64): `scripts/check_guard_isa.py`, promoted
from T7's evidence copy, disassembles both installed images' static
initializers, their `PyInit_*` entries, outlined `.cold` parts included, and
every local function an entry calls or tail-jumps to at any depth (stopping
at `create_strata_module`/`create_hook_module`, which run after the guard,
and at PLT stubs), and fails the wheel on any instruction above the baseline,
on a symbol that disassembles to nothing, or on an image with no entry. Where
`objdump`, `nm` and `readelf` (Linux) or `otool` (macOS), under those names
or their `llvm-` ones, are not on `PATH`, it fails under `CI` and otherwise
prints that it skipped the scan and why. Windows
prints a skip too, since its test environment has no disassembler: the
clang-cl images rest on T7's static evidence that clang-cl takes the same
baseline attribute (`STRATA_CPU_GUARD_ENTRY`) as clang.

## Versions and tags

The grammar is `YYYY.M.D[.N][rcK]` (`scripts/release.py`: `VERSION_RE`,
`bump`). It allows no leading zeros and requires a real calendar date. `.N`
is a same-day re-release and `rcK` a release candidate, and both count from 1.
This is PEP 440 normal form, ordered `D rcK < D < D.N rcK < D.N`. `bump`
rewrites only the `__version__` line of `python/strata/__init__.py`, and only
to a strictly higher version.

The tag must be `v` plus the literal (`check-tag`). On a tag push,
`release.yml` also requires that `origin/main` contains the tagged commit.
`check-tag --require-final` rejects any `rcK`, and so does `check-promotion`,
which reads the version from the tag rather than from a `__version__` literal
(it runs from the dispatching ref, not from the tagged commit).

**A version is burned once it is uploaded to TestPyPI.** No index accepts the
same filename twice, so a failed or superseded candidate moves to `rc(K+1)`.
For a final version that has already been uploaded, the fix moves to `.N`.

## Promotion: the files TestPyPI verified, byte for byte

- **`release.yml` (tag push `v*`)** runs `check`, then `sdist` and the 25
  `wheels` jobs, then `verify`. `verify` runs `verify-dist`, which writes
  `SHA256SUMS` to `manifest/`, outside the upload directory `upload/`. Next
  come `publish-testpypi` and 26 verify jobs:

  - 25 `verify-testpypi` jobs, one per wheel. Each downloads its wheel from
    TestPyPI, checks the filename and sha256 against `SHA256SUMS`, installs
    it and runs `check-install`.
  - 1 `verify-testpypi-sdist` job. It fetches the sdist by URL, checks its
    sha256, installs it through both build gates and runs `check-install`.

  `publish-testpypi` itself checks the downloaded `dist` against `SHA256SUMS`
  (every listed file matches, no other file is present) before it uploads,
  because the upload burns the version.

- **`release.yml` (`workflow_dispatch`)** is the rehearsal. It runs the same
  build and `verify-dist` without the tag checks and without either TestPyPI
  stage, so it publishes nothing.

- **`publish-pypi.yml` (`workflow_dispatch`, input `release_run_id`)** runs
  two jobs, so that no candidate code runs where the PyPI token can be
  minted:

  - `verify` holds no `id-token` and no environment. It first validates the
    run with `jq` on `gh run view`'s JSON: workflow `Release`, event `push`,
    concluded `success`, a run URL in this repository, a `headBranch` of the
    form `v` + final version, and a 40-hex `headSha`. It then reads
    `refs/tags/<headBranch>` through the API, dereferences an annotated tag
    through its tag object, and requires that the tag names `headSha`. Only
    then does it check out the **dispatching ref's own code** (`github.sha`,
    never `headSha`), download the run's `dist` and `dist-manifest` artifacts
    (`SHA256SUMS` kept outside `dist/`), check them with `sha256sum -c` and
    an exact file-set comparison, fetch TestPyPI's JSON for the version, and
    run that code's `check-promotion`. `check-promotion` repeats the run and
    tag checks from the same JSON (the tag ref must be `refs/tags/<headBranch>`
    and name `headSha`, an annotated tag dereferenced once), then requires
    that the `dist` files match `SHA256SUMS` and that TestPyPI lists exactly
    those filenames and digests. The verified `dist` is uploaded as the job
    artifact `verified-dist`.
  - `publish` (`needs: verify`) holds `id-token: write` and environment
    `pypi`, checks nothing out, downloads `verified-dist` and runs only
    `pypa/gh-action-pypi-publish`.

  Nothing is rebuilt between TestPyPI and PyPI.

The `.build.json` provenance packet ships inside every wheel, beside both
extension images, and `verify-dist` fails a wheel without it. The sdist must
contain both source lists, every `*.inc`, the fuzz corpus and
`scripts/build_identity.py`.

## Trusted publishing, no secrets

Both indexes authenticate the workflows by OIDC (`id-token: write` on the
publishing job only). **No API token or repository secret exists anywhere**,
in the repository or in either workflow.

| Index    | Project      | Owner                 | Repository | Workflow           | Environment |
| -------- | ------------ | --------------------- | ---------- | ------------------ | ----------- |
| TestPyPI | `strata-plf` | `PrimeLab-Foundation` | `strata`   | `release.yml`      | `testpypi`  |
| PyPI     | `strata-plf` | `PrimeLab-Foundation` | `strata`   | `publish-pypi.yml` | `pypi`      |

**Trusted publishing alone does not restrict the ref.** The index checks the
repository, workflow file and environment name, not which branch or tag the
run came from, so a workflow dispatched from any branch could mint a token.
The environments carry that restriction, and both protections are required:

- **`pypi`**: deployment branches restricted to `main`, with a required
  reviewer.
- **`testpypi`**: deployment refs restricted to tags matching `v*`.

Third-party actions (`pypa/cibuildwheel`, `pypa/gh-action-pypi-publish`) are
pinned by full commit SHA with the tag in a comment; `actions/*` follow the
repository's tag convention.

## Post-release verification

After `publish`, a third job of `publish-pypi.yml`, `post-release` (`needs: [verify, publish]`, `contents: read`), calls `post-release.yml` through
`workflow_call`. It passes the version `verify` derived from the Release run's
tag, which `check-promotion` matched against the uploaded files. The same
workflow takes `workflow_dispatch` with a `version` input. Each of the five
release legs runs Python 3.12, as `benchmark.yml` does. The run proceeds in
six steps:

1. `scripts/release_post.py install` refuses anything but a final version in
   `release.py`'s grammar. It then runs
   `pip install --no-cache-dir --index-url https://pypi.org/simple/ --only-binary=strata-plf strata-plf==<version>`, up to eight times with
   backoff from 15 s to 120 s (about ten minutes) while the index propagates.
   Without the cache, a retry cannot read an index page from before the
   upload.
2. `bench-requirements` lists the `bench` extra from the checkout's
   `pyproject.toml`. Those are the rivals `make install-bench` installs; strata
   itself is not installed from the checkout.
3. `check-installed` runs under the benchmark's own `PYTHONPATH=.` and working
   directory. It prints `strata.__version__` and each module's file. It fails
   unless the version matches and `strata`, `strata._strata` and
   `strata._dumps_hook` all resolve into site-packages and outside the checkout.
   The package is checked first, so a shadowing tree is named before a missing
   submodule can fail to import.
4. The canonical small-tier suite runs as in `benchmark.yml`.
   `benchmarks.provenance.capture` reads the wheel's own `.build.json`, whose
   hash survives repair (above). The report's commit and compiler flags are
   therefore the release build's.
5. `benchmarks.supportability_check` gates the report.
6. `scripts/release_post.py standings-check` gates the same report on ranks
   (below), running even after a failed tripwire so that its leg reads INVALID
   or FAIL rather than missing. It writes
   `post_release_standings_<os>-<arch>.json`, and the report, its companion and
   that summary are uploaded as `post-release-<os>-<arch>`.

The workflow's second job, `standings` (`needs: verify`, runs unless cancelled), downloads
the five summaries and runs `standings-combine`. That prints one table (leg,
rows won, each behind row with its ratio, verdict) and uploads it as
`post-release-standings`. The job fails unless all five legs are present and
passed, so a leg with no summary reads MISSING and fails it too.

The checkout is the caller's commit: a local `uses:` resolves there, so the
harness belongs to the dispatching ref, never to the released tag. There are
two gates, and both are within-run. Neither is the 2% regression gate, and
neither compares a time with another run's, a baseline or another platform.

- **The tripwire**: no ERROR rows, strata in every declared row and category,
  and no row past 3.0x. It catches a wheel whose fast path is broken or
  accidentally scalar on hardware it was not built on. It stays the hard
  backstop.

- **The standings gate** ranks strata in each of the 27 declared rows of the
  leg's own report against the rivals measured in the same job. It reads the
  full-precision companion when present, through `harness.read_report` and
  `ci_summary.standings`. A report that is not gateable evidence is INVALID
  (exit 2): an ERROR row, a row short of the declared workload, a row with no
  rival, or a leg that cannot be told or is reported twice. A leg FAILs (exit 1)
  on either of two bounds, both constants in `scripts/release_post.py`:

  - **More than 3 rows behind** (`MAX_BEHIND_ROWS`, the per-leg coin band).
    The 28 distinct five-leg CI samples archived in September 2026, read with
    the same code, have 0–3 of 27 rows behind on 137 of 140 leg-draws. They
    are kept in the tree at `docs/benchmarks/evidence/standings-gate/` (one
    directory per sample, `MANIFEST.tsv` of sources and hashes, the gate's
    `replay.tsv`) plus `docs/benchmarks/ci/`, not only in `build/evidence/`.
    windows-x86_64 reads 3 on four draws of shipped source. The three
    leg-draws above the band are all linux-x86_64: 4 rows behind on 79fa3df
    (run 34064174240, E26-P6's x86 serializer regression, a real code effect),
    and 5 and 6 behind on c16eaa6 (runs 35305165291 and 35301268133, the
    source of the 135/135 sweep, on an AMD EPYC 9V45 host). The c16eaa6 pair is
    the gate's known false-alarm envelope: 2 of 28 draws on that leg for a
    known-good build.
  - **Any row past 1.25x its best rival** (`MAX_BEHIND_RATIO`). The worst
    behind ratio on any in-band leg-draw of those samples is 1.169x
    (windows-x86_64 `dump mixed`, b32d398). R1 (experiment-ledger.md) priced
    the release ISA's largest resolved cost against `-march=native` at +6.79%
    (linux-x86_64 `search users $[*].id`). 1.169 × 1.0679 = 1.248, rounded to
    1.25x.

  Replayed over those archived samples, the gate fails exactly the three
  leg-draws named above. Treat a lone FAIL as a coin draw until a rerun of the
  workflow (`workflow_dispatch`) on the same version reproduces it.

A failure leaves the release on PyPI. The remedy is the one under Rollback.

## Rollback

Yank the release on PyPI, then fix forward with `.N`. A filename that has
been uploaded can never be uploaded again, so there is no way to replace a
release in place.

## Rejected and removed

- **`pgo.yml`'s wheel step is removed** (`HEAD:.github/workflows/pgo.yml`
  lines 48–57). It built with `PGO_MODE=use` and no `STRATA_HOOK_PGO_*`, so its
  hook image was unprofiled, and that wheel was never a release artifact.
  `pgo.yml` keeps `make pgo` and its benchmark artifact.
