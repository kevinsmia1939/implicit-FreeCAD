# Native cylindrical material regions

`BRepClass3d_ImplicitCylinder::BuildSides` builds actual OCCT cylindrical faces
for F<=0, or F>=0 with the positive selection. It reuses a single tracing/cap
snapshot. These are conventional cylinder surfaces with native trim topology,
not triangle faces. The field equation remains authoritative. Implicit-face
attachment, component assembly, closed-solid proof, persistence of the combined
native graph and live GUI source/property workflows remain required.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
result = native.build_cylinder_sides(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (3, 3 + 2 * math.pi), (0, .5), distance_tolerance=.1,
)
assert result["side_topology_validated"], result["side_reason"]
assert not result["solid_validated"]
side_faces = result["sides"]
cap_faces = result["caps"]
fresh = native.inspect_cylinder_sides(result["sides_handle"])
```

The fork runtime is `build-freecad-occt8/bin/FreeCADCmd`. The immutable capsule
owns the graph, field snapshot and settings; the returned Part.Face objects are
independent previews. Editing a preview does not modify the owned graph. A fresh
inspection does not retrace the field or reconstruct material regions. Serialized
individual face previews do not preserve cross-face sharing; that remains in the
native owner. This is a query API, not yet a document feature or toolbar workflow.

Field contours are split at logical chart-seam crossings and cap endpoints.
Their degree-1 B-spline edges retain all intermediate samples as internal poles,
not selectable topological vertices. A closed winding curve is rotated to a
logical seam node before splitting. No endpoint proximity welding is used.
The matching UV curve uses the same knot parameters. The cylinder-vs-chord
deviation on each segment is bounded by R*(delta_angle)^2/8 plus a coordinate
reserve. The maximum is reported as `side_surface_deviation_bound`; edge
tolerances include it and the endpoint tolerances and must remain below the user
tolerance. `side_contour_bound` separately reports the inherited regular-cell
field-contour bound. These are engineering floating-point policies, not interval
certificates or a global implicit-solid geometry proof.

Tangent/gradient/Hessian guards orient field edges for the selected material.
Cap rings use the opposite incidence from their planar faces. Selected axial
seam intervals have two uses: forward at the upper angle and reversed at the
lower angle. The two UV occurrences of each seam vertex are distinct in the
directed graph while retaining the same native vertex handle. Directed degree
checks, finite signed chart area with a reserve, bounded cycle nesting and
BRepCheck_Analyzer gate the retained cylinder faces. Nesting uses the existing
resource policy, with cancellation checkpoints. A failed side batch returns no
side faces, though a separately qualified source/cap snapshot can remain.

Supported inputs retain the existing restriction to a childless exact gyroid,
a full-turn finite cylinder and a conservative enclosure inside the field box.
The side builder requires both cap disks/regions to qualify. Critical/uncertain
cells, ambiguous circle attachment, orientation uncertainty and nesting budget
exhaustion reject rather than guess. Exploratory near-maximum cases were refused
by the cap-skeleton gate; they do not establish isolated-loop/hole support.

Native tests cover both signs, open and closed winding contours, matching UV/3D
curves, cylinder deviation, shared endpoint identity, complementary side areas,
material classification away from the uncertainty tube, full/empty sides,
translation, unchanged source records, failed budgets and late cancellation.
FreeCAD CLI checks cover typed previews, detached mutation, owned inspection,
source records and explicit refusal of non-owner handles. No closed cylinder
Boolean, STEP export or seamless FreeCAD workflow follows from these tests.

## Field-boundary incidence prerequisite

The C++ `BuildTrimBoundary` entry point retains `Source` (the side/cap snapshot),
`Wires` and `EdgeWires`. It first audits actual face-edge occurrences: each field
edge is used exactly once with its expected orientation, while every other edge
has one forward and one reversed use. It rejects duplicate/unused field edges.
Reversing the field-edge uses then gives the incidence required on implicit
faces. Directed one-in/one-out vertex checks assemble closed wires using native
vertex identity; no proximity welding or extra sample vertices are introduced.
`EdgeWires` follows side-field edges, bottom-field edges, then top-field edges.
Wire indices are snapshot-local and do not provide persistent topology names.

`IncidenceVerified` proves this boundary graph and ordinary wire validity. It
does not assign wires to field-surface components, prove interior sheet topology,
construct implicit faces, or qualify a shell/solid. Even an empty wire list does
not by itself establish an empty implicit surface. Failed wire batches clear
both output arrays; independently qualified source regions can remain. Late
cancellation propagates. This prerequisite is currently C++ only.

## Axial graph qualification prerequisite

The C++ `CheckAxialSheet` result owns that boundary graph and separately reports
`MonotonicityVerified`, `Direction`, `DerivativeLowerBound`, visited `Cells` and
`Reason`. It covers the entire finite cylinder with adaptively bisected local
boxes and uses the global gyroid Hessian norm bound 4*(2*pi/period/model_scale)^2
to bound variation of the axial derivative. Cells strictly outside the disk plus
a coordinate reserve can be skipped. Every retained box must have the same
strict derivative sign. The final lower bound is the minimum accepted signed
cone bound over the cover. Failure leaves direction and lower bound zero.

Default work limits are 24 binary split levels and 200000 visited cells; maximum
limits are 32 levels and 2000000 cells. Invalid budgets throw. Unresolved depth,
cells, precision or direction produce explicit refusal; cancellation propagates.
This uses analytic derivative bounds with floating reserves, not interval
arithmetic. It proves at most one field zero along each axial fibre under that
numerical policy. A field with no zero can still pass. It does not establish
projected boundary correspondence, existence, connected components or which
wires bound each component. Those remain required before attaching native
implicit faces. Folded/multi-sheet cases require the general component path and
are not covered by this sufficient graph-like-sheet criterion.
