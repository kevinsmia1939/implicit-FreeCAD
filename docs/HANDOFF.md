# Development handoff

Last updated: 2026-10-18. This file is the entry point for a developer or coding
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
| GDB | Exported-function/exception debugging; OCCT Release lacks source-line symbols |
| CTest | Standalone and native regression orchestration |
| clang-format | Available; use the FreeCAD formatting configuration |
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
The native-face entry now contains 76 GTest cases. See `occt-native-faces.md` for its
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
