# Native gyroid sections and restricted split seams

This implements the next native OCCT increment, not completion of all four
requested stages. It is independent of the older standalone marching-squares
prototype and runs against the rebuilt OCCT 8.1 kernel.

| Requested stage | Current result | Remaining acceptance gate |
|---|---|---|
| 1: gyroid–plane intersections | Native bounded-rectangle sections with derivative-bound acceptance, explicit unresolved diagnostics and GUI preview | General domain polygons, tangent/singular intersection resolution rather than rejection |
| 2: trimming and geometric-bound meshing | Restricted identity-placed Z-plane split with closed, resolved loops and shared tolerance-bearing seam edges | Open/domain-side boundaries, arbitrary/nested trims, surface meshing conforming to shared seams |
| 3: native mixed Booleans | Split/predicate groundwork only; earlier sphere-only operations and FreeCAD field-space Booleans remain separate | Gyroid/B-Rep intersections, region classification, splitting, result/history and solid validation |
| 4: wholly OCCT-8 FreeCAD | Strict source-build selection/preflight added; required OCCT modules configured except GLTF | Missing RapidJSON headers, remaining toolkit builds, FreeCAD API migration/build/runtime tests, native persistence/consumer dispatch |

## Native section contract

`BRep_ImplicitPlaneSection::Trace(face, settings)` operates on a childless,
untrimmed native face containing an **exact `Geom_ImplicitGyroid`**. A finite
rectangle in an orthonormal world plane frame must lie entirely inside the
field's computational domain. Arbitrary plane orientation and combined
geometry/instance placements are supported. Mathematical fields remain
authoritative; output polylines are explicitly approximate intersection data,
not an invented `Geom_Surface` or exact `Geom_Curve`.

The tracer refines uniform shared grids from 4 up to at most 512 cells per axis
(power-of-two budgets). Root vertices are shared by grid-edge identity, not
distance welding. It does not decide saddle connectivity using a centre sign.
For wave number `k` and uniform placement scale magnitude `s`, it uses:

- A global field Lipschitz bound `2*sqrt(3)*k/s`.
- A Hessian operator bound `4*(k/s)^2`: diagonal entries are bounded by
  `2*k^2`, off-diagonal entries by `k^2`, before placement.
- Centre-value bounds to exclude empty cells; otherwise a partial derivative
  must retain a nonzero sign throughout the cell.
- Unique monotone edge roots, or derivative/Taylor bounds excluding edge roots.
  Two unique crossings plus the interior monotonicity condition establish a
  single regular arc. No boundary roots plus interior monotonicity exclude an
  internal loop. A cell failing these tests is **unresolved**, not silently empty.

An accepted segment and its true arc lie in the same cell. Consequently the
cell diagonal bounds their bidirectional geometric deviation, conservatively;
the numerical coordinate reserve is included in the report. Accepted cells
must meet `DistanceTolerance` in model-length units. Root sign brackets refine
to at most `DistanceTolerance/16`. The residual is reported separately in
dimensionless gyroid field units and is never called a millimetre error.

`Resolved` requires every cell to pass and the assembled graph to have no
branches/interior-ended fragments. Results distinguish verified empty sections
from unresolved ones and include resolution, unresolved/ambiguous/potential-
critical counts, graph defects, geometric bound, root bracket and residual.
Partial curves can exist in an unresolved result; **do not use them as trim
topology or treat their reported bound as whole-section coverage**.

Grid-aligned/near-zero vertices, tangent edges, critical points, ambiguous
crossings and exhausted resolution may return unresolved. This is intentional
rejection, not a claim of solving singular or tangent intersections. Numerical
reserves and a local trigonometric phase limit of `1e6` control the supported
coordinate regime; this is ordinary floating-point bound checking, not a
directed-rounding interval-arithmetic proof for every library operation.

## Restricted Z-plane split

`BRep_ImplicitGyroidOps::SplitZ(face, z, tolerance, maximumCells)` requires an
identity-placed, forward native gyroid and an interior Z plane. It traces the
full XY computational rectangle and rejects **all unresolved or open sections**.
The two returned `BRep_TImplicitPlanePatch` faces retain the same field/domain
with opposite immutable half-space predicates. Their closed seams share the
same degree-1 B-spline edge/wire geometry with opposite effective orientations.
Those edges are bounded polyline approximations, not exact gyroid intersections.

The field plus half-space predicate, not the polygon, defines retained geometry.
Explicit domain/ray queries honor the plane; existing sampled-ray completeness
and near-trim-boundary limitations still apply. Placement uses the face
instance; modifying inherited geometry location is rejected. `EmptyCopy`
preserves the predicate; schema-1 primitive persistence rejects these subtypes
even without their children, preventing trim loss.

There are no p-curves, planar caps, domain-side wires, trim-conforming surface
meshes, nested trims or manufacturing-solid guarantees. These are not generic
`BRepCheck`/BOP/BRepMesh faces or Part solids. Empty sections return two masked
field patches without asserting that both contain a nonempty surface. This
constructor is a closed-seam topology experiment, not a Boolean solid builder.

## GUI and protocol

The existing isolated OCCT 8 worker has a new `section` mode. The in-tree
`ImplicitSection.py` adapter creates editable `App::FeaturePython` objects with
period, level, plane normal/offset, span, geometric tolerance and resolution
budget. Successful sections display retained Coin line buffers; unresolved or
failed results clear the display and report errors. FCStd stores mathematical
recipe properties and reconstructs sections on restore, not mesh/native shape
data. Executable selection remains trusted environment/user configuration,
never a path from the document. Calls currently block recompute, with a timeout.

The GUI still uses OCCT 7.9.3. No OCCT 8 handles/libraries are loaded into it and
no native faces are assigned to `Part::Feature::Shape`. This preview displays
**intersection curves**, not a meshed trimmed gyroid face. The restricted native
`SplitZ` API is tested in OCCT but is not yet exposed as a GUI surface object.

Build using the existing trees:

```sh
cmake -S OCCT -B build-occt-implicit \
  -DCMAKE_JOB_POOLS=occt_link=1 -DCMAKE_JOB_POOL_LINK=occt_link
cmake --build build-occt-implicit --target TKBRep --parallel 2
cmake --build build-occt-implicit/native-face-tests --parallel 2
cmake --build build-freecad-implicit --target ImplicitScripts --parallel 2
cmake -S . -B build -DIMPLICIT_OCCT_NATIVE_BUILD="$PWD/build-occt-implicit"
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

In **Native Implicit**, choose **OCCT gyroid-plane section (experimental)**.
Default period 10, level 0.21, plane Z=0.37, span 8 and target 0.1 mm produce a
resolved preview. In the development layout the worker is located automatically;
other layouts use `IMPLICIT_OCCT_NATIVE_WORKER` as documented for sphere examples.
FreeCAD Python console:

```python
import ImplicitSection
section = ImplicitSection.create()
section.GeometricTolerance = 0.05
App.ActiveDocument.recompute()
```

Native tests include independent analytic slice points (using the exact
fixed-Z trigonometric equation) to check geometric coverage against polylines,
not just residuals. Other checks cover placed oblique frames, empty versus
unresolved results, deterministic connectivity, saddle rejection, shared closed
seams, opposite predicates, source immutability and persistence rejection.
Protocol/GUI tests cover recipe edits, unresolved-buffer cleanup, finer targets
and FCStd reconstruction. There are now 24 isolated native GTest cases and 24
combined host CTest entries, including the strict migration preflight. The final
combined run passed all 24 entries in 25.72 seconds, after the six-library
rebuild triggered by the additional module configuration; incremental builds
were confirmed up to date before that run.

## OCCT 8 migration: dependency gate

FreeCAD's new `FREECAD_OCCT_BUILD_DIR` opt-in imports the source build's exported
targets directly, rather than treating an install-tree Config file as a build
export. It requires every toolkit in FreeCAD's OCC/OCAF lists to be configured
and built and forbids falling back to installed OCCT libraries. Default installed
OCCT discovery is unchanged. Preflight is tested with valid/incomplete and
invalid paths; this is not a completed FreeCAD API/runtime migration.

On this host, ApplicationFramework, DataExchange and Visualization now configure
using existing dependencies, including FreeType 2.14.3. **Only the original six
OCCT libraries have been built.** The current preflight reports 27 required
toolkits configured but unbuilt, and `TKDEGLTF` not configured because RapidJSON
headers are absent. `rpm` and the local openSUSE repository query confirm that
`rapidjson-devel` is not installed and is available from repo-oss. User action:

```sh
sudo zypper install rapidjson-devel
```

No package was installed automatically and no system libraries were replaced.
After the user confirms installation, configure `USE_RAPIDJSON=ON` alongside the
three modules, build the required toolkit targets with two compile jobs and one
link job, then rerun:

```sh
cmake -S tests/occt_native_faces/preflight \
  -B build-occt-implicit/freecad-preflight \
  -DIMPLICIT_FREECAD_SOURCE="$PWD/FreeCAD" \
  -DFREECAD_OCCT_BUILD_DIR="$PWD/build-occt-implicit"
```

Only after that passes should a **separate FreeCAD migration build** be
configured with this flag and the documented minimal dependency/feature options.
It is necessary to preserve the working OCCT 7 binary while recompiling every
OCCT-dependent consumer; do not retarget the existing working FreeCAD tree
piecemeal or copy OCCT 8 general headers into its OCCT 7 compatibility include
directory. API ports, `ldd`/runtime audits, native persistence and consumer
dispatch tests still follow. A successful dependency preflight alone does not
authorize arbitrary native faces in Part or establish manufacturing readiness.
