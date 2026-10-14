"""GUI integration smoke test executed by the FreeCAD application."""

import os
import sys
import tempfile

import FreeCAD as App
import FreeCADGui as Gui


REPOSITORY = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ADDON_ROOT = os.path.join(REPOSITORY, "addon")
if ADDON_ROOT not in sys.path:
    sys.path.insert(0, ADDON_ROOT)

from ImplicitGyroid.freecad_feature import create_gyroid  # noqa: E402


def require(condition, message):
    if not condition:
        raise AssertionError(message)


require(bool(App.GuiUp), "FreeCAD GUI is not active")
document = App.newDocument("ImplicitGyroidGuiSmoke")
obj = create_gyroid(document)
initial_coordinate_node = obj.ViewObject.Proxy._coordinates
initial_index_node = obj.ViewObject.Proxy._faces
obj.Resolution = 10
document.recompute()
view_proxy = obj.ViewObject.Proxy
mesh = obj.Proxy._mesh

require(view_proxy.__class__.__name__ == "ViewProviderImplicitGyroid",
        "view-provider proxy was not attached")
require(view_proxy._coordinates is initial_coordinate_node,
        "resolution change replaced the retained Coin coordinate node")
require(view_proxy._faces is initial_index_node,
        "resolution change replaced the retained Coin index node")
require(
    view_proxy._coordinates.point.getNum() == len(mesh.vertices),
    "Coin coordinate buffer {} does not match cache {}".format(
        view_proxy._coordinates.point.getNum(), len(mesh.vertices)
    ),
)
require(
    view_proxy._normals.vector.getNum() == len(mesh.normals),
    "Coin normal buffer {} does not match cache {}".format(
        view_proxy._normals.vector.getNum(), len(mesh.normals)
    ),
)
require(
    view_proxy._faces.coordIndex.getNum() == 4 * len(mesh.triangles),
    "Coin index buffer {} does not match cache {}".format(
        view_proxy._faces.coordIndex.getNum(), 4 * len(mesh.triangles)
    ),
)

for mode, expected_child in (("Shaded", 0), ("Wireframe", 1), ("Flat Lines", 2)):
    obj.ViewObject.DisplayMode = mode
    require(view_proxy._switch.whichChild.getValue() == expected_child,
            "display mode {!r} did not select the expected Coin child".format(mode))

obj.ViewObject.SurfaceColor = (0.9, 0.3, 0.1)
color = view_proxy._material.diffuseColor.getValues()[0].getValue()
require(all(abs(actual - expected) < 1.0e-5
            for actual, expected in zip(color, (0.9, 0.3, 0.1))),
        "surface color did not propagate to Coin")

Gui.activeDocument().activeView().viewAxonometric()
Gui.activeDocument().activeView().fitAll()
require(obj.Proxy._mesh is mesh, "camera operations regenerated the geometry cache")

with tempfile.TemporaryDirectory(prefix="implicit-gyroid-gui-") as temporary_directory:
    path = os.path.join(temporary_directory, "gui-smoke.FCStd")
    document.saveAs(path)
    App.closeDocument(document.Name)
    loaded_document = App.openDocument(path)
    loaded = loaded_document.getObject("ImplicitGyroid")
    require(loaded is not None, "GUI reload did not restore the feature")
    require(loaded.Proxy._mesh is not None, "GUI reload did not rebuild the cache")
    require(loaded.ViewObject.Proxy.__class__.__name__ == "ViewProviderImplicitGyroid",
            "GUI reload did not restore the view provider")
    require(loaded.ViewObject.Proxy._coordinates.point.getNum() == len(loaded.Proxy._mesh.vertices),
            "GUI reload did not upload the Coin coordinate buffer")
    restored_color = loaded.ViewObject.SurfaceColor
    require(
        all(abs(float(actual) - expected) < 3.0e-3
            for actual, expected in zip(restored_color, (0.9, 0.3, 0.1))),
        "GUI reload did not preserve surface color; restored={!r}".format(restored_color),
    )
    require(loaded.ViewObject.DisplayMode == "Flat Lines",
            "GUI reload did not preserve the display mode")
    App.closeDocument(loaded_document.Name)

print("FreeCAD GUI smoke test passed: vertices={}, triangles={}".format(
    len(mesh.vertices), len(mesh.triangles)
))
Gui.getMainWindow().close()

