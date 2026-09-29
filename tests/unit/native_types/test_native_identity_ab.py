"""scripts/identity_ab.py's own gating (M15b acceptance item 4, review finding 2).

`image_identity` exits 2 when it cannot read an image at all -- "unverified",
not "differs". Pins that identity_ab.py treats that the same as a proven
difference (exit 1): a comparison it could not make is not evidence the code
is identical, and neither is comparing an image with itself or one that is
missing on either side.
"""

from scripts import identity_ab


def test_compare_images_refuses_to_compare_a_file_with_itself(tmp_path):
    image = tmp_path / "a.so"
    image.write_bytes(b"x")
    calls = []
    status = identity_ab.compare_images(
        image, image, tmp_path / "out.json", "label", lambda argv: calls.append(argv) or 0
    )
    assert status == 2
    assert calls == []  # image_identity is never even invoked


def test_compare_images_fails_on_a_missing_image_on_either_side(tmp_path):
    present = tmp_path / "a.so"
    present.write_bytes(b"x")
    missing = tmp_path / "b.so"
    calls = []
    identity_main = lambda argv: calls.append(argv) or 0  # noqa: E731

    assert (
        identity_ab.compare_images(present, missing, tmp_path / "o.json", "l", identity_main) == 2
    )
    assert (
        identity_ab.compare_images(missing, present, tmp_path / "o.json", "l", identity_main) == 2
    )
    assert calls == []


def test_compare_images_delegates_to_image_identity_when_both_images_exist(tmp_path):
    a = tmp_path / "a.so"
    b = tmp_path / "b.so"
    a.write_bytes(b"x")
    b.write_bytes(b"y")
    out_json = tmp_path / "out.json"
    calls = []

    def fake_identity_main(argv):
        calls.append(argv)
        return 1  # a genuine code-section difference

    status = identity_ab.compare_images(a, b, out_json, "label", fake_identity_main)
    assert status == 1
    assert calls == [[str(a), str(b), "--json", str(out_json)]]


def _stub_build(monkeypatch, *, plain_status: int, pgo_status: int) -> list:
    """Stub every build step so `main()` can be exercised without a real
    compiler, and record which comparisons it asked for."""
    monkeypatch.setattr(identity_ab, "worktree_checkout", lambda ref, arm_dir, **kw: None)
    monkeypatch.setattr(identity_ab, "worktree_remove", lambda arm_dir, **kw: None)

    def fake_build_plain(arm_dir, out_dir, dest_name, **kw):
        path = out_dir / f"{dest_name}{identity_ab.EXT}"
        out_dir.mkdir(parents=True, exist_ok=True)
        path.write_bytes(b"x")
        return path

    def fake_build_pgo(arm_dir, out_dir, dest_name, **kw):
        image = fake_build_plain(arm_dir, out_dir, dest_name)
        profile = out_dir / f"{dest_name}.profdata"
        profile.write_bytes(b"p")
        return image, profile

    def fake_held_profile_rebuild(arm_dir, profile, out_dir, dest_name, **kw):
        return fake_build_plain(arm_dir, out_dir, dest_name)

    monkeypatch.setattr(identity_ab, "build_plain", fake_build_plain)
    monkeypatch.setattr(identity_ab, "build_pgo", fake_build_pgo)
    monkeypatch.setattr(identity_ab, "held_profile_rebuild", fake_held_profile_rebuild)

    labels: list[str] = []

    def fake_compare_images(a, b, out_json, label, identity_main):
        labels.append(label)
        out_json.write_text("{}", encoding="utf-8")
        return pgo_status if "pgo" in label else plain_status

    monkeypatch.setattr(identity_ab, "compare_images", fake_compare_images)
    return labels


def test_main_fails_when_the_held_profile_comparison_is_unreadable(tmp_path, monkeypatch):
    labels = _stub_build(monkeypatch, plain_status=0, pgo_status=2)
    exit_code = identity_ab.main(
        ["--base", "abc123", "--arm-dir", str(tmp_path / "arm"), "--out", str(tmp_path / "out")]
    )
    assert exit_code == 1
    assert set(labels) == {"plain", "pgo (held profile)"}


def test_main_fails_when_the_plain_comparison_differs(tmp_path, monkeypatch):
    _stub_build(monkeypatch, plain_status=1, pgo_status=0)
    exit_code = identity_ab.main(
        ["--base", "abc123", "--arm-dir", str(tmp_path / "arm"), "--out", str(tmp_path / "out")]
    )
    assert exit_code == 1


def test_main_succeeds_only_when_both_comparisons_are_identical(tmp_path, monkeypatch):
    _stub_build(monkeypatch, plain_status=0, pgo_status=0)
    exit_code = identity_ab.main(
        ["--base", "abc123", "--arm-dir", str(tmp_path / "arm"), "--out", str(tmp_path / "out")]
    )
    assert exit_code == 0


def test_dry_run_prints_the_plan_and_builds_or_compares_nothing(tmp_path, monkeypatch, capsys):
    calls = []
    monkeypatch.setattr(
        identity_ab, "compare_images", lambda *a, **kw: calls.append("compared") or 0
    )
    exit_code = identity_ab.main(
        [
            "--base",
            "abc123",
            "--arm-dir",
            str(tmp_path / "arm"),
            "--out",
            str(tmp_path / "out"),
            "--dry-run",
        ]
    )
    assert exit_code == 0
    assert calls == []
    out = capsys.readouterr().out
    assert "git worktree add" in out
    assert not (tmp_path / "out").exists()
