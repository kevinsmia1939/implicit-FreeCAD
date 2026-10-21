# implicit-FreeCAD

An experimental **FreeCAD fork for implicit modelling**, working toward native
field-based workflows, lattices, and mixed implicit/B-Rep operations. The
long-term target is an nTop-class implicit modelling system.

For editable primitive modelling, use the new [Implicit Modelling workbench](docs/implicit-modelling-workbench.md):
one toolbar icon creates a native 10 × 10 × 10 mm gyroid with editable dimensions,
period and level-set value in the tree's property editor.
The OCCT-8 workbench's experimental native-boundary menu now demonstrates
[topology-backed edge/vertex selection](docs/native-edge-vertex-selection.md),
distinct from the simple default field preview.

New: [native mixed gyroid/Part-box boundaries](docs/native-mixed-box.md), with
intersection/subtraction, shared box-edge roots and trim wires, planar caps and live
FreeCAD recipes. An opt-in [trim-conforming surface candidate](docs/native-conforming-preview.md)
now joins the implicit mesh to shared cap/seam nodes, with a separate
[exact mesh contact audit](docs/native-mesh-contact-audit.md) and
[whole-domain clipped proximity checks](docs/native-clipped-coverage.md).
Mesh-independent [field-box regularity checks](docs/native-box-regularity.md)
now cover volume/face/edge/corner strata. A separate
[stored trim-graph correspondence check](docs/native-trim-correspondence.md)
compares shared topology and full seam/p-curve geometry with fresh reference traces.
A [whole-box topology witness](docs/native-box-topology.md) now checks a numerical
field-to-nodal-linear homotopy and reports reference components/boundary loops.
A [closed region reference](docs/native-region-topology.md) adds shared planar
caps and groups boundary shells by retained material connectivity.
[Stored cap attachment](docs/native-cap-attachment.md) now maps conventional cap
regions to that reference using common retained points beyond seam uncertainty.
[Dual-cap trim attachment](docs/native-trim-attachment.md) now identifies contour
arcs and matches complete directed native/reference trim wires.
[Genuine native shells](docs/native-shells.md) now use exact field-backed faces,
checked component splitting and native manifold topology checks. Shell
construction alone does not establish geometric/solid validity or Part compatibility.
An explicit [restricted native-solid factory](docs/native-solids.md) now adds
whole-cap/edge/vertex tolerance checks before constructing actual material-region
solids. Its numerical implicit-aware validation does not qualify generic Part
consumers or curved operands.
An [explicit native solid topology codec](docs/native-solid-topology-io.md) now
preserves those actual result solids without recipe rebuilding; restored results
remain unverified. A [dedicated solid property](docs/native-solid-property.md)
now supplies FCStd topology and native subelements. The
[live native-solid workflow](docs/live-native-solids.md) adds source-driven
recompute and actual component-aware rendering/picking under the workbench's
experimental Native solids menu; the default Add Gyroid command is unchanged.
The [native-solid task editor](docs/native-solid-editor.md) now adds staged build
controls/shared-source operation, independent geometry depth, rollback and failure
recovery. Primitive period/level and box dimensions can be staged with shared
native-result updates and preserved source expressions. It preserves legacy
documents and supports the shifted 20-mm fixture at 0.1 mm. The native-solid
creation command now offers tolerance, depth and work settings before the first
build, with atomic failure/retry. Its [background build path](docs/native-solid-jobs.md)
keeps Cancel available and rejects stale results; existing editing/recompute and
checked display still run synchronously. General placed/curved operands remain unfinished.
Its [default corner identities and local material cones](docs/native-corner-germs.md)
now have checked kernel queries. The [lattice-node plane tracer](docs/lattice-plane-sections.md)
resolves all six default cube faces, retaining isolated point contacts separately.
The later [corner-aware native-solid pipeline](docs/native-corner-solids.md)
now constructs and persists actual restricted default-gyroid solids at 0.1 mm,
with [opt-in live rendering/selection](docs/native-corner-mesh.md) and
[corner-aware clipped proximity](docs/native-corner-coverage.md).
An optional [checked display mode](docs/native-checked-display.md) checks the
actual binary32 upload buffers for two-way proximity and exact triangle contacts.
It is source-local numerical evidence, not native-solid/world-rendering validation;
the default Add Gyroid command still uses the older field preview.
General curved operands, broader native-solid qualification and the remaining
Stage 4 integration gates remain open.

This repository is the development umbrella: the actual FreeCAD fork is pinned
in the `FreeCAD/` submodule on
[`feature/native-implicit`](https://github.com/kevinsmia1939/FreeCAD/tree/feature/native-implicit).
The root also contains
the standalone geometry engine, prototype addon, benchmarks, and project notes.
`OCCT/` contains the core signed-distance extension used by mixed field graphs,
on `feature/implicit-distance`, plus the experimental OCCT 8.1 native field-face
storage/query and primitive persistence layer. Restricted sphere trimming and
Boolean boundaries now have [experimental GUI examples](docs/native-sphere-operations.md)
through a separate OCCT 8 worker. Both the preserved OCCT 7.9 build and a separate
OCCT 8.1-linked command-line/GUI build are tested; these worker examples are not
general implicit Booleans or validated Part solids.
[Native gyroid-plane sections](docs/native-gyroid-sections.md) now have geometric-
bound curve previews and a restricted closed-loop split prototype. General
gyroid surface trimming/Booleans and native implicit-face Part/persistence
integration remain open.

[In-process native faces](docs/in-process-native-faces.md) are now available in
the OCCT 8 build: immutable handles, placed queries, restricted sphere operations,
bounded display meshing and explicit primitive serialization. An opt-in native
GUI backend retains real OCCT face handles; they are not ordinary Part shapes.

**Continuing development? Start with [the handoff](docs/HANDOFF.md).** It records
completed work, source locations, exact build/test commands, tools, known gaps,
and the next implementation milestones. [AGENTS.md](AGENTS.md) provides a short
entry point for coding agents.

See [the architecture report](docs/architecture.md) for the OCCT findings and
[the roadmap](docs/roadmap.md) for the proposed progression toward interoperable
CAD operations. [Development-stage acceptance gates](docs/DEVELOPMENT_STAGES.md)
distinguish delivered prototypes from unfinished production/kernel work.
The [core integration plan](docs/core-integration-plan.md)
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

The FreeCAD submodule tracks `feature/native-implicit` in
`kevinsmia1939/FreeCAD`; OCCT tracks `feature/implicit-distance` in
`kevinsmia1939/OCCT`. See [current implementation progress](docs/PROGRESS.md)
for the core changes and remaining milestones.

## Build and test

To try the existing development build interactively, run
`bash tools/launch_demo.sh`. See [the hands-on guide](docs/TRY_IT.md) for editable
examples, display settings, save/reload, and current limitations.
Use `bash tools/launch_demo.sh --occt8` for the separately built OCCT 8.1 GUI.

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
- box signed-distance and field-band nodes for bounded gyroid regions;
- constant/linear [scalar controls](docs/scalar-controls.md), spatially varying
  level-set bands and smooth field unions;
- union, intersection, difference, translation, and offset field nodes;
- versioned JSON field-graph persistence in `.FCStd` documents;
- Python evaluation, gradient, classification, graph validation, and graph
  getter/setter APIs;
- native ray queries and local stationary projection, with explicit numerical
  diagnostics; [viewport field probes and sampled mesh-quality inspection](docs/native-projection-and-quality.md);
- [field-bound-controlled extraction](docs/controlled-extraction.md) with explicit
  refinement limits, and an [experimental local B-spline face bridge](docs/local-patch-bridge.md);
- indexed C++ surface extraction for native field graphs, exposed through
  `Implicit.tessellate(feature, resolution)`;
- conservative octree sampling through `Implicit.tessellateAdaptive`, plus
  scalar batch evaluation and compiled-graph caching;
- a native C++ view provider with retained Coin buffers and display properties;
- opt-in [background viewport meshing](docs/background-meshing.md), with bounded
  jobs, cancellation and stale-result suppression;
- a Native Implicit workbench with editable primitives, bands and live linked Booleans;
- optional live Part inputs, B-Rep signed-distance snapshots and mixed field
  composition using the OCCT fork extension;
- placement-aware document dependencies and cancellable Python extraction
  that releases the interpreter lock;
- `FreeCADCmd` smoke tests covering numerical behavior, mesh connectivity,
  orientation, clipping, input validation, and save/reload.

Native extraction returns local-coordinate vertices, normals, and triangles.
It uses a uniform grid with shared edge vertices and refined field roots. See
[the extraction API](docs/native-field-graph.md#surface-extraction) for the
contract, limits, and examples. The native view provider uses this cache.
The older addon remains available as a separate gyroid-only prototype.

This foundation does not yet make implicit geometry interchangeable with every
Part/B-Rep operation. Remaining stages include variable-resolution rendering,
field-aware selection, a tolerance-controlled B-spline manufacturing bridge,
and genuine OCCT implicit-face topology/Boolean dispatch.

## Environment observed

- openSUSE Tumbleweed, Linux x86-64
- GCC 16.2.1, CMake 4.4.3, Ninja 1.13.2, Python available
- installed FreeCAD: 1.1.4, revision 20260928
- installed OCCT development package: 7.9.3
- source trees: FreeCAD 27.1.0-dev on `feature/native-implicit` and
  OCCT 8.1.0-dev1 (`3d097a0328`); `git submodule status` shows exact revisions

The native module and `FreeCADCmd` were built and runtime-tested; fork GUI
verification is covered by `FreeCAD/src/Mod/Implicit/Tests/gui_smoke.py`.
Earlier addon GUI testing used installed FreeCAD. The new OCCT class is compiled
against installed OCCT 7.9.3 using isolated staged headers. The complete OCCT 8.1
source toolkit has not been rebuilt. A FreeCAD runtime remains optional for the
standalone library.

## Current limitations

The OCCT fork now includes an experimental [native implicit-face foundation](docs/occt-native-faces.md),
tested separately against OCCT 8.1 at a [0.1 mm geometric tolerance target](docs/accuracy-policy.md).
The preserved OCCT-7.9 build uses field composition or an isolated worker, not
in-process native implicit faces. The wholly OCCT-8 build has native ownership,
restricted gyroid/box boundaries, topology persistence and the opt-in conforming
preview. Generic implicit-face Booleans, curved operands and validated native
solids remain unfinished; older mixed-field operations remain a separate path.

- Surface extraction is a visualization approximation. The standalone/addon
  baseline duplicates triangle vertices; native extraction shares vertices.
  Computational-domain boundaries are not capped; explicit box fields can
  supply cap surfaces. Grid sampling can miss thin components,
  tangencies, and multiple roots along an edge. There is no general guarantee
  of watertightness, geometric error, or correct topology for arbitrary fields.
- Ray intersection detects sign-changing roots and sampled roots; unsampled
  tangencies and thin crossings may be missed. Native rays require exact zeros
  for unbracketed sampled hits; see the separate native query tolerance contract.
- Projection finds a local constrained stationary point, not a certified global
  nearest point. The native solver constrains accepted iterates to the domain
  and returns explicit convergence/failure diagnostics. The older standalone
  solver requires a separate domain check. Viewport probes do not replace
  FreeCAD semantic selection, and sampled quality reports do not certify solids.
- Bounded plane-patch and cylinder intersection curves are implemented in the
  standalone engine. General trimming topology, validated manufacturing solids,
  physical wall thickness and whole-solid STEP export remain roadmap work.
  Local fitted open B-spline faces now support tested Part clipping and STEP
  round-trips, not global implicit conversion or a manufacturing certificate. Mixed Booleans
  operate in field space; arbitrary implicit faces are not yet accepted by
  OCCT's existing B-Rep Boolean algorithms.

## License

Project code is licensed under **LGPL-2.1-or-later**, matching FreeCAD. The
[LICENSE](LICENSE) contains the same LGPL 2.1 text as `FreeCAD/LICENSE`.
Upstream and third-party components retain their own license notices,
including the OCCT submodule.

