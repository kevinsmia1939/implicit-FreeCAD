"""FreeCAD document object and Coin3D view provider for implicit gyroids."""

import FreeCAD as App

from .implicit_core import Bounds, Gyroid, extract_mesh


GEOMETRY_PROPERTIES = {
    "Period", "Level", "LengthX", "LengthY", "LengthZ", "Resolution", "Placement"
}


class ImplicitGyroidProxy:
    """Persistent mathematical feature; generated triangles are never authoritative."""

    def __init__(self, obj):
        obj.Proxy = self
        self._mesh = None
        self._updating = False
        obj.addProperty("App::PropertyLength", "Period", "Implicit Geometry",
                        "Gyroid unit-cell period").Period = 10.0
        obj.addProperty("App::PropertyFloat", "Level", "Implicit Geometry",
                        "Dimensionless level-set constant").Level = 0.0
        for name in ("LengthX", "LengthY", "LengthZ"):
            obj.addProperty("App::PropertyLength", name, "Domain",
                            "Finite rectangular-domain size")
            setattr(obj, name, 10.0)
        obj.addProperty("App::PropertyIntegerConstraint", "Resolution", "Display",
                        "Visualization cells along each axis").Resolution = (24, 4, 200, 1)
        if "Placement" not in obj.PropertiesList:
            obj.addProperty("App::PropertyPlacement", "Placement", "Domain",
                            "Spatial placement of the bounded implicit geometry")
        obj.addProperty("App::PropertyInteger", "TriangleCount", "Display",
                        "Current visualization-cache triangle count", 1)
        obj.setEditorMode("TriangleCount", 1)

    def execute(self, obj):
        if self._updating:
            return
        self._updating = True
        try:
            period = float(obj.Period.Value)
            lengths = (float(obj.LengthX.Value), float(obj.LengthY.Value), float(obj.LengthZ.Value))
            surface = Gyroid(period, float(obj.Level))
            local = extract_mesh(surface, Bounds((0.0, 0.0, 0.0), lengths), int(obj.Resolution))
            placement = obj.Placement
            transformed_vertices = []
            for point in local.vertices:
                transformed = placement.multVec(App.Vector(*point))
                transformed_vertices.append((transformed.x, transformed.y, transformed.z))
            local.vertices = transformed_vertices
            rotation = placement.Rotation
            transformed_normals = []
            for normal in local.normals:
                transformed = rotation.multVec(App.Vector(*normal))
                transformed_normals.append((transformed.x, transformed.y, transformed.z))
            local.normals = transformed_normals
            self._mesh = local
            obj.TriangleCount = len(local.triangles)
            if App.GuiUp and obj.ViewObject and obj.ViewObject.Proxy:
                obj.ViewObject.Proxy.update_mesh(local)
        finally:
            self._updating = False

    def onChanged(self, obj, property_name):
        if property_name in GEOMETRY_PROPERTIES and not self._updating:
            obj.touch()

    def dumps(self):
        return None

    def loads(self, _state):
        self._mesh = None
        self._updating = False

    def onDocumentRestored(self, obj):
        # The mathematical properties persist, while the display mesh does not.
        # Recreate it here because FreeCAD clears the touched state after the
        # restoration callback has completed.
        self._mesh = None
        self._updating = False
        self.execute(obj)


class ViewProviderImplicitGyroid:
    """Retains Coin nodes and only replaces their field buffers on recompute."""

    def __init__(self, view_object):
        self._ensure_properties(view_object)
        # Assigning Proxy invokes attach() synchronously, so all properties used
        # by attach must exist before this assignment.
        view_object.Proxy = self

    @staticmethod
    def _ensure_properties(view_object):
        if "SurfaceColor" not in view_object.PropertiesList:
            view_object.addProperty("App::PropertyColor", "SurfaceColor", "Display",
                                    "Implicit-surface color")
            view_object.SurfaceColor = (0.20, 0.70, 0.85)

    def attach(self, view_object):
        from pivy import coin

        # On FCStd restore the view proxy may attach before dynamically added
        # view properties have been replayed.
        self._ensure_properties(view_object)
        self._view_object = view_object
        self._coordinates = coin.SoCoordinate3()
        self._normals = coin.SoNormal()
        self._faces = coin.SoIndexedFaceSet()
        self._material = coin.SoMaterial()
        self._wire_material = coin.SoMaterial()
        self._wire_material.diffuseColor = (0.05, 0.05, 0.05)
        self._normal_binding = coin.SoNormalBinding()
        self._normal_binding.value = coin.SoNormalBinding.PER_VERTEX_INDEXED
        hints = coin.SoShapeHints()
        hints.vertexOrdering = coin.SoShapeHints.COUNTERCLOCKWISE

        shaded = coin.SoSeparator()
        for node in (self._material, hints, self._coordinates, self._normals,
                     self._normal_binding, self._faces):
            shaded.addChild(node)

        draw_style = coin.SoDrawStyle()
        draw_style.style = coin.SoDrawStyle.LINES
        wire = coin.SoSeparator()
        for node in (self._wire_material, draw_style, self._coordinates, self._faces):
            wire.addChild(node)

        combined = coin.SoSeparator()
        combined.addChild(shaded)
        combined.addChild(wire)
        self._switch = coin.SoSwitch()
        self._switch.addChild(shaded)
        self._switch.addChild(wire)
        self._switch.addChild(combined)
        for mode in ("Shaded", "Wireframe", "Flat Lines"):
            view_object.addDisplayMode(self._switch, mode)
        self._update_color()
        if hasattr(self, "_pending_state"):
            self._apply_state(self._pending_state)
            del self._pending_state

    def update_mesh(self, mesh):
        if not hasattr(self, "_coordinates"):
            return
        # setValues() overwrites/grows a Coin multi-field but does not truncate
        # it, so resize explicitly when lowering the feature resolution.
        self._coordinates.point.setNum(len(mesh.vertices))
        self._normals.vector.setNum(len(mesh.normals))
        self._coordinates.point.setValues(0, len(mesh.vertices), mesh.vertices)
        self._normals.vector.setValues(0, len(mesh.normals), mesh.normals)
        indices = [index for triangle in mesh.triangles for index in (*triangle, -1)]
        self._faces.coordIndex.setNum(len(indices))
        self._faces.coordIndex.setValues(0, len(indices), indices)

    def updateData(self, obj, _property_name):
        if getattr(obj.Proxy, "_mesh", None) is not None:
            self.update_mesh(obj.Proxy._mesh)

    def onChanged(self, _view_object, property_name):
        if property_name == "SurfaceColor" and hasattr(self, "_material"):
            self._update_color()

    def _update_color(self):
        color = self._view_object.SurfaceColor
        self._material.diffuseColor = (float(color[0]), float(color[1]), float(color[2]))

    def getDisplayModes(self, _view_object):
        return ["Shaded", "Wireframe", "Flat Lines"]

    def getDefaultDisplayMode(self):
        return "Shaded"

    def setDisplayMode(self, mode):
        self._switch.whichChild = {"Shaded": 0, "Wireframe": 1, "Flat Lines": 2}.get(mode, 0)
        return mode

    def dumps(self):
        if not hasattr(self, "_view_object"):
            return None
        color = self._view_object.SurfaceColor
        return {
            "surface_color": tuple(float(component) for component in color),
        }

    def loads(self, state):
        if hasattr(self, "_view_object"):
            self._apply_state(state)
        else:
            self._pending_state = state

    def _apply_state(self, state):
        if not state:
            return
        if "surface_color" in state:
            self._view_object.SurfaceColor = tuple(state["surface_color"])


def create_gyroid(document=None, name="ImplicitGyroid"):
    document = document or App.ActiveDocument or App.newDocument()
    obj = document.addObject("App::FeaturePython", name)
    obj.Label = "Implicit Gyroid"
    ImplicitGyroidProxy(obj)
    if App.GuiUp:
        ViewProviderImplicitGyroid(obj.ViewObject)
    document.recompute()
    return obj

