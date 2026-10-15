# Core integration plan for an nTop-class FreeCAD

## Direct answer

Yes, FreeCAD must eventually gain upstream/core-level implicit support to make
implicit modeling a native workflow. OCCT must be modified only if implicit
geometry is required to behave as a first-class `TopoDS`/B-Rep representation
inside OCCT's existing algorithms. Those are related but separate decisions.

The recommended target is a dual-kernel FreeCAD architecture:

1. a native FreeCAD implicit/field module owns field graphs, evaluation,
   persistence, visualization, selection, and implicit operations;
2. OCCT continues to own parametric curves, surfaces, B-Reps, drawings, and
   conventional manufacturing geometry;
3. explicit, tolerance-bearing bridges convert or combine representations only
   when an operation requires it.

This is not a permanent addon-only proposal. The addon is the low-risk place to
stabilize the geometry contracts before upstreaming them.

## Why changing only OCCT is not enough

nTop-style modeling is primarily a field-computation system, not merely a new
surface class. A useful kernel needs scalar/vector field expression graphs,
bounded domains, transforms, composition, smooth and sharp Boolean operators,
distance-field operations, spatially varying parameters, batch evaluation,
derivatives, caches, and adaptive visualization. `Geom_Surface` represents a
two-parameter map and cannot express that model.

Likewise, deriving a class from `Geom_Surface` would still leave FreeCAD's
document model, property system, recompute graph, selection, view providers,
editors, Python API, serialization, and workbench commands unaware of fields.

## FreeCAD changes

### Stage F1: upstream Implicit module

This can initially be implemented as a new in-tree module without destabilizing
`App` or `Part`:

- `Implicit::Field` immutable expression nodes and an evaluation context;
- `Implicit::Shape` combining a field, finite domain, tolerance policy, and
  inside convention;
- `Implicit::PropertyShape`, derived from FreeCAD's complex-geometry property
  infrastructure, with versioned FCStd persistence;
- `Implicit::Feature` and Python bindings;
- `ImplicitGui::ViewProviderShape` with adaptive cached rendering and field-aware
  picking;
- task panels and commands for primitives, transforms, Boolean composition,
  shells, lattices, clipping, and conversion;
- CPU batch evaluation first, followed by a backend-neutral route to SIMD/GPU.

This avoids putting an implicit object into `Part::Feature::Shape`, which is a
`PropertyPartShape` containing a `TopoDS_Shape`. The inspected FreeCAD source
shows that this property owns `TopoShape` persistence and all ordinary Part
operations dispatch through OCCT.

### Stage F1 implementation status

The first in-tree foundation now exists under `FreeCAD/src/Mod/Implicit`:

- `BUILD_IMPLICIT` and native module registration in FreeCAD's build;
- immutable `Implicit::Field` implementations for gyroid and sphere fields;
- `Implicit::Shape`, combining a field, finite domain, tolerance, and inside
  convention without pretending to be a `TopoDS_Shape`;
- a registered `Implicit::Feature` with persistent FreeCAD properties for field
  type, parameters, domain, classification policy, and schema version;
- a versioned field-expression graph supporting sphere and gyroid primitives,
  translation, offsets, and hard union/intersection/difference composition;
- Python entry points for scalar evaluation, analytical gradient evaluation,
  and point classification;
- an automated `FreeCADCmd` smoke test covering recompute and `.FCStd`
  save/reload in addition to numerical behavior.
- native indexed surface extraction for arbitrary supported graphs, exposed as
  `Implicit.tessellate`, with shared roots, gradient normals, input/resource
  limits, and a second smoke test for residuals, orientation, connectivity,
  domain clipping, and restoration.

Subsequent increments add a native C++ GUI view provider and workbench, compiled
graph/display caches, a batch query API, conservative octree pruning, band/box
nodes and optional OCCT signed-distance snapshot fields. See
[`PROGRESS.md`](PROGRESS.md) for their tested contracts and current limitations.

The live-operation increment adds persistent document-level operand links,
placement/sign-aware field composition, direct single-solid Part dispatch and
live B-Rep distance fields. Source failures invalidate skipped downstream caches.
Python extraction releases the GIL and accepts cancellation tokens. The mixed
GUI and persistence paths are tested; `Part`, `PartGui`, and `PartScripts` are
built on the development machine. These changes do not provide an OCCT implicit
face or a manufacturing B-Rep approximation.

Opt-in background viewport extraction is now implemented with a bounded worker,
coalesced requests, cancellation and snapshot/generation-checked main-thread
uploads. It does not move document recompute, B-Rep validation or Coin uploads off
the GUI thread. Global memory budgets and broader scheduling remain future work.

This is the document-model and kernel seam, not completion of Stage F1. A
dedicated complex-geometry property, shared-node/DAG representation, richer
field nodes, field-aware selection and variable-resolution surface extraction
remain to be implemented. The uniform-grid native extractor remains the
reference backend alongside octree-pruned sampling. Native geometry revisions
now drive display-cache updates. The older addon remains a separate prototype.
The native `Shape::intersectRay` and Python `Implicit.intersectRay` provide
domain-clipped sampled roots with explicit bracket widths and field residuals.
This is a query seam for future picking, not GUI selection or certified OCCT
intersection topology. See [native ray queries](native-ray-queries.md).
Local stationary projection, a selected-field one-shot viewport probe and
sampled tessellation diagnostics are now implemented. They establish query and
inspection seams, not global closest-point certification, integrated semantic
selection or error-controlled extraction. See
[projection and quality contracts](native-projection-and-quality.md).
The experimental native chart / Part B-spline bridge now produces individually
checked local open faces, with conventional face clipping and STEP round-trip
tests. It does not provide shared patch topology, a manufacturing solid, global
coverage or native OCCT implicit face dispatch. See
[local patch bridge](local-patch-bridge.md).
The current
graph schema is documented in `docs/native-field-graph.md`.
Build commands, toolchain, tested scope, and the continuation checklist are in
[`HANDOFF.md`](HANDOFF.md).

### Stage F2: cross-workbench integration

Once the contracts are stable, FreeCAD's upstream modules need representation
dispatch rather than assuming every solid is a `TopoDS_Shape`:

- mixed Part/Implicit Boolean commands;
- generic bounding boxes, placement, selection, and element references;
- conversion-result provenance and tolerance metadata;
- recompute scheduling and cancellation for expensive field graphs;
- background visualization-cache generation;
- FEM/CAM/mesh consumers that can request sampled fields or boundary meshes;
- document-level cache budgets and invalidation.

Some of this can remain in the new module; shared selection, property, and
cross-workbench dispatch may justify small changes to FreeCAD `App`, `Gui`, and
`Part` after the APIs have proven stable.

## OCCT changes if native implicit B-Rep is required

The user explicitly directed this work with a **0.1 mm geometric tolerance
target**. The first native representation/query increment now exists in OCCT 8.1:
`Geom_ImplicitField`, a `BRep_TImplicitFace` subtype, builder/tool dispatch and
field-aware queries, with guarded rejection of UV access and lossy persistence.
Explicit schema-1 single-face sphere/gyroid persistence is now implemented;
general B-Rep/OCAF persistence, including child topology, remains unsupported.
Restricted sphere-plane trimming and two-sphere Boolean boundary splitting now
have explicit native predicates, exact shared circle edges and source mapping.
Sphere-only geometric-bound meshing and an isolated-worker Python GUI adapter
are tested. They do not complete general trimming/BOP or in-process native Part
exposure; see [restricted sphere contracts](native-sphere-operations.md).
Native gyroid/plane sections now use derivative bounds and explicit unresolved
status. Restricted closed-loop Z splits share approximate seam edges with exact
field/plane predicates. Strict OCCT source-build selection is implemented in
FreeCAD, but the complete migration and general mixed BOP remain pending. See
[gyroid sections and current four-step gates](native-gyroid-sections.md).
The following checklist remains the full integration requirement, not a claim
that representation alone meets it. See [native faces](occt-native-faces.md).

The conventional OCCT representation is explicit. `BRep_TFace` stores a
`Handle(Geom_Surface)` plus triangulations. The executable bridge probe in this
repository demonstrates that a triangulation-only `TopoDS_Face` has no surface
for `BRepAdaptor_Surface` to load.

A real OCCT-native design would require at least:

1. **Geometry representation:** a field/implicit geometry hierarchy independent
   of `Geom_Surface`, with evaluation, gradient, bounds, transform, and
   persistence contracts.
2. **Face representation:** `BRep_TFace`, `BRep_Builder`, and `BRep_Tool` support
   for implicit geometry, including an unambiguous rule when parametric and
   implicit representations coexist.
3. **Trimming model:** an alternative to mandatory 2D p-curves, or a system of
   local charts with consistent p-curves. A 3D curve merely lying near F=0 is not
   sufficient robust topology.
4. **Adaptors and dispatch:** new implicit adaptors and changes throughout
   projection, extrema, classification, meshing, and intersection code.
5. **Intersection:** implicit/analytic, implicit/parametric, and
   implicit/implicit curve tracing with singularity, tangency, coincidence, and
   tolerance handling.
6. **Booleans:** pave filling, face splitting, point/region classification,
   result construction, validation, and history for mixed face types.
7. **Persistence:** ASCII/binary BRep and OCAF persistence with versioning.
8. **Exchange:** explicit approximation policies for STEP and other formats that
   cannot preserve the original arbitrary field semantics.

That is a multi-subsystem OCCT program, not a small subclass. Carrying it as a
private OCCT fork imposes a high maintenance cost. The user has explicitly
authorized a conformance-first native prototype; this does not imply upstream
design approval or readiness of the full kernel integration.

## Recommended representation bridges

| Direction | Near-term bridge | Semantics |
|---|---|---|
| Implicit to viewport | Adaptive boundary mesh | Display cache only |
| Implicit to OCCT visualization | `Poly_Triangulation` | No surface geometry or B-Rep guarantees |
| Implicit to manufacturing B-Rep | Validated local B-spline atlas, sewing, healing | Tolerance-controlled approximation |
| OCCT solid to implicit | Signed-distance/classification field over bounded domain | Numerical field; retain source link |
| Mixed Boolean | Evaluate the OCCT operand as a field, compose fields | Exact field composition relative to numerical evaluator; boundary extraction is approximate |
| Implicit to STEP | Export validated B-spline B-Rep | Loses authoritative field unless accompanied by native project data |

## Modification policy

The validated geometry algorithms remain reusable outside both upstream trees,
and FreeCAD changes currently live in the in-tree Implicit App/Gui module.
OCCT's TKTopAlgo now contains `BRepClass3d_SignedDistance`, a core evaluator used
for mixed field composition. This does not add native implicit B-Rep faces.
Changes to shared FreeCAD `App`, `Gui`, or `Part`
code should follow only when the module exposes a proven cross-representation
contract. OCCT changes remain a separate, evidence-driven project gated by
successful mixed-intersection and topology prototypes.

