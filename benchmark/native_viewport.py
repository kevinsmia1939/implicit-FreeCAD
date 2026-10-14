# SPDX-License-Identifier: LGPL-2.1-or-later
"""Run with the fork GUI (safe mode); output path: IMPLICIT_VIEWPORT_CSV.

Measures completed Qt framebuffer swaps, not monitor presentation/vsync FPS. Resolution
is six grid cells per gyroid period for each size; no geometric error bound is
implied. RSS is the entire process peak, not isolated mesh allocation.
"""
import csv
import gc
import os
import platform
import resource
import sys
import time
import traceback
import FreeCAD as App
import FreeCADGui as Gui
import Implicit
import ImplicitGui  # noqa: F401
from PySide6 import QtCore, QtWidgets, QtOpenGLWidgets


def milliseconds(call):
    start = time.perf_counter()
    result = call()
    return (time.perf_counter() - start) * 1000, result


def main():
    Gui.getMainWindow().resize(1024, 768)
    Gui.getMainWindow().show()
    doc = App.newDocument("NativeViewportBenchmark")
    field = doc.addObject("Implicit::Feature", "Gyroid")
    field.Function = "Gyroid"
    field.Period = 10
    field.DomainMin = (0, 0, 0)
    field.ViewObject.AdaptiveSampling = False
    view = Gui.activeDocument().activeView()
    Gui.updateGui()
    widgets = [widget for widget in Gui.getMainWindow().findChildren(QtOpenGLWidgets.QOpenGLWidget)
               if widget.isVisible()]
    assert widgets, "no visible OpenGL viewport"
    viewport = max(widgets, key=lambda widget: widget.width() * widget.height())
    quarter = next(widget for widget in Gui.getMainWindow().findChildren(QtWidgets.QWidget)
                   if widget.property("maxFrameRate") is not None and widget.isVisible())
    # The default follows display refresh rate and may defer redraw requests.
    previous_limit = quarter.property("maxFrameRate")
    quarter.setProperty("maxFrameRate", 0)
    rendered = [0]
    loop = QtCore.QEventLoop()
    timer = QtCore.QTimer()
    timer.setSingleShot(True)
    timer.timeout.connect(loop.quit)
    def swapped():
        rendered[0] += 1
        loop.quit()
    viewport.frameSwapped.connect(swapped)
    def render_frame(rotation):
        before = rendered[0]
        view.setCameraOrientation(rotation)
        view.redraw()
        timer.start(10000)
        if rendered[0] == before:
            loop.exec()
        timer.stop()
        assert rendered[0] > before, "viewport did not complete a frame within 10 seconds"
    rows = []
    for cells in (1, 3, 5, 10):
        resolution = 6 * cells
        extent = 10 * cells
        field.ViewObject.Resolution = resolution
        field.DomainMax = (extent, extent, extent)
        field.IsoValue = 0.17
        doc.recompute()
        view.viewAxonometric()
        view.fitAll()
        Gui.updateGui()
        points = [(extent*x/15, extent*y/15, extent*z/15)
                  for z in range(16) for y in range(16) for x in range(16)]
        evaluation_ms, _ = milliseconds(lambda: Implicit.evaluateBatch(field, points))
        extraction_ms, mesh = milliseconds(lambda: Implicit.tessellate(field, resolution))
        vertices, triangles = len(mesh["vertices"]), len(mesh["triangles"])
        del mesh
        gc.collect()
        def change():
            field.IsoValue = 0.18
            doc.recompute()
            Gui.updateGui()
            render_frame(view.getCameraOrientation())
        recompute_ms, _ = milliseconds(change)
        builds = field.ViewObject.CacheBuildCount
        frames = 12
        render_frame(view.getCameraOrientation())  # warm up rendering before timing
        first_frame = rendered[0]
        start = time.perf_counter()
        for frame in range(frames):
            rotation = App.Rotation(App.Vector(0, 0, 1), frame * 3)
            render_frame(rotation)
        elapsed = time.perf_counter() - start
        assert field.ViewObject.CacheBuildCount == builds, "camera regenerated geometry"
        rows.append(dict(cells=cells, resolution=resolution, evaluation_points=len(points),
                         evaluation_ms=round(evaluation_ms, 3),
                         extraction_and_transfer_ms=round(extraction_ms, 3),
                         parameter_recompute_upload_redraw_ms=round(recompute_ms, 3),
                         qt_frame_swaps=rendered[0]-first_frame,
                         qt_frame_swaps_per_second=round((rendered[0]-first_frame)/elapsed, 3),
                         viewport_width=viewport.width(), viewport_height=viewport.height(),
                         vertices=vertices, triangles=triangles,
                         packed_mesh_bytes=48*vertices + 24*triangles,
                         process_peak_rss_kib=resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,
                         platform=platform.system(),
                         software_gl=os.environ.get("LIBGL_ALWAYS_SOFTWARE", "unspecified")))
        print("viewport case completed:", cells, "cells", flush=True)
    output = os.environ.get("IMPLICIT_VIEWPORT_CSV")
    stream = open(output, "w", newline="") if output else sys.stdout
    try:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    finally:
        if output:
            stream.close()
    quarter.setProperty("maxFrameRate", previous_limit)
    App.closeDocument(doc.Name)
    print("native viewport benchmark passed", flush=True)
    Gui.getMainWindow().close()


try:
    main()
except BaseException:
    traceback.print_exc()
    os._exit(1)
