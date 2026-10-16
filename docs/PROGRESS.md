# Implementation progress and remaining work

Updated 2026-10-16. The user has authorized continuing the full roadmap,
including FreeCAD/OCCT core modifications, commits and pushes. This log records
concrete increments; the overall nTop-class goal remains a substantial project.

Implementation heads: FreeCAD `e88c92efaa` on
`feature/native-implicit`, OCCT `aa6ced5047` on `feature/implicit-distance`.
The parent repository pins both commits.

## Latest increment: placed native mathematical-region queries

`Implicit.classifyNative` accepts whole local/pre-placed native ComplexGeoData,
not Part shapes or selected boundary elements. It reuses the native classifier
with placement-aware separation/witness bounds, inverse/forward error reserves
and near-rigid stretch bounds. Exhausted placement precision returns unresolved;
solid_validated is always False. Container/link snapshot and GUI instance paths
now have mathematical-region queries independent of preview meshes.

The local budget reserves endpoint-transform errors for /256 witness brackets;
the first placed gyroid-root check exposed the need for that reserve and now
passes. Tests independently bound known cap distances and check transformed
witness locations, root signs/domain, stationary unresolved cases, both operations,
restore, retained snapshots, invalid/selected representations and extreme
coordinates. Nested tests use static snapshots: grouping live mixed recipes can
move their inputs outside the unchanged identity-placement kernel contract.
This does not implement general placed-input Booleans, conforming surfaces,
validated solids or the remaining Stage 4 integration gates.

Both incremental builds succeed. Full OCCT-8 migration passes **27/27**
(55.33 seconds), preserved compatibility **25/25** (28.21 seconds), including
unchanged **35** kernel cases. Additional direct CLI analytic/witness checks pass.
Logs: `/tmp/implicit-placed-query-build-final.log`,
`/tmp/implicit-placed-query-legacy-build.log`, `/tmp/implicit-placed-query-cli-final.log`,
`/tmp/implicit-placed-query-migration.log`, `/tmp/implicit-placed-query-compatibility.log`.
No OCCT source, new dependencies or system libraries changed.

## Previous increment: whole native geometry and rigid App::Link extraction

Empty typed-owner getSubObject references now return independently owned
NativePlacedGeometry/ComplexGeoData, retaining NativeFace/Cap/Seam counts/maps,
boundary lines and conservative full-domain bounds. Null properties return None;
rigid placement validation and domain-bound transformation are shared with the
selected-element path. Surface meshing/nonrigid queries still reject.

New native_link_smoke covers both LinkTransform modes and all subelements,
whole boundaries, Part-to-link paths, mapped cap links, same-document FCStd,
retained snapshots, scaled extraction rejection and source clearing. External
whole/cap queries, instance-file reopen with the source already loaded and source
map immutability under core link re-tagging are tested. GUI tests require an
actual native cap ray hit on a placed link and independently placed selected/
whole geometry. Whole selections follow normal FreeCAD semantics: SubObjects is
empty with no selected subelement names; use selected Object.getSubObject("").

Automatic external dependency loading, missing/moved files, arrays, all instance/
consumer modes and placed operands in kernel Booleans remain unqualified. This
is not full Stage 4 completion; all unfinished Stage 2/3/4 requirements remain.
No OCCT source, packages or system libraries changed. Both incremental builds
succeed. Logs: `/tmp/implicit-link-build.log`, `/tmp/implicit-link-legacy-build.log`,
`/tmp/implicit-link-cli-final.log`, `/tmp/implicit-link-targeted.log`,
`/tmp/implicit-link-migration-final.log`, `/tmp/implicit-link-compatibility.log`.
Two initial test assumptions were corrected: whole selections normally have
empty SubObjects, and an external link requires a saved owner document before
setting its target. No shared App::Link/selection behavior was changed.
The preserved compatibility suite passes **25/25** (37.32 seconds), including
unchanged **35** kernel cases.
Final OCCT-8 migration passes **26/26** (52.25 seconds), including the corrected
external-link fixture and strengthened GUI instance checks.

## Previous increment: owned native subobject extraction

Typed native getSubObject now returns C++ NativeElementGeometry through the
standard ComplexGeoData Python interface. GUI SelectionEx.SubObjects exposes
native selected caps/seams, not None or Part shapes. Each result owns its
segment and authoritative snapshot, exposes Element1 with the original mapped
identity, boundary lines and bounds, and remains usable after document close.
Native face bounds use all transformed finite-domain corners, not trim extrema.

Queries are pre-placed exactly once using the accumulated incoming matrix and
optional owner Placement; the result's geometry transform is identity. Tests
compare every native fixture subelement against independent rotated/translated
expectations, transform=False, nested App::Part paths and incoming composition.
Unsupported nonrigid/nonfinite/projective matrices, surface/point sampling,
geometry mutation, independent persistence and Part/native-property assignments
reject. The C++ first-boundary-vertex hook follows the same placement; integrated
snapping and general App::Link/container/workbench qualification remain open.
All unfinished Stage 2/3/4 requirements remain in scope; no solid or manufacturing
accuracy claim follows. No OCCT source, system libraries or dependencies changed.

Both incremental builds succeed. Full migration passes **25/25** (51.79 seconds).
The final direct element test also verifies conservative domain bounds and
incoming matrix composition. Logs: `/tmp/implicit-element-build-final.log`,
`/tmp/implicit-element-legacy-build.log`, `/tmp/implicit-element-migration.log`,
`/tmp/implicit-element-cli-final.log`, `/tmp/implicit-element-compatibility.log`.
Preserved compatibility passes **25/25** (27.54 seconds), including unchanged
**35** isolated kernel cases.

## Previous increment: mapped native references and explicit stale attachments

Native geometry now reconstructs standard FreeCAD element maps from a SHA-256
digest of the stored mesh-free topology record plus native index/type. Typed
owners validate native subobject lookups; current mixed GUI mapped paths resolve,
while stale paths reject instead of falling back to their trailing CapN index.
No separate map payload or OCCT/property schema change is needed.

New native-reference tests cover all mapped subelements on the multi-face
crossing fixture, unchanged FCStd restoration, recursive copy of a static linked
snapshot, real LinkSub/LinkSubList invalidation, saving/reopening unresolved
references, undo/redo, cleared geometry and exact topology recovery. Negative
cases intentionally emit missing-element warnings. Native Python subobject
extraction remains None, not a Part shape; nativeGeometry provides explicit
representation-aware inspection.

This is conservative whole-snapshot invalidation, NOT surviving-element Boolean
history. Even local changes can invalidate all mapped references. Plain indices,
legacy owners, external-document/container/placement references and broader
consumers remain limited. Snapping, conforming meshes, validated solids, curved
operands and every other unfinished Stage 2/3/4 gate remain in scope. No OCCT
source, dependencies or system libraries changed.

Both native incremental builds succeed; the initial full migration run passes
**24/24** (49.96 seconds), compatibility **25/25** (27.85 seconds), including
unchanged **35** kernel cases. Logs: `/tmp/implicit-reference-build.log`,
`/tmp/implicit-reference-legacy-build.log`, `/tmp/implicit-reference-targeted.log`,
`/tmp/implicit-reference-cli.log`, `/tmp/implicit-reference-migration.log`,
`/tmp/implicit-reference-compatibility.log`. The test harness initially reopened
the already-open file rather than an independent document; using a distinct
copied fixture fixed that test-lifetime issue, without changing CAD behavior.
Final migration rerun including LinkSubList and null-output checks passes
**24/24** (49.60 seconds; `/tmp/implicit-reference-migration-final.log`).

## Previous increment: native snapshot subelements and cap/seam picking

The native ComplexGeoData representation now returns owned one-based
NativeFace/Cap/Seam segments, boundary lines and a first boundary vertex for
native consumers. `Implicit.nativeGeometry(obj)` exposes an independently owned
standard ComplexGeoData snapshot. Unsupported surface meshes and invalid
segments reject; no arbitrary field is converted to Part Shape.

Both the display preview and C++ seam indices now follow stored trim-wire edge
order instead of transient tracing-section order. Open-contour/multi-face tests
compare every subelement before/after native topology I/O. Mixed GUI caps/seams
render as individual Coin shapes with semantic pick names and whole-node detail
paths. Real ray hits on caps and seams, selection registration, restore and
failure clearing are tested. Snapshot replacement clears GUI selections.

Names remain snapshot-local indices, not stable external LinkSub attachments;
history mapping, snapping, implicit-surface picking, visual highlight pixel
qualification and all unfinished Stage 2/3/4 gates remain open. This is partial
selection integration, not seamless FreeCAD completion. No OCCT source or system
dependencies changed. See [contracts](native-geometry-property.md).

Full OCCT-8 migration regression passes **23/23** (46.70 seconds), including
expanded native-property and mixed GUI tests. Logs:
`/tmp/implicit-subelement-build.log`, `/tmp/implicit-subelement-legacy-build.log`,
`/tmp/implicit-subelement-cli.log`, `/tmp/implicit-subelement-gui.log`,
`/tmp/implicit-subelement-migration.log`.
The preserved compatibility suite passes **25/25** (27.41 seconds), including
the unchanged **35** isolated kernel cases; log
`/tmp/implicit-subelement-compatibility.log`. Both incremental builds succeed.

## Previous increment: native geometry property and FCStd topology

The OCCT-8 FreeCAD module now provides a native ComplexGeoData property and typed
GeoFeaturePython owner for mixed boundaries. C++ geometry ownership, discovery,
transactions and versioned FCStd `.nativegeo` persistence replace proxy-only
storage for new mixed features. Unchanged restoration does not numerically
rebuild; edits revalidate live inputs and publish new snapshots. Unsupported
surface consumers/transforms reject rather than returning partial geometry.

Mixed schema-2 fingerprints ignore source cache flags, verify stored primitive
consistency and preserve field authority. Corrupt payloads require explicit
repair. A C++ native-owner hook clears outputs skipped after upstream errors.
Schema-1 recipes migrate in place but retain legacy owner limitations. New CLI
and strengthened GUI tests cover persistence without rebuilding, source changes,
undo/redo, atomic rejection, corruption, mismatch, invalidation and migration.
The rest of Stage 2/3/4 remains unfinished; see [property contracts](native-geometry-property.md).

Verified full runs: **23/23** OCCT-8 migration entries (46.55 seconds), **25/25**
compatibility entries (27.66 seconds), including unchanged **35/35** kernel cases.
Logs: `/tmp/implicit-property-build.log`, `/tmp/implicit-property-legacy-build.log`,
`/tmp/implicit-property-full-migration.log`, `/tmp/implicit-property-full-legacy.log`.
Additional property/object content-transfer, XML-only inline restore, null
geometry and static cross-document copy checks pass in the OCCT-8 fork Cmd
(`/tmp/implicit-property-transfer.log`). Interactive GUI clipboard remains open.
Full migration rerun including those transfer checks also passes **23/23**
(46.08 seconds; `/tmp/implicit-property-transfer-migration.log`).

## Previous increment: explicit native shared-topology persistence

Schema-1 `BRepClass3d_ImplicitBoxIO` persists native gyroid math and the shared
cap/edge/wire graph, not a recipe, mesh substitute or fake UV face. Native faces
are reconstructed with the stored shared wires; planar p-curves and edge/vertex
identity survive. Bounded parsing and structural checks reject invalid records
atomically. Mesh caches are omitted without mutating source caches. Loaded
seam-bound metadata is not re-certified (`SeamBoundVerified=false`); all results
remain nonvalidated boundary compounds.

The OCCT-8 FreeCAD facade exposes `dumps_mixed`/`loads_mixed`, restoring immutable
owned topology and independent cap copies with native region queries. Two new
kernel cases cover both operations, open/closed topology, repeated I/O, shared
seams/p-curves, failure atomicity, lock rejection and cache omission. CLI/GUI
tests cover the public API, accuracy downgrade and lifetime after document close.
At that checkpoint FCStd still used recipes; the latest increment above adds
the dedicated mixed geometry-property path. The remaining Stage 2/3/4 gates
are not completed by this codec.
See [native-topology-io.md](native-topology-io.md).

Verified: **35/35** kernel cases (CTest 3.17 seconds), **22/22** migration tests
(46.09 seconds), **25/25** compatibility tests (28.08 seconds). Logs:
`/tmp/implicit-topology-build.log`, `/tmp/implicit-topology-freecad-build.log`,
`/tmp/implicit-topology-targeted.log`, `/tmp/implicit-topology-full-migration.log`,
`/tmp/implicit-topology-full-legacy.log`. The first round-trip exposed nonempty
identity-location chains; exact-identity canonicalization fixed it without
tolerance-snapping geometry. No new packages/system-library changes.

## Previous increment: conservative native mixed-region classification

`BRepClass3d_ImplicitBox::Classify` queries the mathematical box/field intersection,
independent of meshes and approximate cap polygons. Inside/outside require a
physical boundary-distance lower bound exceeding the requested tolerance;
near-boundary requires a retained cap point or sign bracket wholly in the box
and tolerance ball. Other cases return unresolved. Global Lipschitz and local
Taylor exclusions keep dimensionless field residuals separate from length.
The FreeCAD binding and mixed feature expose these immutable-snapshot queries;
failed executions clear the query cache. `solid_validated` remains false.

Three new kernel cases test Boolean direction, root/cap witnesses, stationary
ambiguity, bad inputs and length scaling at identical field values. CLI/GUI
tests cover restore/edit/lifetime/invalidation. This advances classification,
not conforming meshing, topology validation or seamless Stage 4 integration.
See [native-region-classification.md](native-region-classification.md).

Verification: **33/33** isolated kernel cases (CTest 2.40 seconds), **22/22**
OCCT-8 migration CTest entries (48.32 seconds), **25/25** preserved compatibility
CTest entries (27.63 seconds). Logs: `/tmp/implicit-region-build.log`,
`/tmp/implicit-region-freecad-build.log`, `/tmp/implicit-region-full-migration.log`,
`/tmp/implicit-region-full-legacy.log`. No new packages/system-library changes.

## Previous increment: open box contours and multiple retained regions

Active objective: [through Stage 4, seamless FreeCAD integration](stage-4-acceptance.md).
This increment advances Stage 2, not completion of that objective.

The mixed native box constructor replaces closed-loop-only rejection with
shared derivative-isolated edge roots, split box-edge intervals, directed face
graphs and closed implicit boundary wires spanning adjacent planes. Planar loops
are grouped into retained regions/holes, yielding separate caps where needed.
Both Common and explicit BoxMinusField retain field authority and source
immutability. Canonical endpoint adjustment is budgeted within the .1 mm seam
target. Caps and edge incidence/orientation are validated; whole solids are not.

Plane sections add Taylor empty-cell bounds and exact sinusoidal range/unique
root tests on field-coordinate axes, retaining conservative oblique handling.
Alternative bounded grids use the same acceptance criteria, not guessed empty
cells. Singular/grid/corner ambiguity and exhausted budgets still reject.

The new independent harmonic box-edge reference covers both operations. A
multiple-loop case checks two holes versus separate retained disks. Additional
tests cover isovalue changes, placed-field rejection and isolated-max tangency.
FreeCAD adds crossing-count diagnostics and a standard iso-0 demo; CLI/GUI tests
cover successful crossings and failed low-budget invalidation. Display remains
caps/seams only; conforming gyroid meshes and validated solids are unfinished.

Verified: **30/30** isolated kernel GTest cases (CTest 2.34 seconds), **22/22**
OCCT-8 migration CTest entries (51.04 seconds), **25/25** preserved compatibility
CTest entries (29.80 seconds). Logs: `/tmp/implicit-open-box-build.log`,
`/tmp/implicit-open-box-freecad-build.log`, `/tmp/implicit-open-box-targeted.log`,
`/tmp/implicit-open-box-full-migration.log`, `/tmp/implicit-open-box-full-legacy.log`.
No additional dependencies or system libraries were installed/modified.

## Previous increment: restricted native mixed gyroid/box boundaries

OCCT `BRepClass3d_ImplicitBox` now accepts a real conventional axis-aligned box
solid and a native gyroid face. Common uses F<=0 inside; BoxMinusField explicitly
retains the positive region in the box. All six planar sections must resolve,
with at most one closed contour per face and no edge crossings. New shared
seams have matching planar p-curves; planar caps pass BRepCheck and every hybrid
compound edge has exactly two opposite uses. Source topology is copied.
The result is NOT a validated solid or generic BOP result. The 0.1 mm default
bound covers seams, not an implicit surface mesh or whole manufacturing model.

The native FreeCAD binding owns hybrid compounds opaquely, exposing independent
native face snapshots and ordinary Part cap copies. `ImplicitMixed` adds linked
operands, recompute, recipe save/restore, operation commands and a supported demo.
Its GUI displays caps and seams only. Failure invalidates both geometry and
display caches. No new dependencies/system-library changes were needed.

Three new kernel cases cover shared topology/p-curves/caps, immutability,
unsupported inputs, translated boxes and zero-seam cases. New CLI/GUI tests
exercise live links, operand changes, snapshot lifetime, operation switching,
restore and retained/cleared Coin buffers. See [contracts](native-mixed-box.md).
Verification: **27/27** isolated kernel GTest cases pass (CTest 0.57 seconds),
the full OCCT-8 migration suite passes **22/22** (47.96 seconds), and the preserved
OCCT-7/root suite passes **25/25** (26.32 seconds). Logs:
`/tmp/implicit-mixed-full-migration.log`, `/tmp/implicit-mixed-full-legacy.log`,
`/tmp/implicit-mixed-occt-build.log`, `/tmp/implicit-mixed-freecad-build.log`.
An earlier overlapping library rebuild correctly failed the runtime library
audit on a `(deleted)` mapped inode; the non-overlapping rerun passed. These
results are targeted regression checks, not all-workbench/GPU qualification.

## Previous increment: in-process native face ownership and guarded GUI backend

The OCCT-8-only `_ImplicitOCCT` C++ extension now owns genuine native face
snapshots, exposed through an ABI/build-origin-checking `ImplicitOCCT` facade.
Sphere/gyroid construction, placed queries, translation, restricted sphere
operations, bounded sphere/patch display meshes and explicit primitive I/O reuse
the existing OCCT kernel implementation. Opaque immutable handles are not Part
shapes and are not picklable. Trim/child-topology serialization rejects rather
than silently dropping geometry. No new OCCT source changes were needed.

The existing boundary GUI feature gains explicit `Backend=InProcess`, retaining
native handles and transient meshes. The default Worker backend and old recipes
remain supported. FCStd restores mathematical recipes/backend, not native
topology or pointers. A native in-process sphere command is registered only in
the supported build; failed operations clear both caches, with no worker fallback.
The GUI test forbids worker subprocesses throughout creation, edits and restore.

All **20/20** OCCT 8 migration tests pass (50.92 seconds), and **25/25** combined
compatibility tests pass (31.38 seconds), including the new legacy import guard.
Tests exercise immutable lifetime across document close, translation/reflection
mesh frames and winding, ray brackets, primitive I/O and unsupported-operation
guards. Calls are synchronous/GIL-held; general Part consumers, native trim-
topology persistence, conforming gyroid meshing and mixed BOP remain unfinished.
See [contracts and try/build commands](in-process-native-faces.md).

## Current increment: native gyroid sections, closed seams and migration preflight

`BRep_ImplicitPlaneSection` now traces native gyroid/plane rectangles in OCCT 8.1,
using placed-coordinate Lipschitz/Hessian bounds, monotone-edge root isolation,
interior regularity checks and shared grid-edge connectivity. Only fully
resolved cells count as successful coverage; grid zeros, ambiguity, potential
critical regions or resolution exhaustion return explicit unresolved results.
The geometric bound is an accepted-cell diagonal plus numerical reserve, not a
dimensionless field residual. Independent analytic fixed-Z reference points,
placed planes, verified emptiness and saddle rejection are tested.

`BRep_ImplicitGyroidOps::SplitZ` accepts only resolved closed-loop sections and
creates opposite `BRep_TImplicitPlanePatch` predicates sharing tolerance-bearing
degree-1 B-spline seam edges. It rejects open/domain-side and unresolved cases.
The edges approximate intersection curves; the field and plane remain
authoritative. There are no trim-conforming meshes, p-curves, general mixed BOP
results, caps or validated solids. This is part of requested step 2, not its
completion or completion of step 3.

The native worker's section mode and in-tree `ImplicitSection.py` adapter expose
editable gyroid-plane contour previews with retained Coin lines and FCStd
recipe reconstruction. Unresolved/error states clear old buffers. This is not
a trimmed-surface GUI object or OCCT 8 handles in the OCCT 7.9 Part process.

For requested step 4, FreeCAD's `FREECAD_OCCT_BUILD_DIR` now supports strict
source-build target imports and rejects missing/unbuilt toolkit dependencies,
without installed-library fallback. ApplicationFramework/DataExchange/
Visualization configure in the existing OCCT tree using installed dependencies.
The original dependency preflight reported 27 configured-but-unbuilt toolkits
and `TKDEGLTF` missing because RapidJSON was absent. The user has now installed
and confirmed `rapidjson-devel`; its package/header were verified and OCCT
reconfiguration with `USE_RAPIDJSON=ON` succeeds, including `TKDEGLTF`.
The required toolkit build completed all 5,202 steps with exit code zero
(log `/tmp/implicit-occt-build.log`), and strict dependency preflight now passes,
selecting OCCT 8.1 source-build libraries only. No packages or system libraries were
installed/replaced by the agent, and the working FreeCAD build stays on OCCT
7.9. This is migration preparation, not a completed OCCT-8 FreeCAD build.

A separate `build-freecad-occt8` FreeCAD configuration now succeeds using the
documented minimal module settings and existing dependencies. The Implicit
bridge uses the signed-distance class exported by the fork's `TKTopAlgo`, rather
than compiling duplicate definitions into `Implicit.so`. The OCCT 7 configuration
still stages/compiles only that extension; both configurations and generated
Ninja graphs were checked. The first Part build stopped on legacy alias headers
that OCCT provides only during installation. FindOCC now propagates the selected
source tree's compatibility aliases through `TKernel`, with no system fallback;
Part's removed `Standard_Byte.hxx` include is guarded for OCCT 7. The compiled
alias-header regression passes. Command-line/Part/Implicit targets now build,
as does bundled Pivy/Coin, required by startup in this GUI-enabled configuration.
An initialization traceback remains a test failure even if the script later
prints a success marker.

The separate migration harness passes **12/12** tests (4.90 seconds), using
`build-freecad-occt8/bin/FreeCADCmd`. Its runtime fixture verifies Part/Implicit
module paths, all 32 actual loaded OCCT libraries under the fork's 8.1 library
directory, conventional Booleans and the kernel signed-distance bridge, followed
by the eleven native smoke tests including live Part links and local patch/STEP
round-trip. It rejects the working OCCT 7 binary as an intentional negative
check. The affected OCCT 7 targets also rebuild successfully. The GUI build
subsequently completed with exit code zero (log
`/tmp/implicit-freecad-occt8-gui-build.log`).

The extended suite now passes **18/18** (42.59 seconds): the twelve command-line
checks, an actual GUI loaded-library audit, synchronous/background/worker GUI
smoke tests and the four-example/live-Part-background launcher demos. The GUI
audit checks PartGui/ImplicitGui and bundled Pivy/its extension, Coin and all 32
OCCT libraries against the intended build paths; a negative OCCT 7 GUI check
rejects correctly. The existing OCCT 7 combined suite passes **24/24** again
(25.61 seconds). Test preferences are isolated; rendering uses Xvfb/software
OpenGL. No physical-GPU/FPS, all-workbench or manufacturing qualification is
claimed. Native implicit-face consumer/persistence support remains open.

`bash tools/launch_demo.sh --occt8` explicitly selects the tested OCCT 8 build;
`--live-part --background` selects the live mixed-field workflow. The default
launcher still uses the preserved OCCT 7 build.

After rebuilding the affected OCCT 7 targets, the existing combined suite passes
**24/24** (35.61 seconds). This includes the isolated 24-case OCCT 8 conformance
executable, but is separate from the new 12-entry FreeCAD OCCT 8 migration suite.

Earlier, after the OCCT dependency build and both FreeCAD configurations, the
existing combined regression suite passed **24/24** (31.35 seconds), including the
24-case isolated OCCT 8 conformance executable. The FreeCAD runtime tests still
used the working OCCT 7 build; that run did not validate the then-unfinished
FreeCAD 8 migration binary.

Before the expanded dependency build, all **24 native GTest cases** and
**24/24 combined CTest entries** passed (25.72 seconds after rebuilding all six kernel libraries), including
the strict source-build preflight and extended GUI/protocol tests. Only TKBRep,
the isolated harness/worker and `ImplicitScripts` were built. See
[contracts, stage status, try commands and migration gate](native-gyroid-sections.md).

## Previous increment: restricted native sphere trimming/Boolean/GUI vertical slice

OCCT now has `BRep_TImplicitSpherePatch` (sphere + plane predicate + exact circle
wire) and `BRep_ImplicitSphereOps` for transverse plane trims and two-sphere
common/fuse/cut boundary decomposition. Transverse results share a circle edge;
orientation, operand provenance, disjoint/containment/cavity cases and immutable
inputs are covered. Near-tangent/coincident or tolerance-ambiguous cases,
non-sphere fields, incomplete input domains and nested trims reject explicitly.
This is a compound of native boundary faces, not a validated solid or general
`BRepAlgoAPI`/BOP support. No fake UV surface or mesh-face substitution is used.

A sphere-specific display mesher has a geometric parameter-interpolation bound
and precision/resource guards, tested at the 0.1 mm target. It does not mesh
gyroids, cap trimmed patches or weld result meshes across shared analytic edges.
Patch domain/ray filtering honors the plane; sampled roots still have the
documented completeness and near-trim-boundary limitations. Schema-1 persistence
rejects the trim subtype even after children are removed, preventing field loss.

The OCCT 8 worker executable runs these native operations in a separate process.
The in-tree FreeCAD `ImplicitNative.py` GUI adapter adds editable experimental
sphere/trim/common/fuse/cut examples. It persists mathematical recipe properties,
reconstructs native geometry on restore and displays transient retained Coin
buffers. It never places OCCT 8 handles in the current OCCT 7.9 GUI/Part process
or fills a `Part::Feature::Shape`. This is Python GUI integration inside the
native module, not a new C++ native geometry property or wholly OCCT-8 FreeCAD.
Worker calls currently block recompute with a timeout; global async budgets and
semantic selection/operand linking remain future work.

All **19 native GTest cases** and **23/23 combined CTest entries** passed
(26.47 seconds in the final run), including new worker protocol and fork GUI/FCStd tests plus
all previous GUI, command-line and mixed Part regressions. `ldd` verifies the
worker loads only OCCT 8.1 libraries; no system libraries/packages changed.
Only TKBRep/dependencies, the isolated test/worker project and FreeCAD
`ImplicitScripts` were built. General trimming/intersections/BOP, topology
persistence, validated solids, gyroid accuracy and in-process Part exposure
remain open. See [native sphere contracts/try guide](native-sphere-operations.md).

## Previous increment: explicit OCCT-native single-face persistence

`BRep_ImplicitIO` now provides experimental schema-1 stream persistence for a
single childless native sphere/gyroid face. The authoritative equation,
finite local domain, raw geometric tolerance, geometry/instance placements and
orientation survive round-trip; triangles and shape flags do not. The format
does not depend on a display mesh or fake UV surface.

Writers preflight before touching output and reject custom/derived equations
and child topology rather than lose data. Readers consume a bounded record,
reject malformed/unknown/trailing/nonfinite input and invalid transforms, and
replace the destination only after validation. Matrix validation prevents
OCCT's loader from silently repairing shear/nonuniform scale. Locale, locks,
stream failures, every truncated record prefix, scaled/reflected placements,
both primitives and retained 0.1 mm sphere ray brackets are regression-tested.

All **14 isolated native-face GTest cases** and **21/21 combined CTest entries**
passed (23.00 seconds for the combined run), including the existing fork GUI
and command-line tests. The isolated executable still links only rebuilt OCCT
8.1 libraries; FreeCAD and compatibility tests remain on OCCT 7.9.3. Only
TKBRep and its existing dependencies/conformance harness were rebuilt. No
packages or system libraries were modified.

This is not generic BRepTools/BinTools, OCAF, FCStd or STEP persistence for
native implicit faces. Those guards remain intact. Native meshing, trim/wire
contracts, BOP, general graph/topology persistence and FreeCAD exposure remain
open. The next kernel increment is field-aware meshing with explicit geometric
error/coverage limits, distinct from dimensionless gyroid residuals. See
[native face contracts](occt-native-faces.md).

## Previous increment: OCCT-native implicit face foundation

The user selected **0.1 mm geometric tolerance** and directed work on native
OCCT implicit faces. The OCCT 8.1 fork now has immutable `Geom_ImplicitField`
geometry (sphere/gyroid), field-backed `BRep_TImplicitFace`, builder creation,
representation predicates/accessors and `BRep_ImplicitTool` placed-coordinate
evaluation, gradient/normal, bounds/domain and sampled ray queries. It does not
invent a UV parameterization or store arbitrary fields in `Geom_Surface`.

Low-level Surface reads/writes reject native implicit faces. Tests verify that
UV adaptors and ASCII/binary serializers explicitly reject unsupported field
loss. Copy retains authoritative geometry/domain/location/tolerance and drops
transient mesh data. Eleven isolated conformance cases pass, including analytic
sphere root brackets at the 0.1 mm target, transformed coordinates, gyroids,
critical/sampling failure cases and conventional-face regressions.

TKernel/TKMath/TKG2d/TKG3d/TKGeomBase/TKBRep were built from OCCT 8.1 source with
two compile jobs and a one-job link pool in the existing OCCT build tree. The
test executable's dependencies were checked to load only OCCT 8.1 libraries.
The FreeCAD build and earlier compatibility tests remain on installed OCCT 7.9.3;
no system libraries or packages were changed. Native face tests run in a separate
process and project, not by mixing those ABIs.

This is a native representation/query foundation, **not full implicit B-Rep
support**: generic BOP/meshing/validation, trim curves/wires, persistence/exchange
and exposure in the current FreeCAD GUI remain open. A stored face tolerance or
isolated root test is not a whole-model 0.1 mm certificate. See
[native face contracts](occt-native-faces.md) and [accuracy policy](accuracy-policy.md).

Verification: **21/21 combined CTest entries passed** (24.02 seconds), including
the isolated executable's **11 native-face GTest cases** and both existing GUI
regressions. Only the required six OCCT foundation/modeling-data libraries and
this conformance harness were built; full OCCT BOP/exchange/regression coverage
has not been run. Existing FreeCAD source and its OCCT 7.9 linkage are unchanged.

## Previous increment: scalar controls and refinement scaling

Native graphs now support constant/linear scalar fields, spatially varying
level-set bands and polynomial smooth unions. Analytical gradients include the
width gradient and smooth blend weights; conservative Lipschitz bounds feed the
existing pruning/refinement queries. Strict node validation, finite-difference
gradient comparisons, sign behavior, sampled Lipschitz inequalities, controlled
extraction and FCStd round-trips are covered by `scalar_smoke.py`.
Units remain caller-managed and widths are not inherently physical thickness;
see [scalar controls](scalar-controls.md).

All **20 combined CTest tests passed** (21.87 seconds in the final run), including both native
GUIs and the local B-spline/STEP tests. The Part regression additionally checks
that controlled extraction rejects mixed fields without a finite Lipschitz
bound. A three-run-median controlled-extraction sweep now records sphere and
1/3/5/10-cell gyroid timings, bounds and explicit failures at the configured
resolution cap. This is not an equal-accuracy, hardware FPS or mesh-memory
benchmark; see [results/limits](controlled-extraction.md#scaling-observation).

## Previous increment: native local charts and experimental B-spline patches

Native `Implicit.sampleChart` projects a seed, builds a fixed-normal tangent
chart and samples unique ray roots with normal-alignment/resource/cancellation
checks. `ImplicitPatch.fit_patch` orchestrates this C++ sampler with Part/OCCT
B-spline interpolation, shape validity and sampled field/normal/domain checks.
It rejects dirty/error sources and changes during fitting; reports retain local
frame, source identity/revision and all approximation settings. The one-shot
cancellation state is readable through `Implicit.isCancelled`.

Sphere and gyroid open patches pass checks. A genuine B-spline face is clipped
against a conventional Part box and round-tripped through STEP, retaining face
validity, B-spline representation and area. No document/file mutation occurs
unless a caller explicitly stores/exports the returned face. This is not a
sewn atlas, closed solid, geometric error certificate or OCCT implicit face.
See [local patch bridge](local-patch-bridge.md). The native scripts target now
installs `ImplicitPatch.py`; no additional dependencies or system changes.
All **19 tests passed** in the final combined run (**21.81 seconds**), including
the new chart/patch checks, both native GUI tests and existing OCCT regressions.

## Previous increment: field-bound-controlled extraction

`Implicit.tessellateControlled` adds global-grid refinement against a
conservative triangle-wide field residual bound, plus per-attempt history,
explicit resolution-limit/empty-unverified status and cancellation through
quality inspection. The previous mesh is dropped before the next extraction.
Unsupported Lipschitz bounds are rejected rather than inventing accuracy.
These are field-value bounds, not geometric-distance/coverage certificates or
variable-resolution cells. [Contract](controlled-extraction.md).
All **17 combined tests passed** (22.27 seconds), including both fork GUI tests.

## Previous increment: Stage 2–3 projection, probes and quality diagnostics

Native `Shape::project` / `Implicit.project` now solve for a local constrained
stationary point on a compiled graph. Row-scaled pivoted KKT steps use a numerical
gradient Hessian, domain-bounded backtracking and separate field/distance tests.
Critical points, singular systems, line-search failure and iteration exhaustion
return explicit non-convergence diagnostics. A gradient-scaled residual criterion
avoids treating a small-valued field as a geometric success at the seed.

The Native Implicit workbench now offers a selected-field, one-shot viewport
probe and a resolution-24 sampled mesh-quality report. Probes use actual
orthographic/perspective rays, transform global placement, and query the field
rather than its mesh. Pending callbacks cancel on replacement, target deletion
and document close. They do not create persistent subelements or replace normal
FreeCAD selection. Inspection samples vertices, triangle centroids and unique
edge midpoints and reports edge-incidence/winding defects and exact degeneracy.

These are partial deliveries toward Stages 2–3. No globally nearest-point,
Hausdorff-error, complete-intersection or valid-solid claim follows. True
variable-resolution extraction, error-controlled refinement, robust tangencies,
arbitrary boundary intersection, integrated semantic selection/snapping and
richer editing remain open. Full API and limitation contracts:
[native projection and quality](native-projection-and-quality.md).

Verification includes 60 deterministic analytical sphere projections, gyroid
stationarity/field scaling, failure and invalid-input tests, coarse/fine sphere
interior residual reduction, uniform/pruned quality equality, clipping and empty
results, live mixed Part projection, actual GUI camera rays, cancellation,
deletion and document-close lifecycle handling. Native modules and scripts were
built with the existing toolchain and two compile jobs. No OCCT change or package
installation was needed.
The final combined CTest run passed **16/16 tests** in **20.48 seconds**, including
both native GUI regressions under Xvfb and the new native geometry-query test.

## Previous increment: native ray–field queries

Native `Shape::intersectRay` and Python `Implicit.intersectRay` query compiled
immutable field graphs independently of display tessellation. The direction is
normalized, the finite interval is clipped to the effective local domain, and
sampled sign brackets are bisected to an explicit model-length tolerance. Each
sorted hit carries local distance/point, absolute field residual and bracket
width. The GIL is released only after capturing the Shape snapshot. Input and
iteration limits bound work; failed refinement and adjacent zero samples raise
explicit errors. See [query contract](native-ray-queries.md).

The new regression covers sphere/gyroid/band roots, normalization, negative and
zero-length intervals, domain clipping without artificial caps, placement/sign
conventions, tangencies sampled or missed, ambiguity and invalid arguments.
The live Part regression now checks mixed ray roots before and after editing
the source. These queries are a foundation for picking, not an implemented GUI
selection feature. Unsampled tangencies, thin crossings, close-root merging and
non-isolated/tolerance-plateau ambiguity remain explicit limitations. Native
local projection, chart fitting and manufacturing topology remain open.

Verification: native `Implicit` and `ImplicitGui` rebuilt with two compile jobs;
all **15 combined CTest tests passed** (21.14 seconds), including both GUI tests
under Xvfb, the new ray regression and live Part ray invalidation. No new OCCT
change or dependency installation was needed for this increment.

## Previous increment: opt-in background viewport extraction

`ImplicitGui::ViewProviderShape.BackgroundMeshing` now offers experimental
background extraction; the default remains synchronous. A module-local Qt
Concurrent pool allows one viewport extraction at a time. Each provider has one
dispatched job and one replaceable pending request, so rapid edits coalesce.
Immutable `Shape` snapshots and atomic cancellation flags are the only worker
inputs. Document objects, Python, Coin nodes and widgets stay on the GUI thread.

Completed meshes upload only if their generation is still current and the
native feature accepts the snapshot identity/domain/inside convention. They
populate the existing display cache, including reuse after switching back to
synchronous mode. Geometry edits clear obsolete data and await recompute;
invalid inputs, cancellation, hidden views, deletion and document close cannot
resurrect an obsolete result. Retained Coin nodes are reused. Tolerance-only
definition edits now notify cache revision changes as well, avoiding a blank
background view waiting forever for a recompute notification.

`CancelMeshing` is a self-resetting action. `Meshing`, `MeshStatus`, request and
displayed generation IDs, and `MeshJobCount` expose transient diagnostics.
The enabled mode persists in FCStd; meshes/jobs/diagnostics do not. There is no
whole-document async recompute or global memory budget: source Part recompute,
B-Rep snapshots/validation, graph compilation and Coin uploads can still block.
Cancellation cannot interrupt a single OCCT query mid-call. See
[`background-meshing.md`](background-meshing.md) for the full contract.

All **14 combined CTest tests passed** (19.82 seconds in the captured final run),
including the new async GUI regression with live Part coverage. The regression
checks GUI heartbeat during extraction, coalescing, stale suppression, retained
nodes, final coordinates/counts, cache reuse, mode switching, tolerance/error
recovery, empty results, hidden views, cancellation, live B-Rep edits/deletion,
provider deletion/document close with outstanding jobs, and restore. The async
test also passed three consecutive final repeat runs (17.84 seconds total) to
stress lifecycle handling, including the final result-cleanup path.
Both demo launchers passed
checks with `--background`, including a live Part edit followed by async upload.

Try `bash tools/launch_demo.sh --live-part --background`. The demo first builds
its initial model synchronously, then enables background extraction for edits.

## Live dependencies and extraction control

Native `Implicit::Feature` now has document schema 3 with `Operation`, `Left`,
`Right`, `Amount`, and `AutoDomain`. Definitions retain schema-1 JSON or editable
primitive properties. Linked union/intersection/difference, band and offset
nodes share immutable child fields through FreeCAD document dependencies.
Recompute invalidates downstream compiled/display caches after source edits;
missing or failed sources produce errors and clear stale display results.
FreeCAD skips downstream execution after an upstream failure; a post-recompute
dependency check explicitly invalidates those skipped features. Queries validate
the dependency DAG before returning cached geometry. `StatusMessage` exposes the
error; tests and commands use `isValid()` (FreeCAD's Python state label is
`Invalid`, not `Error`).
Queries guard against cycles and excessive native dependency depth. Undo and
FCStd restore retain the actual links rather than duplicated field snapshots.

Rigid source placements map into the result's parent frame, with transformed
gradients and sign normalization for positive-inside operands. The result's own
Placement remains an additional object transform. Automatic sampling domains
enclose transformed operand domains; `Implicit.bounds` reports effective bounds.
Manual domains remain available, particularly for expanded offsets. Domains
remain computational limits, not artificial cap surfaces.

With Part and the OCCT bridge enabled, linked operations accept single-solid
Part features directly. `Operation=BRep` supplies a reusable live B-Rep distance
field. Each recompute captures isolated BRep geometry and refreshes automatic
bounds. Source dimensions and placements propagate. The workbench offers live
B-Rep creation and linked Booleans; primitive commands no longer mask parameter
editing with JSON graphs. Earlier snapshots remain supported and do not
silently become live objects.

Python extraction and the scalar batch numeric loop release the GIL after
capturing immutable geometry. One-shot atomic cancellation tokens interrupt
sampling/root extraction, returning an explicit error instead of partial mesh
data. These Python calls remain synchronous; optional background GUI scheduling
is added by the later increment above. Neither interruption inside a single
OCCT query nor cancellable Python-container conversion is claimed. FreeCAD
document APIs must still be called from the main thread.

The OCCT signed-distance evaluator now retains the fixed boundary operand's
distance-query maps/bounds between points. Additional native GoogleTests check
alternating interior/exterior/boundary queries and recovery after invalid input.
All **13 combined CTest tests pass** (13.41 seconds in the captured run),
including the fork GUI with direct Part selections, live B-Rep creation,
failed-source display clearing/recovery and the new linked/cancellation/Part
regressions. `Part`, `PartGui` and `PartScripts` are built. The separate ordinary
interactive demo and live Part/gyroid demo both pass automated GUI checks. The
live demo verifies that a Part dimension edit updates its linked result.
`PartScripts` is required alongside PartGui: it supplies AttachmentEditor and
other Python resources that are not copied by building PartGui alone.

`benchmark/native_viewport.py` adds the 1/3/5/10-cell GUI benchmark with six grid
cells per period. It separates scalar batch evaluation, extraction plus Python
transfer, combined parameter recompute/upload/render latency, completed Qt frame
swaps, packed mesh storage and entire-process peak RSS. It explicitly disables
Quarter's frame cap and waits for `frameSwapped`; counting deferred `redraw()`
calls produced invalid results and was corrected before recording this baseline.
Qt swaps/second are not monitor presentation/vsync FPS, and software-rendered
Xvfb observations are not GPU performance claims.

The recorded single-pass software-rendered run in `benchmark/native_viewport.csv`
used a 1024x456 viewport. At 1/3/5/10 cells it observed approximately
140/81/39/9 completed Qt swaps per second and 18/66/225/1448 ms for a parameter
change including recompute, upload and rendering. The largest case had 960,000
triangles, approximately 46 MB packed mesh storage, and 861 MiB entire-process
peak RSS. Compilation was stopped for measurement; unrelated desktop workloads,
software rendering and single-run variability still limit interpretation.

`implicit_intersection_benchmark` adds projection/ray/plane/cylinder timings for
the same four cell counts; the recorded run is `benchmark/intersections.csv`.
The radius-scaling sweep exposed a chart-tolerance issue: cylinder angles are
not model lengths. Chart stopping tolerance now accounts for the map's maximum
stretch. The radius-25 gyroid regression catches the former approximately 1e-8
residual; the corrected benchmark's sampled roots are below 1e-10. This is
vertex residual evidence, not a curve-interior error or completeness guarantee.

Reproduce the added measurements from the umbrella root:

```sh
cmake --build build --target implicit_intersection_benchmark --parallel 2
./build/implicit_intersection_benchmark
IMPLICIT_VIEWPORT_CSV=benchmark/native_viewport.csv python3 tools/with_xvfb.py \
  build-freecad-implicit/bin/FreeCAD --safe-mode benchmark/native_viewport.py
```

## Native display and document integration

`FreeCAD/src/Mod/Implicit/Gui` adds an in-tree C++ view provider for
`Implicit::Feature`, registered through `ImplicitGui` and `InitGui.py`.
It owns retained Coin coordinate, normal and indexed-face nodes; exposes
resolution, adaptive sampling, surface color and shaded/wire/flat-line modes;
and shrinks buffers
when resolution decreases. The GeoFeature view-provider base applies Placement.
Geometry revision notifications trigger cache uploads. Feature-local compiled
graphs and meshes are cached in the native document object. Field/domain edits
invalidate them, including edits during restoration; ordinary camera/placement
changes reuse local geometry. Invalid recomputes clear display geometry.

The earlier display increment provided a Native Implicit workbench with sphere, gyroid,
bounded band, field Boolean and B-Rep snapshot commands. Commands are undoable.
Those historical Boolean snapshots required matching Placement and negative-inside
operands. They copy definitions rather than maintaining live dependency links.
B-Rep snapshot results retain the source label for provenance, but source edits
do not update them. Native GUI tests exercise cache reuse, buffer shrink,
definition changes, bad-graph recovery, placement and FCStd restoration.

## Conservative octree sampling

`Implicit.tessellateAdaptive(feature, resolution=32)` subdivides an octree and
rejects blocks only when a global Lipschitz bound and a center sample rule out
a zero crossing. Sphere and box bounds are one; gyroid uses
`2*sqrt(3)*(2*pi/period)`. Min/max Booleans inherit the larger operand bound;
translation, offset and absolute-value bands preserve it. A field without a
finite proven bound disables pruning. B-Rep snapshots currently use that
fallback.

All surviving surface cells reach the same finest grid size and use the same
tetrahedron decomposition and global edge keys. Thus there are no mixed-level
transition faces. This is adaptive empty-space sampling, not a variable-density
surface mesh. The grid cache remains densely allocated. A small floating-point
margin guards ordinary rounding; no interval-arithmetic proof is claimed.
Regressions compare exact indexed geometry after reordering with the uniform
reference and check closed sphere connectivity and reduced sample counts.

## Bands, caps and mesh winding

Native graph nodes `band` and `box` implement `abs(F)-halfWidth` and an exact
axis-aligned box signed distance. Intersecting a band with a box supplies
mathematical cap surfaces. Keep the computational domain larger than the box
so the zero surface is sampled on both sides. Computational-domain clipping
alone still produces no caps.

The bounded gyroid regression checks paired, oppositely directed triangle edges.
It exposed a winding defect in the previous gradient-based orientation: hard
Boolean branches can change analytic normals across a triangle. Winding now
follows the linear tetrahedron sign configuration; gradients remain shading
normals. Crease normals may oppose a coarse triangle and are not a substitute
for topological orientation. Closed visualization meshes are not OCCT solids.

Band widths use the input field's units. A gyroid band width is dimensionless;
a true signed-distance field's band width is a model length. Combining fields
of different units preserves sign-set operations but does not make the combined
value a signed distance. General unit validation and physical wall-thickness
controls remain to be implemented.

## OCCT core signed-distance extension and mixed fields

The OCCT branch `feature/implicit-distance` adds
`BRepClass3d_SignedDistance` to the TKTopAlgo package manifest, implementation,
public header and native GoogleTest manifest. It accepts one finite, valid,
outward-oriented solid. It builds a boundary-face compound for nearest distance
and uses solid classification for the sign; cavity points are outside material.
Measuring against faces avoids the solid-to-interior-point zero-distance result.
The tolerance creates an explicit on-boundary zero band. Invalid inputs and
unresolved classification/distance fail explicitly. Instances are noncopyable
and not thread safe; input geometry must remain immutable.

The extension implementation is built against installed OCCT 7.9.3 for current
compatibility testing. Only its new header is staged into an isolated include
directory, preventing accidental use of the fork's OCCT 8.1 headers with 7.9
binaries. A complete OCCT 8.1 toolkit rebuild and its GoogleTest suite have not
been run. The newly added OCCT GoogleTest source is also compiled and executed
against the compatibility build when GTest is available. No system libraries
are replaced.

The new extension translation unit is also compiled directly by the OCCT 8.1
source build, without building all toolkit dependencies. This caught removal
of the legacy `Raise()` exception API; explicit throws now compile under 8.1
and pass the 7.9 runtime tests. The isolated configuration is in ignored
`build-occt-implicit`.

FreeCAD's optional `IMPLICIT_OCCT_SOURCE_DIR` compiles this extension into the
native Implicit module. The `brep` graph node persists an ASCII BRep snapshot,
tolerance and field definition within the graph's existing size limits.
`BRepField` locks the mutable classifier and computes finite-difference
gradients. Union/intersection/difference then work in field space. The mixed
smoke test checks signs, gradients, numerical extraction and FCStd reload.
Results remain implicit fields and transient boundary meshes; existing
`BRepAlgoAPI_*` operations do not accept arbitrary implicit faces.

## Cylinder contours

The standalone `CylinderIntersection` maps `(angle,height)` into a full periodic
cylinder and reuses plane-chart contour tracing. Root refinement stays in the
cylinder chart, so output points lie on the cylinder rather than its polygonal
chords. Endpoint stitching joins the periodic seam. Tests cover two analytical
sphere/cylinder circles, a rotated axis and out-of-domain rejection. The entire
cylinder must fit in the field domain. Tangencies, coincident regions and
singular graph intersections are not guaranteed; curves are sampled polylines.

## Still planned

The following are not completed by these increments:

- native complex-geometry property, shared-node JSON DAG format and graph editor
  (document-level source-link DAGs are now implemented);
- field-aware semantic picking and broader container/App::Link integration tests;
- whole-workflow async scheduling, SIMD/GPU and global cache/snapshot budgets;
- variable-resolution surface cells, transition meshing, rigorous approximation
  error control, broader topology and singularity tests;
- general plane-box polygon clipping, trimming topology and validated distance
  offsets for uniform physical thickness;
- fitted B-spline atlases with common boundaries/p-curves, sewing, healing,
  geometric error measurement, validated manufacturing solids and STEP export;
- native OCCT implicit face storage, adaptor/intersection/Boolean dispatch,
  persistence and exchange design;
- cross-workbench FEM/CAM consumers and systematic viewport/FPS benchmarks.

The roadmap's full completion should be evaluated against these capabilities,
not merely the presence of code in the two source trees.

## Verified results

Before the current live increment, all **10 CTest tests passed** in one combined run (5.50 seconds),
including the fork's native GUI on an isolated Xvfb software-rendered display.
The GUI test uses FreeCAD safe mode to isolate user-installed addons. It checks
the workbench command/undo, retained nodes, buffer shrink, adaptive-mode changes,
cache reuse during camera/placement changes, placed scene bounds, invalid-graph
recovery and save/reload of definition and view properties.

The OCCT extension object compiled successfully against the OCCT 8.1 source
headers. Its standalone and GoogleTest runtime checks passed against 7.9.3.
A complete OCCT toolkit rebuild and full toolkit regression run remain open.

`benchmark/native_sampling.py` and its recorded CSV compare uniform and
octree-pruned sampling, including Python result conversion. In the captured
single run, the sparse sphere at resolution 64 used 3,187 instead of 274,625 grid
samples and took 7.677 instead of 20.342 ms. Dense gyroids saw little/no pruning
and extra octree overhead. This is evidence for selective use, not a general
speedup claim. The gyroid sweep fixes resolution at 32, so increasing unit-cell
count reduces samples per period and does not preserve geometric accuracy.
Packed mesh bytes exclude working buffers, root maps, allocator overhead and
Python copies; that sampling script does not measure rendering. The new GUI
benchmark and its completed-frame measurements are described above.

Reproduce the full test run after building the native targets:

```sh
cmake -S . -B build -DIMPLICIT_BUILD_OCCT_DISTANCE=ON \
  -DIMPLICIT_NATIVE_FREECADCMD="$PWD/build-freecad-implicit/bin/FreeCADCmd" \
  -DIMPLICIT_NATIVE_FREECADGUI="$PWD/build-freecad-implicit/bin/FreeCAD" \
  -DIMPLICIT_NATIVE_PART_TESTS=ON
cmake --build build --parallel 2
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
./build-freecad-implicit/bin/FreeCADCmd benchmark/native_sampling.py
```

The earlier reduced build omitted Part/PartGui. This increment explicitly builds
`Part`, `PartGui` and `PartScripts` for interactive conventional-solid workflows.
The original `mixed_smoke.py` still uses a BRep fixture without Part's Python
module; `part_linked_smoke.py` and the optional mixed GUI checks test live Part
dependencies. Build only the relevant targets with two compile jobs; do not
mistake a configured workbench for a built one.
