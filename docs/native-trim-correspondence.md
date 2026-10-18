# Stored native trim-graph correspondence

The OCCT-8 native module independently compares a stored mixed gyroid/box
boundary graph with a fresh derivative-bound reference trace. This is a
graph-level prerequisite for native-solid validation, not surface isotopy,
feature-size/reach certification or a valid CAD solid.
The [corner-aware extension](native-corner-attachment.md) now also accepts
explicit regularized boundaries using a fixed-corner homotopy precondition.
The strict zero-excluding policy remains unchanged; complete native-to-affine
trim-cycle/whole-face attachment is still a separate gate.

```python
import ImplicitOCCT
report = ImplicitOCCT.check_mixed_correspondence(boundary_handle, 512, 2000000)
assert report["stored_graph_correspondence_resolved"], report["reason"]
assert not report["topology_validated"]
assert not report["solid_validated"]
```

The call is explicit, synchronous and OCCT-8-only. The GUI does not automatically
run it. Supported sources are the topology codec's restricted identity-placed
primitive gyroid/axis-aligned-box representation, for Common and BoxMinusField.
Invalid representations/settings reject; tracing, pairing or numerical limits
produce an unresolved report. Source geometry, caches, stored accuracy provenance
and display buffers remain unchanged.

## Construction and checks

1. Validate and private-copy the source through the topology codec, then require
   whole-box stratified field regularity. Stored seam flags are not evidence.
2. Trace a fresh canonical graph from the authoritative field and exact box
   domain using the existing derivative-bound six-face tracing and shared
   box-edge roots. Build and this checker share canonical construction; no
   synthetic conventional box or TKPrim dependency is introduced.
3. Require equal graph counts and a unique edge bijection. Candidates must match
   seam/nonseam roles and endpoints, forward or reversed. Compare entire
   piecewise-linear curves under monotone normalized polygon-length parameters.
   On the union of parameter knots their difference is affine, so endpoint norms
   bound every interval, with numerical reserves. This permits collinear
   resampling, not merely identical serialized poles.
4. Induce a consistent bijection of shared vertices from edge endpoints. Compare
   conventional cap side/grouping and directed cyclic wire sequences, then the
   implicit face's directed wire cycles. Cap ordering and cyclic wire starts
   are irrelevant. No nearest-neighbor welding or repair occurs.
5. Check planar cap degree-1 p-curves, including range, knots, multiplicities
   and pole-by-pole lifting to 3D edges. Equal linear parameters bound entire
   segments rather than sparse samples. Require positive tangent-alignment
   lower bounds against the field/plane intersection direction on stored seams.

`matched_edges`, `matched_vertices`, `matched_caps` and `matched_wires` report
completed checks (wires means implicit trim wires; caps excludes the native
face). Partial counts are diagnostic only. `comparisons` and `sample_tests`
record bounded work. `minimum_tangent_alignment` has field/model-length units,
not distance units. Empty seam sets have a vacuous zero alignment report.

## Tolerances and limits

For M=max(1, absolute box coordinates), the engineering numerical reserve is
8192*epsilon*M. Matching tolerance is max(16*reserve, native tolerance/1048576).
This intentionally tight reproduction policy is separate from the user's
0.1 mm geometry target. It can conservatively reject a valid differently traced
graph, relocated closed-curve anchor or unsupported parameterization. It never
silently retraces and replaces the source.

Success requires `reference_seam_bound + curve_deviation_bound +
cap_parameter_deviation_bound <= native face tolerance`. The fresh reference
bound is separate metadata: a restored source's `seam_bound_verified` stays
False. Numerical reserves are not directed interval arithmetic.

Trace resolution must be a power of two from 4 through 512. Candidate comparisons
default to two million, with allowed range 1 through ten million. Curve and cap
parameter checks share an eight-million-sample budget. The regularity prerequisite
uses its existing depth-14/two-million-cell limits. Existing topology codec
size/geometry limits also apply. Exhaustion is unresolved, never success.

## Evidence and remaining work

Four kernel cases cover both operations, open/closed contours, multiple regions,
round-trip records, shuffled caps, resampled curves and cyclic wire starts.
Changed authoritative math with old wires/forged bounds fails correspondence.
A cap p-curve perturbation accepted by ordinary cap tolerance is rejected by the
stricter parameter check. Source immutability and low-budget failures are tested.
FreeCAD smoke coverage adds restore provenance, ownership after document close,
handle/settings guards and both operations/contour fixtures.

This compares an abstract oriented graph and its bounded geometric realization
to the fresh traced graph. It does not establish equivalence of the entire
implicit surface to a mesh, absence of thin handles, surface isotopy or native
shell/solid validity. Those remain required before promotion, alongside curved
operands and the unfinished [Stage 4 integration gates](stage-4-acceptance.md).
