# Technical roadmap

## Capability status

| Capability | Status | Representation / accuracy |
|---|---|---|
| F and analytical gyroid gradient | Implemented | Double precision |
| Numerical-gradient fallback | Implemented | Centered finite difference |
| Normal | Implemented | Normalized gradient; undefined at critical points |
| Point classification | Implemented | Explicit negative/on-surface/positive result with caller tolerance |
| Closest-point projection | Implemented | Local constrained Newton solve |
| Ray intersection | Implemented | Sampled bracketing plus safeguarded refinement |
| Bounded surface visualization | Implemented | Marching-tetrahedra cache |
| FreeCAD editable feature | Implemented and runtime-tested with FreeCAD 1.1.4 | Python feature + retained Coin nodes |
| Plane intersection | Implemented | Marching-squares tracing with refined F=0 roots on a bounded plane patch |
| Cylinder intersection | Planned | Numerical curve tracing in a cylindrical chart |
| Rectangular trimming topology | Planned | Domain clipping plus boundary-loop construction |
| Closed gyroid solid | Planned | Implicit classification and capped extraction first |
| OCCT Boolean | Unsupported | Later B-spline B-Rep bridge |
| STEP export | Unsupported | Only after valid supported B-Rep approximation |

## Next increments

1. Add an adaptive octree sampler with shared edge vertices, ambiguity handling,
   deterministic topology, boundary clipping, and error estimates. Preserve the
   current simple extractor as a correctness baseline.
2. Generalize the implemented plane-patch contour tracer to arbitrary clipped
   plane/box polygons and cylinders in their native `(angle,height)` chart.
3. Add band and domain classifiers for `abs(F)<=t` and `F<=c0`. Treat `t` as a
   dimensionless level-set band, never as physical thickness. Create capped,
   validated visualization solids inside finite domains.
4. Add approximate signed-distance evaluation (`F/|grad F|` as a local seed,
   followed by projection) and quantify its failure near critical points before
   exposing physical thickness.
5. Prototype a local surface atlas: seed charts from the implicit normal,
   project chart samples onto F=0, fit bounded B-spline patches, measure Hausdorff
   and normal error, generate common boundary curves/p-curves, sew, and validate
   with OCCT shape checks.
6. Use only the validated patch B-Rep for `BRepAlgoAPI_Common/Cut`; retain links
   to the authoritative implicit definition and rebuild the bridge when it
   changes. Record fitting and Boolean tolerances separately.
7. Explore an OCCT-native proposal only after the external algorithms stabilize.
   It would need new geometry/adaptor contracts, intersection dispatch, face
   representation and persistence rules, meshing hooks, classifiers, BOP
   integration, and a migration story for p-curves—not merely a class derived
   from `Geom_Surface`.

## Performance plan

Track equation evaluation, sampling/extraction, cache upload, recompute latency,
and viewport FPS separately for 1, 3, 5, and 10 cells. The current executable
measures the first two and estimated vector storage. Add Coin timing inside a
real FreeCAD runtime before reporting rendering performance.

Baseline on the development host with six grid cells per unit cell:

| Unit cells | Grid | Evaluations | Evaluation ms | Mesh ms | Triangles | Estimated mesh bytes |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 6³ | 343 | 0.029 | 0.401 | 936 | 157,248 |
| 3 | 18³ | 6,859 | 0.314 | 10.335 | 25,740 | 4,324,320 |
| 5 | 30³ | 29,791 | 1.328 | 47.558 | 119,520 | 20,079,360 |
| 10 | 60³ | 226,981 | 9.982 | 389.145 | 958,620 | 161,048,160 |

These are single-run engineering baselines, not statistically rigorous results.
They exclude allocator overhead, FreeCAD/Coin upload, and viewport rendering.

