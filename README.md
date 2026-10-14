# Native implicit geometry for FreeCAD

This repository contains a working implicit-geometry vertical slice and the
first native FreeCAD integration. It combines a standalone C++ field engine,
numerical surface extraction, tests and benchmarks, an editable FreeCAD/Coin3D
visualization addon, and a pinned FreeCAD branch containing the native
`Implicit` module. Triangle meshes are explicitly transient display caches; they
are not presented as OCCT B-Reps.

See [the architecture report](docs/architecture.md) for the OCCT findings and
[the roadmap](docs/roadmap.md) for the proposed progression toward interoperable
CAD operations. The [core integration plan](docs/core-integration-plan.md)
defines which FreeCAD and OCCT seams must change for an nTop-class target.
The [native field-graph documentation](docs/native-field-graph.md) describes the
current persistence schema and Python API. The
[openSUSE dependency audit](docs/build-dependencies-opensuse.md) records the
packages used for the in-tree FreeCAD module build.

## Clone

Clone recursively so the pinned FreeCAD and OCCT source revisions are checked
out with the standalone project:

```sh
git clone --recurse-submodules https://github.com/kevinsmia1939/implicit-FreeCAD.git
cd implicit-FreeCAD
```

The FreeCAD submodule tracks the `feature/native-implicit` branch in
`kevinsmia1939/FreeCAD`. The OCCT submodule is an unmodified reference used for
architecture research and future integration work.

## Build and test

The standalone component has no dependency on OCCT or FreeCAD:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

When `FreeCADCmd` is installed, CMake automatically adds the FCStd
create/recompute/save/reload smoke test. To run the GUI/Coin integration test on
a desktop session:

```sh
FreeCAD -P "$PWD/addon" tests/freecad_gui_smoke.py
```

Run the benchmark with the number of grid cells sampled per gyroid unit cell:

```sh
./build/implicit_benchmark 6
```

It emits CSV for 1, 3, 5, and 10 unit cells. Runtime and memory grow with the
sample grid volume. The extractor currently emits a deliberately simple triangle
soup, so its reported memory is an upper-bound baseline for later indexed/cached
implementations. The latest captured run is in `benchmark/results.csv`.

## FreeCAD addon

Copy or symlink `addon/ImplicitGyroid` into the user's FreeCAD `Mod` directory,
restart FreeCAD, select the **Implicit Geometry** workbench, and run
**Implicit Gyroid**. The saved document retains the equation parameters,
finite domain, placement, resolution, color, and display mode. On reload the
addon recomputes the visualization cache.

The addon exposes:

- `Period` and dimensionless `Level` equation parameters;
- `LengthX`, `LengthY`, and `LengthZ` finite-domain dimensions;
- `Placement` for position and orientation;
- `Resolution` for the display quality/performance tradeoff;
- `SurfaceColor` and Shaded, Wireframe, and Flat Lines display modes.

Changing geometry properties marks the feature for recompute. Camera motion does
not recompute geometry. Coin coordinate, normal, and index nodes are allocated
once and their field buffers are updated after recompute.

## Native FreeCAD module

The `FreeCAD` submodule contains the first in-tree implementation under
`src/Mod/Implicit`. Configure FreeCAD with `BUILD_IMPLICIT=ON` to build it. The
module currently provides:

- native `Implicit::Field`, `Implicit::Shape`, and `Implicit::Feature` types;
- sphere and gyroid primitives;
- union, intersection, difference, translation, and offset field nodes;
- versioned JSON field-graph persistence in `.FCStd` documents;
- Python evaluation, gradient, classification, graph validation, and graph
  getter/setter APIs;
- a `FreeCADCmd` smoke test covering numerical behavior and save/reload.

This foundation does not yet make implicit geometry interchangeable with every
Part/B-Rep operation. The next integration stages are adaptive native rendering,
field-aware selection, mixed implicit/B-Rep conversion and Booleans, and a
tolerance-controlled B-spline B-Rep bridge.

## Environment observed

- openSUSE Tumbleweed, Linux x86-64
- GCC 16.2.1, CMake 4.4.3, Ninja 1.13.2, Python available
- installed FreeCAD: 1.1.4, revision 20260928
- installed OCCT development package: 7.9.3
- source trees: FreeCAD 27.1.0-dev (`da2682dcd9` on
  `feature/native-implicit`) and OCCT 8.1.0-dev1 (`3d097a0328`)

The FreeCAD source tree was modified, built, and runtime-tested; its native
module branch is published in the project fork. OCCT remains unmodified. A
FreeCAD runtime remains optional for the standalone library. The addon has also
been validated in both `FreeCADCmd` and the live Coin3D GUI.

## Current limitations

- Marching tetrahedra is a visualization approximation and duplicates triangle
  vertices; it is neither a watertight bounded solid nor a B-Rep.
- Ray intersection detects sign-changing roots and sampled roots. Even-multiplicity
  tangencies can be missed unless a sample falls within tolerance.
- Projection is a local constrained Newton solve; convergence to the global
  closest component is not guaranteed for arbitrary starting points. Projection
  operates on the mathematical surface; callers must test the returned point
  against the finite domain when they require bounded-surface semantics.
- General plane-box clipping, cylinder intersection, trimming topology, solid
  construction, OCCT Boolean interoperability, physical wall thickness, and
  STEP export remain roadmap work. Bounded plane-patch intersection curves are
  implemented in the standalone engine.

