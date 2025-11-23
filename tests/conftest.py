import pytest

import strata
from strata import JsonCursor


@pytest.fixture
def sample_json_text() -> str:
    return """
    {
      "name": "Alice",
      "age": 30,
      "active": true,
      "score": 99.5,
      "tags": ["dev", "json"],
      "address": {
        "city": "Berlin",
        "zip": "10115"
      }
    }
    """


@pytest.fixture
def json_root(sample_json_text) -> JsonCursor:
    """
    Return a JsonCursor positioned at the root of the sample JSON document.
    """
    return strata.parse_json(sample_json_text)
