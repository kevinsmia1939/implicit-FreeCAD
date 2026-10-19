# Corner-aware clipped-boundary proximity

`BRepMesh_ImplicitBoundary::CheckCoverage` and FreeCAD's explicit
`ImplicitOCCT.check_mixed_coverage(handle)` now support the Regularized corner
policy. This extends the [existing proximity checker](native-clipped-coverage.md)
to the unchanged default 10-mm gyroid/box Common and BoxMinusField cases at the
0.1-mm target. It does not change the field, level, domain, native topology,
saved proof flags or default workbench command.

## Forward coverage

The checker still partitions the whole volume and each physical box side.
Ordinary Lipschitz/Taylor exclusions and explicit triangle-distance upper bounds
are unchanged. A fresh checked corner model supplies exact lattice anchors and
derivative information, independently of mesh metadata.

For a candidate cell and exact corner, the global Hessian bound limits derivative
variation over the convex hull of the anchor and cell. If all three inward
derivatives have the same strict sign throughout that hull, its only possible
zero is the isolated anchor. Neither sign's isolated anchor belongs to the
restricted 2-D field face, so that field cell can be excluded. This is not
same-sign-corner pruning or omission of a small feature based on sampling.

On a cap plane, only the two in-plane inward derivatives are used. Both must
point strictly into unretained material to exclude the cell from closure of
retained side area. A retained uniform corner remains part of the caps even
though it is not part of the implicit face. Split corners remain part of the
appropriate field/cap closures. Unproved cases subdivide or return unresolved.

## Reverse proximity

Every triangle patch still needs a true boundary witness and a whole-patch
distance bound. The normal-line root slab must stay inside the source box and
be strictly disjoint, with reserves, from every canonical field-face exclusion.
An untrimmed field root in an excluded neighborhood cannot qualify that slab.

Cap witnesses include exact corner anchors only when a checked strictly retained
in-plane ray proves membership in retained side closure. Raw F=0 is insufficient.
Existing strict retained-side samples and regularized region-classifier witnesses
remain available. These witnesses bound distance to the union of the mathematical
field/cap boundary, not a separate per-triangle face-membership certificate.

## API diagnostics and invariants

Arguments, defaults and work limits are unchanged. Additional counters are:

- `regularized_field_cells`: field cells excluded by checked uniform cones.
- `regularized_cap_cells`: cap cells excluded by checked unretained side cones.
- `corner_witnesses`: exact retained-side corner witnesses improving a search.
- `exclusion_slab_rejections`: normal-slab/exclusion overlaps rejecting that
  particular sufficient witness; another witness or subdivision may still work.

Only a resolved report gives global directed upper bounds. Partial counters and
bounds are diagnostics, not success. Independent candidate extraction continues
to report `surface_coverage_verified=False`: running this separate checker does
not mutate buffers or attach a stale certificate to a future recomputation.
Topology/solid validation and contact-audit flags remain false in this report.
Numerical reserves are engineering bounds, not directed interval arithmetic.

Kernel tests cover both default operations, independent harmonic roots and cap
samples, retained cap-only corners, shifted/scaled restored sources, immutability,
missing surfaces/caps, forged flags, extraneous field and cap triangles near
excluded contacts, and depth/cell/patch exhaustion. FreeCAD runtime tests cover
both operations at periods 10 and 5 (including translation), restored ownership
and unchanged seam-bound provenance. Full regression results are in PROGRESS.md.

The enlarged kernel test harness has a 300-second wall-clock timeout (previously
240); this accommodates added tests, not increased geometry-check budgets.

## Next integration gates

The explicit proximity API does not yet qualify every parameter change or certify
the GUI's current buffers automatically. Broader source/task editing, failure
invalidation and default native-workbench exposure remain required. Curved B-Rep
operands, snapping, surviving-element history and cross-workbench consumers remain
part of the full Stage 2–4 goal. No generic Part shape or UV-based BOP claim follows.

For the next GUI increment, check the same native `Result` that supplies display
buffers before publishing them, and bind the report to the source/solid snapshot,
tolerance and extraction settings. Merely calling the current explicit API
regenerates a separate candidate. A successful old report must not survive changed
inputs, failed extraction, restore to a different snapshot or preview-cache
replacement. Keep proximity, exact contact auditing, native-solid qualification
and edit-stable naming as distinct contracts.
