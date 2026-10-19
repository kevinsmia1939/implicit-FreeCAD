# Corner-aware complete trim-cycle attachment

`CheckTrimAttachment` now accepts regularized-corner boundaries after the full
stored-graph/cap-region prerequisites. The complementary native boundary is
rebuilt with the SAME corner policy; both material references must use the same
actual grid and root provenance. No source field, phase, domain or tolerance is
changed. This is directed boundary-cycle attachment, not whole-face membership,
native shell/solid construction or 0.1 mm whole-model certification.

## Anchor identities

Interior box-edge roots retain ordered `(physical_edge, rank)` labels, with
edge IDs 0..11. A checked exact split corner instead has `(12+corner_index, 0)`.
It is one shared corner, NOT three roots on its incident edges. Only exact box
coordinates and a checked split-cone germ permit this identity. Uniform contacts,
duplicate corner anchors and near-corner ambiguity fail rather than weld/snap.
Native/reference anchor counts include the interior roots plus split corners.

Complementary surface roots match by originating grid-edge identities, including
the canonical `(i,i)` fixed-corner nodes. Arc identities combine physical side,
retained cap region, complementary cap region and ordered endpoint anchors;
closed interior arcs retain the existing winding test. Continuous curve matching
and unique complementary ownership remain required. Complete oriented cyclic
sequences must match bijectively; equal counts or nearby endpoints are not enough.

For default Common and Cut, 18 seams attach to one directed reference loop and
one reference surface component. Its Euler characteristic is -3, and the
reference assigns it to closed component 0/material region 0. The six split
corners are shared anchors; neither isolated surface contact is a trim anchor.

## API and scope

`ImplicitOCCT.check_mixed_trim_attachment(handle)` now reports corner_policy,
the resolved reference_resolution and reference surface_component_euler,
surface_component_shells and surface_component_regions, alongside the existing
seam/loop/component mappings. The component labels describe the REFERENCE,
not themselves a native face restriction. The later [native face-domain work](native-face-domain.md)
adds that restriction separately. Reports remain transient and snapshot-local.

Source topology/caches and original/restored seam-bound provenance are unchanged.
No schema or transient C++ result-layout change. Restored records/properties
retain owned topology after document close. Existing strict-policy paths remain
unchanged; comparison/work/precision failures do not publish success.

`implicit_face_attachment_validated`, `native_shell_constructed`,
`topology_validated` and `solid_validated` stay false on this trim-only report.
The later [whole-face/native shell extension](native-corner-shells.md) qualifies
native topology separately. Solid, mesh, coverage and cap-geometry consumers
still guard the corner policy. Actual face membership excludes isolated contacts;
merely omitting display-mesh vertices is insufficient.

## Verification and next work

Kernel tests cover both operations, all 18 arc identities, full directed loop
and reference component/Euler mappings, genuine rotation of a stored wire's
starting edge, stored mixed-topology restore, source immutability and budget refusal.
The CLI independently accounts for native seam edge/vertex connectivity and
the six shared physical corner anchors, then tests restored ownership and
guarded downstream builders. No native face is reconstructed from reference
triangles or disguised as a UV surface.

The monolithic kernel suite now includes repeated dual-cap/closed-reference
audits. Both CTest harnesses allow 180 seconds instead of 120; the expanded
94-case run takes 126.76 seconds on this host. Numerical work/accuracy budgets
are unchanged. This is test orchestration headroom, not looser geometric gates.

Whole restricted-face attachment/native shells now exist. Next qualify native
geometry/solids and expose the default GUI workflow. All unfinished
Stage 2–4 scope remains, including curved operands, snapping/history, richer
editing and broader consumer/placement integration.
