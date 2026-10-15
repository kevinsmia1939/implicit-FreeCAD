# Field-bound-controlled extraction

```python
result = Implicit.tessellateControlled(feature, 0.4, 4, 32)
print(result["status"], result["history"], result["quality"])
if not result["tolerance_met"]:
    print("Requested bound was not met")
```

Positional arguments are feature, positive finite **field-value** tolerance,
minimum resolution (default 8), maximum resolution (default 64), optional
cancellation token. Both resolutions must be powers of two in `2..64`, ordered.
The field must supply a finite conservative Lipschitz bound. Current B-Rep
distance fields return infinity and are rejected, even in mixed graphs. The
ordinary tessellation and sampled inspection APIs still accept them.

At each level the native extractor uses conservative octree pruning and a
common finest grid, then computes quality diagnostics. Resolutions double until
the target is met or the limit is reached. This is **global resolution
refinement**, not variable-size surface cells. The previous level's mesh is
released before extracting the next one. Existing million-element budgets still
apply and raise errors if exceeded. Cancellation is checked during extraction
and quality inspection; it cannot interrupt an individual field call.

For triangle centroid `c`, radius `r = max(|vertex-c|)` and conservative field
Lipschitz bound `L`, every triangle point `p` satisfies:

`|F(p)| <= |F(c)| + L*r`.

The maximum of this quantity over produced triangles, with a floating-point
allowance, is `quality.max_triangle_residual_bound`. It is also reported by
`inspectTessellation`; unsupported field bounds produce infinity there. This
is an analytic field bound evaluated in ordinary floating-point arithmetic,
not an interval-arithmetic or Hausdorff certificate. Gyroid field tolerances
are dimensionless. A small field value need not imply small geometric distance.

Results include the usual mesh arrays plus `quality`, `history` (resolution,
triangle count and bound per attempt), `tolerance_met`, `status` and
`surface_coverage_verified=False`. Status is `field_bound_met`,
`resolution_limit` or `empty_unverified`. A resolution-limit result includes the
last mesh for diagnostics, **not** silently accepted tolerance compliance. An
empty mesh never claims success and is refined through the maximum level.

The bound covers produced triangles only. It does not prove that all surface
components were sampled, prevent multiple roots within a cell, establish
watertightness or remove creases/self-intersections. It does not alter GUI
quality/cache settings. Coverage certification, variable-resolution transitions
and manufactured-solid validation remain separate acceptance gates.

`controlled_smoke.py` checks refinement success, full-triangle sample values
against the bound, limit failure, unverified empty meshes, gyroids, invalid
settings and cancellation. Existing quality and GUI regressions remain enabled.

## Scaling observation

`benchmark/controlled_sampling.py` reports three-run medians for extraction,
inspection and Python transfer at a requested field bound of 0.5, with grid
resolutions 8 through 32. The captured baseline is
[`controlled_sampling.csv`](../benchmark/controlled_sampling.csv).
The radius-2 sphere met the bound at 16 cells/axis (5.182 ms median). Gyroids
with 1, 3, 5 and 10 cells did not meet this conservative bound at the configured
32-cell cap; medians were 82.866, 267.981, 432.779 and 719.920 ms. This is evidence
of a refinement-limit shortfall, not an equal-accuracy speed comparison or proof
that actual geometric errors exceed the bound target. Gyroid tolerances here
are dimensionless; sphere and gyroid values are not physically comparable.

The 668.04 MiB peak RSS is the entire process's lifetime high-water mark and
carries between cases; it is not isolated mesh memory. This one-host baseline
does not measure GUI uploads, hardware FPS, GPU speed or production accuracy.
