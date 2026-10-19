# Corner-aware stored graph correspondence and cap-region attachment

The native correspondence and cap-region checkers now accept explicit
`CornerPolicy::Regularized` boundaries. This connects the stored native graph
and conventional cap regions to fresh field traces and the closed material
reference. It is NOT full trim-cycle/implicit-component attachment or native
shell/solid validation, and does not change the default workbench primitive.

## Chosen regularity policy

Strict `Reject` boundaries retain their existing box-stratified regularity gate.
For the corner policy, that zero-exclusion gate cannot apply at fixed zero
corners. Instead, the complete fixed-corner whole-box homotopy/surface checker
must resolve, including its volume/face/edge gradient checks and every incident
tetrahedron's inward corner signs. No near-zero snapping or field perturbation.

`field_box_regularity_resolved` in the correspondence result refers to the chosen
policy. The new `regularity_policy` explicitly distinguishes `strict_strata`
from `fixed_corner_homotopy`. The separate legacy regularity API remains strict;
it is not silently made to accept zero-valued corners.

## Stored native graph versus fresh native traces

The source is validated/private-copied through schema-2 I/O. Fresh native traces
are built from the same field, domain, tolerance and corner policy. Counts also
include the exact corner/contact metadata. Existing curve/vertex bijections,
complete directed cap/wire signatures, continuous polyline deviation bounds,
parameter-by-parameter planar p-curve checks and positive seam tangent alignment
remain required. These are not nearest-vertex welding or component-count tests.

The default cube matches 30 actual native edges, 19 shared vertices, 12 caps and
one implicit wire for both operations. This wire match is against fresh native
tracing. It is NOT the independent native-to-affine-reference trim-cycle and
whole implicit-component attachment required by the next gate.

## Native cap regions versus the closed reference

The cap checker builds the corner-aware closed material reference and uses its
ACTUAL resolution (31 for a requested 32). Planar interpolation uses the same
exact corner zeros as that reference, not rounded trigonometric residues.
Strict common F/L retained points must lie inside the corresponding conventional
native cap. Their clearance to all stored cap segments must exceed the fresh
seam, curve, p-curve and matching uncertainty budget. The grouping is bijective;
missing, ambiguous or budget-exhausted witnesses remain unresolved.

The default's 12 cap regions attach to one closed reference component and one
material region. Native arrays follow source-cap order, including reordered caps.
Queries preserve source topology/cache state and original/restored seam-bound
provenance. Fresh check reports never upgrade saved proof flags or replace source
geometry with the affine mesh. No schema or transient result-layout change.

## FreeCAD API and remaining guards

Existing `check_mixed_correspondence` now reports corner_policy and
regularity_policy. `check_mixed_cap_attachment` reports corner_policy and the
resolved reference_resolution (zero if no completed reference is published).
Restored snapshots/properties retain ownership after document close.

The [corner-aware trim-cycle extension](native-corner-trim.md) now distinguishes
shared box corners from incident edge-root anchors and matches full directed
cycles. The later [whole-face/native shell extension](native-corner-shells.md)
adds separate whole-component checks. [Cap geometry](native-corner-cap-geometry.md)
now has separate directed bounds, and [native solids](native-corner-solids.md)
use those checks separately. Mesh preparation/coverage remain guarded. Graph/cap/cycle reports
alone still leave native validity/construction flags false.

## Verification and next work

Kernel tests cover both operations, curve/vertex/cap/wire counts, actual native
cap classification, strict field witnesses, cap permutations, restored sources,
work/comparison/clearance limits and source immutability. The CLI independently
checks conventional Part edge distances, policy labels, source/restored property
lifetime and downstream refusals. Legacy strict paths retain their tests.

Corner-aware whole restricted faces and native shells now attach to the reference.
Next come geometric/native-solid
validation and default GUI exposure. Curved operands, surviving-element history,
snapping, richer editing and all unfinished Stage 2–4 requirements remain active.
