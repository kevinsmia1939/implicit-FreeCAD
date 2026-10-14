# SPDX-License-Identifier: LGPL-2.1-or-later
"""Run with the fork's FreeCADCmd; reports extraction plus Python transfer time.

Grid resolution is fixed for the gyroid sweep, so this is a workload comparison,
not a claim of equal geometric accuracy across cell counts. Packed bytes exclude
grid/root-map storage, reference vertices, allocator overhead and Python copies.
"""
import time
import FreeCAD
import Implicit

document = FreeCAD.newDocument("NativeSamplingBenchmark")
try:
    feature = document.addObject("Implicit::Feature", "Field")
    feature.Function = "Sphere"
    feature.Radius = 1
    feature.DomainMin = (-5, -5, -5)
    feature.DomainMax = (5, 5, 5)
    print("case,resolution,mode,elapsed_ms,grid_samples,pruned_blocks,vertices,triangles,packed_bytes")

    def measure(name, resolution):
        for mode, extractor in (("uniform", Implicit.tessellate),
                                ("octree", Implicit.tessellateAdaptive)):
            start = time.perf_counter()
            mesh = extractor(feature, resolution)
            elapsed = 1000 * (time.perf_counter() - start)
            vertices, triangles = len(mesh["vertices"]), len(mesh["triangles"])
            print("{},{},{},{:.3f},{},{},{},{},{}".format(name, resolution, mode, elapsed,
                mesh["sample_count"], mesh["pruned_blocks"], vertices, triangles,
                48 * vertices + 24 * triangles), flush=True)

    for resolution in (16, 32, 64):
        measure("sphere", resolution)
    feature.Function = "Gyroid"
    feature.Period = 10
    feature.IsoValue = 0.17
    feature.DomainMin = (0, 0, 0)
    for cells in (1, 3, 5, 10):
        extent = cells * 10
        feature.DomainMax = (extent, extent, extent)
        measure("gyroid-{}cells".format(cells), 32)
finally:
    FreeCAD.closeDocument(document.Name)
