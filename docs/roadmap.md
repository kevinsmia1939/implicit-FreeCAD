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
| Native FreeCAD field graph | Implemented and command-line runtime-tested | C++ feature, primitives/Booleans/transforms, versioned JSON persistence |
| Native graph surface extraction | Implemented and command-line runtime-tested | Indexed uniform-grid tetrahedra, shared refined roots, gradient normals; no boundary caps |
| Native graph GUI view provider | Implemented and fork-GUI-tested | C++ retained scene nodes, placement, restore and workbench commands |
| Octree sampling | Implemented | Conservative empty-space pruning, common finest surface cells |
| Native batches | Implemented | Scalar batch API and cached compiled graph |
| Plane intersection | Implemented | Marching-squares tracing with refined F=0 roots on a bounded plane patch |
| Cylinder intersection | Implemented in standalone engine | Refined chart roots and periodic seam joining; tangencies unsupported |
| Rectangular trimming topology | Planned | Domain clipping plus boundary-loop construction |
| Closed gyroid visualization | Implemented for explicit band/box graphs | Paired-edge regression; no general solid/topology guarantee |
| B-Rep-to-field | Implemented | OCCT core signed-distance extension; snapshot persistence |
| Mixed field Boolean | Implemented | Sign composition and numerical extraction; no B-Rep result |
| Native OCCT implicit-face Boolean | Unsupported | Requires geometry/topology/algorithm dispatch work |
| STEP export | Unsupported | Only after valid supported B-Rep approximation |

## Next increments

The latest increments add the native view provider, workbench commands,
conservative octree sampling, batch queries, capped band graphs, cylinder
contours and an OCCT signed-distance core extension used for mixed field graphs.
See [`PROGRESS.md`](PROGRESS.md) for implementation details and verification scope,
and [`HANDOFF.md`](HANDOFF.md) for commands and environment.

Remaining stages:

1. Extend the conservative octree sampler to variable-resolution surface cells,
   shared edge vertices, transition handling,
   deterministic topology, boundary clipping, and error estimates. Preserve the
   current standalone extractor and indexed native extractor as regression
   references. Avoid pruning merely on same-sign corners without field bounds.
2. Generalize plane-patch contours to arbitrary clipped plane/box polygons and
   strengthen cylinder tracing for tangencies and singular cases.
3. Extend the implemented band/box classifiers for `abs(F)<=t` and `F<=c0`. Treat `t` as a
   dimensionless level-set band, never as physical thickness. Create capped,
   broader validated visualization solids inside finite domains.
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

The current native comparison is in `benchmark/native_sampling.csv`; run it
with the fork's `FreeCADCmd benchmark/native_sampling.py`. It measures extraction
plus Python transfer, not rendering. Conservative pruning helps the sparse
sphere but adds overhead to dense gyroids; the fixed-grid gyroid sweep is not
an equal-accuracy comparison. Full results and verification scope are recorded
in [`PROGRESS.md`](PROGRESS.md).

