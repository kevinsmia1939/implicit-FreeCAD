# Development handoff

Last updated: 2026-10-15. This file is the entry point for a developer or coding
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
results and native OCCT implicit-face operations are still future work.

| Location | Purpose |
|---|---|
| `FreeCAD/` | Actual FreeCAD fork; native work on `feature/native-implicit` |
| `OCCT/` | OCCT fork with TKTopAlgo signed-distance extension on `feature/implicit-distance` |
| `include/implicit/`, `src/` | Standalone C++17 evaluator and numerical algorithms |
| `addon/ImplicitGyroid/` | Editable Python gyroid feature and retained Coin scene graph |
| `tests/` | Standalone, addon command-line/GUI tests and OCCT bridge probe |
| `benchmark/` | Standalone timing program and recorded CSV baseline |
| `FreeCAD/src/Mod/Implicit/` | Native module, field graph, extraction, Python API, smoke tests |
| `docs/` | Architecture findings, capability roadmap, build notes, API and this handoff |
| `build/` | Ignored standalone build; optionally registers native tests |
| `build-freecad-implicit/` | Ignored native development build |

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

The native module and `FreeCADMainCmd` targets have been built successfully.
The unified CTest run passed **6/6 tests** on 2026-10-14, including both native
smoke tests; the mesh smoke test completed in approximately 0.44 seconds on this
host. That runtime is a regression-test observation, not a rendering benchmark.
That six-test result predates the current increments. The current combined run
passed **10/10 tests** before the live increment. The latest run passes
**19/19**, including chart/patch/STEP checks and synchronous/background mixed fork GUIs on Xvfb; see results
and benchmark observations in `PROGRESS.md`. OCCT is now modified. Its new extension is compiled against
installed OCCT 7.9.3 through isolated staged headers; a full OCCT 8.1 rebuild
has not been performed. The earlier addon GUI tests used installed FreeCAD.

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
the existing build on this machine rather than configuring another copy.

```sh
# Standalone build, with native tests when the native command exists.
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DIMPLICIT_BUILD_OCCT_DISTANCE=ON \
  -DIMPLICIT_NATIVE_FREECADCMD="$PWD/build-freecad-implicit/bin/FreeCADCmd"
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure

# Incremental native build; no installation into the system.
cmake -S FreeCAD -B build-freecad-implicit -DIMPLICIT_OCCT_SOURCE_DIR="$PWD/OCCT"
cmake --build build-freecad-implicit --target Implicit ImplicitScripts FreeCADMainCmd --parallel 2
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
string to clear a cached setting). NativeCmd now registers eight native tests;
the Part option adds two more. NativeGUI registers synchronous and async GUI
tests (requires DISPLAY). The OCCT extension adds its C++ checks, optional GTest,
and the native mixed snapshot test when NativeCmd is present. Other standalone,
addon and OCCT-probe tests depend on detected/configured components; inspect
`ctest --test-dir build -N` for the actual set. The complete host configuration
has 19 tests. Installed `FreeCADCmd` cannot import this fork's native module.
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
- The graph lives in `App::PropertyString`; there is no dedicated complex geometry
  property, shared-node JSON DAG format, graph editor or SIMD/GPU backend. Scalar batch
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
  face Booleans and a whole-solid manufacturing/STEP bridge remain open. Local
  B-spline open-face fitting, clipping and STEP round-trip are now tested.
- Both engines now support local projection and sampled rays. Native projection
  reports stationary-point convergence/failure, not a globally nearest point;
  hard creases, critical points and ill conditioning can fail. Rays can miss
  tangencies/thin components. Native ray roots carry bracket widths and residuals;
  viewport probes are not integrated semantic selection. Mesh-quality interior
  residuals and edge counts do not certify geometric error or solid validity.

## Next milestones and acceptance criteria

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
