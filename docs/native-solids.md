# Restricted native mixed solids

`BRepClass3d_ImplicitBox::BuildNativeSolids` constructs actual `TopoDS_Solid`
objects from the supported gyroid/axis-aligned box region. This is a restricted
implicit-aware **numerical** validation path at a requested physical tolerance
(default **0.1 mm**). It does not qualify generic UV-dependent OCCT algorithms,
ordinary Part solids, STEP export or every implicit field/operand.

## Construction gates

1. Fresh `BuildNativeShells` attaches whole exact gyroid components and actual
   conventional cap faces to the checked mathematical reference. It checks
   oriented incidence, connected cyclic vertex links, face connectedness, Euler
   characteristics and shell/material grouping. Axis component splits require
   F/L-zero-free planes; other unresolved component restrictions still reject.
   Its graph prerequisite also checks the fresh seam-trace bound plus continuous
   stored-curve and cap p-curve deviation against the source field-face tolerance.
   This seam check is independent of cap-region proximity: two nearby filled cap
   regions alone would not bound every stored trim curve against the zero set.
2. Every stored face/edge/vertex tolerance must be at most the requested
   tolerance. Existing tolerances are not silently tightened.
3. `CheckCapGeometry` checks the actual cap geometry in both directions against
   the mathematical retained box-face regions. It includes stored 3D edges and
   vertices, not just face interiors or mesh samples. The newly constructed
   field faces retain their exact equations and checked covering domains.
4. Only after all gates pass are shells placed into genuine solids grouped by
   material region. A cavity shell stays with its outer shell; separate islands
   become separate solids. An empty region returns an empty compound/solid list.

The solid output remains a dedicated owned native snapshot, **not** a
`Part::Feature::Shape`. Never wrap or sew a mesh to make it appear Part-compatible.
`Validated` is scoped to the numerical implicit-aware policy above, not a formal
interval certificate, ordinary `BRepCheck_Analyzer` result or general BOP support.

## Whole-cap geometry check

The checker privately validates/copies conventional planar candidates, accepts
only supported axis-aligned planes and full-range matched degree-one 3D/2D
B-spline parameterizations, and lifts all p-curve poles to their actual planes.
Polygon parity handles holes; distances use actual polygon boundary segments.
A spatial segment tree prunes only when a node cannot affect either nearest
distance bound **or** the half-open parity ray. A proximity-only index would be
incorrect for holes. It reorders only private segment buffers.

Forward checks partition all six mathematical box sides. Lipschitz/Taylor
gradient/Hessian bounds exclude strictly nonretained cells. Remaining cells
need a whole-cell distance upper bound to stored caps. Interior projected cells
use the plane offset; other cells use a boundary witness plus cell radius.

Reverse checks partition every actual cap's projected bounds. Polygon boundary
clearance excludes cells wholly outside the cap, including holes. Retained
field bounds or explicit strict retained witnesses on the true box side bound
all remaining cells. Projection clamps to the real box rectangle, including
slight cap overshoots. Surface/curve/vertex discrepancies and coordinate/field
evaluation reserves consume the requested budget rather than enlarging it.

The 3D curve/p-curve deviation is bounded over the complete piecewise-linear
parameter intervals by pole deviations. Endpoint/vertex deviations are included
conservatively. Coordinate magnitudes include plane parameters, lifted poles,
raw curve poles and vertices. Unsupported representations reject; exhausted
depth, precision, cells or segment work return unresolved. These are engineering
floating-point bounds with reserves, not directed-rounding interval arithmetic.
They do not turn arbitrary unchecked cap topology into a valid solid.

Defaults: resolution 32, depth 16, topology cells 4M, attachment tests 2M,
separator cells 2M, geometry cells 2M and segment tests 8M. Geometry cell and
segment caps are shared across both directed checks. A segment tree is bounded
by the input limit of one million poles. These are bounded-work/input policies,
not hard wall-time or total allocator guarantees.

## FreeCAD API (OCCT 8 build only)

```python
import ImplicitOCCT as native
report = native.build_mixed_solids(obj.Geometry, .1)
if report["solid_validated"]:
    handle = report["solid_handle"]
    diagnostics = native.inspect_mixed_solids(handle)
    state = native.classify_mixed_solids(handle, (1, 2, 3), .1)
```

The build call's remaining positional arguments are resolution, depth, topology
cells, attachment tests, separator cells, geometry cells and segment tests.
Failure reports contain reasons/work diagnostics but no `solid_handle`.
Success reports include solid count, shell/material mapping, independent owned
field faces, geometry directional bounds and maximum stored tolerance.
The nested `geometry.boundary_bound` is the whole **cap/edge/vertex** check's
bound. The inherited trim check is bounded separately by the stored field-face
tolerance. A conservative combined bound is the maximum of that cap bound and
`maximum_stored_tolerance`; neither may exceed the requested tolerance.
`part_compatible` and `generic_brep_check_performed` remain false.
`classify_mixed_solids` queries the authoritative material-region **union**,
not per-solid membership, and is independent of display triangles. Its
`solid_validated` flag records the owned snapshot's scoped validation policy.

Snapshots/faces survive document close. The source topology, caches and bound
provenance are unchanged. An [explicit solid topology codec](native-solid-topology-io.md)
now preserves actual result topology without retracing, but clears restored
validation flags. FCStd still persists the original mixed boundary; a dedicated
solid geometry property/owner remains required. A fresh solid build rechecks
restored boundary data; it never trusts a saved seam-verification flag.

## Verification and remaining work

Kernel cases cover both operations/open and closed contours, original source
immutability, empty/full regions, separate islands, cavity shell grouping,
scaled and integer-period-translated coordinates at the fixed 0.1 mm target,
missing/extraneous caps and depth/work/tolerance failure. CLI coverage adds
restoration, ownership/lifetime, classification equality, diagnostic inspection
and unsupported Part/handle rejection. See PROGRESS.md for executed results.

This advances Stage 3, not completion of Stages 2–4. General curved B-Rep
operands, non-axis-separable field restrictions, history/snapping/consumer
contracts, solid-property persistence and the integrated CAD-style GUI workflow
remain required. The default workbench gyroid is still the simpler native field
feature; this factory is not silently run by Add Gyroid or the surface preview.
