# Native solid geometry property and document owner

The OCCT-8 fork now provides `Implicit::PropertyNativeSolidGeometry` and
`Implicit::NativeSolidFeaturePython`. This is actual result topology in FreeCAD's
ComplexGeoData/property infrastructure, not the original source boundary under
a solid name, a Part shape or a rebuild recipe.
The [corner-aware solid extension](native-corner-solids.md) uses the same property
and owner with schema-2 native topology payloads. The property's XML carrier
schema remains 1; inner codec versions and verification flags are separate.

```python
import FreeCAD as App
import Implicit
import ImplicitOCCT as native

# boundary is an existing supported mixed gyroid/box boundary feature.
result = native.build_mixed_solids(boundary.Geometry, .1)
assert result["solid_validated"], result["reason"]
solid = boundary.Document.addObject("Implicit::NativeSolidFeaturePython", "NativeSolid")
solid.Geometry = result["solid_handle"]
geometry = Implicit.nativeGeometry(solid)
print(geometry.getElementTypes())
selected = solid.getSubObject("Solid1")
```

The subsequent [live solid workflow](live-native-solids.md) now supplies a Python
modelling proxy and solid-specific component-aware viewport renderer on top of
this low-level owner. Its richer task editor remains unfinished. The ordinary
Add Gyroid command is unchanged. Do not reuse the source's single field face as
though it were the actual solid result.

## Representation and subelements

The solid property is a separate registered type from the existing boundary
property. Both owners share native subobject/placement/error-invalidation code,
but neither owner has a dummy property for the other representation. Geometry
accepts a matching owned solid capsule, an explicit solid record, or None.
Boundary capsules/records, Part shapes and generic ComplexGeoData wrappers reject
without replacing the previous value. Cross-representation content transfer and
property Paste also reject. There is no `Part::Feature::Shape` property.

`SolidN`, `ShellN` and `FaceN` refer to actual result objects. `NativeFaceN` and
`CapN` filter that result's faces. `SeamN` enumerates shared edges on the actual
component field faces; `EdgeN` follows the actual solid compound, and `VertexN`
uses canonical forward-edge endpoints by native identity. No coordinate welding
or triangle-derived vertices are used. Empty results have zero elements; a
closed field component can have no wires yet still have valid domain bounds.
Field-face bounds use their saved component domains, not the whole source box.

Mapped names use `ImplicitSolidV1_` plus a SHA-256 digest of the actual mesh-free
solid record. These are conservative snapshot identities, not history tracking.
Owned whole/selected snapshots retain the entire solid after document closure.
Rigid object/container/App::Link extraction applies placement once. Unsupported
nonrigid or generic Part operations remain explicitly unsupported.

`Implicit.classifyNative` on a whole solid snapshot classifies the authoritative
source mathematical region union and reports that scope explicitly. It reports
the snapshot's current `solid_validated` flag. Classification of a selected
`SolidN` is rejected: it must not silently classify the union instead of that
particular material region.

## Persistence, transactions and failure

FCStd uses a distinct `<NativeSolidGeometry schema="1">` property element and
`.nativesolid` payload containing the [actual solid codec](native-solid-topology-io.md).
Inline Base64 content and property-content transfer use the same record.
Restore reconstructs saved topology without numerical Boolean regeneration;
loaded numerical validity remains false. Runtime undo/redo can retain the
original immutable qualified snapshot, whereas serialization does not preserve
its proof flags. An explicit new build is not certification of the old record.

Assignment validates/maps a candidate before property mutation. Invalid inline
content cannot commit before the complete character stream closes. Corrupt
FCStd payloads leave fresh restored properties empty rather than showing stale
solids. An upstream error clears the solid property even if dependency failure
skips Python execution. The empty property retains its solid representation type.

Tests cover both operations, separate components, enclosed/cavity and full/empty
results, transactions, mismatched representations, content/cross-document copy,
restore without rebuilding, corruption, retained lifetime, bounds and rigid
object/container/link placement. They do not qualify solid GUI picking, live
source recompute, snapping, Boolean history or arbitrary conventional operands.
The subsequent live-workflow tests qualify their own documented scope; snapping,
Boolean history and all other unfinished Stage 2–4 gates remain required.

Run the focused test against the existing OCCT-8 build:

```sh
ctest --test-dir build-freecad-occt8/migration-tests \
  -R '^implicit_occt8_native_solid_property$' --output-on-failure
```

CTest also runs its actual loaded-library runtime fixture. The system FreeCAD
and the preserved OCCT-7 fork do not contain this native solid property.
