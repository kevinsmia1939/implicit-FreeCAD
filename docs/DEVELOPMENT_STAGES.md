# Development stages and completion gates

This is an acceptance checklist, not a claim that the full nTop-class project
is complete. `PROGRESS.md` records tested increments; `HANDOFF.md` records how to
continue. The original prototype phases and the native-kernel roadmap have
different completion criteria.

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

1. Geometry property and graph system: a dedicated versioned complex-geometry
   property; shared-node JSON/DAG persistence; unit contracts; graph/task UI;
   robust migration fixtures. Document links already provide a native dependency
   DAG, but do not replace every one of these contracts.
2. Sampling and interaction: true variable-resolution surface cells with shared
   transition vertices; measured approximation/error criteria; topology stress
   tests; semantic field picking; background GUI jobs with cancellation, stale-job
   suppression and memory budgets. Opt-in background extraction with bounded
   concurrency and stale suppression is implemented; whole-workflow scheduling,
   global memory budgets and richer progress remain open.
3. Geometry robustness: arbitrary plane/domain clipping, consistent boundary
   loops, singular and tangent intersection handling, and validated distance
   estimates before exposing millimetre wall-thickness controls.
4. Manufacturing bridge: local charts, fitted B-spline patches with common
   boundaries and p-curves, sewing/healing, shape checks, quantified geometric
   error and explicit failure policies. Test OCCT Booleans and STEP round-trips
   only on validated approximations, retaining implicit provenance. A mesh
   or triangle-face conversion does not pass this gate.
5. OCCT-native representation: geometry ownership and persistence independent of
   `Geom_Surface`, face/adaptor dispatch, chart/trimming conventions, robust
   intersections and BOP integration. Require conformance tests and a full OCCT
   toolkit build/regression run; a new class name or triangulation is insufficient.
6. Cross-workbench and scale: explicit consumer contracts for FEM/CAM/mesh,
   container/App::Link and cross-document behavior, systematic cache budgets,
   optional SIMD/GPU backends and reproducible accuracy/performance measurements.

Continue these stages as separately verified changes. Do not mark them complete
simply because FreeCAD and OCCT source trees have both been modified.
