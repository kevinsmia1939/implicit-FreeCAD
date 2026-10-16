# Native mixed gyroid/box boundaries

This is a **native mixed-Boolean box boundary implementation**, not
general mixed BOP or a manufacturing solid. It modifies OCCT's `TKTopAlgo` and
FreeCAD's in-process Implicit module. The mathematical field remains authoritative.

Supported results now have a [transactional task editor](native-mixed-editor.md)
for operation, seam tolerance and tracing-budget changes, with preview, cancel,
undo/redo and save/reopen coverage. It does not turn the boundary into a solid.

An explicit [native cap/seam constraint API](native-mesh-constraints.md) prepares
shared indexed nodes and cap triangles. An opt-in
[trim-conforming surface candidate](native-conforming-preview.md) now joins the
implicit surface mesh to those same samples; it does not validate coverage,
self-intersections or native solids. Default preview remains caps/seams.

## Four goals and delivered scope

1. **Inside convention:** the primitive gyroid defines `F <= 0` as inside.
   `Common` retains the box intersected with that region. `BoxMinusField`
   explicitly retains the box where `F >= 0`; it is not field-minus-box.
2. **Intersection:** trace all six actual conventional box faces using the
   existing derivative-bound gyroid section algorithm. Successful seam
   polylines meet the requested **0.1 mm** default bound. Transverse box-edge
   roots are isolated once and shared by adjacent face contours. Unresolved
   cells, corner/grid degeneracies, tangencies and exhausted budgets reject.
3. **Hybrid boundary:** retain an exact bounded native implicit face, conventional
   planar cap faces and shared degree-1 B-spline seam edges. Planar p-curves have
   the same knots/parameters as the 3D seam. Caps pass conventional BRepCheck;
   every compound edge must have exactly two oppositely oriented uses. This is
   a necessary topology check, **not whole-solid validation**. Open contours
   join into closed implicit trim wires across faces; planar regions can have
   multiple holes or split into separate retained cap faces. A field face has
   no fake UV surface, and the output is a compound, never a TopoDS_Solid.
4. **FreeCAD integration:** OCCT-8-only API/commands, live Part/gyroid links,
   recompute and FCStd recipe restoration. Immutable native snapshots retain the
   hybrid topology in-process. Display shows **planar caps and orange seams
   only**, not the complete implicit boundary. Conforming surface meshing and
   automatic restoration without an explicit recompute remain unfinished.

## Exact support limits

- One valid, forward, conventional six-face axis-aligned box solid with eight
  corner vertices, twelve straight edges, one shell and one wire per face.
  Translation is supported; arbitrary rotations, curved solids, compounds and
  open faces are not. The box must fit wholly inside the field's query domain,
  and each dimension must exceed four requested tolerances.
- One identity-placed, forward, childless native primitive gyroid. FreeCAD's
  linked field must be `Implicit::Feature`, `Operation=Definition`,
  `Function=Gyroid`, empty FieldGraph, `InsideNegative=True`, identity global
  placement. Nonidentity parent-container placements and App::Link dispatch
  are unsupported; Part's own box placement is supported.
- Resolved open and closed contours are supported, including `IsoValue=0`
  gyroids crossing box edges. Canonical roots use Lipschitz/derivative-bound
  interval isolation, with root brackets <= tolerance/256. Each root must join
  exactly two face contours, through shared vertex identity, not coordinate
  welding. Near-corner ambiguity and tangential/critical edge cases reject.
- All six sections must resolve. Three quarters of the requested tolerance
  goes to face tracing; replacing face endpoints by canonical roots must move
  them by <= tolerance/8, and the combined seam bound is checked before publish.
  Polygon orientation and containment group outer loops and holes. Two-hole
  and disconnected two-disk cap cases are tested; arbitrary nested-region
  stress testing remains an acceptance gap. No general singularity resolution.
- Section tracing adds a Taylor empty-cell exclusion and exact harmonic
  edge-range/uniqueness tests on field-coordinate axes. A bounded set of
  alternative grids can be tried after dyadic refinement; every accepted cell
  still meets the same empty/regular-arc bounds. `resolution` can therefore be
  non-dyadic, but never exceeds MaximumCells. No geometry perturbation is used.
- The bounded implicit face may contain disconnected or empty zero-set regions.
  No component-count, nonempty-result or enclosed-solid claim follows from the
  returned face count or the edge-incidence checks.
- Numerical bounds use the existing floating-point/reserve policy, not directed
  interval arithmetic. The seam bound is **not** a gyroid surface mesh error,
  a whole-model Hausdorff certificate or a bound on planar-cap topology near
  singularities. Field values are dimensionless, not millimetres.

## Try it

Start the wholly OCCT-8-linked GUI:

```sh
bash tools/launch_demo.sh --occt8
```

Activate **Native Implicit**, then **Native gyroid/box demo (caps/seams only)**.
Or use the FreeCAD Python console:

```python
import ImplicitMixed
result = ImplicitMixed.demo()
result.Operation = "BoxMinusField"
App.ActiveDocument.recompute()
snapshot = result.Proxy.native_boundary()
# Standard zero-isovalue gyroid with open contours across box edges:
open_result = ImplicitMixed.demo(open_contours=True)
```

The demo creates real linked operands: period 10, iso 1.45, field domain
(-5,-5,-5)..(6,6,6), box (-0.31,-0.37,1.25)..(2.93,2.91,4).
Edit `result.Field.IsoValue` to 1.46 or the box height to 2.6 and recompute.
Select a supported primitive gyroid and Part box for either native mixed command.
The **Native zero-gyroid/box demo (caps/seams only)** command creates the open
fixture: box (-3.83,-4.07,-2.67)..(4.17,3.93,3.37), period 10, iso 0.
The older **Linked field intersection/difference** commands remain the more
general numerical field-space workflow; they are a different representation.

API without a document feature:

```python
import ImplicitOCCT as native
face = native.gyroid(10, 1.45, (-5,-5,-5), (6,6,6))
boundary = native.mixed_box(face, result.Box.Shape, "common", .1, 512)
```

The result has an opaque `boundary_handle` owning the whole shared-topology
compound, an independent immutable `implicit_face` handle, `planar_caps` (ordinary
Part faces), traversal-index `cap_sources`, seam point arrays, `seam_count`,
`seam_geometric_bound`, `box_edge_root_count`, and `solid_validated=False`.
Exported Part caps are copies;
their topology is not shared with the opaque compound. Do not sew them into a
purported implicit Part solid. `value/normal/in_domain` on `implicit_face` query
the original field and bounded surface domain, not the mixed region.
[Native region classification](native-region-classification.md) is available
separately through `native.classify_mixed(boundary_handle, point, .1)` or
`result.Proxy.classify(point, .1)`. It does not establish whole-solid validity.

Snapshots survive operand edits and document close. New mixed owners use a
[native geometry property](native-geometry-property.md): FCStd stores links,
settings and native shared topology, not capsules or generated meshes. Unchanged
inputs restore without numerical rebuilding. Legacy recipe owners migrate in
place with documented limitations. The [explicit shared-topology codec](native-topology-io.md)
now supports `native.dumps_mixed`/`loads_mixed` without recipe rebuilding;
restored seam bounds are marked unverified. Generic BRepTools/BinTools/STEP
serialization and primitive-only `native.dumps` of trimmed faces still reject.
Failures in execution clear
native, cap and seam display caches; no worker or mesh-solid fallback is used.
Calls are synchronous and hold the GIL. Saving a failed recipe does not make its
unsupported geometry supported on restore.

## Build and tests

No new packages or system-library modifications are needed. Reuse the builds:

```sh
cmake -S OCCT -B build-occt-implicit
cmake --build build-occt-implicit --target TKTopAlgo --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitScripts --parallel 2
cmake -S tests/occt_native_faces -B build-occt-implicit/native-face-tests
cmake --build build-occt-implicit/native-face-tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

These reconfigure commands assume the documented cached OCCT paths/options.
Do not rebuild a shared library while a runtime/linkage audit is using it:
Linux can report its old mapped inode as `(deleted)`, correctly failing the audit.
The preserved OCCT-7 build never loads this API; its compatibility path stages
only the pre-existing signed-distance extension header, not OCCT-8 headers.

The new kernel tests cover both operations, shared opposite seam uses, planar
p-curves/cap validity, bounded native queries, operand immutability, trim-I/O
rejection and invalid/unresolved operands. FreeCAD CLI/GUI tests cover links,
parameter/box edits, snapshot lifetime, operation switching, retained Coin
buffers, FCStd topology restoration without rebuilding and failure clearing.

Historical closed-loop baseline: 27/27 kernel cases, 22/22 OCCT-8 migration CTest entries (47.96 s),
25/25 preserved OCCT-7/root CTest entries (26.32 s). Core commits: OCCT
`459f71a3a9`, FreeCAD `dfb000b373`.

## Remaining gates

Current open-contour checkpoint: OCCT `ef20e3f9d7`, FreeCAD `1daddc4c3a`;
30/30 kernel cases, 22/22 migration tests (51.04 s), 25/25 compatibility tests
(29.80 s). The independent harmonic reference covers both Boolean directions.

Next: nested/degenerate topology stress tests, trim-conforming gyroid meshing,
implicit-aware solid validation/classification and full topology/history
persistence, plus curved B-Rep operands and general BOP dispatch. The active
[through-Stage-4 goal](stage-4-acceptance.md) includes broader property/persistence
coverage, selection/snapping, editing and cross-workbench gates. The supported
mixed property path is implemented, not all those integration requirements.
This increment does not complete Stage 2, 3 or 4 merely by enabling box crossings.
