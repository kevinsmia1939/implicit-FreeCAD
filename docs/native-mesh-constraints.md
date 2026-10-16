# Native mixed-boundary meshing constraints

The preparation API is a Stage 3 prerequisite, **not a mesh of the implicit
surface or a validated solid**. The mathematical field and native topology
remain authoritative. A newer explicit Tessellate API and opt-in GUI now consume
these constraints; see [trim-conforming preview](native-conforming-preview.md).

## Kernel contract

`TKMesh/BRepMesh_ImplicitBoundary::Prepare` accepts the supported
`BRepClass3d_ImplicitBox::Boundary`. It validates and privately copies that graph
through the bounded native topology codec, then meshes **only its conventional
planar caps**. It never sends an implicit face through OCCT's UV mesher.
The implementation belongs to TKMesh: adding mesh dependencies to TKTopAlgo
would introduce a toolkit dependency cycle.

A private meshing context preserves every native degree-1 B-spline knot and
subdivides each segment to spacing at most capDeflection/4. A conservative
preflight counts these additional samples as well as the original knots.
The ordinary chordal discretizer can drop short knots using the seam edge's
approximation tolerance; that tolerance must not erase connectivity constraints.
No general OCCT mesher behavior or source edge tolerance is changed.

Canonical node indices derive from shared native vertex identity and ordered
edge samples, not coordinate welding. Adjacent caps must agree on sample
parameters and canonical nodes. All native seam poles must occur in these
samples. Returned wires use effective native orientation and repeat the first
node at the end. Cap interior/box-edge incidences must be paired and opposite;
each native wire segment has exactly one opposite cap incidence. Unsupported
or inconsistent correspondence rejects the entire result.

The source graph, shape flags and triangulation caches remain untouched.
Outputs are disposable buffers; no triangulation is attached to the implicit
face or persisted. `NativeSurfaceMeshed` and `SolidValidated` are always false.
The seam geometric bound and verification flag preserve input provenance only;
stored/reloaded bounds remain unverified. Cap deflection is not a whole-surface
accuracy certificate, and seam bounds are not implicit triangle error bounds.

Defaults: cap deflection .1 mm, maximum 200,000 nodes / 400,000 triangles.
Deflection must be finite and at least 1e-7; nodes 3..1,000,000; triangles
1..2,000,000. A conservative sum of cap edge pole counts is checked before
meshing, followed by actual output limits. These bound the supported inputs and
outputs, not exact process allocation or time. Calls are synchronous without
cancellation; memory/scheduling refinements remain required.

## FreeCAD API (OCCT 8 build only)

```python
import ImplicitOCCT as native
constraints = native.prepare_mixed_mesh(obj.Geometry, .1, 200000, 400000)
```

The first argument is an immutable hybrid-boundary handle (including the typed
feature's `Geometry` property), not a Part shape, native single-face handle or
placed `ComplexGeoData` wrapper. Output keys:

- `vertices`: local `(x,y,z)` coordinates, millimetres.
- `cap_triangles`: `(a,b,c,cap_index)`; all indices zero-based, outward winding.
  Cap index follows `inspect_mixed(handle)["planar_caps"]`.
- `seams`: lists of vertex indices in native unique seam order, curve-forward.
- `wires`: lists of vertex indices in effective implicit wire orientation.
- `seam_geometric_bound`, `seam_bound_verified`: unchanged input provenance.
- `native_surface_meshed=False`, `solid_validated=False`.

Invalid settings raise `ValueError`, unsupported topology/exhausted budgets
raise `NotImplementedError`, and the wrong handle type raises `TypeError`.
No partially prepared result is published on failure. Results and retained
handles remain usable after document closure. This explicit preparation API
does not replace the current view provider's independent cap tessellation.

## Verification and next step

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests -R 'implicit_occt8_mixed_mesh$' --output-on-failure
```

Reuse existing configured builds. The isolated harness and OCCT build must be
configured again when adding the new TKMesh files/registered test. The preserved
OCCT 7 build refreshes only ImplicitScripts and continues rejecting public
ImplicitOCCT imports before loading an OCCT 8 toolkit.

Two new OCCT cases independently check outward cap normals, paired incidence,
all seam poles, both operations, open/closed contour fixtures, restored bound
provenance, budgets and source cache preservation. The FreeCAD
`mixed_mesh_smoke.py` additionally checks deterministic buffers, cap-index
mapping, property/capsule equivalence, independent Part cap caches, failure
atomicity and retained lifetime. These do not establish a complete surface mesh.

The native candidate now consumes these exact wire constraints/cap node IDs and
checks orientation, mesh closure and one-sided distance bounds. It still does
not prove coverage, embeddedness or native-solid validity. Those remain the next
qualification gates, along with curved operands, native history, snapping and
broader consumers in the active through-Stage-4 goal.
