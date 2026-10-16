# Whole-box field-to-linear topology witness

`BRepClass3d_ImplicitBox::CheckTopology` now checks a numerical sufficient
condition relating the complete box-clipped gyroid zero set to a transient,
shared nodal-linear reference. It is separate from trim-graph reproduction,
mesh proximity/contact checks and native shell/solid validation.

```python
import ImplicitOCCT
report = ImplicitOCCT.check_mixed_topology(boundary_handle)
# Or: ImplicitOCCT.check_box_topology(native_gyroid_face)
assert report["whole_box_homotopy_resolved"], report["reason"]
assert report["reference_manifold"]
assert not report["topology_validated"]
assert not report["solid_validated"]
```

Both adapters check the authoritative field's **whole box domain**. Stored trim
wires, cap regions, provenance flags and preview buffers are intentionally not
evidence. Only identity-placed primitive native gyroids are supported. Calls are
explicit/synchronous; the GUI does not run them automatically. Queries do not
modify source geometry or recertify stored seam metadata.

## Sufficient condition and reference

The domain has a common uniform grid with six conforming Freudenthal tetrahedra
per cell. F is sampled at shared grid vertices. L is the continuous piecewise
affine interpolant of those stored values, NOT a root-projected display field.
All grid vertex signs must be separated from the field precision reserve.

Consider H(t,x)=(1-t)F(x)+tL(x), for 0<=t<=1. For each full tetrahedron, and
each simplex on an actual exterior box face/edge, bounded longest-edge
subdivision must establish one of these conditions throughout every leaf:

- F and L have the same strict sign, excluding every homotopy zero; or
- with a the gradient of L restricted to the applicable box stratum and
  u=a/|a|, both a positive lower bound on |a| and
  grad(F)(center).u - HessianBound*radius - gradientReserve > 0.

The second condition means projected-grad(F).grad(L)>0 everywhere in that
leaf. The continuous projected field gradient is therefore an increasing
direction for F and **every incident affine piece** of L near possible H zeros.
In the interior, this also handles nondifferentiable L across artificial grid
faces/edges: no transversality to an artificial grid edge is required. Such a
requirement initially rejected harmless grid tangencies and was removed.

On exterior faces/edges the same argument uses the corresponding tangent
projection. At their intersections the positive-direction cones agree with the
lower-dimensional restrictions. Strict positivity is an open condition, so
local increasing directions can be extended and combined while remaining
tangent to the true boundary strata. The resulting zero-set deformation cannot
create or remove components/handles or change boundary incidence under these
sufficient conditions. This is the analytical rationale for the check, not a
claim of a formal machine-checked isotopy certificate: scalar, derivative,
linear-solve and coordinate bounds use **engineering floating reserves**, not
directed interval arithmetic. Limits or uncertain signs stay unresolved.

The reference zero set is extracted using affine edge roots, shared by grid-edge
identity, with deterministic tetrahedral triangles. No root projection, collar,
coordinate welding or native-face UV parameterization occurs. Paired oriented
interior edges, directed boundary cycles and connected path/cycle vertex links
check its combinatorial manifold structure. Components are counted using shared
incidences; Euler characteristics are computed from V-E+T per component.

This is not an implementation of Plantinga–Vegter. Their
[original isotopic approximation paper](https://www.math.rug.nl/~gert/documents/2004/pv-sgp-2004.pdf)
provides related background on why sign sampling alone is insufficient and why
global derivative information matters. Its interval-certified octree construction
must not be confused with this floating-reserve nodal-linear homotopy check.

## Results and limits

`whole_box_homotopy_resolved` means all sufficient-condition checks completed.
`reference_manifold` is a separate incidence/link check on the transient
reference. Partial buffers/counters on failure are diagnostic only.

`reference_vertices` and `reference_triangles` are zero-based local-coordinate
buffers; triangles follow the native face orientation (Common versus
BoxMinusField). `reference_boundary_loops` repeat the first index at the end.
`reference_component_euler` is sorted, not persistent component identity.
An empty zero set can resolve with empty buffers and a vacuous manifold result.
`minimum_homotopy_derivative` is the minimum accepted local monotone directional
lower bound in field/model-length units; it is zero for vacuous empty checks.
It is not a signed-distance bound or necessarily the global norm of the gradient.

`maximum_simplex_diameter` describes the reference grid only. The deformation is
not required to preserve individual artificial simplices, so this is **not**
a transport-distance/error bound. `geometry_accuracy_verified` remains False:
the reference is a topology witness, not a 0.1 mm production mesh. The separate
geometric tolerance policy and native conforming candidate are unchanged.

Resolution defaults to 32, a power of two from 4 through 64. Certification
subdivision defaults to depth 16, allowed 0 through 16. Total work defaults to
four million cells, allowed 1 through ten million. Work includes grid sign checks
and repeated incident boundary-simplex checks. Buffers are limited conservatively
to 200,000 vertices and 400,000 triangles. Exhaustion is unresolved, never success.
Dimension-zero counters describe all grid sign checks; dimensions 1/2 concern
actual box-edge/face restrictions and dimension 3 the full tetrahedra. Per-stratum
counters are work diagnostics, not independent completion flags. Unresolved
dimension/bounds identify the last failed cell. Unsupported types/placements or
invalid/precision-exhausted settings reject explicitly.

## Evidence and next gate

Five kernel cases cover both native orientations, open disk references,
enclosed sphere topology at resolutions 16 and 32, an empty reference, physical
scaling, source-record immutability, critical volume/face/edge/corner cases,
invalid representations and exhausted budgets. A near-maximum gyroid component
is deliberately hidden between resolution-four samples: all nodal values are
negative while the exact central maximum is positive. The checker refuses that
otherwise empty/manifold nodal reference.

The FreeCAD CLI checks both operations and open/closed trim fixtures, raw/mixed
adapter equality, independent buffer Euler/edge/loop accounting, native-property
snapshot lifetime, explicit codec restore and unchanged seam provenance. The
larger edge-crossing fixture has Euler characteristic -1 and three boundary
loops at resolutions 32 and 64. No native CAD solid is produced.

The newer [closed region reference](native-region-topology.md) now adds shared
planar caps, closed boundary components and retained-volume grouping.
Next connect this entire-surface witness to the **stored native trim/cap graph**
and its geometric realization, including component grouping/orientation and
shell construction. The existing graph checker compares to fresh planar traces,
not to this three-dimensional homotopy witness; simply combining True flags or
equal loop counts does not establish that missing correspondence. Curved
operands and unfinished [Stage 4 gates](stage-4-acceptance.md) remain required.
