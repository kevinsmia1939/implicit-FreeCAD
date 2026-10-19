# Native implicit surface domain exclusions

The regularized default gyroid now has an actual native face-domain restriction,
not merely missing display triangles. `BRep_TImplicitFace` owns immutable closed
axis-aligned exclusion boxes. `BRep_ImplicitTool::IsInDomain` transforms query
points into the face-local frame and rejects these boxes; `IntersectRay` uses
the same predicate before publishing hits. Empty copies retain restrictions.
Value and Gradient intentionally evaluate the authoritative underlying field;
IsInDomain is a domain predicate, not an on-surface test or distance certificate.

## Default gyroid and proof scope

`SurfaceCornerExclusions` checks the existing local lattice identities and
inward coordinate derivative bounds. A uniform positive or negative cone has
no zero in its checked neighborhood except its anchor. Removing that entire
closed neighborhood therefore removes only an isolated zero contact from the
zero set, never a two-dimensional surface patch. The mathematical source field,
outer box, phase and 0.1 mm tolerance remain unchanged.

Both uniform signs are excluded from the implicit face, for both Common and
BoxMinusField. The retained uniform corner still belongs to the planar caps;
it is not a point of the implicit surface face. Six split corners remain shared
trim vertices. This is different from material regularization, which excludes
only one of the two isolated closed-set contacts from each material region.

The generic storage constructor merely records restrictions; arbitrary caller-
supplied boxes have no proof attached. Mixed topology validation requires the
stored boxes to equal fresh, canonical checked corner exclusions. Existing
whole-face attachment and native shells are now supported by the later
[corner shell extension](native-corner-shells.md). [Native solid qualification/I/O](native-corner-solids.md)
now integrates fresh topology and cap bounds. This domain change alone is not a validated
default solid, generic OCCT Boolean support, or Stage 4 completion.

## Persistence and consumers

Unrestricted primitive output stays schema 1. Restricted primitive output uses
schema 2 with EXCLUDED_DOMAINS, finite positive-extent boxes inside the outer
domain, and a maximum of 64 boxes. Readers accept both versions, preserve
geometry/instance placements, and reject invalid input before replacing the
destination. These records are still the explicit implicit codec, not generic
BRepTools serialization of implicit faces.

The mixed topology envelope remains schema 2 for regularized boundaries and
now embeds the restricted primitive record. Legacy schema-2 mixed records with
an unrestricted primitive migrate by deriving fresh local corner exclusions;
stored wires/caps are not regenerated and restored seam bounds remain unverified.
The writer preserves restrictions and checks them against the local math.

Sphere operations/analytic meshing, plane-section tracing, gyroid splitting and
new mixed-Boolean input paths that cannot honor arbitrary exclusions reject them
explicitly. Whole-field mathematical reference queries intentionally still use
the source field. No unsupported consumer is made implicit-aware by this change.

FreeCAD's mixed snapshot API exposes `surface_domain_exclusion_count` and its
owned `implicit_face` supports the existing in_domain/translated queries. FCStd
continues to use the owned native geometry property and mixed topology codec.
Migration changes canonical serialized bytes. NativeGeometry's existing
whole-snapshot digest naming therefore changes too; this is not surviving-
element history or a promise that legacy mapped LinkSub names survive migration.

## Verification and next gate

Kernel tests cover domain/ray rejection, unchanged field evaluation, empty copies,
placed primitive round trips, invalid boxes, both mixed operations, all eight
physical corners, current and legacy mixed restore, and rejected re-use as an
untrimmed operand. FreeCAD tests query both isolated contacts and retained split
corners on source/restored/translated faces and owned properties.

Whole restricted-face attachment/native shells and [qualified native solids](native-corner-solids.md)
now exist. Next implement corner-aware mesh/view/live workflow qualification and default GUI
exposure. Curved operands, snapping, history, richer editing and cross-workbench
consumer requirements remain part of the full Stage 2–4 goal.
