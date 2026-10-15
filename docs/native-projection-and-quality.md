# Native projection, surface probes and sampled quality

These are increments toward roadmap Stages 2 (usability) and 3 (reliability),
not completion of either stage. Mathematical fields remain authoritative;
neither projection nor diagnostics creates an OCCT B-Rep or modifies a document.

## Local surface projection

```python
import Implicit
result = Implicit.project(feature, (3, 1, 0), 1e-7, 1e-9, 64, 1e-5)
if result["converged"]:
    print(result["point"], result["distance"])
else:
    print(result["status"], result["residual"], result["stationarity"])
```

Positional arguments: native feature, local seed point, optional distance
tolerance (`1e-7`), field residual tolerance (`1e-9`), iteration limit (`64`),
and finite-difference gradient step (`1e-5`). Tolerances and step must be finite
and positive; iteration limit is `1..256`. The seed must lie in the effective
domain. Domain boundaries are not artificial cap surfaces.

The solver seeks a **local constrained stationary point** of distance to the
seed subject to `F=0`: `x-query + lambda*gradient(F) = 0`. It uses a numerical
Hessian of the native gradient, a row-scaled pivoted 4x4 KKT solve and at most
24 backtracking attempts per iteration. Spatial steps are bounded by the domain
diagonal, with the multiplier step scaled consistently. Accepted iterates stay
in the domain and reduce a combined field/stationarity merit. Hessian gradient
probes can lie outside the sampling domain; supported fields define their
equation there as well. The domain constrains accepted solutions, not the
underlying field equation.

Success requires all three: absolute field residual within field tolerance,
`abs(F)/|gradient(F)|` within distance tolerance, and stationarity norm within
distance tolerance. The gradient-scaled test avoids accepting a seed solely
because a poorly scaled field is numerically small; it is a first-order local
test, not a geometric error certificate. `distance`, `stationarity`, the step
and distance tolerance use local model units; residual tolerance uses field
units (dimensionless for a raw gyroid).

The dictionary contains `point`, `distance`, absolute `residual`, `stationarity`,
`iterations`, `converged` and `status`. Status is `converged`, `critical_point`,
`singular_system`, `line_search_failed` or `iteration_limit`. Invalid arguments
and field evaluation exceptions raise errors; ordinary solver failure returns
diagnostics with `converged=False`. Do not consume a failed iterate as a valid
projection. Even success does not prove a local minimum, global nearest point,
unique solution or signed distance. Hard CSG creases, critical points, poorly
conditioned fields, tolerance-clamped B-Rep distances and tiny features may fail.
Finite-difference step selection is caller-controlled, not automatically scaled.
Placement is not applied by this local API. The GIL is released during numeric
work after capturing the immutable compiled Shape.

## Viewport surface probe

In the **Native Implicit** workbench, select one native implicit feature in the
tree and choose **Probe selected implicit surface**. Left-click in the viewport
to query that field; right-click cancels. Results appear in the **Report view**
(`View > Panels > Report view`). The tool handles orthographic and perspective
rays and transforms through the feature's global placement. It queries the
field directly, even if the click does not hit its approximate display mesh.
It reports the first sampled ray root, including local/world coordinates,
bracket width and residual. Existing ray-sampling limitations still apply.

This is a one-shot inspection tool, **not replacement of FreeCAD's selection
system**: it creates no stable Face/Edge names, persistent point, snap reference
or subelement selection. It probes only the selected field, not all visible
objects or occlusion against other objects. It is synchronous and does not
accept cancellation during a query. Starting again removes the previous
callback; deleting the target or closing its document cancels a pending probe.
`ImplicitCommands.probe_ray` and `probe_view` are reusable GUI-side helpers.

## Sampled mesh-quality inspection

```python
report = Implicit.inspectTessellation(feature, 32, True)
print(report)
```

Arguments: feature, resolution (default 24), adaptive/pruned mode (default
False). Extraction limits remain `2..64`; pruned mode requires powers of two.
A fresh transient extraction is inspected without replacing viewport caches.
The GIL is released during extraction and inspection. The workbench's **Inspect
implicit mesh quality** command reports a fixed resolution-24 uniform result;
it does not silently reuse the current viewport resolution.

The report contains vertex/triangle counts, maximum absolute field residuals
at vertices, triangle centroids and unique edge midpoints, plus counts of:

- boundary edges: one incident triangle;
- non-manifold edges: more than two incident triangles;
- inconsistent edges: two triangles traversing the shared edge the same way;
- degenerate triangles: exactly zero computed cross-product length.

Interior residuals reveal curvature approximation that small vertex residuals
alone hide. They remain **sampled field-value errors**, not measured Hausdorff
distance, physical thickness or a bound over entire triangles. These topology
counts do not detect every vertex singularity, self-intersection, duplicate
sheet, near-degenerate triangle or missed component; consistent winding does
not certify outward orientation. Boundary edges may be expected for domain
clipping. An empty mesh has zero residual statistics, not proof that no surface
exists. No `valid_solid` or manufacturing approval flag is provided.

## Verification and remaining work

`geometry_smoke.py` tests deterministic random sphere projections against
analytic solutions, gyroid stationarity, field scaling, invalid arguments,
critical points, iteration limits and domain-constrained failures. It compares
coarse/fine sphere interior residuals, uniform/pruned equality, open clipping
and empty meshes. The Part regression covers projection on a live mixed field.
GUI tests cover placed world rays, orthographic/perspective pixels, probe
replacement/cancellation, deletion/document-close cleanup and inspection.

Still open: integrated semantic selection/snapping and editing UI; true
variable-resolution cells and transition topology; error-controlled refinement;
certified root completeness/tangencies; arbitrary chart/domain intersection;
robust physical distance and thickness. These tools expose and test errors;
they do not solve all those problems.
