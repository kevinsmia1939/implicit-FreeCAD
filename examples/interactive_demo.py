# SPDX-License-Identifier: LGPL-2.1-or-later
"""Interactive native demo. Run with the fork GUI, not system FreeCAD."""
import json
import os

import FreeCAD as App
import FreeCADGui as Gui
import Implicit
import ImplicitGui  # noqa: F401


def create_demo():
    Gui.activateWorkbench("NativeImplicitWorkbench")
    document = App.newDocument("ImplicitInteractiveDemo")
    document.Label = "Native implicit modelling demo"
    document.UndoMode = 1

    def feature(name, label, x, color):
        obj = document.addObject("Implicit::Feature", name)
        obj.Label = label
        obj.DomainMin = (-8, -8, -8)
        obj.DomainMax = (8, 8, 8)
        obj.Placement = App.Placement(App.Vector(x, 0, 0), App.Rotation())
        obj.ViewObject.Resolution = 24
        obj.ViewObject.SurfaceColor = color
        return obj

    gyroid = feature("EditableGyroid", "1 - Gyroid: edit Period / IsoValue", 0, (0.2, 0.7, 0.85))
    gyroid.Function = "Gyroid"
    gyroid.Period = 8
    gyroid.IsoValue = 0.17
    # Empty FieldGraph enables the native primitive property interface.
    gyroid.FieldGraph = ""

    band = feature("GyroidBand", "2 - Capped gyroid band: edit FieldGraph", 24, (0.85, 0.6, 0.2))
    Implicit.setFieldGraph(band, json.dumps({"schema": 1, "root": {
        "type": "intersection",
        "left": {"type": "band", "halfWidth": 0.3,
                 "field": {"type": "gyroid", "period": 8, "iso": 0.17}},
        "right": {"type": "box", "center": [0, 0, 0], "halfSize": [7, 7, 7]}}}))

    sphere = feature("EditableSphere", "3 - Sphere: edit Radius", 48, (0.4, 0.8, 0.4))
    sphere.Function = "Sphere"
    sphere.Radius = 6
    sphere.FieldGraph = ""

    difference = feature("FieldDifference", "4 - Sphere difference: edit FieldGraph", 72,
                         (0.8, 0.4, 0.65))
    Implicit.setFieldGraph(difference, json.dumps({"schema": 1, "root": {
        "type": "difference",
        "left": {"type": "sphere", "center": [0, 0, 0], "radius": 6},
        "right": {"type": "sphere", "center": [4, 0, 0], "radius": 5}}}))
    document.recompute()
    if os.environ.get("IMPLICIT_BACKGROUND_MESHING") == "1":
        for obj in document.Objects:
            obj.ViewObject.BackgroundMeshing = True
    Gui.activeDocument().activeView().viewAxonometric()
    Gui.activeDocument().activeView().fitAll()
    Gui.Selection.clearSelection()
    Gui.Selection.addSelection(gyroid)
    Gui.updateGui()
    App.Console.PrintMessage(
        "Native demo ready. Select Gyroid/Sphere and edit Data properties; "
        "use View for Resolution, AdaptiveSampling and DisplayMode.\n"
        "For composed objects, FieldGraph is authoritative. "
        "Save/reopen with this fork executable.\n")
    return document


demo = create_demo()
if os.environ.get("IMPLICIT_DEMO_CHECK") == "1":
    assert len(demo.Objects) == 4
    for obj in demo.Objects:
        assert obj.TypeId == "Implicit::Feature"
        assert obj.isValid()
        assert obj.ViewObject.TriangleCount > 0
    print("interactive native demo verified", flush=True)
    App.closeDocument(demo.Name)
    Gui.getMainWindow().close()
