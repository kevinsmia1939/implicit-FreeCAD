# Development handoff

Last updated: 2026-10-14. This file is the entry point for a developer or coding
agent continuing the project. It records the state of the code, not a claim
that all goals in the original brief are complete.

## Goal and repository layout

Build a FreeCAD fork with native implicit modelling, ultimately competitive
with nTop in field composition, lattices, performance, and interoperability.
The user's original brief is `instructions.txt`. Its initial Phases 1–3 have
working prototypes; later work added plane intersections and an in-tree native
FreeCAD module. Mixed B-Rep operations are still future work.

| Location | Purpose |
|---|---|
| `FreeCAD/` | Actual FreeCAD fork; native work on `feature/native-implicit` |
| `OCCT/` | Unmodified OCCT source for investigation; pinned at `3d097a0328` |
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

Native support initially lives in `FreeCAD/src/Mod/Implicit` with build-system
registration. Shared App/Gui/Part dispatch changes should follow demonstrated
integration needs. OCCT changes are deferred until there is evidence that a
native OCCT representation is worth its broad geometry/topology/Boolean impact.

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

The native module and `FreeCADMainCmd` targets have been built successfully.
The unified CTest run passed **6/6 tests** on 2026-10-14, including both native
smoke tests; the mesh smoke test completed in approximately 0.44 seconds on this
host. That runtime is a regression-test observation, not a rendering benchmark.
`BUILD_GUI=ON` is a configuration setting: the fork's GUI executable has **not**
yet been built or tested. The previously tested GUI was installed FreeCAD.
OCCT was neither modified nor built from the reference source; the native build
links the installed OCCT 7.9.3 libraries.

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
  -DIMPLICIT_NATIVE_FREECADCMD="$PWD/build-freecad-implicit/bin/FreeCADCmd"
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure

# Incremental native build; no installation into the system.
cmake --build build-freecad-implicit --target Implicit FreeCADMainCmd --parallel 2

# Native tests also run directly without enabling all FreeCAD developer tests.
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/native_smoke.py
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/mesh_smoke.py

# Existing addon GUI test, on a desktop session using installed FreeCAD.
FreeCAD -P "$PWD/addon" tests/freecad_gui_smoke.py

# Existing standalone benchmark; argument is cells per unit cell.
./build/implicit_benchmark 6
```

Without a native build, omit `IMPLICIT_NATIVE_FREECADCMD` (or set it to an empty
string to clear a cached setting). Standard CTest then has four tests on this
host; enabling the native executable adds two for six total. Installed
`FreeCADCmd` cannot import this fork's `Implicit` module. Python scripts can
print tracebacks even when FreeCADCmd exits successfully: CTest explicitly checks
for success markers and failure patterns for the two native tests.

`ENABLE_DEVELOPER_TESTS=OFF` avoids the dependency and cost of FreeCAD-wide
GoogleTest targets. Native module tests are registered inside FreeCAD only with
that option on; the umbrella registration is the tested alternative.

## Remaining limits

- Native graphs have no dedicated native GUI view provider, workbench commands,
  field-aware picking, cache invalidation machinery or background evaluation yet.
- The graph lives in `App::PropertyString`; there is no dedicated complex geometry
  property, shared-node DAG, graph editor, SIMD/GPU or batch evaluation API.
- The native extractor is a bounded uniform-grid baseline, **not adaptive**.
  Refining edge roots improves vertex residuals, not triangle-interior error or
  a Hausdorff bound. Grid cells can hide small components, tangencies and multiple
  crossings. Hard Boolean creases can be smoothed by interpolated vertex normals.
- Native extraction uses 2–64 cells per axis and limits vertices and triangles
  independently to one million. It retains full grid samples, a root map, and
  Python result copies. No asynchronous cancellation or GIL release is implemented.
- Root positions and native scalar APIs are local to the feature. Rendering must
  apply `Placement` once. Domain clipping does not generate caps. Sphere/shell
  connectivity checks do not guarantee topology for arbitrary graphs.
- Classification uses the field's sign and a field-value tolerance. Raw gyroid
  values and offsets are dimensionless; they are not distances in millimetres.
- No cylinder contours, general trimming topology, capped gyroid solids, uniform
  physical thickness, B-Rep-to-field evaluator, mixed OCCT Boolean or STEP bridge.
- Projection can converge to a local component; sampled ray roots can miss
  tangencies. These standalone algorithms have not all been ported into the
  native graph API.

## Next milestones and acceptance criteria

1. **Native feature display:** implement `ImplicitGui` view-provider registration,
   use indexed native extraction, retain Coin nodes, expose resolution/color/modes,
   rebuild only when relevant field/domain properties change, apply Placement
   once, and regenerate after FCStd restore. Build the fork GUI target and verify
   a composed graph, save/reload, buffer shrink, parameter edits, and camera motion.
2. **Adaptive extraction:** keep the uniform native extractor as a regression
   reference. Add conservative bounds/error criteria, shared vertices across
   refinement levels, transition handling, cancellation and explicit budgets.
   Test topology across coarse/fine cells and measure performance separately
   from viewport upload. Do not prune cells merely because all corners agree.
3. **Field usability:** batch evaluation, shared-node graph representation,
   dimensional contracts, more transforms, band/shell nodes and distance estimates.
   A level-set band must remain distinct from physical thickness.
4. **Geometry operations:** cylinder contour tracing, general domain boundaries,
   capped visualization solids and quantitative distance/offset validation.
5. **Mixed representation:** retain a source B-Rep link, sample signed distance or
   classification, compose with native fields, record sampling error and tolerance.
6. **Manufacturing bridge:** local charts and fitted B-spline patches, common
   boundaries/p-curves, sewing and shape validation, then OCCT Booleans and STEP
   export with measured approximation error and retained implicit provenance.
7. **Broader integration:** cross-workbench selection and recompute contracts,
   FEM/CAM consumers, performance backends, and only then an evidence-based OCCT
   native geometry proposal. Full details are in the roadmap/core integration plan.

## Publishing changes

There are two relevant repositories. Commit and push native changes inside
`FreeCAD` first, then stage that submodule pointer alongside umbrella changes.
OCCT should stay clean unless an explicit implementation justifies changing it.

```sh
git -C FreeCAD switch feature/native-implicit
git -C FreeCAD status --short
git -C FreeCAD diff --check
# Add only the intended native module/build files, commit, then:
git -C FreeCAD push origin feature/native-implicit
# In the umbrella, stage intended changes including the FreeCAD gitlink.
git status --short
git diff --check
# Commit, then:
git push origin main
git submodule status
```

Keep this handoff current when completing work. Record tested outcomes and
separate configured options from targets actually built and run.
