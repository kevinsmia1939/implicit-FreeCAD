# Native FreeCAD geometry property and FCStd topology

The OCCT-8 build now registers `Implicit::PropertyNativeGeometry`, derived from
`App::PropertyComplexGeoData`, and `Implicit::NativeGeometry`, derived from
`Data::ComplexGeoData`. New mixed results use `Implicit::NativeFeaturePython`,
whose C++ `getPropertyOfGeometry()` returns that property. This is native C++
geometry ownership/persistence, not an App::PropertyString, a proxy-only recipe
or an implicit object assigned to Part::Feature::Shape.

This advances the active [through-Stage-4 goal](stage-4-acceptance.md), but does
not finish it. The property currently supports the native gyroid/axis-box
boundary representation. It does not migrate every field-graph/sphere/prototype
feature to complex geometry, provide conforming meshes or validate solids.

## Ownership and consumers

The property owns an immutable native boundary snapshot, including authoritative
field math, caps, shared edges/vertices, p-curves and trim wires. Copy/Paste use
immutable sharing for transactions; replacing/clearing a property never mutates
an earlier snapshot. Python getters return independently owned immutable handles
accepted by the ABI-guarded native API; handles are not editable Part shapes.

Generic geometry discovery works for new native owners. Bounds conservatively
cover the finite box domain (not a nonempty/volume certificate). Stored boundary
lines can be retrieved through the complex-geometry interface. Requests for
whole surface faces/points fail explicitly: returning only caps or a partial
surface would be misleading. Native subelements now expose one-based
`NativeFace1`, `CapN` and `SeamN` segments. Segments own the immutable snapshot;
boundary lines and a first boundary vertex are available to native consumers.
Unknown indices and segments from another snapshot reject. NativeFace lines
are trim boundaries, not a surface tessellation. Subelement face meshing still
rejects. Snapping, stable naming/history and implicit-surface picking remain open.
Nonidentity geometry transforms reject; instance/container/world-query handling
is not yet qualified as seamless integration. New owners have the GeoFeature
placement infrastructure, not a claim of completed placement consumer support.

Only the wholly OCCT-8-linked build compiles/registers these types. The OCCT-7
configuration does not compile the source, stage OCCT-8 headers or load its
private extension. No additional dependencies/system libraries are installed.

## Persistence and live recompute

Property schema 1 stores a `.nativegeo` archive member in FCStd, using the
[explicit native topology codec](native-topology-io.md). Null geometry is explicit;
an inline Base64 path supports force-XML serialization. Property/object content
dump/restore, XML-only Content with inline restore, and static cross-document
object copy are tested. Interactive GUI clipboard qualification remains pending.
No pointer or display mesh is
stored. Native topology is restored without calling the Boolean builder.

Mixed-feature schema 2 retains live Field/Box links and parameters. A fingerprint
of supported geometric inputs, excluding BRep mutable/cache flags, determines
whether recompute can reuse the restored property or must build a new snapshot.
The stored primitive parameters, operation, bounds and tolerance are also checked
against unchanged inputs: readable but inconsistent topology cannot override the
linked field definition. `inspect_mixed` reconstructs only cap/seam preview data.

Restoration refreshes the display from stored geometry immediately; an unchanged
recompute does not retrace. Parameter changes build a fresh property. As with
other CAD outputs, edited-but-not-recomputed inputs can still show the previous
snapshot until recompute. Freshly traced seams have `SeamBoundVerified=True`;
restored bounds are stored provenance, not re-certified, so the diagnostic is
False. All snapshots remain `solid_validated=False`.

Execution failure clears geometry, proxy caches and display. A native-owner
document-recompute hook also clears skipped outputs after upstream source errors.
Corrupt/missing native data with a saved fingerprint is not silently replaced by
a recipe. Explicit repair is available through `obj.Proxy.rebuild(obj)` when the
linked inputs remain valid. Static snapshots without recipes cannot be repaired
from missing topology. Parser/source-consistency errors are surfaced, not proof
of production-grade hostile-file handling.

Old schema-1 recipe owners migrate in place: they gain the native geometry
property and rebuild once on recompute, preserving names/links. Their original
App::FeaturePython type is retained, so they do not automatically acquire the
new owner's GeoFeature geometry-discovery hook or C++ dependency invalidation.
Broader legacy-owner/consumer migration remains a completion gate.

## API and tests

```python
import ImplicitMixed
import ImplicitOCCT as native
obj = ImplicitMixed.demo()
assert obj.getPropertyNameOfGeometry() == "Geometry"
snapshot = obj.Geometry
data = native.inspect_mixed(snapshot)
query = native.classify_mixed(snapshot, (0, 0, 2), .1)
```

The existing proxy query helpers remain available. Static native owners can
store a snapshot or explicit topology record directly; assigning Part.Shape or
an invalid record raises before changing the property. None clears it.

`native_property_smoke.py` checks native type/discovery, immutable lifetime,
property undo/redo, invalid-assignment atomicity, FCStd payloads, restoration with
both gyroid construction and Boolean rebuilding disabled, parameter recompute,
upstream failure invalidation/recovery, schema-1 migration, corrupt data rejection
and mismatched stored math versus unchanged inputs. Negative cases intentionally
emit expected embedded-file/source errors. The mixed GUI regression requires
restored display buffers before recompute with rebuilding disabled.
Content-transfer tests cover external archive and inline Base64 topology, null
geometry and independent cross-document copies.

## Snapshot subelements and GUI picking

`Implicit.nativeGeometry(obj)` returns an owned `Data::ComplexGeoData` snapshot,
sharing immutable native geometry, never a pointer into a document property's
mutable storage. It supports the standard `getElementTypes`,
`countSubElements` and `getLinesFromSubElement` interfaces. For example:

```python
import Implicit
geometry = Implicit.nativeGeometry(obj)
points, lines = geometry.getLinesFromSubElement("Seam", 1)
```

Cap indices follow stored face order; seam indices follow the unique edges of
the native implicit trim-wire graph. Both the C++ interface and display preview
use that order, including after topology I/O. Tracing section order is not used
as an identity. Multi-face/open-contour tests compare every subelement's lines
before/after I/O. Geometry snapshots remain queryable after source document close.

The mixed view renders each cap and seam as its own Coin shape, preserving the
aggregate coordinate buffers. Real Coin ray hits map to `CapN`/`SeamN`; detail
paths target the whole cap/seam node, not a disposable triangle/line segment.
FreeCAD selection registration, invalid-name rejection, restore and failed-
recompute clearing are tested. These tests do not certify visual highlight
pixels, generic consumer support or mathematically exact surface picking.

Plain names remain **snapshot-local indices**, not history mappings across
parameter edits or Boolean changes. Current GUI selections are discarded when
the snapshot is replaced/cleared. The mapped-reference contract below now
protects new typed owners against silent external LinkSub index retargeting.
This is partial native selection integration, not completion of Stage 4's
selection, snapping and history gates. No implicit surface is selectable through
a mesh that does not exist yet.

## Mapped references and explicit stale attachments

Each native geometry snapshot now reconstructs a standard FreeCAD element map.
Mapped names combine `ImplicitV1_`, a SHA-256 digest of the mesh-free native
topology record, and the subelement index/type. The digest includes the stored
field, operation, cap/seam topology and approximation policy. It is a conservative
snapshot identity, not a geometric equivalence proof, accuracy certificate or
security boundary. Different encodings may deliberately invalidate references.
Map reconstruction uses existing dependencies and no separate FCStd schema.

```python
geometry = Implicit.nativeGeometry(obj)
identity = geometry.getElementName("Cap1", 1)
reference = ";" + identity + ".Cap1"   # standard mapped name + old-name suffix
assert obj.getSubObject(reference, retType=1) == obj
```

Standard PropertyLinkSub/LinkSubList machinery can record this map for new
`Implicit::NativeFeaturePython` owners. Identical stored topology preserves the
map across FCStd/content restoration, copying and undo/redo. A changed/cleared
snapshot leaves old mapped references unresolved, even when the trailing Cap1
index would still fit. It never silently falls back to that index. Native owner
subobject lookup validates the map and rejects unknown/out-of-range names;
the GUI also rejects stale mapped detail paths. A missing-reference warning from
FreeCAD's link machinery is intentional in the negative regression.

This is **invalidation, not history tracking**: even a local edit invalidates all
references when the serialized boundary changes. Exact restoration can recover
them, but surviving elements are not matched through topology changes. Explicit
raw indices still address the current snapshot and are not stable attachments.
Legacy App::FeaturePython owners do not gain typed-owner core reference validation.
Subobject Python extraction intentionally returns None, not a Part shape;
representation-aware geometry inspection uses `Implicit.nativeGeometry`.
Generic consumers, external-document links, container placement and persistent
history across different geometry remain unqualified.

`native_reference_smoke.py` checks every mapped subelement on the multi-face
crossing fixture, unchanged FCStd restoration, recursive copy of a linked static
snapshot, real LinkSub invalidation, saving/reopening unresolved references,
undo/redo and exact recovery. The GUI regression checks current mapped paths
and rejection after replacement. These do not establish manufacturing validity.

Run the documented incremental Implicit/ImplicitScripts builds and the separate
OCCT-8 migration harness. Stage 2 curved operands, Stage 3 conforming meshing/
solid validation and Stage 4 selection/editors/consumer/placement coverage remain
open; passing these property tests does not prove those requirements.
