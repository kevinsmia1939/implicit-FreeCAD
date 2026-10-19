# Checked native-solid display buffers

The OCCT-8 FreeCAD bridge adds `tessellate_mixed_checked`, and the live native
solid view adds opt-in `CheckSurfaceDisplay`. The default command and ordinary
unchecked candidate API are unchanged. This connects numerical clipped proximity
and exact triangle-contact checks to the actual transient display buffers,
without changing authoritative field definitions or native topology.

## Same-buffer and upload contract

Coin's `SoCoordinate3` stores binary32 coordinates. A binary64 mesh check alone
does not bound the resulting upload or rule out newly collapsed/contacting
triangles. The checked path therefore:

1. Extracts one native candidate, privately retaining its binary64 nodes.
2. Rounds those nodes to binary32 and bounds the largest displacement with a
   numerical reserve. Nonfinite/out-of-range/insufficient-precision uploads reject.
3. Checks both directed clipped-boundary distances of the original candidate
   against `requested_tolerance - upload_bound`.
4. Audits the rounded candidate with the existing exact binary64 predicates
   (binary32 coordinates embed exactly), shared-node rules and work limits.
5. Returns those rounded vertices and a `display_checks` report only if both
   checks finish successfully. Limits/unresolved contacts do not publish buffers.

The original and rounded triangle lists are identical. At corresponding convex
triangle points the displacement is no greater than the maximum vertex bound,
so adding that bound to both directed proximity results accounts for the upload.
This is still an engineering numerical bound, not directed interval arithmetic.
The contact audit checks actual rounded coordinates, not an epsilon-welded mesh.

`assignment_vertices` retains canonical pre-upload coordinates for unique native
component-face assignment. Display vertices must remain within the reported
upload bound of those points. Native face identity comes from actual component
domains and this canonical assignment, not triangle order or a new CAD face for
each triangle. Analytical field normals and separate cap normals remain display
attributes, not geometry definitions.

Reports are explicitly `source_local_binary32`. They do not bound world-placement
matrix rounding, camera/GPU transforms, pixels, antialiasing or driver behavior.
They do not validate native topology, solid validity, surviving-element names or
the component restriction policy. The source/solid owner retains those separate
contracts, including unverified restoration.

## API and view workflow

```python
import ImplicitOCCT as native
mesh = native.tessellate_mixed_checked(source.Geometry, .1)
report = mesh["display_checks"]
assert report["boundary_proximity_resolved"]
assert report["triangle_contacts_clear"]

# Existing live native solid; setting is opt-in and can take several seconds.
solid.ViewObject.CheckSurfaceDisplay = True
print(solid.ViewObject.DisplayProximityChecked)
print(solid.ViewObject.SurfacePreviewStatus)
```

The native positional arguments are handle, tolerance=.1, maximum_cells=256,
maximum_nodes=200000, maximum_triangles=400000, maximum_depth=14,
maximum_spatial_cells=2000000, maximum_triangle_patches=1000000,
maximum_pair_tests=2000000, maximum_broad_tests=50000000. Existing kernel bounds
and allowed budget ranges apply. This API is separate from the legacy candidate
API and the explicit coverage/audit APIs, which regenerate their own candidates.

`display_checks` reports requested tolerance, upload coordinate bound, both global
directed bounds, coverage/audit work counts and the coordinate frame. Topology
and solid-validation flags are false. The candidate's legacy
`surface_coverage_verified`/`solid_validated` flags stay false; the new report has
an explicit different contract. Its untrimmed-field bound includes upload error.

The view cache key includes the actual solid snapshot digest, surface tolerance,
extraction budget and checked-mode setting. Camera and edge/vertex visibility
changes reuse the buffers/report. Changed source/settings discard checked status.
Failure removes the field surface and report while keeping native geometry and
cap/edge/vertex display available, with an explicit failure status. Recompute can
retry a failed extraction after its cause is repaired; failure is not cached as
a permanently successful key. Disabled field display clears checked status.

FCStd stores the opt-in setting, not transient buffers or a reusable proof.
Restore generates and checks fresh buffers; it never upgrades the saved native
solid's independent unverified qualification flag. Checking remains synchronous;
whole-workflow scheduling/cancellation and broader editing qualification remain.

## Verification and remaining scope

CLI tests exercise both default operations, exact float-upload coordinates,
canonical connectivity/pre-upload nodes, work-budget failures, restored sources
and a strict box with non-binary32 cap coordinates. GUI tests inspect uploaded
`SoCoordinate3` values, semantic selection, cache reuse, injected extraction
failure/retry, changed inputs, operation undo/redo, display toggles and FCStd
restore without native-solid rebuilding. Results are recorded in PROGRESS.md.

This advances the native gyroid workflow, not completion of Stage 4. The default
Add Gyroid remains the older field display. Broader parameter/task editing,
curved conventional operands, snapping, surviving-element history and consumer/
placement qualification remain part of the active Stage 2–4 requirements.
