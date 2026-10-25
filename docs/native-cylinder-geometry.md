# Bidirectional native cylinder shell geometry

The later [native solid builder](native-cylinder-solids.md) uses this owned
geometry and bounded mathematical material volume to construct an actual solid.
The geometry query alone retains its separate no-solid contract.

`BRepClass3d_ImplicitCylinder::CheckShellGeometry` now bounds both directions of
boundary-set correspondence for the qualified full-disk cylinder material path.
It retains the topology, tolerance audit and field/component reference. A pass
qualifies the boundary under the existing analytic-bound/floating-reserve policy;
it does not construct a solid or qualify volume, general BRep consumers,
partial/multi-sheet cases or GUI workflows.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
geometry = native.check_cylinder_shell_geometry(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3), distance_tolerance=.1,
)
assert geometry["shell_geometry_validated"], geometry["geometry_reason"]
assert geometry["boundary_bound"] <= .1
assert not geometry["solid_validated"]
fresh = native.inspect_cylinder_shell_geometry(geometry["geometry_handle"])
```

Use the fork's `build-freecad-occt8/bin/FreeCADCmd`. The immutable owner retains
the full shared source graph. Native implicit-face handles and detached side/cap
previews retain their earlier contracts. Inspection returns fresh reports without
repeating the covering calculation. No arbitrary field is assigned to Part.Shape.
The C++ result reports `BoundaryVerified`, two directed side bounds, the combined
`BoundaryBound`, visited `Cells`, tested `SegmentTests` and `Reason`.

## Whole-chart coverage

The physical cylindrical chart uses u=R*angle and v=height. Mapping this chart
to the cylinder does not increase Euclidean distances. The checker covers the
entire finite chart with adaptively bisected rectangles, including periodic seam
and height boundaries. It does not infer global geometry from sampled points.

Each cell has a center and a model-length radius. A global field Lipschitz bound
and scalar/coordinate reserves determine when the entire cell is strictly
selected material or strictly outside. The retained cylinder face's p-curves are
degree-1 splines or lines in the scaled chart. Their segments include cap edges
and both angular uses of the seam. Native classification is constant over a cell
only when trim clearance exceeds its radius plus classification reserves that
include stored edge/vertex tolerances.

For mathematical-to-native distance, a qualified native interior center or the
nearest native trim point supplies a witness. The trim witness is on the actual
retained p-curve boundary. Center/witness distance plus the cell radius bounds
every potentially mathematical point in that cell. Cells known outside the
mathematical set, or entirely within the native set, need no displacement bound.

For native-to-mathematical distance, a strict material center supplies a witness.
Otherwise the checker brackets the unique field root at the center angle using
the already qualified axial direction and derivative lower bound. Endpoint
existence is inherited from the full-disk correspondence proof. Near-zero
evaluations retain a residual/reserve-derived root interval; they are not snapped
to an unqualified zero. Root bracket distance plus the cell radius bounds every
potentially native point in that cell. Known-empty native cells and cells
entirely in the mathematical set need no displacement bound.

A cell is accepted only after both directed bounds fit the request. Otherwise it
splits or refuses on budget/precision exhaustion. The global directed bounds are
maxima over the accepted cover. The exact field-sheet and full-cap correspondence
come from the earlier native reference construction. The final whole-boundary
bound also includes the retained field-wire contour-plus-chord allowance, so
approximate 3D edge geometry is not silently treated as exact surface geometry.

## Limits and evidence

Default geometry work limits are 24 binary split levels, 200000 cells and
20000000 trim-segment tests. C++ accepts separate bounded geometry budgets;
invalid settings throw and unresolved budgets return a reason without a geometry
pass. Cancellation checkpoints include cell, segment and root work. Failure does
not promote partial bounds into a certificate. Source fields and topology are
unchanged. This is analytic numerical engineering with floating reserves, not
interval arithmetic or an ordinary BRepCheck certificate for implicit surfaces.

Native tests cover both material signs and 0.1/0.05 requests, independent analytic
gyroid height roots against conventional face distance/classification, translation,
reversed axes, partial refusal, explicit depth/cell/segment failures and late
cancellation. CLI tests cover owned reports, unchanged source records, false
solid flags and wrong owner rejection. Accurate volume/material queries, native
solid promotion, partial/multi-sheet correspondence, combined persistence and
live GUI workflows remain required by the overall roadmap.

## Material point queries

```python
point = native.classify_cylinder_material(
    geometry["geometry_handle"], (0, 0, -.5), point_tolerance=1.e-7,
)
assert point["mathematical_state"] in ("IN", "OUT", "ON", "UNKNOWN")
assert point["distance_lower_bound"] <= point["distance_upper_bound"]
```

The C++ `ClassifyMaterial` entry point accepts an unchanged qualified
ShellGeometry reference. FreeCAD requires its immutable geometry owner. Queries
reuse that reference; they do not rebuild the geometry. The authoritative region
is the selected field half-space inside the finite cylinder, not an ordinary
OCCT solid classifier's result. Source fields and graph snapshots are unchanged.

The query projects to the closed finite cylinder before evaluating the field.
Cylinder clearance and a field Lipschitz cone provide lower distance bounds.
The selected full end cap and the unique axial field root provide upper-bound
witnesses. Strictly selected side witnesses can sharpen the upper bound. A local
Hessian cone over the globally enclosed query-to-root segment can sharpen an
overly conservative axial derivative bound. Scalar and coordinate reserves are
retained. `field_value_at_domain_projection` is diagnostic scalar F, not distance.

ON requires the upper bound to fit the model-length query tolerance. IN/OUT
require the lower bound to exceed it and a definite mathematical side. Otherwise
the state remains UNKNOWN. Invalid tolerance/points and unqualified references
reject. `boundary_allowance_separated` means the lower bound exceeds the geometry
approximation allowance plus the query tolerance. It is separate from the
mathematical classification and does not authorize an unsupported consumer.
No native solid or volume claim follows from this point query.
