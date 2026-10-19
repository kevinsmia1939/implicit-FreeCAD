# Whole-box corner homotopy and surface topology reference

The explicit `CheckTopology(..., CornerPolicy::Regularized)` path now handles
the unchanged default gyroid's exact box-corner zeros. This extends the existing
whole-domain field-to-nodal-linear sufficient-condition checker, not just local
corner queries or a display mesh. Ordinary floating-point engineering reserves
apply; this is not directed interval arithmetic or native-solid qualification.

## Construction and checks

The requested resolution remains a power of two in [4,64]. Explicit corner mode
uses the odd subdivision count one smaller than that request (32 -> 31). This
avoids systematic quarter/eighth-period interior samples without moving the
field, its phase/isovalue or the domain. Only physical box corners with checked
exact lattice identities may be zero nodes. Other near-zero or interior zero
samples remain unresolved, including known lattice nodes not covered by this
specific corner policy. Resource/precision limits fail explicitly.

The existing checker bounds the full convex homotopy between F and the shared
piecewise-affine reference L on every tetrahedron, with projected checks on real
box faces/edges. Internal mesh facets are not falsely treated as box strata.
Strict same-sign exclusion or positive gradient alignment must cover every
tested simplex/subdivision. These sufficient-condition checks use field bounds,
not the display mesh or incidence alone, to relate F to the reference.

At fixed zero corners, every incident tetrahedron additionally needs all three
inward coordinate signs of grad(L) to agree with the fresh checked F germ, with
linear-solve/gradient reserves. Since F(a)=L(a)=0 exactly, these signs persist
through the convex homotopy in a common corner neighborhood. Split cones retain
their entering/contact pattern; uniform cones retain isolated point contacts.
No corner stratum is silently skipped or displaced to obtain a regular mesh.

Reference extraction gives one shared identity `(grid_id, grid_id)` per exact
surface corner. Ordinary roots keep their originating grid-edge pair. A cell
with an isolated zero and only one strict sign creates no fake triangle or
surface vertex. Uniform corner contacts are retained separately in PointContacts.
The existing oriented incidence, cyclic/path vertex links and component/Euler/
boundary-loop checks apply to the two-dimensional reference. ReferenceManifold
refers to that surface part, NOT a zero set with its contacts erased or a closed
material boundary. Contacts must be consumed separately.

## Default result and scope

At requested resolution 32, the default `[0,10]^3`, period-10, iso-0 cube resolves
with eight fixed corners, six shared surface-corner vertices and two separate
point contacts. Its two-dimensional reference has 13,779 nodes, one connected
component, Euler characteristic -3 and one boundary loop, in both orientations.
These reference arrays are transient topology witnesses, not native CAD faces,
a manufacturing mesh or a 0.1 mm surface certificate. Accuracy, stored native
trim/cap correspondence and solid validation flags remain false.

## FreeCAD API and ownership

In the wholly OCCT-8 fork:

```python
report = native.check_box_topology(face, 32, 16, 4000000, "regularized")
report = native.check_mixed_topology(boundary_handle)
```

The raw face API defaults to strict legacy policy; mixed snapshots automatically
use their stored policy. Optional `auto`, `reject`, `regularized` values are
accepted as the final positional argument. A mixed query cannot override its
source policy. Results expose corner_policy, fixed_corner_count, point_contacts
and reference_grid_edges alongside the existing surface/counter diagnostics.
The check is over the face's computational box, not stored wire membership.

Primitive/restored exact identity datums work; nonidentity placement rejects.
Queries preserve source topology, source meshes and original/restored seam-bound
provenance. No new persistence schema or saved proof flags. The newer
[closed-region reference](native-corner-region.md) now adds shared caps and
strict material connectivity; ordinary strict-region queries remain unchanged.
[Whole restricted faces/native shells](native-corner-shells.md) now attach through
separate checks. [Cap geometry](native-corner-cap-geometry.md) is qualified
separately too. [Native solid construction/I/O](native-corner-solids.md) now uses
fresh qualification. [Mesh/live display](native-corner-mesh.md) is now supported
opt-in; [clipped coverage](native-corner-coverage.md) now has a separate checker. This reference
alone does not enable those consumers.

## Verification and next work

Kernel tests cover both orientations, shared identity, contact separation,
scale/periodic translation, primitive restore, source immutability, insufficient
depth/work, unknown near-zero identities and placement/settings refusal.
`corner_topology_smoke.py` independently accounts for the returned triangles,
oriented edges, boundary loops and Euler values, and checks raw/mixed/restored
ownership, policy mismatch and guarded downstream consumers.

Shared-ID capping and strict-volume connectivity now exist in the
[closed material reference](native-corner-region.md). Next relate stored native
caps/trims and whole implicit components to that reference before constructing/
validating default native solids and GUI exposure.
Curved operands, snapping/history, richer editing and all remaining Stage 2–4
acceptance requirements remain active.

TopologyWitness and nested transient result layouts changed. Rebuild TKMesh
dependencies, native tests/worker and FreeCAD Implicit/ImplicitGui/_ImplicitOCCT;
restart the fork, not merely Python. Existing builds/dependencies only, two
compile jobs and one FreeCAD link job. No OCCT-8 headers enter the OCCT-7 runtime.
