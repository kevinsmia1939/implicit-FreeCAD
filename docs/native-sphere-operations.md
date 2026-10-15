# Restricted native trimming, Boolean boundaries and GUI exposure

This increment is an **analytic sphere-only vertical slice**, not completion of
general implicit trimming/Booleans. It runs in the rebuilt OCCT 8.1 kernel.
Gyroid trimming, arbitrary intersections, nested operations, mixed Part-solid
Booleans, generic BOP history/validation and in-process OCCT-8 FreeCAD exposure
remain open. The existing FreeCAD field-space Booleans are a separate feature.

## Native contracts

`BRep_ImplicitSphereOps::Trim(face, plane)` keeps the negative signed-distance
side of a world-coordinate plane on a native sphere face. It constructs a
`BRep_TImplicitSpherePatch`: the sphere remains authoritative, with an immutable
local plane predicate and a 3D circle edge/wire. No triangles or fake UV surface
define the geometry. The cut is an **open surface patch**, without a planar cap.

`Boolean(left, right, Operation::{Common,Fuse,Cut})` computes an analytic
two-sphere boundary decomposition. Transverse intersections produce two native
patches sharing the exact same circle edge, with opposite effective uses.
Cut reverses the inner patch normal. Separated and strictly contained cases
produce the appropriate empty/full sphere boundaries; a cavity has a reversed
inner face. `Sources` records which input each face came from (0/1); this is
limited source provenance, not the complete BOP modified/generated history API.

Inputs must be childless, forward, exact native sphere fields with computational
domains enclosing their entire spheres. Combined geometry/instance placements,
including uniform scaling/reflection, are canonicalized into world-coordinate
output sphere parameters. The inputs are not mutated. Trim and Boolean results
cannot be fed into another operation yet. Unknown/derived fields, tangencies,
coincidences, insufficient domains and separation/containment boundaries within
the maximum operand tolerance are rejected explicitly. Additional near-pole or
small-circle cases also reject when unresolved at that tolerance.

The Boolean return is a **compound of boundary faces, not a validated
`TopoDS_Solid`**. Wires have 3D circle geometry but no UV p-curves. The explicit
implicit consumers understand the plane membership contract; legacy
`BRepCheck`, meshing and BOP algorithms do not. Do not call generic CAD operations
on these results or describe them as manufacturing-ready solids. Shared edge
identity plus a trim predicate is an initial restricted topology contract, not a
general replacement for OCCT's p-curves, face splitting and pave filling.

`BRep_ImplicitTool::IsInDomain` and its sampled ray hit filter now honor patch
planes as well as the computational box. Value/gradient remain queries of the
underlying field, including outside the trim. Ray completeness and near-boundary
classification are still limited by sampling/refinement: a midpoint may fall on
the rejected side of a trim even when its root bracket touches the boundary.
Bounds remain conservative whole-domain bounds, not tight patch bounds.
Placement of patches must use the face-instance location; mutation of their
inherited geometry location is rejected by explicit query/meshing consumers
because it would separate the predicate from the wire. Low-level arbitrary
topology edits are unsupported.

`EmptyCopy` retains the plane and field but omits children/cache. The schema-1
single-face writer now rejects subtype storage even if its wires were removed,
so a trim cannot silently disappear. General native topology persistence remains
unsupported. The GUI persists a reconstruction recipe instead (see below).

## Sphere-specific geometric display bound

`Tessellate(face, deflection)` creates a disposable `Poly_Triangulation` in
field-local coordinates. It supports only full native spheres and the analytic
patch subtype, not gyroids or generic `BRepMesh`. The requested deflection and
returned bound are in placed/model length units; FreeCAD uses millimetres.

Latitude rings include the exact trim circle latitude; longitude wraps use
shared nodes, poles use fans, and reversed faces reverse triangle winding.
Vertices lie on the mathematical sphere up to floating-point evaluation error.
The mesh does not cap the patch, and meshes of separate result faces are not
welded across their shared analytic edge (different radii may use different
sampling). It is not a watertight Boolean result mesh guarantee.

For the spherical map, every second partial derivative has norm at most radius
`r`. Barycentric interpolation on each angular cell has a distance remainder
bounded by `r/2 * (deltaTheta + deltaPhi)^2`. Corresponding parameter points give
a bidirectional geometric interpolation bound, including the trim boundary;
this is not a dimensionless residual or a gyroid distance estimate. Face scale
multiplies the bound. A floating-point coordinate precision reserve is deducted
from the requested budget and included in the report; unresolved coordinates or
requests over 120,000 vertices / 240,000 triangles per face reject. The analytic
bound and numerical reserve are not an interval-arithmetic certificate for all
rounded floating-point operations or a CAD topology/solid certificate.

The test fixtures target **0.1 mm**. Tests check exact circle incidence on both
unequal-radius operands, predicate classification, effective edge orientation,
source mapping, transforms, reversed normals, sampled triangle interiors,
edge incidence/winding, pole handling and precision/resource rejection.

## FreeCAD GUI bridge: process isolation, not ABI mixing

The preserved fork GUI/Part uses installed OCCT 7.9.3; a separate OCCT 8.1-linked
GUI now also passes targeted migration tests (see
[migration validation](native-gyroid-sections.md#occt-8-migration-dependency-gate)).
Both retain this worker adapter; native in-process Part exposure is unfinished.
The executable
`implicit_occt_native_worker` links only the isolated rebuilt OCCT 8.1 libraries.
It creates real native fields/patches and performs the restricted operations
there, then emits a bounded-budget JSON response with display vertices, normals,
triangles, per-face source/orientation/trim flags and geometric bounds. Failed
requests emit no partial JSON. No native handles, OCCT 8 headers or libraries
enter the OCCT 7 GUI process.

The in-tree `ImplicitNative.py` adapter exposes editable `App::FeaturePython`
objects in the Native Implicit workbench. It is deliberately **not**
`Part::Feature::Shape`, and cannot be used by arbitrary Part tools. Only operand
centres/radii, operation, plane normal/offset and tolerance persist in FCStd.
The native faces are reconstructed in a short-lived worker on recompute/restore;
only transient Coin buffers display them. Executable selection is environment
or user configuration, never a path loaded from the document. There is no
in-process native-face property, cross-workbench semantic selection, operand
linking or generic OCCT shape persistence in this adapter.

Five commands create editable analytic examples: **OCCT sphere** and **OCCT sphere
trim / common / fuse / cut (experimental boundary)**. These do not replace the existing
linked field-graph commands. Edit `LeftCenter`, `LeftRadius`, `RightCenter`,
`RightRadius`, `PlaneNormal`, `PlaneOffset` and `GeometricTolerance` in the data
properties. Trim uses `unit(PlaneNormal) dot point <= PlaneOffset`. Diagnostics
show face count, display bound and the explicit non-solid status. A failed
operation clears old buffers. Camera changes reuse retained Coin nodes.

Worker calls currently block recompute with a 30-second timeout and bounded
mesh budgets; asynchronous scheduling/cancellation and global budgets remain
future work. Opening an FCStd file reconstructs the geometry and therefore
requires the worker to be available. A missing worker or unsupported case is an
error, never a fallback to cached or fabricated Part geometry.

## Build, try and verify

Reuse the existing builds; no system libraries/packages need changing:

```sh
cmake -S OCCT -B build-occt-implicit \
  -DCMAKE_JOB_POOLS=occt_link=1 -DCMAKE_JOB_POOL_LINK=occt_link
cmake --build build-occt-implicit --target TKBRep --parallel 2
cmake -S tests/occt_native_faces -B build-occt-implicit/native-face-tests \
  -DIMPLICIT_OCCT_NATIVE_BUILD="$PWD/build-occt-implicit"
cmake --build build-occt-implicit/native-face-tests --parallel 2
cmake --build build-freecad-implicit --target ImplicitScripts --parallel 2
export IMPLICIT_OCCT_NATIVE_WORKER="$PWD/build-occt-implicit/native-face-tests/implicit_occt_native_worker"
./build-freecad-implicit/bin/FreeCAD
```

Activate **Native Implicit**, then use the new experimental sphere commands.
The worker is also found automatically in this sibling development build
layout; other layouts can set the environment variable above or the FreeCAD
parameter `BaseApp/Preferences/Mod/Implicit/NativeWorker`. The executable is a
trusted local tool, not selected from FCStd contents. FreeCAD Python console:

```python
import ImplicitNative
obj = ImplicitNative.create("Common")
obj.Operation = "Cut"
App.ActiveDocument.recompute()
```

For the combined suite, register the worker as well as the isolated native tests:

```sh
cmake -S . -B build \
  -DIMPLICIT_OCCT_NATIVE_WORKER="$PWD/build-occt-implicit/native-face-tests/implicit_occt_native_worker"
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

The new GUI test covers commands, property edits, retained buffers, all three
Boolean modes, failure cleanup, camera stability and FCStd reconstruction. The
protocol test checks schema, finite/index-valid meshes, empty results and
explicit error output. Native conformance now includes 19 GTest cases; the full
host configuration has 23 CTest entries with the optional worker GUI/protocol
tests registered. Neither the full upstream OCCT test suite nor a wholly
OCCT-8-linked FreeCAD build has been built or tested.

## Next acceptance gates

1. General plane/field and field/field intersection tracing with tangency and
   singularity status; certified component coverage cannot be inferred from
   sampled vertices.
2. General trim regions, intersection curve ownership, shared boundaries and
   topology-aware meshing; then splitting/classification/history in BOP.
3. General native topology persistence and a dedicated FreeCAD geometry property.
4. A wholly OCCT-8-linked FreeCAD build before exposing native handles to Part;
   keep this process bridge as an explicit experimental alternative meanwhile.
5. Closed-solid validity and whole-model 0.1 mm verification, including small
   feature policy, before manufacturing or STEP claims.
