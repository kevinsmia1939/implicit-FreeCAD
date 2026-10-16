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
