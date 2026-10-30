# Native cylinder surface preview

Qualified full-disk cylinder bodies now have an opt-in surface preview with
native face, edge and vertex picking. The cap, cylinder side and implicit sheet
share native boundary samples. Render triangles remain transient; the saved
geometry is the authoritative field and actual native topology.

Given a qualified `Implicit::NativeCylinderFeature` from
[document geometry](native-cylinder-document.md), use the fork GUI console:

```python
from ImplicitCylinder import attach_view
renderer = attach_view(obj)
obj.ViewObject.PreviewResolution = 16  # 2..64
```

The existing gyroid toolbar is unchanged. This renderer supports the qualified
single-sheet full-disk cylinder family, not general partial/multiple gyroid
sheets. `PreviewState` explicitly reports that surface distance is unverified.
`SurfaceDistanceVerified` remains false.

## Geometry and accuracy contract

`Implicit.cylinderPreview(obj, resolution)` produces display buffers separately
from generic surface extraction. It retains the actual cap/field boundary
indices, triangulates the planar cap, joins the cylindrical strip, and solves
interior field-sheet nodes on polar rings. The polar coordinates are a display
chart, not a synthetic UV surface in OCCT. Shared seam indices, opposite two-use
mesh-edge incidence, used-node coverage and Euler characteristic are checked.
The graph construction connects both disks through the strip.

Fresh axial monotonicity and bracketed roots establish a spatial bound for
interior sheet nodes. Field residuals are not compared directly with millimetres.
The root point bound is at most min(.001, trim tolerance/64), with numerical
reserves. It does not certify triangle interiors or the stored boundary poles.
Analytic field gradients provide smoothly interpolated, material-oriented
normals; the side and cap use cylinder and planar normals.

Circle deflection is min(.01, trim tolerance/8, radius/(4*resolution**2)). Native
splines retain their stored spans. Source boundary approximation and binary32
upload displacement are reported separately. The latter must fit the source
tolerance, but no combined global surface-error certificate is claimed.
Generic `ComplexGeoData.getFaces` continues to refuse until that consumer
accuracy contract can be met. No triangles become CAD faces or Part shapes.

Limits are 250000 nodes, 500000 triangles, 2000000 root value tests and resolution
2..64. Unresolved roots, unsupported topology, degenerate triangles, bad incidence,
upload overflow or exceeded budgets refuse the preview. Construction is currently
synchronous; cancellable background preview/requalification remains required.

## Native selection and lifecycle

Rendered groups map to the actual native Face1..3, Edge1..4 and Vertex1..3.
`obj.getSubObject("Face3")` returns an owned native element geometry snapshot,
not the triangle that happened to be hit. Selection snapshots expose one
`Element1` and retain their source owner. Vertex bounds are point bounds;
other selected bounds are conservative cylinder enclosures. Nonidentity
placement and link paths are not qualified here; persistent edit-surviving
subelement history and broader consumer integration remain open.

The view clears buffers on unverified/empty geometry, hide or preview failure.
It rebuilds after qualification/show and stages new Coin nodes before publishing.
FCStd saves the graph and scalar view settings, while `dumps` excludes render
buffers. Reopen leaves the surface absent until the actual graph is requalified.
Undo/redo invalidates or rebuilds the preview with the corresponding owner.

Tests cover both signs, translated/reversed frames, independent root checks,
mesh incidence, positive preview orientation, analytic normals, unchanged source
records, native viewport hits and typed selection objects, failure/hide clearing,
save/reopen and undo/redo. The optional test environment variable
`IMPLICIT_CYLINDER_PREVIEW_SCREENSHOT` captures the actual GUI render. Visual
inspection caught an RGBA/RGB conversion error; material-value assertions now
guard that path too.


## Whole uploaded field-triangle directed distance

Milestone 103 adds `field_triangle_distance_verified`,
`field_triangle_distance_bound`, `field_triangles_checked`,
`field_triangle_reason` and `field_triangle_buffer` to cylinderPreview.
The buffer is binary32_upload: every input vertex is converted exactly as for
Coin. Each field triangle is contracted radially by a common affine factor to
keep its witnesses inside the finite cylinder. The contraction displacement is
included in the bound. A global gyroid Hessian bound controls the interpolation
residual over all barycentric points (H*diameter^2/6), and a fixed axial derivative
cone turns that residual into a spatial root bound. Both ends of the axial
witness prism must lie inside the actual implicit face domain. Convexity of the
qualified box/full-cylinder domain then covers all witnesses, including triangle
interiors. Numerical reserves are included. No sampling-only promotion occurs.

The bound is valid only when the subset flag is true. A failed sufficient test
reports a reason and a processed prefix, not evidence that the mesh is invalid.
The overall surface_distance_verified flag remains false: this directed subset
proof does not establish reverse coverage, cap/side accuracy or triangle contact
freedom. Tests exercise successful resolution-6 and unresolved resolution-2
cases, both material signs, translated/reversed frames and independent interior
root distances using binary32 vertices. Source topology and records are unchanged.


## Uploaded cap fan versus the retained mathematical disk

Milestone 104 adds cap_disk_distance_verified, cap_disk_distance_bound and
cap_disk_reason. The actual cap boundary order must have positive angular gaps
smaller than pi and wind exactly once about the source axis. An ideal inscribed
polygon uses the same radial directions at the qualified retained cap height.
The source fan and ideal fan have matching indexed triangles. Maximum vertex
perturbation (including the actual binary32 upload and numerical reserve) bounds
the Hausdorff distance of the two fans by barycentric correspondence. The disk
outside the ideal convex polygon is bounded by R*maximum_gap^2/8, with an angular
reserve. Their sum bounds both directions between the entire uploaded cap fan
and the mathematical retained disk. Failed sufficient checks leave the subset
unverified. This is distinct from field-face distance and does not establish a
whole-boundary certificate, triangle contact freedom or Stage 4 completion.
Independent point-to-triangle tests sample the disk interior and rim against the
actual uploaded fan, and fan interior samples against the mathematical disk.


## Reverse mathematical sheet-to-uploaded-triangle coverage

Milestone 105 adds sheet_reverse_distance_verified,
sheet_reverse_distance_bound, sheet_reverse_triangles_checked and
sheet_reverse_reason. A separate proof chart uses concentric uniformly scaled
rings of an inscribed polygon at radius R minus a numerical inset. Its angular
gaps are positive, smaller than pi and sum to one turn. The builder's matching
fan/annulus triangles tile that convex polygon; these coordinates are proof
witnesses, not substitute geometry or CAD topology. Reference vertices retain
the actual binary32 uploaded heights. Vertex membership in the actual convex
masked domain covers whole reference triangles.

The gyroid Hessian bounds the residual over each whole reference triangle.
Fresh axial uniqueness and the already qualified full-disk root existence give
vertical distance at most residual/m, where m is a whole-cylinder derivative
lower bound. Matching barycentric triangles add the maximum uploaded/reference
vertex displacement D. The disk portion outside the reference polygon has
planar deficit delta, bounded by radial inset plus circular sagitta. The true
root graph has slope at most L/m, so the full reverse bound is
D + delta + numerical_reserve + (maximum_residual + delta*L)/m.

OCCT's original CheckAxialField overload retains its ABI and zero-threshold
behavior. A new five-argument overload accepts a finite nonnegative minimum
absolute axial derivative. The same Hessian cell cover refines until every
accepted cell exceeds that threshold; unattainable requests, depth/cell budgets
and cancellation still refuse. The display audit derives the required threshold
from its actual residuals and the requested distance tolerance. It never replaces
a failed cover by a sampled slope estimate or weakens the target tolerance.

Both directed field flags and the cap flag are separate from the still-false
overall surface_distance_verified. Side distance and triangle contact checks
remain required. Reverse samples independently bisect mathematical roots over
the disk interior/rim and measure distance to the actual uploaded triangles.
Both material signs, translated/reversed and tilted frames pass. Coarse
resolution remains unresolved rather than promoted from vertex roots.


## Bidirectional uploaded cylinder-side distance

Milestone 106 adds side_distance_verified, side_distance_bound,
side_triangles_checked and side_distance_reason. The existing zipper records
matching unwrapped (angle,height) triangle witnesses. Both angular boundary
sequences must increase from zero to one full turn with gaps smaller than pi.
Field heights must lie strictly on the material side of the retained cap and
inside the finite cylinder height range. The construction triangulates the
strip between that cap and a piecewise-linear field-height graph.

For every entire uploaded triangle, matching barycentric points on the curved
cylinder lift differ by at most the maximum uploaded/reference vertex
perturbation plus R*angular_span^2/8 and numerical reserves. This uses the actual
binary32 coordinates, including the periodic seam endpoints. Along each lifted
field-height boundary interval, the composite field second derivative is bounded
by H*(R^2 + height_slope^2) + L*R. The whole-interval interpolation residual is
therefore bounded by endpoint residuals plus
(H*(R^2*delta_angle^2 + delta_height^2) + L*R*delta_angle^2)/8,
with reserves. A fresh full-cylinder axial lower bound m converts this to a
vertical distance to the unique mathematical field root. The required m is
derived from the remaining distance budget, using the existing requested-bound
OCCT overload when necessary.

At fixed angle, the reference and mathematical wall intervals share the cap
endpoint. Their Hausdorff distance is bounded by their field-height endpoint
difference, giving both directions over the entire wall. The final side bound
sums the cylinder-lift and field-height allowances. Failed sufficient checks
remain unverified. No source fields, topology or saved records are changed.

Independent tests sample mathematical wall interiors and both boundaries against
the actual uploaded triangles, and uploaded triangle interior points against
explicit valid mathematical wall witnesses. Both material signs and translated,
reversed and tilted frames pass. The overall surface_distance_verified remains
false: triangle contacts and the publication policy still need qualification.
This does not complete Stage 4 or enable generic OCCT consumers.
