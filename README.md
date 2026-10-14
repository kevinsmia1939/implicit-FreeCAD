# implicit-FreeCAD

An experimental **FreeCAD fork for implicit modelling**, working toward native
field-based workflows, lattices, and mixed implicit/B-Rep operations. The
long-term target is an nTop-class implicit modelling system.

This repository is the development umbrella: the actual FreeCAD fork is pinned
in the `FreeCAD/` submodule on
[`feature/native-implicit`](https://github.com/kevinsmia1939/FreeCAD/tree/feature/native-implicit).
The root also contains
the standalone geometry engine, prototype addon, benchmarks, and project notes.
`OCCT/` is a pinned reference checkout; its source has not been modified.

**Continuing development? Start with [the handoff](docs/HANDOFF.md).** It records
completed work, source locations, exact build/test commands, tools, known gaps,
and the next implementation milestones. [AGENTS.md](AGENTS.md) provides a short
entry point for coding agents.

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
- indexed C++ surface extraction for native field graphs, exposed through
  `Implicit.tessellate(feature, resolution)`;
- `FreeCADCmd` smoke tests covering numerical behavior, mesh connectivity,
  orientation, clipping, input validation, and save/reload.

Native extraction returns local-coordinate vertices, normals, and triangles.
It uses a uniform grid with shared edge vertices and refined field roots. See
[the extraction API](docs/native-field-graph.md#surface-extraction) for the
contract, limits, and an example. Rendering those native features in a dedicated
view provider is the next milestone; the existing addon renders gyroids today.

This foundation does not yet make implicit geometry interchangeable with every
Part/B-Rep operation. The next integration stages are adaptive native rendering,
field-aware selection, mixed implicit/B-Rep conversion and Booleans, and a
tolerance-controlled B-spline B-Rep bridge.

## Environment observed

- openSUSE Tumbleweed, Linux x86-64
- GCC 16.2.1, CMake 4.4.3, Ninja 1.13.2, Python available
- installed FreeCAD: 1.1.4, revision 20260928
- installed OCCT development package: 7.9.3
- source trees: FreeCAD 27.1.0-dev on `feature/native-implicit` and
  OCCT 8.1.0-dev1 (`3d097a0328`); `git submodule status` shows exact revisions

The native module and `FreeCADCmd` were built and runtime-tested. The build is
configured with GUI support, but a complete fork GUI executable has not yet
been built. Addon GUI testing used the installed FreeCAD. A FreeCAD runtime
remains optional for the standalone library.

## Current limitations

- Surface extraction is a visualization approximation. The standalone/addon
  baseline duplicates triangle vertices; native extraction shares vertices.
  Domain boundaries are not capped. Grid sampling can miss thin components,
  tangencies, and multiple roots along an edge. There is no general guarantee
  of watertightness, geometric error, or correct topology for arbitrary fields.
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

## License

Project code is licensed under **LGPL-2.1-or-later**, matching FreeCAD. The
[LICENSE](LICENSE) contains the same LGPL 2.1 text as `FreeCAD/LICENSE`.
Upstream and third-party components retain their own license notices,
including the OCCT submodule.

