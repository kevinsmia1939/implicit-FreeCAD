# Closed region reference and material grouping

The OCCT-8 native module now completes the [whole-box homotopy reference](native-box-topology.md)
with retained nodal-linear planar caps, and groups its closed boundary components
by adjacent retained **volume region**. This advances native shell construction
without pretending that reference triangles are native B-Rep faces or solids.

```python
import ImplicitOCCT
report = ImplicitOCCT.check_mixed_region_topology(boundary_handle)
# Or: ImplicitOCCT.check_box_region_topology(native_gyroid_face)
assert report["whole_box_homotopy_resolved"], report["reason"]
assert report["closed_reference_manifold"]
assert not report["native_shell_constructed"]
assert not report["solid_validated"]
```

The adapters query the primitive gyroid's entire mathematical box region.
Forward faces retain F<=0 (Common); reversed faces retain F>=0 (BoxMinusField).
Stored native trim wires/caps are not matched or substituted. Existing GUI
previews do not run these explicit synchronous queries automatically. Source
math, topology, caches, native geometry properties and stored seam-bound
provenance remain unchanged. Only identity-placed primitive gyroids are supported.

## Shared closed reference

The prior field-to-linear homotopy check must resolve first, and its surface
reference must pass incidence/link checks. Each reference root now retains its
originating pair of grid-vertex IDs as internal transient provenance. On the
six box sides, the same Freudenthal exterior triangles are clipped against the
retained affine half-space. Crossing nodes must already exist in that root map;
retained grid vertices are shared by grid ID, including across box sides.
There is no position welding, new root projection, collar or UV implicit face.

Surface root nodes and triangles are preserved as prefixes of the closed
buffers. New cap triangles face outward from the box for either operation.
All closed reference edges must have two opposite uses, and each vertex link
must be a connected cycle. Component Euler characteristics and per-triangle
closed-component labels come from these shared incidences. No mesh node is
inserted into authoritative native topology or Part::Feature::Shape.

Closed boundary components are not the same as connected material regions.
Within each tetrahedron the retained affine half-space is convex. Every
nonempty retained tetrahedron or shared face has a strictly retained nodal
vertex, because unresolved/zero nodal signs already reject. Joining retained
vertices in the common tetrahedral 1-skeleton therefore gives retained volume
connectivity, including the connections across cell faces.

Each boundary triangle is assigned through its retained grid vertices/root
endpoints to that volume connectivity. All triangles of a closed component
must agree on the adjacent region; every retained region must have a boundary.
This is not a signed-volume guess or nearest-point pairing. For example:

| Retained reference region | Closed boundary components | Material regions |
|---|---:|---:|
| Box with one enclosed cavity | Outer shell and inner shell | 1 |
| Two disconnected islands | Two outer shells | 2 |
| Full box / empty region | 1 / 0 | 1 / 0 |

## Result contract

- `whole_box_homotopy_resolved` and `surface_reference_manifold` describe the
  prerequisite check. `closed_reference_manifold` additionally requires cap
  closure, cyclic links and consistent retained-volume grouping. Partial
  counters/buffers on failure are diagnostics only.
- `reference_vertices`, `reference_triangles` and
  `reference_implicit_vertex_count` / `reference_implicit_triangle_count` expose
  the combined buffers and original surface prefixes. Indices are zero-based
  local coordinates. `reference_triangle_sides` is -1 for implicit triangles,
  otherwise 2*axis+side, with axes X=0/Y=1/Z=2 and side low=0/high=1.
- `reference_triangle_components` labels closed boundary components.
  `reference_component_euler` is sorted; labels use that order with a deterministic
  internal tie-break, not persistent identity. `reference_component_regions`
  maps each such component to a retained volume region.
- `reference_region_count` counts retained volume regions, not boundary shells.
  `reference_surface_component_euler` and `reference_surface_boundary_loops`
  preserve the original open/closed field-surface reference diagnostics.
- `reference_component_signed_volumes` sums translated scalar triple products
  using long double accumulation. Cavity shells have negative oriented volume;
  outer shells have positive volume. `reference_region_volumes` groups those
  sums by material region. These are **numerical volumes of the linear
  reference**, not accuracy-qualified volumes of F's region or containment
  certificates. Grouping uses connectivity, not these signs.

Resolution/depth/homotopy-work limits are unchanged: default 32/16/four million,
with resolution a power of two from 4 through 64. Cap construction and volume
connectivity have separate finite work bounds implied by that resolution:
at most twelve times resolution-squared exterior triangles and six times
resolution-cubed tetrahedra. Combined buffers remain conservatively limited to
200,000 nodes and 400,000 triangles. There is no package or toolkit dependency
change. A missing root, inconsistent grouping or exhausted limit is unresolved.

`stored_trim_correspondence_validated`, `native_shell_constructed`,
`topology_validated`, `solid_validated` and `geometry_accuracy_verified` remain
False. The homotopy bounds use engineering floating reserves, not directed
interval arithmetic; this does not add a formal isotopy certificate or a
0.1 mm mesh/volume accuracy claim. The user's production tolerance is unchanged.

## Evidence and next step

Three kernel cases cover both operations with closed/open native trim fixtures,
source-record immutability, opposite paired reference edges, cap normals, Euler
accounting and complementary reference volumes summing to the box volume.
They distinguish cavity/outer-shell grouping from two disconnected islands,
test full/empty regions and retain explicit critical/resource/input failures.
The cavity reference also passes the separate exact binary64 triangle-contact
audit; this test does not change the core check's numerical contract or add a
TKMesh dependency to TKTopAlgo.

FreeCAD CLI tests independently rebuild edge/Euler/volume accounting from the
buffers, compare raw/mixed adapters and the original surface prefixes, and
exercise both operations, larger open contours, restored seam provenance,
snapshot lifetime and representation/placement guards. A two-cavity example
requires three boundary components but one retained region.

The newer [cap attachment check](native-cap-attachment.md) now maps stored cap
regions using common retained points beyond fresh seam uncertainty.
Next connect the complete stored native trim/cap realization to this region reference,
including contour identities and component attachment, then construct and
validate native shells/solids. Equal counts or independently successful graph
and homotopy checks are insufficient to establish that missing correspondence.
General curved operands and unfinished [Stage 4 gates](stage-4-acceptance.md)
remain in scope; the overall goal is not complete.
