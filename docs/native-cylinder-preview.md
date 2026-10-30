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
