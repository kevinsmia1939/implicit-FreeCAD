# Technical roadmap

## Capability status

| Capability | Status | Representation / accuracy |
|---|---|---|
| F and analytical gyroid gradient | Implemented | Double precision |
| Numerical-gradient fallback | Implemented | Centered finite difference |
| Normal | Implemented | Normalized gradient; undefined at critical points |
| Point classification | Implemented | Explicit negative/on-surface/positive result with caller tolerance |
| Surface projection | Standalone and native graph APIs implemented | Local constrained stationary solve; no global nearest-point certificate |
| Ray intersection | Standalone and native graph APIs implemented | Domain-clipped sign sampling and refined brackets; tangencies/thin features may be missed |
| Bounded surface visualization | Implemented | Marching-tetrahedra cache |
| FreeCAD editable feature | Implemented and runtime-tested with FreeCAD 1.1.4 | Python feature + retained Coin nodes |
| Native FreeCAD field graph | Implemented and command-line runtime-tested | C++ feature, primitives/Booleans/transforms, versioned JSON persistence |
| Native graph surface extraction | Implemented and command-line runtime-tested | Indexed uniform-grid tetrahedra, shared refined roots, gradient normals; no boundary caps |
| Native graph GUI view provider | Implemented and fork-GUI-tested | C++ retained scene nodes, placement, restore and workbench commands |
| Octree sampling | Implemented | Conservative empty-space pruning, common finest surface cells |
| Native batches | Implemented | Scalar batch API and cached compiled graph |
| Spatial controls and smooth unions | Implemented, tested | Constant/linear scalars, variable level-set bands, polynomial smooth min; no automatic dimensional checking |
| Live document composition | Implemented and native command-line/GUI tested | Source links, rigid placements, signs, band/offset and direct Part dispatch |
| Python extraction cancellation | Implemented and runtime-tested | Atomic one-shot token, GIL released during native extraction |
| Background viewport extraction | Implemented, opt-in and GUI-tested | One worker, request coalescing, cancellation and checked main-thread uploads |
| Viewport field probe | Implemented, GUI-tested | One-shot selected-field ray query; not integrated semantic selection |
| Sampled extraction diagnostics | Implemented | Vertex/centroid/edge residuals and edge-incidence checks; not a solid/error certificate |
| Field-bound refinement | Implemented | Doubled global grids until triangle-wide Lipschitz field bound meets target or explicit limit; coverage unverified |
| Plane intersection | Implemented | Marching-squares tracing with refined F=0 roots on a bounded plane patch |
| Cylinder intersection | Implemented in standalone engine | Refined chart roots and periodic seam joining; tangencies unsupported |
| Rectangular trimming topology | Planned | Domain clipping plus boundary-loop construction |
| Closed gyroid visualization | Implemented for explicit band/box graphs | Paired-edge regression; no general solid/topology guarantee |
| B-Rep-to-field | Snapshot and live bridge tested | OCCT core signed-distance extension; live Part source or JSON snapshot |
| Mixed field Boolean | Implemented | Sign composition and numerical extraction; no B-Rep result |
| OCCT-native field-backed face storage/queries | Experimental, isolated OCCT 8.1 tests | No UV surface; explicit unsupported-operation guards; no OCCT 8 handles inside the OCCT 7.9 GUI |
| OCCT-native single-face persistence | Experimental schema 1, isolated OCCT 8.1 tests | Sphere/gyroid, domain, tolerance, placements/orientation; no child topology, mesh, generic B-Rep/OCAF or FCStd support |
| Native sphere-plane trimming | Restricted analytic prototype, tested | Plane predicate + exact circle wire; open patch, no general field trimming |
| Native OCCT implicit-face Boolean | Restricted two-sphere analytic boundary prototype | Shared exact circle, classifications/orientation/source provenance; NOT generic BOP or validated solids |
| Native sphere geometric-bound meshing | Restricted analytic prototype, tested at 0.1 mm | Parameter-interpolation bound; no gyroid meshing, mesh welding or solid validity claim |
| Native gyroid-plane section | Implemented for bounded rectangles, derivative-bound resolved cases | Geometric cell bound, shared roots and explicit unresolved statuses; tangent/singular cases reject |
| Native gyroid Z-split seams | Restricted closed-loop prototype | Opposite authoritative plane predicates, shared approximate seam edges; no open-domain boundaries or conforming surface mesh |
| Native gyroid section GUI | Experimental, worker/GUI/FCStd tested | Native contour previews only, not a trimmed-surface Part object |
| Wholly OCCT-8 FreeCAD | Migration preflight implemented; dependency build underway | RapidJSON installed/enabled; required toolkit completion, passing preflight and separate FreeCAD API/runtime migration outstanding |
| OCCT native-boundary GUI examples | Experimental in-tree Python adapter, GUI/FCStd tested | Process-isolated OCCT 8 worker -> OCCT 7 GUI; editable recipes, transient Coin data, no Part handles |
| Local B-spline bridge | Experimental, tested | Native height charts; sampled field/normal validation; open face only |
| STEP export | Local fitted open-face round-trip tested | Whole implicit solid export remains unsupported |

## Next increments

The user selected **0.1 mm geometric tolerance** and explicitly requested native
OCCT faces. The [native representation/query foundation](occt-native-faces.md)
is now implemented and conformance-tested in rebuilt OCCT 8.1 libraries.
[Restricted sphere trimming, Boolean boundaries and GUI examples](native-sphere-operations.md)
are also implemented. Next native gates are general field-aware consumer/meshing
dispatch and open/domain-side trim topology. [Native gyroid sections and restricted
closed split seams](native-gyroid-sections.md) are now implemented; singular/tangent
resolution and trim-conforming surface meshes remain open. Other gates are
graph/topology persistence, general intersection topology, Boolean
splitting/history and a wholly OCCT-8-linked
FreeCAD build. These are not completed by attaching a field to a face. See the
[accuracy policy](accuracy-policy.md); field residuals are not millimetre errors.

Native ray queries and local stationary projection now query fields independently
of viewport meshes, including live mixed fields. A one-shot viewport probe and
sampled mesh-quality command are implemented. These are partial deliveries for
Stages 2–3: integrated semantic selection/snapping, richer editing, truly adaptive
error-controlled extraction and robust chart/intersection work remain open. See
[query contracts](native-projection-and-quality.md). Probes do not provide stable
subelements, and diagnostics do not certify OCCT topology or geometric accuracy.

The latest increments add the native view provider, workbench commands,
conservative octree sampling, batch queries, capped band graphs, cylinder
contours and an OCCT signed-distance core extension used for mixed field graphs.
See [`PROGRESS.md`](PROGRESS.md) for implementation details and verification scope,
and [`HANDOFF.md`](HANDOFF.md) for commands and environment.

The current increment replaces workbench snapshots with document-linked
operations, adds direct Part operands, GIL release/cancellation, and a native
viewport benchmark. These do not complete the manufacturing or OCCT-native face
stages. Remaining work is evaluated by the acceptance gates below.

The subsequent background increment adds opt-in asynchronous viewport extraction
with bounded concurrency, stale-result suppression and safe lifecycle handling.
Full document recompute, source validation and Coin uploads remain synchronous;
global memory budgets and field-aware picking are still open. See
[`background-meshing.md`](background-meshing.md).

Remaining stages:

The [local patch prototype](local-patch-bridge.md) now fits and validates sampled
B-spline faces, with open-face Part clipping and STEP round-trip tests. It does
not complete atlas construction, common boundaries, sewing, solid validation or
native OCCT implicit-face topology.

Global-grid [controlled extraction](controlled-extraction.md) now uses conservative
triangle-wide field bounds with explicit resolution-limit and empty-result
failure states. It is not the variable-resolution surface-cell milestone below.

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
measures the first two and estimated vector storage. The native GUI benchmark
adds redraw and parameter-update timings; actual hardware/presentation FPS and
systematic intersection-time sweeps remain open.

`benchmark/native_viewport.py` now provides native GUI measurements at six grid
cells per period, including completed Qt frame-swap throughput and
parameter-change latency. Its peak RSS includes the entire process. Software
Xvfb results must be distinguished from hardware presentation FPS; no universal
accuracy or speed claim follows from these samples.
`benchmark/native_viewport.csv` records the software-rendered baseline.
`./build/implicit_intersection_benchmark` measures local projection and sampled
ray/plane/cylinder operations; `benchmark/intersections.csv` records one run.

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

