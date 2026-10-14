# Implementation progress and remaining work

Updated 2026-10-15. The user has authorized continuing the full roadmap,
including FreeCAD/OCCT core modifications, commits and pushes. This log records
concrete increments; the overall nTop-class goal remains a substantial project.

Implementation heads: FreeCAD `7d6ec00e18` on
`feature/native-implicit`, OCCT `711d0d0275` on `feature/implicit-distance`.
The parent repository pins both commits.

## Current increment: live dependencies and extraction control

Native `Implicit::Feature` now has document schema 3 with `Operation`, `Left`,
`Right`, `Amount`, and `AutoDomain`. Definitions retain schema-1 JSON or editable
primitive properties. Linked union/intersection/difference, band and offset
nodes share immutable child fields through FreeCAD document dependencies.
Recompute invalidates downstream compiled/display caches after source edits;
missing or failed sources produce errors and clear stale display results.
FreeCAD skips downstream execution after an upstream failure; a post-recompute
dependency check explicitly invalidates those skipped features. Queries validate
the dependency DAG before returning cached geometry. `StatusMessage` exposes the
error; tests and commands use `isValid()` (FreeCAD's Python state label is
`Invalid`, not `Error`).
Queries guard against cycles and excessive native dependency depth. Undo and
FCStd restore retain the actual links rather than duplicated field snapshots.

Rigid source placements map into the result's parent frame, with transformed
gradients and sign normalization for positive-inside operands. The result's own
Placement remains an additional object transform. Automatic sampling domains
enclose transformed operand domains; `Implicit.bounds` reports effective bounds.
Manual domains remain available, particularly for expanded offsets. Domains
remain computational limits, not artificial cap surfaces.

With Part and the OCCT bridge enabled, linked operations accept single-solid
Part features directly. `Operation=BRep` supplies a reusable live B-Rep distance
field. Each recompute captures isolated BRep geometry and refreshes automatic
bounds. Source dimensions and placements propagate. The workbench offers live
B-Rep creation and linked Booleans; primitive commands no longer mask parameter
editing with JSON graphs. Earlier snapshots remain supported and do not
silently become live objects.

Python extraction and the scalar batch numeric loop release the GIL after
capturing immutable geometry. One-shot atomic cancellation tokens interrupt
sampling/root extraction, returning an explicit error instead of partial mesh
data. No background GUI scheduler, interruption inside a single OCCT query, or
cancellable Python-container conversion is claimed. FreeCAD document APIs must
still be called from the main thread.

The OCCT signed-distance evaluator now retains the fixed boundary operand's
distance-query maps/bounds between points. Additional native GoogleTests check
alternating interior/exterior/boundary queries and recovery after invalid input.
All **13 combined CTest tests pass** (13.41 seconds in the captured run),
including the fork GUI with direct Part selections, live B-Rep creation,
failed-source display clearing/recovery and the new linked/cancellation/Part
regressions. `Part`, `PartGui` and `PartScripts` are built. The separate ordinary
interactive demo and live Part/gyroid demo both pass automated GUI checks. The
live demo verifies that a Part dimension edit updates its linked result.
`PartScripts` is required alongside PartGui: it supplies AttachmentEditor and
other Python resources that are not copied by building PartGui alone.

`benchmark/native_viewport.py` adds the 1/3/5/10-cell GUI benchmark with six grid
cells per period. It separates scalar batch evaluation, extraction plus Python
transfer, combined parameter recompute/upload/render latency, completed Qt frame
swaps, packed mesh storage and entire-process peak RSS. It explicitly disables
Quarter's frame cap and waits for `frameSwapped`; counting deferred `redraw()`
calls produced invalid results and was corrected before recording this baseline.
Qt swaps/second are not monitor presentation/vsync FPS, and software-rendered
Xvfb observations are not GPU performance claims.

The recorded single-pass software-rendered run in `benchmark/native_viewport.csv`
used a 1024x456 viewport. At 1/3/5/10 cells it observed approximately
140/81/39/9 completed Qt swaps per second and 18/66/225/1448 ms for a parameter
change including recompute, upload and rendering. The largest case had 960,000
triangles, approximately 46 MB packed mesh storage, and 861 MiB entire-process
peak RSS. Compilation was stopped for measurement; unrelated desktop workloads,
software rendering and single-run variability still limit interpretation.

`implicit_intersection_benchmark` adds projection/ray/plane/cylinder timings for
the same four cell counts; the recorded run is `benchmark/intersections.csv`.
The radius-scaling sweep exposed a chart-tolerance issue: cylinder angles are
not model lengths. Chart stopping tolerance now accounts for the map's maximum
stretch. The radius-25 gyroid regression catches the former approximately 1e-8
residual; the corrected benchmark's sampled roots are below 1e-10. This is
vertex residual evidence, not a curve-interior error or completeness guarantee.

Reproduce the added measurements from the umbrella root:

```sh
cmake --build build --target implicit_intersection_benchmark --parallel 2
./build/implicit_intersection_benchmark
IMPLICIT_VIEWPORT_CSV=benchmark/native_viewport.csv python3 tools/with_xvfb.py \
  build-freecad-implicit/bin/FreeCAD --safe-mode benchmark/native_viewport.py
```

## Native display and document integration

`FreeCAD/src/Mod/Implicit/Gui` adds an in-tree C++ view provider for
`Implicit::Feature`, registered through `ImplicitGui` and `InitGui.py`.
It owns retained Coin coordinate, normal and indexed-face nodes; exposes
resolution, adaptive sampling, surface color and shaded/wire/flat-line modes;
and shrinks buffers
when resolution decreases. The GeoFeature view-provider base applies Placement.
Geometry revision notifications trigger cache uploads. Feature-local compiled
graphs and meshes are cached in the native document object. Field/domain edits
invalidate them, including edits during restoration; ordinary camera/placement
changes reuse local geometry. Invalid recomputes clear display geometry.

The earlier display increment provided a Native Implicit workbench with sphere, gyroid,
bounded band, field Boolean and B-Rep snapshot commands. Commands are undoable.
Those historical Boolean snapshots required matching Placement and negative-inside
operands. They copy definitions rather than maintaining live dependency links.
B-Rep snapshot results retain the source label for provenance, but source edits
do not update them. Native GUI tests exercise cache reuse, buffer shrink,
definition changes, bad-graph recovery, placement and FCStd restoration.

## Conservative octree sampling

`Implicit.tessellateAdaptive(feature, resolution=32)` subdivides an octree and
rejects blocks only when a global Lipschitz bound and a center sample rule out
a zero crossing. Sphere and box bounds are one; gyroid uses
`2*sqrt(3)*(2*pi/period)`. Min/max Booleans inherit the larger operand bound;
translation, offset and absolute-value bands preserve it. A field without a
finite proven bound disables pruning. B-Rep snapshots currently use that
fallback.

All surviving surface cells reach the same finest grid size and use the same
tetrahedron decomposition and global edge keys. Thus there are no mixed-level
transition faces. This is adaptive empty-space sampling, not a variable-density
surface mesh. The grid cache remains densely allocated. A small floating-point
margin guards ordinary rounding; no interval-arithmetic proof is claimed.
Regressions compare exact indexed geometry after reordering with the uniform
reference and check closed sphere connectivity and reduced sample counts.

## Bands, caps and mesh winding

Native graph nodes `band` and `box` implement `abs(F)-halfWidth` and an exact
axis-aligned box signed distance. Intersecting a band with a box supplies
mathematical cap surfaces. Keep the computational domain larger than the box
so the zero surface is sampled on both sides. Computational-domain clipping
alone still produces no caps.

The bounded gyroid regression checks paired, oppositely directed triangle edges.
It exposed a winding defect in the previous gradient-based orientation: hard
Boolean branches can change analytic normals across a triangle. Winding now
follows the linear tetrahedron sign configuration; gradients remain shading
normals. Crease normals may oppose a coarse triangle and are not a substitute
for topological orientation. Closed visualization meshes are not OCCT solids.

Band widths use the input field's units. A gyroid band width is dimensionless;
a true signed-distance field's band width is a model length. Combining fields
of different units preserves sign-set operations but does not make the combined
value a signed distance. General unit validation and physical wall-thickness
controls remain to be implemented.

## OCCT core signed-distance extension and mixed fields

The OCCT branch `feature/implicit-distance` adds
`BRepClass3d_SignedDistance` to the TKTopAlgo package manifest, implementation,
public header and native GoogleTest manifest. It accepts one finite, valid,
outward-oriented solid. It builds a boundary-face compound for nearest distance
and uses solid classification for the sign; cavity points are outside material.
Measuring against faces avoids the solid-to-interior-point zero-distance result.
The tolerance creates an explicit on-boundary zero band. Invalid inputs and
unresolved classification/distance fail explicitly. Instances are noncopyable
and not thread safe; input geometry must remain immutable.

The extension implementation is built against installed OCCT 7.9.3 for current
compatibility testing. Only its new header is staged into an isolated include
directory, preventing accidental use of the fork's OCCT 8.1 headers with 7.9
binaries. A complete OCCT 8.1 toolkit rebuild and its GoogleTest suite have not
been run. The newly added OCCT GoogleTest source is also compiled and executed
against the compatibility build when GTest is available. No system libraries
are replaced.

The new extension translation unit is also compiled directly by the OCCT 8.1
source build, without building all toolkit dependencies. This caught removal
of the legacy `Raise()` exception API; explicit throws now compile under 8.1
and pass the 7.9 runtime tests. The isolated configuration is in ignored
`build-occt-implicit`.

FreeCAD's optional `IMPLICIT_OCCT_SOURCE_DIR` compiles this extension into the
native Implicit module. The `brep` graph node persists an ASCII BRep snapshot,
tolerance and field definition within the graph's existing size limits.
`BRepField` locks the mutable classifier and computes finite-difference
gradients. Union/intersection/difference then work in field space. The mixed
smoke test checks signs, gradients, numerical extraction and FCStd reload.
Results remain implicit fields and transient boundary meshes; existing
`BRepAlgoAPI_*` operations do not accept arbitrary implicit faces.

## Cylinder contours

The standalone `CylinderIntersection` maps `(angle,height)` into a full periodic
cylinder and reuses plane-chart contour tracing. Root refinement stays in the
cylinder chart, so output points lie on the cylinder rather than its polygonal
chords. Endpoint stitching joins the periodic seam. Tests cover two analytical
sphere/cylinder circles, a rotated axis and out-of-domain rejection. The entire
cylinder must fit in the field domain. Tangencies, coincident regions and
singular graph intersections are not guaranteed; curves are sampled polylines.

## Still planned

The following are not completed by these increments:

- native complex-geometry property, shared-node JSON DAG format and graph editor
  (document-level source-link DAGs are now implemented);
- field-aware semantic picking and broader container/App::Link integration tests;
- asynchronous GUI scheduling, SIMD/GPU and cache budgets;
- variable-resolution surface cells, transition meshing, rigorous approximation
  error control, broader topology and singularity tests;
- general plane-box polygon clipping, trimming topology and validated distance
  offsets for uniform physical thickness;
- fitted B-spline atlases with common boundaries/p-curves, sewing, healing,
  geometric error measurement, validated manufacturing solids and STEP export;
- native OCCT implicit face storage, adaptor/intersection/Boolean dispatch,
  persistence and exchange design;
- cross-workbench FEM/CAM consumers and systematic viewport/FPS benchmarks.

The roadmap's full completion should be evaluated against these capabilities,
not merely the presence of code in the two source trees.

## Verified results

Before the current live increment, all **10 CTest tests passed** in one combined run (5.50 seconds),
including the fork's native GUI on an isolated Xvfb software-rendered display.
The GUI test uses FreeCAD safe mode to isolate user-installed addons. It checks
the workbench command/undo, retained nodes, buffer shrink, adaptive-mode changes,
cache reuse during camera/placement changes, placed scene bounds, invalid-graph
recovery and save/reload of definition and view properties.

The OCCT extension object compiled successfully against the OCCT 8.1 source
headers. Its standalone and GoogleTest runtime checks passed against 7.9.3.
A complete OCCT toolkit rebuild and full toolkit regression run remain open.

`benchmark/native_sampling.py` and its recorded CSV compare uniform and
octree-pruned sampling, including Python result conversion. In the captured
single run, the sparse sphere at resolution 64 used 3,187 instead of 274,625 grid
samples and took 7.677 instead of 20.342 ms. Dense gyroids saw little/no pruning
and extra octree overhead. This is evidence for selective use, not a general
speedup claim. The gyroid sweep fixes resolution at 32, so increasing unit-cell
count reduces samples per period and does not preserve geometric accuracy.
Packed mesh bytes exclude working buffers, root maps, allocator overhead and
Python copies; that sampling script does not measure rendering. The new GUI
benchmark and its completed-frame measurements are described above.

Reproduce the full test run after building the native targets:

```sh
cmake -S . -B build -DIMPLICIT_BUILD_OCCT_DISTANCE=ON \
  -DIMPLICIT_NATIVE_FREECADCMD="$PWD/build-freecad-implicit/bin/FreeCADCmd" \
  -DIMPLICIT_NATIVE_FREECADGUI="$PWD/build-freecad-implicit/bin/FreeCAD" \
  -DIMPLICIT_NATIVE_PART_TESTS=ON
cmake --build build --parallel 2
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
./build-freecad-implicit/bin/FreeCADCmd benchmark/native_sampling.py
```

The earlier reduced build omitted Part/PartGui. This increment explicitly builds
`Part`, `PartGui` and `PartScripts` for interactive conventional-solid workflows.
The original `mixed_smoke.py` still uses a BRep fixture without Part's Python
module; `part_linked_smoke.py` and the optional mixed GUI checks test live Part
dependencies. Build only the relevant targets with two compile jobs; do not
mistake a configured workbench for a built one.
