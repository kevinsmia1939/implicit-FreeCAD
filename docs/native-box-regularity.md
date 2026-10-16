# Mesh-independent box-stratified field regularity

The OCCT kernel now checks regularity of the native gyroid zero set in its whole
box domain, including transversality on every box face/edge and exclusion of
corner zeros. This is a Stage 3 validation prerequisite, **not stored trim
correspondence, topology equivalence, reach or native-solid validity**.

```python
import ImplicitOCCT as native
report = native.check_mixed_regularity(obj.Geometry)
# For a primitive/owned face instead:
report = native.check_box_regularity(native_face_handle)
```

The kernel entry point is `BRepClass3d_ImplicitBox::CheckRegularity(face,
maximumDepth=14, maximumCells=2000000)` in TKTopAlgo. It does not depend on TKMesh,
triangulation, tracing-cache provenance or display buffers. It evaluates the
mathematical gyroid on the face's BOX domain; stored trim wires are deliberately
ignored. Only identity-placed base native gyroid faces are supported. Ordinary
Part faces, spheres, other native subtypes and placed face handles reject.
The mixed adapter obtains the owned native field face from its immutable
boundary/property snapshot. It does not change or promote the representation.

## Mathematical/numerical check

The volume (dimension 3), all six faces (2), twelve edges (1) and eight corners
(0) are checked independently, highest dimension first. Each stratum is
adaptively partitioned. For a cell center `p` and radius `r`, project the field
gradient onto that stratum's free coordinate directions. A cell can be:

- excluded from F=0 with the minimum of a Lipschitz variation bound and
  `|projected_gradient(p)|*r + H*r*r/2`, plus numerical reserves; or
- accepted as regular if `|projected_gradient(p)|-H*r-gradient_reserve` is
  positive and at least half the center projected-gradient magnitude.

Here `H=4*(2*pi/Period)^2` bounds the gyroid Hessian operator norm. The half-norm
criterion retains a useful quantitative lower bound rather than an arbitrarily
tiny positive roundoff margin. Corners have no free direction: their field
interval must exclude zero. Other cells subdivide until resolved or limited.
Equal-sign corners alone are never an empty-cell proof.

A nonzero FULL gradient does not rule out a face/edge tangency. The projected
gradient is essential: tests include a face-critical example with regular 3D
gradient, and an edge-critical example with regular volume and faces. The
separate corner test also has nonzero full/face/edge derivatives.

This uses floating-point engineering reserves and explicit finite/trigonometric
precision guards, **not directed interval arithmetic or exact transcendental
predicates**. The check does not resolve singularities, perturb geometry, trace
curves or guess topology. A limit or uncertain cell remains unresolved.

## API/report

Both Python functions accept optional `maximum_depth` and `maximum_cells`
positionally. Depth is 1..20 (default 14). The single cell budget across ALL
strata is 1..10,000,000 (default 2,000,000); it is not a per-stratum allowance.
Invalid settings/numerical ranges reject; exhausted budgets/depth return an
unresolved report. Calls are explicit and synchronous, without cancellation.

- `field_box_regularity_resolved`: True only after every stratum finishes.
- `strata`: dimension-indexed list (0..3), each with `complete`, `visited`,
  `excluded`, `regular_cells`, `minimum_projected_gradient`.
- Minimum derivative bounds apply to possible-zero cells in that stratum and
  are global only when its `complete` is True. Otherwise they are partial
  diagnostics. Zero means no regular potential-zero cell was accepted; it does
  not claim a singularity or existence/nonexistence of roots. Cell counts are
  NOT root/component counts.
- Derivatives have field-value/model-length units (normally F/mm); they are
  not distances. `hessian_bound` has F/model-length-squared units. Bounds scale
  with model length, as checked by scaled/periodically translated tests.
- `unresolved_dimension`, `unresolved_minimum`, `unresolved_maximum`: failed
  stratum/cell, meaningful only if unresolved. A false result may mean critical
  geometry, uncertainty or resource exhaustion, not a resolved singular point.
- `trim_correspondence_validated=False`, `topology_validated=False`,
  `solid_validated=False`, always.

The source, caches, geometry flags, GUI buffers, FCStd topology and stored seam
verification provenance stay unchanged. Restored native snapshots can be checked
without retracing, and retained handles remain queryable after document closure.
The ordinary GUI preview does not run or persist a validity claim automatically.

## Evidence and next step

Three new kernel cases check both Boolean directions, open/closed-contour box
domains, stored snapshots, source immutability, limits, independent analytic
volume/face/edge/corner critical cases, model-length scaling and periodic
translations. Full gradients remaining nonzero in the face/edge negative cases
do not bypass the stratum checks. Native CLI tests cover the same critical
dimensions, adapters, invalid/placed representations and snapshot lifetime.

This does not associate the stored polylines/wires with the certified regular
strata or prove a mesh/native topology is equivalent to the field. It does not
provide a feature-size/reach or isotopy certificate. Next establish that trim
correspondence and the required topology conditions, then construct/validate
native shells/solids with implicit-aware dispatch. Curved operands and the
unfinished Stage 4 selection/history/snapping/consumer gates remain required.
No dependencies or system libraries were installed or changed.

Final checkpoint: 48 kernel cases (36.31 seconds), 33/33 OCCT-8 migration tests
(171.99 seconds), 25/25 compatibility tests (67.86 seconds). These are regression
observations under shared host load, not performance claims.

Reproduce using existing builds:

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests -R 'implicit_occt8_mixed_regularity$' --output-on-failure
```

TKMesh builds the modified TKTopAlgo prerequisite. No reverse toolkit dependency
is introduced: regularity stays independent of meshing.
