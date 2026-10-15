# SPDX-License-Identifier: LGPL-2.1-or-later
"""Live Part/implicit workflow; requires this fork's Part/PartGui and OCCT bridge."""
import os
import FreeCAD as App
import FreeCADGui as Gui
import Part  # noqa: F401
import PartGui  # noqa: F401
import Implicit
import ImplicitGui  # noqa: F401

Gui.activateWorkbench("NativeImplicitWorkbench")
doc = App.newDocument("LivePartImplicitDemo")
doc.Label = "Live Part + implicit modelling"
doc.UndoMode = 1
box = doc.addObject("Part::Box", "BoundingBox")
box.Label = "1 - Part box: edit dimensions or Placement"
box.Length, box.Width, box.Height = 14, 14, 14
box.Placement.Base = (-7, -7, -7)
gyroid = doc.addObject("Implicit::Feature", "Gyroid")
gyroid.Label = "2 - Gyroid: edit Period / IsoValue"
gyroid.Period = 8
gyroid.IsoValue = 0.17
gyroid.ViewObject.Resolution = 16
band = doc.addObject("Implicit::Feature", "Band")
band.Label = "3 - Live band: edit Amount (dimensionless)"
band.Operation = "Band"
band.Left = gyroid
band.Amount = 0.3
band.ViewObject.Resolution = 16
result = doc.addObject("Implicit::Feature", "MixedIntersection")
result.Label = "4 - Live result: Part box intersected with band"
result.Operation = "Intersection"
result.Left, result.Right = box, band
result.Tolerance = 1e-7
result.ViewObject.Resolution = 16
result.ViewObject.SurfaceColor = (0.2, 0.7, 0.85)
doc.recompute()
for source in (box, gyroid, band):
    source.Visibility = False
if os.environ.get("IMPLICIT_BACKGROUND_MESHING") == "1":
    # Opt in after the initial model has been built. Subsequent edits extract
    # in the background; source Part recompute still runs on the main thread.
    result.ViewObject.BackgroundMeshing = True
Gui.activeDocument().activeView().viewAxonometric()
Gui.activeDocument().activeView().fitAll()
Gui.Selection.addSelection(band)
Gui.updateGui()
App.Console.PrintMessage(
    "Live mixed demo ready. Change Band.Amount, Gyroid.Period, or the Part box's "
    "dimensions/Placement, then recompute. MixedIntersection retains live links.\n"
    "This is an implicit result, not a STEP-exportable B-Rep.\n")

if os.environ.get("IMPLICIT_DEMO_CHECK") == "1":
    assert all(obj.isValid() for obj in doc.Objects)
    assert result.ViewObject.TriangleCount > 0
    before = result.CacheRevision
    box.Length = 12
    doc.recompute()
    assert result.CacheRevision > before
    if result.ViewObject.Meshing:
        from PySide6 import QtCore
        loop = QtCore.QEventLoop()
        poll = QtCore.QTimer()
        poll.setInterval(5)
        poll.timeout.connect(lambda: loop.quit() if not result.ViewObject.Meshing else None)
        deadline = QtCore.QTimer()
        deadline.setSingleShot(True)
        deadline.timeout.connect(loop.quit)
        poll.start()
        deadline.start(60000)
        loop.exec()
        poll.stop()
        deadline.stop()
        assert not result.ViewObject.Meshing, "background demo timed out"
    assert result.ViewObject.MeshStatus == "Ready", result.ViewObject.MeshStatus
    assert result.ViewObject.TriangleCount > 0
    assert band.Left == gyroid and result.Left == box and result.Right == band
    print("live Part implicit demo verified", flush=True)
    App.closeDocument(doc.Name)
    Gui.getMainWindow().close()
