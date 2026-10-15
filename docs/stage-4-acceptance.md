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
