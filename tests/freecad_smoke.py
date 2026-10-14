"""Integration smoke test executed by FreeCADCmd, not system Python."""

import os
import sys
import tempfile

import FreeCAD as App


REPOSITORY = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ADDON_ROOT = os.path.join(REPOSITORY, "addon")
if ADDON_ROOT not in sys.path:
    sys.path.insert(0, ADDON_ROOT)

from ImplicitGyroid.freecad_feature import create_gyroid  # noqa: E402


def require(condition, message):
    if not condition:
        raise AssertionError(message)


document = App.newDocument("ImplicitGyroidSmoke")
obj = create_gyroid(document)
require(obj.Proxy.__class__.__name__ == "ImplicitGyroidProxy", "feature proxy was not attached")
require(obj.TriangleCount > 0, "initial recompute did not build a visualization mesh")
initial_count = obj.TriangleCount

obj.Level = 0.2
obj.Resolution = 12
document.recompute()
require(obj.TriangleCount > 0, "parameter recompute produced no visualization mesh")
require(obj.Proxy._mesh is not None, "visualization cache is missing")
require(len(obj.Proxy._mesh.triangles) == obj.TriangleCount, "cache count is inconsistent")
changed_count = obj.TriangleCount

with tempfile.TemporaryDirectory(prefix="implicit-gyroid-") as temporary_directory:
    path = os.path.join(temporary_directory, "smoke.FCStd")
    document.saveAs(path)
    App.closeDocument(document.Name)

    loaded_document = App.openDocument(path)
    loaded = loaded_document.getObject("ImplicitGyroid")
    require(loaded is not None, "saved feature was not restored")
    require(loaded.Proxy.__class__.__name__ == "ImplicitGyroidProxy", "proxy was not restored")
    require(abs(loaded.Level - 0.2) < 1.0e-12, "level property was not persisted")
    require(int(loaded.Resolution) == 12, "resolution property was not persisted")
    loaded_document.recompute()
    require(
        loaded.Proxy._mesh is not None,
        "reload recompute did not rebuild the cache; state={!r}, proxy={!r}".format(
            loaded.State, loaded.Proxy.__dict__
        ),
    )
    require(loaded.TriangleCount > 0, "reload recompute produced no triangles")
    App.closeDocument(loaded_document.Name)

print(
    "FreeCAD smoke test passed: initial_triangles={}, changed_triangles={}".format(
        initial_count, changed_count
    )
)

