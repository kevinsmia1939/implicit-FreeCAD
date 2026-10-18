# Corner-aware native gyroid/box boundary

The OCCT-8 constructor now accepts explicit `CornerPolicy::Regularized`;
the FreeCAD facade accepts `mixed_box(..., corner_policy="regularized")`.
The default remains `"reject"`. This is native trim-graph construction,
**not** default-primitive solid qualification or completed Stage 4 integration.

For the unchanged period-10, iso-0 cube `[0,10]^3`, both Common and
BoxMinusField produce 18 approximate native seam edges, 12 retained conventional
box-edge intervals, 12 interior box-edge roots and 19 actual shared vertices.
Six split corners each join two adjacent face contours. The completely retained
corner joins conventional box edges; the other corner is an isolated contact
of the closed inequality and is excluded from the regularized boundary.
That contact remains explicit `excluded_corner_contacts` metadata, not a fake
zero-length edge or a vertex attached to an unrelated surface component.

The operation keeps the field definition, phase, isovalue and box unchanged.
Exact lattice identities and checked inward derivative cones qualify corners.
Axis-edge roots are isolated once; endpoint roots reuse canonical corner IDs,
and internal roots reuse canonical edge IDs. Each planar contour endpoint must
match that graph. Projected derivative signs distinguish entering curves from
isolated planar contacts. Retained box intervals and contour arcs form directed
cap/implicit wire cycles, with two opposite uses per actual edge. Conventional
caps pass BRepCheck; implicit faces retain the exact field and have no fake UV
surface. This does not prove whole-field topology or validity of a solid.

Policy-aware recognition requires exact coordinate-plane intercepts, axis lines
and corner coordinates, with effective identity placements; it must not snap a
nearly rectangular operand onto exact corner identities. Exact restored identity
datum chains work. Unknown near-zero nodes, tangencies, coordinate uncertainty,
nonidentity placement and exhausted budgets fail explicitly.

## Accuracy and ownership

The requested geometric tolerance defaults to 0.1 mm. It bounds seams only,
using the existing face tracing and endpoint replacement budgets. It is not a
whole-surface Hausdorff or topology certificate. General floating-point
engineering reserves still apply; this is not directed interval arithmetic.
No mesh is promoted to native faces. Input geometry, flags and caches are not
modified. Returned C++/FreeCAD handles own their native topology independently.

The facade reports `corner_policy`, `exact_corner_count`, `corner_contour_ends`,
`planar_corner_contacts` and `excluded_corner_contacts`, alongside actual
edge/vertex IDs, conventional cap copies and owned native handles.

## Deliberately unfinished consumers

Schema-1 mixed topology I/O cannot encode the new material policy and therefore
rejects it before writing anything. Native geometry property assignment, existing
mesh/reference/shell/solid factories using that codec also reject. The previous
physical-distance classifier explicitly rejects this policy: its closed-set
corner witnesses would not describe the regularized boundary correctly.
These guards prevent silent contact loss or false qualification. The default
Add Gyroid workbench command is unchanged and still creates a field display.

Next: policy-aware persistence and region classification; corner-aware
whole-domain homotopy/reference and actual trim/cap/component attachment; then
validated native solids and default-workbench exposure. Curved operands and all
remaining Stage 2–4 requirements remain required, including snapping, history,
richer editing and representation-aware consumer dispatch.

## Verification

Kernel tests `DefaultCornerBoundaryUsesSharedNativeTopology` and
`CornerBoundaryRestoredIdentityScaleAndFailurePolicies` cover both operations,
actual edge identity/incidence, cap validity, excluded contacts, restored exact
identity, scale/periodic translation, unchanged sources, legacy refusal, budgets,
placements and guarded solid/persistence consumers. FreeCAD's
`corner_boundary_smoke.py` tests the actual OCCT-8 binding, handle lifetime,
inspection, cap copies and atomic property refusal.

`Boundary` layout changed. Rebuild TKMesh dependencies, native tests/worker and
FreeCAD `Implicit ImplicitGui _ImplicitOCCT`; restart the fork, not merely Python.
Use existing builds and two compile jobs, with one FreeCAD link job. Never load
these OCCT-8 native handles or headers into the preserved OCCT-7 runtime.
