# Experimental local B-spline bridge

This increment begins the manufacturing-bridge research milestone. It creates
an **open, local conventional B-spline face**, not a whole implicit B-Rep solid,
global atlas, native OCCT implicit face or manufacturing-ready model.

## Native chart sampling

```python
chart = Implicit.sampleChart(feature, (0, 0, 2), 1.2, 1.2, 0.75)
```

The example assumes a radius-2 sphere at the local origin. Arguments are native
feature, local seed, total chart width/height, half-depth for normal rays,
optional points per axis (9), ray segments (128), distance tolerance (`1e-9`),
minimum gradient/normal alignment (0.2), optional cancellation token.
Dimensions/tolerance must be positive finite; alignment is in `(0,1]`.
Points per axis are `4..33`, ray segments `2..4096`, and their product
`pointCount^2 * raySamples` cannot exceed 262144.

The seed is projected onto the field using native stationary projection. A
deterministic orthonormal tangent frame is built at that point. A rectangular
grid in its tangent plane is sampled by rays along the **fixed center normal**,
restricted to the depth interval and clipped to the field domain. Each ray must
have exactly one sampled root, and its normalized field gradient must align
with the center normal above the requested threshold. Missing/multiple roots,
critical points, failed seed projection and sampled normal folds raise errors;
no partial chart is returned. Shrink the patch/depth or choose a different seed.

The result contains 2D point rows, center, tangent axes, center normal, maximum
sampled field residual and minimum sampled normal alignment. Coordinates are
feature-local, independently of Placement and inside convention. Shape snapshots
are captured before releasing the GIL. Cancellation is checked between chart
rays; individual projection/ray field calls cannot be interrupted mid-call.

Uniqueness and regularity are **sampled checks**, not proofs. Ray sampling may
miss tangencies or thin components. A regular sampled grid does not guarantee
regularity between samples, global coverage or compatible neighboring charts.

## Fitting and validation

```python
import ImplicitPatch
result = ImplicitPatch.fit_patch(feature, (0, 0, 2), 1.2, 1.2, 0.75)
face = result["face"]
print(result["report"])
```

This installed native-module Python helper orchestrates the C++ chart sampler
and the existing Part/OCCT `BSplineSurface.interpolate` API. It does not import
the GUI or create document objects/files. Optional keyword arguments:

- `point_count=9`, `ray_samples=128`, `distance_tolerance=1e-9`,
  `minimum_alignment=0.2`, `cancellation_token=None`;
- `validation_count=17` (`5..65` per parameter axis);
- `field_tolerance=1e-5` (positive finite, **field units**, not millimetres);
- `normal_tolerance_degrees=2.0` (strictly between 0 and 90).

After OCCT interpolation it constructs the face with ordinary rectangular
parameter boundaries/p-curves and requires OCCT shape validity. It then samples
the fitted face over its full UV range, including edges: points must remain in
the source domain, field residuals must stay below the requested threshold,
normal/gradient angles below the requested threshold, and fitted normals must
retain the chart alignment. Failed checks raise errors rather than returning an
accepted face. Source/dependency dirty/error states and source revision changes
also reject the result. Numerical extraction is native C++; fitting is OCCT;
validation orchestration currently uses Python and native batch/scalar queries.

The report records source document/object/revision, local frame, seed,
dimensions, sampling and tolerance settings, measured residual/normal maxima,
and explicit `surface_coverage_verified=False` / `manufacturing_solid=False`.
This is provenance metadata, **not a serialized immutable field definition or
live document link**. Keep the original field/document; regenerate after edits.
Returned faces are static approximations in local coordinates. Applying global
placement or storing them in a separate Part object is an explicit caller step.

Cancellation can be polled through the new `Implicit.isCancelled(token)` API.
Fitting checks before/after OCCT interpolation and throughout validation; the
OCCT interpolation call itself is not interruptible. This is not a background
GUI manufacturing workflow.

## What STEP/Part support means here

The returned face genuinely contains a `Geom_BSplineSurface`, not triangle
faces or a mesh-only face. Tests clip it against an ordinary Part box using
OCCT Common, and export/import that **open face** through STEP, checking shape
validity, surface type and area. A caller can explicitly export the face with
`face.exportStep(path)`; no file is written automatically.

This does **not** provide arbitrary implicit-solid STEP export, a sewn/closed
gyroid, common boundaries between patches, a certified approximation or native
OCCT implicit-face Boolean dispatch. Sampled field/normal checks are not a
Hausdorff or topology certificate and STEP does not preserve the implicit graph.
Adjacent independently fitted patches must not be assumed sewable. General
atlas construction, shared curves/p-curves, gaps/error validation, solid sewing,
manufacturing checks and persistent conversion provenance remain open gates.

## Tests

`chart_smoke.py`: sphere/gyroid roots and normals, local placement semantics,
missing/ambiguous charts, critical seeds, resource limits and cancellation.
`patch_smoke.py`: valid B-spline face fitting, no hidden document mutations,
sampled field/normal checks, strict-tolerance rejection, open-face OCCT clipping,
STEP round-trip, gyroid fitting, cancellation and source-edit rejection. The
Part-dependent test is enabled with `IMPLICIT_NATIVE_PART_TESTS`.
