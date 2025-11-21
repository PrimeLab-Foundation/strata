from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

ext_modules = [
    Pybind11Extension(
        "strata._strata",
        [
            "strata/cpp/module_pybind.cpp",
            "strata/cpp/dummy/dummy.cpp",
            "strata/cpp/json_document.cpp",
            "strata/cpp/json_cursor.cpp",
            "strata/cpp/json_parse.cpp",
        ],
        cxx_std=20,
    ),
]

setup(
    # Metadata mostly comes from pyproject.toml (PEP 621)
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
