# Development stages and completion gates

This is an acceptance checklist, not a claim that the full nTop-class project
is complete. `PROGRESS.md` records tested increments; `HANDOFF.md` records how to
continue. The original prototype phases and the native-kernel roadmap have
different completion criteria.

The focused [Implicit Modelling workbench](implicit-modelling-workbench.md) now
provides one-click native gyroid creation with editable dimensions and field
parameters. This is a primitive GUI usability increment, not completion of the
remaining mixed-solid, snapping, history or cross-workbench integration gates.
The [restricted native solid factory](native-solids.md) now gates actual solids
on whole-field topology, cap/edge/vertex coverage and stored tolerance checks.
It is numerical implicit-aware validation, not general Part compatibility or
completion of solid persistence, GUI selection/snapping and other Stage 4 gates.
The restricted mixed GUI now exposes [all native edges and vertices](native-edge-vertex-selection.md),
including point extraction, rigid subobjects and seam aliases. This is not
integrated snapping, edit-stable naming or completion of the default gyroid's
trimmed-solid workflow.
The [corner-aware transient display](native-corner-mesh.md) now renders both
default operations opt-in with actual native selection. A separate
[corner-aware clipped proximity check](native-corner-coverage.md) supports the
0.1-mm target; it is not automatic qualification of GUI buffers or every edit.
[Opt-in checked display](native-checked-display.md) now adds same-buffer
binary32-aware proximity and contact checks; it does not qualify every edit.
The [native-solid task editor](native-solid-editor.md) now covers build settings
and shared-source operations, independent geometry depth, legacy migration and
transaction/failure recovery. Exact translated conventional corner boxes are
supported. Primitive shared period/level/box dimensions can now be staged with
preserved workbench expressions, same-document peer invalidation, rollback and
unchanged restored-input precision. Arbitrary transforms, curved operands,
general field graphs and source-link replacement are not covered. Native-solid
creation now stages tolerance/depth/work settings before the first build and
rolls back failed attempts, with creation/source-visibility undo/redo.
The creation task now uses bounded background native jobs with cooperative
cancellation, stale-input rejection and initial display reuse. Existing editing,
recompute and checked-display scheduling remain open.
Broader live/task workflows and default-command exposure remain required, as do
curved operands and the other Stage 2–4 acceptance gates.
The [explicit native solid codec](native-solid-topology-io.md) preserves actual
component/wire/shell/material topology without numerical rebuilding, with restored
validation flags false. It is a prerequisite, not completion of the dedicated
solid property/owner and FCStd workflow.

## Original prototype phases

| Phase | Delivered | Remaining acceptance gap |
|---|---|---|
| 1: architecture | Source investigation, three-architecture comparison, executable mesh-only OCCT probe | Native-face architecture remains a separate program |
| 2: geometry engine | Independent C++ evaluator, derivatives, local projection, sampled/refined rays, domains and tolerances | Global closest-point/root completeness and singularity robustness are not proven |
| 3: viewport | Editable addon and native feature, retained nodes, placement, persistence, quality controls, opt-in background extraction | Native semantic picking and whole-workflow responsiveness |
| 4: OCCT proof of concept | Dual representation, core signed-distance evaluator, mixed fields and live Part inputs | General implicit B-Rep faces and ordinary OCCT Boolean dispatch |
| 5: geometry operations | Classification/projection/rays, sampled plane/cylinder contours, domain-clipped display, capped band fields, mixed field intersection | General trimming topology, tangencies/coincidence/singular intersections, validated CAD solid results |
| 6: gyroid solid | Mathematical `abs(F)<=t` and `F<=c0`, finite bounding fields, closed-mesh regressions and Part field intersection | Uniform physical wall thickness and manufacturing-quality B-Rep conversion |
| 7: performance | Evaluation/extraction baselines, native sampling comparison, GUI/intersection benchmark tools | Hardware presentation FPS, repeated statistical sweeps, isolated allocation accounting, large-model scaling |

## Remaining native-development gates

The restricted [native mixed-box increment](native-mixed-box.md) implements
Common/BoxMinusField boundary compounds with closed seams and planar caps plus
live FreeCAD recipes. It does not close the general trimming, conforming meshing,
implicit-solid validation or manufacturing acceptance gates below.
Native [cap/seam meshing preparation](native-mesh-constraints.md) now shares
canonical edge/vertex samples between conventional cap triangles and oriented
implicit trim wires. The newer [trim-conforming candidate](native-conforming-preview.md)
triangulates the implicit surface and matches those seams, with one-sided field
distance bounds and combinatorial closure. Coverage, embeddedness, native-solid
validation and broader operands remain required before those gates can close.
An explicit [mesh contact audit](native-mesh-contact-audit.md) now checks all cap/
implicit pairs with exact integer predicates on stored binary64 coordinates,
including adjacent triangles. This is mesh-only evidence: coverage/clipping
and implicit-aware native-solid validation remain required.
The newer [clipped proximity check](native-clipped-coverage.md) partitions the
whole box/faces and checks reverse clipped witnesses against the 0.1 mm target.
This is two-sided numerical proximity, not preserved topology, minimum feature
size/reach or native-solid validation.
The [box-stratified regularity check](native-box-regularity.md) now checks
whole-domain gradients and face/edge transversality independently of meshes.
Stored trim correspondence, topology/reach and native-solid validation still
remain required; regularity alone does not complete those gates.

1. Geometry property and graph system: a dedicated versioned complex-geometry
   property; shared-node JSON/DAG persistence; unit contracts; graph/task UI;
   robust migration fixtures. Document links already provide a native dependency
   DAG, but do not replace every one of these contracts.
   Constant/linear scalar controls, variable level-set bands and smooth unions
   are now implemented with analytic gradients and conservative bounds. Widths
   remain field-valued; automatic unit checking and physical thickness are open.
2. Sampling and interaction: true variable-resolution surface cells with shared
   transition vertices; measured approximation/error criteria; topology stress
   tests; semantic field picking; background GUI jobs with cancellation, stale-job
   suppression and memory budgets. Opt-in background extraction with bounded
   concurrency and stale suppression is implemented; whole-workflow scheduling,
   global memory budgets and richer progress remain open.
   One-shot viewport field probes now work with placed objects and both camera
   types; integrated semantic selection, snapping and stable subelements remain open.
3. Geometry robustness: arbitrary plane/domain clipping, consistent boundary
   loops, singular and tangent intersection handling, and validated distance
   estimates before exposing millimetre wall-thickness controls.
   Native graph ray queries now report domain-clipped sampled roots and explicit
   bracket widths/residuals, including live mixed fields; this does not complete
   tangency handling, certified intersection completeness or semantic selection.
   Native local stationary projection and sampled mesh-quality reports are also
   implemented. Their explicit failures/interior residuals expose numerical
   limitations, without certifying nearest points, topology or Hausdorff error.
4. Manufacturing bridge: local charts, fitted B-spline patches with common
   boundaries and p-curves, sewing/healing, shape checks, quantified geometric
   error and explicit failure policies. Test OCCT Booleans and STEP round-trips
   only on validated approximations, retaining implicit provenance. A mesh
   or triangle-face conversion does not pass this gate.
   Native local height charts and individually fitted open B-spline faces are
   now implemented with sampled residual/normal checks and open-face Part/STEP
   tests. This is a first prototype, not completion of the gate above.
5. OCCT-native representation: geometry ownership and persistence independent of
   `Geom_Surface`, face/adaptor dispatch, chart/trimming conventions, robust
   intersections and BOP integration. Require conformance tests and a full OCCT
   toolkit build/regression run; a new class name or triangulation is insufficient.
   The first native field-backed face storage/query layer now exists in OCCT 8.1
   with 24 isolated conformance cases and UV/generic-persistence rejection guards.
   Explicit schema-1 single-face sphere/gyroid persistence is implemented;
   Restricted analytic sphere trimming, two-sphere Boolean boundaries and a
   sphere-specific geometric-bound mesher are implemented. An in-tree Python
   adapter exposes editable GUI examples via an isolated OCCT 8 worker on both
   tested FreeCAD ABI builds. This is not a validated solid or Part handle.
   The user's target is 0.1 mm geometric accuracy; general native trimming/BOP,
   field graph/topology persistence, full-toolkit conformance and general Part
   consumers remain open. Restricted native in-process ownership,
   queries, sphere operations, primitive serialization and opt-in GUI display
   are now tested; see `in-process-native-faces.md` and `native-sphere-operations.md`.
   Native gyroid/plane-rectangle sections now have derivative-bound acceptance
   and explicit unresolved status. Restricted closed-loop Z splits share
   approximate seam edges; contour GUI previews are tested. Open/domain-side
   seams and conforming surface meshing remain open, as do general mixed BOP
   and whole-model validity. Strict OCCT build-tree selection/preflight, required
   toolkit builds and targeted FreeCAD OCCT 8 command-line/GUI migration checks
   now pass. Restricted in-process native faces are exposed explicitly, not as
   Part shapes; trimmed-topology persistence and general consumers remain open. See
   `native-gyroid-sections.md` for the four requested steps' actual status.
6. Cross-workbench and scale: explicit consumer contracts for FEM/CAM/mesh,
   container/App::Link and cross-document behavior, systematic cache budgets,
   optional SIMD/GPU backends and reproducible accuracy/performance measurements.

Continue these stages as separately verified changes. Do not mark them complete
simply because FreeCAD and OCCT source trees have both been modified.
