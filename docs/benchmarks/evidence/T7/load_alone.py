import importlib.machinery, importlib.util, sys, types, pathlib
name, path = sys.argv[1], sys.argv[2]
package = types.ModuleType("strata"); package.__path__ = [str(pathlib.Path(path).parent)]
sys.modules["strata"] = package
loader = importlib.machinery.ExtensionFileLoader(name, path)
spec = importlib.util.spec_from_loader(name, loader)
try:
    module = importlib.util.module_from_spec(spec); loader.exec_module(module)
    print("loaded", sorted(n for n in sys.modules if n.startswith("strata.")))
except ImportError as e:
    print("ImportError:", e)
