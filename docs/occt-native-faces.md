# Experimental OCCT-native implicit face foundation

The user authorized proceeding with OCCT-native faces at a **0.1 mm geometric
tolerance target**. This increment implements a field-backed `TopoDS_Face`
representation and explicit native queries in the OCCT 8.1 fork. It is **not**
full implicit B-Rep support: wires/trimming topology, meshing, global validation,
generic Boolean dispatch, persistence/exchange and FreeCAD integration remain
unsupported. See [accuracy policy](accuracy-policy.md).

## Representation and contracts

`TKG3d/Geom` now owns `Geom_ImplicitField`, an immutable, thread-safe const query
contract derived from `Standard_Transient`, **not `Geom_Surface`**. Its APIs are
`Value`, `Gradient`, and a conservative `LipschitzBound` (unknown defaults to
infinity). Initial native implementations are an exact signed-distance sphere
and dimensionless gyroid. Invalid parameters/coordinates and evaluation
overflow raise errors. Custom fields may implement this contract; no fake UV
parameterization is required or supplied.

`TKBRep/BRep` adds `BRep_TImplicitFace : BRep_TFace`. It owns the authoritative
field handle and a finite local query domain. The inherited location and
geometric tolerance retain their normal OCCT roles. Its virtual `EmptyCopy`
preserves field/domain/location/tolerance and drops triangulations. The field
is shared as immutable geometry; any attached triangles remain transient data.
The domain clips queries only: it is **not** a set of sewn boundary wires,
natural UV restrictions or cap faces, and does not establish a valid closed
solid. Topological `TopoDS_Face` storage is not a `BRepCheck` validity claim.

`BRep_Builder::MakeImplicitFace(face, field, min, max, location, tolerance)`
creates the representation. The tolerance must be finite and positive and uses
model-length units, not field units. Construction validates before replacing an
existing face. Locked shapes remain protected. `BRep_Tool::IsImplicit` and
`ImplicitGeometry(face, location)` dispatch to field geometry. `IsGeometric`
recognizes both representations, while `HasParametricSurface` specifically
identifies a `Geom_Surface` face. Callers must dispatch explicitly; geometric
does not mean compatible with UV algorithms.

## Field-aware queries

```cpp
TopoDS_Face face;
BRep_Builder().MakeImplicitFace(face, new Geom_ImplicitGyroid(10.0),
    gp_Pnt(-12,-1,-1), gp_Pnt(12,1,1), TopLoc_Location(), 0.1);
auto hits = BRep_ImplicitTool::IntersectRay(
    face, gp_Pnt(-12,0,0), gp_Vec(1,0,0), 0, 24);
```

`BRep_ImplicitTool` implements placed-coordinate `Value`, `Gradient`, `Normal`,
`Bounds`, `IsInDomain` and `IntersectRay`. It composes face instance location and
geometry location, transforms gradients by the inverse transpose, and reverses
normals for reversed faces. Field values do not change with orientation. Normal
queries normalize the gradient at the supplied point; they do not first project
an off-surface point. Critical gradients/unsupported normal orientations fail.

Ray direction is normalized in the placed/model frame, while slab clipping is
performed in the field-local domain without rescaling the distance parameter.
Sample counts are bounded to `1..65536`. Opposite-sign intervals are bisected
with at most 100 refinement steps; exact zero samples are retained. Hits report
model distance/point, field residual and geometric bracket width. Zero explicit
distance tolerance uses `BRep_Tool::Tolerance(face)` (including OCCT's standard
minimum tolerance floor); otherwise a positive finite distance tolerance is
required. The conformance fixtures store 0.1 mm and verify its bracket policy
against analytic sphere roots, plus stricter explicit queries.

These remain sampled roots: tangencies/thin features may be missed, roots within
the distance tolerance can merge, and adjacent zero samples raise an ambiguity
error. No global coverage or robust tangency/curve-intersection claim follows.

## Preventing unsupported fallback

`BRep_TFace::Surface` now rejects reads **and writes** for the native implicit
subtype with `Standard_NotImplemented`. This also protects callers using the
low-level face getter, not only `BRep_Tool`. No base data members or virtual
slots were added to `BRep_TFace`; subtype dispatch uses existing RTTI and a new
nonvirtual query. `NaturalRestriction(true)` is rejected. Builder surface
replacement cannot overwrite the field implicitly.

Conformance tests require explicit rejection from `BRep_Tool::Surface`,
`BRepAdaptor_Surface`, ASCII BRep and binary BRep serialization, rather than
silently losing the field or inventing a surface. Generic algorithms that
require these paths are not supported. **Do not assume every legacy entrypoint
has been audited or try ordinary BOP/meshing/validation on this subtype.**
Writers may have emitted partial output before an exception; discard failed
exports. A versioned field/topology persistence format is still needed.

Consumers must be rebuilt against the fork: inline Surface guards cannot be
retroactively installed into binaries compiled with older headers. The changes
are experimental source-level contracts, not a binary compatibility promise.

## Isolated build and tests

The existing FreeCAD build and compatibility tests link installed OCCT 7.9.3.
The native-face tests use **only rebuilt OCCT 8.1 headers/libraries** in the
existing `build-occt-implicit` tree. Never stage the changed general OCCT 8 headers
into the OCCT 7.9 compatibility build or load these faces into its FreeCAD process.
No packages or system libraries are installed/replaced.

```sh
cmake -S OCCT -B build-occt-implicit \
  -DCMAKE_JOB_POOLS=occt_link=1 -DCMAKE_JOB_POOL_LINK=occt_link
cmake --build build-occt-implicit --target TKBRep --parallel 2
cmake -S tests/occt_native_faces -B build-occt-implicit/native-face-tests \
  -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DIMPLICIT_OCCT_NATIVE_BUILD="$PWD/build-occt-implicit"
cmake --build build-occt-implicit/native-face-tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
```

The separate test project imports the OCCT build's exported targets and includes
only its collected headers. It does not discover/link installed OCCT. To include
the isolated executable as a subprocess in the combined suite:

```sh
cmake -S . -B build \
  -DIMPLICIT_OCCT_NATIVE_FACE_TESTS="$PWD/build-occt-implicit/native-face-tests/implicit_occt_native_face_tests"
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

The GTest source is also registered in the OCCT TKBRep GTests manifest. The
isolated harness deliberately builds just these tests, not the full upstream
test suite or every modeling/exchange toolkit.

Captured verification: all **11 native-face GTest cases passed**; the combined
suite including existing FreeCAD/OCCT 7.9 regressions passed **21/21 CTest entries**
in 24.02 seconds. Runtime library dependencies were checked to use exclusively
the rebuilt OCCT 8.1 libraries for the isolated native-face executable.

## Next gates

1. Versioned native field/face persistence and explicit geometry dispatch audits.
2. Field-aware meshing and bounding/selection consumers, with geometry-error and
   coverage criteria distinct from field residuals.
3. Trim regions, boundary curves/chart contracts, singularities and intersections.
4. Classification, splitting and history integration in native Boolean algorithms.
5. A FreeCAD build linked wholly to the OCCT fork before exposing the new native
   face through Part; no mixing with the current 7.9 executable.
6. Whole-model 0.1 mm verification and solid/manufacturing validation; STEP needs
   a separate supported approximation bridge, not implicit-field data loss.
