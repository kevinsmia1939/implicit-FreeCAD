# Dual-cap trim-wire attachment

OCCT now provides `BRepClass3d_ImplicitBox::CheckTrimAttachment`, exposed in the
OCCT-8 FreeCAD API as `ImplicitOCCT.check_mixed_trim_attachment(handle)`. It maps
stored contour arcs and complete directed trim wires to the common field/linear
reference, including multiple holes on a single retained cap. It does not attach
the whole native implicit face or construct/validate native CAD shells or solids.
The [corner-aware extension](native-corner-trim.md) now adds canonical shared
corner anchors, complementary corner-policy preservation and fixed-node reference
provenance. Whole-face/contact membership and native-solid validity remain separate.

```python
report = ImplicitOCCT.check_mixed_trim_attachment(handle, 32, 16, 4000000, 2000000)
assert report["trim_wires_attached"], report["reason"]
assert not report["implicit_face_attachment_validated"]
assert not report["solid_validated"]
```

The call is explicit, synchronous and restricted to the supported identity-placed
gyroid/axis-aligned-box native boundary representation. Source math, topology,
caches and saved seam-bound provenance remain unchanged. Restored records do
not become `seam_bound_verified=True`; no document property or GUI validity flag
is changed. Field definitions stay authoritative and reference meshes transient.

## Arc identity and directed wires

First require the existing [cap attachment policy](native-cap-attachment.md),
including fresh stored-graph correspondence and the whole-box homotopy reference.
Privately construct the opposite operation's canonical boundary from the SAME
field/domain, then attach its cap regions too. This reuses native canonical
construction without a conventional-box reconstruction or TKPrim dependency.

Both linear references have the same mathematical zero surface. Root nodes are
matched by their originating grid-vertex pairs, not by coordinate welding or
nearest points. Each reference surface-boundary segment must have exactly one
cap owner from each operation, on the same physical box side.

Maximal contiguous segments with the same (side, retained cap, complement cap)
form reference contour arcs. Where a contour crosses a box edge, its anchor is
identified by physical box-edge ID and ordinal along that edge. The independently
checked edge-stratum homotopy preserves root order; the stored canonical graph
supplies the corresponding native roots. Counts per physical edge must agree,
and coordinate/root-order precision ambiguities reject.

Native seams match their private complementary traces by continuous monotone
PWL curve comparison, including reversed parameters and collinear resampling.
The resulting current/complement cap-region pair and ordered endpoint anchors
must identify exactly one reference arc. A closed contour wholly on one face
has no box-edge anchor: the **dual** region pair distinguishes holes that share
one retained cap. Nonunique descriptors are unresolved, never assigned arbitrarily.

Open arcs have canonical direction from their ordered root keys. Closed arcs
use signed planar winding area relative to the outward box-side normal, with
local-origin accumulation and numerical reserves. Full effective native wire
cycles must match the full signed reference cycles up to cyclic start position;
wire order is not used to establish identity. Every arc and wire needs a
bijection. Counts alone do not establish attachment.

Reference triangle connectivity additionally identifies which zero-surface
component each attached wire borders. This can reveal different components
within the one currently stored native implicit face; it does not split that
face or establish a CAD-valid connected-face representation.

## Report contract and limits

`cap_regions_attached` and `complement_cap_regions_attached` are prerequisite
statuses. `trim_wires_attached` requires the complete arc and directed-cycle
checks. Partial arrays on failure are diagnostic only.

`seam_reference_arcs` follows unique implicit-edge traversal order, matching the
existing native seam snapshot ordering. `wire_reference_loops` and
`wire_surface_components` follow direct native wire traversal order. Labels are
transient query identities, not persistent element names or Boolean history.
Surface-component labels are local to this query, not indices into the separately
sorted Euler-characteristic list of another query.

`reference_arc_count`, `reference_wire_count` and
`reference_surface_component_count` describe the reference. `comparisons` and
`curve_samples` count dual-trace matching work. Reference components without
boundary wires cannot be attached through wire correspondence; even a vacuously
complete empty wire mapping would not validate such an implicit face.

The existing resolution/depth/homotopy limits apply independently to each
operation (defaults 32/16/four million). `maximum_tests`, default two million,
allowed 1 through ten million, independently bounds each cap search and the
dual-trace candidate comparisons, not aggregate CPU time. Continuous curve
comparison retains an eight-million-knot-test ceiling. Existing fresh graph,
codec, mesh and numerical limits remain in force. Numerical reserves, exhausted
budgets, missing/nonunique matches and uncertain winding/root anchors remain
unresolved. Unsupported settings/representations reject explicitly.

These are engineering floating-reserve checks, not directed intervals or a
formal global isotopy certificate. `implicit_face_attachment_validated`,
`topology_validated`, `native_shell_constructed` and `solid_validated` remain
False. The user's 0.1 mm target and existing geometric tolerance policies are
unchanged; no manufacturing-solid or generic UV-based BOP support is claimed.

## Evidence and next milestone

Three new kernel cases cover both operations, closed contours, three-wire open
fixtures, two holes sharing a retained cap, separate surface components,
restored provenance, source-record immutability and resource/settings failures.
Existing changed-math/forged-bound tests now stop this query at its cap/graph
prerequisite; resampling plus cyclic native wire starts also pass this checker.
CLI tests add handle/lifetime/restore guards, actual source-record equality,
complete array bijections and the multi-hole/two-component fixture.

The newer [native shell builder](native-shells.md) now attaches whole field faces,
uses checked zero-free planes for supported disconnected components, and assembles
actual native shells with topology checks.
Remaining work includes general component restriction policies when axis
separation fails, whole-boundary geometric validation and actual native solids.
Do not fabricate native faces from reference triangles or treat wire counts as
face-interior evidence.
Curved operands and all unfinished [Stage 4 gates](stage-4-acceptance.md) remain
required; the full goal is not complete.
