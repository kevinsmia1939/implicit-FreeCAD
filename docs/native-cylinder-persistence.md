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
