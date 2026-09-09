"""Profile controls must retain rival residency and complete C++ symbol names."""

from collections import Counter
from types import SimpleNamespace

import pytest

from benchmarks import profile_interleaved
from benchmarks.native_interleaved_profile import sampled_symbols


def test_sampled_symbols_preserves_clone_names_and_skips_unsampled():
    name = 'strata::bindings::(anonymous namespace)::Serializer::write(_object*) [clone .llvm.1]'
    report = f'  12.30% [.] {name}\n  0.00% [.] unused\n  5.0% [k] kernel\n  1.00% [.] {name}\n'
    assert sampled_symbols(report) == [name]
    assert sampled_symbols('# No samples\n') == []


@pytest.mark.parametrize('condition', ['resident', 'interleaved'])
def test_profile_condition_keeps_warmup_and_gc_contract(tmp_path, monkeypatch, condition):
    import sys

    import strata

    monkeypatch.chdir(tmp_path)
    dataset = tmp_path / 'benchmarks/data/generated/small/mixed.json'
    dataset.parent.mkdir(parents=True)
    dataset.write_text('{}')
    counts = Counter()

    def encoder(name):
        def call(*args, **kwargs):
            counts[name] += 1
            return b'{}'

        return call

    monkeypatch.setitem(sys.modules, 'orjson', SimpleNamespace(dumps=encoder('orjson')))
    monkeypatch.setitem(sys.modules, 'ujson', SimpleNamespace(dumps=encoder('ujson')))
    monkeypatch.setitem(
        sys.modules, 'msgspec', SimpleNamespace(json=SimpleNamespace(encode=encoder('msgspec')))
    )
    monkeypatch.setattr(strata, 'dumps', encoder('strata'))
    monkeypatch.setattr(profile_interleaved.json, 'dumps', encoder('json'))
    monkeypatch.setattr(profile_interleaved.gc, 'collect', encoder('gc'))
    assert profile_interleaved.main(['3', '--condition', condition]) == 0
    assert counts['strata'] == 4
    expected_rivals = 4 if condition == 'interleaved' else 1
    assert all(counts[name] == expected_rivals for name in ('orjson', 'ujson', 'msgspec', 'json'))
    assert counts['gc'] == (15 if condition == 'interleaved' else 3)
