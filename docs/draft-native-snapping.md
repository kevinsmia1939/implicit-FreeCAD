# Draft snapping on native implicit topology

Draft's object snap dispatch recognizes native mixed-boundary and native-solid
geometry properties before calling Part.getShape. It extracts the picked native
subobject through the complete parent/link path and asks the core for candidates.
Importing Draft alone does not load or require Implicit.

`Implicit.snapNative(selected_geometry, mode, cursor, coordinateTolerance=.1)`
accepts an owned selected NativeElementGeometry and returns coordinate tuples in
the selected path's frame. Supported modes are:

| Mode | Native support |
| --- | --- |
| Endpoint | Actual stored topological vertices; duplicate closed-edge endpoints appear once. |
| Midpoint | Half the arc length of the stored finite edge. |
| Near | Closest point on the stored finite edge's piecewise linear B-spline segments, clamped to their bounds. |

Seam aliases work because they refer to native edges. Endpoint snapping does not
use every curve pole as a vertex. Midpoint and Near do not use display triangles
or the cursor's unprojected hit position. Geometry calculations use local edge
coordinates and apply the qualified rigid path placement once. Finite input and
coordinate-scale checks reject precision exhaustion. Whole-region queries and
unknown modes fail explicitly. Unsupported face modes return no candidates;
there is no untrimmed implicit-face projection fallback.

These are picks on the stored native topology. Its seam approximation/provenance
remains distinct from exact field evaluation, solid validation and display
qualification. The coordinateTolerance protects coordinate resolution; it does
not recertify the seam or establish a new whole-model geometric error bound.
The native surface field remains authoritative, and no Part Shape or permanent
mesh representation is introduced.

The Draft adapter honors the existing Lock and Endpoint/Midpoint/Near switches,
snap winner selection, marker/cursor styles, constraints and optional WorkingPlane
projection. Native snapshots have their path placement already applied. Working
plane auto-alignment skips native ComplexGeoData rather than assuming a Part
ShapeType or invoking UV-based Part face alignment. Unavailable geometry, stale
mapped references and unsupported transforms produce no native candidates.
Existing conventional Part snapping retains its dispatch.

This is endpoint/edge snapping integration, not completion of snapping to native
implicit faces, cross-representation intersections, edit-surviving references,
or the full Stage 4 consumer contract.

## Build and test

Draft was previously disabled in the OCCT 8 build. Its CMake dependency checks
also require TechDraw, PartDesign, Spreadsheet and Import to be enabled. Sketcher
was enabled but its GUI library also needed building. The bundled lazy_loader
helper must be copied through its existing target; it is not the PyPI package
with the same name. Reuse installed dependencies and the existing build:

```sh
cmake -S FreeCAD -B build-freecad-occt8 -DBUILD_DRAFT=ON -DBUILD_TECHDRAW=ON \
  -DBUILD_PART_DESIGN=ON -DBUILD_SPREADSHEET=ON -DBUILD_IMPORT=ON
cmake --build build-freecad-occt8 --target Implicit Draft DraftUtils TechDrawGui SketcherGui --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests \
  -DIMPLICIT_MIGRATION_DRAFT_TESTS=ON
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests \
  -R '^implicit_occt8_(native_snap|gui_draft_snap)$' --output-on-failure
```

Draft's target now includes its runtime libraries and bundled helper, so building
`Implicit Draft` is sufficient once these flags are configured. The explicit
library targets above also document which dependencies were actually built.

Keep the existing one-link-job pool. Do not replace libraries during runtime
tests. The native snap CLI check covers stored vertices, bounded straight and
curved edges, selected-path placement, unsupported inputs, precision and snapshot
lifetime. The GUI check activates Draft, uses actual viewport picks and its
snapper, checks markers/working-plane behavior, creates a Draft Line from native
vertices, then saves and reopens it. Runtime success is recorded in HANDOFF and
PROGRESS after qualification; configured modules alone do not prove it.
