# Implementation progress and remaining work

Updated 2026-10-29. The user has authorized continuing the full roadmap,
including FreeCAD/OCCT core modifications, commits and pushes. This log records
concrete increments; the overall nTop-class goal remains a substantial project.

The parent repository pins the authoritative FreeCAD/OCCT submodule heads.
FreeCAD remains on `feature/native-implicit`, OCCT on `feature/implicit-distance`.

## Stored-cylinder primitive and masked seam proximity prerequisite

The actual stored seam now has a whole-segment spatial distance audit against
unrestricted primitive gyroid zeros. Endpoint interpolation/Hessian bounds and
a positive directional derivative cone provide a root-distance bound in model
length, including world scale/placement and bounded adaptive work. FreeCAD
exposes check_stored_cylinder_primitive_seam through the distinct stored owner.
The additional masked query uses bounded radial contraction and axial brackets
inside the actual convex cylinder/source mask. No stored flags are promoted.
Focused IO passes 1/1 (6.440 s); runtime/IO passes 2/2 (6.98 s), with both signs,
translations, altered interior seam and budget failure. Reverse coverage, material regions and
volume/solid requalification remain required before live document/GUI integration.
This does not complete Stage 4. See native-cylinder-persistence.md.

## Stored-cylinder curve coherence prerequisite

The new OCCT CheckCurveCoherence and FreeCAD check_stored_cylinder_curves bound
stored 3D/p-curve consistency over whole knot intervals, using speed and second
derivative bounds with adaptive subdivision. Stored edge tolerances and the
200000-interval default budget are preserved. The earlier speed-only method
exhausted that budget for valid tight circles; interpolation curvature bounds
resolve the issue. Interior spline corruption and budget exhaustion reject.
The immutable stored owner's geometry/volume/solid certificates remain false.
Focused IO passes 1/1 (6.834 s); cylinder core passes 22/22 (42.037 s);
runtime/IO passes 2/2 (7.75 s).
This is a prerequisite for full stored-field/region revalidation, not its
completion. Dedicated document/GUI and general Stage 2–4 gates remain open.

## Milestone 99: explicit cylinder graph IO

OCCT now stores and restores the actual shared three-face cylinder solid graph,
including native mathematical face, authoritative primitive, analytic curves,
bounded degree-one splines/p-curves, orientations and tolerances. Explicit schema
and allocation limits reject unsupported/malformed records atomically. A shared
structural validator checks shell incidence, wire joins, vertex links and counts.
No mesh serialization or recipe retracing substitutes for stored topology.

FreeCAD exposes dumps_cylinder_solid, loads_cylinder_solid and
inspect_stored_cylinder_solid. The restored immutable owner is distinct from a
qualified solid. Geometry/volume/solid flags remain false and qualified queries
reject it. Numeric bound provenance cannot become a certificate through report
dictionary edits. No document property or GUI lifecycle is claimed.

Full native regression passes 160/160 (345.09 s); focused IO passes 1/1 (4.188 s).
Runtime, cylinder family and legacy solid IO pass 11/11. Tests completed October
25 and logs were rechecked October 29. Existing builds/dependencies only.
See [persistence contracts](native-cylinder-persistence.md). Next are stored-graph
geometric revalidation, trim-conforming display and live property/FCStd/GUI
integration. General partial/multi-sheet and Stage 2–4 gates stay open.

## Milestone 98: actual cylinder solids and bounded material volume

BoundMaterialVolume uses adaptive disk cells, conservative area intervals and
whole-cell gradient/Hessian root cones to bound mathematical material volume.
BuildNativeSolid attaches the shared closed shell to an actual TopoDS_Solid only
after topology, boundary geometry, material and positive/partial volume gates.
FreeCAD exposes owned solids, inspection, volume bounds and material queries;
generic OCCT support remains false. Focused native checks pass 38/38 (36.981 s),
and runtime/cylinder/solid-I/O checks pass 10/10 (21.69 s). Independent analytic
root quadrature, complementary volume, actual shape membership, reversed/
translated frames, budget/coarse-interval refusal and cancellation are tested.
Full native regression passes 159/159 (CTest 327.90 s); syntax and whitespace
checks pass. Trim-conforming display, general partial/multi-sheet
support, broader dispatch, persistence and live GUI/property workflows remain.
See native-cylinder-solids.md.

## Earlier prerequisite after milestone 97: bounded material classification

ClassifyMaterial reuses qualified geometry owners and returns mathematical
IN/OUT/ON/UNKNOWN with model-length boundary-distance bounds. Cylinder/field
cones, retained cap witnesses and global/local axial cones separate scalar F
from distance tolerance. Clearance beyond the approximation allowance is a
separate flag. Both signs, analytic roots, cap/side exclusions, explicit ambiguity
and translated/reversed frames are tested. Focused checks pass 36/36 (28.205 s),
and runtime/cylinder/solid-I/O checks pass 9/9 (17.54 s). Full 157-case regression
was not rerun for the added query. No solid/volume or generic consumer claim is
introduced. General partial/multi-sheet and Stage 4 requirements remain open.

## Milestone 97: bidirectional cylinder boundary geometry

CheckShellGeometry covers the entire cylindrical chart with bounded field/native
membership cones and trim/root witnesses. It proves both directed boundary-set
distance bounds for qualified full-disk material shells, including the retained
3D field-wire allowance. Explicit geometry budgets and cancellation refuse
unproved results. FreeCAD exposes owned geometry reports, separate from topology
and tolerance audits; solid validation stays false. Focused kernel checks pass
35/35 (24.846 s), and runtime/cylinder/solid-I/O checks pass 9/9 (18.00 s).
Full native regression passes 156/156 (CTest 331.56 s); syntax and whitespace
checks pass. Solid/material/volume, partial/multi-sheet support,
persistence and GUI workflows remain open. See native-cylinder-geometry.md.

## Earlier prerequisite after milestone 96: trim tolerance coherence

Generated implicit cylinder faces now declare the requested trim tolerance after
the sum of contour and cylinder/chord bounds fits it. Source field tolerances and
records remain unchanged. CheckShellTolerance audits that combined bound and all
stored face/edge/vertex tolerances, including required face/edge allowances.
FreeCAD exposes owned audits and inspection; geometry and solid flags stay false.
Focused native checks pass 33/33 (7.857 s), including source tolerances 1e-6 and
0.5 against a 0.05 trim request. Fork runtime/cylinder/solid-I/O checks pass 8/8
(14.11 s). Full native regression passes 154/154 (CTest 312.89 s); Python syntax
and whitespace checks pass. This does not replace global geometry,
volume, partial/multi-sheet or GUI/persistence requirements.

## Milestone 96: closed native cylinder shell topology

BuildNativeShell assembles the qualified native implicit face, retained cylinder
face and material-selected cap into an actual TopoDS_Shell. Opposite edge uses,
paired-p-curve self seams, ordered traversal joins, connected cyclic vertex links,
connected face adjacency and Euler characteristic 2 gate closure. FreeCAD exposes
owned shell query handles and detached previews with topology counts. Geometry
and solid validation remain false. Focused kernel checks pass 32/32 (6.686 s);
runtime/cylinder/solid-I/O checks pass 7/7 (12.06 s). Full native regression
passes 153/153 (CTest 300.11 s); Python syntax and whitespace checks pass.
Partial/multi-sheet support, solid geometry/volume, graph persistence and GUI
workflows remain open. See [native-cylinder-shells.md](native-cylinder-shells.md).

## Milestone 95: full-disk native implicit face attachment

ProjectedRegions now retains planar reference topology and source-wire mappings,
with exact circular projections for side seams and affine cap projections.
Full-disk correspondence additionally proves opposite field signs over both end
disks and a strict axial derivative sign over the cylinder. This sufficient case
has exactly one connected sheet. BuildImplicitFaces attaches its shared wire to
an actual cylinder-restricted implicit field face, checking effective incidence
and material orientation. Partial/multi-sheet correspondence remains unqualified.
The FreeCAD source adds owned native-face query handles and detached side/cap
previews; no shell/solid or GUI workflow is inferred. Focused checks pass 31/31.
Full native regression passes 152/152 (307.735 s); fork runtime/cylinder/solid-I/O
checks pass 6/6 (12.39 s). Python syntax and whitespace checks pass.
See [native-cylinder-implicit-faces.md](native-cylinder-implicit-faces.md).

## Earlier prerequisite after milestone 94: axial sheet qualification

CheckAxialSheet adaptively covers the cylinder with Hessian gradient cones and
requires a single strict axial derivative sign everywhere. Resource and precision
failures refuse; late cancellation propagates. Positive lower bounds publish
only after complete coverage. This is an at-most-one-root-per-axial-fibre gate,
not existence, projected region correspondence or component/face attachment.
General folded/multi-sheet handling remains required by the full roadmap.
Focused cylinder/cap checks pass 29/29 (6.181 s); fork runtime/cylinder checks
pass 4/4 (2.20 s). Full 150-case regression has not been rerun for this new guard.

## Earlier prerequisite after milestone 94: cylinder field-boundary incidence

BuildTrimBoundary audits field-edge single incidence and opposite paired uses
of every other conventional side/cap edge, then constructs oppositely oriented
closed field-boundary wires by native vertex identity. Open/winding/full/empty
and cap-only interior-loop fixtures exercise both material signs. It does not
assign wires to implicit surface components or construct implicit faces/solids.
The next gate is an interior component reference and wire assignment; existing
box-shell behavior requires that qualification before face attachment.
Focused native checks pass 28/28 (6.198 s); adjacent runtime/cylinder checks pass
4/4 (2.38 s). Full 149-case regression has not been rerun for the new API.

## Milestone 94: native cylinder-side material regions

BuildSides now assembles actual cylindrical faces from seam-split bounded field
edges, shared cap rings and paired-p-curve axial seams. Directed UV occurrences,
material orientation, signed area, bounded nesting and BRepCheck gate the faces.
Side contour and cylinder/chord deviation policies stay distinct. FreeCAD owns
the shared graph through build_cylinder_sides/inspect_cylinder_sides and returns
detached conventional face previews. No mesh triangles become trim faces.
Focused native checks pass 27/27; full native regression passes 148/148
(317.114 s). After the final report rebuild, runtime/cylinder/solid-I/O CLI checks
pass 5/5 (12.50 s). Implicit-face attachment, shell/solid qualification,
combined graph persistence and live FreeCAD GUI/source workflows remain open.
See [native-cylinder-sides.md](native-cylinder-sides.md).

## After milestone 93: shared periodic chart and cap regions

The side tracer retains cell/node provenance and continuous UV coordinates.
BuildChartSkeleton splits circle wrap arcs and constructs axial seam edges with
paired p-curves. BuildChartCaps reuses that single boundary snapshot for disk
tracing and actual planar cap regions, preserving identical root/edge handles.
Both material signs, full/empty regions, complementary areas and late failure
cleanup pass the new focused shared-snapshot case. Cylinder-side material cycles,
implicit-face attachment and closed-solid qualification remain open. No FreeCAD
source or GUI behavior changes in these kernel prerequisites.
Full kernel regression passed 144/144 (301.233 s). A subsequent face probe found
and corrected the seam-p-curve branch order; post-correction focused checks pass
24/24, including actual complete-cylinder face validity, area and classification.
The full 145-case suite has not been rerun after the correction. Four adjacent
FreeCAD runtime/cylinder/solid-I/O tests pass (11.36 s).

## Milestone 93: native planar cap regions

BRepClass3d_ImplicitCylinder now builds actual bounded planar cap faces with
shared root vertices, analytic circles, bounded field seams and matching p-curves.
Directed cycles, material orientation, nesting, finite-area guards and ordinary
OCCT face validation qualify both caps; holes/islands and complementary signs
are tested. Work is bounded/cancellable and failed batches publish no faces.
FreeCAD exposes owned native snapshots plus detached conventional face previews
through build_cylinder_caps/inspect_cylinder_caps. Field definitions/settings
remain authoritative. No cylinder-side, shell or solid proof is promoted.
Focused kernel passes 5/5 (303 ms); cap/runtime CLI passes 2/2 (0.78 s).
Full kernel passes 141/141 (294.85 s; CTest total 294.86 s), and adjacent
cap/section/plane/snap/solid-I/O/runtime compatibility passes 6/6 (20.74 s).
See [native cylinder caps](native-cylinder-caps.md). Full Stage 4 remains open.

## Milestone 92: constrained disk contour graphs

Native TraceGraph retains accepted Cartesian cell/root identities. Cylinder
TraceEndCapDisks uses them and narrow shared circle-root brackets to retain
open contours plus interior closed loops without proximity welding. Ambiguous
attachments, critical contacts and failed batches publish no disk curves or
success flags. FreeCAD exposes copied reports with a separate disk qualification
flag; this is not cap-region or solid validation.
Focused new kernel passes 4/4 (238 ms); final cylinder/runtime CLI passes 2/2
(1.46 s). Native cap regions, cylinder trims and full Stage 4 remain open.
Full kernel regression passes 136/136 (294.92 s); syntax/whitespace pass.
Final cylinder/plane/face-snap/runtime compatibility passes 4/4 (11.82 s).

## Milestone 91: native cylindrical query-domain storage and I/O

BRep_TImplicitCylinderPatch stores an analytic finite-cylinder field restriction;
domain, projection and ray queries honor it with source placements retained.
Schema 3 preserves this known subtype, while primitive schemas 1/2 are unchanged.
Unsupported topology/subtypes reject and malformed reads are atomic. Existing
sphere/box consumers refuse the subtype safely. FreeCAD exposes immutable owned
faces through restrict_cylinder_local and versioned dumps/loads, not Part shapes.
Focused restriction checks pass 2/2; CLI/runtime passes 4/4 (5.27 s).
An initial full run hit the 420-second timeout without assertion failure. Exact
base-face fast paths now avoid irrelevant patch dispatch without bypassing box
exclusions or derived restrictions. Final focused cylinder passes 11/11 (1.151 s);
expanded CLI/runtime passes 5/5 (19.33 s). Full kernel passes 132/132 (417.75 s;
CTest total 417.77 s) under the unchanged 420-second limit. Syntax/whitespace pass.
Cap regions, cylinder trim topology, solids and broader Stage 4 remain open.
See [native cylinder sections](native-cylinder-sections.md).

## Milestone 90: native analytic boundary-circle edges

BuildBoundaryRings creates actual OCCT circle edges sharing the side trace's
cap-root vertices, with bottom/top orientation and field-sign interval checks.
Root-free rings use two shared-vertex semicircles. Failed batches publish no
edges. FreeCAD exposes copied descriptors with a separate ring qualification
flag; cap regions, cylindrical trim faces and Boolean solids remain open.
Final focused kernel passes 9/9 (1.265 s) and CLI/runtime 3/3 (2.65 s), including
strict rejection when ring precision or edge tolerance exceeds the requested
budget. A first full run was intentionally stopped for that guard before rebuild.
Final full kernel passes 130/130 (348.92 s); syntax/whitespace pass.
See [native cylinder sections](native-cylinder-sections.md).

## Milestone 89: shared cylinder end-cap root anchors

TracePeriodicBoundary retains the side trace's canonical OCCT root vertices,
brackets and ordered root IDs, with exact open-curve endpoint incidence.
Unresolved/inconsistent traces publish no anchors. FreeCAD exposes copied root
descriptors and a separate incidence flag. Native cap regions, cylindrical trim
edges and Boolean solids remain open; IDs are snapshot-local, not edit history.
Focused kernel passes 6/6 (1.259 s); CLI/runtime passes 3/3 (2.66 s).
Full kernel passes 127/127 (351.19 s; CTest total 351.20 s); syntax/whitespace pass.
See [native cylinder sections](native-cylinder-sections.md).

## Milestone 88: shared periodic cylinder seam

The native cylindrical query now supports explicit full-turn periodic charts.
One indexed seam root is reused on both chart sides; no distance welding or
guessed closure. Only height boundaries can contain open graph ends. Closed
loops reuse their exact first point. FreeCAD exposes periodic=True with a strict
bool policy and separate false topology/solid flags. Settings/Result layouts
and records are unchanged. Focused kernel passes 5/5 (1.218 s), including analytic
closed-loop coverage and moved-seam equivalence; CLI/runtime passes 3/3 (2.79 s).
Full native regression passes 126/126 (340.35 s); syntax/whitespace pass.
Native end-cap joining, curved trim topology and Boolean solids remain open.

## Milestone 87: native open-cylinder section query

OCCT now traces finite open cylindrical patches against exact gyroid faces with
curvature-aware derivative bounds and physical cell-diameter acceptance.
FreeCAD exposes the owned transient query through ImplicitOCCT. Periodic joining,
end-cap roots, native curved trims and cylinder Boolean solids remain open.
See [native cylinder sections](native-cylinder-sections.md).
Focused kernel passes 3/3 (0.48 s); cylinder/plane CLI/runtime passes 3/3 (2.74 s).
Full kernel passes 124/124 (339.05 s; CTest total 339.07 s). Adjacent face-snap/
solid-I/O CLI/runtime passes 3/3 (22.93 s). Syntax/whitespace pass; native and
migration LastTest.log retain the full kernel and adjacent checks, respectively.
No packages/system libraries or existing type/record layouts changed.

## Milestone 86: native gyroid as the primary toolbar workflow

Implicit_NativeGyroid stages mathematical field/box records without document
owners, then publishes the editable field, expression-driven box, native boundary
and validated solid in one transaction. Default dimensions/period are 10 mm,
tolerance 0.1 mm, topology depth 16 and independent geometry depth 18. The latter
supports the documented 20 mm resize case without weakening validation.
Original field-display command remains available in the menu. The task shares
Part dispatch's cancellable worker pipeline, now in a scrollable panel.
Focused create/resize/save/reopen/undo/redo GUI/runtime passes 3/3 (37.31 s).
Expanded gyroid/Part/default-creation GUI/runtime passes 5/5 (79.85 s), including
four-owner rollback after injected publication failure and document-close cancel;
synchronous native construction is forbidden by the new test. Syntax/whitespace pass.
See [workbench guide](implicit-modelling-workbench.md). Full Stage 4 remains open.

## Milestone 85: representation-aware standard Part commands

Part Common and box-first Cut now stage native boundary and solid jobs before
publishing both live owners in one undo transaction. Explicit rejection prevents
unsupported native selections from entering generic Part BOP. Ordinary B-Reps
keep stock dispatch. See [native Part Booleans](native-part-booleans.md).
OCCT is unchanged; full Stage 4 remains open.
Expanded Part/creation-default/solid-creation GUI/runtime checks pass 5/5
(80.08 s), including Part 17.45 s. Syntax and CRLF-aware whitespace pass.

## Milestone 84: native mathematical face Near queries

OCCT BRep_ImplicitTool::Project adds a bounded numerical normal-foot query with
stationarity and tangent minimum checks, line search and an admissible nearby
sign bracket. Explicit domains, patch predicates and excluded boxes are honored;
the whole bracket is checked against exclusions. Invalid settings throw and
unresolved/critical/precision/budget cases report failure. It is a numerical local
candidate, not global nearestness, arbitrary trim support or interval proof.

FreeCAD native Near snapping now uses it for mathematical faces. Conventional
planar caps use their actual trim classifier and boundary edges, including holes.
Native implicit faces never enter Part conversion or UV classification. Rigid
path placement is applied once; geometry/proof state stays unchanged. The 0.1 mm
coordinate policy remains independent of seam/solid/display qualification.

Rebuilt TKBRep, native-face tests and FreeCAD Implicit/Draft using two compile
jobs and one link job. No dependency/system-library changes. Public API is added
without changing existing layouts or records. Focused kernel tests pass 2/2;
full kernel passes 121/121 (333.05 s), retained in its separate LastTest.log.
Expanded CLI/face-GUI/Draft-Line/runtime passes 5/5 (28.76 s), including face GUI
10.44 s, proving distinct mathematical projection from actual triangle hits.
Cap results compare to independent ordinary OCCT distance queries, including
outside/holed faces. Both-operation face/edge/element/link/placed-query CLI/runtime
passes 6/6 (19.16 s), face CLI 11.38 s. Final solid-display/face-snap/Line GUI/runtime
passes 5/5 (50.71 s), with GUI 27.49/10.10/10.29 s; migration LastTest.log retains
that run. Production code did not change after the full kernel run.
Syntax/whitespace pass. The harness now
registers 88 tests with GUI/Draft enabled; only affected subsets were run.
See [face snapping](native-face-snapping.md). Stage 4 remains active; global face
constraints, intersection/history, curved mixed operands and broader consumers
remain open.

## Milestone 83: Draft consumes native vertex/edge snaps

The core now exposes Implicit.snapNative on owned selected geometry. Endpoint
uses stored topological vertices, Midpoint uses finite-edge arc length, and Near
clamps to the stored piecewise linear B-spline segments. Local calculations apply
qualified path placement once and reject coordinate precision exhaustion. No
display mesh, implicit-face projection or proof mutation is involved.

Draft dispatch recognizes native geometry before Part conversion. Its existing
mode switches, winner selection, markers, constraints and working-plane projection
consume native candidates. Auto-alignment safely skips non-Part ComplexGeoData.
The actual Draft Line command accepts native viewport endpoints and creates a
conventional coordinate-based line, without embedding an implicit field in Shape.
Face/intersection snapping and surviving-element history remain open.

The active build enables Draft and its required TechDraw/PartDesign/Spreadsheet/
Import flags. The corresponding dependencies, SketcherGui and FreeCAD's bundled
lazy_loader were built from existing sources/dependencies. Draft's CMake target
now includes required libraries/helpers for minimal builds. No packages/system
libraries or OCCT source/header/ABI changed; two compile jobs and one link job.

Native CLI initially passes (3.19 s). After resolving the previously unbuilt
runtime dependencies, real Draft GUI/runtime passes 3/3 (13.58 s), GUI 10.64 s:
actual picks/markers, conversion guard, native-to-Draft Line creation, working-plane
behavior, stale/unsupported/unavailable inputs, Part compatibility and save/reopen.
Adjacent native snap/element/link/placed-classification and solid-display/feedback
GUI/runtime passes 8/8 (59.13 s), including native snap 3.16 s, solid GUI 27.00 s
and feedback GUI 23.04 s. Its LastTest.log retains that final run; syntax and
whitespace pass. No snap/geometry code changed after the successful Draft GUI run.
Core tests cover stored vertices, finite projections, arc length, immutable
proof/revision state, path placement, precision and snapshot ownership. There are
86 migration tests with GUI/Draft enabled; only affected subsets were run.
See [Draft snapping contract](draft-native-snapping.md). Stage 4 is still active.

## Milestone 82: live input instances and exact decimal corners

Same-document whole App::Link inputs, nested aliases and translated box containers
now enter native creation/recompute/task paths through effective matrix resolution.
Gyroid instances must remain effectively identity placed; general transforms,
scaled/array/subelement inputs, cross-document edits and curved operands remain
open. New global-scope recipe links and legacy dynamic-link migration remove
container scope warnings. Instance fingerprints retain target identity and poses;
direct-root fingerprints retain their previous format. Shallow shape wrappers
preserve source B-Reps and location precision. Container placement/group changes
touch dependent recipes; normal worker guards reject stale poses. Shared primitive
editing resolves targets and invalidates aliases/peers before staged jobs.

OCCT's native boundary codec now preserves binary64 vertex coordinates through a
private max_digits10 ShapeSet. Its previous 15-digit output lost decimal corner
identities, causing valid translated boundaries to fail private clone validation
inside solid construction. Generic B-Rep output, record format, exact graph
validation and 0.1 mm checks are unchanged. Errors now identify failed invariants.
TKTopAlgo and native-face tests rebuilt; only FreeCAD ImplicitScripts rebuilt.
No ABI/header, dependency or system-library changes.

Full kernel 119/119 passes (345.52 s). Headless parameter/mixed/solid-codec/live
compatibility passes 5/5 (52.47 s). Initial instance CLI/runtime passes 2/2 (9.92 s)
and GUI/runtime passes 3/3 (34.56 s). Subsequent GUI instance, feedback, creation,
task-source and parameter checks all pass (32.67/25.61/19.85/74.35/35.42 s).
The combined run's CLI negative fixture needed a saved external document before
FreeCAD would permit constructing the link; both documents are now saved first.
Final instance CLI/GUI/runtime checks pass 4/4 (43.03 s), CLI 10.16 s and GUI
30.62 s, including clean same-math retargeting rejection before publication.
Syntax/whitespace pass. Separate native/migration LastTest.log files retain the
full kernel and final instance runs, respectively.
The harness registers 84 tests; only relevant subsets were run. See
[input instances](native-input-instances.md). Wider Stage 2–4 gates remain open.

## Milestone 81: live native build feedback

Mixed boundaries and native solids now have tree state badges and a read-only
BuildFeedback View property. Live pending/failure/dirty-input/unverified/ready
feedback preserves the separation between boundary seams, native solid validity
and display qualification. Whole-object selection survives geometry refresh,
keeping the property panel attached; snapshot-local subelement selections are
still cleared, including link selections.

The native App property subclass supplies a normal editor for a dynamic Python
value. Transient feedback is calculated on read and recreated on attach. Weak
view polling emits icon notifications/repaints without status property writes,
recompute or proof changes, including during unrelated transactions. A failed
owner remains inspectable rather than being treated as deleted.

Implicit and ImplicitScripts rebuilt with two compile jobs and one link job.
Initial GUI/runtime passes 3/3 (26.47 s); expanded feedback/selection/surface/
creation compatibility passes 7/7 (121.42 s), including feedback 42.68 s. Actual
property text and icon changes, failure/repair, restore provenance, foreign
transaction repaint, undo and stale selection invalidation are covered.
Final GUI/runtime passes 3/3 (29.20 s), including feedback 26.02 s, reselected
property text after undo and empty watcher/stopped timer after closure. Its
CTest LastTest.log retains that run; syntax/whitespace checks pass.
The harness registers 82 tests; relevant subsets were run. No OCCT/dependency
changes. Broader source families, default native-solid primitive exposure,
curved operands, snapping/history and cross-workbench consumers remain open.
See [build feedback](native-build-feedback.md).

## Milestone 80: background defaults for GUI creation

Mixed Common/BoxMinusField selection now opens a cancellable boundary-creation
task, with no provisional owner/transaction during numerical work. It captures
logical inputs and original B-Rep records, guards controls/dirty inputs/foreign
transactions and publishes the completed boundary atomically. Transient meshing
does not invalidate unchanged logical inputs. Defaults are 0.1 mm, 512 tracing
cells, Regularized corners and hidden operands after success; visibility and
creation share one undo transaction.

New GUI boundary and solid owners have BackgroundRecompute=true, so the normal
GUI sequence gets background source-to-solid property edits without manual flags.
GUI solid creation does not alter existing source flags. Public synchronous
factories and legacy/restored false policies retain their behavior. No loaded
topology is silently revalidated. Basic Add Gyroid remains the compiled primitive.

Initial creation GUI/runtime passes 3/3 (21.09 s). Existing factory/publication/
solid-creation compatibility passes 6/6 (62.06 s), including GUI solid creation
40.57 s, headless live solid 17.85 s and mixed boundary 0.50 s. Final expanded
creation GUI/runtime checks pass 3/3 (20.27 s), GUI 17.91 s: cancel, strict-corner
failure/repair, actual command routing, transient mesh stability, changed controls,
dirty operands, foreign transactions, visibility undo/redo, native-solid creation,
default background edits, save/reopen provenance and document closure. CTest
LastTest.log retains the final run. Syntax/whitespace checks pass.

Only native-module Python/scripts/tests/docs change; OCCT stays 0a22e0c5cc and
no dependencies changed. The harness now registers 81 tests; targeted subsets
were run. Tree pending/failure feedback, broader sources, default native-solid
primitive exposure and unfinished Stage 2–4 geometry/consumer gates remain open.
See [GUI creation policy](native-gui-creation.md).

## Milestone 79: source preparation in task-panel previews

Both source and native-solid task Apply/OK now use PreparedSolidBatch: all changed
source boundaries prepare privately before their group publishes, then native
solid peers build and publish with their existing guards. Waiting tokens suppress
ordinary recompute fallback; target settings/source recipes and the owned edit
transaction are checked throughout. Queue-full submissions retry. Source and solid
persisted background flags are unchanged. Direct Python apply() stays synchronous.
Task previews can take over ordinary background jobs without taking another task's
tokens; Cancel can resume prior ordinary work through its normal policy.

The new GUI test first exposed fingerprint clearing during rollback. Preserving
the journaled Geometry/fingerprint pair during Document.Transacting avoids
retracing unchanged topology; metadata checks still reject inconsistent restores.
The strict translated-box editor exposed Part.Shape.copy() changing one coordinate's
last bit. Requests now capture original B-Rep strings and use the new facade
start_boundary_job_from_record helper, preserving exact metadata checks and all
geometry/tolerance policies. An old mixed-editor helper was updated to wait for
asynchronous button actions and tolerate compiled view providers without Proxy.

Initial task GUI/runtime 3/3 passes (42.79 s). Existing source-parameter CLI
20.61 s, solid-editor GUI 104.09 s, source-parameter GUI 32.53 s and source pipeline
117.44 s pass. Final boundary/editor/property/runtime checks pass 6/6 (198.18 s),
including strict mixed editor 5.52 s, expanded task GUI 67.71 s and ordinary source
GUI 117.73 s. Final task/shared-batch/runtime checks pass 4/4 (120.40 s), including
task GUI 68.84 s and shared-batch GUI 49.27 s. CTest LastTest.log retains the final
run; changed Python syntax and whitespace pass. Qualification includes
atomic two-source preparation, second-source failure, no synchronous fallback,
changed controls, cancellation/rollback, source preflight repair, deferred OK,
lost/foreign transaction isolation, ordinary-job takeover and document closure.

Only native-module Python, script packaging, tests and docs change; no OCCT or
system dependency changes. The 0.1 mm policy is unchanged. Ordinary property
background scheduling remains opt-in; default native-solid exposure and wider
Stage 2–4 geometry/consumer requirements remain open. See
[task source contract](native-task-sources.md).

## Milestone 78: GUI background source-to-solid recompute

MixedBoundary now has a persisted BackgroundRecompute opt-in, default false.
ImplicitMixedRecompute adopts private boundary jobs with live operand/recipe,
owner/proxy, exact result descriptor and transaction guards. It reuses the solid
coordinator's timer/lifecycle cancellation. Native solids report pending source
preparation without errors; source publication triggers their ordinary recompute,
and opted-in owners then build on the worker. Direct rebuilds and early input
failures invalidate same-document native-solid output immediately; source failure
propagates an explicit unavailable status. Partial publication is cleared.

Initial source pipeline GUI/runtime passes 3/3 (108.61 s). Expanded affected
headless/GUI/property/task compatibility passes 6/6 (226.73 s): pipeline 115.15 s,
solid recompute 75.35 s, source-parameter task 32.75 s. Final direct repair,
unsupported definition, partial-publication failure, restore and lifecycle GUI
checks pass with runtime fixtures 3/3 (143.43 s), GUI 141.21 s. That final run is
retained in migration CTest LastTest.log. Changed Python syntax and whitespace pass.
The headless test saves BackgroundRecompute=true and retains synchronous behavior.

Both source and solid flags must be enabled for the fully background native
pipeline. Serialization, recipe checks, cap display, result conversion, Coin and
document recompute remain on the GUI thread. Task previews with a background
source and default exposure remain unqualified. Only native-module Python,
script packaging, tests and docs change; OCCT remains 0a22e0c5cc and no dependency
changes were needed. The 0.1 mm policy is unchanged. See
[source recompute contracts](native-source-recompute.md).

## Milestone 77: native boundary-preparation worker backend

The shared native executor now prepares supported mixed gyroid/box boundaries
from private primitive-field and conventional B-Rep records. start_boundary_job
and take_boundary_job retain exact field/box/settings descriptors, enforce kind
guards before consumption and share queue/cancellation/shutdown rules with solid
and display jobs. No document/Python/Part object enters the worker; serialization,
report conversion and conventional cap reconstruction stay on the caller thread.
GUI source recompute/task adoption remains open.

OCCT plane tracing now accepts an optional caller checkpoint callback, without
depending on higher-level work-control libraries. Mixed Build bridges it to the
existing thread-local scope and adds root/side checkpoints. The new calibrated
kernel test cancels halfway, retries and compares exact curves/bounds/resolution.
Full kernel 117/117 passes (340.12 s). Initial boundary CLI/runtime 2/2 passes
(7.03 s), covering both operations, private snapshots, numerical equivalence,
calibrated tracing cancellation/retry and mixed queue/failure guards. Expanded
strict-policy coverage uncovered a primitive I/O issue: exact identity matrices
decoded as nonempty locations and strict Build rejected them. The reader now
returns an empty location for exact input identity only; a regression protects
a 1e-15 translation from snapping. Focused kernel 2/2 passes (20 ms); expanded
boundary/shutdown/runtime 3/3 passes (5.91 s), covering both policies/operations
and invalid settings. Final full kernel 118/118 passes (321.56 s) after that I/O
fix. Its CTest LastTest.log retains the result. Final worker/shutdown/runtime/GUI
compatibility passes 7/7 (186.74 s), including checked-display GUI restore
100.46 s, solid jobs 27.31 s and surface jobs 51.54 s. The migration CTest log
retains that run. Syntax and whitespace checks pass; no numerical policies were
relaxed. The migration harness now registers 78 tests; this was a targeted run.

The optional settings callback extends the fork's native extension ABI. Active
OCCT 8 libraries, FreeCAD extension and native test/worker consumers were rebuilt
with matching headers/libraries. No packages/system libraries or OCCT 7 legacy
build changes. Per-algorithm geometry/tolerance policies are unchanged. See
[boundary job contracts](native-boundary-jobs.md). Broader/default GUI scheduling,
curved operands, snapping/history and consumers remain required for Stage 4.

## Milestone 76: opt-in GUI property recompute

BackgroundRecompute is a persisted opt-in native-solid property (default false).
Changed GUI build inputs clear geometry/validation and queue private native solid
jobs. ImplicitSolidRecompute coalesces descriptors, guards owner/source/settings,
waits through foreign transactions and publishes without an extra output
transaction. Undo/redo/abort/deletion/document closure cancel pending requests;
downstream recompute follows successful publication on the GUI thread. Failures
clear partial output and remain repairable. Restored topology is not recertified.

Initial GUI/runtime checks pass 3/3 (51.50 s). Expanded property GUI passes
(81.18 s), adding dirty source rejection, injected partial-publication failure
and persisted setting/unverified topology to transaction/lifecycle/heartbeat
coverage. Expanded GUI/runtime/editor/batch compatibility passes 5/5 (301.68 s),
including editor 129.08 s and shared batch 87.71 s.
Additional abort/expression-consumer and headless tests pass 4/4 (173.25 s),
including GUI 136.70 s and CLI 28.78 s. Final direct-stale-recipe guard and all
expanded GUI cases pass with runtime fixtures 3/3 (130.25 s), GUI 124.49 s.
The final CTest LastTest.log retains this result. Syntax and whitespace pass.
This changes Python components in the native module only; OCCT and system dependencies are
unchanged. Existing Qt 6.12 runtime audits pass. Configuration now observes
installed Python 3.13.16 and CMake 4.4.4; only ImplicitScripts was rebuilt/copied.

This remains opt-in. Headless/default recompute and source preparation remain
synchronous. Broad default scheduling, source families, curved mixed operands,
snapping/history and consumer integration remain open Stage 2–4 requirements.
See [property recompute contracts](native-solid-recompute.md).

## Milestone 75: background unchecked display regeneration

Both display modes now route cache misses through the same native worker.
The checked-mode bit is part of the descriptor/key; switching modes rejects stale
callbacks. Returning to a cached or initial candidate cancels superseded work.
Unchecked output retains candidate-only status and cannot set checked proof flags.
Initial solid-job buffers are still reused. No OCCT or dependency changes.

The expanded async GUI test forbids both synchronous extraction APIs and checks
unchecked coalescing, heartbeat, stale callbacks and mode switching before the
existing checked lifecycle/priority checks. It passes (117.64 s). Existing corner
(28.39 s), checked-display (102.16 s), editor (108.41 s) and source-parameter
(36.20 s) GUI regressions pass on the same implementation. The live-solid test
was updated for the existing staged creation command; it passes (27.72 s).
Creation/restore passes (41.01 s) after waiting for its asynchronous display.
Final focused live/async-display/creation/shared-batch/runtime run passes 6/6
(244.72 s), including shared-batch GUI 55.61 s. CTest LastTest.log retains it.
The first regression run exposed two test assumptions (immediate command creation
and no pending initial views); both were corrected before the focused rerun.

Ordinary solid recompute/source preparation and result conversion/component
assignment/Coin upload still execute on the caller/GUI thread. Broader scheduling,
default native-solid exposure and remaining Stage 2–4 gates remain open.

## Milestone 74: asynchronous checked-display views

NativeSolidView now submits checked extraction through ImplicitSurfaceJobs.
Weak view references, request coalescing, generation/record/settings guards and
live-source checks reject stale results. Display work yields to pending solid
batches; one outstanding display job preserves queue capacity. Hiding, deletion
and document closure cancel work. Checked status requires successful upload of
the current checked buffers; manually setting or restoring the flag cannot prove it.

The actual GUI lifecycle test covers heartbeat, rapid settings changes, stale
callbacks, solid-batch priority, changed source inputs and cancellation. Final
async-display/editor/runtime tests pass 4/4 (231.97 s); existing checked-display
GUI passes separately (103.29 s). Initial cleanup failure was test-only and fixed.
The installed Qt stack required rebuilding the existing GUI targets against
6.12.0; runtime/GUI library audits pass. No packages or system libraries changed.

Unchecked fallback extraction, ordinary property recompute and source preparation
remain synchronous. Conversion/component assignment/Coin upload remain on the
GUI thread. Default native-solid exposure and all broader Stage 2–4 gates remain
open. OCCT is unchanged in this increment. See HANDOFF.md for continuation.

## Milestone 73: native checked-display jobs

OCCT clipped coverage and exact mesh contact auditing now have optional work
phases and cancellation checkpoints. The new test calibrates progress, cancels
midway through each algorithm for both default operations, retries successfully,
and verifies unchanged source records, candidate buffers and proof flags.
Focused 1/1 passes (63.800 s); full kernel 116/116 passes (326.96 s). The harness
timeout increases from 300 to 420 s to accommodate that extra test, with all
numerical budgets/tolerances preserved.

FreeCAD's CheckedDisplay helper performs the same native binary32 displacement,
clipped proximity and rounded-coordinate exact audit without Python references.
Synchronous and worker results share a report converter. Display-only native jobs
reuse the solid executor, retain private source geometry and return buffers with
exact input record/settings descriptors. Kind guards prevent confusing display
jobs with native-solid construction. The existing APIs remain available.

The new display-job CLI smoke passes: checked/unchecked output equals the
synchronous calculation on an identical private snapshot; Python runs while the
worker computes; mixed solid/display queues remain bounded; coverage and audit
cancel; limited work fails without a result; one-shot and source lifetime rules
hold. Earlier checked-display CLI/runtime passes 2/2 (64.59 s) and actual GUI
smoke passes (104.13 s) before the worker extension. Logs originally:
/tmp/implicit-display-cancel-focused.log, /tmp/implicit-checked-native-cli.log,
/tmp/implicit-checked-native-gui.log and /tmp/implicit-surface-jobs-cli.log.
Full kernel evidence is retained in the native-face-tests CTest LastTest.log.
Final mixed-job compatibility/runtime/shutdown tests pass 4/4 (32.54 s), including
normal process exit during coverage and the existing solid-job lifecycle suite.
Log: /tmp/implicit-surface-job-compatibility.log.
The expanded display-job/runtime run passes 2/2 (51.61 s), adding the complementary
BoxMinusField display path and exact snapshot/output comparison. Log:
/tmp/implicit-surface-jobs-qualified.log.

The GUI is not yet connected to display-only jobs. Ordinary property recompute,
source tracing and existing checked display remain synchronous. The next change
must enforce view generation/digest/settings and same-buffer upload checks while
coalescing/cancelling jobs. Default Add Gyroid and every unfinished Stage 2–4
curved-operand/snapping/history/consumer gate remain open. No packages or system
library changes.

## Milestone 72: asynchronous shared-solid editor previews

GUI Apply/OK now use a main-thread SolidBatch coordinator over the existing
bounded native worker. Affected peers are invalidated, built sequentially and
published only after the whole batch succeeds and its source/settings/document/
transaction descriptors still match. Pending recompute cannot fall back to a
synchronous solid build. Fresh results feed the existing initial-preview cache.
Cancel rolls back owned edits; lost transactions never permit mutation of an
unrelated transaction. Changed controls/inputs, missing owners and document
closure discard work. Unchanged loaded solids are not silently revalidated.

The first shared-parameter GUI run exposed FreeCAD's single-object recompute
semantics: successful evaluation leaves Touched set. SourceEdits now clears only
the successfully evaluated field/box leaf flags, not downstream invalidation or
later external edits. Runtime/batch/shared-source CLI and GUI checks then passed
6/6 (168.70 s), including multi-owner Qt heartbeat, forbidden synchronous native
build/meshing during publication, failure/retry, transaction ownership, undo/redo,
save/reopen, changed controls and close-during-work. An expanded CLI additionally
passes owner deletion during a pending batch. Logs: /tmp/implicit-batch-final.log
and /tmp/implicit-batch-deletion.log.

Final review added explicit invalidation when preflight/submission fails after
settings were changed but before a batch acquired its owners. The GUI test injects
this failure and requires old geometry/validity to be cleared before rollback.
The final affected GUI/runtime suite passes 5/5 (261.73 s): editor 114.76 s,
shared parameters 49.21 s, batch lifecycle 94.23 s, plus runtime fixtures.
Log: /tmp/implicit-batch-qualification.log. A final compatibility adjustment uses
an optional Proxy lookup for compiled native boundary owners; its dedicated CLI
case covers the same worker/publication path without a Python source recipe.
Final expanded CLI/runtime passes 2/2 (6.66 s), including deleted owners and the
compiled-source case; log /tmp/implicit-batch-owner-qualification.log.
No OCCT, dependency or system-library changes. Direct Python task.apply(), ordinary
property recompute, source boundary preparation, checked/custom display and some
rollback rendering remain synchronous. Global result memory budgets, default
native-solid exposure and all other Stage 2–4 gates remain open.

## Milestone 71: asynchronous native-solid creation

The actual creation task now submits/polls native work, exposes phase text and
retains responsive Cancel. Document/form closure cancels outstanding requests;
changed settings, dirty dependencies, changed source records and unrelated
transactions prevent publication. Numerical work holds no document transaction
and creates no temporary owner. Ready results publish atomically and seed the
view's transient candidate cache without synchronous re-meshing. Component
assignment, native picking and scene-upload failure policies remain intact.

Initial GUI/runtime 3/3 passed (65.11 s), with a Qt event-loop heartbeat and
synchronous solid/mesh calls forbidden during successful construction/publication.
The expanded lifecycle test initially stopped on fixture selection after a
source edit: recompute correctly cleared that selection. The test now explicitly
reselects the source; production code did not change for that failure.
Tests also exercise Cancel, setting/source changes, external transactions and
document closure during work, alongside the existing 20-mm/0.1-mm build,
failure/retry, visibility undo/redo and unchanged FCStd restore.
Final creation/editor/runtime regression passes 4/4 (180.37 s): unchanged editor
125.25 s and expanded creation 48.81 s, plus runtime fixtures. Log:
/tmp/implicit-async-create-final.log. A subsequent capture-only test adjustment
adds camera margin for visual inspection with the headless report dock open.
Final capture-adjusted GUI/runtime 3/3 passes (46.17 s), with production code
unchanged from the 4/4 regression. The capture was inspected: native surfaces,
edges and vertices render; the headless dock layout still crops the top of the
model, so it is not a full-model visual qualification. Log:
/tmp/implicit-async-create-margin.log.

Existing editing/recompute and checked-display extraction remain synchronous.
Source serialization, Python result conversion and scene uploads are still
main-thread operations. No default-command switch or Stage 4 completion claim.
See [workflow](native-solid-editor.md) and [job contracts](native-solid-jobs.md).

## Milestone 70: background native-solid backend

OCCT thread-local work scopes propagate cooperative cancellation through the
existing implicit call graph without changing tolerance or geometry policies.
FreeCAD now owns a bounded native executor (one worker, one queued request)
with private source snapshots, status/phase polling, cancellation, one-time
fresh-result consumption and joined shutdown. The initial display candidate is
transient and unverified; no Python/document/Coin objects enter the worker.

An internal atomic owner-publication path checks exact input/settings and fresh
handle provenance, rejects dirty dependencies/external transactions, and reuses
the completed native topology without rebuilding. Public create is unchanged.
This backend is not yet connected to the creation dialog; existing GUI creation,
editing and checked-display execution remain synchronous.

Verification: full kernel 115/115 (254.34 s), focused kernel 2/2 (17.975 s), worker/checked-display/runtime
3/3 (86.58 s), publication/shutdown/live/settings/runtime 5/5 (38.54 s).
Logs: /tmp/implicit-work-control-focused.log,
/tmp/implicit-solid-jobs-regression.log and
/tmp/implicit-solid-publication-regression.log. Tests exercise Python heartbeat,
both operations, synchronous/async topology equivalence, phase cancellation,
bounded queue/capsule cleanup, budget failure/retry, source lifetime, normal
shutdown during work, stale publication and undo/redo. See
[job contracts and GUI follow-up](native-solid-jobs.md). No packages or system
library changes; all remaining Stage 2–4 gates stay active.

## Milestone 69: creation-time native solid settings

The native-solid menu command stages build settings before initial construction.
The new task shares limits/defaults with the existing editor and factory. Cancel
does not create an owner; failed OK removes temporary geometry and retains the
task for adjustment. Success can hide the source in the same undo transaction.
The factory retains its old positional API, with keyword build_limits and
hide_source. Source type/geometry and integer limit checks precede mutation.

CLI/runtime 4/4 (58.61 s) passes for legacy settings/fingerprints, live owners and
shared parameter editing. Initial real GUI creation smoke passes for the shifted
20-mm gyroid at 0.1 mm with geometry depth 18, budget failure/retry, no build on
open/Cancel, no orphan objects/transactions, source visibility undo/redo, and
unchanged saved topology. Final affected GUI/runtime regression passes 5/5
(194.17 s): creation 36.96 s, existing editor 109.37 s and source-parameter editor
45.76 s, plus runtime fixtures. Controls/buttons are fully contained in the
visible task viewport/window. The settings and result screenshots were visually
inspected after camera fitting settled. No OCCT changes or packages;
construction remains synchronous. See [workflow](native-solid-editor.md).

## Milestone 68: shared primitive source-parameter task editing

NativeSolidTaskPanel stages gyroid Period/IsoValue and Part::Box dimensions using
the new GUI-free ImplicitSourceEdit helper. The field/box stay authoritative.
All mixed sources sharing these inputs and their native solid peers are
invalidated before edits and rebuilt explicitly within the task transaction.
Numerical failure clears native geometry/display/check flags instead of leaving
old results presented as valid. Repair, Cancel, undo and redo restore the shared
state through the existing owned lifecycle.

Canonical workbench DomainMax expressions survive fitting by editing their
length inputs. Ordinary API fields without aliases also work; missing-property
introspection is guarded because FreeCAD raises AttributeError. Custom expressions
and external edits are protected; cross-document mutations remain unsupported.
Untouched rounded controls preserve exact authoritative doubles, including solid
tolerance, so unchanged FCStd edits need not rebuild or revalidate saved topology.

New CLI and GUI smoke pass for a 10-to-5-mm unit cell at 0.1 mm, shared
peers, preserved expressions, singular-level rejection, rollback/repair, undo/redo
and unchanged saved topology. Full migration/GUI: 65/66 pass (778.00 s), with
only the new viewport-containment assertion failing. Qt ensureWidgetVisible
uses a spin box's input-method cursor rectangle; the test now scrolls the full
control rectangle with ensureVisible and still requires complete containment.
Final focused GUI/fixtures: 3/3 pass (48.30 s), including a visually inspected
capture. Expanded CLI/runtime checks pass 2/2 (22.60 s), covering distinct
Common and BoxMinusField sources sharing inputs and a saved external App::Link
that blocks mutation before geometry is invalidated. Initial external-link
fixtures needed both documents saved, as required by App::XLink. No production
changes followed the full run. No OCCT changes, tolerance/budget relaxations, package installs
or default-command changes. General field graphs/source replacement, creation
controls, scheduling, curved/general BOP, snapping/history and cross-workbench
consumers remain open. See [editor contracts](native-solid-editor.md).

## Previous increment: live native-solid task editor and independent geometry depth

The live owner exposes MaximumGeometryDepth=-1 or 1..24 and forwards only a
non-default value as a ninth native input setting. Default descriptors/calls and
legacy fingerprints are unchanged. Missing-property migration preserves actual
FCStd topology without numerical rebuilding or restoring proof flags. Atomic
creation accepts the new option and traps expected initial numerical failure
before normal document recompute. The shifted 20-mm live Part::Box qualifies at
the same 0.1-mm target with topology depth 16/geometry depth 18; shared defaults
fail and creation rolls back completely.

That test exposed a corner-policy location guard. The OCCT recognizer now admits
only exact pure translations on conventional boxes/subobjects; world-space exact
vertex/line/plane identities, orientations, domain/precision/corner checks remain.
No source is snapped, baked or changed. Located and directly built boxes yield
identical canonical topology records for both operations, with qualified solids.
Rotated/scaled operands and placed fields remain rejected, not fuzzy-canonicalized.

The new NativeSolidView task panel stages solid geometry/reference/depth/work
settings, followed placement and the same-document live shared-source operation.
It reuses owned-transaction lifecycle, warns of shared-source effects, protects
external transactions, and leaves stored/external source operations read-only.
Preview failures clear geometry/buffers/checked state and support repair/Cancel.
Tasks are shown explicitly; tests wait for actual animated-overlay visibility
before clicking real standard buttons. See [contracts](native-solid-editor.md).

Focused kernel 1/1 (7.629 s), direct larger/legacy CLI smoke, initial GUI fixtures/
test 3/3 (104.35 s, before the read-only-source safeguard), and full kernel
113/113 (242.49 s) and compatibility 24/24 (38.48 s) pass. Full migration/GUI
64/64 (757.00 s) passed before the final layout/core GUI fix described below.
The initial GUI failures were hidden overlay controls, not ignored button tests;
the final test pumps Qt's event loop with a bounded wait and still requires them
visible. An initial location-immutability assertion compared separately allocated
TopLoc datums; it now compares the preserved original datum identity.

Visual inspection exposed clipped Cancel/body controls despite Qt isVisible.
The final module wraps rows above fields and uses the native task scroll area.
Gui OverlayWidgets now honors active task child minimum sizes and reanchors
expanded rectangles; TaskView refreshes overlays after its delayed minimum-width
update. Inactive sizing policy is unchanged. Tests require complete action-button
visible regions within the window and an actually visible geometry-depth entry.
Final focused GUI/runtime 3/3 (112.18 s) passes; its screenshot was visually
inspected. The complete GUI/runtime/demo subset passes 15/15 (324.33 s) after
the core GUI fix, including the final editor test (109.69 s). Native math/CLI
code is unchanged from the full 64-test run.
Upstream GUI file CRLF is preserved; native whitespace checks use the temporary
git -c core.whitespace=cr-at-eol option, not a repository/global setting change.

Logs: `/tmp/implicit-solid-editor-occt-build.log`,
`/tmp/implicit-solid-editor-kernel-build-final.log`,
`/tmp/implicit-translated-corner-box-focused-final.log`,
`/tmp/implicit-solid-settings-focused-final.log`,
`/tmp/implicit-solid-editor-scripts-final.log`,
`/tmp/implicit-solid-editor-gui-await-controls.log`,
`/tmp/implicit-solid-editor-kernel.log`,
`/tmp/implicit-solid-editor-migration.log`,
`/tmp/implicit-solid-editor-compat.log`,
`/tmp/implicit-solid-editor-overlay-build.log`,
`/tmp/implicit-solid-editor-overlay-gui-final.log`,
`/tmp/implicit-solid-editor-overlay-gui-regression.log`.
Actual GUI capture: `/tmp/implicit-solid-editor.png`.

All unfinished Stage 2–4 requirements remain active: broader source parameter/
dimension/link editing, scheduling, curved operands, snapping/history, consumers
and placement workflows. Default Add Gyroid unchanged; no new packages/system
changes or accuracy/algorithm-budget relaxation.

## Previous increment: opt-in same-buffer checked native-solid display

FreeCAD's new tessellate_mixed_checked extracts one candidate, bounds the actual
binary32 upload displacement, checks original clipped proximity against tolerance
minus that reserve, and exact-audits rounded triangles before returning those
coordinates. Both directed bounds include the upload error. The separate
display_checks contract is source-local binary32, not topology/solid validity,
directed intervals or world-placement/GPU/pixel precision. Legacy candidate
SurfaceCoverageVerified and SolidValidated flags stay false. Canonical pre-upload
coordinates retain actual native component assignment; displayed displacement
must remain within the report's bound.

NativeSolidView adds opt-in CheckSurfaceDisplay and read-only
DisplayProximityChecked. Digest/tolerance/resolution/mode bind cached checks;
camera and edge/vertex visibility reuse them. Changed inputs clear checked state.
Extraction failure removes the field and cached checks; scene-upload failure
clears scene nodes/aliases/report, never authoritative geometry. Checked status
and successful cache keys are published only after a complete upload. Recompute
can retry repaired failures. FCStd stores the setting, not transient proofs;
fresh restore checking does not upgrade native-solid qualification.
See [contracts and workflow](native-checked-display.md).

CLI/runtime 2/2 (59.19 s) passes with both default operations, restored geometry,
canonical connectivity, exact binary32 coordinates, reduced-budget/invalid input
failure and non-binary32 strict cap planes. Initial GUI fixtures/test 3/3
(100.06 s) passed before the explicit scene-upload failure test and final atomic
publication tightening. Compatibility 24/24 (26.49 s) and full migration/GUI
62/62 (615.43 s) pass, including the final scene-upload failure GUI test
(99.39 s). OCCT is unchanged; its 112-case kernel baseline passed at milestone 65.

Logs: `/tmp/implicit-checked-display-build-final.log`,
`/tmp/implicit-checked-display-scripts-final.log`,
`/tmp/implicit-checked-display-configure.log`,
`/tmp/implicit-checked-display-cli-final.log`,
`/tmp/implicit-checked-display-gui.log`,
`/tmp/implicit-checked-display-migration.log`,
`/tmp/implicit-checked-display-compat.log`.

The default Add Gyroid command remains unchanged. Check execution is synchronous;
broader source/task editing, scheduling, curved operands, snapping/history,
consumer/placement workflows and all other Stage 2–4 requirements remain active.
Existing builds/dependencies only; no accuracy or algorithm-budget relaxation.

## Previous increment: corner-aware clipped-boundary proximity

The explicit native coverage checker now accepts regularized corners using fresh
exact lattice germs and whole-hull Hessian derivative bounds. Uniform 3-D cones
exclude isolated contacts of either sign from the field face. On physical box
sides, two strictly unretained inward derivatives exclude only cells absent
from retained area closure. Checked retained in-plane corner rays provide
reverse cap witnesses. Whole normal-line root slabs must avoid actual canonical
face exclusions; untrimmed field roots cannot bypass face membership.

Existing two-way proximity bounds, numerical reserves and depth/spatial/patch
limits remain. The four new work counters are exposed by FreeCAD. Both default
operations resolve at 0.1 mm: 8 field/18 cap cone exclusions, 102 improving exact
corner witnesses and 51 exclusion/slab rejections each. There is no automatic
GUI certificate, generic Part/BOP dispatch, topology/solid or directed-interval
claim. Source caches/topology/provenance are unchanged, and separate candidate
metadata SurfaceCoverageVerified stays false. See [contracts](native-corner-coverage.md).

Tests cover both operations, independent harmonic roots/raw cap samples, retained
cap-only corners, translated period-5 restored sources, missing surfaces/caps,
forged flags, isolated-contact field/cap triangles and depth/cell/patch limits.
FreeCAD checks periods 10 and 5, translation, owned lifetime, unverified restore
and immutable serialization. Initial focused kernel 3/3 (22.409 s, before the
extra cap-negative assertion) and CLI/runtime 2/2 (36.55 s) pass. Full kernel
112/112 (232.21 s, including the cap-negative test) and compatibility 24/24
(28.22 s), and full migration/GUI 60/60 (456.97 s) pass. The enlarged kernel harness
timeout is 300 seconds (was 240), not
an increased algorithm work budget or relaxed accuracy target.

Logs: `/tmp/implicit-corner-coverage-build.log`,
`/tmp/implicit-corner-coverage-tests-build-final.log`,
`/tmp/implicit-corner-coverage-freecad-build.log`,
`/tmp/implicit-corner-coverage-configure.log`,
`/tmp/implicit-corner-coverage-focused-final.log`,
`/tmp/implicit-corner-coverage-cli.log`,
`/tmp/implicit-corner-coverage-kernel.log`,
`/tmp/implicit-corner-coverage-compat.log`,
`/tmp/implicit-corner-coverage-migration.log`.

Next qualify broader live/task/source edits and fresh GUI buffers before default
native-workbench exposure. Curved operands, snapping/history, consumers and all
other unfinished Stage 2–4 requirements remain active. Existing builds only.

## Previous increment: corner-aware transient mesh and opt-in live display

Prepare now admits regularized shared cap topology without source-cache changes.
Tessellate uses odd per-axis grids and only freshly checked physical corner zeros;
shared roots use grid identity, not distance welding. Native trim corners remain
canonical, and whole field-triangle bounds must avoid both uniform-contact
exclusion boxes. Existing untrimmed-field distance bounds, paired incidence,
cyclic vertex links and resource limits remain. Reports include corner_policy
and fixed_corner_count; candidate closure does not certify coverage or solids.

The first default contact audit exhausted its existing budget. A balanced 3-D
triangle-AABB hierarchy replaces the X-only sweep; exact integer predicates,
shared-simplex rules and all audit budgets are unchanged. BroadTests now counts
node/leaf box comparisons. A 4,096-triangle coplanar sparse case stays under a
small broad limit and detects an injected duplicate. Both default candidates
audit Clear under default limits. This changes broad-phase work, not the exact
contact gate. See [contracts and scope](native-corner-mesh.md).

ImplicitMixed adds explicit CornerPolicy and create(..., corner_policy=...).
Reject fingerprints preserve legacy hashes; regularized fingerprints include
policy and unchanged-input checks reject mismatched geometry. The opt-in live
solid view renders transient field triangles with smooth field normals and
separate cap normals/creases. GUI tests cover actual face/vertex ray picks,
semantic solid/shell/face/edge/vertex selection, both operations, undo/redo and
FCStd restore/re-render without rebuilding saved solids. The default Add Gyroid
command remains unchanged; clipped coverage still explicitly rejects this policy.

Focused kernel **4/4** (26.414 s), runtime/mesh CLI **2/2** (33.36 s), GUI
fixtures/test **3/3** (28.17 s), and full kernel **109/109** (210.09 s) pass.
Remaining compatibility **24/24** (38.94 s) and full migration/GUI **59/59**
(430.74 s) pass. The actual viewport capture was visually inspected: the clipped
field and flat caps render with distinct boundary edges and native vertex markers.
Some display tessellation/shading remains visible; this is not an analytic or
coverage-certified viewport claim. No
packages/system changes or numerical/accuracy-budget relaxations. All unfinished
Stage 2–4 requirements remain active, including coverage, broader live/task edits,
curved operands, snapping/history and default workbench exposure.

Logs: `/tmp/implicit-corner-mesh-build.log`,
`/tmp/implicit-corner-mesh-bvh-build.log`,
`/tmp/implicit-corner-mesh-bvh-tests-final-build.log`,
`/tmp/implicit-corner-mesh-bvh-focused.log`,
`/tmp/implicit-corner-mesh-freecad-build.log`,
`/tmp/implicit-corner-mesh-configure.log`,
`/tmp/implicit-corner-mesh-cli.log`,
`/tmp/implicit-corner-mesh-gui.log`,
`/tmp/implicit-corner-mesh-kernel.log`,
`/tmp/implicit-corner-mesh-migration.log`,
`/tmp/implicit-corner-mesh-compat.log`.
Viewport capture: `/tmp/implicit-corner-native-solid.png`.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

## Previous increment: corner-aware native solids and schema-2 solid topology

The native solid factory now admits regularized corner sources through fresh
whole-face/component/shell attachment, native topology, cap geometry and stored-
tolerance gates. Both default operations qualify at 0.1 mm and construct actual
TopoDS solids: one solid/shell, 13 faces, 30 edges, 19 vertices, boundary Euler -2.
Both isolated contacts stay outside the implicit face; the retained uniform
corner is cap-only. No mesh/Part/generic UV validity claim.

An overload separates cap-geometry depth from topology/reference depth while
preserving the original C++ signature/shared defaults. FreeCAD appends
maximum_geometry_depth=-1 (-1 inherits reference depth). The shifted 20-mm case
fails safely at geometry depth 16, passes at depth 18 with reference depth 16,
and keeps the physical 0.1-mm target. Stored-tolerance/resource/parameter failures
do not publish constructed/validated solid handles.

Solid codec schema 2 now carries regularized sources; strict output remains
schema 1. Source masks must equal canonical corner exclusions; component masks
must equal their source/domain intersection. Restore derives and retains those
restrictions and actual shared topology, with no Boolean retracing. Outer schema
and source policy must agree. Source/cap/native face identity, domain, wire,
shell/material and bounded-tolerance checks remain. Lost component restrictions
reject before output mutation; version-spoofed reads reject atomically.

Restored geometry/topology/validation/Closed flags stay false. Re-saving restored
actual topology is supported; explicit source rebuilding creates a newly checked
result without upgrading the old snapshot. FreeCAD tests exercise both operations,
domain/region queries, source immutability, recheck, native solid properties and
subelements, runtime undo/redo, FCStd close/reopen and owned lifetime. Obsolete
corner-solid refusal assertions/helpers were removed, not geometry/topology gates.
See [contracts and remaining scope](native-corner-solids.md).

Focused kernel **3/3** (36.942 s), runtime/solid CLI **2/2** (33.45 s), full kernel
**106/106** (179.41 s) and compatibility **24/24** (36.98 s) pass. Full FreeCAD
migration/GUI passes **57/57** (359.29 s). Existing targets/dependencies only; no system/package
changes, mesh/view enablement or default-workbench switch. All unfinished Stage
2–4 requirements stay active, including curved operands and snapping/history.

Logs: `/tmp/implicit-corner-solid-build.log`,
`/tmp/implicit-corner-solid-tests-build.log`,
`/tmp/implicit-corner-solid-focused.log`,
`/tmp/implicit-corner-solid-freecad-build.log`,
`/tmp/implicit-corner-solid-configure.log`,
`/tmp/implicit-corner-solid-cli.log`,
`/tmp/implicit-corner-solid-kernel.log`,
`/tmp/implicit-corner-solid-compat.log`,
`/tmp/implicit-corner-solid-migration.log`.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

## Previous increment: corner-aware native cap geometry

CheckCapGeometry now supports regularized side closures. Whole-cell derivative
cones from exact corner jets and Hessian/radius/reserve bounds exclude isolated
unretained planar contacts in the mathematical-to-cap direction. Native candidate
cells are never pruned this way. Strict retained projected points, or exact
corners with a proved retained side ray, provide physical-distance witnesses.
The retained uniform corner is cap-only; an excluded zero is not a witness.
Existing directed coverage, actual p-curve/3-D edge/vertex checks, work budgets
and source immutability remain. No mesh or residual-to-distance substitution.

The first default test passed its geometry bounds but incorrectly expected
corner-witness use: default caps were covered by strict interior witnesses.
Separate actual planar B-spline patches now exercise that path. A redundant patch
within tolerance of a retained corner passes geometry alone; a similar patch
near the excluded corner fails. Missing/opposite-material caps and resource
failures also reject. This demonstrates why geometry alone is NOT topology.

Shifted/scaled tests keep the physical **0.1 mm** target. The 20-mm case exhausted
default geometry depth 16 and passes with an explicit depth-18 request, without
changing production defaults or error thresholds. Solid integration needs an
independent geometry-depth setting: its current shared depth also feeds graph
routines capped at 16. See [contracts/next work](native-corner-cap-geometry.md).

FreeCAD exposes check_mixed_cap_geometry with directed bounds, policy/counters,
work/failure diagnostics and false topology/solid validity flags. Its CLI tests
exercise both operations, restored/owned source lifetime and budgets. Native
solid factory/codec, mesh/coverage and default GUI remain guarded/unfinished.
All other Stage 2–4 requirements stay active. No packages/system changes.

Focused kernel **4/4** (8.612 s) and native runtime/cap CLI **2/2** (3.88 s) pass.
Full kernel passes **103/103** (145.59 s), remaining compatibility **24/24**
(38.14 s). Full FreeCAD migration/GUI verification passes **56/56** (333.82 s).

Build/test logs: `/tmp/implicit-corner-cap-build.log`,
`/tmp/implicit-corner-cap-tests-depth-build.log`,
`/tmp/implicit-corner-cap-freecad-build.log`,
`/tmp/implicit-corner-cap-configure.log`,
`/tmp/implicit-corner-cap-focused-depth.log`,
`/tmp/implicit-corner-cap-cli.log`,
`/tmp/implicit-corner-cap-kernel.log`,
`/tmp/implicit-corner-cap-migration.log`,
`/tmp/implicit-corner-cap-compat.log`.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

## Previous increment: whole restricted native faces and corner-aware shells

BuildNativeShells now accepts regularized-corner native boundaries after the
global fixed-corner homotopy, graph/cap correspondence and full trim cycles.
Whole component faces preserve the checked domain exclusions, clipped to verified
component boxes, and reject seam segment bounds that might enter exclusions.
Existing opposite edge-use, cyclic vertex-link, connected face, Euler and
material grouping checks remain. Default Common/Cut construct one genuine native
shell: 13 faces, 30 edges, 19 vertices, Euler -2. Isolated contacts are not implicit
face members; the retained uniform corner remains cap-only. No reference mesh
triangles are stored as native faces.

Partition uses actual reference resolution/fixed-corner nodal values. A new
disconnected regularized test initially failed because cap attachment's graph
precheck silently used 32 despite a caller requesting 64. The caller-settings
overload fixes that propagation; the existing entry point preserves its default.
The disconnected case now resolves at requested 64/actual 63, with checked
zero-free component separation. First focused run after the fix passes **3/3**
(29.339 s); later scaled/shifted coverage is included in the full kernel suite.

FreeCAD reports corner_policy/reference_resolution on native shell snapshots.
Its new CLI test covers both operations, restricted/translated face queries,
material queries, source immutability, budget refusal, runtime ownership, restored
provenance and FCStd source close/reopen followed by shell construction. The
current runtime/shell test passes **2/2** (13.01 s). This is source-boundary
persistence plus shell reconstruction, NOT a direct shell codec/property.

BuildNativeSolids retains an explicit corner-policy refusal pending geometric
qualification. Geometry accuracy, solid validity, Part/generic UV compatibility
and the default GUI remain unchanged. All unfinished Stage 2–4 gates stay active;
see [contracts and next work](native-corner-shells.md). No packages/system changes
or numeric/accuracy budget relaxations. Expanded 99-case kernel harness timeout
is 240 s (previously 180 s), allowing the additional full-reference/shell tests.
Fresh full kernel verification passes **99/99** (136.62 s), full FreeCAD
command-line/GUI migration **55/55** (327.43 s), and remaining compatibility
**24/24** (37.69 s).

Revalidation after the session/runtime reset found no surviving build/test
processes or old /tmp logs; existing build targets were up to date. Fresh logs:
`/tmp/implicit-corner-shell-core-oct19.log`,
`/tmp/implicit-corner-shell-tests-oct19.log`,
`/tmp/implicit-corner-shell-freecad-oct19.log`,
`/tmp/implicit-corner-shell-cli-oct19.log`,
`/tmp/implicit-corner-shell-kernel-oct19.log`,
`/tmp/implicit-corner-shell-migration-oct19.log`,
`/tmp/implicit-corner-shell-compat-oct19.log`.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S . -B build
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

## Previous increment: actual native face-domain restrictions

Regularized mixed construction removes both isolated corner zero contacts from
the native implicit face domain, using fresh uniform-cone neighborhood bounds.
The retained material corner remains cap-only; six split corners are unchanged.
Native domain/ray queries, empty copies and primitive schema-2 persistence honor
closed exclusion boxes. Underlying field evaluation and the outer box are
unchanged. Existing unrestricted primitive output stays schema 1. Mixed schema-2
restore migrates old unrestricted math records with local corner checks only,
without retracing stored topology or recertifying restored seam bounds.
Mixed validation requires exact canonical restrictions. Unsupported sphere/
plane-section/split/untrimmed-input consumers reject exclusions explicitly.
FreeCAD exposes the exclusion count and owned/translated face queries.

See [contracts and remaining gates](native-face-domain.md). Whole-face attachment,
corner-aware geometry/shell/solid qualification, default GUI exposure, curved
operands and all unfinished Stage 2–4 requirements remain active. No packages,
system libraries, default workbench command or geometric budgets were changed.
Focused native kernel tests pass **13/13** (38.467 s); native runtime/trim CLI
passes **2/2** (29.20 s). Final kernel after the strict-solid serializer guard
passes **96/96** (156.27 s), compatibility **24/24** (39.55 s).
Full FreeCAD migration/GUI verification passes **54/54** (435.76 s).

Build/test logs: `/tmp/implicit-face-domain-build.log`,
`/tmp/implicit-face-domain-build-final.log`,
`/tmp/implicit-face-domain-tests-build.log`,
`/tmp/implicit-face-domain-focused.log`,
`/tmp/implicit-face-domain-freecad-build.log`,
`/tmp/implicit-face-domain-cli.log`,
`/tmp/implicit-face-domain-guard-build.log`,
`/tmp/implicit-face-domain-kernel-final.log`,
`/tmp/implicit-face-domain-compat-final.log`,
`/tmp/implicit-face-domain-migration-final.log`.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

## Previous increment: complete corner-aware directed trim cycles

CheckTrimAttachment now uses canonical exact split-corner anchors distinct from
the ordered interior box-edge roots (edge IDs 0..11, corner IDs 12..19). It never
counts one corner as three incident edge roots or welds near-corner points.
Complement construction preserves the source policy; dual references must use
the same actual grid/root provenance. All existing dual-cap pair, continuous
curve and complete oriented cyclic bijections remain required.

Default Common/Cut attach 18 seams to one reference loop/surface component,
Euler -3, reference closed component 0/material region 0. API exposes policy,
actual reference resolution and reference surface component Euler/shell/region
labels. These are reference assignments, NOT actual whole-face membership,
native shell/solid construction, topology/accuracy certification or a GUI switch.
Source/restored proof flags are unchanged. See [contracts](native-corner-trim.md).

Kernel tests cover both operations, all mappings, actual stored-wire starting-
edge rotation, mixed topology restore, work/parameter refusal and source
immutability. Native CLI independently checks native seam-vertex connectivity
and the six shared physical corners, then restored property lifetime and remaining
shell/solid guards. Whole-face membership/restriction must exclude isolated
relative-box contacts before native construction; mesh-only omission is insufficient.
All unfinished Stage 2–4 requirements, including curved operands/history/snapping,
remain active.

Existing core/test-worker/FreeCAD targets reused; no packages/system changes,
schema or transient result-layout changes. Both monolithic kernel CTest harnesses
now allow 180 s instead of 120 s: expanded 94-case audits take **126.76 s**.
Numerical work, matching, geometry and accuracy budgets were NOT changed.
Focused kernel tests pass **2/2** (11.056 s), native runtime/trim CLI **2/2**
(19.56 s). Full kernel passes **94/94** (126.76 s), FreeCAD command-line/GUI
migration **54/54** (409.86 s), and remaining compatibility **24/24** (37.89 s).

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S . -B build
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-trim-occt-build.log`,
`/tmp/implicit-corner-trim-tests-build-final.log`,
`/tmp/implicit-corner-trim-freecad-build.log`,
`/tmp/implicit-corner-trim-root-configure.log`,
`/tmp/implicit-corner-trim-target-final.log`,
`/tmp/implicit-corner-trim-kernel-final.log`,
`/tmp/implicit-corner-trim-compat-final.log`,
`/tmp/implicit-corner-trim-migration-final.log`.

## Previous increment: stored corner graph and cap-region attachment

Regularized CheckCorrespondence now uses the complete fixed-corner whole-box
homotopy/surface gate, then all existing curve/vertex/wire/cap/p-curve and seam
tangent checks against fresh native traces with the same material policy. Strict
paths remain unchanged. The API's regularity_policy distinguishes strict_strata
from fixed_corner_homotopy; the separate regularity API still excludes corner
zeros. Default Common/Cut match 30 native edges, 19 vertices, 12 caps and one wire.

CheckCapAttachment now uses the corner-aware closed reference's actual grid and
the same exact corner nodal values in its planar interpolation. Strict common
F/L retained points, native-cap IN classification and clearance beyond fresh
seam/curve/p-curve/matching budgets attach 12 cap regions bijectively to the one
reference component/material region. Independent CLI Part edge distances check
the witnesses and bounds. Cap permutations, restored provenance/ownership,
comparison/work/clearance limits and source immutability are tested.
See [contracts and next work](native-corner-attachment.md).

This is NOT native-to-affine-reference trim-cycle or whole implicit-component
attachment. CheckTrimAttachment has an explicit early corner-policy guard until
canonical corner anchors replace its edge-only model. Mesh/coverage/cap geometry/
shell/solid guards remain. Source geometry and saved-bound flags are not changed;
no native solid/default GUI or .1 mm whole-model claim. Next attach complete
directed cycles and whole components, then geometry/solid/default GUI gates.
All unfinished Stage 2–4 scope, including curved operands/history/snapping,
remains active.

Existing core/test-worker/FreeCAD module targets rebuilt with two compile jobs,
one FreeCAD link job. No packages/system changes, persistence schema or transient
layout changes. Two focused kernel cases pass (10.651 s), native runtime/region/
attachment CLI **3/3** (24.45 s), complete kernel **92 cases** (104.19 s).
Full migration passes **53/53** (311.09 s), remaining compatibility **24/24**
(29.89 s; kernel entry separately passed). No native build/test failures or
weakened downstream guards. The initial combined CLI/CMake patch had a context
mismatch and was reapplied atomically before testing.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-attachment-occt-build.log`,
`/tmp/implicit-corner-attachment-tests-build-final.log`,
`/tmp/implicit-corner-attachment-freecad-build.log`,
`/tmp/implicit-corner-attachment-target-final.log`,
`/tmp/implicit-corner-attachment-kernel-final.log`,
`/tmp/implicit-corner-attachment-compat-final.log`,
`/tmp/implicit-corner-attachment-migration-final.log`.

## Previous increment: closed corner material reference and strict-volume grouping

Explicit regularized CheckRegionTopology now caps the checked surface on its
actual odd grid, sharing canonical surface corner/root IDs without welding.
Zero contacts alone create no cap area. The retained uniform corner is cap-only;
the excluded uniform corner is absent. Volume connectivity joins STRICT retained
grid vertices only. Each zero corner inherits one unique incident strict-volume
region; it never joins regions through a contact. Existing closed incidence,
cyclic vertex links, orientations, Euler and shell/material checks remain required.
See [proof scope/API](native-corner-region.md).

Default Common/Cut each have one closed reference component, Euler -2, one
material region and affine-reference volume 500; their sum is the cube's 1000.
These are numerical L-reference volume diagnostics, NOT F volume/error bounds.
A separate two-island case stays in two material regions. The field/domain/
isovalue/phase are unchanged. Native CLI independently reconstructs closed edges,
Euler, signed volume, prefix sharing and contact roles from the public arrays;
raw/mixed/restored property ownership and downstream guards are tested.

No native CAD shell/solid, stored cap/trim/whole-face attachment, .1 mm surface
certificate or default GUI switch follows. Those guards remain, and source/
restored seam bounds are not recertified. Next connect actual stored topology
to this reference, then native geometry/solid validation and default GUI exposure.
All other unfinished Stage 2–4 scope remains, including general/curved operands,
history/snapping and broader consumers.

Existing TKMesh/native test-worker/FreeCAD module targets reused with two compile
jobs and one FreeCAD link job; no packages/system changes, persistence schema or
transient result-layout change. A dense nodal cache bounds memory without a tree
entry for every sample. Focused kernel cases pass **2/2** (2.151 s), native runtime/
surface/region CLI **3/3** (12.16 s), complete kernel **90 cases** (92.72 s).
Full migration passes **52/52** (296.01 s), remaining compatibility **24/24**
(29.86 s; kernel entry separately passed). No native build/test failures or
weakened downstream guards. Source/rebuilt snapshot checks and the explicit
attachment/solid refusals passed in the full suite.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-region-occt-build-final.log`,
`/tmp/implicit-corner-region-tests-build.log`,
`/tmp/implicit-corner-region-freecad-build.log`,
`/tmp/implicit-corner-region-target-final.log`,
`/tmp/implicit-corner-region-kernel-final.log`,
`/tmp/implicit-corner-region-compat-final.log`,
`/tmp/implicit-corner-region-migration-final.log`.

## Previous increment: whole-box corner homotopy and surface reference

Explicit regularized CheckTopology now covers exact box-corner nodes using an
odd grid <= the requested resolution, unchanged field/domain/phase, and all the
existing whole-volume/face/edge homotopy checks. Each corner-incident linear
tetrahedron must also preserve every inward coordinate derivative sign of the
fresh F germ. Canonical `(i,i)` grid IDs share surface corners; uniform isolated
contacts stay separate from nodes/triangles. Unknown/other zero nodes, precision
and resource exhaustion remain unresolved. Legacy strict behavior is unchanged.
See [proof scope and API](native-corner-topology.md).

The default cube resolves at actual resolution 31: eight fixed corners, six
shared surface-corner nodes, two point contacts, 13,779 surface nodes, one
component/Euler -3/one boundary loop in both orientations. Independent native
CLI accounting checks public edge orientation/incidence, loops, Euler, contact
separation, restored ownership, policy mismatch and source provenance. This is
NOT closed material connectivity, stored native trim/cap attachment, a .1 mm
surface mesh or solid validity. Corner-aware closed-region APIs explicitly reject;
all native mesh/correspondence/solid guards remain. Next add same-ID closed caps
and strict-volume grouping, then actual attachment and default solids/GUI. All
other Stage 2–4 requirements remain active, including curved operands/history.

TopologyWitness/nested transient layout changed. Rebuilt existing TKMesh
dependencies, native tests/worker and FreeCAD Implicit/ImplicitGui/_ImplicitOCCT
with two compile jobs and one FreeCAD link job. No packages/system changes or
OCCT-8/7 mixing. Restart the fork, not just Python. Two focused kernel cases pass
(597 ms), native CLI/runtime **2/2** (5.40 s), complete kernel **88 cases**
(85.27 s). Full migration passes **51/51** (276.67 s), remaining compatibility
**24/24** (38.13 s; kernel entry separately passed). Builds/tests passed without
native failures or weakened downstream guards. A combined documentation patch
failed context verification and was reapplied atomically.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-topology-occt-build-final.log`,
`/tmp/implicit-corner-topology-tests-build-final.log`,
`/tmp/implicit-corner-topology-freecad-build.log`,
`/tmp/implicit-corner-topology-target-final.log`,
`/tmp/implicit-corner-topology-kernel-final.log`,
`/tmp/implicit-corner-topology-compat-final.log`,
`/tmp/implicit-corner-topology-migration-final.log`.

## Previous increment: regularized corner-aware physical classification

The mathematical-region classifier now accepts the regularized corner policy.
Existing strict-sign cap points and in-box opposite-sign brackets already give
genuine regularized boundary witnesses. Fresh local corner identities add exact
retained/split-corner witnesses. Uniform excluded corners never witness a
boundary: an independent gradient/Hessian Taylor bound excludes a clipped ball
about the exact anchor, yielding physical separation even outside the box.
Insufficient bounds stay unresolved, not guessed near-boundary. No raw residual
is treated as distance, and no stored seam bound is re-certified.
See [derivation/contracts](native-corner-classification.md).

Native classify_mixed now reports corner_policy. The existing whole ComplexGeoData
query path applies rigid/container/App::Link precision reserves without changes
to its result layout. New CLI tests cover real property/FCStd restore, placed
links, lifetime and source provenance. Source field/domain are unchanged; no new
persistence schema, default workbench switch or transient ABI layout change.
Mesh/coverage/correspondence/cap-geometry/shell/solid guards remain. Next extend
corner-aware whole-domain reference and actual attachment, then qualified default
solids/GUI; all other unfinished Stage 2–4 requirements remain active.

Existing TKMesh/native tests-worker/FreeCAD module and script targets rebuilt
with two compile jobs, one FreeCAD link job; compatibility scripts refreshed.
No packages/system libraries changed. Targeted kernel cases pass **3/3** (3.626 s),
targeted native runtime/corner APIs **4/4** (7.25 s), complete kernel **86 cases**
(87.94 s). Full migration passes **50/50** (275.98 s), remaining compatibility
**24/24** (27.74 s; kernel entry separately passed). The interrupted goal turn's
specific running test session was revalidated and resumed, not restarted; it
finished with exit zero. No native build/test failures or weakened validation
gates. An initial combined documentation patch failed context verification and
was reapplied atomically. The source checkpoint is published above.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake --build build-freecad-implicit --target ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-classification-occt-build-final.log`,
`/tmp/implicit-corner-classification-tests-build.log`,
`/tmp/implicit-corner-classification-freecad-build.log`,
`/tmp/implicit-corner-classification-compat-scripts.log`,
`/tmp/implicit-corner-classification-target.log`,
`/tmp/implicit-corner-classification-kernel-final.log`,
`/tmp/implicit-corner-classification-compat-final.log`,
`/tmp/implicit-corner-classification-migration-final.log`.

## Previous increment: schema-2 corner topology and FCStd ownership

The native codec now persists explicit regularized corner policy/contact metadata
with the shared graph. Ordinary strict boundaries retain schema-1 output. Local
corner math, actual retained/excluded corner vertices, split-corner seam ends
and exact cap planes are checked without tracing or upgrading saved seam bounds.
Malformed metadata/records reject atomically. Native property assignment, real
FCStd restore, content transfer, subobjects and undo/redo now retain that graph.
Mapped names are record-digest identities; material changes invalidate stale
references, not silently retarget them. No Boolean-history completion claim.

Native mesh Prepare/CheckCoverage, CheckCorrespondence, CheckCapGeometry,
BuildNativeShells and solid codec source validation have explicit corner-policy
guards now that I/O no longer blocks that policy. Distance classification still rejects. No default workbench
switch or valid-solid claim. Next implement corner-aware classification and
whole-domain reference/actual attachment, then qualified default solids/GUI;
all other unfinished Stage 2–4 requirements remain active.
See [contracts](native-corner-persistence.md).

Builds reused existing TKMesh, native tests/worker, native FreeCAD module and
script targets, plus compatibility scripts; no new packages, system-library
changes or transient layout changes. Three focused kernel cases pass (3.854 s),
the initial native corner/property CLI/runtime passes **3/3** (5.15 s), complete
kernel passes **84 cases** (87.63 s), and remaining compatibility **24/24**
(27.85 s). The first full migration passes **49/49** (286.45 s), and the final
reference-target rerun passes **2/2** (2.74 s). Review then added explicit guards
for direct CheckCapGeometry/CheckCoverage calls (previously blocked by the codec)
and direct negative kernel tests. Final rebuilt guard regression passes:
**84 kernel cases** (87.42 s), **49/49 migration** (289.13 s), remaining
**24/24 compatibility** (29.22 s; kernel entry separately passed) and targeted
corner CLI/runtime **3/3** (5.18 s). No native test/build failures occurred;
the initial combined documentation patch failed context verification and was
reapplied without partial file changes. No numerical gates were relaxed.

Commands use the existing directories:

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake --build build-freecad-implicit --target ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
ctest --test-dir build-freecad-occt8/migration-tests -R '^implicit_occt8_corner_persistence$' --output-on-failure
```

Logs: `/tmp/implicit-corner-io-occt-build-final.log`,
`/tmp/implicit-corner-io-tests-build.log`,
`/tmp/implicit-corner-io-freecad-build.log`,
`/tmp/implicit-corner-io-compat-scripts.log`,
`/tmp/implicit-corner-io-kernel-final.log`,
`/tmp/implicit-corner-io-compat-final.log`,
`/tmp/implicit-corner-io-migration-final.log`,
`/tmp/implicit-corner-io-target-final.log`.
The final direct-validation guards additionally use
`/tmp/implicit-corner-io-occt-guards-build.log`,
`/tmp/implicit-corner-io-tests-guards-build.log`,
`/tmp/implicit-corner-io-kernel-guards-final.log`,
`/tmp/implicit-corner-io-migration-guards-final.log` and
`/tmp/implicit-corner-io-compat-guards-final.log`.

## Previous increment: corner-aware native default boundary graph

The explicit regularized corner policy now builds the unchanged default gyroid/
cube boundary for both operations, with shared real topology (30 edges/19
vertices), 18 seams, 12 interior box-edge roots and separate excluded isolated
corner contacts. Canonical corner IDs join adjacent contours; no field/domain
perturbation, root welding, mesh-backed fake surfaces or solid promotion.
See [full contracts](native-corner-boundary.md).

FreeCAD `mixed_box(..., corner_policy="regularized")` and inspect results retain
the policy/contact metadata and owned graph. Legacy default stays strict reject.
Source codec writing, property assignment and dependent reference/mesh/shell/
solid consumers reject the unsupported policy; old distance classification also
rejects. Default Add Gyroid stays a field display. Corner-aware persistence,
classification, whole-domain reference/actual attachment and qualified solids
remain next; all remaining Stage 2–4 requirements stay active.

Builds reused existing targets/dependencies: OCCT TKMesh (transitive core), native
tests/worker, FreeCAD Implicit/ImplicitGui/_ImplicitOCCT/ImplicitScripts and
compatibility ImplicitScripts. Boundary ABI changed; restart the fork. No package,
system-library or security-policy modifications. New tests pass **2/2** (2.823 s),
native CLI/runtime **2/2** (2.26 s), complete kernel **83 cases** (81.99 s).
Full verification: **48/48 OCCT-8 migration** (268.45 s); compatibility
**24/24 excluding the separately passed kernel entry** (27.15 s), covering all
25 registered compatibility entries across the two runs. GUI surface/live-solid
and demo workflows all pass. New kernel and native CLI tests passed first try;
no existing accuracy/validity gates were weakened to obtain these results.

Commands/logs:

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake --build build-freecad-implicit --target ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build -R '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build -E '^implicit_occt_native_faces$' --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure
```

Logs: `/tmp/implicit-corner-boundary-occt-build.log`,
`/tmp/implicit-corner-boundary-tests-build-final.log`,
`/tmp/implicit-corner-boundary-freecad-build.log`,
`/tmp/implicit-corner-boundary-compat-scripts.log`,
`/tmp/implicit-corner-boundary-kernel-final.log`,
`/tmp/implicit-corner-boundary-compat-final.log`,
`/tmp/implicit-corner-boundary-migration-final.log`.

## Previous increment: exact lattice-node plane sections with isolated contacts

`BRep_ImplicitPlaneSection::Settings.AllowExactLatticeNodes` explicitly enables
analytic quarter-period nodes without changing the field. Shared grid-node
identity prevents duplicate corner roots; whole-edge monotonicity qualifies
endpoint roots, and signed derivative cones distinguish a curve entering a cell
from an isolated quadrant contact. Isolated rectangle-corner zeros survive in
Result.PointContacts instead of zero-length fake curves. Interior exact nodes
must still join with valid graph degree. Unknown near-zeros and tangent/zero
edges remain unresolved. Odd bounded fallback grids avoid repeated nonrecognized
eighth-period samples with identical bounds and no geometry/domain perturbation.

New mode requires effective identity field placement and coordinate-plane frames.
TwoSum verifies the affine map itself rather than treating a rounded sum that
lands on a lattice coordinate as exact. Legacy mode, constructors and worker
protocol remain unchanged/default false; no consumer silently drops contacts.
Point-only sections can be resolved even when Curves is empty, but are not empty
geometry. Three curves/two contacts per default cube face and shared interior
nodes have independent analytic coverage and graph tests. Reversed coordinate
frames, pure contacts and unknown/rounded-affine/placement/budget refusals are
covered. This is section computation, not shared cross-face B-Rep topology.

OCCT-8 FreeCAD exposes `trace_gyroid_plane`, with keyword settings and an actual
bool opt-in. Numeric point/curve data outlives documents and primitive round-trip;
topology/solid validation flags remain false. Six-face endpoint/contact counts
agree with the local corner model. The first CLI test incorrectly placed its
interior-crossing rectangle partly outside the `[0,10]^3` source domain and was
correctly refused. That fixture now tests the real interior lattice point
`(5,5,0)` instead; the domain guard was not relaxed. Final CLI/runtime passed
2/2 (1.11 seconds). Initial new kernel tests passed 2/2 in 408 ms, the complete
interior/contact tests passed in 410 ms, and final reversed-frame coverage
passed 2/2 in 465 ms. Logs:
`/tmp/implicit-lattice-trace-targeted.log`, `/tmp/implicit-lattice-trace-targeted-final.log`,
`/tmp/implicit-lattice-trace-orientation.log`, `/tmp/implicit-lattice-trace-cli.log`,
`/tmp/implicit-lattice-trace-cli-final.log`.

Settings/Result transient layout changed: TKMesh dependencies, native-face tests
AND worker, and FreeCAD Implicit/ImplicitGui/_ImplicitOCCT consumers were rebuilt.
Use a fresh fork process after rebuilding, not Python-only hot reload. No new
persistence schema/saved proof flags/GUI command or default primitive change.
Existing build targets and dependencies were reused, two compile/one link jobs;
no packages/system libraries were changed. Commands are in
[section contracts](lattice-plane-sections.md). Build/configure logs:
`/tmp/implicit-lattice-trace-occt-build.log`, `/tmp/implicit-lattice-trace-occt-final-build.log`,
`/tmp/implicit-lattice-trace-tests-build.log`, `/tmp/implicit-lattice-trace-tests-final-build.log`,
`/tmp/implicit-lattice-trace-test-orientation-build.log`,
`/tmp/implicit-lattice-trace-freecad-build.log`, `/tmp/implicit-lattice-trace-compat-scripts-build.log`,
`/tmp/implicit-lattice-trace-harness-configure.log`.

Final full regression passed **25/25 compatibility** (105.34 seconds), including
**81 kernel cases** (78.51 seconds), then **47/47 OCCT-8 migration** (262.15
seconds). Full logs: `/tmp/implicit-lattice-trace-compat-final.log`,
`/tmp/implicit-lattice-trace-migration-final.log`. The compatibility facade was
refreshed before testing; the native worker was rebuilt for the changed result
layout. No library builds overlapped runtime/library audits.
Source checkpoints: OCCT `6e3876f47d`, FreeCAD `000e83bba2`, published before
the umbrella gitlinks. Existing persistence schemas and default GUI commands
remain unchanged.

Next construct canonical cross-face box-edge/corner roots and actual cap/implicit
cycles, then extend corner-aware homotopy/reference/attachment and material/
persistence policies. Resolving sections does not enable the default native
solid. Curved/general operands, snapping/history, richer editing and broader
consumers remain required; the full through-Stage-4 goal stays active.

## Previous increment: exact corner identities and local material models

OCCT adds `Geom_ImplicitGyroid::LatticeJet`: normalized binary-rational
quarter-period recognition uses an exact FMA residual and leaves outputs unchanged
on failure. No near-zero snapping and no Value/Gradient behavior changes.
TKTopAlgo CheckCornerGerms isolates zero-free or coordinate-monotone inward
neighborhoods using Lipschitz/Hessian bounds and existing engineering reserves.
ClassifyCornerGerm separates closed-inequality membership from closure-of-interior
regularization; outside-neighborhood and uncertain signs remain unresolved.
The unchanged default gyroid has eight exact corner identities, six split cones
and an isolated contact at a different corner for each operation.

This is a necessary local kernel model, NOT whole regularity, tracing, topology
or solid construction/validation. Existing default trim/strict regularity guards
remain unchanged and tests explicitly require their refusal. The model ignores
stored trim wires and checks the computational box only. Radius is neighborhood
size, not a geometric approximation or distance-classification tolerance.
See [corner contracts](native-corner-germs.md) for the remaining integration.

Three new kernel cases cover exact/nextafter identities, output atomicity,
scaling and exactly representable period translations, independent derivative
samples over the neighborhood, both operations/material conventions, source
immutability, primitive round-trip, tangent/unknown/precision/type/placement
refusals. Initial targeted tests passed 3/3 in 7 ms; final targeted tests passed
3/3 in 5 ms. Initial FreeCAD CLI/runtime failed the new primitive round-trip:
an exact identity transform reloads as a nonempty datum chain. The corner query
now compares the effective matrix for EXACT identity without mutating topology;
even a 1e-18 translation still rejects. Final CLI/runtime passed 2/2 (1.12 seconds).
Logs: `/tmp/implicit-corner-targeted.log`, `/tmp/implicit-corner-targeted-final.log`,
`/tmp/implicit-corner-cli.log`, `/tmp/implicit-corner-cli-final.log`.

Build commands reused the existing trees:

```sh
cmake -S OCCT -B build-occt-implicit
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
cmake --build build-freecad-occt8 --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
```

The first OCCT build ran 39 steps; the final identity/radius correction rebuilt
only TKTopAlgo/dependent links. FreeCAD's requested minimal targets triggered
their cached header/version dependencies (141 steps, including Part/Gui); no
all-target build or packages/system-library changes. The final binding target
check reported up to date, and runtime tests exercised the new exports. Logs:
`/tmp/implicit-corner-occt-configure.log`, `/tmp/implicit-corner-occt-build.log`,
`/tmp/implicit-corner-occt-final-build.log`, `/tmp/implicit-corner-tests-build.log`,
`/tmp/implicit-corner-tests-final-build.log`, `/tmp/implicit-corner-freecad-build.log`,
`/tmp/implicit-corner-binding-final-build.log`, `/tmp/implicit-corner-harness-configure.log`.
No library builds overlapped loaded-library audits. Final full regression and
the fresh OCCT-7 facade guard outcomes follow below.

Final full regression passed **46/46 OCCT-8 migration** (334.76 seconds) and
**25/25 compatibility** (158.47 seconds), including **79 kernel cases** (119.70
seconds in that concurrent run). After the compatibility suite completed, its
script target refreshed the latest facade and the fresh OCCT-7 import guard
passed **1/1** (0.28 seconds). The unchanged guard must reject before importing
new private exports; no OCCT 8 headers/libraries enter the OCCT 7 process.

Additional exact sine/cosine-gradient assertions and the all-sine quarter-period
corner model strengthened the same three new kernel cases. Their final full
kernel rerun passed **79 cases**, CTest **1/1** (85.53 seconds). This is a
regression runtime observation, not a performance claim. Final logs:
`/tmp/implicit-corner-migration-final.log`, `/tmp/implicit-corner-compat-final.log`,
`/tmp/implicit-corner-compat-scripts-build.log`, `/tmp/implicit-corner-compat-guard-final.log`,
`/tmp/implicit-corner-tests-strengthened-build.log`, `/tmp/implicit-corner-kernel-final.log`.
Source checkpoints: OCCT `27ba200574`, FreeCAD `b79fbbe49d`; both branches are
published before the umbrella gitlinks. No new persistence schema or saved proof
flags were introduced.

Next extend plane tracing/canonical corner roots/cap cycles, then the nodal-zero
homotopy reference and actual attachment pipeline, consuming an explicit local
material policy. Do not perturb the default field, weld corner roots after the
fact or bypass independent validation gates. Curved/general operands and every
unfinished Stage 2–4 requirement stay active.

## Previous increment: live native-solid feature and component-aware GUI

`ImplicitSolid.py` adds a source-linked NativeSolidFeaturePython controller and
actual-result renderer. The typed solid property stays authoritative; saved
source/settings fingerprints only select unchanged reuse versus a new checked
build. Embedded source/tolerance consistency is checked; restore refreshes
without numerical building and leaves qualification false. Explicit rebuild
creates a new qualified snapshot, never upgrades a retained loaded result.
Source errors, work exhaustion and corrupt/inconsistent records clear geometry,
diagnostics and view buffers; corruption requires explicit repair.

Source has Global link scope, avoiding App::Part's automatic dependency adoption.
The initial Local source link moved upstream field/box operands into the output
container and correctly failed the older identity-input contract. Global scope
preserves the independent source coordinate frame. FollowSourcePlacement maps
source-global to output-container local; independent output positioning is
explicit. Live operation edits, undo/redo, source consistency, restore, corruption,
explicit repair, budgets, independent/followed/container placement and skipped
C++ upstream-error invalidation pass the new CLI test.

The view uses actual Solid/Shell/Face hierarchy and per-component NativeFaceN
nodes, separately shaded caps, actual EdgeN/VertexN ComplexGeoData and native
Seam-to-Edge identity aliases. It never uses the source's aggregate face as the
actual result. Every candidate field triangle must fit one saved component
domain; ambiguity/missing components/crossings reject the preview. Face buffers
compact existing node indices without coordinate welding. Camera/visibility
changes reuse caches; numerical preview failure retains authoritative geometry.
Workbench commands create a live solid from a selected boundary or a supported
demo. The default Add Gyroid icon is unchanged.

Only existing script targets and `_ImplicitOCCT` were rebuilt, with two compile/
one link jobs. No OCCT/dependency/system-library changes. Logs:
`/tmp/implicit-live-solid-scripts-build.log`, `/tmp/implicit-live-solid-binding-build.log`.
Initial CLI/runtime passed 2/2 (17.80 seconds); initial GUI/runtime tests passed
3/3 (25.68 seconds). After actual seam alias/compacted-buffer additions,
targeted CLI/GUI/runtime passed 4/4 (44.14 seconds). Logs:
`/tmp/implicit-live-solid-cli.log`, `/tmp/implicit-live-solid-gui.log`,
`/tmp/implicit-live-solid-targeted.log`. Expanded viewport-link coverage and final
regression outcomes follow below. See [live native-solid contracts](live-native-solids.md).

Expanded rotated-link checks exposed stale instance selection after source
replacement: clearing only the target object left its App::Link selection alive.
The renderer now clears this target and its linked instances, preserving
unrelated objects. The failing assertion then opened a modal dirty-document
save prompt during test cleanup; the original expanded run timed out at 180
seconds. New GUI assertion failures immediately exit with failure instead of
opening that prompt. Corrected expanded GUI/runtime tests passed 3/3 (27.37
seconds), log `/tmp/implicit-live-solid-link-gui.log`; the initial failure is
`/tmp/implicit-live-solid-expanded.log`. A read-only GDB attach attempt was
denied by host ptrace policy (`/tmp/implicit-live-solid-gui-hang-gdb.log`);
no system security settings were changed. Subsequent tests also check unrelated
selection preservation, active transaction ownership, native seam aliases,
full/empty result display and missing/ambiguous component-display refusal.

Viewport captures `/tmp/implicit-live-solid-demo.png` and
`/tmp/implicit-live-solid-components.png` were visually inspected. The second
fixture's field components are internal from that camera, so real per-component
ray hits and domain/alias checks, not its exterior screenshot alone, establish
their selection coverage. This is software-OpenGL verification, not a GPU/FPS
benchmark. Final script refresh logs:
`/tmp/implicit-live-solid-scripts-final-build.log`,
`/tmp/implicit-live-solid-compat-scripts-build.log`.

Final full regression passed **45/45 OCCT-8 migration** (263.22 seconds) and
**25/25 compatibility** (105.80 seconds), including unchanged **76 kernel cases**
(79.10 seconds). The new live CLI test passed in 18.11 seconds; expanded GUI
coverage, including unrelated selection preservation and full/empty result
display, passed in 27.94 seconds. Logs:
`/tmp/implicit-live-solid-migration-final.log`,
`/tmp/implicit-live-solid-compat-final.log`. Libraries were never rebuilt while
runtime/library audits were active. The final primary screenshot is captured
before the deliberate camera/placement/link stress tests, with a GUI update
before fitting the initial scene; a focused GUI rerun records that capture.
That final focused GUI/runtime rerun passed **3/3** (29.97 seconds), with the
live GUI case at 27.88 seconds; log
`/tmp/implicit-live-solid-gui-final-capture.log`. Source checkpoint: FreeCAD
`8bdaff16bd`, OCCT unchanged `0e6377579d`.

This is restricted source/renderer integration, not Stage 4 completion. Default
gyroid trim degeneracies, curved/general operands/restrictions, snapping/history,
richer editors and broader representation-aware consumers remain required.

## Previous increment: actual native solid property and FCStd owner

The OCCT-8 C++ module adds `PropertyNativeSolidGeometry` and
`NativeSolidFeaturePython`. The geometry is the actual native result, with
Solid/Shell/Face/NativeFace/Cap/Seam/Edge/Vertex subelements and a separate
`ImplicitSolidV1_` snapshot digest. Shared NativeFeatureBase handles rigid
subobject paths and upstream-error invalidation for both owner families without
inheriting a dummy boundary property into solids. Result component domains give
field bounds, including enclosed components with no edges; empty results have
no fabricated source face. The retained source alias owns the complete solid
snapshot and is used only for explicit mathematical-region queries.

Distinct schema-1 NativeSolidGeometry XML and `.nativesolid` FCStd records reuse
the actual solid codec. Restore does not numerically rebuild and remains
unverified; runtime transaction copies retain immutable qualified snapshots.
Cross-representation assignment, Paste and content transfer reject atomically.
Inline restoration commits only after its character stream closes. Tests cover
both mixed operations, separate islands/cavities, full/empty, typed assignment,
undo/redo, content/FCStd/copy, corrupt payloads, mapped references, lifetime,
component bounds, container/link poses and skipped-execution source failures.
Whole classification explicitly covers the authoritative source union, not an
individual SolidN; selected-region classification refuses.

Existing minimal targets `Implicit ImplicitGui _ImplicitOCCT ImplicitScripts`
built with two compile/one link jobs; a final `Implicit ImplicitGui` rebuild
compiled the representation-specific base-setter guard. Logs:
`/tmp/implicit-solid-property-build.log`, `/tmp/implicit-solid-property-final-build.log`.
Initial targeted property/reference/element/link/query/runtime tests passed 7/7
(17.61 seconds). Expanded link/container/invalidation tests passed 2/2 including
runtime (11.06 seconds). The first expanded test expected an incompatible
content-transfer exception but omitted OSError from its accepted exception
types; the test now accepts the actual API error and still checks unchanged
destination geometry. Logs: `/tmp/implicit-solid-property-targeted.log`,
`/tmp/implicit-solid-property-expanded.log`,
`/tmp/implicit-solid-property-expanded-final.log`.

Further reference coverage checks mapped Solid/Shell/Face/component/edge/vertex
links through clear/undo/FCStd, rejects a changed result's stale mapped name and
recovers it only for the original snapshot. Its first full-suite run caught a
test-local `names` variable shadowing the saved object-name tuple during the
new link iteration; renaming it preserves the corruption assertion. Corrected
property/runtime tests passed 2/2 (11.36 seconds), log
`/tmp/implicit-solid-property-references-final.log`. Compatibility passed 25/25
(116.84 seconds), including unchanged 76 kernel cases (79.31 seconds), log
`/tmp/implicit-solid-property-compat-final.log`.

Final clean OCCT-8 migration passed **43/43** (216.50 seconds), including the
expanded solid reference test (10.75 seconds) and all existing GUI regressions.
Log: `/tmp/implicit-solid-property-migration-clean.log`. The earlier full run
(`/tmp/implicit-solid-property-migration-final.log`, 42/43, 212.60 seconds)
failed only at the subsequently corrected test-local variable shadowing noted
above. Final source checkpoint: FreeCAD `46bb773deb`; OCCT unchanged
`0e6377579d`. No runtime/library audits overlapped library rebuilds.

This is a low-level native document snapshot owner, not the finished live solid
GUI workflow. Next add source/settings recompute and actual component-aware
rendering/picking; do not display the source as the result. Default primitive
trimming/degeneracies, curved operands, general restrictions, snapping/history
and broader consumers remain required. No OCCT/dependency/system-library edits
were needed. See [solid property contracts](native-solid-property.md).

## Previous increment: actual native solid topology persistence

`BRepClass3d_ImplicitSolidIO` schema 1 preserves the actual restricted result's
component domains, local source-wire assignments, face/shell/material grouping
and requested tolerance, reusing its shared source cap/edge/wire graph. Loading
constructs saved topology, not a numerical Boolean/meshing/separator recipe.
Structural parent/child, incidence/connectedness, provenance/domain and work/input
checks reject unsupported/corrupt structures. Restored solids are constructed
but unverified; numerical/attachment/accuracy flags and shell/solid Closed flags
are false. Saved Euler numbers are provenance, not accepted proof.

Exports preserve original caches/flags and strip meshes from the copied payload;
noncanonical freed children reject before touching source state. The cut-face
orientation regression caught a builder-composition error: local stored wires
must be added before assigning the reversed parent orientation. Topology hash
lookup avoids quadratic wire matching. Reads are complete/atomic and bounded;
source conventional-BRep parsing is not claimed to be allocator/DoS hardened.

FreeCAD adds immutable `dumps_mixed_solids`, `loads_mixed_solids` and explicit
`source_mixed_solids` APIs. Tests cover owned lifetime, original versus loaded
validity, source classification, explicit new checking/building, corruption and
safe rejection by existing boundary properties and Part. This codec does NOT
yet provide the dedicated solid ComplexGeoData/property/owner or FCStd workflow;
these remain the immediate next integration requirement. The full through-Stage-4
goal is active, including all unfinished Stage 2/3/4 gates.

Three new kernel cases (six mixed operation/component fixtures plus empty/full,
locale, corruption, stream/destination and cache/flag checks) bring the suite to
76. First full run passed 76 kernel cases (79.44 seconds), migration 42/42
(207.24 seconds), compatibility 25/25 (106.60 seconds). Targeted new FreeCAD
I/O/runtime tests passed 2/2 (10.24 seconds). Final ownership-guard verification:
**76 kernel cases** (79.10 seconds), **42/42 migration** (207.57 seconds),
**25/25 compatibility** (105.78 seconds). Final logs:
`/tmp/implicit-solid-codec-kernel-final.log`, `/tmp/implicit-solid-codec-migration-final.log`,
`/tmp/implicit-solid-codec-compat-final.log`. First-run logs: `/tmp/implicit-solid-codec-kernel-full.log`,
`/tmp/implicit-solid-codec-migration-full.log`, `/tmp/implicit-solid-codec-compat-full.log`,
`/tmp/implicit-solid-codec-cli.log`. See [codec policy](native-solid-topology-io.md).
No packages/system libraries changed; separate OCCT 8 and preserved OCCT 7 builds
remain isolated. Existing minimal targets and two compile/one link policy reused.
Source checkpoints: OCCT `0e6377579d`, FreeCAD `61f412c1fe`. Build logs:
`/tmp/implicit-solid-codec-config.log`, `/tmp/implicit-solid-codec-ownership-build.log`,
`/tmp/implicit-solid-codec-freecad-build.log`. GDB exported-function breakpoints
and exception catches diagnosed the reversed-parent wire-composition failure;
the OCCT Release build does not provide source-line debug symbols.

## Previous increment: native edge/vertex discovery and GUI selection

The mixed ComplexGeoData property now exposes actual Edge/Vertex subelements,
canonical edge-endpoint indexing, mapped reference invalidation and independently
owned point/curve results. No point welding or promotion of mesh/polyline points
to topological vertices occurs. Selected vertices extract one stored point with
empty normals/lines and point bounds, with supported rigid poses applied once.
The C++ inspection API exposes native edge/vertex coordinates, endpoint IDs and
seam aliases. Public recipe metadata containers are copied defensively.

The mixed view draws conventional box edges and actual vertex markers, shares
seam/edge nodes, retains legacy seam picking and adds persistent edge/vertex
visibility switches without remeshing. The focused workbench adds a supported
native-boundary selection demo in an experimental menu; the original default
10 mm field gyroid is not silently replaced by an unrelated fixture or solid.
See [native edge/vertex contracts](native-edge-vertex-selection.md).
The mixed surface now shades with oriented analytical field-gradient normals
instead of per-triangle flat normals, leaving cap normals/creases independent.
Mesh coordinates and all geometric/solid validity flags remain unchanged.

Tests caught round-trip differences in BRep vertex text (15 significant digits).
They now check exact endpoint/edge identities plus machine-scale coordinate
reserves instead of snapping actual vertices to curve poles or claiming bitwise
precision. This is separate from the 0.1 mm whole-solid validation policy.
Targeted CLI/property/reference/element/link/topology **6/6** (21.96 seconds),
camera/pixel/instance/visibility/editor/surface GUI **5/5** (96.23 seconds).
Logs: `/tmp/implicit-native-topology-cli-final.log`,
`/tmp/implicit-native-topology-gui-retry.log`. Final OCCT-8 suite **41/41**
(295.33 seconds), including independent analytical normals for both operations
and the new camera/pixel/workbench/cache tests. Compatibility **25/25**
(132.52 seconds), including unchanged **73 kernel cases**. Logs:
`/tmp/implicit-native-topology-normal-migration-full.log`,
`/tmp/implicit-native-topology-compat-full.log`.
Viewport captures `/tmp/implicit-native-topology-preview.png` (before) and
`/tmp/implicit-native-topology-preview-smooth.png` (after) were visually inspected;
the final capture process completed after explicitly closing its temporary doc.
Build log: `/tmp/implicit-native-topology-index-build.log` plus the normal-only
increment `/tmp/implicit-native-topology-normal-build.log`. Two initial compiler
errors (incomplete TopoDS_Edge type and mixed derived-shape ternary) were fixed
before final verification; do not mistake the earlier failed build logs for the
final result. Runtime tests were not run while libraries were being rebuilt.
The remaining solid property/codec, default primitive trimming, snapping/history,
curved operands and broader representation-aware consumer gates keep the full
through-Stage-4 goal active. No OCCT, package or system-library changes this increment.
FreeCAD checkpoint: `9a154a3f91`; OCCT remains `b032b19ae2`.

## Previous increment: restricted native solids and focused primitive workbench

GUI follow-up (2026-10-18, FreeCAD `93a2bb06ac`): **Implicit Modelling** workbench with a
packaged SVG Add Gyroid icon and compiled `Implicit::Feature` creation. Default
domain is [0,10] mm on each axis; Length/Width/Height are editable unit-aware
properties driving DomainMax through saved expressions. Period/IsoValue remain
native editable properties. New workbench and existing experimental workbench
coexist. Lightweight entry point deployed at the user's requested FreeCAD
v27-1 Mod path; ordinary FreeCAD without the compiled core remains unsupported.
Targeted OCCT-8 CTest passed **3/3** (12.70 seconds), covering runtime audits and
the expanded GUI smoke test: icon loading, no-document creation, selection,
dimension/equation edits, undo/redo, FCStd reopening and repeated creation.
Log: `/tmp/implicit-workbench-gui-test.log`. No full-suite rerun is claimed.
The user subsequently resumed the full Stage 4 goal. The previously exhausted
segment budget is now resolved by the spatial index described below.
See [workbench usage](implicit-modelling-workbench.md).

Native-solid follow-up (2026-10-18): `BuildNativeSolids` now gates genuine
material-region solids on fresh whole-field/shell topology, actual cap geometry
coverage in both directions, 3D edge/vertex deviations and maximum stored
tolerances. A private segment BVH preserves both distance and parity checks,
resolving the open-contour budget failure without increasing the work limit or
weakening the validation. Unsupported/uncertain/limited cases never publish a
solid handle. This is restricted numerical implicit-aware 0.1 mm validation,
NOT formal intervals, generic UV BRepCheck/BOP or ordinary Part compatibility.
FreeCAD adds owned `build_mixed_solids`, `inspect_mixed_solids` and
`classify_mixed_solids` APIs. Original geometry/caches/provenance remain unchanged;
restored records are freshly checked. No solid property/FCStd codec or default
GUI solid creation is claimed. See [native-solid policy](native-solids.md).
Five kernel cases cover both operations/open and closed contours, whole-cap
missing/extra negatives, full/empty regions, islands/cavities, coordinate scaling/
periodic translations and work/tolerance failures. Full kernel suite:
**73/73** (86.01 seconds). Targeted native CLI + runtime: **2/2** (12.95 seconds).
Full OCCT-8 migration suite: **40/40** (260.53 seconds); compatibility suite:
**25/25** (134.06 seconds). These ran concurrently for part of their duration;
timings are regression observations, not benchmarks. Logs:
`/tmp/implicit-native-solid-kernel-full.log`, `/tmp/implicit-native-solid-cli.log`,
`/tmp/implicit-native-solid-migration-full.log`,
`/tmp/implicit-native-solid-compat-full.log`. Both source branches are clean at
OCCT `b032b19ae2` and FreeCAD `94175dd569`; only the umbrella gitlink/docs update
follows those source commits. No packages or system libraries changed.
Remaining Stage 2–4 gates, including curved operands, general component
restrictions, solid persistence, semantic vertex picking/snapping/history and
representation-aware GUI/consumer contracts, keep the resumed goal active.

## Previous increment: whole-face attachment and genuine native shells

TKTopAlgo `BuildNativeShells` now attaches the entire field zero set, including
enclosed/empty components, and builds actual native shells. Reference bounds and
native wire poles only propose component separators: each accepted axis plane
must be F-zero-free and have L's same strict sign throughout the homotopy. Exact
field equations with checked component box domains become new BRep_TImplicitFace
faces, preserving assigned shared native wires. Empty field faces are omitted.
No reference triangle becomes a CAD face; non-axis-separable cases stay unresolved.

Native shell validation checks opposite paired edge incidences, connected cyclic
vertex-germ links (including closed-edge multiplicities), connected face graphs
and V-E+sum(face Euler) against the whole-field reference. Conventional caps use
planar topology; exact field components use the homotopy Euler evidence. Shells
attach to retained material regions, distinguishing cavity shells from islands.
Source math/topology/caches/flags stay unchanged; only new private shell flags
change. Engineering topology checks, NOT geometric/solid validity or generic UV.

OCCT-8 FreeCAD adds build_mixed_shells / inspect_mixed_shells /
classify_mixed_shells, with owned shell and field-face handles, local authoritative
region queries and explicit not-Part/not-solid-valid contracts. No new FCStd
shell codec or property migration. See [native shell contracts](native-shells.md).

Three new cases bring the suite to **68**, covering both operations/open and
closed fixtures, exact field data/no fake triangulated faces, source immutability,
component splitting/limits, enclosed cavities and full/empty regions. CLI adds
lifetime/restore/classification/domain/handle guards. Its negative Part constructor
test found getPyShapes outside the Python exception boundary; moving that single
line inside fixes the escaping C++ TypeError. Preserved upstream CRLF, with a
CR-aware diff check. Both Part ABI targets were rebuilt; part_linked_smoke now
checks ordinary invalid objects/sequences plus valid/default constructors.

The initial CLI also incorrectly called the string kernel_version as a function;
that test typo is fixed. Kernel CTest **1/1**, **68 cases** (86.35 seconds) and
targeted runtime/shell CTest **2/2** (15.69 seconds) pass. Existing targets and
dependencies reused; no package/system-library changes.
Logs: `/tmp/implicit-native-shell-config.log`, `/tmp/implicit-native-shell-build.log`,
`/tmp/implicit-native-shell-test-build.log`, `/tmp/implicit-native-shell-targeted.log`,
`/tmp/implicit-native-shell-tests.log`, `/tmp/implicit-native-shell-binding-build.log`,
`/tmp/implicit-native-shell-part-guard-build.log`, `/tmp/implicit-native-shell-part-guard-compat-build.log`,
`/tmp/implicit-native-shell-binding-tests.log`, `/tmp/implicit-native-shell-migration-all.log`,
`/tmp/implicit-native-shell-compat-all.log`. Whole-boundary geometric validation,
native solids, general component restrictions, curved operands and unfinished
Stage 4 gates remain required; the full goal stays active.

Final full regression: OCCT-8 command-line/GUI migration **39/39** (194.48 seconds),
preserved compatibility **25/25** (93.32 seconds). Both core branches are published;
the parent pins the source heads above. Relevant Part targets built successfully
for OCCT 8 and preserved OCCT 7; no system-library/ABI mixing was introduced.

## Previous increment: dual-cap full trim-wire attachment

TKTopAlgo `CheckTrimAttachment` privately constructs/attaches the complementary
cap graph from the same field/domain. Shared grid-edge root provenance identifies
reference zero-surface boundary segments. Physical side, retained/complement cap
pair and ordered box-root anchors identify open arcs; closed contours use the
dual region pair and signed winding direction. Continuous PWL matching links
native seams to complementary traces, including reversed parameters/resampling.
Complete signed native/reference wire cycles must agree bijectively, up to
cyclic start and wire ordering. Ambiguous pairings and exhausted limits reject.

The explicit synchronous OCCT-8 API `check_mixed_trim_attachment` reports arc,
wire and bordering reference surface-component identities. These are transient
diagnostics, not Boolean history or whole implicit-face attachment. Source
math/topology/caches and restored seam provenance stay unchanged; native-shell/
solid/whole-face validity flags remain False. No formal interval certificate,
generic UV-based BOP or completed Stage 4 claim follows. See
[trim attachment contracts](native-trim-attachment.md).

Three new cases bring the kernel suite to **65**, covering both operations,
closed and three-wire open fixtures, two holes on one cap, distinct surface
components, restore/immutability and limits. Existing resampling/cyclic-start
regressions now check this query too; forged math fails the cap prerequisite.
Native CLI checks complete mappings, both contour/operation cases, source-record
equality, restore/lifetime/guards and the two-component multi-hole fixture.

The worktree/build artifacts from October 16 were revalidated after resuming on
October 18; prior /tmp logs were absent and no build/test processes were live.
Existing binaries were newer than changed sources; the relevant native build
reported no work, and tests were rerun. No packages/system libraries changed.
Current logs: `/tmp/implicit-trim-attachment-binding-build.log`,
`/tmp/implicit-trim-attachment-binding-tests.log`, `/tmp/implicit-trim-attachment-tests.log`,
`/tmp/implicit-trim-attachment-migration-all.log`, `/tmp/implicit-trim-attachment-compat-all.log`.
Kernel CTest **1/1**, **65 cases** (52.37 seconds) and targeted runtime/trim
CTest **2/2** (13.05 seconds) pass. Whole-face component restriction/attachment,
native shell/solid construction, curved operands and unfinished Stage 4 gates
remain required; the full goal stays active.

Final full regression: OCCT-8 command-line/GUI migration **38/38** (178.21 seconds),
preserved compatibility **25/25** (80.61 seconds), including the current 65-case
isolated executable. Both core branches are published; the parent pins the
source heads above.

## Previous increment: stored conventional cap-region attachment

TKTopAlgo `CheckCapAttachment` now maps source cap regions to closed reference
cap/shell/material labels. Independent stored-graph and homotopy-reference checks
must resolve first. Actual common F/L retained points classify IN source caps
and must clear every stored cap polyline beyond the fresh seam, continuous curve,
p-curve and matching uncertainty. A shared-edge/side reference grouping and
bijective assignment avoid nearest-cap matching or merely comparing counts.
Deep-triangle graph ordering is only a search heuristic, never acceptance evidence.

OCCT-8 FreeCAD exposes `check_mixed_cap_attachment`, with source-order transient
labels, witness points, clearance bounds and work diagnostics. Source math,
topology/caches and seam provenance stay unchanged. Budgets/ambiguity/missing
witnesses remain unresolved. Engineering reserves, not directed intervals or
formal whole-native isotopy. Trim-wire/implicit-face attachment, actual native
shell/solid construction, curved operands and all unfinished Stage 2/3/4 gates
remain required. See [cap attachment contracts](native-cap-attachment.md).

Two new cases bring the kernel suite to **62**, covering both Boolean directions,
open/closed contours, restore provenance/immutability, actual IN/F-sign witnesses,
multiple regions and cap reordering, limits and inputs. Forged math/bounds stop
at the graph prerequisite. CLI independently checks Part cap/edge distances,
restore/record equality, owned lifetime and settings/handle guards.

Reused existing targets/dependencies; explicitly reconfigured the OCCT manifest
for the new source file. No packages/system changes or persistence schema change.
Logs: `/tmp/implicit-cap-attachment-config.log`, `/tmp/implicit-cap-attachment-build.log`,
`/tmp/implicit-cap-attachment-test-build.log`, `/tmp/implicit-cap-attachment-targeted.log`,
`/tmp/implicit-cap-attachment-tests.log`, `/tmp/implicit-cap-attachment-binding-build.log`,
`/tmp/implicit-cap-attachment-binding-tests.log`, `/tmp/implicit-cap-attachment-migration-all.log`,
`/tmp/implicit-cap-attachment-compat-all.log`. Kernel CTest **1/1**, **62 cases**
(56.93 seconds) and targeted runtime/cap CTest **2/2** (12.75 seconds) pass.
Full OCCT-8 command-line/GUI migration **37/37** (219.58 seconds) and preserved
compatibility **25/25** (98.39 seconds) pass. Both core branches are published;
the parent pins the source heads above.

## Previous increment: closed region reference and material connectivity

TKTopAlgo `CheckRegionTopology` adds retained affine-clipped box caps to the
shared homotopy surface reference. Cap roots reuse originating grid-edge IDs;
retained grid vertices are shared across faces. No coordinate welding, new
field projection or authoritative geometry mutation. Closed edge/cyclic-link
checks produce closed-component labels and Euler characteristics.

Retained volume connectivity is computed from the common tetrahedral 1-skeleton:
affine retained cells/faces are convex and contain retained grid vertices, so
these vertex connections represent volume regions. Every boundary triangle and
closed component must agree on its adjacent region; every region needs a shell.
Cavity/outer shells attach to one region, while separate islands get distinct
regions. Translated long-double component/region volumes are reference-only
diagnostics, never field-volume accuracy or the grouping rule.

OCCT-8 FreeCAD exposes `check_box_region_topology` and
`check_mixed_region_topology`, with local combined buffers, original surface
prefixes, side/component/region labels and unchanged source provenance. Calls
are explicit/synchronous. No native-shell/solid construction, stored trim/cap
correspondence or .1 mm mesh/volume accuracy is claimed. The full Stage 4 goal,
curved operands and all unfinished integration/solid gates remain active. See
[region-reference contracts](native-region-topology.md).

Three kernel cases bring the suite to **60**, with both operations/open and
closed trim fixtures, opposite shared edges, cap normals, Euler accounting,
complementary reference volumes, cavity/island/full/empty topology, immutability
and limits. The cavity reference passes the separate exact binary64 contact
audit (test-only TKMesh use; no TKTopAlgo dependency cycle). CLI independently
reconstructs buffer Euler/edge/volume checks, validates source surface prefixes,
raw/mixed adapters, restore/lifetime/guards and two-cavity material grouping.

Existing build/dependency directories reused, no packages/system changes.
Logs: `/tmp/implicit-region-topology-build.log`,
`/tmp/implicit-region-topology-test-build.log`, `/tmp/implicit-region-topology-targeted.log`,
`/tmp/implicit-region-topology-tests.log`, `/tmp/implicit-region-topology-binding-build.log`,
`/tmp/implicit-region-topology-binding-tests.log`, `/tmp/implicit-region-topology-migration-all.log`,
`/tmp/implicit-region-topology-compat-all.log`. Kernel CTest **1/1**, **60 cases**
(46.44 seconds) and targeted runtime/region CTest **2/2** (12.19 seconds) pass.
Full OCCT-8 command-line/GUI migration **36/36** (219.92 seconds) and preserved
compatibility **25/25** (85.85 seconds) pass. TKTopAlgo/TKMesh, isolated tests,
the native binding and script targets were rebuilt against the current witness
layout. Both core branches are published; the parent pins the heads above.

## Previous increment: whole-box numerical field/linear homotopy

TKTopAlgo now checks the entire mathematical box zero set against a shared
Freudenthal nodal-linear reference. Adaptive simplex checks exclude homotopy
zeros or prove positive field/linear gradient alignment throughout each cell.
The continuous field-gradient direction increases all incident linear pieces
at internal grid boundaries; actual exterior box-face/edge restrictions have
separate projected checks. Initial artificial-grid transversality requirements
were too strict at harmless internal tangencies and were replaced by this
common-direction condition. Coarse grids and derivative/subdivision limits can
still conservatively remain unresolved. Engineering reserves, not intervals.

The explicit OCCT-8 APIs `check_box_topology` and `check_mixed_topology` return
transient affine-root reference buffers, component Euler characteristics,
boundary loops and incidence/link checks. No source state changes; no native
topology/solid or geometric accuracy flags are promoted. The witness still
needs correspondence to stored trims/caps before native-shell construction.
Curved operands and all unfinished Stage 2/3/4 gates remain active. See
[whole-box topology contracts](native-box-topology.md).

Five new kernel cases bring the suite to **57**, including hidden between-node
components, enclosed sphere/empty topology, critical strata, scaling, orientation,
immutability, bad inputs and limits. CLI covers both operations/open and closed
fixtures, raw/mixed equivalence, restore provenance/lifetime and independent
Euler/edge/loop accounting. The larger fixture has Euler -1 and three boundary
loops at resolutions 32 and 64. Its topology differs from the small disk fixture;
the CLI was corrected to avoid incorrectly assuming both were disks.

Existing build/dependency directories reused; the OCCT package manifest was
explicitly reconfigured to pick up the new translation unit. No packages/system
libraries changed. Logs: `/tmp/implicit-topology-config.log`,
`/tmp/implicit-topology-build.log`, `/tmp/implicit-topology-test-build.log`,
`/tmp/implicit-topology-targeted.log`, `/tmp/implicit-topology-tests.log`,
`/tmp/implicit-topology-binding-build.log`, `/tmp/implicit-topology-binding-tests.log`,
`/tmp/implicit-topology-migration-all.log`, `/tmp/implicit-topology-compat-all.log`.
Kernel CTest **1/1**, **57 cases** (43.77 seconds); targeted runtime/topology
CTest **2/2** (7.88 seconds) pass.
Full OCCT-8 command-line/GUI migration **35/35** (187.84 seconds) and preserved
compatibility **25/25** (74.48 seconds) pass. Both core branches are published;
the umbrella pins the source heads above.

## Previous increment: stored native trim-graph correspondence

TKTopAlgo independently retraces the authoritative field/box after stratified
regularity, then requires unique continuous PWL edge matches, induced shared
vertex bijections, cap grouping and directed cyclic wire agreement. Strict
degree-1 cap p-curve parameter checks and derivative-based positive seam tangent
alignment supplement geometric matching. The existing Build canonical graph
construction is shared, without a new toolkit dependency. No source geometry,
caches or stored seam-bound provenance changes.

`ImplicitOCCT.check_mixed_correspondence` exposes the explicit synchronous query
in OCCT-8 FreeCAD. Tight reproduction tolerance and the combined fresh seam,
3D curve and cap parameter budget are separate from whole-model accuracy.
Four new cases bring the suite to **52 GTests**, covering both operations,
open/closed/multiple regions, restore, reordered caps, curve resampling, cyclic
wire starts, changed math with forged flags, p-curve mismatch and limits. Native
CLI adds ownership/restore/provenance and handle/settings guards. See
[correspondence contracts](native-trim-correspondence.md).

This is graph correspondence, NOT full surface isotopy/reach or native-solid
validity. The full Stage 4 goal remains active, including curved operands and
every unfinished Stage 2/3/4 acceptance gate. No packages/system libraries changed.

Build/test logs: `/tmp/implicit-correspondence-build.log`,
`/tmp/implicit-correspondence-test-build.log`, `/tmp/implicit-correspondence-tests.log`,
`/tmp/implicit-correspondence-binding-build.log`, `/tmp/implicit-correspondence-binding-tests.log`,
`/tmp/implicit-correspondence-migration-all.log`, `/tmp/implicit-correspondence-compat-all.log`.
Isolated kernel CTest passes **1/1**, **52 cases** (41.16 seconds); targeted
runtime/correspondence CTest passes **2/2** (6.72 seconds).
Full OCCT-8 migration passes **34/34** (169.72 seconds), including actual GUI
and launcher workflows; preserved compatibility passes **25/25** (70.03 seconds).
The initial CLI budget-one assertion incorrectly assumed every graph needed
multiple comparisons; the one-edge closed cut legitimately needs just one.
The test now distinguishes that successful bounded case from exhaustion.

## Previous increment: mesh-independent box-stratified field regularity

TKTopAlgo now adaptively checks projected gradients over the whole native
gyroid box volume, all faces and edges, and excludes corner zeros. Scalar
Lipschitz/Taylor empty-cell bounds and Hessian gradient-variation bounds use
engineering reserves, not directed intervals. A full gradient alone cannot
resolve face/edge tangencies; each stratum has its own completion/bounds.
No mesh/tracing caches or stored verification flags are consulted or changed.

`ImplicitOCCT.check_mixed_regularity` and `check_box_regularity` expose this in
the OCCT-8 build. The latter checks a native face's BOX domain, not stored trim
wires. Derivatives are field/model-length, not mm errors; partial bounds are
global only for completed strata. Depth/work limits remain unresolved. Reports
explicitly keep trim correspondence, topology and solid validity False.

Three new cases bring the kernel suite to **48 GTests**, including independent
analytic volume/face/edge/corner critical cases, both operations/open and closed
contours, source immutability/restore, physical scaling and periodic translation.
The negative face/edge cases have higher-dimensional regularity, so they test
projection dispatch rather than merely a zero full gradient. CLI tests add
raw/mixed equivalence, lifetime/restore and representation/placement guards.
See [regularity contracts](native-box-regularity.md). Stored trim correspondence,
topology/reach and native-solid validation remain required; the Stage 4 goal stays
active. No dependencies/system libraries changed.

Build/logs: `/tmp/implicit-regularity-build.log`,
`/tmp/implicit-regularity-test-build.log`, `/tmp/implicit-regularity-tests.log`,
`/tmp/implicit-regularity-binding-build.log`, `/tmp/implicit-regularity-binding-tests.log`,
`/tmp/implicit-regularity-migration-full.log`, `/tmp/implicit-regularity-compatibility-full.log`.
Isolated kernel CTest passes **1/1**, **48 cases** (36.31 seconds); targeted
runtime/regularity CTest **2/2** (3.18 seconds). Full migration passes **33/33**
(171.99 seconds), compatibility **25/25** (67.86 seconds). TKTopAlgo/TKMesh,
isolated tests, the native binding and both script targets build successfully.

## Previous increment: whole-domain clipped-boundary proximity

Native CheckCoverage partitions the entire box/retained faces using field
exclusion or explicit mesh-point distance plus radius. Reverse patches use
clipped normal slabs, classifier brackets or retained cap-plane witnesses, not
untrimmed-distance flags. A private triangle BVH accelerates upper-distance
queries. Initial cap failures revealed a 3D-gradient witness search leaving the
box; cap-plane gradients/in-plane directions now find actual retained points.

`ImplicitOCCT.check_mixed_coverage` is explicit/synchronous and leaves source
math/topology/caches, GUI buffers and stored seam-bound provenance unchanged.
Depth/spatial/patch exhaustion is unresolved, never success. Global bounds apply
ONLY when boundary_proximity_resolved is True; other bounds are partial diagnostics.
These are engineering floating reserves, not directed intervals, preserved
topology or native-solid validity. All unfinished Stage 2/3/4 gates remain active.
See [clipped proximity contracts](native-clipped-coverage.md).

Three new cases bring the kernel suite to 45: independent harmonic roots/raw
cap samples and a separate brute-force distance reference, one missing component
while retaining the other, missing whole surfaces, extra interior triangles,
forged flags and limits. CLI covers both operations/open and closed contours,
source immutability, restore, lifetime and invalid/limited calls.
TKMesh, isolated tests, OCCT-8 binding and both script targets build. Final suites:
**45 GTests**, CTest **1/1** (37.18 seconds), migration **32/32** (182.37 seconds),
compatibility **25/25** (69.67 seconds). Targeted runtime/coverage **2/2** passed
(23.49 seconds) before independent reference expansion. No dependencies changed.
Logs: `/tmp/implicit-coverage-build.log`, `/tmp/implicit-coverage-test-build.log`,
`/tmp/implicit-coverage-tests.log`, `/tmp/implicit-coverage-binding-build.log`,
`/tmp/implicit-coverage-binding-tests.log`, `/tmp/implicit-coverage-migration-full.log`,
`/tmp/implicit-coverage-compatibility-full.log`.

## Previous increment: exact transient triangle-contact audit

The native TKMesh audit now checks all cap/implicit pairs, including adjacent
triangles, using exact stored-binary64 integer predicates and bounded AABB
sweeping. Clear results require an exact separating witness confining contacts
to shared node-ID simplices; no separating plane may ignore part of a shared
edge. A negative coplanar-overlap test caught that initial adjacency bug.
Integer arithmetic is local (32-bit words/64-bit carries, common dyadic scale),
with explicit exponent and pair/broad-phase budgets. No dependencies changed.

`ImplicitOCCT.audit_mixed_mesh` privately generates/audits a candidate, retaining
source immutability. Reports distinguish clear, contact_or_unresolved and limit;
limits never pass. Result metadata names generation settings, sizes, diagnostic
triangle IDs and work counts; solid/coverage flags remain False. The API is
separate from GUI buffers and does not persist or change a document validity
flag. GUI previews do not automatically run the audit.

Two new kernel cases bring the executable to **42 GTests**, covering valid
adjacency, crossings beyond shared vertices, same-side shared-edge overlaps,
duplicates, unrelated coincident nodes, near separation, exact coplanar oblique
separation under binary scaling/winding changes, limits and invalid indices.
The native iso-0 candidate passes 906,095 exact pair checks. CLI regression
passes both Boolean directions/open and closed contours, limited-audit states,
source immutability, retained lifetime and bad-handle/settings guards.

This is mesh-only embeddedness evidence, not field coverage, clipped-field
Hausdorff error or native-solid validity. All unfinished Stage 2/3/4 gates
remain active; see [audit contracts](native-mesh-contact-audit.md).
Logs: `/tmp/implicit-contact-build.log`, `/tmp/implicit-contact-test-build.log`,
`/tmp/implicit-contact-tests.log`, `/tmp/implicit-contact-binding-build.log`,
`/tmp/implicit-contact-binding-tests.log`, `/tmp/implicit-contact-migration-full.log`,
`/tmp/implicit-contact-compatibility-full.log`.
Final results: isolated CTest **1/1**, **42 GTests** (20.03 seconds), full
OCCT-8 migration **31/31** (121.03 seconds), compatibility **25/25**
(45.43 seconds). Both script targets, TKMesh, the native test executable and
the OCCT-8 binding build successfully. GCC UBSan additionally passes the exact
predicate case (including overlapping-box tiny-gap/crossing regressions), with
only the new audit/test translation units instrumented, not all OCCT libraries.
Sanitizer logs: `/tmp/implicit-contact-ubsan-build.log`, `/tmp/implicit-contact-ubsan.log`.

## Previous increment: native trim-conforming surface candidate and GUI preview

The native TKMesh mesher now uses field-root extraction, native-wire/interior-
loop correspondence, an oriented dynamic-programming collar and shared-edge
projected refinement. Cap and surface triangles share canonical nodes; original
trim samples remain fixed. A private cap discretizer retains every knot and
adds samples no farther apart than capDeflection/4, with preflight limits.
No arbitrary field is assigned to Part Shape or passed through UV meshing.

The wider two-loop test exposed that grid resolution alone did not resolve
collar error. Aspect-balanced grids plus conforming projected refinement now
handle that case, both Boolean directions/open and closed contours, and physical
scale/periodic translations. Kernel tests independently sample root witnesses
and check source caches, exact seam identity, orientation, incidence and failures.
All **40 GTests** pass (CTest **1/1**, 12.11 seconds).

`ImplicitOCCT.tessellate_mixed` reports a closed candidate and a triangle-wide
one-sided distance bound to the UNTRIMMED gyroid. This is not clipped-root or
Hausdorff/coverage certification. Mesh closure checks include connected cyclic
vertex links; embeddedness/self-intersections remain unverified. Public flags
retain surface_coverage_verified=False and solid_validated=False.

`ShowImplicitSurface` in the GUI View properties enables the explicit candidate
with separate tolerance/grid settings and a read-only diagnostic. Caps then use
the same constrained samples; actual ray picks/detail paths identify NativeFace1.
Native snapshot/settings cache reuse avoids camera-driven regeneration. Preview
failure removes stale buffers without deleting valid math; source failure clears
all views. FCStd restores native topology/settings and regenerates transient
buffers. Meshing is synchronous, without cancellation/background qualification.

See [native conforming preview](native-conforming-preview.md) for API, accuracy
derivation, resource limits, usage and remaining gates. This advances Stage 3
and Stage 4 preview/selection, not native-solid validation or overall completion.
Curved operands, coverage/clipping, embeddedness, history/snapping and broader
consumer integration remain required. No dependencies/system libraries changed.
Build/test logs: `/tmp/implicit-surface-build.log`,
`/tmp/implicit-surface-test-build.log`, `/tmp/implicit-surface-tests.log`,
`/tmp/implicit-surface-binding-build.log`, `/tmp/implicit-surface-migration-full.log`,
`/tmp/implicit-surface-compatibility-full.log`.
Both script targets and the OCCT-8 binding build successfully. Full migration
passes **30/30** (96.24 seconds); compatibility passes **25/25** (48.58 seconds),
including the isolated 40-case kernel suite. Earlier runs in this increment
preceded the wider-fixture failure/fix; the results above are the final algorithm.

## Previous increment: native shared cap/seam meshing constraints

`TKMesh/BRepMesh_ImplicitBoundary` privately validates/copies native mixed-box
topology and meshes conventional planar caps only. A private exact-polyline
discretizer preserves every seam knot: ordinary chordal tessellation initially
discarded short knots and failed the new correspondence tests. Canonical node
IDs use native shared vertices and ordered edge samples, never position welding.
Every native wire segment has one opposite cap incidence; other cap edges are
paired. Source flags, topology and triangulation caches stay untouched.

The OCCT-8-only `ImplicitOCCT.prepare_mixed_mesh` exposes vertices, cap triangle/
cap indices, seam indices and oriented closed wire indices. Results retain
input seam-bound provenance; native_surface_meshed and solid_validated are
explicitly False. No implicit face goes through generic UV meshing, no fake Part
shape is assigned, and the GUI remains caps/seams only. See
[native mesh constraints](native-mesh-constraints.md) for precise limits/API.

Two kernel cases exercise both operations/open and closed contours, knot
retention, orientation/incidence, restored provenance, budgets and source mesh
cache preservation. Native CLI checks additionally cover deterministic buffers,
property/capsule equivalence, cap-index mapping, independent Part cap caches,
failure atomicity and retained lifetime after document close.

Incremental TKMesh, native test executable, OCCT-8 `_ImplicitOCCT` and both
ImplicitScripts targets build successfully using existing dependencies. Kernel
CTest passes **1/1**, containing **37 GTests** (4.12 seconds). Targeted native
runtime/preparation checks pass **2/2** (6.03 seconds). Full OCCT-8 migration
passes **29/29** (64.67 seconds); compatibility passes **25/25** (28.69 seconds).
Logs: `/tmp/implicit-mesh-constraints-build.log`,
`/tmp/implicit-mesh-binding-build.log`, `/tmp/implicit-mesh-binding-tests.log`,
`/tmp/implicit-mesh-migration-full.log`, `/tmp/implicit-mesh-compatibility-full.log`.
No packages/system libraries changed. This is preparation for a trim-conforming
implicit surface mesh, not its completion; all remaining Stage 2/3/4 requirements
remain active, including curved operands and native solid validation.

## Previous increment: transactional native mixed task editor

Supported mixed results now open a Qt task panel through standard edit mode or
tree double-click. Operation, seam tolerance (mm) and tracing budget are staged
until Apply/OK; field/box source links remain read-only. One owned transaction
groups previews, OK commits and Cancel restores prior native geometry/properties.
Failed previews clear geometry/display and stay open for correction or cancel.
Successful publication is followed by normal dependency recompute; failed direct
preview is pending recompute, not a claim that all C++ error bits are already set.

New GUI regression covers real task dialogs and Qt Apply/Cancel/OK buttons,
standard Edit/double-click, selection and region queries, mapped identity on
cancel, immutable lifetime, commit/undo/redo, failed-budget clearing/recovery,
reset-edit rollback, unrelated transaction guards, accepted FCStd reopen/re-edit
and document close with an active preview. Initial transaction testing exposed
that framework edit-mode closing can affect an unrelated transaction: normal
task actions now refuse that situation, and only close their owned ID.

This is the restricted recipe's three-setting editor, not graph/equation/source-
link editing, history repair, complete consumer integration or any Stage 2/3
surface/solid/Boolean completion. All remaining Stage 4 requirements stay active.
No C++/OCCT source, new dependencies or system libraries changed; both
ImplicitScripts targets rebuild successfully. See [editor contracts](native-mixed-editor.md).

Full migration passes **28/28** (56.20 seconds), compatibility **25/25**
(27.78 seconds), including unchanged **35** kernel cases. Additional targeted
menu/button tests pass **3/3** (4.86 seconds). Logs:
`/tmp/implicit-editor-scripts.log`, `/tmp/implicit-editor-legacy-scripts.log`,
`/tmp/implicit-editor-targeted.log`, `/tmp/implicit-editor-buttons.log`,
`/tmp/implicit-editor-migration.log`, `/tmp/implicit-editor-compatibility.log`.
Final task-only regression additionally covers the open-contour fixture and
the power-of-two 4..512 budget selector; results/log:
`/tmp/implicit-editor-final.log`. Both script targets were refreshed afterward.
Final **3/3** task-only checks pass (7.04 seconds), including multiple previews
in one undo transaction and cancellation directly from a failed preview.

## Previous increment: placed native mathematical-region queries

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
