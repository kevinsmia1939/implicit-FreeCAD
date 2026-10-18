# Corner-aware closed material reference

`CheckRegionTopology(..., CornerPolicy::Regularized)` now caps the checked
[corner-aware surface reference](native-corner-topology.md) on the same actual
grid, root and corner identities. It constructs a transient closed boundary
reference and labels connected strict-sign volume regions. It does NOT construct
native CAD shells/solids, check stored native attachment or certify 0.1 mm error.

## Shared caps and contact semantics

The closed builder uses the surface witness's actual resolution (31 for a
requested 32), not the requested count. Exact box-corner values are the same
checked mathematical zeros as in the surface witness; other values are unchanged.
A dense bounded nodal cache avoids a separate tree entry for every grid sample.

Each exterior Freudenthal triangle is clipped against the retained affine
half-space. Strictly retained material must exist on the triangle before it can
create a cap. A zero alone is not a retained cap. Mixed zero/strict vertices use
one canonical vertex; shared surface corners reuse `(i,i)` identities instead of
coordinate welding. Strict crossings reuse the original surface edge roots.

The surface has two isolated relative-box zero contacts in the default cube.
For each operation, one is excluded from the regularized material and does not
appear in the closed reference at all. The other is a retained cap-only box
corner, included once after the surface-node prefix. It is not a fake implicit
surface vertex. The six split corners use the same indices on caps and surface.

## Volume grouping is strict

Only strictly retained grid vertices enter the volume disjoint-set graph.
Within a tetrahedron, the strict retained affine region is convex, and such
vertices join through its strict material. Zero vertices never join that graph.
For each zero corner, incident tetrahedra contribute their already-connected
strict-volume representatives. They must all agree on one material region;
otherwise the reference remains unresolved instead of joining regions through
a point contact. Root/cap nodes then inherit those unique region labels.

Existing opposite paired edges, cyclic vertex links, closed component/Euler and
shell-to-volume checks remain required. Shared edges alone are insufficient.
All prerequisites of the whole-box F-to-L homotopy still apply, including fixed
corner cones; this is not classification of an arbitrary display mesh.

For the unchanged default cube, each operation has one closed component with
Euler characteristic -2, belonging to one connected material region. Its affine
reference volume is approximately 500; complementary references sum to the
cube volume 1000. These signed-volume sums describe L's numerical reference,
NOT F's physical volume or a geometric-accuracy bound. A separate two-island
case stays in two material regions.

## FreeCAD API

```python
report = native.check_box_region_topology(face, 32, 16, 4000000, "regularized")
report = native.check_mixed_region_topology(boundary_handle)
```

The final optional policy argument follows the surface API: raw defaults remain
strict; mixed snapshots honor their stored policy and reject overrides. Reports
include corner_policy, fixed_corner_count and surface_point_contacts. The last
field describes isolated contacts of the SURFACE zero set: one may legitimately
be a retained cap-only vertex of the closed material boundary.

The surface arrays remain prefixes of the closed arrays, with explicit cap-side,
closed-component and material-region labels. Native-shell/solid construction,
stored correspondence, topology-validity and geometry-accuracy flags remain
false. Restored source seam bounds remain unverified; explicit reference queries
do not recertify them. No new persistence schema or transient struct-layout
change in this increment. Source math/topology/caches remain immutable.

## Verification and remaining gates

Kernel tests cover both operations, complete shared edge/corner incidence,
outward cap orientation, excluded versus retained-only corners, complementary
reference volumes, scaling/periodic translation, primitive restore, source
immutability, work limits and independent material islands. Native CLI checks
independently reconstruct edges, Euler and signed volumes from the public arrays;
they cover raw/mixed/restored property ownership and guarded downstream builders.

Next relate STORED native cap regions, complete trim cycles and whole implicit
components to this reference. Then apply representation-aware geometric checks
and construct/validate default native solids before changing the default GUI.
Native mesh/correspondence/cap-geometry/shell/solid guards remain in force.
Curved operands, snapping/history, richer editing and all other unfinished
Stage 2–4 acceptance gates stay active.
