# Native cylinder graph persistence

`BRepClass3d_ImplicitCylinderIO` writes the actual qualified full-disk cylinder
solid. Schema `OCCT_IMPLICIT_CYLINDER_SOLID 1` retains three faces, four shared
edges and three shared vertices, including the cylinder's periodic seam uses.
The original authoritative gyroid and cylinder-restricted mathematical face are
separate explicit math records. No display mesh or callback is serialized.

```python
record = native.dumps_cylinder_solid(solid["solid_handle"])
loaded = native.loads_cylinder_solid(record)
assert loaded["topology_validated"]
assert loaded["requires_revalidation"]
assert not loaded["solid_validated"]
fresh = native.inspect_stored_cylinder_solid(loaded["stored_solid_handle"])
```

Use the fork's OCCT 8 runtime. The immutable stored owner has a distinct type
from a qualified solid owner. Classification, volume and qualified-solid export
reject it. Editing the report dictionary cannot alter that state.

The reader restores shared native handles, analytic curves and degree-one
splines/p-curves rather than retracing field contours. Binary64 coordinates use
17 significant digits. Structural checks cover effective face/edge incidence,
ordered wire joins, vertex links, face connectivity and the expected graph
counts; conventional cap/side faces also undergo ordinary face validation.
This is structural acceptance, not recertification of boundary geometry.

Stored boundary/volume numbers are provenance, never reusable certificates.
GeometryVerified, VolumeVerified and SolidValidated remain false after read.
Revalidation against the stored curves and authoritative field is still needed;
rebuilding a different boundary from the recipe would not validate this graph.

Resource limits are 16 MiB per record, 64 KiB per embedded math record and 65536
poles per supported spline. Parsing rejects unknown versions, nonfinite numbers,
bad counts, unsupported curves, malformed topology, truncation and trailing data.
Destination assignment occurs only after successful validation. Writing stages
the bounded record before output; an external stream failure can still leave
partial bytes in that stream, as with ordinary stream IO.

Tests cover both material signs, translated sources, original math-record
preservation, shared topology, dropped certificates, malformed versions/counts,
oversized records and atomic destination preservation. FreeCAD tests also check
owner isolation and refusal by qualified query APIs.

This API does not yet implement a cylinder document property, FCStd integration,
trim-conforming display or general OCCT consumer support. Those remain required
for the Stage 4 workflow.

## Stored curve consistency

`CheckCurveCoherence` / `check_stored_cylinder_curves(stored_solid_handle)`
audits the restored 3D edges against their conventional cap/cylinder p-curves.
It splits at every 3D and 2D spline knot. Each remaining interval has affine
spline pieces or analytic circle/line pieces. Constant speed bounds provide
a Lipschitz bound; second derivative bounds provide an interpolation remainder
of `M * width**2 / 8`. Adaptive subdivision checks the entire parameter interval
against the actual stored edge tolerance, with a floating-point reserve.
Cylinder composition includes both angular curvature and p-curve acceleration.

The default budget is 200000 intervals (hard maximum 2000000). Exhaustion,
unsupported curves or inconsistent geometry produce an unverified report with
a reason. This audit neither retraces the graph nor changes the stored owner.
It does not verify the field seam, material region or mathematical volume, and
does not promote GeometryVerified or SolidValidated. Those gates remain open.

## Stored seam proximity to primitive zeros

`CheckPrimitiveSeam` / `check_stored_cylinder_primitive_seam(handle)` checks every
point of the actual stored implicit-wire spline against the unrestricted gyroid
zero set. It partitions affine spline spans and bounds the complete segment's
field residual by endpoint interpolation plus the global gyroid Hessian bound.
The midpoint gradient defines a fixed direction. A Hessian cone bounds that
directional derivative away from zero throughout a neighbourhood containing
every segment point and a tolerance-length displacement in either direction.
The intermediate value theorem then gives a zero within `residual/lower` model
length, rather than treating dimensionless field residual as a distance.

World-space period/scale, source placements, floating reserves and explicit
interval limits are included. A global Lipschitz lower-distance test rejects
endpoints demonstrably outside the requested distance band. Unresolved or
budget-exhausted queries return an unverified report. Both material signs and
translated sources are tested; altered interior geometry rejects.

This is a directed bound to the unrestricted primitive zero set. The witness
root may lie outside the cylinder mask or source box. Cylinder-masked seam
correspondence, reverse coverage, retained material regions and solid/volume
requalification remain required. The query changes no stored-owner flags.

`CheckMaskedSeam` / `check_stored_cylinder_masked_seam(handle)` adds constrained
axial witnesses. Each affine segment is contracted radially by a bounded amount
to avoid roundoff putting analytic-cylinder endpoints outside the mask. That
displacement consumes part of the distance budget. A directional cone along
the cylinder axis bounds the remaining zero displacement. Both signs of that
axial displacement at both adjusted segment endpoints must belong to the actual
stored field face's domain. Its source box and finite cylinder are convex, so
these four checks contain every intermediate witness. Reports use
`masked_seam_verified/bound/intervals/reason`; no qualification flags change.

This constrained query proves the directed stored-seam-to-masked-zero distance
only. It does not establish reverse coverage of the mathematical intersection,
retained-region correspondence, volume or solid validity. Near-cap witnesses
that cannot fit the available axial bracket remain unverified.

## Reverse seam coverage

`CheckSeamCoverage` / `check_stored_cylinder_seam_coverage(handle)` bounds the
other direction: every zero on the full recipe-cylinder side must lie near the
stored seam. `CheckAxialField` reruns a pure whole-cylinder axial derivative
cover without constructing or retracing topology. Its positive derivative
bound proves at most one zero on each axial fibre. Its conservative cylinder
enclosure must fit inside the authoritative primitive's computational box.

The stored closed degree-one spline must wind once with strictly monotone
angular spans. Each span defines a reference circular arc with affine height;
the chord/arc deviation and endpoint radial errors are bounded. Gyroid Hessian
and Lipschitz bounds control the field residual over that whole arc. Global and
local axial derivative cones bracket a unique zero for every angle, inside the
finite height range. Angular winding covers the circle, so no other cylinder
side zeros can be missed. This audits the stored curves, without replacing them.

Reports use `seam_coverage_verified/bound/intervals/reason`; the interval budget
includes the derivative cover and arc checks. Unsupported angular reversals,
multiple winding, open curves, missing height brackets, distance overruns or
budget failures remain unverified. This sufficient full-disk path does not
qualify partial/multi-sheet geometry, retained conventional regions, caps,
material volume or solid validity. Stored-owner qualification flags stay false.

## Remaining stored-region acceptance

`CheckRetainedCap` / `check_stored_cylinder_retained_cap(handle)` now implements
the full-disk cap prerequisite. It reruns axial uniqueness and adaptive opposite
end-disk signs without trusting StoredAxialDirection. The selected material sign
determines which end is retained. The actual stored plane must match that end
and point outward; both stored analytic circle arcs must cover one consistently
oriented full circle with shared endpoints. Plane/circle tilt is bounded in
model length, including radius. Analytic coefficient differences bound complete
3D/p-curve arcs rather than a few samples. The bounded disk interior and stored
cap tolerance are checked. Reports use `retained_cap_verified/bound/intervals/reason`.
The bound describes geometric cap alignment; whole-disk field signs are a
separate logical gate. No loaded geometry/volume/solid flag is promoted.

The next gate must inspect the retained faces already in the graph. Rebuilding
new faces from source parameters would leave the loaded graph unverified.

`CheckRetainedSide` / `check_stored_cylinder_retained_side(handle)` now checks
the actual stored cylindrical material face. It reruns the cap/field,
curve-consistency and seam-coverage prerequisites with a shared work budget.
The cylinder frame/radius and outward orientation must match the recipe. Its
single periodic UV wire must have exactly one strictly monotone field graph,
two cap arcs, and the paired straight seams at the chart ends. Every graph pole
must lie inside the finite height range; cap direction, full angular coverage
and ordered UV joins select the material side determined by the fresh field
proof. This is a complete simple graph region, not a point-classification test.

For each stored spline span, the seam-coverage arc/root construction bounds
both its points against cylinder-side roots and every root against the span.
The curve-consistency bound transfers this correspondence to the stored UV
graph. Matching material-side orientation selects the corresponding region.
The combined distance includes seam, curve/surface consistency, frame alignment
and UV join reserves. It must fit the request and the implicit trim-face
tolerance. Cylinder surface alignment separately fits its own face tolerance;
the contour approximation is not charged to that exact surface's tolerance.
Reports use `retained_side_verified/bound/intervals/reason`. No flags are promoted.

- Completed cap prerequisite: prove opposite strict field signs over both complete end disks, with fresh
  adaptive bounds and axial uniqueness; do not trust StoredAxialDirection.
- Completed cap prerequisite: match the retained planar surface, circle geometry, full-disk trim and outward
  orientation to the correct material end. Check the actual stored cap, including
  the collinear split circle arcs and their shared vertices.
- Completed side prerequisite: match the conventional cylinder surface/frame and its actual periodic seam
  p-curves to the recipe. Establish which UV side of the field seam is retained,
  with material orientation and complete chart coverage rather than one witness.
- Recheck stored face/edge/vertex tolerances against fresh geometric bounds.
  Curve consistency and seam proximity are separate prerequisites.
- Combine both directions of stored-boundary geometry and material-region
  correspondence before any geometry/volume/solid qualification. Loaded numeric
  bounds remain provenance. Add wrong-cap/region/orientation and shifted-surface
  negative tests, plus translations, budget failures and unchanged-source checks.

Only after these gates should a restored owner become eligible for material
queries, bounded volume integration, document persistence and live GUI display.

## Owned restored geometry

`CheckGeometry` combines the fresh topology, cap/side material regions and
whole-boundary distance/tolerance checks for the unchanged stored graph. The
mathematical domain is derived from the actual native cylinder mask in world
space. The original source recipe and stored numeric bounds remain provenance;
they do not replace the geometry being audited. Every declared face/edge/vertex
tolerance is checked, including curve endpoints against vertex tolerances.

```python
geometry = native.qualify_stored_cylinder_geometry(loaded["stored_solid_handle"])
assert geometry["geometry_verified"], geometry["geometry_reason"]
assert not geometry["volume_verified"] and not geometry["solid_validated"]
fresh = native.inspect_stored_cylinder_geometry(geometry["stored_geometry_handle"])
```

The new immutable geometry owner shares the original actual solid graph; it
does not reconstruct topology or mutate the loaded owner's flags. The original
owner still reports GeometryVerified/VolumeVerified/SolidValidated false. Failed
qualification returns an inspectable failed geometry owner. Reports cannot
alter owner state. C++ callers must preserve the unchanged shared graph, as for
the existing owned construction contracts.

This establishes the explicit implicit-aware boundary geometry contract for
the supported full-disk path. It is not generic OCCT consumer support, volume
qualification or a valid-solid promotion. Restored geometry still needs a fresh
bounded material-volume/solid gate and representation-aware document/GUI wiring.
The existing construction-owner material APIs do not yet accept this owner.

## Fresh restored volume and solid qualification

```python
solid = native.qualify_stored_cylinder_solid(geometry["stored_geometry_handle"],
                                            volume_tolerance=.01)
assert solid["solid_validated"], solid["solid_reason"]
handle = solid["qualified_stored_solid_handle"]
volume = native.bound_stored_cylinder_volume(handle, volume_tolerance=.02)
record = native.dumps_cylinder_solid(handle)
loaded_again = native.loads_cylinder_solid(record)
assert not loaded_again["solid_validated"]
```

`BoundVolume` and `QualifySolid` consume unchanged verified stored geometry.
They share the construction path's private conservative disk/fibre integrator,
with adaptive area and Taylor root-height bounds. The requested absolute width
is in model length cubed, independent of the geometric trim tolerance. Saved
volume numbers are ignored. A bounded strictly positive volume strictly below
the full cylinder volume is required to qualify the solid. Coarse bounds can be
valid volume intervals and still fail that solid gate.

The qualified result retains the same TopoDS_Solid identity. No replacement
shell, mesh or fitted boundary is created. The original raw/geometry owner stays
unchanged; the new solid owner has independent immutable qualification. Failed
geometry/solid owners refuse volume queries. Late cancellation propagates before
the stored shape is published as qualified; no callbacks are retained.

`inspect_qualified_stored_cylinder_solid` does not rebuild. Both geometry and
qualified solid owners support repeat `bound_stored_cylinder_volume` queries.
The default volume settings remain .01 absolute width, depth 24, 200000 cells
and 10000000 root tests. Export accepts qualified restored solids using the
same bounded explicit schema with fresh numeric provenance; Read never restores
cached validity. This remains the implicit-aware contract, not ordinary OCCT
mass properties or generic consumer support. Dedicated document/GUI integration
and restored-owner material-point queries remain required.

The existing `NativeGeometry.h/.cpp` property implementation owns box-family
types. Cylinder document integration must add an explicit representation-aware
property/owner path. Restore should read the actual graph as unverified, then
schedule cancellable requalification and publish the immutable qualified owner
atomically. It must not perform expensive qualification inside FCStd parsing or
claim a ready display before qualification. Save uses explicit graph IO; display
caches remain transient. Semantic topology selection, trim-conforming display,
undo/redo, failure invalidation and save/reopen tests remain acceptance gates.
