# Native trim-conforming surface candidate

The OCCT 8 native mixed gyroid/box boundary now has an experimental surface
mesher and an opt-in FreeCAD preview. This advances Stage 3 surface meshing;
it does **not** validate a native solid, complete general mixed Booleans or
finish Stage 4. Fields/topology remain authoritative; mesh buffers are transient.

## Try it

Use `build-freecad-occt8/bin/FreeCAD`, not the installed or OCCT 7 executable.
In its Python console:

```python
import ImplicitMixed
obj = ImplicitMixed.demo(open_contours=True)
obj.ViewObject.ShowImplicitSurface = True
Gui.activeDocument().activeView().fitAll()
```

The View properties under **Experimental surface preview** are
`ShowImplicitSurface`, `SurfaceTolerance` (default .1 mm),
`SurfaceMaximumCells` (default 256) and read-only `SurfacePreviewStatus`.
The status explicitly distinguishes a closed mesh candidate from coverage,
embeddedness and native-solid validation. Default display remains caps/seams.

When enabled, cap triangles come from the same constrained mesh as the surface,
not independent Part tessellation. Native seam points and additional samples
are shared without distance welding. The surface is picked/highlighted as
`NativeFace1`, not a disposable triangle index. Existing cap/seam selection
remains available. Placement is applied by the feature's existing view path.

The preview now uses analytical field-gradient normals with per-vertex indexed
binding, replacing flat triangle lighting. Cap normals remain independent, so
cap/field creases stay crisp. This is display shading only, not a change to mesh
positions, tolerances, silhouette accuracy or any validation flag. Native boundary
edges/vertices are also selectable; see [selection contracts](native-edge-vertex-selection.md).

Settings/snapshot changes regenerate synchronously; ordinary camera changes
and repeated unchanged view updates reuse the cache. Failure clears the surface
and reports an explicit preview error, leaving valid authoritative topology and
the ordinary cap/seam preview intact. Native geometry failure clears everything.
FCStd saves settings/math/topology, never mesh buffers; restore rebuilds the
candidate. This can block the GUI while computing; cancellation/background
scheduling and global cache accounting remain open.

## Native API

```python
import ImplicitOCCT as native
mesh = native.tessellate_mixed(obj.Geometry, .1, 256, 200000, 400000)
```

The arguments are the immutable supported mixed-boundary handle, physical
tolerance, maximum longest-axis cells, maximum nodes and total triangles.
Single-face handles, Part shapes and placed ComplexGeoData wrappers reject.
The output extends `prepare_mixed_mesh`:

- `vertices`, `cap_triangles`, `seams`, `wires`: canonical local coordinates
  and zero-based indices, as documented in native-mesh-constraints.md.
- `implicit_triangles`: oriented triples indexing those SAME vertices.
- `implicit_normals`: aligned with `vertices`; oriented unit field normals at
  nodes referenced by implicit triangles, zero at unused cap-only nodes. These
  display normals are separate from cap normals and authoritative geometry.
- `native_surface_meshed=True`: a candidate exists, not a coverage certificate.
- `mesh_closed=True`: opposite paired edge uses and connected cyclic vertex links.
- `field_distance_bound`: triangle-wide, one-sided distance to the **untrimmed**
  gyroid, at most the requested tolerance under ordinary floating-point reserves.
- `surface_coverage_verified=False`, `solid_validated=False`, always.
- `cells`: accepted longest-axis grid resolution.

Seam-bound metadata preserves source provenance. Restored seam bounds remain
unverified; meshing does not re-certify them. The candidate independently checks
its triangle-to-untrimmed-field bound, not clipped-root or cap error. A mesh is
never assigned to `Part::Feature::Shape`, persisted as authoritative geometry,
or promoted to a native TopoDS_Solid.

## Algorithm and limits

`TKMesh/BRepMesh_ImplicitBoundary::Tessellate` starts from a validated private
cap/seam graph. Cap edge discretization retains every polyline knot and adds
samples with spacing at most a quarter of the cap deflection; a conservative
sample-count preflight bounds this work. The public preparation API uses the
same policy. It changes no source curve, mesh or tolerance.

Aspect-balanced Cartesian grids use consistent Freudenthal tetrahedra and
shared grid-edge root IDs. Brackets are refined to tolerance/4096 (with bounded
iterations); exact grid-node roots use canonical IDs. The boundary strip of
the extracted surface is discarded. Oriented interior loops must pair uniquely
with the stored native wires under a bounded correspondence gate. No points
are welded. A dynamic-programming collar joins each paired loop to the exact
native sample sequence. Shared-edge refinement projects only new interior nodes
back to the field, never moves or splits fixed native seam samples, and propagates
subdivisions to both adjacent triangles without hanging nodes. Final triangles
must be correctly oriented and distance-bounded. Ambiguous connectivity or
correspondence, empty/critical cases and
unresolved bounds reject atomically; finer dyadic grids are attempted.

Maximum cells must be 8..256; tried levels are 8,16,... up to the maximum.
At most 2,200,000 grid samples, 16,000,000 loop-matching sample pairs and
8,000,000 collar correspondence pairs are allowed. Existing node/triangle
limits and coordinate-precision guards also apply. These are input/output/work
limits. Projected refinement permits at most eight subdivision rounds with
bounded/backtracked local Newton solves (24 iterations/backtracks per solve).
These do not guarantee exact peak allocation or runtime. Valid but difficult
geometry can still fail at the budget.

For each triangle, let `d` be its longest edge, `r` its maximum vertex distance
from its centroid, `g` the centroid gradient magnitude, and `H=4*(2*pi/L)^2`
the global gyroid Hessian operator bound. Barycentric interpolation gives
`|F| <= max_vertex_residual + H*d*d/6`, plus a floating-point reserve. Along
the fixed centroid-gradient direction, the derivative is initially at least
`g-H*r` and can decrease at most `H*s`. A positive quadratic witness interval
therefore bounds triangle-to-zero-set distance. Failed discriminants, wrong
orientation or a distance above tolerance reject. This is not directed interval
arithmetic and is not a two-sided Hausdorff or coverage proof. Witnesses may
leave the clipped box: their bound is explicitly to the untrimmed field.

Grid sign sampling can miss components/multiple roots. Local orientation and
combinatorial closure do not exclude triangle self-intersections, an incorrect
global collar, missed sheets or clipped-domain discrepancies. The public flags,
GUI status and remaining roadmap retain these gaps; no manufacturing/solid
consumer should infer validity from `mesh_closed`.

## Verification and next gates

An additional [whole-domain clipped-boundary proximity check](native-clipped-coverage.md)
now validates both directed distances to the mathematical mixed boundary, with
spatial partitions and clipped witnesses. It is explicit, uses floating reserves,
and does not establish topology equivalence or native-solid validity. The plain
preview still does not run these checks or set a validation flag.

A separate [exact triangle contact audit](native-mesh-contact-audit.md) is now
available through `ImplicitOCCT.audit_mixed_mesh`. Clear reports prove absence
of improper stored-coordinate contacts for that privately generated candidate,
not field coverage or native-solid validity. The preview does not automatically
run this audit, and its existing unverified status remains intentional.

Rebuild the existing TKMesh and native test targets, then the OCCT-8
`_ImplicitOCCT` / `ImplicitScripts` targets. The preserved OCCT-7 tree only
refreshes `ImplicitScripts`. Run:

```sh
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

Final checkpoint: 40 kernel cases (12.11 seconds), 30/30 migration tests
(96.24 seconds), 25/25 compatibility tests (48.58 seconds). These are regression
observations, not a performance benchmark or broad kernel/workbench qualification.

Kernel tests independently inspect shared seam IDs, preserved source caches,
outward orientation, paired incidence, sampled root witnesses, both Boolean
directions, open/closed contours, multiple loops, physical scale/translation,
stored provenance and budget failures. CLI tests cover deterministic buffers,
property/capsule equivalence and retained lifetime. GUI tests exercise actual
native-face ray picking/detail paths, cache reuse, failure recovery, operation
edits, FCStd reconstruction and native-geometry failure invalidation.

Next: prove/resolve coverage and clipped-boundary consistency, check embeddedness
and develop implicit-aware solid validation before claiming valid solids.
Curved B-Rep operands, broader history/snapping and cross-workbench consumers
remain required for the active through-Stage-4 goal.
