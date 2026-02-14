import os
import platform
import shutil
import subprocess
import sys

# Get Python include directory
from sysconfig import get_config_var, get_paths

from setuptools import Extension, find_packages, setup
from setuptools.command.build_ext import build_ext

python_include = get_paths()['include']


class TestGatedBuildExt(build_ext):
    """Custom build_ext that runs C++ tests before build and Python tests after."""

    def run(self):
        """Override run to add test gates.

        Per Rule 15: C++ tests run by DEFAULT. Tests are part of build, not optional.
        Escape hatch: SKIP_TESTS=1 (strongly discouraged, logged, not for CI/release).
        """
        # Check for escape hatch (strongly discouraged)
        skip_tests = os.environ.get('SKIP_TESTS', '0') == '1'

        if skip_tests:
            print("\n" + "⚠️ " * 35)
            print("⚠️  WARNING: SKIP_TESTS=1 - Tests are being SKIPPED")
            print("⚠️  This is STRONGLY DISCOURAGED and NOT allowed for CI/releases")
            print("⚠️  Per Rule 15: Tests are MANDATORY part of build")
            print("⚠️ " * 35 + "\n")
        else:
            print("\n" + "=" * 70)
            print("PHASE 1: Running C++ tests (pre-build gate)")
            print("Per Rule 15: C++ tests are DEFAULT and MANDATORY")
            print("=" * 70)

            # Run C++ tests before building (REQUIRED by Rule 15)
            if not self.run_cpp_tests():
                raise RuntimeError("C++ tests failed - aborting build (Rule 15 gate)")

        print("\n" + "=" * 70)
        print("PHASE 2: Building C++ extension")
        print("=" * 70)

        # Run the actual build
        super().run()

        if not skip_tests:
            print("\n" + "=" * 70)
            print("PHASE 3: Running Python tests (post-build gate)")
            print("Per Rule 14: Python tests gate the build")
            print("=" * 70)

            # Run Python tests after building (REQUIRED by Rule 14)
            if not self.run_python_tests():
                raise RuntimeError("Python tests failed - rolling back build (Rule 14 gate)")

            print("\n" + "=" * 70)
            print("BUILD SUCCESS: All tests passed (Rules 14+15 compliant)")
            print("=" * 70 + "\n")
        else:
            print("\n" + "=" * 70)
            print("BUILD COMPLETE (tests SKIPPED via SKIP_TESTS=1)")
            print("⚠️  WARNING: This build is NOT release-ready")
            print("Run 'make gate' to verify compliance with Rules 14+15")
            print("=" * 70 + "\n")

    def run_cpp_tests(self):
        """Run C++ tests using CMake/ctest."""
        try:
            # Check if build directory exists, create if not
            build_dir = "build"
            if not os.path.exists(build_dir):
                os.makedirs(build_dir)

            # Run CMake configuration
            print("Configuring C++ tests with CMake...")
            result = subprocess.run(
                ["cmake", "-S", ".", "-B", build_dir], check=False, capture_output=True, text=True
            )

            if result.returncode != 0:
                print("CMake configuration failed:")
                print(result.stdout)
                print(result.stderr)
                return False

            # Build C++ tests
            print("Building C++ tests...")
            result = subprocess.run(
                ["cmake", "--build", build_dir], check=False, capture_output=True, text=True
            )

            if result.returncode != 0:
                print("C++ test build failed:")
                print(result.stdout)
                print(result.stderr)
                return False

            # Run C++ tests
            print("Running C++ tests...")
            result = subprocess.run(
                ["ctest", "--test-dir", build_dir, "--output-on-failure"],
                check=False,
                capture_output=True,
                text=True,
            )

            print(result.stdout)
            if result.returncode != 0:
                print("C++ tests failed:")
                print(result.stderr)
                return False

            print("✅ All C++ tests passed")
            return True

        except FileNotFoundError as e:
            print(f"Error: CMake/ctest not found ({e})")
            print("C++ tests are mandatory. Install CMake and re-run the build.")
            return False
        except Exception as e:
            print(f"Error running C++ tests: {e}")
            return False

    def run_python_tests(self):
        """Run Python tests using pytest.

        Note: In pip's isolated build environment, pytest may not be available.
        For full compliance with Rule 14, use 'make install' or 'make gate'.
        """
        try:
            # Check if pytest is available
            result = subprocess.run(
                [sys.executable, "-c", "import pytest"], check=False, capture_output=True, text=True
            )

            if result.returncode != 0:
                print("Error: pytest not available in build environment")
                print("Python tests are mandatory. Install pytest and re-run the build.")
                return False

            print("Running Python test suite...")
            project_root = os.path.abspath(os.path.dirname(__file__))
            build_cmd = self.get_finalized_command("build")
            build_platlib = os.path.abspath(build_cmd.build_platlib)
            build_py = self.get_finalized_command("build_py")
            # Copy pure-Python modules into platlib so the package and extension live together.
            build_py.build_lib = build_platlib
            build_py.ensure_finalized()
            build_py.run()
            build_lib = build_platlib
            pkg_src = os.path.join(project_root, "python", "strata")
            pkg_dst = os.path.join(build_lib, "strata")
            if not os.path.exists(os.path.join(pkg_dst, "__init__.py")):
                os.makedirs(pkg_dst, exist_ok=True)
                shutil.copytree(pkg_src, pkg_dst, dirs_exist_ok=True)
            env = os.environ.copy()
            # Ensure build_lib (with extension) is searched before source tree.
            paths = [build_lib, project_root]
            existing = env.get("PYTHONPATH")
            if existing:
                paths.append(existing)
            env["PYTHONPATH"] = os.pathsep.join(paths)

            result = subprocess.run(
                [
                    sys.executable,
                    "-c",
                    (
                        "import sys; "
                        f"sys.path[:0] = [r'{build_lib}', r'{project_root}']; "
                        "import pytest; "
                        "raise SystemExit(pytest.main(['tests/py', '-v', '--tb=short']))"
                    ),
                ],
                check=False,
                capture_output=True,
                text=True,
                cwd=project_root,
                env=env,
            )

            print(result.stdout)
            if result.returncode != 0:
                print("Python tests failed:")
                print(result.stderr)
                return False

            print("✅ All Python tests passed")
            return True

        except Exception as e:
            print(f"Error checking for pytest: {e}")
            return False


# PGO mode: set via environment variable
# PGO_MODE=generate python -m pip install -e .  (generate profile)
# PGO_MODE=use python -m pip install -e .       (use profile)
pgo_mode = os.environ.get('PGO_MODE', '').lower()  # 'generate', 'use', or ''
enable_lto = os.environ.get('STRATA_ENABLE_LTO', '0') == '1'
pgo_profile = os.environ.get('STRATA_PGO_PROFILE', 'build/pgo/strata.profdata')


def detect_compiler_kind():
    cxx = os.environ.get("CXX") or get_config_var("CXX") or ""
    cxx_lower = cxx.lower()
    if "clang" in cxx_lower:
        return "clang"
    if "g++" in cxx_lower or "gcc" in cxx_lower:
        return "gcc"
    return "unknown"


compiler_kind = os.environ.get("STRATA_COMPILER_KIND", "").lower() or detect_compiler_kind()


def is_universal_build():
    parts = [
        os.environ.get("ARCHFLAGS"),
        os.environ.get("CFLAGS"),
        os.environ.get("CXXFLAGS"),
        get_config_var("ARCHFLAGS"),
        get_config_var("CFLAGS"),
        get_config_var("CXXFLAGS"),
    ]
    flags = " ".join(p for p in parts if p)
    return "-arch arm64" in flags and "-arch x86_64" in flags


def build_compile_flags():
    flags = [
        "-std=c++20",
        "-O3",
        "-DNDEBUG",
        "-fno-math-errno",
        "-fassociative-math",
        "-funroll-loops",
        "-ffunction-sections",
        "-fdata-sections",
        "-fvisibility=hidden",
    ]
    if not is_universal_build():
        flags.append("-march=native")
    if enable_lto:
        flags.append("-flto=thin" if compiler_kind == "clang" else "-flto")
        if compiler_kind == "gcc":
            flags.append("-fno-fat-lto-objects")
    if pgo_mode == "generate":
        if compiler_kind == "clang":
            flags.append("-fprofile-instr-generate")
        elif compiler_kind == "gcc":
            flags.append("-fprofile-generate")
    elif pgo_mode == "use":
        if compiler_kind == "clang":
            flags.append(f"-fprofile-instr-use={pgo_profile}")
        elif compiler_kind == "gcc":
            flags.extend(["-fprofile-use", "-fprofile-correction"])
    return flags


def build_link_flags():
    flags = ["-O3"]
    if enable_lto:
        flags.append("-flto=thin" if compiler_kind == "clang" else "-flto")
    if pgo_mode == "generate":
        if compiler_kind == "clang":
            flags.append("-fprofile-instr-generate")
        elif compiler_kind == "gcc":
            flags.append("-fprofile-generate")
    elif pgo_mode == "use":
        if compiler_kind == "clang":
            flags.append(f"-fprofile-instr-use={pgo_profile}")
        elif compiler_kind == "gcc":
            flags.extend(["-fprofile-use", "-fprofile-correction"])
    return flags


ext_modules = [
    Extension(
        "strata._strata",
        sources=[
            # Python C API bindings
            "src/strata/bindings/python_module.cpp",
            "src/strata/bindings/python_dumps.cpp",
            "src/strata/bindings/python_loads.cpp",
            "src/strata/bindings/python_tape.cpp",
            "src/strata/bindings/python_document.cpp",
            "src/strata/bindings/python_ndjson.cpp",
            "src/strata/bindings/python_mmap.cpp",
            "src/strata/bindings/python_search.cpp",
            "src/strata/bindings/python_lazy_cursor.cpp",
            # C++ core (KEEP)
            "src/strata/json/json_document.cpp",
            "src/strata/json/json_cursor.cpp",
            "src/strata/json/json_lazy_cursor.cpp",
            "src/strata/json/json_parse.cpp",
            "src/strata/json/json_tape.cpp",
            "src/strata/search/search_compile.cpp",
            "src/strata/search/search_eval.cpp",
            "src/strata/search/search_ndjson_fused.cpp",
            "src/strata/json/ndjson_stream.cpp",
            "src/strata/json/parallel_ndjson.cpp",
            "src/strata/json/json_mmap.cpp",
            # Utilities (KEEP)
            "src/strata/util/ryu_dtoa.cpp",
            "src/strata/util/dragonbox.cpp",
            "src/strata/util/simd_escape.cpp",
            "src/strata/util/simd_string_eq.cpp",
            "src/strata/util/simd_newline.cpp",
            "src/strata/util/simd_utf8.cpp",
            "src/strata/util/simd_numbers.cpp",
            "src/strata/util/simd_structural.cpp",
            "src/strata/util/fast_parse.cpp",
            "src/strata/util/thread_pool.cpp",
        ],
        include_dirs=[
            "include",
            python_include,
        ],
        extra_compile_args=(
            build_compile_flags()
            if sys.platform != "win32"
            else [
                "/std:c++20",
                "/O2",
                "/DNDEBUG",
            ]
            + (["/arch:AVX2"] if (platform.machine() in ("AMD64", "x86_64")) else [])
            + (["/GL"] if enable_lto else [])
        ),
        extra_link_args=(
            build_link_flags()
            if sys.platform != "win32"
            else [
                "/LTCG",
            ]
            if enable_lto
            else []
        ),
        language="c++",
    ),
]

setup(
    # Metadata comes from pyproject.toml (PEP 621)
    packages=find_packages(where="python"),
    package_dir={"": "python"},
    ext_modules=ext_modules,
    cmdclass={
        'build_ext': TestGatedBuildExt,
    },
)
