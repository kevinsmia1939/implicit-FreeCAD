# Architecture report: implicit geometry in OCCT and FreeCAD

## Decision

For the initial prototype, use an external implicit-geometry engine (architecture
B) and keep OCCT integration behind explicit conversion/adaptation boundaries.
Render a bounded numerical mesh directly in FreeCAD through Coin3D. Do not model
an arbitrary level set as `Geom_Surface`, and do not label a triangulation-only
`TopoDS_Face` as a native implicit B-Rep.

This is the prototype architecture, not the final ambition. For an nTop-class
system, the target is a native FreeCAD implicit/field module with a dual-kernel
model. OCCT core changes become necessary only if implicit faces must participate
directly as first-class `TopoDS` B-Reps. The detailed modification boundary is in
[`core-integration-plan.md`](core-integration-plan.md).

The inspected sources are FreeCAD 27.1.0-dev at `d732e2f13a` and OCCT 8.1.0-dev1
at `3d097a0328`; the installed development libraries are OCCT 7.9.3. The first
vertical slice deliberately does not link OCCT, so that source/runtime version
difference does not affect it.

## Findings from the source

### `Geom_Surface` cannot honestly represent an unparameterized level set

The base contract requires U/V reversal, transformed parameters, U/V bounds,
closure/periodicity, point evaluation, and partial derivatives. In the inspected
tree these are visible in
`OCCT/src/ModelingData/TKG3d/Geom/Geom_Surface.hxx` (notably lines 87–184 and
244–330). `Geom_BoundedSurface` narrows the category but does not introduce an
implicit alternative. Returning invented U/V values would violate identities
documented by the API and destabilize algorithms that depend on derivatives and
parameter continuity.

`Adaptor3d_Surface` is also a parametric interface: it exposes first/last U/V,
`Value(U,V)`, and U/V derivatives. It is useful for adapting a *local
parameterization*, but not as a direct F(x,y,z) abstraction.

### A genuine OCCT face carries more than renderable triangles

`BRep_Builder` can make a face from a `Geom_Surface` or from a
`Poly_Triangulation` (`BRep_Builder.hxx`, lines 62–105). The latter is valuable
for presentation/storage, but does not manufacture surface geometry, boundary
curves, edge p-curves, UV domains, orientation, or a closed shell. This is why a
mesh-only face is not equivalent to a B-Rep face usable by general modeling
algorithms.

`BRepAdaptor_Surface::Initialize` asks `BRep_Tool` for the underlying
`Geom_Surface`, returns early when it is null, and otherwise loads UV bounds from
the face (`BRepAdaptor_Surface.cxx`, lines 56–80). Similarly,
`BRepBuilderAPI_MakeFace` accepts parametric surfaces and explicit U/V bounds
(`BRepBuilderAPI_MakeFace.hxx`, lines 88–135).

### Operations that fundamentally depend on U/V

- B-Rep face construction and trimming need boundary edges represented both in
  3D and as p-curves in the face's 2D parameter space.
- `BRepMesh` discretizes faces through surface adaptors and UV range splitters;
  it is not an isosurface extractor.
- OCCT point projection accepts a `Geom_Surface` plus a U/V search box
  (`GeomAPI_ProjectPointOnSurf.hxx`, lines 45–123).
- Surface/surface intersection (`GeomInt_IntSS`/`IntPatch_Intersection`) consumes
  `Adaptor3d_Surface`, tracks parameter-space domains, and can build 2D curves on
  both surfaces.
- Boolean `Common` and `Cut` delegate to the BOP pipeline and pave filler. Their
  face-face intersection, splitting, classification, and reconstruction assume
  valid B-Rep geometry and topology. A display triangulation does not satisfy
  those assumptions.

### Operations that naturally work directly on F

- Point classification by sign, with an explicit convention and tolerance.
- F evaluation, analytical/numerical gradient, and normal evaluation away from
  critical points.
- Local closest-point projection by solving the constrained stationarity system.
- Ray roots through bracketing/refinement, supplemented later by tangency tests.
- Plane or analytic-surface intersections by reducing the problem to a lower
  dimensional implicit equation and numerically tracing connected curves.
- Bounded isosurface extraction, adaptive spatial subdivision, and conservative
  bounding-box rejection.

These operations should remain in an engine whose API is expressed in 3D, rather
than acquiring artificial U/V parameters.

## Architecture comparison

| Architecture | Fidelity | Cost / memory | OCCT Booleans | FreeCAD fit | Maintenance |
|---|---|---|---|---|---|
| A. Native OCCT implicit type | Best potential fidelity | High implementation cost; potentially low geometry memory | Requires extensive new intersection, classification, meshing, topology, persistence, and exchange support | Good only after broad upstream work | Very high; invasive OCCT fork/upstream effort |
| B. External engine + explicit bridges | Exact F evaluation; numerical operations are tolerance-controlled | Low base memory; visualization scales with sampled volume | Unsupported directly; use a deliberate patch/mesh/voxel bridge per operation | Excellent for an isolated addon and incremental tests | Low initially; clear ownership boundary |
| C. Local B-spline patches | Approximate to a declared tolerance | Patch fitting, overlap, sewing, and validation can be expensive | Best compatibility once a valid sewn B-Rep exists | Good as an export/modeling mode | Medium/high; robust atlas generation is difficult |

Architecture B is selected now. Architecture C is the preferred future bridge
for conventional OCCT Booleans and STEP where approximation is acceptable.
Architecture A should only be pursued with an upstream design agreement covering
the full topology/algorithm impact—not by adding a nominal subclass alone.

The OCCT bridge probe builds a `Poly_Triangulation`-backed `TopoDS_Face` and
asserts that `BRep_Tool::Surface()` remains null. This is an intentional
interoperability diagnostic, not a claim of native implicit geometry.

## Implemented vertical slice

The C++ library has no FreeCAD dependency and provides:

- abstract F evaluation and analytical-gradient override with centered numerical
  fallback;
- normalized surface normals;
- a local closest-point solver for `x-q + lambda*grad(F)=0, F(x)=0`, using a
  numerical Hessian and guarded Newton iteration;
- ray intersection over a finite parameter interval with sampling and safeguarded
  secant/bisection refinement;
- finite AABB domains and caller-controlled tolerances;
- analytical gyroid value and gradient;
- bounded marching-tetrahedra visualization extraction;
- sign classification and bounded plane-intersection curve tracing.

The FreeCAD feature persists the mathematical and display parameters. Its mesh
lives only in the Python proxy and retained Coin buffers. Recompute follows
parameter changes; camera interaction only traverses the existing scene graph.

## Accuracy language

The gyroid equation and analytical gradient are evaluated in double precision.
Projection, ray roots, and plane-intersection curves are numerical,
tolerance-controlled results. Extracted triangles are sampled approximations.
There is currently no native implicit `TopoDS_Shape`, closed solid, Boolean
result, or STEP representation in this prototype.

