"""scripts/release_post.py: the post-release workflow's install, installed-wheel proof and rivals.

Nothing here installs a package or imports the extension: pip, the imports
and the interpreter's paths are replaced by fakes.
"""

from __future__ import annotations

import importlib.util
import types
from pathlib import Path

import pytest

PROJECT_ROOT = Path(__file__).resolve().parents[2]
SCRIPT = PROJECT_ROOT / "scripts" / "release_post.py"
VERSION = "2026.10.7"


def _load():
    spec = importlib.util.spec_from_file_location("release_post_under_test", SCRIPT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


post = _load()


@pytest.mark.parametrize("version", [VERSION, f"{VERSION}.1", "2026.1.31"])
def test_require_final_accepts_a_final_version(version):
    post.require_final(version)


@pytest.mark.parametrize(
    ("version", "message"),
    [
        (f"{VERSION}rc1", "is a release candidate"),
        (f"v{VERSION}", "is not YYYY.M.D"),
        ("2026.10.07", "is not YYYY.M.D"),
        ("", "is not YYYY.M.D"),
        (f"{VERSION}; echo pwned", "is not YYYY.M.D"),
        ("2026.2.30", "does not name a calendar date"),
    ],
)
def test_require_final_refuses(version, message):
    with pytest.raises(SystemExit, match=message):
        post.require_final(version)


@pytest.fixture
def pip(monkeypatch):
    """A pip that fails ``fail_first`` times, then installs; sleeps are recorded, not slept."""
    state = types.SimpleNamespace(runs=[], sleeps=[], fail_first=0)

    def fake_run(cmd, check):
        state.runs.append(cmd)
        return types.SimpleNamespace(returncode=1 if len(state.runs) <= state.fail_first else 0)

    monkeypatch.setattr(post, "subprocess", types.SimpleNamespace(run=fake_run))
    monkeypatch.setattr(post, "time", types.SimpleNamespace(sleep=state.sleeps.append))
    return state


def test_install_runs_pip_for_the_wheel_from_pypi(pip):
    assert post.install(VERSION) == 0
    assert len(pip.runs) == 1
    cmd = pip.runs[0]
    assert cmd[:4] == [post.sys.executable, "-m", "pip", "install"]
    assert cmd[-1] == f"strata-plf=={VERSION}"
    assert "--only-binary=strata-plf" in cmd
    assert "--no-cache-dir" in cmd
    assert cmd[cmd.index("--index-url") + 1] == "https://pypi.org/simple/"
    assert pip.sleeps == []


def test_install_retries_with_backoff_until_the_index_serves_it(pip):
    pip.fail_first = 3
    assert post.install(VERSION) == 0
    assert len(pip.runs) == 4
    assert pip.sleeps == [15, 30, 60]


def test_install_gives_up_after_the_last_attempt(pip):
    pip.fail_first = post.ATTEMPTS
    with pytest.raises(SystemExit, match=f"strata-plf=={VERSION} did not install"):
        post.install(VERSION)
    assert len(pip.runs) == post.ATTEMPTS
    assert pip.sleeps == [15, 30, 60, 120, 120, 120, 120]


def test_install_refuses_a_candidate_before_running_pip(pip):
    with pytest.raises(SystemExit, match="release candidate"):
        post.install(f"{VERSION}rc1")
    assert pip.runs == []


@pytest.fixture
def installed(monkeypatch, tmp_path):
    """check_installed against a fake wheel in a fake site-packages: no import."""
    site = tmp_path / "site-packages"
    (site / "strata").mkdir(parents=True)
    modules = {
        "strata": types.SimpleNamespace(
            __version__=VERSION,
            __file__=str(site / "strata" / "__init__.py"),
        ),
        "strata._strata": types.SimpleNamespace(__file__=str(site / "strata" / "_strata.so")),
        "strata._dumps_hook": types.SimpleNamespace(
            __file__=str(site / "strata" / "_dumps_hook.so"),
        ),
    }
    monkeypatch.setattr(post, "importlib", types.SimpleNamespace(import_module=modules.__getitem__))
    monkeypatch.setattr(
        post,
        "sysconfig",
        types.SimpleNamespace(get_paths=lambda: {"purelib": str(site), "platlib": str(site)}),
    )
    return types.SimpleNamespace(modules=modules, site=site)


def test_check_installed_accepts_the_wheel_and_prints_what_it_read(installed, capsys):
    assert post.check_installed(VERSION) == 0
    out = capsys.readouterr().out
    assert f"+ strata.__version__ = '{VERSION}'" in out
    for name, module in installed.modules.items():
        assert f"+ {name}.__file__ = {Path(module.__file__).resolve()}" in out


@pytest.mark.parametrize("version", ["2026.10.6", f"{VERSION}.1", f"{VERSION}rc1", ""])
def test_check_installed_refuses_another_version(installed, version):
    installed.modules["strata"].__version__ = version
    with pytest.raises(SystemExit, match=f"strata.__version__ is '{version}', not the released"):
        post.check_installed(VERSION)


@pytest.mark.parametrize("name", ["strata", "strata._strata", "strata._dumps_hook"])
def test_check_installed_refuses_the_checkout_shadowing_the_wheel(installed, name):
    installed.modules[name].__file__ = str(PROJECT_ROOT / "python" / "strata" / "__init__.py")
    with pytest.raises(SystemExit, match=f"{name} imports from .* must not shadow it"):
        post.check_installed(VERSION)


def test_check_installed_refuses_a_file_outside_site_packages(installed, tmp_path):
    installed.modules["strata._strata"].__file__ = str(tmp_path / "elsewhere" / "_strata.so")
    with pytest.raises(SystemExit, match="strata._strata imports from .*elsewhere"):
        post.check_installed(VERSION)


def test_bench_requirements_are_the_declared_competitor_set():
    requirements = post.bench_requirements()
    names = {requirement.split(";")[0].split(">")[0].strip() for requirement in requirements}
    rivals = {"orjson", "msgspec", "ujson", "pysimdjson", "jmespath", "jsonpath-ng"}
    assert rivals | {"psutil"} <= names
    assert not any(name.startswith("strata") for name in names)


def test_bench_requirements_refuses_a_pyproject_without_the_extra(tmp_path):
    pyproject = tmp_path / "pyproject.toml"
    pyproject.write_text('[project]\nname = "x"\n[project.optional-dependencies]\ndev = ["a"]\n')
    with pytest.raises(SystemExit, match="no 'bench' extra"):
        post.bench_requirements(pyproject)


def test_main_prints_one_requirement_per_line(capsys):
    assert post.main(["bench-requirements"]) == 0
    assert capsys.readouterr().out.splitlines() == post.bench_requirements()
