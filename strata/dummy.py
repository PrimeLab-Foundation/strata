from . import _strata as _c

# --- DUMMY FUNCTIONS FOR EDUCATIONAL PURPOSE ---------------------------------


def add(
    a: int,
    b: int,
) -> int:
    return _c.add(a, b)


def hello(
    name: str,
) -> str:
    return _c.hello(name)


__ALL__ = ["add", "hello"]
