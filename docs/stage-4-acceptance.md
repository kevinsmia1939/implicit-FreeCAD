# Active goal: through Stage 4, seamless FreeCAD integration

This is the six-stage overview agreed with the user, not the differently
numbered original prototype phases. The goal is **active**, not achieved by
the restricted mixed-box prototype. Preserve its full scope across increments.

## Stage 2: mixed Boolean topology

- Shared box-edge roots and open-contour joining across adjacent faces.
- Multiple/nested boundary loops; retained-region splitting and cap construction.
- Curved conventional B-Rep operands, not only axis-aligned boxes.
- Field-aware intersection, splitting/classification/history and dispatch;
  explicit tangent/singular/ambiguous failure policies, no guessed topology.
- Numerical error budgets recorded against the 0.1 mm geometric target.

Evidence must include both operations and operand order, independent geometric
reference checks, shared edge/vertex identity, orientation, cap validity,
placement/scale cases, degeneracy/resource failures and source immutability.

## Stage 3: usable native solids

- Trim-conforming surface meshes match the native boundary seams.
- Closed-shell construction, orientation and field-aware solid classification.
- Implicit-aware topology and geometric validation before claiming valid solids.
- Supported positive and negative cases tested; mesh incidence alone is not
  evidence for a native CAD solid or global geometric error.

## Stage 4: seamless FreeCAD integration

- Dedicated representation-aware geometry property, not arbitrary fields in
  Part::Feature::Shape or just an opaque capsule/Python recipe.
- Versioned topology persistence, ownership, restoration and migration tests.
- Integrated semantic selection/snapping and meaningful subelement references.
- Editing tools, live dependencies, recompute, undo/redo and failure invalidation.
- Representation-aware cross-workbench dispatch with explicit consumer contracts.
- Container/App::Link/placement handling; unsupported consumers fail safely.
- End-to-end GUI tests across create/edit/select/save/reopen/undo/redo workflows.

Manufacturing exchange (Stage 5) and nTop-scale compute/features (Stage 6) are
outside this goal's completion boundary, but existing invariants and upstream
licensing must be preserved. Stage 4 cannot be declared complete while Stage 2
or 3 requirements remain missing or merely supported by narrow proxy tests.

## Current evidence

Latest default-gyroid increment constructs actual corner-aware native trim
graphs for both operations, including shared canonical corner vertices and
separate excluded closed-set contacts. The field/domain are unchanged and
the seam target remains 0.1 mm. The later solid extension now qualifies the
restricted default operations numerically; corner-aware mesh/view/live GUI qualification remains before
switching the default GUI primitive. Unsupported legacy consumers reject explicitly.
See [corner boundary contracts](native-corner-boundary.md). The full goal
and curved-operand/snapping/history/consumer requirements remain active.
Schema-2 [corner persistence](native-corner-persistence.md) now retains policy,
contacts and actual native topology in FCStd/content transfer/undo/redo without
rebuilding. This advances document ownership only; it does not remove the
explicit mesh/viewport gates above or replace fresh solid qualification.
The [corner-aware physical classifier](native-corner-classification.md) now
supports regularized material, exact retained-corner witnesses and excluded-
contact distance bounds, including placed/restored native owners. This is
mathematical-region evidence, not stored native-solid/topology qualification.
The [whole-box corner homotopy/reference](native-corner-topology.md) now resolves
the default zero set's 2-D component plus separate isolated contacts. Actual
native attachment and solid qualification still remain; neither reference
incidence nor equal component counts closes that gap.
The [closed corner material reference](native-corner-region.md) now adds shared
caps, strict-volume connectivity and correct isolated-contact separation. Default
Common/Cut references each have Euler -2 and one material region. Actual stored
native correspondence/attachment and geometric/solid qualification still remain.
The [corner-aware graph/cap checker](native-corner-attachment.md) now matches the
stored graph to fresh traces and attaches native cap regions to the reference
with strict common retained witnesses and clearance. Whole implicit-component
attachment remains a separate required gate; this is not
native shell/solid or Stage 4 completion.
The [corner-aware trim-cycle checker](native-corner-trim.md) now matches complete
directed cycles with canonical shared corner anchors. Actual native 2-D face
restriction/membership, isolated-contact exclusion and whole-face validation
remain necessary before native-solid/default-GUI qualification.
The [native face-domain restriction](native-face-domain.md) now excludes both
isolated surface contacts using checked uniform-cone neighborhoods, including
owned/placed queries and saved topology migration. Whole restricted-face
attachment and geometric/native-solid qualification are still separate gates;
this is not a default GUI switch or Stage 4 completion.
The [corner-aware native shell extension](native-corner-shells.md) now attaches
whole restricted field faces and constructs actual native shells for both default
operations. It preserves exclusions, oriented edge pairing, cyclic vertex links,
face connectivity and reference Euler/material grouping. This is implicit-aware
topology qualification only: native geometric/solid validation, default GUI,
curved operands and all unfinished Stage 2–4 requirements remain open.
The [corner-aware cap geometry extension](native-corner-cap-geometry.md) now
checks both directed cap-region distance bounds with actual p-curve/edge/vertex
deviations and regularized side membership. It does not itself construct or
validate corner-aware native solids; factory/persistence/view integration and
every other unfinished Stage 2–4 requirement remain required.
The [corner-aware native solid extension](native-corner-solids.md) now qualifies
both default operations at 0.1 mm and persists actual restricted solids through
the native property/FCStd path. Restoration drops proof flags; explicit rebuilding
rechecks separately. Corner-aware mesh/view/live-task GUI qualification and
default-command exposure remain open, as do curved operands and every other
unfinished Stage 2–4 gate. Restricted default solids do not complete the goal.

At goal start: root `6397787`, OCCT `459f71a3a9`, FreeCAD `dfb000b373`.
Restricted closed-loop gyroid/box Common/BoxMinusField native boundaries exist;
27 kernel cases, 22 OCCT-8 migration tests and 25 compatibility tests passed.
These establish only the restricted baseline. General edge crossings, conforming
gyroid meshing, validated mixed solids and most Stage 4 gates remain open.

First goal increment: native axis-aligned box crossings now use shared,
uniqueness-checked edge roots. Open contours join into closed implicit wires;
planar boundary graphs construct multiple regions and holes. New tests compare
crossing endpoints against independent harmonic solutions and exercise two-hole
versus two-disk caps. FreeCAD exposes crossing counts and a zero-gyroid demo.
This is Stage 2 progress, not completion of the overall goal. Curved operands,
conforming meshing, native-solid validation and Stage 4 integration are still open.
Verified checkpoint heads: OCCT `ef20e3f9d7`, FreeCAD `1daddc4c3a`; 30 kernel
cases, 22 migration CTest entries and 25 compatibility CTest entries pass.

Next increment: native mathematical-region classification now uses physical
distance lower bounds and cap/root witnesses, not raw field residuals or preview
meshes. The FreeCAD mixed recipe exposes immutable-snapshot classification with
explicit unresolved cases and failure invalidation. Whole-native-solid validity,
conforming surface meshing, curved operands and the Stage 4 property/persistence/
selection/editing/consumer requirements remain unfulfilled; the goal stays active.
Checkpoint heads: OCCT `1ba27f551a`, FreeCAD `256ccafef0`; 33 kernel cases,
22 migration entries and 25 compatibility entries pass. This evidence does not
prove completion of any remaining Stage 2/3/4 gate.

Next increment: explicit native schema-1 topology I/O now preserves the field,
shared cap/seam edges/vertices, planar p-curves and trim wires without numerical
recipe rebuilding. Loaded seam-bound metadata is explicitly unverified and
solid validity stays false. This advances topology persistence/ownership only;
dedicated FreeCAD geometry-property/FCStd migration integration, semantic
selection/editing/consumers and all other unfinished Stage 2/3 gates remain.
Verified heads: OCCT `aa6ced5047`, FreeCAD `eacfd7c7e6`; 35 kernel cases,
22 migration entries and 25 compatibility entries pass. This is not evidence
that the overall through-Stage-4 goal has been completed.

Next increment: a C++ ComplexGeoData property and typed native GeoFeaturePython
owner now persist mixed-boundary topology in FCStd, with immutable transactions,
restore without numerical rebuilding, input-consistency checks and explicit
corruption handling. Schema-1 recipes migrate in place with legacy limitations.
This implements a supported mixed-representation property path, not migration of
every field/sphere feature or completed semantic selection/consumer/placement
integration. All unfinished Stage 2/3/4 requirements above remain in scope.
FreeCAD checkpoint `d2fec1801c`; OCCT remains `aa6ced5047`. Property/object
content transfer, inline XML restore and static cross-document copy are tested;
interactive clipboard and broader container/placement consumers remain open.
Full suites pass 23/23 migration and 25/25 compatibility entries, including the
unchanged 35 kernel cases. These do not establish Stage 2/3/4 completion.

Next increment: native owned subelements and standard ComplexGeoData snapshot
access expose boundary lines independent of document lifetime. Seam identity
uses stored trim-wire edge order, not tracing/triangle order. The mixed GUI now
maps actual Coin ray hits to CapN/SeamN and provides whole-node detail paths.
Selection clears on snapshot replacement/failure. These are snapshot-local
indices, not persistent LinkSub history mappings. Snapping, stable naming,
implicit-surface picking, broader consumer/placement qualification and every
unfinished Stage 2/3 gate remain required; the full goal is still active.
Checkpoint: FreeCAD `1a514006be`, OCCT unchanged `aa6ced5047`; 23/23 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.

Next increment: a standard FreeCAD element map derives mapped native names from
the stored mesh-free topology digest. Typed-owner lookups and mixed GUI detail
paths reject stale mapped references instead of retargeting their old indices.
Real LinkSub tests cover unchanged save/copy, changed/cleared geometry, saved
unresolved links, undo/redo and exact recovery. This is conservative snapshot
invalidation, NOT tracking of surviving elements across Boolean/parameter edits.
Plain indices and legacy owners retain their documented limits. Semantic
history/snapping, broader consumers/placements and every unfinished Stage 2/3
gate remain required; the overall goal remains active.
Checkpoint: FreeCAD `e036a6855a`, OCCT unchanged `aa6ced5047`; 24/24 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.

Next increment: typed-owner getSubObject and GUI SelectionEx.SubObjects now
return owned native selected-element ComplexGeoData. Boundary lines/bounds and
pre-placed rigid queries are tested for every fixture subelement, transform=False
and nested App::Part paths. Unsupported sampling, mutation, persistence,
Part/native-property assignment and nonrigid transforms reject. This does not
qualify general consumers/App::Link/containers, integrated snapping/history or
the unfinished Stage 2/3 surface/solid/Boolean gates. The full goal stays active.
Checkpoint: FreeCAD `1f46ce6988`, OCCT unchanged `aa6ced5047`; 25/25 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.

Next increment: empty native subobject references now extract owned whole
ComplexGeoData. Rigid App::Link whole/subelement paths, both placement modes,
mapped cap links, Part-to-link paths, save/reopen and GUI native cap ray picking
have explicit regression coverage. External queries/instance-file restore are
tested with the source already loaded; source maps remain unchanged. Arrays,
automatic external dependency loading, missing files and all broader consumer/
instance modes remain unqualified. No completion claim follows for Stage 2/3
or remaining Stage 4 history/snapping/editing/workbench gates.
Checkpoint: FreeCAD `6f93521083`, OCCT unchanged `aa6ced5047`; 26/26 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.

Next increment: classifyNative now queries the mathematical region through whole
local/pre-placed ComplexGeoData. Separation/witness bounds account for rigid
placement precision and near-rigid stretch; exhausted precision remains unresolved.
Tests cover both operations, independent cap distances, cap/root witnesses,
stationary cases, container/link snapshots, restore/lifetime and invalid selected
elements. GUI instance queries are tested. This is not solid validation, native
surface meshing, placed-input Booleans or completion of remaining Stage 2/3/4 gates.
Checkpoint: FreeCAD `e88c92efaa`, OCCT unchanged `aa6ced5047`; 27/27 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.

Next increment: the mixed recipe has a transactional Qt task editor for operation,
seam tolerance and tracing budget. Real edit/button, preview/cancel, selection,
commit/undo/redo, failed-preview clearing/recovery, accepted save/reopen/re-edit,
foreign transaction guards and document-close cleanup are tested. This advances
editing workflow only; graph/equation tools, broader editing/consumer/history/
snapping support and every unfinished Stage 2/3 gate remain open. The full goal
is not achieved by the restricted mixed editor.
Checkpoint: FreeCAD `f35700d659`, OCCT unchanged `aa6ced5047`; 28/28 migration
and 25/25 compatibility entries pass, including unchanged 35 kernel cases.
Final expanded task-only GUI/fixture checks pass 3/3.

Next increment: native TKMesh preparation meshes planar caps on a private
snapshot, preserving every native seam knot and sharing canonical vertex/edge
sample IDs. Effective implicit wires and cap triangles have opposite boundary
incidences. The OCCT-8 FreeCAD API exposes those transient constraints, preserving
source topology/caches and bound provenance. Two new kernel cases plus a native
CLI test cover both operations, open/closed contours and failure/lifetime guards.
This is preparation for trim-conforming implicit surface meshing, NOT that
surface mesh or solid validation. GUI still shows caps/seams only; every
unfinished Stage 2/3/4 requirement remains in scope. See native-mesh-constraints.md.
Checkpoint: FreeCAD `8a4dd8ef32`, OCCT `f20e7c4b63`; 37 kernel cases,
29/29 migration and 25/25 compatibility tests pass. No completion claim follows
for the implicit-surface mesh or remaining native-solid/integration gates.

Next increment: the native mesher now attaches a field-extracted interior to
the exact shared trim samples with an oriented collar and projected shared-edge
refinement. It checks triangle-wide one-sided distance to the untrimmed field,
opposite paired edges and cyclic vertex links. The opt-in GUI uses the same
cap/surface buffers and picks NativeFace1 semantically; restore, failure recovery
and unchanged cache reuse have actual GUI tests. Three new kernel cases (40
total) include wider multiple loops and scaled/translated operands. This is a
trim-conforming candidate, NOT verified coverage, embeddedness, clipped-field
Hausdorff error or native-solid validity. These Stage 3 gates and all other
unfinished Stage 2/4 requirements remain active. See native-conforming-preview.md.
Verification: 40 kernel cases, 30/30 migration tests and 25/25 compatibility
tests pass. These do not prove coverage, embeddedness or native-solid validity.
Checkpoint heads: FreeCAD `ef5c5f283e`, OCCT `3ca2e7235e`.

Next increment: an explicit native transient-mesh contact audit now checks all
cap/implicit triangle pairs, including pairs sharing node IDs. Exact integer
separators operate on stored binary64 coordinates; allowed common simplices are
not permission for overlaps elsewhere. Limits return unresolved, never clear.
Both operations and closed/open contour fixtures pass the audit through the
FreeCAD API; source geometry remains unchanged. Two new kernel cases (42 total)
include adjacency/crossing, coplanar, scale, precision/resource and bad-input
regressions. This advances mesh embeddedness evidence only. Surface coverage,
clipped-field consistency, native-solid validation and all unfinished Stage 2/4
requirements remain active; GUI previews do not automatically run the audit.
Verification: 42 kernel cases, 31/31 migration and 25/25 compatibility tests
pass, plus a targeted GCC UBSan predicate run. None of these replace field
coverage/clipping or native-solid validity evidence.
Checkpoint heads: FreeCAD `836b609498`, OCCT `72b5a40c8d`.

Next increment: whole-box/face partitions and reverse clipped witnesses now check
two-sided numerical proximity to the mathematical boundary. Independent harmonic/
cap references, missing-component and extraneous-triangle tests extend the kernel
suite to 45 cases; 32/32 migration and 25/25 compatibility tests pass. This is
not directed interval arithmetic, preserved topology or native-solid validity.
Regularity/trim correspondence, topology/reach criteria, native-solid construction,
curved operands and unfinished Stage 4 integration remain required. The full
goal stays active; see native-clipped-coverage.md.
Checkpoint heads: FreeCAD `d267f58f7a`, OCCT `18ed5c0a52`.

Next increment: mesh-independent stratified regularity now covers the complete
box volume/faces/edges/corners with projected-gradient bounds and zero exclusion.
Analytic volume, face, edge and corner negative cases, scaled/translated inputs,
both operations and stored/native snapshot paths extend the kernel suite to 48.
The explicit FreeCAD API preserves source state and rejects unsupported poses/
representations. This is numerical field regularity only, NOT stored trim-wire
correspondence, topology equivalence, reach or native-solid validity. These and
all unfinished Stage 2/4 gates remain required; see native-box-regularity.md.
Verification: 48 kernel cases, 33/33 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `094407d71f`, OCCT `fd9a53d063`.

Next increment: stored native trim graphs now have independent fresh-reference
correspondence checks, with unique continuous curve matches, induced shared
vertex bijections, cap grouping, directed cyclic wire comparison and strict
cap p-curve parameter bounds. Resampling/cyclic starts and shuffled caps pass;
changed math with forged flags, parameter mismatches and limits reject. Four new
kernel cases bring the suite to 52. This is graph correspondence, NOT entire
surface isotopy/reach or native-solid validity. Those and all unfinished
Stage 2/4 gates remain required; see native-trim-correspondence.md.
Verification: 52 kernel cases, 34/34 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `6a24569e8d`, OCCT `ec71be2ccd`.

Next increment: whole-box field-to-nodal-linear homotopy now checks shared
increasing field-gradient directions across incident tetrahedral pieces, with
separate true box-face/edge restrictions. The transient reference reports
component Euler characteristics, boundary loops and manifold incidences/links.
Hidden components missed by coarse nodal signs, critical strata and exhausted
budgets remain unresolved. Five new kernel cases bring the suite to 57; native
CLI adds both operation/contour fixtures, restored provenance, lifetime and
resolution-32/64 topology agreement on the larger edge-crossing fixture.
This is numerical sufficient-condition evidence, not directed intervals,
stored-trim/cap or preview equivalence, accuracy qualification or native-solid
validity. Those relations, shell construction and every unfinished Stage 2/4
gate remain required. See native-box-topology.md; the full goal stays active.
Verification: 57 kernel cases, 35/35 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `9874d2e306`, OCCT `3f9aa48bcf`.

Next increment: the field/linear homotopy reference now has shared retained box
caps, closed-component incidences/cyclic vertex links and tetrahedral material
connectivity. Boundary shells are attached to retained volume regions rather
than guessed from signed volume: cavity shells share the outer shell's region,
while disconnected islands have separate regions. Three new kernel cases bring
the suite to 60, including complementary reference volumes, full/empty regions,
source immutability, limits and an exact contact audit on a cavity reference.
Native CLI independently checks buffers/Euler/volumes, both operations/contour
fixtures, restore/lifetime/guards and two-cavity grouping. These are mathematical
region reference buffers, NOT native trim/cap correspondence, CAD shells/solids
or accuracy-qualified field volumes. Native correspondence/construction and all
unfinished Stage 2/3/4 requirements remain active; see native-region-topology.md.
Verification: 60 kernel cases, 36/36 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `7351987bdb`, OCCT `9ee814feb0`.

Next increment: stored conventional cap regions now attach bijectively to closed
reference cap/shell/material components using strict common retained F/L points,
actual cap classification and clearance beyond fresh seam uncertainty. This
requires the independent graph and region-reference prerequisites, not equal
counts or source verification flags. Two new cases bring the kernel suite to
62, with both operations/contour fixtures, restore/immutability, multiple regions,
cap reordering and limits; forged math/bounds fail before attachment. Native CLI
also independently checks Part cap/edge distances and snapshot lifetime.
Complete trim-wire/implicit-face attachment and native shell/solid construction
remain required, as do curved operands and all unfinished Stage 2/3/4 gates.
See native-cap-attachment.md; no full-goal completion follows from cap mapping.
Verification: 62 kernel cases, 37/37 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `81cf92e7da`, OCCT `2e5bc37269`.

Next increment: private complementary cap construction and dual cap attachment
now identify contour arcs by physical side, retained/complement cap-region pair
and ordered box-root anchors. Every native/reference arc must match bijectively,
and complete signed cyclic wire sequences must agree. Closed contours use winding
area for direction, not arbitrary vertex anchors. Three new cases bring the
kernel suite to 65, including both operations, three-wire open fixtures, multiple
holes on one cap, separate surface components, restored provenance and limits.
Resampled curves/cyclic wire starts pass; forged math fails the prerequisite.
Native CLI verifies both operation/contour fixtures, complete mappings, record
immutability, restore/lifetime and two-hole/two-component queries.
This attaches trim boundaries, NOT the whole native implicit face or CAD shells/
solids. Empty/enclosed components and a component-restriction/splitting policy
remain necessary, along with curved operands and every unfinished Stage 2/3/4
gate. See native-trim-attachment.md; the full goal remains active.
Verification: 65 kernel cases, 38/38 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `88253649f0`, OCCT `ee9f07f4a2`.

Next increment: whole exact field faces now attach to the field/linear reference.
Disconnected components split only across checked F/L-zero-free axis planes;
their native wire geometry must fit the resulting domains. Empty field faces
are omitted. Actual native shells group copied caps and exact field faces by
reference boundary/material identity; native opposite edge uses, vertex-germ
cycles, connected face graphs and field-aware Euler characteristics are checked.
Three new kernel cases bring the suite to 68, covering both operations/open and
closed fixtures, separate components/limits, enclosed cavities and full/empty
regions. Native CLI exercises owned handles/faces, local region classification,
restore/lifetime, genuine split domains and rejection guards. A Part constructor
conversion moved inside its exception boundary after the unsupported-capsule
test exposed an escaped C++ TypeError; both ABI builds test ordinary invalid
inputs and valid/default construction.
These are actual native shells with topology checks, NOT geometrically validated
solids or generic Part shapes. Geometric/solid validation, general restrictions
for non-axis-separable components, curved operands, shell/solid property
persistence and all unfinished Stage 4 requirements remain active. See native-shells.md.
Verification: 68 kernel cases, 39/39 migration and 25/25 compatibility tests pass.
Checkpoint heads: FreeCAD `ee951e3c78`, OCCT `adeac78d8c`.

Next increment: the restricted native solid factory now adds actual cap coverage
in both directions and bounds 3D edge/vertex deviations, alongside whole-field
attachment/native topology and maximum stored tolerance checks. Only then are
actual solids constructed by material region. A private spatial segment tree
preserves both distance and parity queries while keeping the open-contour case
within the unchanged work budget. Five new kernel cases bring the suite to 73;
the full isolated kernel suite passes. FreeCAD exposes owned build/inspect/
classify solid snapshots, not Part shapes; CLI lifetime/restoration/negative
tests pass. Whole-boundary validation is explicitly numerical implicit-aware
tolerance validation, NOT formal intervals or generic UV BRepCheck/BOP.
The focused Implicit Modelling workbench also creates editable native 10 mm cube
gyroids with saved dimensional expressions and actual GUI regression coverage.
That simple field command is not yet the trimmed/solid CAD workflow. Curved
operands, general component restriction, solid-property persistence, semantic
vertex picking/snapping/history and remaining consumer/GUI gates still prevent
completion of Stages 2–4. The full resumed goal stays active.
Verified checkpoint: OCCT `b032b19ae2`, FreeCAD `94175dd569` (workbench commit
`93a2bb06ac`); **73 kernel cases**, **40/40 migration**, **25/25 compatibility**
passed. These tests qualify the documented restricted increment, not the full goal.

Next increment: the mixed native property now exposes all Edge/Vertex subelements
from shared OCCT topology, without welding coordinates or promoting polyline
poles to vertices. Stored edge order and forward endpoint identity define vertex
indices. The renderer shares seam/edge alias nodes, draws conventional boundary
edges/actual point markers, and supplies exact stored-point extraction with
once-applied rigid placements. Camera/pixel picks, native selection SubObjects,
instance vertices, mapped links, restoration/visibility/failure clearing and
cache reuse are covered. Standard BRep vertex text rounding is tested against
machine-scale reserves with exact endpoint identity, not disguised by snapping.
The focused workbench adds an explicit native-boundary demo, separate from the
default simple field gyroid. This advances semantic topology selection only;
integrated snapping/history, solid property persistence, curved operands and
every other unfinished Stage 2–4 gate still prevent completion of the full goal.
The mixed surface also uses analytical field-gradient normals for smooth shading,
with cap normals/creases separate and no geometry/validation-policy change.
Final verification: 41/41 OCCT-8 migration and 25/25 compatibility tests passed,
including the unchanged 73 kernel cases. These establish the documented mixed
selection/display increment, not completion of the overall goal.
Checkpoint: FreeCAD `9a154a3f91`, OCCT unchanged `b032b19ae2`.

Next increment: explicit solid topology schema 1 now restores actual component
field faces, source-wire assignments, shared caps/edges/vertices and shell/material
relationships without numerical rebuilding. Restored qualification flags and
shell/solid Closed flags stay false; structural graph checks do not replace the
independent field/topology/geometry pipeline. Both operations, multiple solids,
cavity shells, ownership, corruption, locale and source cache/flag preservation
are covered. FreeCAD exposes owned codec/source APIs; its existing boundary
property and Part correctly reject these solid snapshots. Dedicated solid
geometry properties/owners, FCStd integration and all remaining Stage 2–4 gates
remain required. The full goal is not achieved by this codec prerequisite.
Verified checkpoint: OCCT `0e6377579d`, FreeCAD `61f412c1fe`; 76 kernel cases,
42/42 OCCT-8 migration and 25/25 compatibility tests passed.

Next increment: a dedicated C++ solid ComplexGeoData property and native document
owner now expose actual Solid/Shell/Face/component/edge/vertex topology with a
distinct mapped-name namespace and FCStd payload. Restore does not numerically
rebuild or trust saved qualification flags. Runtime transactions retain owned
snapshots; source-error invalidation preserves the solid representation type.
Tests cover separate components/cavities/full/empty, mismatched property types,
content/copy/FCStd corruption, mapped references and rigid container/link paths.
Whole-region classification explicitly covers the source material union;
selected SolidN classification refuses rather than silently using that union.
This closes a restricted solid document-property gate, not the end-to-end solid
GUI/live-dependency workflow. Curved operands, general restrictions, default
gyroid degeneracies, snapping/history and broader consumers remain required.
The full through-Stage-4 goal stays active. See native-solid-property.md.
Verified checkpoint: FreeCAD `46bb773deb`, OCCT unchanged `0e6377579d`;
43/43 OCCT-8 migration and 25/25 compatibility tests passed, including 76 kernel
cases. Existing GUI regressions protect the shared owner refactor; solid-specific
GUI rendering/picking is not implemented or claimed by those tests.

Next increment: a live NativeSolidFeaturePython controller now links a native
boundary through a global-scope source property, reuses unchanged actual topology,
checks source/tolerance consistency and builds new qualified results on edits.
Unchanged FCStd restore/recompute does not numerically rebuild or recertify saved
topology. Corruption requires explicit repair; source failures/budgets clear the
property, diagnostics and view. Followed/independent output placement respects
container coordinates without moving upstream operands.

The new renderer uses actual result solid/shell/face relations, individually
pickable field components, actual ComplexGeoData edges/vertices and native seam
aliases. Display triangles must fit exactly one saved component domain; preview
failure is separate from authoritative solid qualification. Workbench commands
expose a supported demo and selected-boundary construction. Default Add Gyroid
remains a simple field. Camera/visibility caches, real ray/pixel picks, rotated
output links, two components, full/empty results and saved unverified views are
covered by separate live-workflow tests. These do not establish general source
instances, snapping/history, default trim degeneracies, curved/general operands,
richer editing or broader consumers. All unfinished Stage 2–4 gates remain active.
Final full regression passed 45/45 OCCT-8 migration and 25/25 compatibility
tests, including unchanged 76 kernel cases. This qualifies the documented
restricted live-solid increment, not completion of Stage 4.
Checkpoint: FreeCAD `8bdaff16bd`, OCCT unchanged `0e6377579d`. Final focused
GUI/runtime rerun also passed 3/3 after the primary viewport-capture adjustment.

Next increment: exact quarter-period identities and checked local corner
neighborhoods distinguish the default gyroid's isolated contacts from its six
split material cones. A local query explicitly separates closed inequalities
from closure-of-interior regularization; it never changes current Boolean or
whole-solid validation policies. Nearby coordinates, unknown zeros, tangent
coordinate directions and exhausted precision do not receive guessed topology.
The existing default native trim builder and strict regularity checks still
reject. This supplies a necessary corner model, not the pending plane tracing,
shared corner roots, nodal-zero reference, actual attachment or solid workflow.
Every unfinished Stage 2–4 requirement remains active.
Verification: 79 kernel cases, 46/46 OCCT-8 migration and 25/25 compatibility
tests passed; the refreshed OCCT-7 facade guard also passed 1/1. These establish
the documented local model only, not support for default whole-box trimming.
Checkpoint: OCCT `27ba200574`, FreeCAD `b79fbbe49d`.

Next increment: explicit lattice-node plane tracing now resolves all six default
cube-face rectangles with three open curves/two isolated contacts per face.
Exact grid identity, whole-edge endpoint monotonicity and signed cell cones
handle corner/interior zeros without welding. Rounded affine identities and
unknown zeros remain unresolved. Isolated point contacts are preserved rather
than silently treated as curves or omitted. Identity/coordinate-plane limits
are explicit; existing builders/worker retain strict mode. The FreeCAD API is a
transient mathematical section query, not native trim subelements or a solid.
Independent analytic coverage, graph/point cases, six-face incidence, refusal,
ownership and restore tests pass. Final regression: 81 kernel cases, 25/25
compatibility and 47/47 OCCT-8 migration. Shared cross-face native vertices,
box roots/cap cycles, corner-aware reference/attachment/material persistence and
all unfinished Stage 2–4 requirements remain necessary. The full goal stays active.
Checkpoint: OCCT `6e3876f47d`, FreeCAD `000e83bba2`.
