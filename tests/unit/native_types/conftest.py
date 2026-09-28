"""Contract suites for native types (docs/architecture/native_types.md).

This directory exists to keep these tests out of the PGO profile, not to group
them by topic. The serializer's native tail and the ``parse_types`` revival are
cold paths that must carry zero training counts: a trained cold path is laid
out hot (E26-P23), and every trained test addition moves every row's profile
(E26-P7b). ``scripts/py_tests.py --training`` ignores this directory, and only
the PGO instrumented pass passes that flag; every gate runs these tests.

A native-type test placed anywhere else trains the profile. The autouse
``restore_config`` fixture of ``tests/unit/conftest.py`` applies here too.
"""
