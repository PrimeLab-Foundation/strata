"""Release tooling: scripts/release.py's verify-dist and check-promotion.

Split from test_release.py by responsibility (the ~800-line rule): these pin the
distribution checks on fixture files -- filename and tag-set grammar, wheel
METADATA and contents, sdist contents, SHA256SUMS, and the promotion evidence.
Run by `make test-release`; nothing here builds, downloads or uploads.
"""

from __future__ import annotations

import hashlib
import importlib.util
import io
import json
import re
import tarfile
import zipfile
from pathlib import Path

import pytest

PROJECT_ROOT = Path(__file__).resolve().parents[2]
RELEASE_SCRIPT = PROJECT_ROOT / "scripts" / "release.py"


def _load_release():
    spec = importlib.util.spec_from_file_location("release_dist_under_test", RELEASE_SCRIPT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


release = _load_release()


def _init_file(tmp_path: Path, version: str) -> Path:
    init = tmp_path / "__init__.py"
    init.write_text(f'__version__ = "{version}"\n', encoding="utf-8")
    return init


# --- verify-dist: filenames ------------------------------------------------

RELEASE = "2026.10.6"
# Per leg: the platform tag set its repair step writes, and its extension suffix.
LEG_FILES = {
    "manylinux_x86_64": (
        "manylinux_2_27_x86_64.manylinux_2_28_x86_64",
        "cpython-{d}-x86_64-linux-gnu.so",
    ),
    "manylinux_aarch64": ("manylinux_2_28_aarch64", "cpython-{d}-aarch64-linux-gnu.so"),
    "macosx_13_0_x86_64": ("macosx_13_0_x86_64", "cpython-{d}-darwin.so"),
    "macosx_11_0_arm64": ("macosx_11_0_arm64", "cpython-{d}-darwin.so"),
    "win_amd64": ("win_amd64", "cp{d}-win_amd64.pyd"),
}
SDIST_REQUIRED = [
    "src/strata/core_sources.txt",
    "src/strata/native_sources.txt",
    "scripts/build_identity.py",
    "include/strata/util/table.inc",
    "tests/fuzz/corpus/loads/seed",
]


def _names(version: str = RELEASE) -> list[str]:
    names = [f"strata_plf-{version}.tar.gz"]
    for python in release.PYTHON_TAGS:
        for leg in release.LEGS:
            names.append(f"strata_plf-{version}-{python}-{python}-{LEG_FILES[leg][0]}.whl")
    return names


@pytest.mark.parametrize(
    ("name", "slot"),
    [
        (
            "strata_plf-2026.10.6-cp310-cp310-manylinux_2_28_x86_64.whl",
            ("cp310", "manylinux_x86_64"),
        ),
        (
            "strata_plf-2026.10.6-cp314-cp314-manylinux_2_27_aarch64.manylinux_2_28_aarch64.whl",
            ("cp314", "manylinux_aarch64"),
        ),
        (
            "strata_plf-2026.10.6-cp312-cp312-manylinux2014_x86_64.manylinux_2_17_x86_64.whl",
            ("cp312", "manylinux_x86_64"),
        ),
        (
            "strata_plf-2026.10.6-cp313-cp313-macosx_13_0_x86_64.whl",
            ("cp313", "macosx_13_0_x86_64"),
        ),
        ("strata_plf-2026.10.6-cp311-cp311-macosx_11_0_arm64.whl", ("cp311", "macosx_11_0_arm64")),
        ("strata_plf-2026.10.6-cp310-cp310-win_amd64.whl", ("cp310", "win_amd64")),
    ],
)
def test_wheel_slot_reads_each_leg(name, slot):
    assert release.wheel_slot(name, RELEASE) == slot


@pytest.mark.parametrize(
    ("name", "message"),
    [
        ("strata_plf-2026.10.6-cp312-cp312-manylinux_2_34_x86_64.whl", "newer than manylinux_2_28"),
        (
            "strata_plf-2026.10.6-cp312-cp312-manylinux_2_28_x86_64.manylinux_2_28_aarch64.whl",
            "name 2 architectures",
        ),
        ("strata_plf-2026.10.6-cp312-cp312-musllinux_1_2_x86_64.whl", "none of the release legs"),
        ("strata_plf-2026.10.6-cp312-cp312-macosx_14_0_arm64.whl", "none of the release legs"),
        ("strata_plf-2026.10.6-cp312-cp312-macosx_10_9_x86_64.whl", "none of the release legs"),
        ("strata_plf-2026.10.6-cp312-cp312-win32.whl", "none of the release legs"),
        ("strata_plf-2026.10.6-cp312-cp313-win_amd64.whl", "with the same ABI tag"),
        ("strata_plf-2026.10.6-cp39-cp39-win_amd64.whl", "expected one of"),
        ("strata_plf-2026.10.6-cp315-cp315-win_amd64.whl", "expected one of"),
        ("strata_plf-2026.10.6-cp312-abi3-win_amd64.whl", "not a strata_plf CPython wheel"),
        ("strata_plf-2026.10.6-cp313t-cp313t-win_amd64.whl", "not a strata_plf CPython wheel"),
        ("strata-2026.10.6-cp312-cp312-win_amd64.whl", "not a strata_plf CPython wheel"),
        ("strata_plf-2026.10.7-cp312-cp312-win_amd64.whl", "carries version 2026.10.7"),
    ],
)
def test_wheel_slot_rejects(name, message):
    with pytest.raises(SystemExit, match=message):
        release.wheel_slot(name, RELEASE)


def test_dist_layout_accepts_one_sdist_and_25_wheels():
    sdist, wheels = release.dist_layout(_names(), RELEASE)
    assert sdist == "strata_plf-2026.10.6.tar.gz"
    assert sorted(wheels) == sorted((py, leg) for py in release.PYTHON_TAGS for leg in release.LEGS)
    assert len(wheels) == 25


def test_dist_layout_names_a_missing_wheel():
    names = [name for name in _names() if "-cp313-cp313-win_amd64" not in name]
    with pytest.raises(SystemExit, match="24 of 25 wheels; missing cp313-win_amd64$"):
        release.dist_layout(names, RELEASE)


def test_dist_layout_refuses_a_second_wheel_for_one_slot():
    names = [*_names(), "strata_plf-2026.10.6-cp310-cp310-manylinux_2_17_x86_64.whl"]
    with pytest.raises(SystemExit, match="two wheels for cp310 on manylinux_x86_64"):
        release.dist_layout(names, RELEASE)


@pytest.mark.parametrize("extra", ["SHA256SUMS", "strata_plf-2026.10.6.zip", "notes.txt"])
def test_dist_layout_refuses_any_other_file(extra):
    with pytest.raises(SystemExit, match="neither the sdist nor a wheel"):
        release.dist_layout([*_names(), extra], RELEASE)


@pytest.mark.parametrize(
    "sdists",
    [[], ["strata_plf-2026.10.5.tar.gz"], ["strata_plf-2026.10.6.tar.gz"] * 2],
)
def test_dist_layout_needs_exactly_the_one_sdist(sdists):
    with pytest.raises(SystemExit, match="expected exactly one sdist"):
        release.dist_layout([*_names()[1:], *sdists], RELEASE)


# --- verify-dist: fixture distribution -------------------------------------


def _metadata(**overrides) -> str:
    fields = {
        "Metadata-Version": "2.4",
        "Name": "strata-plf",
        "Version": RELEASE,
        "Requires-Python": release._requires_python(),
        "License-Expression": "MIT",
    }
    fields.update(overrides)
    head = "".join(f"{key}: {value}\n" for key, value in fields.items() if value is not None)
    return head + "\nA fixture description.\n"


def _write_wheel(dist: Path, python: str, leg: str, metadata: str | None = None, skip=()) -> str:
    platform_tags, suffix = LEG_FILES[leg]
    name = f"strata_plf-{RELEASE}-{python}-{python}-{platform_tags}.whl"
    members = {f"strata_plf-{RELEASE}.dist-info/METADATA": metadata or _metadata()}
    for extension in ("_strata", "_dumps_hook"):
        image = f"strata/{extension}.{suffix.format(d=python[2:])}"
        members[image] = "not an image"
        members[image + ".build.json"] = "{}"
    with zipfile.ZipFile(dist / name, "w") as wheel:
        for member, text in members.items():
            if member not in skip:
                wheel.writestr(member, text)
    return name


def _write_sdist(dist: Path, members=SDIST_REQUIRED) -> None:
    with tarfile.open(dist / f"strata_plf-{RELEASE}.tar.gz", "w:gz") as sdist:
        for rel in members:
            info = tarfile.TarInfo(f"strata_plf-{RELEASE}/{rel}")
            info.size = 1
            sdist.addfile(info, io.BytesIO(b"x"))


@pytest.fixture
def dist(tmp_path):
    upload = tmp_path / "upload"
    upload.mkdir()
    _write_sdist(upload)
    for python in release.PYTHON_TAGS:
        for leg in release.LEGS:
            _write_wheel(upload, python, leg)
    return upload


def _verify(dist: Path, sums: Path) -> int:
    return release.verify_dist(dist, RELEASE, sums, run_twine=False, required=SDIST_REQUIRED)


def test_verify_dist_writes_sums_outside_the_upload_dir(dist, tmp_path):
    sums = tmp_path / "manifest" / "SHA256SUMS"
    assert _verify(dist, sums) == 0
    listed = release.read_sums(sums)
    files = {path.name: hashlib.sha256(path.read_bytes()).hexdigest() for path in dist.iterdir()}
    assert listed == files
    assert len(listed) == 26
    data = sums.read_bytes()
    assert data.endswith(b".tar.gz\n")
    assert b"\r" not in data


@pytest.mark.parametrize("inside", ["SHA256SUMS", "sub/../SHA256SUMS"])
def test_verify_dist_refuses_sums_inside_the_upload_dir(dist, inside):
    with pytest.raises(SystemExit, match="inside the upload directory"):
        _verify(dist, dist / inside)
    assert not (dist / "SHA256SUMS").exists()


def test_verify_dist_refuses_a_subdirectory(dist, tmp_path):
    (dist / "nested").mkdir()
    with pytest.raises(SystemExit, match=r"not regular files: \['nested'\]"):
        _verify(dist, tmp_path / "SHA256SUMS")


def test_verify_dist_refuses_a_version_outside_the_grammar(dist, tmp_path):
    with pytest.raises(SystemExit, match="is not YYYY.M.D"):
        release.verify_dist(dist, "2026.10.06", tmp_path / "SHA256SUMS", run_twine=False)


@pytest.mark.parametrize(
    ("field", "value"),
    [
        ("Name", "strata"),
        ("Version", "2026.10.5"),
        ("Requires-Python", ">=3.9"),
        ("License-Expression", None),
        ("License-Expression", "Apache-2.0"),
    ],
)
def test_verify_dist_checks_each_wheels_metadata(dist, tmp_path, field, value):
    _write_wheel(dist, "cp312", "win_amd64", metadata=_metadata(**{field: value}))
    sums = tmp_path / "SHA256SUMS"
    with pytest.raises(SystemExit, match=f"win_amd64.whl: METADATA {field} is"):
        _verify(dist, sums)
    assert not sums.exists()


@pytest.mark.parametrize(
    ("leg", "member", "message"),
    [
        (
            "win_amd64",
            "strata/_strata.cp312-win_amd64.pyd.build.json",
            "no strata/_strata.cp312-win_amd64.pyd.build.json",
        ),
        ("win_amd64", "strata/_dumps_hook.cp312-win_amd64.pyd", "one strata._dumps_hook image"),
        (
            "manylinux_aarch64",
            "strata/_dumps_hook.cpython-312-aarch64-linux-gnu.so.build.json",
            "no strata/_dumps_hook.cpython-312-aarch64-linux-gnu.so.build.json",
        ),
        ("macosx_11_0_arm64", "strata/_strata.cpython-312-darwin.so", "one strata._strata image"),
        (
            "win_amd64",
            f"strata_plf-{RELEASE}.dist-info/METADATA",
            "no strata_plf-2026.10.6.dist-info",
        ),
    ],
)
def test_verify_dist_needs_both_images_and_their_identities(dist, tmp_path, leg, member, message):
    _write_wheel(dist, "cp312", leg, skip={member})
    with pytest.raises(SystemExit, match=re.escape(message)):
        _verify(dist, tmp_path / "SHA256SUMS")


def test_verify_dist_refuses_an_image_of_another_python(dist, tmp_path):
    name = _write_wheel(dist, "cp312", "win_amd64", skip={"strata/_strata.cp312-win_amd64.pyd"})
    with zipfile.ZipFile(dist / name, "a") as wheel:
        wheel.writestr("strata/_strata.cp311-win_amd64.pyd", "not an image")
    with pytest.raises(SystemExit, match="one strata._strata image for cp312"):
        _verify(dist, tmp_path / "SHA256SUMS")


@pytest.mark.parametrize("dropped", SDIST_REQUIRED)
def test_verify_dist_needs_every_sdist_requirement(dist, tmp_path, dropped):
    _write_sdist(dist, members=[rel for rel in SDIST_REQUIRED if rel != dropped])
    with pytest.raises(SystemExit, match=re.escape(f"lacks 1 required files: ['{dropped}']")):
        _verify(dist, tmp_path / "SHA256SUMS")


def test_sdist_requirements_come_from_the_checkout():
    required = release._sdist_required()
    assert required[:3] == list(release.SDIST_FILES)
    assert "include/strata/util/eisel_lemire_table.inc" in required
    assert any(rel.startswith("tests/fuzz/corpus/loads/") for rel in required)
    assert any(rel.startswith("tests/fuzz/corpus/ndjson/") for rel in required)


def test_verify_dist_cli_runs_twine_strict_on_every_file(dist, tmp_path, monkeypatch):
    calls = []
    monkeypatch.setattr(release, "_run", lambda cmd, env=None: calls.append(cmd))
    monkeypatch.setattr(release, "_sdist_required", lambda: SDIST_REQUIRED)
    sums = tmp_path / "SHA256SUMS"
    assert release.main(["verify-dist", str(dist), "--version", RELEASE, "--sums", str(sums)]) == 0
    assert len(calls) == 1
    assert calls[0][1:5] == ["-m", "twine", "check", "--strict"]
    assert sorted(Path(arg).name for arg in calls[0][5:]) == sorted(release.read_sums(sums))


# --- SHA256SUMS ------------------------------------------------------------

DIGEST_A, DIGEST_B = "a" * 64, "b" * 64


@pytest.mark.parametrize(
    "text",
    [
        f"{DIGEST_A} one.whl\n",
        f"{DIGEST_A}  dir/one.whl\n",
        f"{DIGEST_A.upper()}  one.whl\n",
        f"{DIGEST_A[:-1]}  one.whl\n",
        f"{DIGEST_A}  one.whl\n\n",
        f"{DIGEST_A} *one.whl\n",
    ],
)
def test_read_sums_refuses_a_malformed_line(tmp_path, text):
    path = tmp_path / "SHA256SUMS"
    path.write_text(text, encoding="utf-8")
    with pytest.raises(SystemExit, match="is not '<sha256>  <filename>'"):
        release.read_sums(path)


def test_read_sums_refuses_a_repeated_file_and_an_empty_list(tmp_path):
    path = tmp_path / "SHA256SUMS"
    path.write_text(f"{DIGEST_A}  one.whl\n{DIGEST_B}  one.whl\n", encoding="utf-8")
    with pytest.raises(SystemExit, match="lists one.whl twice"):
        release.read_sums(path)
    path.write_text("", encoding="utf-8")
    with pytest.raises(SystemExit, match="lists no files"):
        release.read_sums(path)


@pytest.mark.parametrize(
    ("actual", "message"),
    [
        ({"one.whl": DIGEST_A, "two.whl": DIGEST_A}, r"digest differs \['two.whl'\]"),
        ({"one.whl": DIGEST_A}, r"missing \['two.whl'\]"),
        (
            {"one.whl": DIGEST_A, "two.whl": DIGEST_B, "three.whl": DIGEST_A},
            r"extra \['three.whl'\]",
        ),
    ],
)
def test_compare_sums_reports_each_mismatch(actual, message):
    listed = {"one.whl": DIGEST_A, "two.whl": DIGEST_B}
    release.compare_sums(listed, dict(listed), "the same files")
    with pytest.raises(SystemExit, match=message):
        release.compare_sums(listed, actual, "the files")


# --- check-promotion -------------------------------------------------------

HEAD_SHA = "0123456789abcdef0123456789abcdef01234567"
TAG_OBJECT_SHA = "fedcba9876543210fedcba9876543210fedcba98"


class _Promotion:
    """A Release run's gathered evidence, as publish-pypi.yml lays it out."""

    def __init__(self, dist: Path, tmp_path: Path) -> None:
        self.dist = dist
        self.sums = tmp_path / "manifest" / "SHA256SUMS"
        _verify(dist, self.sums)
        self.run = {
            "databaseId": 123,
            "workflowName": "Release",
            "event": "push",
            "status": "completed",
            "conclusion": "success",
            "headBranch": f"v{RELEASE}",
            "headSha": HEAD_SHA,
        }
        self.tag_ref = {
            "ref": f"refs/tags/v{RELEASE}",
            "object": {"sha": HEAD_SHA, "type": "commit"},
        }
        self.tag_object: dict | None = None
        self.testpypi = {
            "info": {"version": RELEASE},
            "urls": [
                {"filename": name, "digests": {"sha256": digest, "md5": "unused"}}
                for name, digest in release.read_sums(self.sums).items()
            ],
        }
        self.run_json = tmp_path / "run.json"
        self.testpypi_json = tmp_path / "testpypi.json"
        self.tag_ref_json = tmp_path / "tag-ref.json"
        self.tag_object_json = tmp_path / "tag-object.json"

    def annotate(self) -> None:
        """Make the tag annotated: the ref names a tag object, which names the commit."""
        self.tag_ref["object"] = {"sha": TAG_OBJECT_SHA, "type": "tag"}
        self.tag_object = {
            "sha": TAG_OBJECT_SHA,
            "tag": f"v{RELEASE}",
            "object": {"sha": HEAD_SHA, "type": "commit"},
        }

    def check(self, run_id: str = "123") -> int:
        self.run_json.write_text(json.dumps(self.run), encoding="utf-8")
        self.testpypi_json.write_text(json.dumps(self.testpypi), encoding="utf-8")
        self.tag_ref_json.write_text(json.dumps(self.tag_ref), encoding="utf-8")
        tag_object_json = None
        if self.tag_object is not None:
            self.tag_object_json.write_text(json.dumps(self.tag_object), encoding="utf-8")
            tag_object_json = self.tag_object_json
        return release.check_promotion(
            run_id,
            run_json=self.run_json,
            dist_dir=self.dist,
            sums=self.sums,
            testpypi_json=self.testpypi_json,
            tag_ref_json=self.tag_ref_json,
            tag_object_json=tag_object_json,
        )


@pytest.fixture
def promotion(dist, tmp_path):
    return _Promotion(dist, tmp_path)


def test_check_promotion_accepts_a_matching_run(promotion):
    assert promotion.check() == 0


@pytest.mark.parametrize(
    ("field", "value"),
    [
        ("databaseId", 124),
        ("workflowName", "CI"),
        ("event", "workflow_dispatch"),
        ("status", "in_progress"),
        ("conclusion", "failure"),
        ("conclusion", None),
    ],
)
def test_check_promotion_needs_a_successful_release_push(promotion, field, value):
    promotion.run[field] = value
    with pytest.raises(SystemExit, match=f"run 123: {field} is"):
        promotion.check()


def test_check_promotion_refuses_a_run_id_that_is_not_a_number(promotion):
    with pytest.raises(SystemExit, match="is not a workflow run id"):
        promotion.check("12a")


@pytest.mark.parametrize("branch", ["main", "2026.10.6", None, 7])
def test_check_promotion_needs_a_v_tag(promotion, branch):
    promotion.run["headBranch"] = branch
    with pytest.raises(SystemExit, match=r"headBranch is .*, not a v\* tag"):
        promotion.check()


@pytest.mark.parametrize("branch", ["v2026.02.6", "v2026.2.30", "vmain"])
def test_check_promotion_needs_a_tag_in_the_grammar(promotion, branch):
    promotion.run["headBranch"] = branch
    with pytest.raises(SystemExit, match="is not YYYY.M.D|does not name a calendar date"):
        promotion.check()


def test_check_promotion_refuses_a_release_candidate(promotion):
    promotion.run["headBranch"] = "v2026.10.6rc1"
    with pytest.raises(SystemExit, match="release candidate"):
        promotion.check()


def test_check_promotion_reads_no_version_literal(promotion, monkeypatch, tmp_path):
    # The dispatching ref's literal may have moved on; the tag is the version.
    monkeypatch.setattr(release, "INIT_FILE", _init_file(tmp_path, "2026.10.7"))
    assert promotion.check() == 0


@pytest.mark.parametrize("sha", [None, "", "0" * 39, "G" * 40, "0" * 41])
def test_check_promotion_needs_a_head_sha(promotion, sha):
    promotion.run["headSha"] = sha
    with pytest.raises(SystemExit, match="run 123: headSha is .*, not a commit sha"):
        promotion.check()


def test_check_promotion_refuses_a_head_branch_that_is_not_the_tag_ref(promotion):
    # A run on another final tag (or on a branch named like one) has no ref of its own here.
    promotion.run["headBranch"] = "v2026.10.5"
    with pytest.raises(SystemExit, match="headBranch 'v2026.10.5' is not the tag ref"):
        promotion.check()
    promotion.run["headBranch"] = f"v{RELEASE}"
    promotion.tag_ref["ref"] = f"refs/heads/v{RELEASE}"
    with pytest.raises(SystemExit, match="is not the tag ref"):
        promotion.check()


def test_check_promotion_refuses_a_tag_on_another_commit(promotion):
    promotion.tag_ref["object"]["sha"] = "1" * 40
    with pytest.raises(SystemExit, match=f"names {'1' * 40}, not run 123's headSha {HEAD_SHA}"):
        promotion.check()


def test_check_promotion_dereferences_an_annotated_tag(promotion):
    promotion.annotate()
    assert promotion.check() == 0


def test_check_promotion_refuses_an_annotated_tag_on_another_commit(promotion):
    promotion.annotate()
    promotion.tag_object["object"]["sha"] = "1" * 40
    with pytest.raises(SystemExit, match="not run 123's headSha"):
        promotion.check()


def test_check_promotion_needs_the_annotated_tag_object(promotion):
    promotion.annotate()
    promotion.tag_object = None
    with pytest.raises(SystemExit, match="--tag-object-json must give its tag object"):
        promotion.check()


def test_check_promotion_refuses_another_tag_object(promotion):
    promotion.annotate()
    promotion.tag_object["sha"] = "2" * 40
    with pytest.raises(SystemExit, match="the tag object JSON describes"):
        promotion.check()


def test_check_promotion_dereferences_once(promotion):
    promotion.annotate()
    promotion.tag_object["object"]["type"] = "tag"
    with pytest.raises(SystemExit, match="names a 'tag', not a commit"):
        promotion.check()


@pytest.mark.parametrize("kind", ["tree", "blob", None])
def test_check_promotion_refuses_a_tag_on_a_non_commit(promotion, kind):
    promotion.tag_ref["object"]["type"] = kind
    with pytest.raises(SystemExit, match="not a commit or an annotated tag"):
        promotion.check()


def test_check_promotion_refuses_a_changed_file(promotion):
    wheel = next(promotion.dist.glob("*-cp310-cp310-win_amd64.whl"))
    with wheel.open("ab") as handle:
        handle.write(b"tampered")
    with pytest.raises(SystemExit, match=r"run 123's files does not match SHA256SUMS.*win_amd64"):
        promotion.check()


def test_check_promotion_refuses_sums_that_omit_a_file(promotion):
    lines = promotion.sums.read_text(encoding="utf-8").splitlines(keepends=True)
    promotion.sums.write_text("".join(lines[1:]), encoding="utf-8")
    with pytest.raises(
        SystemExit,
        match=r"run 123's files does not match SHA256SUMS: missing \[\], extra",
    ):
        promotion.check()


def test_check_promotion_refuses_a_different_testpypi_digest(promotion):
    promotion.testpypi["urls"][0]["digests"]["sha256"] = DIGEST_A
    name = promotion.testpypi["urls"][0]["filename"]
    with pytest.raises(SystemExit, match=re.escape(f"digest differs ['{name}']")):
        promotion.check()


def test_check_promotion_refuses_a_testpypi_file_set_that_differs(promotion):
    dropped = promotion.testpypi["urls"].pop()
    with pytest.raises(SystemExit, match=re.escape(f"missing ['{dropped['filename']}']")):
        promotion.check()
    promotion.testpypi["urls"] += [dropped, {"filename": "x.whl", "digests": {"sha256": DIGEST_A}}]
    with pytest.raises(SystemExit, match=re.escape("extra ['x.whl']")):
        promotion.check()
    promotion.testpypi["urls"].append(dict(dropped))
    with pytest.raises(SystemExit, match="lists .* twice"):
        promotion.check()


def test_check_promotion_refuses_another_testpypi_version(promotion):
    promotion.testpypi["info"]["version"] = "2026.10.5"
    with pytest.raises(SystemExit, match="describes version '2026.10.5'"):
        promotion.check()


@pytest.mark.parametrize("annotated", [False, True])
def test_check_promotion_cli(promotion, annotated):
    if annotated:
        promotion.annotate()
    promotion.check()
    argv = [
        "check-promotion",
        "123",
        "--run-json",
        str(promotion.run_json),
        "--dist",
        str(promotion.dist),
        "--sums",
        str(promotion.sums),
        "--testpypi-json",
        str(promotion.testpypi_json),
        "--tag-ref-json",
        str(promotion.tag_ref_json),
    ]
    if annotated:
        argv += ["--tag-object-json", str(promotion.tag_object_json)]
    assert release.main(argv) == 0
