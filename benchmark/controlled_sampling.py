# SPDX-License-Identifier: LGPL-2.1-or-later
"""Run with fork FreeCADCmd. Three-run extraction/inspection/transfer medians.

Field bounds do not certify geometric error or surface coverage. Peak RSS is
the whole Linux process's lifetime high-water mark, not per-case allocation.
"""
import gc
import resource
import statistics
import time
import FreeCAD as App
import Implicit

doc = App.newDocument("ControlledBenchmark")
try:
    field = doc.addObject("Implicit::Feature", "Field")
    print("case,field_units,tolerance,maximum_resolution,attempts,status,final_bound,triangles,median_ms,process_peak_mib", flush=True)
    cases = [("sphere", 0)] + [("gyroid_" + str(n), n) for n in (1, 3, 5, 10)]
    for name, cells in cases:
        if cells == 0:
            field.Function, field.Radius = "Sphere", 2
            field.DomainMin, field.DomainMax = (-3, -3, -3), (3, 3, 3)
        else:
            field.Function, field.Period, field.IsoValue = "Gyroid", 10, 0.17
            field.DomainMin, field.DomainMax = (0, 0, 0), (10 * cells,) * 3
        doc.recompute()
        times = []
        for repeat in range(3):
            gc.collect()
            start = time.perf_counter()
            result = Implicit.tessellateControlled(field, 0.5, 8, 32)
            times.append(1000 * (time.perf_counter() - start))
            summary = (len(result["history"]), result["status"],
                       result["quality"]["max_triangle_residual_bound"], len(result["triangles"]))
            del result
        attempts, status, bound, triangles = summary
        print("{},{},0.5,32,{},{},{:.6f},{},{:.3f},{:.2f}".format(
            name, "length" if cells == 0 else "dimensionless", attempts, status, bound,
            triangles, statistics.median(times), resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024), flush=True)
    print("controlled sampling benchmark completed", flush=True)
finally:
    App.closeDocument(doc.Name)
