import strata


def test_add():
    assert strata.dummy.add(1, 2) == 3
    assert strata.dummy.add(-5, 5) == 0


def test_hello():
    assert strata.dummy.hello("Alice") == "Hello, Alice!"
