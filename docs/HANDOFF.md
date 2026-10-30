# Development handoff

Last updated: 2026-10-30. This file is the entry point for a developer or coding
agent continuing the project. Read [`PROGRESS.md`](PROGRESS.md) for the latest
FreeCAD GUI, live Part dependencies, native projection/probes/quality, ray queries, background meshing,
benchmarks and OCCT core increments.
The user has authorized implementation, commits and pushes in both core source
trees. This file records the state of the code, not a claim
that all goals in the original brief are complete.
[`DEVELOPMENT_STAGES.md`](DEVELOPMENT_STAGES.md) maps the original phases to
delivered prototypes and explicit remaining production/kernel acceptance gates.

Latest GUI increment: [Implicit Modelling workbench](implicit-modelling-workbench.md)
adds an icon-driven editable 10 mm cube gyroid using compiled `Implicit::Feature`.
Saved expressions connect Length/Width/Height to DomainMax. The legacy workbench
is retained. A user-directory entry point is deployed under the requested
`~/.local/share/FreeCAD/v27-1/Mod/ImplicitModelling`; it requires the fork runtime.
The user has now resumed the full through-Stage-4 goal. The separate OCCT
solid-geometry work now uses a private segment spatial index; the previously
budget-exhausted targeted case passes without weakening the geometry checks.
See [restricted native solids](native-solids.md) for the new actual-solid
construction policy, owned FreeCAD API and the remaining GUI/persistence gates.
Do not infer Stage 4 completion from this restricted factory or GUI increment.

## Milestone 105: reverse mathematical sheet display coverage

Native heads: FreeCAD 29ca79ca13; OCCT bf6afd3bc6.
OCCT cylinder regression passes 23/23 (44.18 s); selected FreeCAD CLI/GUI
checks pass 20/20 (76.02 s). Both signs, translated/reversed and tilted
frames pass independent root-to-triangle distance checks; coarse resolution
remains unverified. Built TKTopAlgo/native harness and matched Implicit/native
bridge using existing builds. Python syntax and whitespace checks pass.
No dependencies or system libraries changed.

The uploaded cylinder sheet now has a reverse distance bound from the entire
mathematical full-disk sheet. A matching concentric reference chart, whole-
triangle residuals, full-cylinder axial derivative cover and rim graph-slope
allowance cover triangle interiors and the curved disk rim. OCCT adds an ABI-
preserving overload to request a minimum derivative lower bound; the display
computes that threshold from residuals and tolerance instead of sampling slopes.
The overall display certificate remains false pending side bounds and contacts.
Background work, restored material queries, live edits and broader kernel/GUI
Stage 2–4 gates remain open.

## Milestone 104: bidirectional uploaded cap-disk distance

Native heads: FreeCAD 4598c8c873; OCCT unchanged at 7f042a8496.
Selected CLI/GUI checks pass 5/5 (18.30 s); final strengthened preview
checks pass 2/2 (6.86 s). Built Implicit with two jobs; whitespace checks pass.

The preview now compares its entire uploaded cap fan with the mathematical
retained disk in both directions. A matching ideal inscribed fan, maximum
binary32 vertex perturbation, checked single angular winding and circular
sagitta bound cover triangle interiors and the curved rim. Independent disk-to-
triangle and triangle-to-disk samples exercise both material signs and the
translated/reversed frame. The field subset remains separately directed;
reverse sheet coverage, side bounds and triangle contacts remain open before
any total 0.1 mm display contract. Stage 4 remains incomplete.

## Milestone 103: whole uploaded field-triangle distance

Native heads: FreeCAD 4322c26a15; OCCT unchanged at 7f042a8496.
Selected CLI/GUI checks pass 5/5 (19.15 s); Implicit built with two jobs.
Whitespace checks pass; no dependencies or system libraries changed.

Cylinder previews now bound every field triangle against the masked mathematical
sheet, using binary32 upload coordinates, a Hessian interpolation bound, axial
derivative cones and convex masked witness prisms. Resolution-6 tests qualify;
coarse resolution-2 stays explicitly unresolved. The bound is directed and covers
only the field face; overall surface verification remains false. Reverse
coverage, cap/side accuracy, contacts, background work, live edits and broader
Stage 2–4 requirements remain open. See native-cylinder-preview.md.

## Milestone 102: conforming cylinder surface preview and native picks

Native heads: FreeCAD 2465b026a5; OCCT unchanged at 7f042a8496.

Implicit.cylinderPreview builds a transient cap/side/field preview from shared
native boundary indices. Interior sheet nodes use bracketed field roots with
spatial bounds; cap/cylinder/analytic field normals respect material orientation.
Opposite edge incidence, used nodes and Euler characteristic are audited.
Source records/topology remain unchanged. The surface-distance certificate is
explicitly false: root-point, source-boundary, curve and upload bounds are
separate. Generic getFaces still refuses rather than imply a stronger contract.

ImplicitCylinder.attach_view supplies an opt-in Coin renderer with native face/
edge/vertex tags, typed owned subobject resolution and detail paths. Identity-
placed frozen snapshots are supported; broad placements/links/history remain
open. View buffers clear on unverified/hidden/failed states and rebuild through
qualification/undo/redo. FCStd stores no render buffers. Visual inspection caught
an RGBA-to-RGB bug, now guarded by a material assertion, and circle sampling now
scales with resolution for a smoother silhouette. See native-cylinder-preview.md.

Selected CLI/GUI integration passes 20/20 (62.26 s): cylinder families, legacy
properties/references/elements/topology, both material signs and translated/
reversed frames, independent roots, mesh incidence, normals, source immutability,
real viewport face/edge/vertex hits, typed selected geometry, save/reopen,
hide/failure clearing and undo/redo. Python syntax and whitespace checks pass.
Built existing Implicit/ImplicitScripts, two compile jobs/one link. OCCT is
unchanged; no packages or system libraries changed. Current screenshot inspected
at /tmp/implicit-cylinder-preview.png; optional test env captures it again.

Next: global bidirectional surface/error coverage and triangle-contact checks
for a total 0.1 mm display contract, cancellable background requalification/
preview, restored material queries and live field/operand editing. General
partial/multi-sheet, Boolean dispatch/history/consumer gates remain required.
This restricted preview does not complete Stage 4.

## Cylinder display prerequisite: shared native boundary sampling

Native heads: FreeCAD fedbda5886; OCCT unchanged at 7f042a8496.

Implicit.nativeGeometry now returns an owned CylinderGeometry snapshot for the
dedicated property. Qualified snapshots expose actual native vertex positions,
per-element boundary lines and whole-graph sampled curves. Native endpoint
identity shares junction indices; no distance welding or mesh vertices become
CAD vertices. Degree-one splines retain their stored poles, lines use native endpoints and
analytic circles use an R*angle^2/(8*N^2) chord bound, including endpoint and
floating reserves. Invalid/too-tight accuracy and bounded node limits refuse.

This is transient native-curve sampling, not surface tessellation, mathematical
field-distance certification or viewport picking. Surface extraction continues
to refuse. The stored field/topology record remains unchanged. Unverified
owners expose no sampled geometry; owned snapshots survive document close.
See native-cylinder-document.md for API/accuracy distinctions.

Runtime/cylinder/property/reference/element/topology checks pass 17/17 (47.26 s).
GUI runtime/property checks pass 3/3 (7.61 s), exercising the same snapshot APIs
in the GUI process as well as tree lifecycle. Tests audit native junction degrees,
three actual vertices, independent circle midpoint errors, refusal paths, source
immutability and snapshot lifetime. Syntax/whitespace checks pass. Existing
Implicit build only, two jobs/one link; CMake regeneration relinked dependencies.
OCCT unchanged; no packages/system libraries changed.

Next: use these shared native boundaries in the trim-conforming field/cap/side
surface cache, then GUI rendering and semantic picking. Combine field-boundary,
surface and upload allowances before claiming total display accuracy. Background
requalification, live editing and all broader Stage 2–4 requirements remain open.

## Milestone 101: native cylinder document/property lifecycle

Native heads: FreeCAD ea496252d4; OCCT unchanged at 7f042a8496.

FreeCAD now registers CylinderGeometry, PropertyNativeCylinderGeometry and
NativeCylinderFeature. The dedicated complex geometry property owns the actual
qualified cylinder graph or an unverified stored owner, without a Part Shape.
It saves explicit graph payloads in FCStd and restores unverified; ordinary
recompute does not promote them. Property assignment stages validation before
mutation. Copy/Paste and undo/redo share immutable owners.

The derived GeometryState label is transient. Inline XML stores a bounded byte
length, fixing the initial content-transfer failure and preventing saved Ready
text from overriding loaded state. Compressed property transfer and inline
restore are tested separately. Qualified topology exposes native subelement
counts/segments to core consumers, but viewport picking/history remain open.
Surface extraction and nonidentity geometry transforms explicitly refuse.

Runtime/cylinder/legacy property and I/O checks pass 14/14 (41.46 s). GUI runtime
and new cylinder property lifecycle pass 3/3 (7.63 s), covering real tree ownership,
whole-object selection, save/reopen, explicit requalification, undo/redo and
failed assignment preservation. Python syntax and repository whitespace checks
pass. Built only the existing Implicit target with two jobs/one link; CMake
regeneration also rebuilt existing Base/App/Part dependencies. No packages or
system libraries changed. OCCT is unchanged; previous full kernel evidence is
separate from these property tests. See native-cylinder-document.md.

Next: trim-conforming transient cylinder display, background requalification,
semantic viewport picks and live field/operand editing. This is not a completed
cylinder workbench workflow or Stage 4 completion. All broader kernel/consumer/
placement/history gates remain open.

## Milestone 100: requalified stored cylinder solids

Native heads: FreeCAD 1b53afda55; OCCT 7f042a8496.

BoundVolume and QualifySolid now use fresh bounded mathematical material volume
on the unchanged verified stored graph. Construction and restoration share a
private integrator; only qualified geometry paths can reach it. Saved volume
numbers and axial provenance are not certificates. Positive partial volume and
closed shape gates qualify the same existing TopoDS_Solid, with no reconstruction.
Work budgets, coarse-volume refusal and final cancellation gates are preserved;
callbacks are not retained. Validity is the explicit implicit-aware contract.

FreeCAD bound_stored_cylinder_volume/qualify_stored_cylinder_solid accept verified
stored geometry or qualified restored-solid owners. Failed owners reject;
inspect_qualified_stored_cylinder_solid reports without rebuilding. Raw loaded
owners and dictionaries cannot promote themselves. dumps_cylinder_solid now
also exports qualified restored owners, with fresh volume/bound provenance.
Read still clears all certificates and checks actual shared topology.

Focused native IO passes 1/1 (6.928 s); runtime/cylinder/legacy-solid-IO passes
11/11 (26.77 s). Full native regression passes 160/160 (CTest 337.25 s).
Python syntax and all repository whitespace checks pass. Tests cover both signs,
translations, original graph identity,
bogus saved volume provenance, failure budgets, coarse-volume refusal, late
cancellation, immutable reports and export/read certificate clearing.

Next: trim-conforming cylinder display and a dedicated live document geometry
property with FCStd restoration/requalification and GUI lifecycle tests. Material
point queries for restored owners are not yet connected. General mixed Boolean
dispatch/history, partial/multi-sheet support and broader Stage 2–4 gates remain
open. Generic OCCT consumers remain unsupported. Existing builds/dependencies only.

## Owned restored cylinder geometry

Native heads: FreeCAD 91f83f3e14; OCCT eb5e4fb72a.

CheckGeometry / qualify_stored_cylinder_geometry combine the stored topology,
cap/side material regions and boundary/tolerance gates without reconstructing
the graph. Audited world parameters come from the actual native mask; original
recipe/volume/bound numbers remain provenance. All declared tolerances and
curve endpoints versus vertex tolerances are checked. The result owns the same
actual shared solid graph through a distinct immutable geometry capsule.

The original loaded owner stays unqualified; the new owner can report verified
geometry but volume/solid flags remain false. Failed results are inspectable;
dictionary edits cannot promote owners. inspect_stored_cylinder_geometry does
not rebuild. The older construction material/volume APIs still reject this type.
Focused IO passes 1/1 (4.158 s); fork runtime/IO passes 2/2 (4.77 s).
Cylinder group passes 22/22 (38.986 s). Earlier full regression is separate
evidence. Python syntax and repository whitespace checks pass; existing builds
and dependencies only, no library mutations during tests.

Next: fresh bounded material volume and solid qualification directly on this
unchanged restored geometry, then document property/display/GUI integration.
Partial/multi-sheet and all broader Stage 2–4 gates remain open. See
native-cylinder-persistence.md for contracts and ownership limitations.

## Stored-cylinder retained-side revalidation

Native heads: FreeCAD c609825193; OCCT f88fdcf9ad.

CheckRetainedSide / check_stored_cylinder_retained_side now inspect the actual
stored cylinder face. Fresh cap/field, curve-consistency and seam coverage
prerequisites share one budget. Cylinder frame/radius/outward orientation,
strictly monotone single field graph, two oriented cap arcs, paired periodic
seams, full angular coverage and ordered UV joins select the correct material
region. No retracing or replacement face construction occurs.

Combined seam/curve/frame/join bounds must fit the query and implicit trim face;
exact cylinder surface alignment is checked against its own face tolerance.
Stored qualification flags stay false. Tests include both signs, translations,
budget failure, shifted cylinder radius and a field p-curve moved a full turn.
Cylinder regression passes 22/22 (38.900 s); runtime/cylinder/legacy-solid-IO
passes 11/11 (25.24 s). Python syntax and repository whitespace checks pass.
The earlier full suite belongs to the previous checkpoint. Existing
build/dependency trees only; no library changes during tests.

Next: combine the actual stored topology, cap/side material regions, implicit
face domain/orientation and all declared tolerances into an owned restored
geometry contract, then rerun volume/solid qualification without reconstructing
the graph. Live document/display/GUI and broader Stage 2–4 gates remain open.
See native-cylinder-persistence.md.

## Stored-cylinder retained-cap revalidation

Native heads: FreeCAD fc5f421723; OCCT 7db5677b48.

CheckRetainedCap / check_stored_cylinder_retained_cap now rerun whole-cylinder
axial uniqueness and strict opposite signs over both complete end disks. Fresh
proofs determine the correct retained material end without trusting stored axial
direction. The actual stored cap plane, analytic circular arcs, shared endpoints,
complete oriented circle, bounded interior and declared tolerance must match it.
Whole-arc 3D/p-curve coefficient bounds and radius-scaled tilt checks qualify
alignment; no sampled-only geometry test is used. No new faces are rebuilt.

Focused IO passes 1/1 (4.989 s); runtime/IO passes 2/2 (6.51 s), including both
signs, translated source, shifted-cap refusal, budget failure and recomputation
despite reversed stored axial provenance. Cylinder group passes 22/22 (42.648 s).
The earlier full 160/160 suite is separate evidence, not a new run for this query.

Next: match the actual retained cylindrical surface/chart and selected UV region
to the material, then combine tolerance and bidirectional geometry gates before
volume/solid requalification. Document property, display and GUI integration,
partial/multi-sheet and broader Stage 2–4 gates remain open. Stored qualification
flags stay false. See native-cylinder-persistence.md.

## Stored-cylinder reverse seam coverage

Native heads: FreeCAD e9c83b40cb; OCCT ad2abd80bd.

CheckSeamCoverage and FreeCAD check_stored_cylinder_seam_coverage now bound
every zero on the full recipe-cylinder side against the unchanged stored seam.
The pure CheckAxialField cover was extracted from CheckAxialSheet and is reused
by construction; it performs no tracing and returns no boundary topology.
Fresh whole-cylinder derivative cones prove uniqueness. Stored closed spline
angular winding and arc residual/root brackets prove existence at every angle.
Chord/arc and radial endpoint errors are included in the distance budget.

The first test exhausted useful distance precision through an overly conservative
global derivative bound; local derivative cones over each arc/tolerance tube
resolve it without weakening uniqueness or the 0.1 mm distance target. Reports
are immutable and do not promote loaded geometry/volume/solid flags. Partial/
multi-sheet support, retained regions and solid requalification remain open.

Focused IO passes 1/1 (5.708 s); runtime/cylinder/legacy-solid-IO passes 11/11
(26.61 s). Full native regression passes 160/160 (CTest 360.03 s). Python syntax
and all repository whitespace checks pass. Existing build/dependency trees only;
no libraries changed during tests and no functional changes followed the run.
Next: qualify the actual stored retained cap/cylinder regions and their material
orientation against the field, then geometry/volume/solid restoration and live
document/display/GUI integration. See native-cylinder-persistence.md.

## Stored-cylinder revalidation prerequisite: primitive and masked seam proximity

Native heads: FreeCAD fcfe8ef0a8; OCCT a6c34e5668.

CheckPrimitiveSeam and FreeCAD check_stored_cylinder_primitive_seam now bound
the actual stored implicit-wire spline's distance to unrestricted gyroid zeros.
Whole-segment residual interpolation and Hessian directional-derivative cones
produce model-length bounds; source world scale/placement and interval budgets
are retained. This is not a dimensionless residual threshold or sample-only
claim. No topology is retraced and no stored certificate is promoted.

CheckMaskedSeam / check_stored_cylinder_masked_seam additionally use bounded
radial contraction and axial zero brackets whose four endpoint witnesses lie
inside the stored convex cylinder/source mask. The contraction consumes distance
budget. This establishes a directed masked-seam bound, not reverse coverage or
retained material-region correspondence.

Focused IO passes 1/1 (6.440 s); runtime/IO passes 2/2 (6.98 s), including both
signs, translated source, interior seam corruption and budget failure. Existing
builds only. The previous 22/22 cylinder and 160/160 full-suite results are
separate earlier evidence, not new complete regressions for this added query.

Next: establish reverse coverage and retained-region correspondence for the
unchanged graph. Directed seam proximity alone is insufficient to qualify
stored geometry or solids.
Document properties, trim-conforming display, GUI and broader Stage 2–4 gates
remain open. See native-cylinder-persistence.md for the exact contract.

## Stored-cylinder revalidation prerequisite: curve coherence

Native heads: FreeCAD 6c8d49d2e0; OCCT 7f3a41e62d.

CheckCurveCoherence now audits the actual restored 3D edges against conventional
face p-curves over whole parameter intervals. Knot partitions, speed/curvature
bounds and adaptive interpolation remainders enforce the stored edge tolerance.
The initial speed-only approach exhausted its budget on tight valid circles;
the curvature bound resolves them without raising tolerances or default budgets.
FreeCAD check_stored_cylinder_curves exposes an immutable report; no geometry,
volume or solid flag is promoted. Tests include an altered interior spline pole
and explicit budget failure. See native-cylinder-persistence.md.

Focused IO passes 1/1 (6.834 s); cylinder core passes 22/22 (42.037 s);
fork runtime/IO passes 2/2 (7.75 s).
The previous full 160-case regression belongs to milestone 99; it is not new
evidence for this added query. Next: field seam and retained-region correspondence
for the unchanged stored graph, followed by volume requalification and live GUI
integration. This prerequisite alone does not complete restored solid validity.

## Milestone 99: explicit shared cylinder graph persistence

Native heads: FreeCAD 8d5c619a77; OCCT 5750846b64.

The OCCT cylinder IO schema stores the actual qualified three-face solid graph,
original primitive, finite cylinder mask, analytic/spline curves, p-curves,
orientations, tolerances and shared vertex/edge identities. It does not rebuild
the boundary from a recipe or serialize meshes. Records are bounded to 16 MiB,
math blobs to 64 KiB and individual spline pole counts to 65536. Unknown schemas,
malformed counts, truncation and trailing data reject atomically.

Read checks the restored shared topology and conventional faces but deliberately
drops geometry, volume and solid qualification. Stored numeric bounds are
provenance only. The FreeCAD dumps_cylinder_solid/loads_cylinder_solid API owns
that restored graph through a distinct immutable stored-solid handle; qualified
classification/volume APIs reject it. inspect_stored_cylinder_solid exposes its
unverified state without rebuilding. See native-cylinder-persistence.md.

Evidence: full native regression 160/160 (345.09 s), focused round-trip/refusal
test 1/1 (4.188 s), and all eleven runtime/cylinder/legacy-solid-I/O checks pass.
Built the existing TKTopAlgo/native harness and _ImplicitOCCT/ImplicitScripts;
no dependencies or system libraries changed. Tests finished October 25; results
were rechecked October 29. No runtime process remains active.

Next: revalidate the stored geometry against its mathematical field before
promoting it, then integrate trim-conforming cylinder display and a dedicated
live document geometry property with save/reopen and GUI workflows. General
partial/multi-sheet, Boolean dispatch/history and Stage 2–4 gates remain open.

## Milestone 98: actual native cylinder solids and bounded material volume

Native heads: FreeCAD 1fa3b9f407; OCCT 3ff06caf61.

BoundMaterialVolume integrates the qualified full-disk material with adaptive
disk rectangles, conservative circle area bounds and gradient/Hessian root-height
cones. Only proved sign cones update whole-cell root intervals; global end-disk
signs supply fallback endpoints. Largest-error refinement, explicit rounding
reserves and cell/root/depth limits gate the requested absolute volume width.
This bounds mathematical material volume, not generic BRepGProp properties of
the approximate stored boundary. VolumeSettings callbacks are never retained.

BuildNativeSolid now creates an actual TopoDS_Solid only after the owned topology,
geometry, material reference and bounded positive/partial volume qualify. It
attaches the existing shared closed shell and retains all provenance. Validity
is the explicit implicit-aware contract, not ordinary OCCT consumer support.
No mesh/UV substitution or arbitrary Part Shape is used. Failures publish no
solid but retain source diagnostics; cancellation propagates before construction.

FreeCAD build_cylinder_solid/inspect_cylinder_solid own the actual native solid
and expose native face handles, detached previews, separate validity flags and
volume intervals. bound_cylinder_material_volume and classify_cylinder_material
accept geometry or qualified solid owners; failed solid owners reject.
solid_generic_occt_supported remains false. See native-cylinder-solids.md.

Focused native checks pass 38/38 (36.981 s), including both signs, independent
analytic-root quadrature/complementary volume enclosure, actual solid type and
shared implicit face, translated/reversed frames, source immutability, work
budgets, coarse-volume refusal and late cancellation. Fork runtime/cylinder
solid/geometry/tolerance/shell/faces/sides/caps/sections/solid-I/O pass 10/10
(21.69 s). Full native regression passes 159/159 (CTest 327.90 s). Python syntax
and all repository whitespace checks pass. Built TKTopAlgo, the existing native
harness and _ImplicitOCCT/ImplicitScripts with the existing two-job/one-link
policy. No libraries changed during tests and no functional changes followed the
full run. Native/migration LastTest.log files retain the evidence.

Next: trim-conforming display caches and combined native graph persistence for
the new cylinder representation, then a dedicated live geometry property and GUI
workflows. General partial/multi-sheet correspondence, broader BRep dispatch,
history, placements/links and the complete Stage 2–4 acceptance gates remain
required. This sufficient full-disk solid is not Stage 4 completion.

## Earlier prerequisite after milestone 97: bounded material classification

Native heads: FreeCAD f67d17d16a; OCCT 7984d918cb.

ClassifyMaterial queries an unchanged owned ShellGeometry reference. It uses
finite-cylinder projection, the selected field sign, Lipschitz lower distance
bounds, axial root upper bounds and exact retained-cap witnesses. A local Hessian
cone sharpens the root bound when the global derivative lower bound is too
conservative. The first test found the analytic root UNKNOWN at a 1e-7 request;
the local cone resolves it without changing the requested tolerance or guessing
zeros. Field residuals are never compared directly to a model-length tolerance.

Results are mathematical IN/OUT/ON/UNKNOWN plus boundary-distance bounds and a
separate BoundaryAllowanceSeparated flag. ON requires an upper distance bound
within the query band; IN/OUT require a lower bound outside it and a qualified
side. Overlapping bands remain UNKNOWN. The separation flag only reports
clearance beyond the retained boundary allowance; no generic OCCT solid
classifier is invoked. Source records remain unchanged. FreeCAD
classify_cylinder_material queries geometry_handle without rebuilding geometry.
No solid, volume certificate, persistence or GUI workflow is introduced.

Focused native cylinder/cap checks pass 36/36 (28.205 s), including both material
signs, analytic roots, selected/unselected caps and sides, near-root points,
explicit ambiguity, bad inputs and translated/reversed frames. Fork runtime/
cylinder/solid-I/O checks pass 9/9 (17.54 s). The full 157-case suite was not rerun
for this added query; milestone 97's full 156/156 is separate evidence.

Next: actual solid construction and bounded volume/material qualification for
the supported path, followed by general partial/multi-sheet correspondence,
broader dispatch/history, combined persistence and live GUI workflows. Stage 4
remains active. See the material-query section in native-cylinder-geometry.md.

## Milestone 97: bidirectional native cylinder boundary geometry

Native heads: FreeCAD 0f74bc5b96; OCCT 7b393c5ba2.

CheckShellGeometry now covers the entire finite cylindrical chart, using physical
u=R*angle and v=height. Field Lipschitz cones qualify mathematical in/out cells;
native trim-segment clearance plus stored classification reserves qualify native
in/out cells. Qualified native centers/trim points and strictly selected field
centers/axial root brackets give witnesses for both directed distance bounds.
Each potentially nonempty cell must have both bounds below the request or split.
Depth/cell/segment budgets, root precision guards and cancellation bound work.
The final boundary bound also includes the retained field-wire allowance.

Exact field-sheet/full-cap correspondence is inherited from the full-disk
reference. This is a boundary-set proof under analytic bounds and floating
reserves, not interval arithmetic or general ordinary-OCCT implicit validation.
It does not construct a solid or prove volume/general partial-sheet support.
FreeCAD check_cylinder_shell_geometry/inspect_cylinder_shell_geometry expose
owned references, directed bounds and counters. Only this separate check can
set shell_geometry_validated true; solid_validated stays false. See
native-cylinder-geometry.md for the covering/witness argument and scope.

Focused native checks pass 35/35 (24.846 s), including both signs, 0.1/0.05
requests, analytic gyroid root comparisons with native face distance, reversed
axes, translation, partial refusal, depth/cell/segment budgets and late
cancellation. Fork runtime/cylinder geometry/tolerance/shell/faces/sides/caps/
sections/solid-I/O pass 9/9 (18.00 s). Full native regression passes 156/156
(CTest 331.56 s). Python syntax and repository whitespace checks pass. Built
TKTopAlgo, the existing native harness and _ImplicitOCCT/ImplicitScripts with
the existing two-job/one-link policy. No libraries changed during tests and no
functional changes followed the full run. Native/migration LastTest.log files
retain the evidence.

Next: actual solid construction and material classification/volume qualification
for the supported path. Partial/multi-sheet correspondence, broader BRep
dispatch/history, combined native graph persistence and live GUI/source/property
workflows remain required by the full Stage 4 goal. Do not infer broad support
or Stage 4 completion from this sufficient full-disk geometry path.

## Earlier prerequisite after milestone 96: explicit trim tolerance and audit

Native heads: FreeCAD 13942e5430; OCCT 30d4313afc.

Generated cylinder-restricted implicit faces now use the requested trim tolerance
on their new TShape, rather than inheriting an unrelated source-face tolerance.
The primitive field/source record stays unchanged. Attachment first requires the
sum of the side contour bound and cylinder/chord deviation allowance to fit the
requested tolerance; separate bounds each fitting are insufficient.

CheckShellTolerance retains the qualified shell and reports the contour bound,
surface-deviation allowance, their combined field-wire bound and maximum stored
face/edge/vertex tolerances. All must fit the request; the native field-face
tolerance must cover the combined bound, and the side-edge tolerance must cover
the cylinder/chord allowance. This is tolerance coherence, not a global shell
geometry, material-volume or solid certificate. Existing geometry/solid flags
remain false. FreeCAD check_cylinder_shell_tolerance/inspect_cylinder_shell_tolerance
expose owned audit snapshots. No source mutation or system changes are involved.

Focused native checks pass 33/33 (7.857 s), including source tolerances 1e-6 and
0.5 against a 0.05 trim request, both material signs, source record immutability,
failed budgets and late cancellation. Fork runtime/cylinder audits/shell/faces/
sides/caps/sections/solid-I/O pass 8/8 (14.11 s). Full native regression passes
154/154 (CTest 312.89 s). Python syntax and repository whitespace checks pass.
Only TKTopAlgo, the native harness and _ImplicitOCCT/ImplicitScripts were built,
using the existing two-job/one-link policy. No libraries changed during tests
and no functional changes followed the full run. Evidence remains in the native
and migration harness LastTest.log files.
See the tolerance-audit section in native-cylinder-shells.md.

Next: qualify shell geometry in both directions against the mathematical material
boundary, then bounded material classification and volume before promoting an
actual solid. Partial/multi-sheet support, combined graph persistence and live
FreeCAD source/property/GUI workflows remain required by Stage 4.

## Milestone 96: closed native cylinder shell topology

Native heads: FreeCAD 5753999f46; OCCT 1fb67ecaa1.

BuildNativeShell reuses the qualified implicit face, one retained cylinder face
and the material-selected end cap. It checks opposite edge uses, permits a
same-face pair only for a genuine conventional periodic seam, verifies private
wire traversal joins, cyclic connected vertex links, connected face adjacency
and Euler characteristic 2. Only then is an actual TopoDS_Shell marked closed.
The cylinder's doubled seam corner uses do not fit the older box validator's
one-corner-per-vertex contract; the new checker retains distinct endpoint germs
and verifies all joins, rather than weakening that existing contract.

FreeCAD build_cylinder_shell/inspect_cylinder_shell retain immutable shell owners,
native implicit-face handles, detached conventional previews and topology
counters. shell_geometry_validated and solid_validated remain false. No ordinary
Part.Shape conversion, lossy shell serialization or GUI workflow is introduced.
Failures publish no shell/output face list; source face diagnostics can remain.
See native-cylinder-shells.md. Partial/multi-sheet support remains required.

Focused native checks pass 32/32 (6.686 s), including both material signs, both
axis directions, translation, native face membership/counts, partial refusal and
late cancellation. Fork runtime/cylinder shell/faces/sides/caps/sections/solid-I/O
checks pass 7/7 (12.06 s). Full native regression passes 153/153 (CTest 300.11 s).
Python syntax and repository whitespace checks pass; no functional changes
followed the full run. Evidence remains in the existing native/migration CTest
LastTest.log files. Built only TKTopAlgo,
the native harness and _ImplicitOCCT/ImplicitScripts with the existing two-job
policy. No runtime libraries are rebuilt during tests.

Next: qualify shell/solid geometry and material classification/volume before
constructing and promoting an actual solid. Extend partial/multi-sheet region
correspondence, then combined native graph persistence and live FreeCAD
source/property/GUI workflows. Stage 4 remains active.
The next geometry gate must also compare inherited source field-face tolerance,
stored edge tolerances and derived contour/deviation bounds with the requested
trim tolerance. BRep_TImplicitCylinderPatch::Restrict preserves source tolerance;
the source need not have the same tolerance as the cylinder settings. The mask's
Contains predicate is analytic and does not itself enlarge the cylinder by that
tolerance. Do not promote topology closure into a missing approximation proof.

## Milestone 95: qualified full-disk native implicit face attachment

Native heads: FreeCAD eb55bab4ab; OCCT 77fabdd18d.

BuildProjectedRegions projects cap field seams affinely and side seams to exact
circular arcs after angular-monotonicity checks. Shared native vertex identity,
signed orientation/area, separate-cycle clearance, nesting and ordinary planar
face checks qualify reference regions and WireRegions. Proxy validation alone
does not promote mathematical correspondence.

CheckProjectedCorrespondence now qualifies a sufficient full-disk case: one
complete circular side boundary, no cap field contours, one proxy disk, a strict
axial derivative sign everywhere, and independently proved opposite field signs
over both end disks. Lipschitz covers prove existence on every axial fibre;
monotonicity proves uniqueness. This establishes one connected disk-like sheet.
BuildImplicitFaces creates its actual BRep_TImplicitCylinderPatch and attaches
the original shared field wire, with material orientation and effective edge
incidence checked. Placements convert world cylinder settings to field space.
No triangle conversion, fake field UV surface or arbitrary Part Shape is used.

FreeCAD source adds build_cylinder_implicit_faces/inspect_cylinder_implicit_faces:
owned graph handles, immutable native field-face handles, detached side/cap
previews, separate correspondence/attachment flags and solid_validated false.
No document feature, GUI command, shell/solid proof or combined persistence is
introduced. See native-cylinder-implicit-faces.md. Partial/folded/multi-sheet
correspondence remains open; the sufficient full-disk path does not replace the
general curved-operand or Stage 4 requirements.

Focused native cylinder/cap checks pass 31/31 (6.901 s), including both signs,
reversed axes, translation, partial refusal, source immutability and late
cancellation. Full native regression passes 152/152 (307.735 s; CTest 307.83 s).
Built TKTopAlgo/native harness and _ImplicitOCCT/ImplicitScripts with two compile
jobs and one link job. Runtime libraries were not rebuilt during tests.
Fork runtime/cylinder implicit faces/sides/caps/sections/native-solid-I/O pass
6/6 (12.39 s), including immutable native handles, detached preview mutation,
owned inspection, source records and partial-sheet refusal. Python syntax and
all repository whitespace checks pass. No functional native changes followed
the full kernel run. Native/CLI evidence remains in the existing CTest logs.

Next: assemble material shells for the qualified full-disk case and qualify solid
topology/geometry; extend partial/multi-sheet region correspondence as required
for the full goal. Then combined native graph persistence
and live FreeCAD source/property/GUI workflows. Stage 4 remains active.

## Earlier prerequisite after milestone 94: axial sheet qualification

OCCT CheckAxialSheet retains the qualified TrimBoundary and adaptively covers
the finite cylinder with local boxes. A global gyroid Hessian bound, transformed
to model coordinates, bounds axial gradient variation over each box. Only boxes
strictly outside the disk plus a coordinate reserve are skipped. Every remaining
box must prove the same strict axial derivative sign. Longest-axis bisection,
depth/cell budgets and cancellation bound work. Direction and a positive signed
derivative lower bound publish only after the entire cover succeeds; unresolved
queries retain zero direction/bound and a reason. Floating-point reserves remain
an engineering policy, not interval certification.

This establishes at most one zero per axial fibre, not existence, projected trim
correspondence, connectedness, wire/component assignments or implicit-face/solid
validity. Folded or unproved fields refuse this path. General multi-sheet support
and the full curved-operand Stage 4 gates remain required. The next step is a
qualified projected-region/reference construction and assignment of the retained
wires; monotonicity alone must not authorize attaching all wires to one face.
This prerequisite is C++ only; FreeCAD source/API is unchanged.

Built TKTopAlgo and the existing native harness with two jobs. Focused native
cylinder/cap checks pass 29/29 (6.181 s), including reversed axes, translated
source/frame, sampled derivative comparisons with the lower bound, bounded
refusal for a folded fixture, empty-surface uniqueness and late cancellation.
Adjacent fork runtime/cylinder checks pass 4/4 (2.20 s). Whitespace passes.
No libraries changed during tests. The full 150-case suite was not rerun for this
added guard; the earlier milestone 94 full 148/148 result remains separate.

## Earlier prerequisite after milestone 94: shared field-boundary wires

OCCT BuildTrimBoundary now audits all conventional side/cap edge occurrences.
Each field edge must have exactly one use with its stored orientation; every
other conventional edge must have exactly two opposite uses. Unused/duplicate
field edges and unexpected orientations reject. Field edges are then reversed
and assembled into directed closed wires by actual vertex identity, including
chart-seam crossings and cap-only interior loops. EdgeWires maps side edges,
bottom field edges and top field edges to snapshot-local wire indices.
Failure clears wires/maps; source side/cap geometry remains owned separately.
Cancellation propagates rather than returning a partial graph.

Focused native cylinder/cap checks pass 28/28 (6.198 s), including both material
signs, open/winding/full/empty and cap-only interior-loop cases, native edge
identity, opposite incidence, failed-budget cleanup and late cancellation with
unchanged source records. Adjacent fork runtime/cylinder checks pass 4/4 (2.38 s).
Whitespace checks pass. Built only TKTopAlgo and the native harness with two
jobs; no runtime libraries changed during tests. The full 149-case suite has not
been rerun for this added API; milestone 94's prior full 148/148 is separate.

This is incidence qualification, not implicit-face construction or surface
component assignment. The existing box-shell builder requires qualified component
references before attaching wires; the cylinder path must meet the same contract.
Do not put every wire on one cylinder-restricted implicit face merely because the
boundary graph closes. Hidden/disconnected sheets and inner loops need component
and material-region qualification before shell/solid assembly.

Next: establish an interior field-surface/component reference and assign these
wires to it, then construct cylinder-restricted implicit faces with matching
orientation and qualify the assembled shell/solid. A directional-monotonicity
proof can support regular graph-like sheets, but must explicitly refuse
unproved cases and cannot replace the general curved-operand Stage 4 gates.
FreeCAD source/API is unchanged by this boundary-wire increment.

## Milestone 94: retained native cylinder-side regions

Native heads: FreeCAD f02a70b243; OCCT e4d7fef293.

BuildSideBoundary splits regular field contours at logical seam nodes, shares
cap/seam vertices and constructs degree-1 3D/UV edges with a curvature-based
cylinder-deviation allowance. Orientation guards reject uncertain contours.
BuildSides combines those edges with shared cap circles and paired-use axial
seams, distinguishes UV seam occurrences by identity/branch, checks directed
cycles, signed chart areas and bounded nesting, and validates actual cylindrical
faces with BRepCheck_Analyzer. Both signs, open/winding contours and full/empty
material cases are tested. These are retained conventional cylinder faces, not
mesh triangles or completed implicit solids.

FreeCAD build_cylinder_sides/inspect_cylinder_sides own the combined source graph
and expose detached Part.Face previews plus separate cap/side validity, contour,
surface-deviation and adaptive-area reports. Source fields/settings stay owned;
preview mutation does not change the graph. No document feature/GUI workflow
or native graph persistence is introduced. See native-cylinder-sides.md.

Focused native checks pass 27/27 (6.144 s), including source immutability,
translation, late cancellation, curve/surface agreement, complementary areas and
sampled material classification. Four adjacent FreeCAD runtime/cylinder tests
pass (2.86 s). Full native regression passes 148/148 (317.114 s; CTest 317.17 s).
A later report-only addition exposes side_contour_bound. After that rebuild,
runtime/cylinder sides/caps/sections/native-solid-I/O pass 5/5 (12.50 s).
Python syntax and repository whitespace checks pass. Exploratory
near-maximum fixtures were refused by cap-skeleton qualification and do not prove
isolated side-loop/hole acceptance. Tolerances/proof policies remain unchanged.

Next: attach the side/cap field seams to actual cylinder-restricted implicit
faces, identify material components and qualify shell/solid topology/geometry,
then native graph persistence and live FreeCAD source/property/GUI workflows.
General BRep dispatch/history and the full Stage 4 acceptance gates remain open.

## Earlier prerequisites after milestone 93: periodic side-chart provenance

`BRep_ImplicitCylinderSection::TracePeriodicGraph` now retains source-cell arcs,
logical node IDs, canonical angle/height coordinates, continuous per-curve UV
coordinates and native vertices only at cap/seam roots. First/last chart cells
use the same seam node; cap endpoints retain the identical boundary-root vertex.
Unresolved passes publish no graph. Existing section/boundary result layouts
remain unchanged. This is a prerequisite for side topology, not side faces.

Built TKBRep and the native harness with two jobs. Focused cylinder/cap tests
pass 21/21 (1.377 s), including UV reconstruction, source-cell seam incidence,
closed-loop winding, cap handle identity and failed-budget cleanup. Full native
regression has not been rerun for this increment. FreeCAD source is unchanged.
`BuildChartSkeleton` now also splits wraparound ring arcs at the selected seam,
retains one shared seam-end vertex per cap and constructs analytic axial seam
edges between the logical crossing vertices. Each axial edge carries paired
p-curves on the retained cylindrical surface; all ring arcs carry matching
cylinder p-curves. Root-free rings keep their two-half-circle topology. Existing
BuildBoundaryRings/BuildCapSkeleton behavior is unchanged. Focused cylinder/cap
checks now pass 22/22, including parameter ranges, circle coverage, endpoint
identity, seam edge chaining and paired-p-curve recognition. Full regression has
not been rerun for that individual increment.
`BuildChartCapSkeleton` now traces the side once and supplies that same boundary
snapshot to disk tracing and seamed circle partitions. `BuildChartCaps` builds
actual planar material regions from those rings, retaining the side chart.
Its cap faces reuse the identical ring edges and root vertices, including the
synthetic chart-seam ends. Existing BuildCaps remains unchanged in behavior.
The focused shared-snapshot case passes (26 ms), covering both material signs,
full/empty caps, complementary areas, native edge/root identity and late budget
failure cleanup. Full native regression passes 144/144 (301.233 s; CTest
301.26 s), before the subsequent seam-order correction described below.
An independent complete-cylinder face probe found the paired seam p-curves had
the wrong branch order for outward traversal: low/high produced an invalid face
with negative area; high/low produced a valid face and the expected positive
area. The order is corrected and a full-face validity/area/classification test
now covers it. This qualifies the root-free reference face, not general retained
material side regions. Post-correction cylinder/cap regression passes 24/24.
The full 145-case suite has not been rerun after that correction. Adjacent fork
runtime/cylinder caps/sections/native-solid-I/O passes 4/4 (11.36 s), before the
correction, which changes only the new chart seam edges. No library rebuild
occurred while runtime regression ran. Native logs remain in the existing CTest
harness directories. FreeCAD source remains unchanged.
Next: assemble retained side regions and
validate those faces before implicit-face/shell/solid integration. An exploratory
face made only from periodic circle wires classified sample points but failed
BRepCheck and gave zero area; do not treat that shortcut as valid side topology.

## Milestone 93: actual planar cylinder cap regions

Native heads: FreeCAD d557e423ac; OCCT 47eb2ecfd6.

TKBRep BuildCapSkeleton now produces one disk/ring snapshot sharing actual
root vertex handles. TKTopAlgo BRepClass3d_ImplicitCylinder::BuildCaps constructs
bounded planar regions for F<=0 or F>=0 using analytic circle edges and
degree-1 B-spline field-contour edges with matching planar p-curves. Gradient/
Hessian orientation guards, directed endpoint incidence, signed-area reserves,
cycle nesting and BRepCheck_Analyzer gate both caps. Interior loops become holes
or positive islands according to material sign. No contour proximity welding,
mesh-face conversion or arbitrary field assignment to Part Shape is used.

The result retains its authoritative field and settings; callback pointers are
not retained. Nesting work is bounded (default 2 million, maximum 10 million
candidate tests) with cooperative cancellation. Failed batches clear faces and
field edges, including after an earlier cap succeeded. Nonfinite area arithmetic
refuses construction. Cap topology and inherited contour bounds remain separate
from cylinder-side topology, shell/solid validation and global geometry proof.

FreeCAD build_cylinder_caps/inspect_cylinder_caps expose immutable owned graph
handles plus independent conventional planar face previews and source/settings
descriptors. Preview mutation does not change the retained graph. Adaptive area
integration is reported separately; initial nonadaptive mass-property assertions
underresolved the many-knot contours, while adaptive integration passes the
unchanged partition tolerance. Preview decoding now specializes Part.Face like
the existing mixed-cap facade, rather than leaving a generic Part.Shape wrapper.

Built TKBRep/TKTopAlgo, native tests and _ImplicitOCCT/ImplicitScripts with two
compile jobs and one link job. No packages/system libraries changed. Focused
native cap checks pass 5/5 (303 ms): positive/negative material classification,
shared root identity, outward orientation, exact complementary area partition,
holes/islands, full/empty cases, budget/critical/nonfinite refusal, placement and
cancellation. Cap/runtime CLI passes 2/2 (0.78 s), covering detached previews,
ownership/inspection, source records, holes, policy types and late budget failure.
Full native regression passes 141/141 (294.85 s; CTest total 294.86 s),
retained in the native harness LastTest.log. Adjacent cap/section/plane/snap/
solid-I/O/runtime compatibility passes 6/6 (20.74 s), retained in migration
LastTest.log. Syntax/whitespace pass; no code changed after the full kernel run.
See native-cylinder-caps.md. Next: cylinder-side retained regions/p-curves,
implicit-face seam attachment/component assembly, then shell/solid qualification,
native graph persistence and live FreeCAD source/property/GUI workflows. Stage 4
remains active; these are real cap faces, not a completed cylinder Boolean.

## Milestone 92: constrained end-cap disk contour graphs

Native heads: FreeCAD e0d029f931; OCCT 4b365ac45e.

Plane TraceGraph now retains accepted regular-cell arcs and shared root vertices
without changing the existing Result layout. TraceEndCapDisks uses those cells
to clip the zero set to both circular end-cap domains, including closed interior
loops with no circle crossings. Circle-root brackets must occupy a unique
Cartesian cell; cut cells require unambiguous inside/outside grid roots and
zero or two boundary crossings. Graph degree/root incidence gates acceptance.
No nearest-point welding or polygon clipping substitutes for the field query.

Detailed plane roots and retained height-circle roots use narrower brackets
for cell attachment (tol/4096 and tol/65536 respectively). Uncertain attachments,
critical contacts and resource failures invalidate the whole disk batch, clear
both cap curves/endpoint IDs and set both cap resolved flags false. An initial
negative fixture resolved correctly through alternative grids; it is retained
as a positive case, with a genuine interior critical contact testing failure.
Circle-grid coincidences and unresolved cells in the full circumscribed square
can still reject admissible disks; this is explicit conservative incompleteness.
Floating reserves remain engineering policies, not interval certification.

FreeCAD trace_gyroid_cylinder_disks returns copied contour/root reports. Check
disk_contours_verified: parent resolved describes the side section only. Native
cap regions, edges/p-curves, cylinder trim faces and solids are not implied.
Primitive records and existing property layouts are unchanged.

Built TKBRep/native tests and _ImplicitOCCT/ImplicitScripts with two compile
jobs and one link job; no packages/system libraries changed. Focused new kernel
checks pass 4/4 (238 ms): analytic x-root coverage on both caps, shared endpoints,
closed interior islands, exact/nonlattice zeros, empty/placed/cancelled queries,
budget refusal and failed-batch invalidation. Initial CLI cylinder/plane/runtime
passes 3/3 (2.17 s); final expanded cylinder/runtime passes 2/2 (1.46 s).
Final cylinder/plane/face-snap/runtime compatibility passes 4/4 (11.82 s),
retained in the migration LastTest.log. Syntax/whitespace pass; no code changed
after the successful full kernel run.
Full native regression passes 136/136 (294.92 s), retained in its LastTest.log.
The harness wall limit is now 600 seconds
because the previous suite reached 417.75 s before these extra plane/disk traces;
no per-operation numerical, topology, cancellation or resource limit changed.
See native-cylinder-sections.md. Next: orient/nest contour and circle-edge cycles,
construct and qualify actual planar cap regions, then cylinder-face trims and
solid/source/property/GUI integration. Full Stage 4 remains active.

## Milestone 91: persistent cylindrical field-domain restriction

Native heads: FreeCAD 36ba9f6d33; OCCT b7f1edda9b.

OCCT BRep_TImplicitCylinderPatch is a distinct field-backed face storage subtype
with an explicit closed radial/height domain in field-local coordinates. Its
conservative full-prism enclosure must fit the computational box. Restrict
preserves both source placements and orientation, refuses existing masks/child
topology and leaves the source unchanged. EmptyCopy retains the subtype/mask.
IsInDomain, local projection bracket guards and ray filtering honor the cylinder.
Scalar evaluation still returns the authoritative equation separately from domain
membership. This is not cap/trim topology or a validated solid.

BRep_ImplicitIO schema 3 stores this known subtype, including frame/radius/height,
field, box, tolerance, both placements and orientation. Primitive schemas 1/2
are unchanged; unsupported subtypes/attached topology still reject, and failed
reads preserve the destination. Sphere tessellation/box builders reject the new
subtype rather than ignore its domain. No fake UV parameterization is supplied.

FreeCAD restrict_cylinder_local exposes an immutable owned face and metadata
flag; dumps/loads preserve the mask. Reconfigured existing OCCT for the new
sources, built TKBRep/native tests and _ImplicitOCCT/ImplicitScripts with two
compile jobs and one link job. No packages/system-library changes.
Focused new restriction checks pass 2/2, including oblique frames, geometry and
instance placements, malformed I/O, EmptyCopy, projection/rays and safe refusal.
Cylinder/plane/native-property CLI/runtime passes 4/4 (5.27 s).
The first full regression hit its 420-second runner timeout without assertion
failure. Exact-base face/query fast paths now skip unrelated patch RTTI checks,
while retaining box/exclusion checks and all derived-mask guards. Final focused
cylinder checks pass 11/11 (1.151 s); expanded cylinder/plane/property/face-snap
CLI/runtime passes 5/5 (19.33 s). Final full native regression passes 132/132
(417.75 s; CTest total 417.77 s) under the unchanged 420-second limit. Its
LastTest.log retains the complete run; migration LastTest.log retains the final
CLI checks. Runner headroom is small on this host. No tolerance, geometry or
resource check was relaxed. Syntax/whitespace pass; no code changed after the
successful full kernel run.
See native-cylinder-sections.md.
Next: disk cut-cell tracing and interior closed contours, root-identity attachment,
cap-region closure and cylindrical face trims, then shell/solid qualification,
source/property persistence and broader Stage 4 consumers. Goal remains active.

## Milestone 90: analytic cylinder boundary-circle edges

Native heads: FreeCAD b76e78c4be; OCCT a63f266983.

OCCT BuildBoundaryRings now constructs actual Geom_Circle edges partitioning
both height circles. It consumes one retained boundary result and reuses its
canonical root vertices; root-free circles use two semicircles with shared
synthetic vertices. Bottom edges have outward reversed cap orientation and top
edges are forward. Field signs are sampled strictly between root brackets;
even root count, alternating signs and strict sign reserves gate the whole batch.
Endpoint neighborhoods retain root uncertainty. Any failure publishes no edges.

FreeCAD build_gyroid_cylinder_rings returns copied edge descriptors and a separate
ring_edges_verified flag, not mutable Part shapes. This qualifies circle
partition/incidence/sign checks only. Cap regions, attached p-curves, cylindrical
trim faces and Boolean solids remain open. Existing types/records/proof flags
are unchanged. See native-cylinder-sections.md.

Built TKBRep/native tests and _ImplicitOCCT/ImplicitScripts with two compile
jobs and one link job. No packages/system libraries changed. Focused cylinder
initially passed 8/8 (1.277 s); the added rigid-placement edge check passes 1/1 (3 ms).
Review found that the ring reserve could exceed the requested tolerance even
when the smaller section reserve passed. The first full run was intentionally
terminated at 201.05 s to add a strict precision/edge-tolerance guard. The final
focused cylinder checks pass 9/9 (1.265 s); CLI/runtime passes 3/3 (2.65 s),
including a resolved-section/refused-ring precision-limit case at translated
coordinates. Tests validate actual circle
edges with BRepCheck_Analyzer, shared endpoint identity, angular coverage,
dense independent sign samples, both root-free signs, orientation, restored
reports and no edges after budget failure. Final full native regression passes
130/130 (348.92 s), retained in the native harness LastTest.log. Migration
LastTest.log retains the final CLI run. Syntax/whitespace pass. No code changed
after the final full kernel run.
Next: planar cap-region contours/closure consuming these ring edges and shared
vertices, then native cylindrical face trims and shell/solid qualification.
Cap builders must detect interior closed contours/holes; circle signs alone do
not classify disk material. Use retained root brackets/IDs for cut-cell attachment,
and add an explicit cylindrical field-domain restriction before exposing clipped faces.
Stage 4 remains incomplete.

## Milestone 89: retained cylinder end-cap root anchors

Native heads: FreeCAD 07509887f9; OCCT d6bab8fc84.

OCCT TracePeriodicBoundary now retains canonical owned TopoDS_Vertex handles
for height-boundary roots, their geometric/angular brackets and 1-based curve
endpoint IDs. Roots are ordered by bottom/top and chart angle. Each root must
have exactly one open endpoint use; closed curves use zero IDs. Unresolved or
inconsistent results publish no anchors. Existing open/periodic section APIs
and Settings/Result layouts are unchanged; the detailed result is a new type.

FreeCAD trace_gyroid_cylinder_boundary returns copied descriptors and an explicit
boundary_incidence_verified flag, without mutable OCCT vertex exposure. Root
identity is snapshot-local; no edit history or persisted naming is implied.
The flag checks root/curve incidence only, not native trim or solid validity.
No end-cap region or cylindrical trim edge is built yet. Future C++ builders can
reuse these exact handles instead of welding independently approximated roots.

Built TKBRep/native tests and _ImplicitOCCT/ImplicitScripts with two compile
jobs and one link job. No packages/system-library changes. Focused cylinder
checks pass 6/6 (1.259 s); cylinder/plane CLI/runtime passes 3/3 (2.66 s).
Tests cover four distinct ordered bottom/top roots, sign brackets, tolerance
containment, exact endpoint identity, handle reuse, closed-loop sentinel IDs,
deterministic restored queries and no anchors after budget failure.
Full kernel regression passes 127/127 (351.19 s; CTest total 351.20 s), retained
in the native harness LastTest.log. Migration LastTest.log retains the CLI run.
Syntax/whitespace pass. No code changed after the full kernel run.
See native-cylinder-sections.md.
Next: planar end-cap regions consuming these anchors and oriented native
cylindrical trims, then shell/solid qualification/persistence. Other unfinished
Stage 2–4 requirements remain active.

## Milestone 88: periodic cylindrical seam identity

Native heads: FreeCAD 1cc5d33791; OCCT 36578fc4e7.

OCCT TracePeriodic now uses a canonical shared seam grid edge/root identity for
one full cylindrical turn. Values and root vertex IDs are generated once and
reused at the opposite chart boundary, without tolerance welding. Seam incidence
is interior; only height boundaries permit open endpoints. Open Trace is unchanged.
The explicit periodic API requires AngleMaximum == AngleMinimum + 2*pi.
Near-zero seam/grid samples remain unresolved; bounded fallback grids retain
identical derivative and physical-diameter acceptance policies.

FreeCAD exposes this as trace_gyroid_cylinder(..., periodic=True), requiring an
actual bool and reporting periodic mode without promoting native topology/solid
proof flags. Existing Settings/Result layouts and primitive records are unchanged.
Rebuilt TKBRep/native tests and _ImplicitOCCT/ImplicitScripts with two compile
jobs and one link job. No packages/system-library changes.

Focused cylinder kernel checks pass 5/5 (1.218 s), including independent analytic
closed-loop coverage, exact closed-point reuse, moved-seam equivalence, height-only
open endpoints, budget refusal and unresolved zero seam samples. Cylinder/plane
CLI/runtime passes 3/3 (2.79 s), including periodic restore and explicit policy
typing. Full native regression passes 126/126 (340.35 s), retained in its native
harness LastTest.log. Migration LastTest.log retains the 3-test CLI run.
Syntax/whitespace checks pass. No code changed after the full kernel run.
See native-cylinder-sections.md.
Next: shared end-cap roots, oriented native cylindrical trims/retained regions,
then shell/solid qualification and persistence. Stage 4 remains incomplete.

## Milestone 87: curved-operand cylindrical section foundation

Native heads: FreeCAD ee0deaec0e; OCCT d7310aa883.

OCCT BRep_ImplicitCylinderSection adds a derivative-bounded query on finite
open cylindrical charts of childless exact gyroids. Arc-length/height charts
include the Lipschitz/radius curvature term in their Hessian bound. Shared-edge
root brackets, regularity cones, cell diameter and graph checks gate resolved
curves. Ambiguous samples, insufficient budgets and tangencies remain unresolved.
Full-turn seams, native trims, end-cap joining and cylinder Boolean solids are
not implemented by this query. The conservative full-cylinder prism enclosure
must fit in the computational field box. See native-cylinder-sections.md.

FreeCAD's matching OCCT 8 binding exposes trace_gyroid_cylinder and shares the
existing plane-section report conversion. Primitive records, existing type
layouts and proof flags are unchanged. No packages/system libraries changed.
Reconfigured the existing OCCT build for new sources, then built TKBRep and
the native tests; FreeCAD rebuilt only _ImplicitOCCT and ImplicitScripts.
Two compile jobs and one link job were used. No libraries rebuilt during tests.

Focused kernel tests pass 3/3 (0.48 s), including analytic height-root coverage,
cylinder membership, root brackets, rigid placement, empty/budget/cancellation
and invalid/full-turn refusal. New-cylinder/plane-section CLI/runtime checks
pass 3/3 (2.74 s), including deterministic restored queries and source immutability.
Adjacent face-snap/native-solid-I/O CLI/runtime passes 3/3 (22.93 s).
Full native regression passes 124/124 (339.05 s; CTest total 339.07 s), retained
in the native harness LastTest.log. Syntax and whitespace checks pass. Migration
LastTest.log retains the adjacent 3-test run. No production code changed after
the successful full kernel run.
The next curved gates are periodic chart seams, shared end-cap roots and native
trim/region construction, then shell/solid validation and persistence. Stage 4
remains active with the other unfinished acceptance requirements unchanged.

## Milestone 86: default toolbar native-solid gyroid creation

Native heads: FreeCAD e6593326ac; OCCT unchanged at 5a93e48c61.

The Implicit Modelling toolbar now opens Implicit_NativeGyroid. Its default
10 mm primitive/domain recipe is staged privately through the boundary/solid
worker pipeline; no field, box, boundary or solid exists until both jobs succeed.
Publication creates all four linked owners and visibility in one transaction.
The original Implicit_Gyroid field-display command remains in the menu for
compatibility. The new task exposes dimensions, period, level and build limits
inside a scrollable panel. Independent geometry depth defaults to 18, retaining
topology depth 16 and the 0.1 mm tolerance; inherited depth 16 exhausted cap
geometry on the 20 mm resize fixture, as previously documented for that scale.

Clipping box dimensions reference the editable field dimensions. Ordinary
background source-to-solid recompute handles dimension changes. Persistence
retains native geometry with unverified restored proof flags. The initial GUI
fixture used a nonexistent getExpression accessor; it now checks ExpressionEngine.
The completed focused create/resize/persistence check passes 3/3 (37.31 s),
gyroid GUI 34.98 s. Expanded gyroid/Part/default-creation GUI/runtime passes 5/5
(79.85 s), including gyroid 40.63 s and Part 17.69 s. The gyroid test disables
synchronous native construction, injects publication failure, verifies all-four
rollback/retry and document-close cancellation. Syntax and whitespace pass.
Only ImplicitScripts was rebuilt; no OCCT or packages changed.
Retained field-display GUI/runtime compatibility passes 3/3 (13.66 s),
GUI 11.36 s; the migration LastTest.log retains that final run.
See [workbench guide](implicit-modelling-workbench.md). Stage 4 remains open:
curved operands, native-solid Boolean composition, surviving history and broader
consumer contracts remain required. Native solid validation and transient display
qualification stay separate.

## Milestone 85: standard Part command dispatch

Native heads: FreeCAD 8a24532906; OCCT unchanged at 5a93e48c61.

Part Common and box-first Cut now recognize native representations before
generic shape access. Supported primitive gyroid/box selections open a native
task with the 0.1 mm default. Boundary and solid jobs precede any document
publication; success creates the live source/validated solid and operand
visibility in one undo transaction. Failures remain repairable and cancellable.
Reverse Cut, Fuse, native solid/boundary composition and subelements explicitly
reject. Ordinary B-Reps retain stock dispatch. OCCT is unchanged.

Rebuilt PartGui and ImplicitScripts with two compile jobs and one link job.
Real command GUI tests cover cancellation, atomic Common undo/redo, Cut resource
failure/retry, unsupported representations/order and conventional B-Rep Common.
Focused Part GUI/runtime passes 3/3 (12.31 s). Expanded Part/creation-default/
solid-creation GUI/runtime passes 5/5 (80.08 s), including Part 17.45 s,
creation defaults 18.30 s and solid creation 41.96 s. Syntax and whitespace
checks pass (CRLF-aware for upstream C++). No packages or OCCT libraries changed.
See [native Part Booleans](native-part-booleans.md). Stage 4 is still incomplete;
broader operands/history/consumers remain required. The default Add Gyroid still
creates the compiled field display rather than this native solid workflow.

## Milestone 84: restricted mathematical-face Near snapping

Native heads: FreeCAD cdeb08482e; OCCT 5a93e48c61.

OCCT BRep_ImplicitTool::Project now solves a bounded local normal-foot problem
without a UV surface. It reports convergence/reason, stationarity, distance,
iterations and a nearby geometric sign bracket. Numerical tangent second
variation, line-search distance and coordinate/iteration guards reject unresolved
candidates. Domain endpoints plus segment/exclusion-box tests require the whole
bracket to respect explicit mathematical restrictions. This is an engineering
numerical local query, not global nearestness, interval certification or support
for arbitrary added trim wires. Existing record/type layouts are unchanged.

FreeCAD snapNative Near now handles native implicit faces through that query and
planar caps through conventional trim classification/closest boundary edges.
Cap holes are respected; implicit faces never enter UV-based classifiers or Part
conversion. Work is local and selected parent/link placement is applied once.
Failed projection yields no candidate; endpoint/midpoint face modes remain empty.
Neither query mutates model geometry, seam/solid validation or display proof.
The native mixed/component domains and exclusions remain authoritative, and
the default 0.1 mm coordinate policy is separate from those proof flags.

Rebuilt active OCCT 8 TKBRep/native-face tests and FreeCAD Implicit/Draft with two
compile jobs and one link job. No packages/system-library changes; the legacy
OCCT 7 build is untouched. New API requires matching rebuilt consumer/library.
The initial bracket width was narrowed to leave rounding room under the existing
strict width acceptance; no tolerance check was relaxed. The first FreeCAD
negative fixture used the older translated demo where origin is a valid interior
surface point; it now explicitly builds the default clipped cube for corner
exclusion checks. See native-face-snapping.md for architecture and reproduction.

Focused new kernel projection tests pass 2/2. Full native suite passes 121/121
(333.05 s; CTest total 333.06 s), retaining the full run in its LastTest.log.
Initial face/edge CLI passes edge 3.22 s; default face CLI/runtime passes 2/2
(4.84 s). Expanded face/hole/Draft GUI/runtime checks pass 5/5 (28.76 s), face
CLI 4.56 s, actual face GUI 10.44 s and prior Draft Line GUI 10.85 s. Independent
cap distance queries verify outside/holed-cap results. GUI face hits with nonzero
field residual must return a distinct field point and the passive snap marker,
without Part conversion. Final both-operation face/edge/element/link/placed
classification CLI/runtime checks pass 6/6 (19.16 s), face CLI 11.38 s. Final solid
display/face-snap/Draft-Line GUI/runtime checks pass 5/5 (50.71 s), including solid
GUI 27.49 s, face GUI 10.10 s and Line GUI 10.29 s; migration LastTest.log retains
that run. No production code changed after the full kernel run. Syntax and
whitespace pass. Harness registers 88 tests with GUI/Draft enabled; affected
subsets were run, not the entire migration suite.

Next: general constrained/boundary face snapping, intersection snaps and surviving
references, along with the everyday native-solid primitive workflow, curved
mixed operands and broader consumers. Stage 4 remains incomplete.

## Milestone 83: native topology snapping in Draft

Native heads: FreeCAD 1e311ea6c5; OCCT unchanged at a3f13c69ea.

Implicit.snapNative accepts owned NativeElementGeometry picks and returns actual
stored vertex endpoints, finite-edge arc-length midpoints or clamped nearest
points on their degree-1 B-spline segments. It works locally, applies the rigid
path placement once and rejects nonfinite inputs/coordinate precision exhaustion.
Whole-region queries and unknown modes reject; unsupported faces return no
candidates. It never meshes or performs untrimmed implicit-face projection and
does not change seam, solid or display proof flags. The 0.1 mm coordinate guard
is separate from stored seam approximation/provenance.

Draft's snapper detects native geometry properties before Part.getShape, resolves
the selected complete parent/link path and uses the existing mode switches,
winner selection, markers, constraints and optional WorkingPlane projection.
Working-plane auto-alignment now skips non-Part selected geometry safely.
Native snapshots remain immutable; Draft Line consumes point coordinates rather
than assigning an arbitrary field/implicit face into a Part Shape.

The active OCCT 8 build now enables Draft, TechDraw, PartDesign, Spreadsheet and
Import. Sketcher was enabled but its GUI was not built. The real Draft workbench
required both SketcherGui and TechDrawGui, plus FreeCAD's bundled lazy_loader.
Draft's target now depends on those GUI libraries, DraftUtils and lazy_loader
(GUI dependencies conditional on BUILD_GUI), so minimal target builds include
its runtime requirements. No packages/system libraries changed. All builds used
two compile jobs and the existing one-link-job pools. See draft-native-snapping.md
for flags and commands. No OCCT source/header/ABI changes in this increment.

Initial native CLI passed (3.19 s). GUI activation first exposed the missing
bundled loader, then the unbuilt SketcherGui; neither was a snap geometry failure.
After building those existing targets, actual Draft GUI/runtime checks pass 3/3
(13.58 s), GUI 10.64 s: viewport picks/markers, no Part conversion for native
snaps, working-plane modes, unavailable/unsupported geometry, Draft Line creation,
stale mapped-reference rejection, conventional Part dispatch and save/reopen.
The native CLI also covers bounded projections, straight/curved midpoints,
read-only proof/revision state, rigid parent/link placement, precision and retained
snapshot lifetime.

Adjacent native snap/element/link/placed-classification and solid-display/feedback
GUI/runtime compatibility passes 8/8 (59.13 s), including native snap 3.16 s,
solid GUI 27.00 s and feedback GUI 23.04 s. Its CTest LastTest.log retains that
final run. Syntax/whitespace pass. No geometry or snap code changed after the
successful Draft GUI test.

The harness adds the CLI test and an optional Draft GUI test
through IMPLICIT_MIGRATION_DRAFT_TESTS=ON; it now registers 86 tests with GUI/Draft
enabled. Relevant subsets were run, not the entire migration suite.

Next: native implicit-face/intersection snapping and edit-surviving topology
references, alongside the everyday native-solid primitive workflow, curved mixed
operands and remaining representation-aware consumers. Stage 4 remains incomplete.

## Milestone 82: live input instances and exact corner persistence

Native heads: FreeCAD 424e3d75fa; OCCT a3f13c69ea.

ImplicitOperands resolves same-document whole links/nested aliases with the
effective FreeCAD instance matrix. Gyroids must remain effectively identity
placed; linked/container boxes currently require pure translations. Template
parent containers are excluded from instance paths, while operand parent groups
are composed. Native root box qualification and the 0.1 mm policy are unchanged.
Field/Box now use global-scope links; legacy dynamic local links migrate with
values/expressions retained. Instance fingerprints add resolved targets/matrices
with signed-zero normalization; legacy direct-root fingerprints remain compatible.

Box snapshots use Part.Shape(shape), a shallow mutable wrapper, then replace its
location. Document Shape properties are immutable; do not set their Placement or
call the flattening Shape.copy(). The container observer touches affected sources
on parent Placement/Group edits without writing geometry/status properties or
adding dependency cycles. Shared source editors reach primitive targets and
invalidate all same-document aliases/peers; alias wrappers are recomputed before
task-owned boundary jobs. Creation, ordinary recompute and task guards use the
same resolved recipe. Curved operands, arbitrary transforms/arrays/subelements
and cross-document input editing remain outside this increment.

The instance solid test exposed a kernel persistence bug: TopTools_ShapeSet's
15-digit per-shape output rounded exact decimal corner vertices, breaking native
record membership validation during private cloning. BRepClass3d_ImplicitBoxIO
now uses a private ShapeSet subclass at max_digits10 for per-shape geometry.
The standard payload format, generic OCCT persistence and exact validation are
unchanged. Failed record errors identify their invariant. No extension ABI or
general OCCT headers changed; TKTopAlgo and the native-face tests were rebuilt.
Only ImplicitScripts was rebuilt for FreeCAD. No packages/system libraries changed.

Full kernel 119/119 passes (345.52 s), including both-operation exact vertex
round-trip and solid construction. Headless source-parameter/mixed/solid codec/
live compatibility passes 5/5 (52.47 s). Initial instance CLI/runtime passes 2/2
(9.92 s); GUI/runtime passes 3/3 (34.56 s), GUI 31.78 s. Broader GUI checks also
pass: instances 32.67 s, feedback 25.61 s, creation defaults 19.85 s, staged task
sources 74.35 s and source parameters 35.42 s. That combined run had one fixture
failure: the external-document negative test attempted to link an unsaved target;
the fixture now saves both documents before testing rejection. Final instance
CLI/GUI/runtime checks pass 4/4 (43.03 s), CLI 10.16 s and GUI 30.62 s, including
legacy scope migration, cross-document rejection and clean same-math link
retargeting rejection before publication. Syntax/whitespace pass. The separate
native and migration CTest LastTest.log files retain the full kernel and final
4-test runs, respectively. The harness registers 84
tests; affected subsets were run. See native-input-instances.md for the contract.

Next: extend everyday native-solid primitive workflow and remaining placement/
source families against acceptance gates; implement curved mixed operands,
snapping/history and representation-aware consumers. Stage 4 remains incomplete.

## Milestone 81: live tree/property build feedback

Native heads: FreeCAD 0f618a1b8a; OCCT unchanged at 0a22e0c5cc.

Mixed sources and native solids now expose BuildFeedback in the View tab and
state badges in the tree. Pending, Failed, Changed, Empty, Unverified and Ready
derive from live jobs, inputs and separate seam/solid proof flags. Display
qualification remains independent. Failed geometry remains inspectable in the
property panel; whole-object selection survives refresh, while snapshot-local
subelement selections (including links) are still cleared.

Implicit::PropertyBuildFeedback is a small App property subclass that exposes a
live Python value through the normal editor. Plain PropertyPythonObject has no
normal editor. ImplicitBuildFeedback calculates text on read and uses a weak
view registry/timer for icon notifications and repaint. Polling writes no model
or view status properties, so it does not journal status changes into unrelated
transactions. The transient property is reconstructed on attach. Failed objects
must not be confused with deleted objects through isValid().

The native Implicit/ImplicitScripts targets were rebuilt with two compile jobs
and one link job. Initial GUI/runtime checks pass 3/3 (26.47 s). Expanded feedback,
mixed-box/surface, native-solid selection and creation-default GUI compatibility
passes 7/7 (121.42 s), including feedback 42.68 s. These checks cover actual
property-model text, actual tree icon changes, source/solid failure/repair,
unverified restore, foreign-transaction repaint, undo, selection preservation and
stale subelement removal. Final GUI/runtime checks pass 3/3 (29.20 s), including
feedback 26.02 s, actual reselected property text after undo and empty watcher/
stopped timer after document closure. Syntax/whitespace checks pass. The migration
CTest LastTest.log retains this final run. The harness registers 82 tests; affected subsets were
run. No OCCT or dependency changes. See native-build-feedback.md for the contract.

Next: qualify broader live input/link/container families and implement the
everyday native-solid primitive workflow against its acceptance gates. Curved
operands, snapping/history and representation-aware consumers remain required;
Stage 4 is not complete.

## Milestone 80: GUI creation defaults and cancellable boundary creation

Native heads: FreeCAD c19c28f57e; OCCT unchanged at 0a22e0c5cc.

Mixed Common/BoxMinusField GUI selection now opens ImplicitMixedCreate's staged
boundary task. It builds from private field/B-Rep snapshots without a provisional
document owner/transaction, guards live inputs/settings/foreign transactions and
publishes a completed source atomically. New GUI sources default to regularized
corner handling, 0.1 mm and BackgroundRecompute=true; operand visibility is changed
only after success in the creation transaction. GUI solid creation now also sets
BackgroundRecompute=true on the new owner, without changing existing source flags.
Console factories and restored false flags retain their existing policy.

Initial default-workflow GUI/runtime passes 3/3 (21.09 s), including cancel,
strict-policy failure/repair, actual command routing, transient box meshing,
visibility undo/redo, default source-to-solid edits and save/reopen with unverified
topology. Existing factory/publication/solid creation compatibility passes 6/6
(62.06 s), GUI solid creation 40.57 s. Final expanded stale-control, dirty-operand
and foreign-transaction GUI/runtime checks pass 3/3 (20.27 s), GUI 17.91 s. The
migration CTest LastTest.log retains that final run. Syntax and whitespace pass.
The harness registers 81 tests; affected subsets were run here. Only Python/scripts/tests/docs
change; no OCCT/dependency changes. Basic Add Gyroid remains the compiled field
primitive; these defaults apply to native boundary/solid GUI commands.

Next: expose pending/failure state clearly in the tree/property workflow, qualify
broader live input families and switch everyday native-solid primitive commands
only when their acceptance gates pass. Curved mixed operands, snapping/history
and representation-aware consumers remain necessary for Stage 4.

## Milestone 79: staged source preparation in task previews

Native heads: FreeCAD 3752147ec5; OCCT unchanged at 0a22e0c5cc.

ImplicitTaskBatch.PreparedSolidBatch now stages all changed source boundaries
through SourceBatch before acquiring solid builds. It retains the task-owned
transaction, freezes target settings, suppresses ordinary recompute through
pending tokens and publishes sources only after the whole source group succeeds.
Both source and native-solid task Apply/OK use it without changing persisted
BackgroundRecompute flags. Direct Python apply() stays synchronous through an
explicit execute override. Queue-full starts retry from the event loop.

The first task test exposed rollback-order fingerprint clearing; MixedBoundary
now preserves journaled fingerprints during Document.Transacting. Metadata checks
still validate unchanged restored geometry. The new task GUI/runtime test then
passes 3/3 (42.79 s), including two-source atomic preparation, no synchronous
fallback, cancellation, changed controls, source-panel OK/repair and closure.
An earlier existing solid-batch GUI passed (51.46 s). Added source preflight
cleanup and broader compatibility passed source-parameter CLI (20.61 s), solid
editor GUI (104.09 s), shared-source parameter GUI (32.53 s) and ordinary source
pipeline GUI (117.44 s). The mixed-editor helper needed asynchronous waiting.
Its strict translated-box fixture then exposed Part.Shape.copy() coordinate
rounding. Requests now store the original B-Rep string and submit it with
start_boundary_job_from_record; exact metadata/tolerance guards remain unchanged.
The final boundary/editor/property GUI/runtime run passes 6/6 (198.18 s): strict
mixed editor 5.52 s, boundary jobs 5.02 s, expanded task GUI 67.71 s and ordinary
source GUI 117.73 s. The task test also covers later-source failure, lost/foreign
transactions and ordinary-job takeover. Final task/shared-batch checks pass 4/4
(120.40 s), including task GUI 68.84 s and shared-batch GUI 49.27 s, after tightening
the later-source-failure assertion. The migration CTest LastTest.log retains
that final run. No production code changed after the 6/6 run. Syntax and whitespace
checks pass. The harness registers 80 tests; affected subsets were run here.
Only native-module Python/scripts/tests/docs change; OCCT remains 0a22e0c5cc.

Next: qualify default background property scheduling, task/tree failure-state
exposure and broader live sources before switching everyday native-solid commands.
Curved mixed operands, snapping/history and representation-aware consumers remain
required for Stage 4. See native-task-sources.md for the staged task contract.

## Milestone 78: GUI background source-to-solid recompute

Native heads: FreeCAD b2bd0f7844; OCCT unchanged at 0a22e0c5cc.

MixedBoundary has a persisted BackgroundRecompute opt-in (default false).
ImplicitMixedRecompute uses private boundary jobs, live recipe/operand/owner
guards and the existing transaction/lifecycle coordinator. Source publication
triggers document recompute; opted-in native solids wait without errors while
the source is pending, then build on the worker. Same-document native consumers
are invalidated immediately, including direct source rebuilds; source failure
propagates an explicit unavailable status. Serialization, planar cap display and
downstream document work still run on the GUI thread.

Initial actual source/solid pipeline GUI/runtime tests pass 3/3 (108.61 s),
including heartbeat, coalescing, undo/redo/abort, foreign transactions, dirty
inputs, failure repair, save/reopen, deletion and close. Follow-up qualification
adds direct rebuild invalidation and failure-status checks: targeted GUI/runtime/
headless/property/task compatibility passes 6/6 (226.73 s), including source
pipeline 115.15 s, solid recompute 75.35 s and source-parameter task 32.75 s.
Final unsupported-definition, direct repair and partial-publication cleanup
GUI/runtime checks pass 3/3 (143.43 s), GUI 141.21 s. The migration CTest
LastTest.log retains that final run. Syntax/whitespace checks pass. The harness
registers 79 tests; only affected subsets were run for this Python increment.
Only Python/native-module
scripts and harness/docs change; OCCT remains 0a22e0c5cc, no dependencies changed.
Task-panel source coordination/default exposure remain subsequent work.

Next: stage source jobs inside the task's owned preview transaction before solid
batches, with cancel/control-change/accept handling. Then qualify default
asynchronous exposure and broader live sources. Curved mixed operands,
snapping/history and representation-aware consumers remain Stage 2–4 gates.

## Milestone 77: native boundary-preparation worker backend

Native heads: FreeCAD fc6b5e53c1; OCCT 0a22e0c5cc.

The shared native executor now accepts primitive-field and conventional box B-Rep
records, prepares the mixed boundary privately and returns exact input descriptors.
start_boundary_job/take_boundary_job expose it through ImplicitOCCT. Kind guards
protect all three take APIs. Caller-thread serialization/report conversion remain;
no document/Python/Part object enters the worker. Expanded boundary/shutdown/
runtime checks pass 3/3 (5.91 s), including both operations under both policies,
same-snapshot synchronous equality, mid-trace cancellation/retry, private source
lifetime, mixed queue/failure guards and invalid settings.

OCCT plane-section Settings now has an optional WorkCheckpoint function pointer,
invoked during sampling/edge work/curve assembly. Mixed Build connects it to the
existing thread-local work control and adds edge-root/side checkpoints. This
extends the native extension ABI: rebuild direct section consumers with matching
headers/libraries. Active OCCT 8 TKBRep/TKTopAlgo, FreeCAD _ImplicitOCCT/scripts and
native-face-tests/worker have been rebuilt; the OCCT 7 legacy build was untouched.
No packages/system libraries changed. Primitive I/O now preserves explicitly
empty locations for exact identity input coefficients; strict private snapshots
therefore remain supported. A 1e-15 translation is not snapped. Final full kernel
118/118 passes (321.56 s), including exact retry curves and identity/near-identity
persistence. Its CTest LastTest.log retains that result. No geometry/tolerance
policy was relaxed. Final existing worker/shutdown/runtime/checked-display GUI
compatibility passes 7/7 (186.74 s), including actual GUI restore 100.46 s.
The migration CTest LastTest.log retains that final run. Syntax/whitespace checks
pass. The harness registers 78 tests; the affected subsets were run here.

Next: connect source recompute/task preparation to these jobs, guard live recipe/
transaction descriptors, coordinate downstream solid builds and qualify GUI
cancellation/restore. The current GUI source paths are still synchronous.
Default exposure, curved operands, snapping/history and consumers remain open.
See native-boundary-jobs.md for API, ABI and exact build/reproduction commands.

## Milestone 76: opt-in GUI property recompute

Native heads: FreeCAD 68a34c34e9; OCCT unchanged at ad3549601d.

NativeSolid now has a persisted BackgroundRecompute setting, default false for
compatibility. When enabled in the GUI, changed build inputs queue private native
solid work, clear unavailable geometry/proof and return from recompute. A new
ImplicitSolidRecompute coordinator coalesces requests, checks source/settings/
owner identity and defers all document writes during unrelated transactions.
Undo/redo/abort/deletion/document-close observers discard pending work. Successful
publication triggers downstream recompute on the GUI thread. Source tracing is
still synchronous. The expanded GUI test passes (81.18 s): heartbeat, coalescing,
undo/redo and pending undo, unrelated-transaction deferral, dirty-source rejection,
resource failure/repair, partial publication failure, save/reopen with unverified
topology, deletion and document closure. Initial focused GUI/runtime run passed
3/3 (51.50 s); expanded GUI/runtime/editor/batch compatibility passes 5/5
(301.68 s), including editor 129.08 s and shared batch 87.71 s. Additional
abort/expression-consumer and headless compatibility checks pass 4/4 (173.25 s),
including GUI 136.70 s and headless 28.78 s. A final guard rejects direct rebuilds
of stale mixed recipes before reusing a cached solid or queuing work; final
focused GUI/runtime checks pass 3/3 (130.25 s), GUI 124.49 s. CTest LastTest.log
retains that final run. Syntax and whitespace checks pass. The harness now
registers 76 tests; this increment ran the relevant subsets, not the full suite.
Broader/default asynchronous recompute remains pending. No OCCT/dependency changes.

Next: qualify task/ordinary-recompute handoff and broader input families before
default asynchronous exposure; move source tracing/preparation to worker jobs.
Default native-solid creation, curved mixed operands, snapping/history and
representation-aware consumer integration remain required for Stage 4.

## Milestone 75: background unchecked display regeneration

Native heads: FreeCAD b6dcd15aa4; OCCT unchanged at ad3549601d.

NativeSolidView now schedules both checked and unchecked cache misses through
ImplicitSurfaceJobs. The job descriptor carries the requested checked mode;
generation/key checks reject mode-switch results. Reusing a cached/initial
candidate cancels superseded work. Default initial buffers remain immediately
available. Result conversion/component assignment/Coin upload still run on the
GUI thread. Ordinary solid recompute and source preparation remain synchronous.
Focused live-solid, async-display, creation/restore and shared-batch GUI tests plus
runtime fixtures pass 6/6 (244.72 s). Earlier on this same implementation: corner
28.39 s, checked display 102.16 s, editor 108.41 s and source parameters 36.20 s
pass. The first run exposed two test assumptions: immediate command creation
and no initial pending views. Both are fixed; mode-switch/coalescing tests forbid
both synchronous extraction APIs. Syntax and whitespace checks pass. CTest
LastTest.log retains the final focused run. No OCCT or dependency changes.

Next: schedule ordinary solid recompute/source preparation, qualify broader live
inputs and expose the default native-solid workflow. Curved mixed operands,
snapping/history and representation-aware consumers remain Stage 2–4 gates.

## Milestone 74: asynchronous checked-display view

Native heads: FreeCAD 0ec74de0ee; OCCT unchanged at ad3549601d.

ImplicitSurfaceJobs adds a GUI-thread scheduler with weak view
references, coalesced requests, generation/digest/settings/source guards and
deletion/document-close cancellation. It submits at most one display job at a
time, reserves queue capacity for solid work and yields to pending solid batches.
NativeSolidView now routes CheckSurfaceDisplay through this scheduler and retains
the existing component-assignment/atomic-upload gate. Restored or manually set
DisplayProximityChecked cannot become true without a current uploaded cache.
Unchecked extraction still follows the existing synchronous/initial-cache path.
The new async_checked_display_gui_smoke.py qualifies heartbeat, request coalescing,
stale generations, solid-job priority, changed inputs, hiding, deletion and
document closure. Final async-display/editor GUI and runtime fixtures pass 4/4
(231.97 s); the existing checked-display GUI separately passes (103.29 s).
An initial test-only cleanup failure was fixed before that final run. Python
syntax and whitespace checks pass. CTest LastTest.log retains the final run.

The installed Qt/PySide/Shiboken stack changed to 6.12.0; old GUI binaries required
missing libpyside6/libshiboken6 6.11 SONAMEs. Existing build configuration was
refreshed and FreeCADMain/PartGui/ImplicitGui were rebuilt with two compile jobs
and the existing single-link pool. PythonWrapper.cpp was touched only to force
compilation against current Shiboken headers; source content/CRLF is unchanged.
Runtime and GUI library audits pass with Qt/PySide/Shiboken 6.12.0 and the
existing bundled Coin/Pivy and OCCT 8 libraries. No packages or system libraries
were modified; the legacy OCCT 7 build was left alone.

Result conversion, component assignment and Coin upload remain on the GUI thread.
Next: schedule unchecked fallback extraction and ordinary property recompute/source
tracing; qualify broader sources and default native-solid exposure. Curved mixed
Booleans, snapping/history and consumer integration remain Stage 2–4 requirements.
This increment changes FreeCAD only; OCCT remains at ad3549601d.

## Milestone 73: native checked-display jobs

Native heads: FreeCAD 5d6153ba5e; OCCT ad3549601d.

Coverage and exact contact auditing now participate in OCCT's optional work
scope, with Coverage/ContactAudit phases and checkpoints in recursive search,
triangle validation, integer conversion and audit traversal. A calibrated
mid-operation cancellation test covers both default operations, successful retry
and unchanged source/mesh/proof flags. Full kernel 116/116 passes (326.96 s).
The runner timeout is now 420 s because the new test adds about 64 s to the
previous 254-s suite; geometry budgets/tolerances are unchanged.

CheckedDisplay.cpp now owns the Python-free binary32 conversion, clipped
proximity and exact contact checks. Both synchronous bindings and display-only
worker jobs use it and the same report converter. start_surface_job and
take_surface_job expose unchecked/checked extraction without constructing solids.
native_job_status/cancel_native_job control either job kind. Surface and solid
jobs share one worker and one additional queue slot, private source snapshots,
capsule cancellation and joined shutdown. Results include surface, input_record
and the full input_settings descriptor; mismatched take operations reject before
consuming a result. Existing solid-job APIs remain compatible.

The display-job CLI test passes for exact synchronous/private-snapshot buffer
equivalence, Python heartbeat, shared queue limits, coverage/audit cancellation,
budget failure, one-shot consumption and source lifetime/immutability. Earlier
checked-display CLI/runtime 2/2 passes (64.59 s); the GUI regression passed
(104.13 s) before extending the worker API. See PROGRESS.md for compatibility
and shutdown results. No packages or system libraries changed.
Mixed-job compatibility/runtime/shutdown checks pass 4/4 (32.54 s), including
normal exit while coverage is running and the existing solid-job lifecycle tests.
Expanded display-job/runtime checks pass 2/2 (51.61 s), including the complementary
BoxMinusField path and exact buffer comparison on private snapshots.

Immediate next step: connect NativeSolidView to display-only jobs, coalesce
settings/geometry changes, cancel on deletion/document close, guard generation/
digest/settings before scene upload, and publish DisplayProximityChecked only
after successful upload of that job's buffers. Existing views still execute
checked display synchronously. Add actual GUI heartbeat, stale-job/failure/restore
and cancellation tests before changing that workflow. Ordinary recompute, source
tracing, default native-solid exposure and remaining Stage 2–4 gates stay open.

## Milestone 72: asynchronous shared-solid editor previews

Native heads: FreeCAD 4e2086008e; OCCT unchanged at 44ae271b27.

Apply/OK now coordinate native solid jobs through ImplicitSolidBatch.SolidBatch.
The task retains its owned edit transaction, invalidates affected native peers,
builds them sequentially and publishes only after all results succeed and current
source/settings/transaction identities match. Pending recompute does not bypass
the worker with a synchronous solid build. Cancel rolls back the edit; changed
controls, dirty inputs, missing owners, document closure and lost transactions
discard pending work. Initial candidate buffers feed the existing view cache.
Unchanged restored geometry stays unverified; it is not rebuilt merely by opening
or applying unchanged editor controls. Direct Python task.apply() remains
synchronous for compatibility; GUI buttons use the background path.

Single-object FreeCAD recompute deliberately leaves Touched set. SourceEdits now
purges that flag only after each successful owned field/box evaluation, leaving
downstream invalidation intact. Later external changes still invalidate batches.
The first shared-parameter GUI regression exposed this distinction; see PROGRESS.md
for final verification and [batch contracts](native-solid-jobs.md).
Final affected GUI/runtime tests pass 5/5 (261.73 s), after explicitly testing
invalidation on preflight failure. The earlier CLI/shared-source/GUI run passed
6/6 (168.70 s); expanded CLI includes owner deletion and compiled boundary owners
without a Python Proxy. Logs are recorded in PROGRESS.md.

Next: ordinary property-driven recompute, source boundary construction, checked
display and rollback display extraction remain synchronous and need scheduling.
Completed batch results are retained until group publication, not a global
memory-budget solution. Broader source/link workflows, default native solids,
curved operands, snapping/history and consumers still block Stage 4 completion.
No OCCT changes, packages or system library changes in this increment.

## Milestone 71: asynchronous native-solid creation task

Native heads: FreeCAD 588ebc0cdd; OCCT 44ae271b27.

NativeSolidCreationTaskPanel now submits native jobs and polls with a 50-ms GUI
timer. Cancel, form destruction and document closure cancel outstanding work.
Inputs/settings/source identity and unrelated transactions are checked before
publishing; failures retain the repairable task. No owner/transaction exists
while numerical construction runs. NativeSolidView consumes the returned initial
candidate instead of synchronously extracting it again, preserving component
assignment and atomic scene-upload behavior. Public console create is unchanged.

Initial actual GUI/fixtures pass 3/3 (65.11 s), including Python/Qt event-loop
heartbeat and forbidden synchronous build/meshing during publication. Expanded
lifecycle tests additionally exercise cancelled builds, changed settings/inputs,
external transactions and closing a document during work. Final creation/editor/
runtime regression passes 4/4 (180.37 s): editor 125.25 s, creation 48.81 s.
Log: /tmp/implicit-async-create-final.log. No packages or system-library changes.
Final capture-adjusted GUI/runtime 3/3 passes (46.17 s). The inspected capture
shows surface/edge/vertex rendering but remains partly cropped by the headless
dock layout; do not treat it as a full-model visual qualification.

Next: make existing solid editing/recompute and checked display responsive;
qualify broader source/link workflows and default native-solid creation. Curved
operands/general mixed dispatch, snapping/history and cross-workbench consumers
remain required. Stage 4 is not complete. Source serialization, Python result
conversion, owner publication and Coin uploads still execute on the GUI thread.

## Milestone 70: background native-solid backend

OCCT now has optional thread-local implicit work scopes and cooperative
cancellation checkpoints through topology, attachments, geometry and meshing.
FreeCAD's OCCT-8 module has one native worker plus one queued request, private
serialized source snapshots, phase/status polling, cancellation, one-shot result
consumption and joined shutdown. Results retain fresh validated native topology
plus an initial transient surface candidate; no Python/document/Coin work runs
on the worker. Source serialization and Python result conversion remain on the
caller thread; cancellation is not a hard real-time guarantee.

The private ImplicitSolid._create(completed=...) publication path rejects
changed records/settings, dirty dependencies, mismatched/unvalidated handles
and external transactions before committing an owner. Public create stays
synchronous and compatible. See [contracts](native-solid-jobs.md).

Focused kernel cancellation tests pass 2/2 (17.975 s). Worker/checked-display/
runtime checks pass 3/3 (86.58 s); publication/shutdown/live/settings/runtime
checks pass 5/5 (38.54 s). They cover actual Python heartbeat, both operations,
topology equivalence, phase cancellation, queue limits/dropped handles,
source lifetime, normal exit with active work, stale-input rejection and
transactional publication without numerical rebuild. Full kernel 115/115 passes
(254.34 s), retained in build-occt-implicit/native-face-tests/Testing/Temporary/LastTest.log.
No packages or system library changes.

Immediate next step: connect NativeSolidCreationTaskPanel to start/status/
cancel/take via a GUI timer; bind source identity/settings, handle document
closure and external edits/transactions, and seed NativeSolidView with the
initial candidate to avoid main-thread remeshing. Add real GUI heartbeat,
Cancel/failure/retry/stale-job/close-document coverage. Existing edit/recompute
and checked-display scheduling remain separate work. The creation GUI is still
synchronous, default Add Gyroid is unchanged, and all unfinished Stage 2–4
curved-operand/snapping/history/consumer gates remain active.

## Milestone 69: creation-time native solid settings

The existing Build live native solid from boundary menu command now opens
ImplicitSolidCreate.py's task before any object or numerical solid is created.
It stages tolerance/resolution, independent geometry depth and all work limits.
Shared BUILD_LIMITS drives native defaults and both creation/editing controls;
the old factory positional arguments and default fingerprints remain compatible.
create additionally accepts validated build_limits and optional hide_source.

OK uses the atomic factory; failure rolls back the temporary owner and leaves
the task open for repair. Optional source hiding belongs to the creation undo
transaction. Cancel before building has no document effects. The GUI test covers
the actual command, a shifted 20-mm model at 0.1 mm/depth 18, budget failure and
retry, source visibility, undo/redo, save/reopen without rebuilding, invalid
inputs/limits and closing a document while staging. CLI/runtime 4/4 (58.61 s)
and final affected GUI/runtime 5/5 (194.17 s) pass, including existing solid and
source-parameter editors. Actual creation buttons and the depth control are
fully visible. The settings/result captures were visually inspected; the result
capture waits for camera fitting to settle. Logs:
/tmp/implicit-solid-create-cli.log and /tmp/implicit-solid-create-gui-regression.log.
No OCCT/library changes or package installation. Builds remain synchronous.
FreeCAD implementation: 7ba0ffe031; OCCT remains 55f6211879.
See [creation/editing workflow](native-solid-editor.md).

Next: responsive native construction/display scheduling and cancellation,
broader source/link workflows, default native-solid creation workflow, general
mixed BOP/curved operands, snapping/history and consumer integration. The full
Stage 2–4 acceptance goal remains active.

## Milestone 68: shared primitive source parameters

NativeSolidTaskPanel now stages Period/IsoValue and Part::Box dimensions through
ImplicitSourceEdit.py. It updates authoritative leaf properties inside the owned
task transaction, not duplicate field definitions or root-to-input expressions.
Canonical workbench dimension expressions are preserved; ordinary fields without
dimension aliases work too. Custom expressions are protected. Unchanged rounded
controls retain exact original doubles. External source edits reject instead of
being adopted; cross-document shared-input mutation is unsupported.

Shared same-document mixed sources and native solid peers are invalidated before
changing math and explicitly rebuilt. Failed singular-level previews clear old
geometry/display/flags, remain repairable and roll back through Cancel. CLI and
real GUI smoke pass for 10-to-5-mm scaling at the 0.1-mm target, shared
peers, expressions, undo/redo, singular rejection, repair, rollback and unchanged
FCStd restore. Full migration/GUI run: 65/66 pass (778.00 s); the only failure
was a new full-control visibility assertion. Qt's ensureWidgetVisible scrolls a
spin box's input-method cursor rectangle, leaving its frame clipped. The test
now scrolls the complete rectangle and retains viewport-containment checks.
Final focused GUI/fixtures pass 3/3 (48.30 s), and the capture was visually
inspected. Production code is unchanged from the full run. Expanded CLI/runtime
checks pass 2/2 (22.60 s): separate Common/BoxMinusField sources sharing inputs,
plus a saved external App::Link that prevents mutation. See PROGRESS.md.
FreeCAD implementation: 4cfd5bf1a1; OCCT remains 55f6211879.
See [editor contracts](native-solid-editor.md). No new OCCT code, packages or
system changes. Default Add Gyroid is still unchanged.

Next: creation-time build controls,
broader source/link workflows and responsive scheduling remain before default
native-solid exposure. Curved operands/general mixed BOP, snapping/history and
consumer integration remain open Stage 2–4 requirements.

## Milestone 67: native-solid task editing and independent geometry depth

NativeSolid adds MaximumGeometryDepth=-1 (inherit) or 1..24. Only non-default
depth appends a ninth input setting, preserving legacy fingerprints/default API
prefixes. Missing properties migrate on restore without rebuilding unchanged
topology. create(..., maximum_geometry_depth=18) sets it before the initial checked
build; expected creation failure aborts cleanly before normal recompute logs an
uncaught traceback. Shifted 20-mm live Part::Box succeeds at 0.1 mm with reference
depth 16/geometry depth 18; shared defaults fail without leaving an owner.

The first larger-model test exposed the corner recognizer's blanket location
guard. OCCT now accepts exactly pure translations for conventional boxes and
each subobject, while preserving exact world-space corner/line/plane, topology,
domain and precision checks. No operand is moved/snapped/baked. Both operations'
canonical topology records match directly built boxes. Rotations, scaling and
placed implicit fields still reject; general placed/link/container BOP is open.

NativeSolidView now opens a staged task panel. Solid tolerances/reference/depth/
budgets/followed placement and same-document live shared-source operation can be
edited. It inherits owned transaction/button lifecycle from MixedBoundaryTaskPanel;
unrelated transactions are protected. Stored/external sources are read-only for
operation. Numerical preview failures clear result/display/checks and can recover
or roll back. GUI tests wait for actual visible animated-overlay controls, not
hidden button invocation. The panel explicitly shows the Tasks view.
Visual inspection then found a clipped Cancel button despite Qt visibility.
FreeCAD Gui now grows/reanchors active task overlays to their actual child
minimum hints and refreshes after the delayed TaskView minimum update. The
module uses wrapped rows and the existing task scroll area. Tests require full
button visible regions and an actually visible depth entry; final focused GUI
3/3 (112.18 s) passes and the capture was visually inspected. Upstream CRLF GUI
files are preserved; use git -c core.whitespace=cr-at-eol diff --check there.
See [contracts/workflow](native-solid-editor.md).

Focused translated-box kernel 1/1 (7.629 s), larger/legacy CLI smoke (success),
initial GUI fixtures/test 3/3 (104.35 s, before the read-only-source safeguard)
and full kernel 113/113 (242.49 s) pass. Compatibility 24/24 (38.48 s) passes;
Full migration/GUI 64/64 (757.00 s) passed before the final GUI layout/core fix;
the complete GUI/runtime/demo subset passes 15/15 (324.33 s) after that fix,
including the strengthened editor test (109.69 s). Native heads: FreeCAD
3ed3882985, OCCT 55f6211879. See PROGRESS.md. No new packages/system
changes or relaxed geometry budgets/tolerance. Default Add Gyroid unchanged.
Next broader source field/dimension/link editing, creation-time build controls
and scheduling before default
exposure; curved operands, snapping/history and all Stage 2–4 gates stay active.

## Milestone 66: opt-in checked native-solid display buffers

FreeCAD's tessellate_mixed_checked validates the same candidate it returns for
display. It bounds binary64-to-binary32 upload displacement, runs clipped
proximity at tolerance minus that bound, then exact-audits the actual rounded
triangles. Returned display_checks add the displacement to both directed bounds;
native topology/solid and legacy candidate proof flags remain false. Unsupported
precision, contacts and work limits reject without publishing buffers. Original
assignment_vertices retain canonical component-domain assignment, with checked
display displacement; no coordinate welding or CAD mesh faces.

NativeSolidView adds opt-in CheckSurfaceDisplay and read-only
DisplayProximityChecked. Cache keys bind actual solid digest/tolerance/resolution/
check mode; visibility/camera reuse is allowed. Failed extraction clears the field
surface/report; failed scene upload clears all nodes/aliases/report and retains
native geometry. Successful checked status/cache key is published only after
complete scene upload. Recompute can retry after repair. FCStd stores settings,
not checks/buffers; restored solid flags stay unverified while new display checks
run. All bounds are source-local binary32, not world-transform/GPU/pixel precision.
See [contracts and workflow](native-checked-display.md).

Expanded CLI/runtime 2/2 (59.19 s) passes, including both default operations,
restore, exact returned upload values, budgets and non-binary32 strict box caps.
Initial GUI/fixtures 3/3 (100.06 s) passed before adding an explicit injected
scene-upload failure check and tightening atomic publication. Final migration/GUI
62/62 (615.43 s) passes, including the tightened GUI test (99.39 s); compatibility
24/24 (26.49 s) passes. FreeCAD commit b87cddaea1.
OCCT is unchanged from milestone 65 (112-case kernel baseline). No packages,
system libraries, default-command changes or geometry-budget relaxations.
Checking remains synchronous; broader source/task editing, responsive scheduling,
curved operands, snapping/history, consumers and all Stage 2–4 gates remain.

Immediate editing follow-up: the native factory already accepts independent
maximum_geometry_depth, but ImplicitSolid.NativeSolid still forwards only the
old shared-depth settings. Expose/migrate that setting without changing legacy
default fingerprints, and qualify larger-model/source/task edits at the same
0.1-mm target. Do not relax tolerance or remove unresolved states to make edits
appear supported. Checked display also needs scheduling/cancellation qualification.

## Milestone 65: corner-aware clipped-boundary proximity

CheckCoverage now supports Regularized sources through fresh exact lattice
corner germs. Whole-cell Hessian bounds prove monotone uniform cones over the
anchor/cell hull: both isolated signs are absent from the 2-D field face, but
only unretained in-plane cones exclude cap closure. Retained exact cap corners
can witness reverse proximity; raw F=0 cannot. Whole normal-line slabs must
also avoid canonical field-face exclusion boxes. Existing two-way distance,
numerical reserves, depth/cell/patch limits and source immutability remain.

The FreeCAD explicit API adds regularized_field_cells, regularized_cap_cells,
corner_witnesses and exclusion_slab_rejections diagnostics. Default Common/Cut
resolve at 0.1 mm with 8 field/18 cap cone cells, 102 improving corner witnesses
and 51 slab/exclusion rejections each. This is numerical proximity, not directed
interval arithmetic, topology/solid validity or generic Part/BOP support.
No GUI/default-command change: the separate report does not certify future
buffers or mutate candidate SurfaceCoverageVerified/seam/solid proof flags.
See [contracts and next gates](native-corner-coverage.md).

Kernel additions include independent harmonic roots/cap samples, both operations,
shifted/scaled restored sources, missing buffers, forged flags, resource failures
and spurious field/cap triangles near isolated excluded contacts. CLI covers
periods 10 and 5 with translation, owned lifetime, restore and provenance.
Initial focused kernel 3/3 (22.409 s, before the extra cap-negative assertion)
and CLI/runtime 2/2 (36.55 s) pass. Full kernel 112/112 (232.21 s, including
the cap-negative test), compatibility 24/24 (28.22 s), and full migration/GUI
60/60 (456.97 s) pass. Native commits: OCCT 1c665b63d3, FreeCAD 8f523e66c6.
The expanded kernel
harness timeout is 300 seconds, not an increased geometry-check work budget.
Existing dependencies/builds only. Broader source/task editing and fresh GUI
buffer qualification precede default exposure; all other Stage 2–4 gates remain.

## Milestone 64: corner-aware transient meshing and opt-in live GUI

Prepare now supports regularized cap topology using native shared vertex/edge
sampling, with all source caches kept private. Tessellate uses actual odd grids
and fresh exact physical-corner identities, preserves six split trim corners,
and checks whole field-triangle AABBs against canonical exclusions. Cap-only
retained corners remain on caps. Mesh policy/fixed-corner diagnostics are exposed;
untrimmed field bound/closed mesh flags do not certify coverage or solids.

The first contact audit hit its existing limit. A balanced 3-D triangle-AABB
hierarchy replaces the X-only sweep, preserving exact predicates, shared-simplex
rules, work limits and unresolved-on-limit behavior. BroadTests counts node and
leaf-box comparisons. A sparse coplanar regression plus injected duplicate and
both default corner candidates pass without raising any audit budgets.

ImplicitMixed now has explicit CornerPolicy and create(..., corner_policy=...).
Legacy Reject fingerprints are unchanged; regularized policy participates in
new fingerprints and unchanged-input checks. The existing native solid view
renders this opt-in path with smooth field normals and separate cap normals.
GUI tests cover actual face/vertex ray picking, semantic selection, both operations,
undo/redo and FCStd restore without solid reconstruction. Default Add Gyroid is
unchanged. See [contracts and next gates](native-corner-mesh.md).

Focused kernel 4/4 (26.414 s), runtime/mesh CLI 2/2 (33.36 s), GUI fixture/test
3/3 (28.17 s), full kernel 109/109 (210.09 s), and compatibility 24/24 (38.94 s)
pass. Full migration/GUI 59/59 (430.74 s) also passes. The actual viewport capture
was visually inspected: clipped field, separate caps, native boundary edges and
vertex markers render; some tessellation/shading remains visible. No analytic or
coverage-certified display claim. Capture: /tmp/implicit-corner-native-solid.png.
Next corner-aware clipped coverage and broader live/task editing/default exposure.
All unfinished Stage 2–4 scope remains active. Existing builds/dependencies only.

## Milestone 63: corner-aware native solids and schema-2 solid topology

BuildNativeSolids now accepts regularized corners through the existing fresh
whole-face/shell/topology, cap geometry and stored-tolerance pipeline. Both default
operations construct one actual native solid (13 faces/30 edges/19 vertices,
shell Euler -2), at the 0.1-mm target. No mesh faces, generic Part/BRepCheck/BOP
or manufacturing claim. The retained uniform corner is cap-only and both isolated
zero contacts remain absent from the implicit face.

The original C++ signature and default depth behavior are preserved; an overload
adds independent geometry depth. FreeCAD appends maximum_geometry_depth=-1
(-1 inherits reference depth). A shifted 20-mm fixture uses geometry depth 18
at 0.1 mm with reference depth 16, without loosening tolerance.

Native solid codec schema 2 admits regularized sources; strict output remains
schema 1. Source restrictions must be canonical, component restrictions must
equal their source/domain intersection, and restored component faces preserve
those restrictions. Version/source-policy mismatches and lost masks reject
atomically/preflight. Actual shared topology is restored, not retraced. Restored
validation/geometry/topology/Closed flags stay false; explicit source rebuilding
rechecks separately. Existing native solid properties, subelements, runtime
undo/redo, FCStd close/reopen and owned handles are exercised by the new CLI test.
See [contracts and next gates](native-corner-solids.md).

Next corner-aware mesh/coverage/view support and live/task/selection qualification
before changing default Add Gyroid. No default GUI switch yet. Curved operands,
snapping/history and all unfinished Stage 2–4 requirements remain active.
Focused kernel 3/3 (36.942 s), runtime/solid CLI 2/2 (33.45 s), full kernel 106/106
(179.41 s), compatibility 24/24 (36.98 s), and migration/GUI 57/57 (359.29 s) pass;
results/logs are in PROGRESS.md. Existing builds/dependencies only.

## Milestone 62: corner-aware native cap geometry

CheckCapGeometry now handles closure of retained area on each physical box side.
Exact corner jets plus whole-cell Hessian derivative bounds exclude cells near
isolated unretained planar zeros; native candidate cells never use this exclusion.
Strict retained side points and exact corners with proved retained side rays
provide physical-distance witnesses. Isolated field zeros are not automatically
cap witnesses; retained uniform material corners are cap-only. Existing directed
coverage, actual lifted p-curve/3-D edge/vertex deviations and work limits remain.
No field residual is treated as a millimetre distance.

FreeCAD exposes check_mixed_cap_geometry with coverage bounds, counter/policy
diagnostics, resource/precision failure information and false topology/solid flags.
The test suite includes actual near-corner planar B-spline patches: a redundant
patch within tolerance of a retained corner can pass geometry alone, but a patch
near the excluded zero fails. Thus this check does not qualify native topology.
Default caps use strict interior witnesses without necessarily using corner ones.
Both operations, shifted/scaled sources, missing/opposite-material caps, restore,
ownership and budgets are covered. See [contracts](native-corner-cap-geometry.md).

The 0.1-mm target/default depth 16 are unchanged. A shifted 20-mm fixture required
an explicit depth-18 geometry request; do not relax physical tolerance to pass.
Native solid integration will need independent geometry depth because its shared
depth argument also feeds reference routines limited to 16. BuildNativeSolids,
corner solid-codec, mesh/coverage and default GUI remain guarded/unfinished.
Next integrate cap qualification into corner-aware native solids and persistence,
then mesh/view/default-workbench exposure. All Stage 2–4 requirements stay active.
Focused kernel 4/4 (8.612 s) and runtime/cap CLI 2/2 (3.88 s) pass; full verification
is recorded in PROGRESS.md. Kernel 103/103 (145.59 s) and compatibility 24/24
(38.14 s) pass; full FreeCAD migration/GUI passes 56/56 (333.82 s).
Existing dependencies/builds only; no default-workbench switch. Restart the fork
before interactive API tests.

## Milestone 61: corner-aware whole native faces and shells

BuildNativeShells now attaches the whole restricted 2-D field face through the
fixed-corner global homotopy and existing cap/trim-cycle checks. Native component
faces preserve canonical source exclusions, clipped to verified component boxes;
seam segment AABBs must exclude these neighborhoods. Both default operations
construct one native shell with 13 faces, 30 edges, 19 vertices and Euler -2.
Both uniform contacts remain absent from the implicit face; the retained one
is cap-only. The existing opposite-edge, cyclic vertex-link, connected face and
Euler/material grouping checks are retained. No reference triangle is a CAD face.

Partition interpolation uses the actual reference grid and exact fixed-corner
nodal values. A disconnected regularized case exposed the hard-coded 32-grid
CheckCorrespondence precondition inside cap attachment. A new overload forwards
caller witness settings while preserving the original entry point's defaults.
The disconnected case now resolves at requested 64/actual 63. Solid construction
still refuses the corner policy explicitly: geometric/cap/coverage gates remain.
Native API reports corner_policy and reference_resolution on shell snapshots;
owned field-face queries preserve exclusions and placements. Source FCStd can be
restored and used to construct shells; no direct shell-property/codec is claimed.
See [contracts and remaining work](native-corner-shells.md).

Next corner-aware native cap geometry/whole-boundary qualification, then native
solids/default GUI. Curved operands and every unfinished Stage 2–4 integration
requirement remain active. The expanded monolithic kernel harness allows 240 s
instead of 180 s; numerical settings/accuracy budgets are unchanged. Final full
verification is in PROGRESS.md; existing builds/dependencies are reused. Fresh
kernel run passes 99/99 cases (136.62 s), runtime/shell CLI 2/2 (13.01 s),
full FreeCAD migration/GUI 55/55 (327.43 s), and compatibility 24/24 (37.69 s).
No default-workbench switch; restart the rebuilt fork for interactive API tests.
For the next geometric gate, preserve closure-of-interior material membership
through cap witnesses and projections: an isolated field zero is not an implicit
surface witness, while the retained uniform corner is cap-only. Mesh/coverage
consumers must honor the actual restricted face domain. Do not remove the solid
guard merely because native shell incidence now passes.

## Milestone 60: native face-domain exclusion of isolated corner contacts

Regularized mixed construction now stores actual closed exclusion boxes in
BRep_TImplicitFace, derived from fresh uniform-cone corner bounds. BOTH isolated
zero contacts are absent from the 2-D face domain; the material's retained corner
remains cap-only. Six split corners remain. Domain/ray queries and empty copies
honor restrictions, while Value/Gradient still query the unchanged source field.
Primitive schema 2 preserves boxes; unrestricted output remains schema 1. Mixed
schema-2 envelopes embed the new primitive format and migrate older unrestricted
math records using local corner checks, without retracing topology or recertifying
saved bounds. Mixed validation checks the exact canonical restriction. Unsupported
primitive consumers refuse restrictions explicitly. FreeCAD exposes exclusion
count and existing owned/translated face queries. See [contracts](native-face-domain.md).

Next: whole restricted-face attachment, corner-aware native geometry/shell/solid
qualification, then default GUI exposure. This does not itself enable native
default solids or complete any Stage 2–4 gate. Verification: 13/13 focused kernel
cases (38.467 s), runtime/trim CLI 2/2 (29.20 s), full 96/96 kernel (156.27 s),
54/54 FreeCAD migration/GUI (435.76 s), and 24/24 compatibility (39.55 s).
Existing targets/dependencies reused; no packages/system changes. Logs are in
PROGRESS.md. Restart the rebuilt fork before testing interactively.
For the shell extension, do not just remove its policy guard: Partition currently
uses requested resolution and raw nodal F. Corner-aware references use their
actual odd grid and fixed exact-corner values. Component faces must preserve
the checked exclusions (clipped to any verified component domain), and reference
component attachment must cover the whole restricted 2-D face, not just its wires.

## Milestone 59: complete corner-aware directed trim cycles

CheckTrimAttachment now gives exact split corners canonical IDs distinct from
the ordered interior box-edge roots. Each is one shared anchor, not three roots.
Complement construction preserves source corner policy; dual references must
share their actual grid/root provenance. Existing dual-cap region pairs,
continuous curve matches and complete directed cyclic bijections remain required.
Default Common/Cut attach 18 seams/one loop/one reference surface component
(Euler -3), assigned to reference closed component 0/material region 0.
See [contracts/API/next gates](native-corner-trim.md).

Native API exposes policy, actual reference resolution and reference component
Euler/shell/material labels. Genuine wire-start rotation, restore, source
immutability, budget refusal and owned property lifetime have positive tests.
No source proof flags recertified, schema/layout change or default GUI switch.
Whole implicit-face attachment/native shell/solid/topology flags remain false.
Next implement actual 2-D native face restriction/membership with isolated
contact exclusion, not merely a filtered display mesh; then native geometry/
shell/solid gates and default GUI. All other Stage 2–4 requirements stay active.

Existing targets/dependencies reused; no packages/system changes. Both monolithic
kernel CTest harnesses now allow 180 seconds because the expanded 94-case suite
takes 126.76 s; all numerical/accuracy budgets are unchanged. Full verification:
94/94 kernel cases, 54/54 FreeCAD migration/GUI checks (409.86 s), and 24/24
remaining compatibility checks (37.89 s). Logs follow in PROGRESS.md.
Restart the rebuilt fork; full goal remains active.

## Milestone 58: corner-aware stored graph and native cap-region attachment

CheckCorrespondence now uses the complete fixed-corner whole-box homotopy gate
for regularized sources, then ALL existing curve/vertex/wire/cap/p-curve and
tangent checks against fresh native traces built with the same policy. Default
Common/Cut match 30 edges, 19 vertices, 12 caps and one native wire. Strict paths
are unchanged. API regularity_policy distinguishes strict_strata from
fixed_corner_homotopy; the separate legacy regularity API stays strict.

CheckCapAttachment now uses the corner-aware closed reference's ACTUAL grid and
the same exact corner nodal values. Strict common F/L retained witnesses plus
conventional IN classification and clearance attach all 12 native cap regions
bijectively to the one reference component/material region. Reordered caps,
restore, source immutability and budget refusals pass focused tests. See
[contracts/API/next work](native-corner-attachment.md).

Not complete trim-cycle or whole implicit-face attachment, native shells/solids
or default GUI integration. CheckTrimAttachment has an explicit early corner-
policy guard until it handles corner anchors distinctly from box-edge roots.
Mesh/coverage/cap-geometry/shell/solid guards remain; no proof flags recertified.
Next attach directed cycles/whole components, then native geometry/solid gates
and default GUI. All other Stage 2–4 requirements, including curved operands,
snapping/history and broader consumers, remain active.
Actual 2-D native face membership/consumers must exclude the isolated relative-
box zero contacts; merely omitting their display-mesh vertices is insufficient.

Existing core/test-worker/FreeCAD targets reused with two compile jobs and one
FreeCAD link job. No packages/system changes, schema or transient layout changes.
Final checks: **92 kernel cases** (104.19 s), **53/53 migration** (311.09 s),
remaining **24/24 compatibility** (29.89 s; kernel separately passed), targeted
runtime/region/attachment CLI **3/3** (24.45 s). Checkpoint: OCCT `3559021067`,
FreeCAD `159ff40dff`. Commands/logs are in PROGRESS.md; restart the rebuilt fork.
Full goal active.

## Milestone 57: closed corner material reference and strict volume grouping

Explicit regularized CheckRegionTopology now caps the checked surface on its
actual odd grid using the SAME `(i,i)` corners and edge-root IDs. Isolated zero
contacts never create cap area; a retained uniform corner is cap-only, while the
excluded uniform corner is absent. Volume DSU uses strictly retained vertices
only. Zero corners inherit one unique incident strict-volume label and cannot
join regions through a point. Existing closed manifold/orientation/Euler checks
remain required. See [contracts and API](native-corner-region.md).

Default Common/Cut each resolve to one component, Euler -2, one material region;
reference volumes are 500 each and complementary sum 1000. These are numerical
AFFINE-reference diagnostics, not authoritative F volume or .1 mm geometry
certification. Source/restored bounds and native validity remain unchanged.
Raw region queries take final explicit policy; mixed queries honor source policy.
No persistence schema/layout change. Native attachment/mesh/solid guards remain.

Next map stored native cap regions/trim cycles/whole implicit components to this
reference, then geometric validation/default native solids/GUI. Do not promote
based on closed mesh incidence or equal component counts. All other Stage 2–4
requirements stay active, including general/curved operands and history/snapping.
Existing TKMesh/test-worker/FreeCAD module targets reused with two compile jobs
and one FreeCAD link job; no packages/system changes. Final verification:
**90 kernel cases** (92.72 s), **52/52 migration** (296.01 s), remaining **24/24
compatibility** (29.86 s; kernel separately passed), targeted CLI/runtime **3/3**
(12.16 s). Checkpoint: OCCT `5a3b910c86`, FreeCAD `a6cbb419b3`.
Commands/logs are in PROGRESS.md; the full goal remains active.

## Milestone 56: whole-box corner homotopy and surface reference

Explicit regularized CheckTopology now handles fixed exact corner zeros on an
odd grid <= requested resolution. Existing whole-volume/face/edge homotopy checks
remain required; every corner-incident linear tetrahedron must additionally
match all inward coordinate signs of the fresh F germ. Canonical `(i,i)` grid
identities share surface corners; uniform isolated contacts stay separate and
never create fake surface triangles/vertices. Other sampled zeros remain
unresolved. No field/domain/phase perturbation or saved proof flags.

The default cube resolves with eight fixed corners, six shared surface-corner
nodes and two point contacts. Its 2-D reference has Euler -3, one component and
one boundary loop at actual resolution 31 (requested 32). This is not closed-
region/material connectivity, stored native attachment, a .1 mm surface mesh or
solid validity. See [derivation/API/contracts](native-corner-topology.md).

Raw check_box_topology takes an optional final policy string; mixed queries
automatically honor source policy and reject overrides. Corner-aware closed-
region queries explicitly reject. Primitive/mixed restore and source provenance
are tested. Next implement shared-ID closed caps and strict-volume grouping,
then stored trim/cap/whole-face attachment and default native solid/GUI exposure.
All other Stage 2–4 requirements remain active, including curved operands.

IMPORTANT: TopologyWitness/nested transient ABI layout changed. Rebuilt TKMesh
dependencies, native tests/worker and FreeCAD Implicit/ImplicitGui/_ImplicitOCCT
using existing builds/dependencies. Restart the fork; no system changes or
OCCT-8/7 mixing. Final checks: **88 kernel cases** (85.27 s), **51/51 migration**
(276.67 s), remaining **24/24 compatibility** (38.13 s; kernel separately passed),
targeted native CLI/runtime **2/2** (5.40 s). Checkpoint: OCCT `f5d703459e`,
FreeCAD `fd5a3fe902`. Commands/logs are in PROGRESS.md; full goal remains active.

## Milestone 55: regularized corner-aware physical classification

Classify now accepts regularized boundary/property/schema-2 snapshots. Existing
strict-sign cap points and in-box sign brackets remain genuine witnesses.
Fresh exact corner germs add retained/split corner boundary witnesses. Excluded
closed-set contacts NEVER witness the regularized boundary: a gradient/Hessian
Taylor bound excludes a clipped ball about each, giving physical distance lower
bounds even outside the box. Insufficient bounds remain unresolved. Field/box
math and seam-bound provenance stay unchanged; no solid/topology validation.
See [derivation and contracts](native-corner-classification.md).

Native classify_mixed reports corner_policy; whole ComplexGeoData queries reuse
the existing rigid/container/link placement reserves. Kernel cases cover both
operations/all corners, scaling/periodic translation, roots, restore and source
immutability. CLI tests add real property/FCStd/placed-link ownership. No default
workbench or mesh changes. Whole-domain corner-aware reference/actual attachment
is next, followed by qualified default solids/GUI. Mesh/coverage/correspondence/
cap-geometry/shell/solid guards remain; all other Stage 2–4 gates stay active.

Existing TKMesh/test-worker/native FreeCAD module/script targets reused with two
compile jobs; no new packages, system changes or transient ABI layout changes.
Final verification: **86 kernel cases** (87.94 s), **50/50 migration** (275.98 s),
remaining **24/24 compatibility** (27.74 s; kernel separately passed), targeted
runtime/corner APIs **4/4** (7.25 s). Checkpoint: OCCT `c9c0bb7915`, FreeCAD
`922d50405d`. Exact commands/logs and resumed-test evidence are in PROGRESS.md.
Restart the rebuilt fork; the full goal remains active.

## Milestone 54: corner-policy topology persistence and document ownership

Schema 2 of BRepClass3d_ImplicitBoxIO retains the regularized material policy,
corner counts and excluded contacts alongside the actual shared graph. Strict
boundaries still write unchanged schema 1. Local corner germs and actual native
corner vertices/seam-end incidences are checked on write/read, not retraced;
restored seam bounds remain unverified. Metadata corruption is atomic.
The existing native geometry property now supports real FCStd, content transfer,
subobjects and undo/redo for that graph. No default workbench change or solid
qualification. See [contracts](native-corner-persistence.md).

IMPORTANT: removing the former codec rejection must not open unsupported
consumers. Native mesh Prepare/CheckCoverage, CheckCorrespondence, CheckCapGeometry,
BuildNativeShells and solid codec source validation explicitly guard the policy.
Old physical-distance classification still rejects. Whole-domain corner-aware reference/actual
attachment and region classification remain next, then validated default solids
and GUI exposure. Curved operands and all unfinished Stage 2–4 gates remain.

Existing TKMesh/native-test-worker/FreeCAD module and script targets reused; no
new dependencies or ABI-layout changes. Restart the rebuilt fork. Final checks:
**84 kernel cases** (87.42 s), **49/49 migration** (289.13 s), remaining **24/24
compatibility** (29.22 s; kernel separately passed), targeted CLI/runtime **3/3**
(5.18 s). Checkpoint: OCCT `91b3bdb2f6`, FreeCAD `a54c95ff6d`.
Exact commands/logs are in PROGRESS.md; full goal remains active.

## Milestone 53: default-corner native boundary graph

Explicit `CornerPolicy::Regularized` / `mixed_box(..., corner_policy="regularized")`
now connects the unchanged default cube's face sections into actual shared
native topology: 18 seams, 12 retained box intervals, 12 interior edge roots,
19 vertices. Six split corners each join two contours by canonical identity;
the isolated closed-set contact stays separate metadata. Conventional caps pass
BRepCheck and every native edge has two opposite uses. Exact field faces have
no fake UV surface; no mesh becomes native geometry. Both operations pass.
Exact restored identity datums, scaling and periodic translation are tested.

Legacy default remains reject. Schema-1 topology writing rejects regularized
policy atomically, protecting property assignment and dependent mesh/reference/
shell/solid factories. Old physical-distance classification explicitly rejects
too. No default workbench or solid-validity claim changed. Next extend policy-
aware persistence/classification and corner-aware whole-domain reference/actual
attachment before validated solids and default GUI exposure. Curved operands
and all unfinished Stage 2–4 requirements stay active. See
[contracts and tests](native-corner-boundary.md).

Boundary transient ABI changed: rebuilt TKMesh dependencies, native tests/worker,
FreeCAD Implicit/ImplicitGui/_ImplicitOCCT, and refreshed compatibility scripts.
Restart the fork; no packages/system libraries changed. Final verification:
**83 kernel cases** (81.99 s), **48/48 OCCT-8 migration** (268.45 s), remaining
**24/24 compatibility entries** (27.15 s; kernel entry run separately).
Checkpoint: OCCT `8ed9300174`, FreeCAD `0ed6b921ce`. Commands/logs are in
PROGRESS.md; full goal remains active.

## Milestone 52: lattice-node plane tracing and isolated contacts

The native plane-rectangle tracer now has explicit AllowExactLatticeNodes mode.
Exact quarter-period nodes share grid identity across cell edges; endpoint roots
need whole-edge monotonicity, and signed derivative cones distinguish entering
arcs from isolated cell-quadrant contacts. Graph checks preserve isolated
rectangle-corner points separately in Result.PointContacts; no zero-length curves
or coordinate welding. The six default cube faces each resolve to three open
curves and two isolated contacts, with two exact corner curve ends. Interior
grid-zero sharing and reversed coordinate frames also have positive tests.

Mode requires identity field placement and coordinate-plane frames. TwoSum
rejects a rounded affine map merely landing on a lattice point; unknown near-
zeros, tangencies, precision/budget exhaustion remain unresolved. Bounded odd
alternative grids avoid repeated eighth-period samples without changing geometry.
Legacy tracing, B-Rep constructors and worker protocol keep strict mode false.
Consumers of the new mode MUST handle contacts even if curves are empty.

OCCT-8 FreeCAD exposes trace_gyroid_plane with keyword settings and explicit
bool policy. Results are owned transient curve/contact data, not native trim
B-Reps, subelements or validated solids. No GUI command/default primitive or
persistence schema changed. See [section contracts](lattice-plane-sections.md).

IMPORTANT: Settings/Result transient layout changed. TKMesh dependencies,
native-face tests/worker and the FreeCAD Implicit/ImplicitGui/_ImplicitOCCT
consumers were rebuilt with existing targets. Restart the fork after rebuilding;
Python refresh alone is unsafe. No OCCT-8 headers enter the OCCT-7 build.

Next integrate canonical box-edge/corner root identity and cap/implicit cycles,
then corner-aware homotopy/reference/native attachment and explicit material/
persistence policies. Whole default native trimming/solids remain unsupported;
do not bypass independent gates or perturb the field. Curved/general operands,
snapping/history, richer editing and broader consumers remain required. The full
through-Stage-4 goal stays active. Verification is in PROGRESS.md.
Final verification: **81 kernel cases**, **25/25 compatibility** (105.34 seconds),
**47/47 OCCT-8 migration** (262.15 seconds); new CLI/runtime **2/2**, targeted new
kernel cases **2/2**. The initial out-of-domain CLI fixture and its correction,
ABI rebuilds, exact commands and logs are recorded in PROGRESS.md.
Checkpoint: OCCT `6e3876f47d`, FreeCAD `000e83bba2`. The new query is available
only in the wholly OCCT-8 fork; installed/OCCT-7 FreeCAD cannot load it.

## Milestone 51: exact lattice identities and checked local corner material

OCCT adds Geom_ImplicitGyroid::LatticeJet, using normalized exact quarter-period
identity recognition without changing Value/Gradient or snapping near-zeros.
CheckCornerGerms bounds zero-free or coordinate-monotone inward neighborhoods;
ClassifyCornerGerm reports closed-inequality versus closure-of-interior membership.
The unchanged default cube has two isolated-contact corners (one per operation)
and six split cones. Unrecognized zeros, tangent coordinate directions, spatial
precision exhaustion and queries outside the local neighborhood remain unresolved.
Exact identity transforms with nonempty restored datum chains work; nonzero
transforms still reject. No source topology/state is changed.

OCCT-8 FreeCAD exposes check_box_corners and classify_box_corner as owned math
queries. They do NOT construct native faces/solids, relax strict regularity,
recertify restored topology or enable the default native trim workflow. Radius
is a local neighborhood size, not an approximation/accuracy bound. See
[corner contracts and next integration](native-corner-germs.md).

Next feed exact corner identities/material policy into plane-rectangle tracing,
canonical box roots and cap/implicit cycles, then extend homotopy/reference and
actual attachment validation. Do not bypass existing gates, perturb the default
field or weld one corner's independent edge roots after the fact. Curved/general
operands, snapping/history, richer editing and broader consumers remain required;
the full through-Stage-4 goal stays active. Verification is in PROGRESS.md.
Final verification: **79 kernel cases** (85.53 seconds), **46/46 OCCT-8 migration**
(334.76 seconds), **25/25 compatibility** (158.47 seconds), plus the refreshed
OCCT-7 facade guard **1/1**. Existing minimal native targets/builds were reused;
no packages/system libraries changed. The first CLI round-trip failure and its
exact-identity fix, commands and full logs are recorded in PROGRESS.md.
Checkpoint: OCCT `27ba200574`, FreeCAD `b79fbbe49d`. Local models are derived
queries only; no new topology/property persistence schema or saved proof flags.

## Milestone 50: live native-solid workflow and component-aware viewport

`ImplicitSolid.py` connects the actual solid property to a live native-boundary
Source, numerical settings/budgets, explicit rebuild and a native solid renderer.
Unchanged inputs reuse actual geometry; embedded-source/tolerance consistency
is checked. FCStd restore refreshes without numerical building and remains
unverified; corruption requires explicit repair. Core skipped-source-error
invalidation clears proxy diagnostics and all viewport geometry too.

Source uses PropertyLinkGlobal so grouping the output does NOT auto-adopt/move
upstream Boolean operands (the initial local-scope link exposed this failure).
FollowSourcePlacement maps source-global into output-container coordinates;
independent output poses are explicit. Output rigid links retain native ownership.
The workbench adds experimental live-solid demo and selected-boundary build
commands. Default Add Gyroid remains a simple field, not a trimmed solid.

Rendering uses actual result solid/shell/face relations and per-component field
nodes; Edge/Vertex buffers come from actual ComplexGeoData. The small native API
increment aliases SeamN to actual result EdgeN by OCCT identity. Source candidate
triangles must lie in exactly one saved component domain; missing/ambiguous
assignments refuse the preview, not replace the result by the source graph.
Smooth analytical field normals stay separate from cap normals; display caches
are transient and compacted by existing vertex indices, never welded.
See [live native solid contracts](live-native-solids.md) for use/tests and limits.

Next continue default-gyroid native trimming/degeneracies, general/curved operands,
snapping/history, richer editing and representation-aware consumer dispatch.
Default iso-0/domain-corner zeros need an explicit topology/regularization policy;
do not silently change the field phase/isovalue/domain to evade that gate.
All unfinished Stage 2–4 requirements stay active; do not claim full Stage 4
from this restricted source/renderer integration. Verification is in PROGRESS.md.
Final full regression: **45/45 OCCT-8 migration** (263.22 seconds),
**25/25 compatibility** (105.80 seconds), including **76 kernel cases**.
Only the existing `_ImplicitOCCT`/script targets were rebuilt; no OCCT,
dependency, system-library or security-policy changes. Initial failures/fixes,
viewport captures and exact logs are in PROGRESS.md.
Checkpoint: FreeCAD `8bdaff16bd`, OCCT unchanged `0e6377579d`. Final focused
GUI/runtime rerun passed 3/3 (29.97 seconds), including the initial demo capture
before placement/link stress. Use the fork OCCT-8 GUI, not installed FreeCAD.

## Milestone 49: actual native solid property and FCStd owner

The OCCT-8 native module now registers `PropertyNativeSolidGeometry` and
`NativeSolidFeaturePython`, with actual result Solid/Shell/Face/component/edge/
vertex ComplexGeoData, distinct snapshot maps and a `.nativesolid` FCStd payload.
The source boundary remains explicit query provenance, never substitute result
topology. Boundary and solid owners share subobject/placement/error invalidation
through NativeFeatureBase, with separate typed Geometry properties and no Shape.

Runtime transactions retain immutable qualified snapshots. File/content restore
uses the actual solid codec without rebuilding and remains numerically unverified.
Tests cover component/cavity/empty/full results, representation mismatch, atomic
content transfer, undo/redo, copy, corrupt FCStd, lifetime, component-domain bounds,
rigid App::Link/containers and upstream-error clearing. Whole classification is
explicitly the source-region union; selected SolidN classification rejects.
See [solid property contracts](native-solid-property.md).

This is a low-level document snapshot owner, not completed solid GUI integration.
Next implement live source/settings recompute and actual component-aware solid
rendering/picking in the workbench, then continue primitive trimming/degeneracy,
snapping/history, consumer dispatch and all unfinished Stage 2–4 requirements.
The default 10 mm gyroid remains the basic field feature. Verification follows
in PROGRESS.md; the full through-Stage-4 goal remains active.
Checkpoint: FreeCAD `46bb773deb`, OCCT unchanged `0e6377579d`. Final clean
OCCT-8 migration passed **43/43** (216.50 seconds); compatibility passed **25/25**
(116.84 seconds), including **76 kernel cases**. Logs, expected corruption/input
diagnostics and initial test-only failures/fixes are recorded in PROGRESS.md.

## Milestone 48: actual native solid topology codec

`BRepClass3d_ImplicitSolidIO` now preserves actual restricted solid result topology
without numerical rebuilding. Its bounded schema-1 record reuses the original
shared cap/edge/wire source graph and stores exact component domains, explicit
wire assignments, face-to-shell and shell-to-material relationships. Structural
checks include canonical provenance/relationships, distinct opposite edge uses
and connected shell face graphs; they are NOT whole-field numerical validity.

Loading reconstructs genuine native solids, including separate islands/cavity
shells, but clears validity/attachment/accuracy flags and shell/solid Closed flags.
Source seam bounds remain unverified. Saved proof flags, display meshes and
transient solver state are never trusted. Export preserves source caches/flags;
manually freed child topology rejects before the source codec can freeze it.
The cut-face loader must add LOCAL source wires while the parent is forward,
then set the parent orientation: reversing first makes BRep_Builder compose the
wire orientation again. The both-operation tests caught and protect this case.

OCCT-8 FreeCAD exposes `dumps_mixed_solids`, `loads_mixed_solids` and the explicit
`source_mixed_solids` getter. Loaded handles/faces survive document closure;
the source getter returns the original boundary, not result solids. Explicit
rebuild from that source creates a new qualified result, not certification of
the old loaded topology. Existing boundary properties/Part APIs reject solid
handles. See [solid codec contracts](native-solid-topology-io.md).

This is a required persistence prerequisite, NOT a dedicated solid property,
FCStd solid owner or completed GUI integration. Next implement actual result
topology in a representation-aware ComplexGeoData/property/owner; do not present
the original SOURCE boundary as if it were that result or use a PropertyString
recipe as the final geometry property. Preserve component/face/wire/shell identity
and explicit unverified restore/recheck policy, transactions, placements and safe
consumer dispatch. Full Stage 2 curved operands/general restrictions and Stage 4
snapping/history/GUI requirements remain active. Verification is in PROGRESS.md.
Checkpoint: OCCT `0e6377579d`, FreeCAD `61f412c1fe`. Final suites passed
**76 kernel cases**, **42/42 migration**, **25/25 compatibility**. No dependencies
were installed and no system libraries changed. Build/test/debug commands and
logs are recorded in PROGRESS.md and the codec guide.

## Milestone 47: native edge/vertex selection and smooth field shading

The supported mixed geometry now exposes all EdgeN and actual VertexN elements
through ComplexGeoData, immutable mapped references and owned selected wrappers.
Edges follow the stored compound; vertices follow canonical forward endpoints
of those edges by OCCT identity, never coordinate welding or display poles.
Selected vertices provide one stored point, empty normals/lines and point bounds,
including once-applied rigid container/App::Link placement and retained lifetime.
The explicit OCCT-8 inspection API derives edge points, vertex points, endpoint
pairs and seam-to-edge aliases from the same graph.

The mixed renderer draws conventional boundary edges and actual point markers,
shares seam/edge alias nodes and preserves legacy Seam picks. ShowBoundaryEdges
and ShowVertices are persistent display-only switches. Clear/failure paths remove
their buffers and mapped highlights. The focused modelling workbench now offers
an explicit native-boundary selection demo (with surface preview), plus supported
selected-operand mixed commands, under its experimental native-boundaries menu.
The default 10 mm gyroid command is intentionally unchanged and is not yet the
trimmed/solid workflow. See [edge/vertex contracts](native-edge-vertex-selection.md).
The optional mixed-surface preview now uses analytical field-gradient vertex
normals rather than flat triangle lighting, retaining separate cap normals and
native edge creases. Geometry and all accuracy/solid-validation flags are unchanged.

Important precision finding: conventional BRep vertex text uses 15 significant
digits. Tiny vertex-coordinate round-trip differences are bounded by machine-scale
test reserves, not hidden by snapping to curve poles. Endpoint identities and
curve data remain checked separately. No bitwise vertex-persistence claim or
automatic solid validation follows; restored solid builds freshly check geometry.

Targeted CLI/property/reference/element/link/topology tests passed 6/6; targeted
camera/pixel/instance/visibility/editor/surface GUI tests passed 5/5. Full OCCT-8
suite passed 41/41 (295.33 seconds), including independent analytic-normal
checks and the updated GUI bindings/cache tests. Compatibility passed 25/25
(132.52 seconds); OCCT is unchanged and retains 73 kernel cases. A rendered
viewport capture was visually inspected before/after shading changes.
Logs are in PROGRESS.md. No OCCT/system-library/dependency edits were needed.
FreeCAD checkpoint: `9a154a3f91`; OCCT remains `b032b19ae2`.
Only existing minimal native targets were built with two compile/one link jobs;
do not rebuild libraries while runtime tests/audits are active.

Next: integrate real solid geometry with persistent properties/FCStd, extend the
primitive-to-native trim workflow including its current degeneracy restrictions,
then qualify snapping/history and representation-aware consumers. Curved operands,
general component restrictions and all unfinished Stage 2–4 gates stay in scope;
this milestone does not complete the full goal.

## Milestone 46: restricted native solids and focused primitive workbench

OCCT `b032b19ae2` adds `BuildNativeSolids`, gated by whole-field/native shell
topology, independently checked actual cap coverage in both directions, complete
piecewise-linear 3D curve/vertex deviations and bounded stored tolerances.
Inherited fresh trim correspondence bounds the seam separately; exact field
definitions and checked component domains remain authoritative. A private
segment spatial tree preserves nearest-distance and parity queries, resolving
the earlier segment-limit failure without increasing budgets. Failures never
publish solids. Separate islands/cavity shells are grouped by material region.
This is restricted numerical implicit-aware validation, not formal intervals,
generic UV BRepCheck/BOP or ordinary Part compatibility.

FreeCAD `94175dd569` exposes owned build/inspect/classify solid snapshots and
owned field faces. Source geometry/provenance and document-close lifetimes are
tested; restored boundaries are freshly checked, not trusted from saved flags.
There is no solid-property FCStd codec yet. Earlier FreeCAD `93a2bb06ac` adds
the focused Implicit Modelling workbench and saved dimension expressions for a
10 mm cube native field gyroid, with create/edit/undo/redo/reopen GUI coverage.
This command still creates the basic field feature, not a validated solid or
the finished crisp face/edge/vertex CAD workflow. The legacy workbench remains.

Full verification: **73 kernel cases** (86.01 seconds), **40/40 OCCT-8 migration**
(260.53 seconds), **25/25 compatibility** (134.06 seconds). Targeted native-solid
CLI/runtime **2/2** (12.95 seconds). Logs are in PROGRESS.md. GUI tests require
Xvfb; only existing minimal targets/dependencies were used, with no system changes.

Next: expose native boundary vertices/edges coherently in the modelling GUI,
integrate representation-aware solid properties/persistence and safe consumer
dispatch, while retaining the full Stage 2 curved-operand/general-restriction
and Stage 4 snapping/history/placement/editing requirements. The full goal remains
active; do not replace its completion criteria with this restricted checkpoint.

## Goal and repository layout

Build a FreeCAD fork with native implicit modelling, ultimately competitive
with nTop in field composition, lattices, performance, and interoperability.
The user's original brief is `instructions.txt`. Its initial Phases 1–3 have
working prototypes; later work added plane intersections and an in-tree native
FreeCAD module. Mixed operations in field space exist; manufacturing B-Rep
results and general native OCCT implicit-face operations are still future work;
restricted analytic sphere trimming/Boolean boundaries now exist.

| Location | Purpose |
|---|---|
| `FreeCAD/` | Actual FreeCAD fork; native work on `feature/native-implicit` |
| `OCCT/` | Native implicit faces/queries/restricted operations and signed-distance bridge on `feature/implicit-distance` |
| `include/implicit/`, `src/` | Standalone C++17 evaluator and numerical algorithms |
| `addon/ImplicitGyroid/` | Editable Python gyroid feature and retained Coin scene graph |
| `tests/` | Standalone, addon command-line/GUI tests and OCCT bridge probe |
| `benchmark/` | Standalone timing program and recorded CSV baseline |
| `FreeCAD/src/Mod/Implicit/` | Native module, field graph, extraction, Python API, smoke tests |
| `docs/` | Architecture findings, capability roadmap, build notes, API and this handoff |
| `build/` | Ignored standalone build; optionally registers native tests |
| `build-freecad-implicit/` | Ignored native development build |
| `build-freecad-occt8/` | OCCT 8 command-line/Part/Implicit/GUI and launcher demos validated |

The public umbrella repo is `kevinsmia1939/implicit-FreeCAD`. The FreeCAD fork
is `kevinsmia1939/FreeCAD`. The parent gitlink pins a commit, even though
`.gitmodules` also names the development branch. A recursive clone initially
checks submodules out at detached commits; switch FreeCAD to its development
branch before committing native changes.

## Decisions already made

Use a dual representation: native implicit fields own mathematical definitions;
OCCT owns conventional parametric B-Reps. Meshes are disposable display or
conversion results. Do not invent inconsistent `(u,v)` maps to make an implicit
field derive from `Geom_Surface`. Do not represent a mesh-only face as a fully
supported CAD surface. The architecture report and runnable OCCT probe document
why triangulation-only faces do not satisfy the surface adaptor contract.

Native support lives in `FreeCAD/src/Mod/Implicit` with build-system registration
and a native GUI provider. OCCT now has a core B-Rep signed-distance evaluator;
it supports mixed field workflows while native OCCT implicit face representation
and general Boolean dispatch remain a separate, larger milestone.

Licensing matches FreeCAD: project code is LGPL-2.1-or-later, with the same LGPL
2.1 license text copied to root `LICENSE`. Existing upstream and third-party
licenses remain applicable to their respective code.

## Completed work and evidence

1. Architecture investigation: `docs/architecture.md` compares native OCCT
   changes, external field ownership, and a local B-spline bridge. The executable
   `tests/occt_bridge_probe.cpp` demonstrates the mesh-only face limitation.
2. Standalone C++ engine: double-precision gyroid evaluation, analytical gradient,
   finite-difference fallback, normals, point classification, bounded domains,
   local projection, and sampled/refined ray roots. `tests/test_geometry.cpp`
   exercises these operations.
3. Display prototype: C++ and Python marching tetrahedra; editable Python FreeCAD
   feature with period, level, dimensions, placement, resolution, surface color,
   and shaded/wire/flat-line display. Coin nodes are retained on updates. FCStd
   restore rebuilds the cache. Tests exercised the installed command-line and
   GUI runtimes. This renderer is gyroid-only and separate from native graphs.
4. Bounded plane-patch contours: marching squares with refined roots in
   `src/PlaneIntersection.cpp`; cylinder tracing and arbitrary plane-box polygon
   clipping remain open.
5. Benchmark baseline: standalone evaluation, extraction time, triangle count,
   and estimated packed vector storage for 1, 3, 5, and 10 cells in
   `benchmark/results.csv`. Viewport FPS has not been measured.
6. Native foundation (FreeCAD commit `da2682dcd9`): `BUILD_IMPLICIT`, immutable
   fields, bounded `Implicit::Shape`, native `Implicit::Feature`, persistent
   primitive properties, and schema-1 JSON composition. Supported nodes:
   sphere, gyroid, union, intersection, difference, translate, offset. Python
   bindings evaluate, differentiate, classify, validate, get and set graphs.
   `native_smoke.py` covers equations, composition, invalid JSON and save/reload.
7. Native display extraction (FreeCAD commit `b7d88bca20`): C++ indexed
   marching tetrahedra with shared grid-edge roots, exact-zero grid-node merging,
   24 bisection steps per non-exact crossing, gradient normals, orientation from
   the inside convention, finite-domain clipping, deterministic traversal, input
   checks, and mesh budgets. `Implicit.tessellate` exposes local-coordinate
   vertices/normals/triangles and grid sample count. `mesh_smoke.py` checks a
   sphere and hollow sphere for paired oriented edges and Euler characteristic,
   vertex residuals, repeated results, gyroid extraction, open clipped boundaries,
   local Placement semantics, invalid resolution/domain, and FCStd restoration.
8. Native integration and operations (FreeCAD `db838e8cb7`, OCCT `1ce4cd29e4`):
   native GUI/workbench, transient compiled and display caches, conservative
   octree pruning, scalar batches, box/band nodes, capped band winding tests,
   OCCT signed-distance snapshots and mixed field composition; standalone
   cylinder contours. All ten combined tests pass. See `PROGRESS.md` for scope,
   performance observations and remaining stages.
9. Live-operation increment (FreeCAD `7d6ec00e18`, OCCT `711d0d0275`): schema-3 native feature links, placed/sign-
   normalized composition, direct Part operands and live B-Rep distance fields,
   automatic bounds, downstream cache invalidation, editable workbench primitives,
   Python extraction cancellation and GIL release. The OCCT evaluator reuses its
   fixed distance operand. All 13 combined tests pass, including the full mixed
   GUI path; both interactive demos pass checks. Part/PartGui/PartScripts are
   built. Viewport/intersection benchmarks are recorded; cylinder chart tolerance
   now accounts for radius. See `PROGRESS.md` for measured scope and limitations.
10. Background viewport extraction (FreeCAD `4615917cde`): opt-in per-view jobs on one bounded Qt
    worker, request coalescing, atomic cancellation, generation/snapshot-checked
    uploads, cache reuse, hidden/deleted/restored provider handling and status
    diagnostics. The combined suite passes 14 tests; both demos support
    `--background`. See `docs/background-meshing.md` for architecture and limits.
11. Native ray–field queries (FreeCAD `28dd78d22a`): `Shape::intersectRay` / `Implicit.intersectRay`
    query immutable compiled graphs with the GIL released, clip to the effective
    domain, sample sign brackets and refine by local distance tolerance. Results
    report distance, point, field residual and bracket width. Primitive, band,
    gyroid and live mixed Part roots are tested. See `native-ray-queries.md`.
    Tangencies/thin features may be missed; GUI picking is not yet wired up.
12. Stages 2–3 increment (FreeCAD `0048ed4f0a`): native local stationary projection with row-scaled KKT
    solve, bounded/backtracked steps, separate field/distance convergence checks
    and explicit failure statuses. A selected-field viewport probe handles world
    placement and orthographic/perspective cameras without relying on triangle
    hits; callbacks cancel on replacement, deletion and document close. Sampled
    mesh-quality inspection measures vertex/centroid/edge residuals and edge
    incidence/winding defects. This is not integrated semantic selection, true
    adaptive refinement, a nearest-point certificate or CAD solid validation.
    See `native-projection-and-quality.md`; the combined suite now has 16 tests.
13. Field-bound-controlled extraction: `Implicit.tessellateControlled` doubles
    common-grid resolution against a triangle-wide Lipschitz field bound,
    releases prior meshes between levels, supports cancellation through quality
    checks and returns explicit limit/empty-unverified statuses. This does not
    certify surface coverage or implement variable-resolution surface cells.
    All 17 tests passed (22.27 seconds). See `controlled-extraction.md`.
14. Native fixed-normal local charts and an experimental B-spline bridge:
    `Implicit.sampleChart` samples projected regular height patches;
    `ImplicitPatch.fit_patch` uses Part/OCCT interpolation and rejects failed
    sampled field/normal/domain checks. A genuine open B-spline face is tested
    with OCCT Common clipping and STEP round-trip. This is not a global atlas,
    closed/manufacturing solid or native implicit face. Reports retain source
    identity/revision/settings; returned faces are static local approximations.
    Cancellation/source-change checks are implemented. See `local-patch-bridge.md`.
    The complete configuration now registers 19 tests.
15. Richer scalar composition: constant and linear control fields,
    spatially varying level-set bands and polynomial smooth unions, including
    analytic gradients, conservative Lipschitz bounds, strict JSON validation
    and FCStd regression. See `scalar-controls.md`. Units remain caller-managed;
    this is not a physical-thickness or typed/shared-DAG implementation.
    All 20 tests passed. Controlled-extraction scaling at 1/3/5/10 gyroid cells
    is recorded in `benchmark/controlled_sampling.csv`; these cases hit the
    configured resolution-32 cap rather than falsely reporting bound success.
16. OCCT-native face foundation (OCCT `1205038a3e`): the user selected **0.1 mm geometric tolerance**
    and explicitly directed native OCCT face work. `Geom_ImplicitField` (sphere,
    gyroid), `BRep_TImplicitFace`, builder/tool representation dispatch and
    field-aware placed-coordinate queries now exist in OCCT 8.1. UV Surface
    reads/writes reject the subtype; ASCII/binary serialization rejects field
    loss. Eleven isolated conformance tests pass, including 0.1 mm ray brackets.
    This is storage/query support, not trimming/BOP/persistence/FreeCAD exposure.
    See `occt-native-faces.md` and `accuracy-policy.md`.
17. Explicit native single-face persistence (OCCT `df05db9b07`): `BRep_ImplicitIO` schema 1 stores
    exact sphere/gyroid definitions, local domain, raw tolerance, separate
    geometry/instance placements and orientation. Meshes/shape flags are not
    persisted. Unsupported custom/derived fields or child topology reject before
    output; malformed/oversized/trailing input leaves the destination unchanged.
    Fourteen isolated native-face cases now cover round-trips, transform and
    locale validation, I/O failures and retained 0.1 mm ray brackets. Generic
    BRepTools/BinTools, OCAF, FCStd and STEP persistence remain unsupported for
    these OCCT-native faces. The existing FreeCAD graph persistence is separate.
18. Restricted analytic native sphere operations (OCCT `4ba228482d`, FreeCAD `1ed832505a`): `BRep_TImplicitSpherePatch`
    retains a plane predicate and exact 3D circle wire. `BRep_ImplicitSphereOps`
    trims a sphere and computes common/fuse/cut boundary decompositions for two
    spheres, with shared transverse circle edges and source provenance. It
    rejects tangencies/uncertain cases and nested/non-sphere operations. Results
    are boundary face compounds, NOT validated solids or generic BOP support.
    A sphere-specific mesher reports a geometric interpolation bound at the
    0.1 mm target; no general gyroid/whole-model certificate follows.
    An isolated OCCT 8 worker and in-tree FreeCAD `ImplicitNative.py` adapter
    expose editable experimental sphere commands through transient Coin buffers.
    FCStd stores recipe properties and reconstructs geometry on restore, not
    native OCCT topology. At that milestone the GUI remained OCCT 7.9; no ABI mixing or
    native handles in Part. Nineteen native conformance cases and all 23 combined
    CTest entries pass (26.47 seconds in the final run). See `native-sphere-operations.md` for
    exact supported cases, build/try commands, toolchain boundaries and next gates.
19. Native gyroid-plane sections (OCCT `e5422e4ced`, FreeCAD `c94a5aae85`): `BRep_ImplicitPlaneSection` uses Lipschitz/
    Hessian bounds, unique edge roots and regular-cell connectivity, returning
    explicit unresolved diagnostics rather than guessed topology. Resolved
    sections carry a geometric cell-diagonal bound at the 0.1 mm target.
    `BRep_ImplicitGyroidOps::SplitZ` constructs opposite plane-mask faces sharing
    approximate closed seam edges only when the entire section resolves and
    every loop is closed. No open/domain-side seams, trim-conforming meshes,
    general mixed BOP, caps or validated solids are implemented.
    `ImplicitSection.py` previews native curves through the isolated worker and
    reconstructs recipe properties on FCStd restore. It is not a trimmed-surface
    GUI object or native OCCT 8 handles in Part. Independent analytic reference,
    saddle rejection, source immutability and split seam tests bring native
    conformance to 24 GTest cases; all 24 combined CTest entries pass (25.72
    seconds after the six-library rebuild triggered by changed module settings).
    FreeCAD now has strict source-build selection via `FREECAD_OCCT_BUILD_DIR`,
    with a tested migration preflight prohibiting system-library fallback.
    ApplicationFramework/DataExchange/Visualization now configure in the existing
    OCCT build. The user subsequently installed and confirmed `rapidjson-devel`;
    its header/package were verified and reconfiguration with `USE_RAPIDJSON=ON`
    succeeds, including `TKDEGLTF`. The required toolkit build completed all
    5,202 steps with exit code zero; the strict dependency preflight now passes,
    selecting only the OCCT 8.1 source-build libraries.
    Do not retarget the working OCCT 7 FreeCAD tree. See `native-gyroid-sections.md`.
20. Whole-FreeCAD OCCT 8 migration started (FreeCAD `a5257ddff3`): separate `build-freecad-occt8`
    configuration succeeds using the working build's minimal module options,
    two compile jobs and one link job. No additional packages were needed.
    The Implicit module now uses the signed-distance implementation exported by
    the fork's `TKTopAlgo` instead of compiling another copy into `Implicit.so`;
    the OCCT 7 compatibility path still stages/compiles only the extension.
    Both configurations succeed, and their generated Ninja graphs confirm this
    distinction. The first build stopped on legacy OCCT headers; milestone 21
    below records the fix and command-line validation. This does not expose
    native implicit faces through Part or implement missing trimming/BOP support.
    The existing combined suite passed 24/24 after the OCCT dependency build
    and both configurations (31.35 seconds); its FreeCAD tests used the
    working OCCT 7 runtime, not the then-unfinished migration binary.
21. OCCT 8 command-line migration validated (FreeCAD `189f6156d8`): FindOCC
    propagates the selected OCCT source manifest's install-only compatibility
    aliases through imported `TKernel`; it never searches installed headers.
    The removed `Standard_Byte.hxx` include is guarded for OCCT 7 in Part's
    umbrella header. `Part`, `Implicit`, scripts and `FreeCADMainCmd` now build.
    Bundled `pivy`/Coin must also be built in a fresh GUI-enabled configuration:
    otherwise Cmd startup reports an initialization traceback despite later
    printing a script success marker. Do not suppress these failures.
    The separate `tests/freecad_occt8` harness passes **12/12** (4.90 seconds).
    Its runtime fixture checks actual module paths, all 32 loaded OCCT libraries
    against the selected 8.1 build directory, a conventional Boolean and the
    kernel's signed-distance bridge. Eleven native smoke tests then cover field
    graphs, queries, extraction, FCStd, live Part links and local patch/STEP work.
    The fixture correctly rejects the OCCT 7 binary. The compiled alias-header
    probe also passes. The affected OCCT 7 targets rebuild successfully and
    the existing combined suite passes 24/24 afterward (35.61 seconds).
    The subsequent GUI build completed successfully (two compile jobs, one link
    job), log `/tmp/implicit-freecad-occt8-gui-build.log`. GUI validation follows.
22. OCCT 8 GUI migration validated (FreeCAD `c89d2217d8`): GUI library-path
    audit, synchronous/background/worker GUI tests and both launcher demos bring
    the separate migration suite to **18/18 passing** (42.59 seconds).
    PartGui/ImplicitGui, bundled Pivy/its extension and the actual Coin library
    resolve inside the migration build; all 32 actual OCCT libraries resolve
    inside the selected 8.1 build. The audit rejects the OCCT 7 GUI as a negative
    check. Tests cover camera/parameter changes, retained display buffers, live
    Part dependencies, invalidation/cancellation and FCStd restore. Preferences
    are isolated and rendering uses Xvfb/software OpenGL; no physical-GPU/FPS or
    all-workbench qualification is claimed. Optional unbuilt Measure commands
    can emit `Std_Measure`/`Std_MassProperties` warnings in this minimal build.
    The preserved OCCT 7 combined suite also passes **24/24** (25.61 seconds).
    Try `bash tools/launch_demo.sh --occt8`; add `--live-part --background` for
    the live mixed-field demo. The launcher defaults to the preserved OCCT 7
    build unless `--occt8` is selected. Native implicit-face Part exposure,
    general trimming/BOP and native topology persistence remain unfinished.
23. Explicit in-process native faces (FreeCAD `dfd157cba3`): the OCCT-8-only
    `_ImplicitOCCT` C++ module owns immutable native face snapshots. The public
    `ImplicitOCCT` facade checks Part's ABI/build root and extension origin before
    loading it. APIs expose sphere/gyroid construction, placed value/gradient/
    normal/domain/ray queries, translation, restricted sphere trim/Boolean
    boundaries, sphere/patch display meshes and explicit primitive schema-1 I/O.
    Handles are not Part shapes, cannot be pickled or assigned to Part Shape,
    and survive recompute/document close when retained. Trim/child-topology I/O
    rejects instead of losing geometry. No new OCCT source or package changes.
    `ImplicitNative` retains the default worker backend but adds explicit
    `Backend=InProcess`, with native handles and transient Coin meshes. A new
    OCCT-8-only workbench command creates an in-process sphere. FCStd stores
    recipe properties/backend and reconstructs faces, not pointer/mesh/native
    trim-topology payloads. Errors clear both caches; no silent worker fallback.
    Legacy schema-1 recipes retain Worker by default. Calls remain synchronous
    with the GIL held; this is not a native complex-geometry property or Part BOP.
    All **20/20** migration tests pass (50.92 seconds), including a worker-disabled
    GUI save/restore test and placement/reflection/lifetime/serialization guards.
    The preserved compatibility suite passes **25/25** (31.38 seconds), including
    a new public-import guard proving no OCCT 8 toolkit loads in the OCCT 7
    process. See `in-process-native-faces.md` for API/build/try commands and the
    next milestone: native gyroid sections/split ownership, topology persistence,
    conforming meshing and general consumer dispatch—not arbitrary Part assignment.

The native module and `FreeCADMainCmd` targets have been built successfully.
The unified CTest run passed **6/6 tests** on 2026-10-14, including both native
smoke tests; the mesh smoke test completed in approximately 0.44 seconds on this
host. That runtime is a regression-test observation, not a rendering benchmark.
That six-test result predates the current increments. The current combined run
passed **10/10 tests** before the live increment. The latest compatibility run passes
**25/25** with the isolated native-face executable, legacy ABI guard, worker GUI/protocol tests and migration preflight registered, including scalar
controls, chart/patch/STEP checks and synchronous/background mixed fork GUIs on Xvfb; see results
and benchmark observations in `PROGRESS.md`. OCCT is now modified. Its new extension is compiled against
installed OCCT 7.9.3 through isolated staged headers. Separately, the required
OCCT 8.1 modeling/exchange/OCAF/visualization toolkit dependencies are built and
the strict FreeCAD dependency preflight passes. The full upstream OCCT test suite
is not built or tested. The separate OCCT 8 command-line/Part/Implicit runtime
passes targeted command-line/GUI migration tests. General native-face Part
consumers, trim-topology persistence and broader workbench validation
remain pending; restricted native in-process ownership/query/primitive I/O is tested.
The earlier addon GUI tests used installed FreeCAD.

## Milestone 24: restricted native mixed-Boolean boundaries

Implementation heads: OCCT `459f71a3a9`, FreeCAD `dfb000b373`.

`TKTopAlgo/BRepClass3d_ImplicitBox` constructs native hybrid gyroid/Part-box
boundary compounds for Common and explicit box-minus-field subtraction, with
F<=0 inside. It validates/copies a conventional axis-aligned box, excludes
box-edge roots using Lipschitz subdivision, traces all six planar faces, builds
shared degree-1 B-spline seams with planar p-curves and retains conventional
caps. Caps pass BRepCheck and every compound edge has two opposite uses; this
does NOT validate a solid or enable generic UV-based BOP. At most one closed
contour per face, away from box edges, is supported. Open, nested, ambiguous and
unresolved cases reject; many low-isovalue gyroid cuts remain unsupported.
The default 0.1 mm bound applies to seams only, not a full surface mesh/model.

FreeCAD's OCCT-8-only binding owns the entire hybrid boundary in an opaque
capsule, plus an independently retained implicit face. Conventional planar caps
are exposed as Part face copies, not shared topology or implicit Part shapes.
`ImplicitMixed.py` adds live gyroid/Part links, Common/BoxMinusField commands,
a supported demo and FCStd recipe rebuild after explicit recompute. The GUI
shows planar caps and orange seams ONLY; no conforming gyroid mesh is claimed.
Failure clears native/display caches. Primitive fields must have identity global
placement; nonidentity parent containers/App::Link dispatch are unsupported.
See [native-mixed-box.md](native-mixed-box.md) for all support gates, API, demo,
build commands and acceptance tests. No new packages/system changes were needed.

The isolated native executable passes **27/27** GTest cases (three new mixed-box
cases; CTest 0.57 seconds). Migration CTest passes **22/22** (47.96 seconds),
including new CLI/GUI mixed-box checks. The preserved OCCT-7/root suite passes
**25/25** (26.32 seconds). Logs are in PROGRESS.md. Do not run library builds
concurrently with the actual loaded-library audit: a replaced Linux mapped inode
appears as `(deleted)` and correctly fails that audit.

## Milestone 25: shared box-edge roots and open-contour topology

Source heads: OCCT `ef20e3f9d7`, FreeCAD `1daddc4c3a`.

The active user goal is **proceed through Stage 4, seamless FreeCAD integration**,
using the six-stage overview, not the original prototype phase numbering.
`stage-4-acceptance.md` records the full scope. Keep that goal active: this
increment advances Stage 2 but does not finish Stage 2, 3 or 4.

`BRepClass3d_ImplicitBox` now validates the real conventional box geometry and
constructs fresh topology without mutating the input. It isolates transverse
roots on all twelve box edges once, creates canonical shared vertices, splits
retained box-edge intervals and maps adjacent face endpoints to those roots.
Every root must have exactly two face-contour uses. Directed boundary graphs
join open seams across faces into closed implicit trim wires; planar signed
loops/containment construct multiple cap regions and holes. Each cap passes
conventional BRepCheck and every compound edge must have two opposite uses.
The result remains a native boundary compound, NOT a validated solid.

Face tracing gets 3/4 of the requested tolerance; endpoint replacement is
limited to 1/8 and the combined seam bound must meet the target (default .1 mm).
Canonical edge brackets are <= tolerance/256. Numerical precision reserves and
ambiguity rejection apply; no distance-welding unrelated roots or geometry
perturbation. Grid/corner zeros, tangencies and exhausted budgets still reject.
The native plane-section algorithm adds Taylor empty-cell exclusion plus exact
harmonic uniqueness/range checks for field-axis edges; arbitrary oblique edges
retain the derivative-bound path. Bounded alternative uniform grids may return
a non-power-of-two resolution without exceeding MaximumCells.

FreeCAD exposes `box_edge_root_count` and a migrated/read-only
`BoxEdgeRootCount` diagnostic. `ImplicitMixed.demo(open_contours=True)` and a new
OCCT-8-only command create a standard iso-0 gyroid/box fixture. CLI/GUI tests now
require iso-0 edge crossings to succeed; low-resolution exhaustion remains a
failure/cache-clearing test. GUI output still previews caps/seams only.
Two new analytic/reference and multiple-region tests, plus level/placement/
degeneracy coverage, extend the isolated executable to 30 cases.
All **30/30** pass (CTest 2.34 seconds). Full OCCT-8 migration tests pass
**22/22** (51.04 seconds); preserved OCCT-7/root tests pass **25/25**
(29.80 seconds). Log paths are in PROGRESS.md. No new packages/system changes.

Next required work: trim-conforming surface meshing and native-solid
classification/validation, curved B-Rep operands and broader/nested/singular
topology coverage; then dedicated FreeCAD geometry property/topology persistence,
semantic selection/snapping, editing/undo/redo and cross-workbench dispatch.
See `native-mixed-box.md` for current limits and `stage-4-acceptance.md` for
the full completion audit. Do not replace that objective with box-only success.

## Milestone 26: conservative mixed-region classification

Source heads: OCCT `1ba27f551a`, FreeCAD `256ccafef0`.

The OCCT native box boundary now has an explicit mathematical-region classifier,
`BRepClass3d_ImplicitBox::Classify`. Common is box AND F<=0; BoxMinusField is
box AND F>=0. Physical input tolerance defaults to .1 mm, never compared directly
with raw gyroid residuals. Global Lipschitz and local gradient/Hessian Taylor
exclusions bound distance from the field boundary. Constraint bounds combine
as a minimum inside the intersection and maximum over violated constraints
outside. Inside/outside require separation greater than the tolerance.

Near-boundary requires a retained cap point or clipped opposite-sign root
bracket inside the tolerance ball, with bracket width <= tolerance/256.
Uncertain midpoint signs retain the bracket; critical/no-witness cases return
Unresolved, not guessed ON. Finite/trigonometric/coordinate precision guards
apply. These remain floating-point/reserve bounds, not directed intervals.

FreeCAD exposes `ImplicitOCCT.classify_mixed(boundary_handle, point, tolerance)`
and `obj.Proxy.classify(point, tolerance)` on the last successful mixed recipe.
Results include physical bounds/witnesses and `solid_validated=False`.
Retained snapshots survive edits/close; failed execution has no queryable cache.
No ordinary Part solid, whole-boundary validity, nearest point or topology
persistence claim follows. GUI remains caps/seams only. See
`native-region-classification.md` for contracts, examples and evidence.

Three new kernel cases cover both operations, cap/root witnesses, stationary
unresolved behavior, invalid inputs and a scaling regression keeping field
values fixed while changing physical distance classification. CLI/GUI tests
cover feature/API queries, restore, operation edits and snapshot/cache lifetime.
This is progress toward the active Stage 4 goal, not completion of Stage 3 or 4.
All **33/33** kernel cases pass (CTest 2.40 seconds); full migration **22/22**
(48.32 seconds) and preserved compatibility **25/25** (27.63 seconds) also pass.
Log paths are in PROGRESS.md. No dependencies/system libraries were changed.

## Milestone 27: explicit shared native topology persistence

Source heads: OCCT `aa6ced5047`, FreeCAD `eacfd7c7e6`.

`BRepClass3d_ImplicitBoxIO` schema 1 now persists the authoritative gyroid
primitive and a shared conventional cap/edge/wire graph, reconstructing a real
native implicit face with those same stored wires. No recipe retracing or fake
UV surface is used. The generic BRepTools payload contains caps and a wire
compound only, never an implicit face. Mesh/polygon caches are stripped from a
deep copy; source caches remain intact. Runtime state flags are normalized.

Read validates supported geometry, domain-bounded poles, closed wire connectivity,
planar cap validity, shared seam p-curves/identity, opposite edge uses and root
counts. Records are bounded to 16 MiB and 128 faces, with stricter primitive/
curve limits. Unknown, malformed, truncated, trailing, oversized and locked-
destination records reject; the destination is unchanged on failure. Primitive
I/O's nonempty identity chains are normalized only for exactly identity matrices.
Seam-bound metadata is stored provenance, NOT re-certified: newly traced results
have `SeamBoundVerified=true`, restored records always false. Solid validity is
still false. See `native-topology-io.md` for the complete contract and limitations.

FreeCAD exposes `ImplicitOCCT.dumps_mixed(boundary_handle)` and `loads_mixed(record)`
with owned immutable snapshots, cap copies, operation/inside convention, preview
curves and accuracy flags. Restored snapshots support native region queries after
document close. Primitive-only face serialization still rejects child topology.
CLI/GUI tests exercise the explicit API without recipe rebuilding. Two new
kernel cases cover both Boolean directions/open/closed graphs, shared identity/
p-curves, query equivalence, repeated I/O, error atomicity, locks and mesh omission.

This is only the explicit native codec: existing FCStd features still save and
rebuild recipes. The dedicated FreeCAD geometry property and FCStd topology/
migration integration remain required, as do curved operands, conforming meshes,
validated solids, semantic selection/snapping and broader Stage 4 consumers.
Do not mark Stage 4 complete from this checkpoint. No new dependencies/system
libraries were installed or replaced.
All **35/35** isolated kernel cases pass (CTest 3.17 seconds), **22/22** full
migration tests pass (46.09 seconds), **25/25** compatibility tests pass
(28.08 seconds). Log paths are in PROGRESS.md.

## Milestone 28: native complex-geometry property and FCStd topology

Source heads: FreeCAD `d2fec1801c`, OCCT unchanged at `aa6ced5047`.

The OCCT-8 build now registers C++ `Implicit::NativeGeometry` (ComplexGeoData),
`Implicit::PropertyNativeGeometry` (PropertyComplexGeoData), and a typed
`Implicit::NativeFeaturePython` owner. Geometry is owned in C++, not a proxy-only
capsule/string recipe or Part Shape. Property Copy/Paste support transactions;
FCStd schema-1 `.nativegeo` payloads preserve native math/shared topology.
New owners expose the standard GeoFeature geometry-discovery hook. Generic
surface/point sampling, subelement retrieval and nonidentity geometry transforms
reject explicitly; conservative bounds and stored topology lines are supported.

Mixed schema 2 keeps live inputs and a geometric fingerprint. Unchanged restores
refresh caps/seams from the property without numerical rebuilding; input edits
publish a new immutable snapshot. Stored math/bounds/operation/tolerance must
agree with unchanged inputs. Corrupt data does not silently trigger recipe
fallback; explicit Proxy.rebuild(obj) repairs from valid linked inputs. Source
errors that skip Python execution clear native-owner geometry via a C++ document
recompute hook. Accuracy provenance remains explicit: restored bounds are not
re-certified. `SeamBoundVerified` is visible as a read-only diagnostic.

Schema-1 owners migrate in place, gain the property and rebuild once, preserving
names/links; their original App::FeaturePython type retains legacy limitations
for core geometry discovery/dependency hooks. This property is currently for
native mixed gyroid/box results, not every implicit field/sphere/prototype.
Force-XML serialization is tested through XML-only Content and inline restore.
Property/object dump/restore and static cross-document object copy also retain
native topology. Interactive GUI clipboard workflows remain unqualified.

New `native_property_smoke.py` covers owner/property types and discovery, undo/
redo, invalid assignment atomicity, FCStd topology with rebuilding forbidden,
source edits, skipped-upstream-error invalidation, legacy migration, corrupt
payloads and readable but inconsistent stored math. The GUI mixed test checks
display restoration before recompute, with rebuilding forbidden. Expected
negative-test file/source errors are intentional. See `native-geometry-property.md`.
No OCCT source or system dependencies changed in this increment.

Full regression: **23/23** OCCT-8 migration entries (46.55 seconds), **25/25**
compatibility entries (27.66 seconds), including the unchanged **35** kernel
cases. Additional transfer checks pass in the fork's OCCT-8 FreeCADCmd.

The full through-Stage-4 goal remains active: curved operands, conforming surface
meshes, solid validation, semantic selection/snapping/history, richer editing,
legacy/container/placement and cross-workbench consumer qualification remain open.

## Milestone 29: native snapshot subelements and mixed-view picking

Source heads: FreeCAD `1a514006be`, OCCT unchanged at `aa6ced5047`.

Native ComplexGeoData now supplies owned one-based NativeFace/Cap/Seam segments,
boundary-line extraction and a first boundary vertex for native consumers.
Missing/foreign segments reject; surface meshing remains explicitly unsupported.
`Implicit.nativeGeometry(obj)` returns an owned immutable geometry snapshot via
the standard ComplexGeoData Python interface, usable after document close.
Preview seam order now follows unique native trim-wire edges instead of tracing
sections, matching C++ subelement order before/after topology I/O.

Mixed-view caps/seams render as individual Coin shapes, with semantic pick names
and whole-node detail paths rather than disposable triangle/segment indices.
GUI tests exercise actual ray hits, selection registration, restore, invalid
names and selection/node clearing on edits/failure. Snapshot names are indexed,
not history-stable; GUI selections clear on replacement. Persistent external
LinkSub history, snapping, implicit-surface picking and visual highlight pixel
qualification remain open. See `native-geometry-property.md` for the contract.
No OCCT source, dependencies or system libraries changed.
Both incremental builds succeed. Full OCCT-8 migration passes **23/23**
(46.70 seconds); preserved compatibility passes **25/25** (27.41 seconds),
including unchanged **35** kernel cases. Logs are recorded in PROGRESS.md.

## Milestone 30: mapped native snapshot references and stale-link rejection

Source heads: FreeCAD `e036a6855a`, OCCT unchanged at `aa6ced5047`.

Native geometry now reconstructs a standard FreeCAD element map from a SHA-256
digest of its mesh-free topology record plus native subelement index/type.
Unchanged payloads preserve names through FCStd/content transfer, copy and
undo/redo. Changed/cleared geometry invalidates old mapped references instead
of falling back to a still-valid CapN/SeamN index. Exact topology restoration
can recover them. Typed native owners validate core subobject lookups; the mixed
view resolves current mapped paths and rejects stale ones.

New native_reference_smoke covers all multi-face fixture subelements, recursive
copy of a real LinkSub target, FCStd restoration, changed/cleared geometry,
saved unresolved links, undo/redo and exact recovery. Expected missing-element
warnings are intentional. Native Python subobject extraction remains None,
never an implicit Part shape; nativeGeometry is the explicit inspection seam.

This is conservative whole-snapshot invalidation, NOT surviving-element Boolean
history or geometric equivalence matching. Plain indices are still snapshot-
local. Legacy owners, broader consumers, external-document/container/placement
references, snapping and all unfinished Stage 2/3/4 gates remain open.
The native OCCT codec/property schema is unchanged; maps reconstruct from stored
geometry rather than introducing another authoritative payload. No OCCT source,
dependencies or system libraries changed. See native-geometry-property.md.
Both incremental builds succeed. Full migration passes **24/24** (49.60 seconds)
and compatibility **25/25** (27.85 seconds), including unchanged **35** kernel
cases. Logs and the expected negative-test warning are recorded in PROGRESS.md.

## Milestone 31: owned native geometry through core subobject extraction

Source heads: FreeCAD `1f46ce6988`, OCCT unchanged at `aa6ced5047`.

Typed native owners now return C++ NativeElementGeometry wrapped by the standard
ComplexGeoData Python interface from getSubObject. SelectionEx.SubObjects now
contains selected native cap/seam geometry, not None/Part shapes. Each owned
result exposes Element1 with the source mapped identity, boundary lines and
bounds; authoritative native math/topology survives source replacement/close.
Native face bounds use the full finite domain, not trim-line extrema.

Subobject geometry is pre-placed once using the accumulated matrix and optional
owner Placement; its reported geometry transform is identity. CLI tests cover
all fixture subelements, independent rotated/translated expectations, raw
transform=False queries and nested App::Part paths. Scales/reflections,
projective/nonfinite transforms, nonidentity mutation, independent persistence,
surface sampling and assignment to Part Shape/native property reject. The C++
first-boundary-vertex hook follows the same transform; integrated snapping is
still open. GUI tests inspect both cap/seam SelectionEx native SubObjects.

This is a representation-aware extraction seam, NOT complete consumer/container/
App::Link qualification, implicit surface meshing, validated solids or Boolean
history. All unfinished Stage 2/3/4 gates remain active. No OCCT source, packages
or system libraries changed. See native-geometry-property.md for usage/contracts.
Both incremental builds succeed; migration passes **25/25** (51.79 seconds),
compatibility **25/25** (27.54 seconds), including unchanged **35** kernel cases.
Additional direct CLI bounds/composition checks pass; logs are in PROGRESS.md.

## Milestone 32: whole native geometry and rigid App::Link paths

Source heads: FreeCAD `6f93521083`, OCCT unchanged at `aa6ced5047`.

Empty native getSubObject references now return owned NativePlacedGeometry,
retaining NativeFace/Cap/Seam counts/maps, pre-placed boundary lines and conservative
domain bounds. Null properties remain None. Rigid validation/bounds helpers are
shared with selected-element geometry; surface meshes/nonrigid transforms remain
unsupported. This closes the previous missing whole-object extraction seam.

New native_link_smoke checks both LinkTransform modes, every native subelement,
App::Part-to-link paths, mapped cap links, same-document FCStd restoration,
retained lifetime, scale rejection and source clearing. External-document queries
and instance-file restoration are tested with the source already loaded; returned
map re-tagging does not mutate source maps. GUI tests require a real cap ray hit
on a translated/rotated native link, placed selected geometry, and whole-object
extraction from a standard selection. Whole selections normally have empty
SubObjects; use the selected Object.getSubObject("").

This does not qualify automatic external dependency loading, missing/moved files,
link arrays, all instance/container/workbench modes, editing or placed operands
in the Boolean kernel. All unfinished Stage 2/3/4 gates remain active. No OCCT
source, dependencies or system libraries changed. Contract: native-geometry-property.md.
Both incremental builds succeed. Full migration passes **26/26** (52.25 seconds)
and compatibility **25/25** (37.32 seconds), including unchanged **35** kernel
cases. Logs and initial test-fixture corrections are recorded in PROGRESS.md.

## Milestone 33: representation-aware placed native region queries

Source heads: FreeCAD `e88c92efaa`, OCCT unchanged at `aa6ced5047`.

`Implicit.classifyNative(wholeGeometry, point, tolerance=.1)` now dispatches to
the existing native mathematical-region classifier through local/pre-placed
ComplexGeoData, including supported container/link paths. Selected Element1
geometry rejects as a non-region. `solid_validated` stays False.

Placed queries include inverse/forward residual and coordinate/domain floating-
point reserves, conservative near-rigid stretch bounds, lower-bound shrinking
and transformed witness upper bounds. Local tolerance reserves 1024 placement
errors for the /256 bracket policy; 2048 errors exhausting the requested budget
return unresolved with placement_resolved=False. This is ordinary floating-point
engineering/reserve arithmetic, NOT directed intervals or solid/model validation.

Tests cover both Boolean regions, independent cap-distance/witness checks,
gyroid root brackets/signs/domain, stationary unresolved points, snapshots,
container/link poses, FCStd restore, lifetime, invalid/selected representations
and extreme-coordinate precision refusal. GUI checks a placed whole-instance
query. A first root test exposed the need to reserve endpoint-transform error
inside the local bracket budget; the implementation now does so explicitly.
Static snapshots are used for nested classification because grouping live mixed
recipes may move inputs outside the kernel's identity-placement contract.
General placed/curved operands, conforming surfaces/solids and remaining Stage 4
history/snapping/editors/consumers remain required. No OCCT source, packages or
system libraries changed. Details: native-geometry-property.md.
Both incremental builds succeed; full migration passes **27/27** (55.33 seconds)
and compatibility **25/25** (28.21 seconds), including unchanged **35** kernel
cases. Additional direct analytic/witness tests pass; logs are in PROGRESS.md.

## Milestone 34: transactional native mixed-boundary task editor

Source heads: FreeCAD `f35700d659`, OCCT unchanged at `aa6ced5047`.

MixedBoundaryView now supports tree double-click/standard edit mode with a Qt
task panel for operation, seam tolerance (mm) and tracing budget. Source links
are read-only. Controls stage values; Apply previews in one owned transaction,
OK applies/commits and Cancel restores prior properties/native geometry. Failed
numerical previews clear geometry/display, show a handled error and stay open;
corrected previews recover. Successful previews run normal recompute after
publishing the snapshot. Failed previews remain pending recompute, not a claim
that direct Python execution sets all C++ document error flags.

Real GUI tests cover standard Edit/double-click, actual Apply/Cancel/OK buttons,
selection and region queries, mapped identity on cancel, immutable lifetime,
commit/undo/redo, failure/recovery, reset-edit rollback, unrelated transaction
guards, accepted FCStd reopen/re-edit and document close with active preview.
Transaction actions are scoped by ID; normal actions refuse a foreign active
transaction. Forced foreign-transaction framework operations, concurrent edits,
active-preview saves and individual input deletion remain unqualified.

This is a three-setting editor for the restricted mixed recipe, not a graph/
equation editor, source-link replacement or completion of geometry/history/
snapping/consumer gates. All unfinished Stage 2/3/4 scope remains active.
No OCCT/C++ source, packages or system libraries changed. Build only
ImplicitScripts; see native-mixed-editor.md for usage and test contracts.
Both script targets refresh successfully. Full migration passes **28/28**
(56.20 seconds), compatibility **25/25** (27.78 seconds), including unchanged
**35** kernel cases. Final task-only checks pass **3/3** (7.04 seconds), adding
real buttons, open contours, multi-preview undo and failed-preview cancel.
Logs are recorded in PROGRESS.md.

## Milestone 35: shared native cap/seam meshing constraints

Source heads: FreeCAD `8a4dd8ef32`, OCCT `f20e7c4b63`.

`TKMesh/BRepMesh_ImplicitBoundary::Prepare` now triangulates conventional planar
caps on a validated private topology snapshot. Canonical nodes use shared
vertices and edge-sample order, never position welding. A private exact-polyline
discretizer retains every native knot; ordinary chordal tessellation initially
dropped short knots and failed the new correspondence checks. Opposite cap/
native wire incidences are checked before returning, and source caches/flags
remain untouched. TKTopAlgo has no new TKMesh dependency.

The OCCT-8-only FreeCAD API `ImplicitOCCT.prepare_mixed_mesh(obj.Geometry)`
returns local vertices, outward indexed cap triangles, shared seam indices and
oriented closed wire indices. It preserves seam-bound provenance and explicitly
reports native_surface_meshed=False / solid_validated=False. No implicit face
is passed to generic UV meshing. GUI output remains caps/seams only.
See native-mesh-constraints.md for limits, errors and the exact API.

Two new kernel tests bring the isolated executable to 37 cases. CLI regression
covers both operations and open/closed contours, deterministic buffers,
property/capsule equivalence, restored provenance, invalid budgets, source
immutability and retained snapshots after document closure. Kernel CTest passes
1/1 (37 GTests, 4.12 seconds), full migration 29/29 (64.67 seconds) and
compatibility 25/25 (28.69 seconds). No dependencies or system libraries changed.
Build commands and logs are recorded in PROGRESS.md and native-mesh-constraints.md.

This is a Stage 3 preparation milestone, not completion of a conforming surface
mesh, native solid or the overall goal. Next implement the implicit surface
mesher using these canonical constraints, then validate the complete boundary.
Curved B-Rep operands and all unfinished Stage 2/3/4 gates remain required.

## Milestone 36: trim-conforming native surface candidate and opt-in GUI

Source heads: FreeCAD `ef5c5f283e`, OCCT `3ca2e7235e`.

`TKMesh/BRepMesh_ImplicitBoundary::Tessellate` now extracts a gyroid surface
candidate, attaches an oriented collar to canonical trim wires, refines shared
interior edges with bounded field projection and checks closed opposite edge
incidences plus cyclic vertex links. Cap/wire nodes are never moved or welded.
The preparation discretizer now preserves every knot AND bounds segment sample
spacing by capDeflection/4, with a conservative preflight sample budget.

Aspect-balanced dyadic grids have a longest-axis maximum of 256 and a hard
2.2-million-sample limit. Loop matching/collar pairs, nodes, total triangles and
eight projected-refinement rounds have explicit budgets. An initial uniform
collar failed the wider two-loop fixture: balanced sampling alone did not solve
it; shared projected refinement and finer fixed seam sampling now do. Retry
diagnostics retain each failed resolution's reason. No generic UV mesher sees
an implicit face, and source math/topology/caches remain untouched.

`ImplicitOCCT.tessellate_mixed(obj.Geometry)` returns cap/implicit triangles
sharing zero-based vertices. A triangle-wide interpolation/Hessian witness
bounds distance to the UNTRIMMED field, NOT clipped-root distance, two-sided
Hausdorff error or surface coverage. MeshClosed is combinatorial only;
SurfaceCoverageVerified and SolidValidated remain false. No self-intersection
or whole-solid certificate follows. See native-conforming-preview.md.

The GUI View property ShowImplicitSurface enables the candidate explicitly.
It uses those same constrained cap triangles, native NativeFace1 picking/detail
paths, transient buffers and unchanged-snapshot/settings cache reuse. Numerical
preview errors clear the surface/report a status but preserve valid math and
ordinary caps; native-geometry failure clears all previews. FCStd saves settings
and native topology, never mesh buffers. Generation remains synchronous, without
cancellation; broader consumers/placements and solid validity remain required.

Three additional kernel cases bring the executable to 40 passing GTests,
including both operations, open/closed contours, multiple loops, physical
scale/translation, independent sampled root witnesses, immutability and failures.
CLI and actual GUI tests cover buffers/lifetime, native-face ray picking,
cache reuse, edit/restore and failure/recovery. Current full verification logs
are in PROGRESS.md: migration **30/30** (96.24 seconds), compatibility **25/25**
(48.58 seconds); isolated kernel CTest **1/1** (40 GTests, 12.11 seconds).
TKMesh, the isolated executable, the native binding and both script targets
build successfully. No packages/system libraries changed.

Next: coverage and clipped-boundary consistency, embeddedness/self-intersections
and implicit-aware solid validation. Curved operands, history/snapping and
remaining cross-workbench Stage 4 requirements are still in scope. The goal is
not complete; this candidate is not a validated native solid.

## Milestone 37: exact transient triangle-contact audit

Source heads: FreeCAD `836b609498`, OCCT `72b5a40c8d`.

`TKMesh/BRepMesh_ImplicitBoundary::Audit` now audits all cap/implicit triangle
pairs, including shared-vertex/edge pairs. Bounded AABB sweeping feeds exact
separating-plane predicates on stored binary64 coordinates. A local signed
integer implementation uses 32-bit words/64-bit carries and common dyadic
scaling; no new dependencies or floating epsilon predicates. Common-node
contacts are allowed only on the entire common simplex. A new same-side overlap
test caught/fixed a first implementation that did not require the separating
plane to contain the whole shared edge.

The explicit OCCT-8 FreeCAD API `ImplicitOCCT.audit_mixed_mesh(obj.Geometry)`
privately regenerates/audits a candidate. Reports distinguish clear,
contact_or_unresolved and limit; all unfinished/limited scans have clearance
False. Caps precede implicit triangle IDs. Arithmetic exponent/work budgets,
source immutability and snapshot lifetime have regression tests. Both Boolean
directions in closed-loop and box-edge-crossing fixtures pass; the iso-0 Common
case examines 906,095 exact pairs. See native-mesh-contact-audit.md for bounds,
API, proof scope and unresolved cases.

This is exact mesh-only contact evidence, not implicit coverage, clipped-root
consistency, native-solid validity or ordinary Part consumer support. The GUI
does not automatically run/claim the audit, and authoritative geometry remains
unchanged. Broader geometry, singular coverage and consumer/history/snapping
gates remain active. Next: authoritative-field coverage/clipped-boundary
validation, then native-solid construction/validation. Full verification logs
are in PROGRESS.md: **42 GTests**, CTest **1/1** (20.03 seconds), migration
**31/31** (121.03 seconds), compatibility **25/25** (45.43 seconds). GCC UBSan
passes the targeted exact-predicate case; only the new audit/test source is
instrumented, not all OCCT. TKMesh, isolated tests, native binding and both script
targets build successfully. No packages/system libraries changed.

## Milestone 38: whole-domain clipped-boundary proximity

Source heads: FreeCAD `d267f58f7a`, OCCT `18ed5c0a52`.

`TKMesh/BRepMesh_ImplicitBoundary::CheckCoverage` now partitions the complete
box/retained faces using field exclusion or mesh-point upper distance plus cell
radius. Reverse triangle patches use correct-sign planar regions, fully clipped
normal-line slabs, native-classifier brackets and actual retained cap witnesses.
The first cap failures exposed a 3D-gradient search leaving the box; projected
cap gradients/in-plane directions now provide actual clipped witnesses.

`ImplicitOCCT.check_mixed_coverage(obj.Geometry)` privately generates/checks a
candidate. Both global bounds are valid ONLY when boundary_proximity_resolved
is True; failures contain partial diagnostics and a failed patch/cell. Numerical
reserves, NOT directed intervals or topology equivalence. Source math/topology/
caches, GUI buffers and stored seam-bound provenance remain unchanged.

Three new kernel cases (45 total) cover both operations/open and closed contours,
independent harmonic roots/raw cap samples, one missing disconnected component,
missing whole surfaces, extra interior triangles, forged flags and limits. CLI
restore/lifetime/immutability checks pass. Final regression: 45 kernel cases
(37.18 seconds), migration 32/32 (182.37 seconds), compatibility 25/25 (69.67
seconds). Existing targets/dependencies reused; no packages/system changes.
See native-clipped-coverage.md and PROGRESS.md for API, budgets, proof scope/logs.

Next: regularity/stratified trim correspondence, topology/feature-size/reach
conditions and native-shell/solid construction/validation. Two point sets within
.1 mm need not share topology. Curved operands and unfinished Stage 4 integration
remain required; the overall goal stays active.

## Milestone 39: mesh-independent box-stratified field regularity

Source heads: FreeCAD `094407d71f`, OCCT `fd9a53d063`.

TKTopAlgo now provides `BRepClass3d_ImplicitBox::CheckRegularity` for an
identity-placed native gyroid's whole box domain. Adaptive cells must exclude
F=0 or bound the projected gradient away from zero. Volume, six faces, twelve
edges and eight corners are checked independently; corner intervals must exclude
zero. Derivative bounds are in field/model-length units, not distance errors.
No TKMesh dependency, tracing, geometry mutation or mesh metadata is involved.

OCCT-8 FreeCAD exposes `check_mixed_regularity(boundary_handle)` and
`check_box_regularity(native_face_handle)`, with per-dimension completion,
quantitative bounds/work and unresolved-cell diagnostics. Limits stay unresolved.
Trim correspondence, topology and solid validation flags remain False. The raw
face adapter checks the BOX DOMAIN and intentionally ignores stored trim wires.
See native-box-regularity.md for formulas, units, precision and API limits.

Three new cases bring the kernel suite to 48. Analytic critical examples
separately fail volume, face, edge and corner strata, even where higher-dimensional
gradients remain regular. Both Boolean directions/open and closed contours,
restore/immutability, model-length scaling and periodic translations pass.
Native CLI adds handle/placement guards, raw/mixed adapter equality, restore,
limits and lifetime. Final regression: isolated kernel **48 cases**, CTest **1/1**
(36.31 seconds), migration **33/33** (171.99 seconds), compatibility **25/25**
(67.86 seconds). Build/test logs are in PROGRESS.md.
No packages/system libraries changed; existing targets/dependencies are reused.

Next: establish a bijection between stored trim-wire graphs and certified face
contours/canonical box-edge root neighborhoods; reject ambiguous pairing rather
than infer correspondence from proximity. Topology/reach conditions and native
shell/solid construction/validation remain required. Curved operands and all
unfinished Stage 4 integration gates remain active; this is not solid validity.

## Milestone 40: stored native trim-graph correspondence

Source heads: FreeCAD `6a24569e8d`, OCCT `ec71be2ccd`.

TKTopAlgo now provides `BRepClass3d_ImplicitBox::CheckCorrespondence` and the
OCCT-8 FreeCAD API `ImplicitOCCT.check_mixed_correspondence`. A private validated
copy is compared with a newly traced canonical graph from authoritative math,
after stratified regularity. Unique continuous PWL curve matches induce shared
vertex bijections; cap grouping and oriented cyclic wires must agree. Planar
p-curves must match 3D edge parameters under tight bounds; derivative bounds
exclude backward seam segments. Resampling, shuffled caps and cyclic wire starts
are supported. No welding, geometry replacement or source-bound recertification.

See native-trim-correspondence.md for scope, budgets and tolerance policy.
Four new kernel cases bring the suite to 52. Native CLI tests cover both
operations/open and closed contours, restore/lifetime/provenance and limits.
Final regression: kernel **52/52** (CTest 41.16 seconds), OCCT-8 migration
**34/34** (169.72 seconds), compatibility **25/25** (70.03 seconds). Existing
build targets and dependencies reused; no packages/system libraries changed.
The check explicitly keeps topology and solid validity False. Full surface
isotopy/reach and native shell/solid validation/construction remain required;
curved operands and unfinished Stage 4 requirements remain active.

The next native-solid gate must relate the WHOLE field surface to a topology
model, not just show two nearby point sets or equivalent trim graphs. Establish
a bounded local-graph/homotopy construction with compatible cell-boundary
incidences, or equivalent quantitative feature-size/reach conditions, before
promoting any candidate to a validated native shell/solid. Existing regularity,
contact, proximity and graph checks remain independent prerequisites, not a
substitute for that missing relation.

## Milestone 41: whole-box numerical homotopy and topology reference

Source heads: FreeCAD `9874d2e306`, OCCT `3f9aa48bcf`.

TKTopAlgo now provides `BRepClass3d_ImplicitBox::CheckTopology`, exposed in OCCT-8
FreeCAD as `check_box_topology` and `check_mixed_topology`. A shared Freudenthal
nodal-linear field is compared to F through the whole convex homotopy. Cells
must exclude homotopy zeros or prove positive gradient alignment; actual box
faces/edges have their own projected checks. Internal grid boundaries instead
share the increasing continuous field-gradient direction, avoiding artificial
grid-transversality requirements. Engineering reserves, not directed intervals.

The transient affine-root reference has shared identities, oriented triangles,
component Euler characteristics and directed boundary loops; incidence and
path/cycle vertex links check its manifold structure. It is NOT a .1 mm mesh,
stored native graph or solid. Source topology/caches/provenance remain unchanged.
Five new cases bring the kernel suite to 57, including a hidden component missed
by every coarse grid sample, enclosed sphere topology, empty fields, critical
strata, orientation, scaling, immutability and limits. CLI adds both operations,
raw/mixed adapters, restore/lifetime/guards and independent buffer accounting;
the larger fixture has Euler -1/three loops at resolutions 32 and 64.

See native-box-topology.md for the analytical rationale, API and budgets. Next
connect this whole-surface reference to stored native trims/caps and their
geometric realization, with component grouping/orientation, then native shell/
solid construction and validation. Equal loop counts/independent True flags do
not establish that missing relation. Curved operands and all unfinished Stage
2/4 requirements remain active. No packages/system libraries changed.

Final verification: kernel **57/57** (CTest 43.77 seconds), OCCT-8 migration
**35/35** (187.84 seconds), preserved compatibility **25/25** (74.48 seconds).
Targeted runtime/topology CTest **2/2** (7.88 seconds). TKTopAlgo/TKMesh, the
isolated test executable, native binding and both script targets built using
existing dependencies. Build/test logs are recorded in PROGRESS.md.

## Milestone 42: closed region reference and material connectivity

Source heads: FreeCAD `7351987bdb`, OCCT `9ee814feb0`.

TKTopAlgo `CheckRegionTopology` now completes the whole-box homotopy reference
with retained planar caps on the same exterior Freudenthal triangles. Caps share
existing affine-root IDs and retained grid vertices; source surface buffers are
preserved as prefixes. Closed edge incidences/cyclic vertex links, Euler/component
labels and cap orientation are checked without a TKMesh dependency.

Retained vertex connectivity across the tetrahedral 1-skeleton groups connected
VOLUME regions. Every boundary triangle/shell must attach consistently and every
region must have a boundary. This distinguishes outer/cavity shells of one region
from disconnected islands; signed volumes are diagnostics, not the grouping rule.
Translated long-double volume sums describe the linear reference ONLY, not F's
physical volume or .1 mm accuracy. No arbitrary mesh is promoted into CAD faces.

OCCT-8 FreeCAD exposes `check_box_region_topology` and
`check_mixed_region_topology`. Queries preserve source math/topology/caches and
seam provenance; native-shell/solid, geometry-accuracy and stored correspondence
flags stay False. Three new cases bring the kernel suite to 60, including both
operations/open and closed native trim fixtures, complementary reference volumes,
cavity versus multiple-island grouping, full/empty/critical/resource cases and
source immutability. The cavity reference additionally passes the separate exact
binary64 mesh contact audit. CLI independently checks buffers, Euler/volumes,
adapter/prefix equality, restore/lifetime and two-cavity/one-region grouping.

See native-region-topology.md for API, connectivity rationale and limits. Next
establish actual stored native trim/cap attachment to this reference, not just
equal counts or independent True flags, then construct/validate native shells
and solids. Curved operands and unfinished Stage 2/3/4 gates remain required;
the full goal stays active. Existing build/dependency directories are reused;
no packages/system libraries changed. Rebuild _ImplicitOCCT after the changed
transient C++ witness layout; this is not a persistence schema change.

Final regression: kernel **60/60** (CTest 46.44 seconds), OCCT-8 migration
**36/36** (219.92 seconds), compatibility **25/25** (85.85 seconds). Targeted
runtime/region CTest **2/2** (12.19 seconds). Build/test logs are in PROGRESS.md.

## Milestone 43: stored conventional cap-region attachment

Source heads: FreeCAD `81cf92e7da`, OCCT `2e5bc37269`.

TKTopAlgo `CheckCapAttachment` first requires fresh stored-graph correspondence
and a resolved closed region reference. Reference cap triangles group by shared
edges on each physical box side. A bijective mapping then needs actual common
retained F/L points, conventional source-cap IN classification and clearance to
all source cap polylines beyond fresh seam/curve/p-curve uncertainty. No nearest-
cap pairing, coordinate welding, source replacement or trusted stored flags.

OCCT-8 FreeCAD `check_mixed_cap_attachment` returns source-cap-order reference
cap/shell/material labels, witnesses, clearance bounds and bounded work. Labels
are transient, not topology history. Calls remain explicit/synchronous and
preserve source geometry/caches/provenance. Missing witnesses, ambiguity and
limits stay unresolved. Numerical engineering reserves, not directed intervals
or a formal isotopy certificate. Trim-wire/native-topology/solid validity and
native-shell-construction flags remain False.

Two new cases bring the kernel suite to 62, covering both operations/open and
closed contours, restored provenance, actual point classification/signs,
multiple material regions, reordered caps, immutability and limits. Changed math
with forged seam flags must stop at graph validation. CLI adds independent Part
cap/edge distances, raw snapshot lifetime, restore and settings/handle guards.
See native-cap-attachment.md for the exact policy and budgets.

Next attach stored trim-wire/contour identities and the native implicit face to
reference components, including distinct holes on one cap, then construct and
validate native shells/solids. Cap-region mapping alone is insufficient. Curved
operands and all unfinished Stage 2/3/4 gates remain active. Reused existing
targets/dependencies; no packages/system libraries changed. The OCCT manifest
was explicitly reconfigured for the new translation unit. No persistence schema
change or authoritative field/mesh substitution.

Suggested next implementation: construct the opposite-operation canonical cap
graph privately from the SAME field/domain and apply cap attachment to it too.
Identify a seam arc by its physical side, retained/complement cap-region pair
and ordered canonical box-edge-root anchors; closed face contours need the
region pair because one retained cap can contain multiple holes. Compare full
directed native/reference wire cycles and reject any nonunique pairing. This is
a proposed next step, not implemented evidence. Do not infer implicit-face
component attachment or native-solid validity merely from these cap labels.

Final verification: kernel **62/62** (CTest 56.93 seconds), OCCT-8 migration
**37/37** (219.58 seconds), preserved compatibility **25/25** (98.39 seconds).
Targeted runtime/cap CTest **2/2** (12.75 seconds). Build/test logs are in PROGRESS.md.

## Milestone 44: dual-cap contour identities and full trim-wire attachment

Source heads: FreeCAD `88253649f0`, OCCT `ee9f07f4a2`.

TKTopAlgo `CheckTrimAttachment` privately constructs the complementary canonical
boundary from the same field/domain and attaches both cap sets. Their shared
linear-reference roots match by originating grid-edge IDs. Retained/complement
cap-region pairs plus ordered physical box-edge roots identify contour arcs;
closed contours use their dual region pair and signed planar winding. Native
seams match complementary traces by continuous PWL correspondence, then complete
directed cyclic native/reference wires must match bijectively. No nearest-point
pairing, root welding, persistent-label claims or source mutation.

OCCT-8 FreeCAD exposes `check_mixed_trim_attachment`. Seam labels follow unique
native edge traversal; wire mappings follow direct native wire order. Reference
triangle connectivity reports which surface component each wire borders, but
does not attach or split the entire native face. Whole-face/native-topology/
shell/solid validity flags stay False. See native-trim-attachment.md for budgets,
precision policies and exact scope.

Three new kernel cases bring the suite to 65: both operations, three-wire open
fixtures, two holes on one retained cap, separate surface components, restore,
source immutability and limits. Resampling/cyclic starts pass this checker too;
forged math stops at graph/cap validation. CLI adds both contour/operation cases,
actual source-record equality, complete array bijections, restore/lifetime/guards
and a two-hole/two-component fixture. Existing build/dependency directories are
reused; no package/system-library or persistence-schema changes. Rebuild the
native binding after the enlarged transient CapAttachment result layout.

Next attach the whole native implicit face to reference components, including
empty/enclosed components and proper restriction/splitting when multiple
components occur. Then construct actual native shells/material solids and apply
representation-aware validation. Do not create native faces from reference
triangles or infer face-interior validity from matched wires. Curved operands
and every unfinished Stage 2/3/4 requirement remain active.

Final verification: kernel **65/65** (CTest 52.37 seconds), OCCT-8 migration
**38/38** (178.21 seconds), compatibility **25/25** (80.61 seconds). Targeted
runtime/trim CTest **2/2** (13.05 seconds). Logs and resumed-build revalidation
are recorded in PROGRESS.md.

## Milestone 45: whole field faces and genuine native shell construction

Source heads: FreeCAD `ee951e3c78`, OCCT `adeac78d8c`.

TKTopAlgo `BuildNativeShells` now generates actual TopoDS_Shell objects from
private conventional caps and exact BRep_TImplicitFace geometry. Whole-face
attachment includes enclosed components; empty field faces are omitted. Multiple
components split only at coordinate-reserved proposal gaps whose planes are
checked zero-free for F and L throughout the homotopy. Component domains retain
the exact field equation and assigned original shared wires, not reference
triangle geometry. Missing/general non-axis-separable restrictions stay unresolved.

Actual native shells must have two opposite edge uses between distinct faces,
connected cyclic vertex-germ links, connected face graphs and Euler agreement
using planar-cap topology plus the checked field-component Euler values. Shells
are grouped with material-region IDs; cavities and disconnected islands differ.
Only newly built private shell flags change. Source geometry/caches/flags stay
unchanged. TopologyValidated is scoped to these engineering-reserve native checks;
whole-boundary geometric/solid validation and generic UV-based checks remain open.

OCCT-8 FreeCAD exposes build_mixed_shells, inspect_mixed_shells and
classify_mixed_shells. Successful owned handles and field-face snapshots survive
document close; source-region classification uses local coordinates and retains
solid_validated=False. Results are not Part shapes and are not assigned to the
existing native geometry property. No native-shell FCStd codec/schema change yet.

Three new kernel cases bring the suite to 68, including both operations and
closed/open fixtures, exact equations/no triangulation-backed fake faces,
source immutability, two-component splitting/limits, enclosed cavity shells and
full/empty regions. CLI adds owned lifetime, restore provenance, classifications,
domains and safe rejection. The Part.Shape(native_handle) test exposed conversion
outside PyInit's exception boundary; moving that single line into the existing
try/catch fixes normal TypeError translation. Both ABI builds now test invalid
objects/sequences and valid/default construction in part_linked_smoke.py.

The Part file retains upstream CRLF (-text). Check its diff with
`git -c core.whitespace=cr-at-eol diff --check`; no whitespace/content rewrite was
made. Reuse the existing Part target in BOTH builds for that guard fix, plus the
OCCT TKMesh dependency target, native binding and script targets. No packages/
system libraries changed. See native-shells.md for the exact proof scope/limits.

Next establish whole-boundary geometric validation, then construct/group actual
native solids with explicit validity policies. General component selectors,
curved operands, shell/solid property persistence and cross-workbench/selection/
history integration remain required. The full Stage 4 goal stays active.

Final verification: kernel **68/68** (CTest 86.35 seconds), OCCT-8 migration
**39/39** (194.48 seconds), compatibility **25/25** (93.32 seconds). Targeted
runtime/shell CTest **2/2** (15.69 seconds). Logs are in PROGRESS.md, including
both ABI Part builds and the initial constructor-test failures/fixes.

## Tools and development environment

Development host: openSUSE Tumbleweed, Linux x86-64. Observed versions:

| Tool/component | Version or role |
|---|---|
| GCC / C++ | 16.2.1; standalone C++17, native build C++23 |
| CMake / Ninja | 4.4.4 / 1.13.2 current; earlier checkpoints used CMake 4.4.3 |
| ccache | Enabled by native CMake configuration |
| Python | 3.13.16 current; earlier checkpoints used 3.13.15 |
| Qt / PySide6 / Shiboken6 | 6.12.0 current; earlier checkpoints used 6.11.2 |
| Installed FreeCAD | 1.1.4, revision 20260928; addon runtime/GUI validation |
| Fork FreeCAD base | 27.1.0-dev, upstream base `d732e2f13a` |
| Installed OCCT | 7.9.3 development package; legacy compatibility build/probe only |
| Active native runtime | build-freecad-occt8 uses the fork OCCT 8 build; never mix its headers/libraries with 7.9 |
| OCCT source reference | 8.1.0-dev1, `3d097a0328` |
| Coin / Pivy | Native configuration uses bundled 4.0.10 / 0.6.11 |
| PyCXX | Bundled 7.1.11 for native Python bindings |
| JSON | FreeCAD's bundled nlohmann JSON |
| Git / GitHub CLI (`gh`) | Commits, branches, submodules, GitHub repo creation/push/settings |
| ripgrep (`rg`), shell, `apply_patch` | Source inspection and edits during assisted development |
| GDB | Exported-function/exception debugging; OCCT Release lacks source-line symbols |
| CTest | Standalone and native regression orchestration |
| clang-format | Available; use the FreeCAD formatting configuration |
| unix2dos | Preserve upstream CRLF in the edited Gui overlay/task source files |
| Xvfb | Available for isolated native GUI regression runs |
| GoogleTest | 1.18.0 available; new OCCT tests also run through the compatibility build |
| GCC UBSan | Targeted exact mesh-predicate checks with `-fsanitize=undefined`; not a full sanitized OCCT build |

The initial audit found missing Xerces, Qt SVG/tools, and PySide/Shiboken
development packages. The user installed requested packages and authorized
continuation; configuration and compilation succeeded. No new packages are
needed for the current extraction milestone. See
`docs/build-dependencies-opensuse.md` for package names and configuration.
Do not store authentication tokens or machine-specific credentials in the repo.

This host has about 7.6 GiB RAM, 31 GiB swap, and 44 GiB disk free at the last
check. Other applications consume much of the RAM. Keep native compilation to
two jobs and linking to one. Do not start a full all-target build casually.

## Reproduce and verify

Run these commands from the umbrella root. Clone with `--recurse-submodules`.
The native configure flags are in `docs/build-dependencies-opensuse.md`; reuse
the existing build for each ABI. The separately configured OCCT 8 migration tree
must not replace or retarget the working OCCT 7 tree.

```sh
# Standalone build, with native tests when the native command exists.
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DIMPLICIT_BUILD_OCCT_DISTANCE=ON \
  -DIMPLICIT_NATIVE_FREECADCMD="$PWD/build-freecad-implicit/bin/FreeCADCmd"
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure

# Incremental native build; no installation into the system.
cmake -S FreeCAD -B build-freecad-implicit -DIMPLICIT_OCCT_SOURCE_DIR="$PWD/OCCT"
cmake --build build-freecad-implicit --target Implicit ImplicitScripts FreeCADMainCmd pivy --parallel 2
# Full native GUI after the command-line targets pass:
cmake --build build-freecad-implicit --target ImplicitGui FreeCADMain FreeCADGui_Resources Stylesheets_data --parallel 2
# Live conventional Part inputs (requires the configured OCCT bridge):
cmake --build build-freecad-implicit --target Part PartGui PartScripts Implicit ImplicitGui --parallel 2

# Native tests also run directly without enabling all FreeCAD developer tests.
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/native_smoke.py
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/mesh_smoke.py
# B-Rep fixture generation and mixed test ordering are handled by CTest.
# GUI test requires a desktop display or Xvfb:
./build-freecad-implicit/bin/FreeCAD --safe-mode FreeCAD/src/Mod/Implicit/Tests/gui_smoke.py
# Or set IMPLICIT_NATIVE_FREECADGUI during root configuration, then:
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
# Add live Part tests after the Part targets have actually been built:
cmake -S . -B build -DIMPLICIT_NATIVE_PART_TESTS=ON
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure

# Existing addon GUI test, on a desktop session using installed FreeCAD.
FreeCAD -P "$PWD/addon" tests/freecad_gui_smoke.py

# Existing standalone benchmark; argument is cells per unit cell.
./build/implicit_benchmark 6
```

Without a native build, omit `IMPLICIT_NATIVE_FREECADCMD` (or set it to an empty
string to clear a cached setting). NativeCmd now registers nine native tests;
the Part option adds two more plus the legacy native-face ABI guard. NativeGUI registers synchronous and async GUI
tests (requires DISPLAY). The OCCT extension adds its C++ checks, optional GTest,
and the native mixed snapshot test when NativeCmd is present. Other standalone,
addon and OCCT-probe tests depend on detected/configured components; inspect
`ctest --test-dir build -N` for the actual set. The complete host configuration
has 25 tests with the isolated OCCT-face executable, legacy ABI guard, worker tests and migration preflight registered
(24 without migration preflight; 22 without worker/preflight tests; 21 without
the isolated native-face entry as well).
The native-face entry now contains 113 GTest cases. See `occt-native-faces.md` for its
separate build and the `IMPLICIT_OCCT_NATIVE_FACE_TESTS` executable-path option.
Installed `FreeCADCmd` cannot import this fork's native module.
The new worker/GUI tests additionally use the executable-path cache option
`IMPLICIT_OCCT_NATIVE_WORKER`; see `native-sphere-operations.md`. Rebuild only
`ImplicitScripts` for this Python GUI adapter increment; FreeCAD's C++ linkage
remains OCCT 7.9 in the preserved build; the separate OCCT 8 command-line/GUI
migration is also tested. The isolated worker/test harness links only OCCT 8.1.
`IMPLICIT_OCCT_NATIVE_BUILD` additionally registers the strict source-build
migration preflight. It reports incomplete dependencies without changing system
libraries; see `native-gyroid-sections.md` for the package/build gate.
Python scripts can print tracebacks while exiting successfully: CTest explicitly
checks success markers and failure patterns for the native tests.

`ENABLE_DEVELOPER_TESTS=OFF` avoids the dependency and cost of FreeCAD-wide
GoogleTest targets. Native module tests are registered inside FreeCAD only with
that option on; the umbrella registration is the tested alternative.

The native command-line test set now also includes `linked_smoke.py` and
`cancellation_smoke.py`. `IMPLICIT_NATIVE_PART_TESTS=ON` additionally registers
`part_linked_smoke.py` and enables direct mixed-selection/B-Rep commands inside
the GUI regression. Do not enable that flag with an older or Part-less build.
The separate `implicit_async_gui` test checks background jobs and uses the same
optional Part flag for B-Rep source coverage.
The earlier six-/ten-/thirteen-test counts are historical; consult `PROGRESS.md`
for the current combined run.

## Remaining limits

- The native GUI provider and workbench cover basic display, live linked
  commands, a selected-field viewport probe and sampled quality inspection.
  Integrated semantic selection/snapping and whole-workflow async scheduling remain open.
- The field graph lives in `App::PropertyString`; the dedicated complex geometry
  property currently covers mixed gyroid/box boundaries only. A shared-node JSON
  DAG format, graph editor and SIMD/GPU backend remain open. Scalar batch
  evaluation and compiled-graph caching are now implemented.
- The native extractor has uniform and conservative octree-pruned sampling paths.
  Refining edge roots improves vertex residuals, not triangle-interior error or
  a Hausdorff bound. Grid cells can hide small components, tangencies and multiple
  crossings. Hard Boolean creases can be smoothed by interpolated vertex normals.
- Native extraction uses 2–64 cells per axis and limits vertices and triangles
  independently to one million. It retains full grid samples, a root map, and
  Python result copies. Python extraction now releases the GIL and supports
  cancellation tokens. The GUI has opt-in background extraction, with one worker
  and per-view request coalescing; Part recompute, snapshots and Coin uploads
  remain synchronous, and global snapshot/cache budgets remain open.
- Root positions and native scalar APIs are local to the feature. Rendering must
  apply `Placement` once. Domain clipping does not generate caps. Sphere/shell
  connectivity checks do not guarantee topology for arbitrary graphs.
- Classification uses the field's sign and a field-value tolerance. Raw gyroid
  values and offsets are dimensionless; they are not distances in millimetres.
- Cylinder contours are implemented in the standalone engine. Capped gyroid-band
  visualization and mixed B-Rep/implicit field operations are implemented.
  General trimming topology, uniform physical thickness, native OCCT implicit
  general face Booleans and a whole-solid manufacturing/STEP bridge remain open.
  Restricted sphere-plane trims and two-sphere native Boolean boundaries are now
  implemented, with process-isolated experimental GUI exposure, not valid Part solids. Local
  B-spline open-face fitting, clipping and STEP round-trip are now tested.
- Both engines now support local projection and sampled rays. Native projection
  reports stationary-point convergence/failure, not a globally nearest point;
  hard creases, critical points and ill conditioning can fail. Rays can miss
  tangencies/thin components. Native ray roots carry bracket widths and residuals;
  viewport probes are not integrated semantic selection. Mesh-quality interior
  residuals and edge counts do not certify geometric error or solid validity.

## Next milestones and acceptance criteria

The user requested proceeding through the last stage. Do not mistake the latest
local open-patch prototype for completion: shared chart boundaries/p-curves,
validated atlas/solid construction and native OCCT implicit topology are still
open, as are broader property/DAG integration, semantic selection and variable-resolution
cells. The user has now selected **0.1 mm geometric tolerance** and directed
OCCT-native face development; minimum feature size remains unspecified. The
native storage/query foundation is implemented, but no whole-model production
accuracy, trimming, BOP or general native B-Rep persistence claim follows from it.
Primitive single-face persistence and restricted sphere trimming, Boolean
boundaries, geometric-bound meshing and process-isolated GUI exposure are now
implemented. Native derivative-bound gyroid/plane-rectangle sections, restricted
closed Z-split seams and contour previews are also implemented. The immediate
native milestone is open/domain-side trim topology, arbitrary/nested trimming
and shared topology-aware surface meshing, plus singular/tangent resolution,
then generic splitting/classification/history and full native persistence.
Do not route these faces through generic BRepMesh/BOP or OCCT 7 Part.
For the wholly OCCT-8 FreeCAD build, required toolkits and command-line targets
and GUI targets are built; strict preflight and all 18 targeted migration checks
passed at the ABI milestone. The explicit native in-process API/GUI increment
now passes 20 migration checks; see `in-process-native-faces.md`. Next expose
gyroid section/split snapshots through the same ownership boundary and extend
topology-aware persistence/conforming meshing. Do not assign arbitrary fields
to Part shapes or route implicit faces through generic UV-dependent BOP/meshing.
Restricted native gyroid/box Common and BoxMinusField boundaries now exist;
see `native-mixed-box.md`. Regular box-edge vertices/open-contour joining and
multiple planar cap regions now work. Next gates are trim-conforming meshes,
broader/nested/singular coverage, curved operands and implicit-aware solid
validation/classification/persistence. General mixed BOP remains separate work.
Parameterize tolerances explicitly and preserve the staged acceptance gates.

1. **Native feature display (implemented and GUI-tested):** `ImplicitGui` view-provider registration,
   use indexed native extraction, retain Coin nodes, expose resolution/color/modes,
   rebuild only when relevant field/domain properties change, apply Placement
   once, and regenerate after FCStd restore. Build the fork GUI target and verify
   a composed graph, save/reload, buffer shrink, parameter edits, and camera motion.
2. **Adaptive extraction (conservative pruning implemented):** keep the uniform native extractor as a regression
   reference. Add conservative bounds/error criteria, shared vertices across
   refinement levels, transition handling, cancellation and explicit budgets.
   Test topology across coarse/fine cells and measure performance separately
   from viewport upload. Do not prune cells merely because all corners agree.
3. **Field usability:** batch evaluation is implemented; shared-node JSON graph representation,
   dimensional contracts, more transforms, band/shell nodes and distance estimates.
   A level-set band must remain distinct from physical thickness.
4. **Geometry operations:** cylinder contour tracing and capped visualization
   bands are implemented; general domain boundaries,
   capped visualization solids and quantitative distance/offset validation.
5. **Mixed representation:** snapshot fields and mixed composition are implemented;
   live Part source links and direct Part operand dispatch are implemented and
   tested. Strengthen container/App::Link tests, measure approximation error
   separately from the OCCT distance/classification tolerance, and document costs.
6. **Manufacturing bridge:** local charts and fitted B-spline patches, common
   boundaries/p-curves, sewing and shape validation, then OCCT Booleans and STEP
   export with measured approximation error and retained implicit provenance.
7. **Broader integration:** cross-workbench selection and recompute contracts,
   FEM/CAM consumers, performance backends, and only then an evidence-based OCCT
   native geometry proposal. Full details are in the roadmap/core integration plan.

## Publishing changes

There are three repositories. Commit and push changes inside `OCCT` and
`FreeCAD` first, then stage their submodule pointers alongside umbrella changes.
OCCT tracks `feature/implicit-distance`; FreeCAD tracks `feature/native-implicit`.

```sh
git -C FreeCAD switch feature/native-implicit
git -C FreeCAD status --short
git -C FreeCAD diff --check
# Add only the intended native module/build files, commit, then:
git -C FreeCAD push origin feature/native-implicit
git -C OCCT push origin feature/implicit-distance
# In the umbrella, stage intended changes including the FreeCAD gitlink.
git status --short
git diff --check
# Commit, then:
git push origin main
git submodule status
```

Keep this handoff current when completing work. Record tested outcomes and
separate configured options from targets actually built and run.
