# Development handoff

Last updated: 2026-10-16. This file is the entry point for a developer or coding
agent continuing the project. Read [`PROGRESS.md`](PROGRESS.md) for the latest
FreeCAD GUI, live Part dependencies, native projection/probes/quality, ray queries, background meshing,
benchmarks and OCCT core increments.
The user has authorized implementation, commits and pushes in both core source
trees. This file records the state of the code, not a claim
that all goals in the original brief are complete.
[`DEVELOPMENT_STAGES.md`](DEVELOPMENT_STAGES.md) maps the original phases to
delivered prototypes and explicit remaining production/kernel acceptance gates.

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

## Tools and development environment

Development host: openSUSE Tumbleweed, Linux x86-64. Observed versions:

| Tool/component | Version or role |
|---|---|
| GCC / C++ | 16.2.1; standalone C++17, native build C++23 |
| CMake / Ninja | 4.4.3 / 1.13.2; out-of-tree builds |
| ccache | Enabled by native CMake configuration |
| Python | 3.13.15; addon, smoke tests, API |
| Qt / PySide6 / Shiboken6 | 6.11.2 for this native build |
| Installed FreeCAD | 1.1.4, revision 20260928; addon runtime/GUI validation |
| Fork FreeCAD base | 27.1.0-dev, upstream base `d732e2f13a` |
| Installed OCCT | 7.9.3 development package; linked by native build/probe |
| OCCT source reference | 8.1.0-dev1, `3d097a0328` |
| Coin / Pivy | Native configuration uses bundled 4.0.10 / 0.6.11 |
| PyCXX | Bundled 7.1.11 for native Python bindings |
| JSON | FreeCAD's bundled nlohmann JSON |
| Git / GitHub CLI (`gh`) | Commits, branches, submodules, GitHub repo creation/push/settings |
| ripgrep (`rg`), shell, `apply_patch` | Source inspection and edits during assisted development |
| CTest | Standalone and native regression orchestration |
| clang-format | Available; use the FreeCAD formatting configuration |
| Xvfb | Available for isolated native GUI regression runs |
| GoogleTest | 1.18.0 available; new OCCT tests also run through the compatibility build |

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
The native-face entry now contains 35 GTest cases. See `occt-native-faces.md` for its
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
