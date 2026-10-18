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
`NativeFace1`, `CapN`, `SeamN`, `EdgeN` and `VertexN` segments. Segments own the immutable snapshot;
boundary lines and a first boundary vertex are available to native consumers.
Unknown indices and segments from another snapshot reject. NativeFace lines
are trim boundaries, not a surface tessellation. Subelement face meshing still
rejects. The optional surface preview supports NativeFace1 picking; integrated
snapping and edit-stable history remain open. See [edge/vertex selection](native-edge-vertex-selection.md).
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

The mixed view renders caps, edges and vertices as individual Coin shapes,
preserving aggregate buffers and sharing seam/edge alias nodes. Real Coin ray
hits map to caps, seams, other edges and vertices; detail
paths target the whole cap/seam node, not a disposable triangle/line segment.
FreeCAD selection registration, invalid-name rejection, restore and failed-
recompute clearing are tested. These tests do not certify visual highlight
pixels, generic consumer support or mathematically exact surface picking.

Plain names remain **snapshot-local indices**, not history mappings across
parameter edits or Boolean changes. Current GUI selections are discarded when
the snapshot is replaced/cleared. The mapped-reference contract below now
protects new typed owners against silent external LinkSub index retargeting.
This is partial native selection integration, not completion of Stage 4's
selection, snapping and history gates. NativeFace1 picking requires the optional
transient surface preview.

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
The original mapped-reference checkpoint returned None for Python subobject
extraction; the owned geometry contract below supersedes that behavior for typed
owners. It does not return a Part shape.
Generic consumers, external-document links, container placement and persistent
history across different geometry remain unqualified.

`native_reference_smoke.py` checks every mapped subelement on the multi-face
crossing fixture, unchanged FCStd restoration, recursive copy of a linked static
snapshot, real LinkSub invalidation, saving/reopening unresolved references,
undo/redo and exact recovery. The GUI regression checks current mapped paths
and rejection after replacement. These do not establish manufacturing validity.

## Owned native subobject geometry

`obj.getSubObject(reference)` now returns an independently owned
`Data::ComplexGeoData` wrapper backed by C++ `Implicit::NativeElementGeometry`.
GUI `SelectionEx.SubObjects` exposes the same representation. The selected
segment and complete authoritative native snapshot remain owned after source
replacement or document close. It is not a Part.Shape or an editable property
snapshot, and assignment to either property type rejects.

The result contains one `Element1`. Its mapped name retains the original
NativeFace/Cap/Seam/Edge/Vertex identity; `getLines(accuracy)` and
`getLinesFromSubElement("Element", 1)` return the selected topology's boundary
polylines. Accuracy is not a surface-meshing request: these are stored degree-1
curves. Bounds for caps/seams enclose their stored boundary geometry; native
face bounds conservatively enclose the entire finite domain, not just the trim
lines. Neither bounds nor lines prove nonempty volume or solid validity.

```python
element = obj.getSubObject("Seam1")
points, lines = element.getLines(.1)
original_identity = element.getElementName("Element1", 1)
```

Following FreeCAD's subobject convention, geometry is pre-placed by the
accumulated incoming matrix and (when transform=True) the object's Placement.
Its own geometry transform is identity, so consumers must not apply Placement a
second time. `transform=False` omits the object's own placement; parent-path
placement handling follows the core path traversal. Rigid orientation-preserving
matrices are supported; scales, reflections, projective/nonfinite matrices reject.
Nonfinite transformed points reject too. This is ordinary double-precision
placement, not a newly certified world-space 0.1 mm error bound at extreme scales.

Surface/edge point sampling, nonidentity geometry mutation and independent selected-
geometry persistence remain unsupported and reject. Save the owning feature and
its mapped link instead. The first-vertex C++ hook applies the same placement,
and selected vertices now support `getPoints` extraction with empty normals and
zero-size bounds. Integrated snapping remains open. Conventional consumers still need explicit
representation dispatch; they cannot feed this wrapper to UV-dependent Part BOP.

`native_element_smoke.py` compares every selected subelement against independent
rigid placements, tests raw/untransformed extraction and nested App::Part paths,
checks bounds/ownership and rejects unsupported transforms, property assignments,
sampling/mutation/persistence. The mixed GUI regression now verifies that both
selected cap and seam SubObjects contain native complex geometry. These checks
do not qualify general App::Link/container workflows, visual highlight pixels,
all workbenches or the unfinished Stage 2/3 solid/Boolean gates.

## Whole-object and rigid App::Link extraction

An empty native reference, `obj.getSubObject("")`, now returns owned C++
`NativePlacedGeometry` through ComplexGeoData when the geometry property is
nonempty. Unlike a selected-element result, it retains the NativeFace/Cap/Seam/Edge/Vertex
element types, counts and mapped names. Whole and per-element boundary lines,
conservative domain bounds and first-vertex hooks follow the same once-applied
rigid-placement convention. Null native geometry returns None, not a valid-
looking empty body. Surface sampling and nonrigid extraction still reject.

This fills the whole-object lookup needed by App::Link. CLI tests compare both
LinkTransform modes against independently composed placements, all native
subelement paths, transformed App::Part-to-link paths, a mapped single-cap link,
same-document save/reopen and retained geometry after close. Scaled link
extraction rejects; clearing the source leaves its mapped cap link unresolved.
External-document whole/cap queries and reopening the instance document are
tested with the source document already loaded, including source-map immutability
when App::Link re-tags the returned map. This does not qualify automatic external
dependency loading, missing/moved files, link arrays or all instance modes.

The GUI regression casts a real Coin ray into a translated/rotated native link,
requires a Cap1 hit and checks selected native geometry against an independent
placement. Whole-object selections follow ordinary FreeCAD behavior: SubObjects
is empty when no subelement names were selected; query the selected Object with
getSubObject("") instead. No Part.Shape is synthesized for an instance.

This is supported rigid extraction/picking on the documented paths, not general
cross-workbench support, editable instance geometry, manufacturing solids or
completed Stage 4 integration. Source fields and supported Boolean input gates
remain unchanged; placing an output instance does not enable arbitrary placed
field/curved-operand Booleans.

## Region classification in extracted geometry coordinates

`Implicit.classifyNative(whole_geometry, point, distanceTolerance=.1)` queries
the authoritative mathematical mixed region of local NativeGeometry or an owned
NativePlacedGeometry snapshot. Input/output coordinates are those of the geometry:
local for `Implicit.nativeGeometry(obj)`, pre-placed for `obj.getSubObject("")`
or a supported container/link path. No viewport mesh or Part solid is involved.
Selected Element1 wrappers reject: a cap/seam/face alone is not a volume region.

```python
import Implicit
geometry = instance.getSubObject("")
query = Implicit.classifyNative(geometry, (x, y, z), .1)
```

Inside/outside require a physical distance lower bound greater than the caller's
tolerance. Near-boundary requires a transformed boundary witness whose distance
upper bound is within tolerance and whose root bracket is at most tolerance/256.
Other cases remain unresolved. `solid_validated` is always False; this API does
not validate boundary topology, component coverage, solidness or manufacturability.
The original ImplicitOCCT.classify_mixed handle API remains local and unchanged.

Placed queries invert the pose, measure the forward residual and reserve
`128*epsilon` times a coordinate/domain magnitude budget for inverse and witness
mapping. A reserved Gershgorin bound on M^T M supplies minimum/maximum stretch
instead of assuming even a numerically near-rigid matrix is exactly orthogonal.
Lower bounds shrink; witness distance/width bounds expand. Local tolerance
reserves 1024 placement errors so transformed root brackets still meet /256.
If 2048 errors consume the requested tolerance, classification returns unresolved
with `placement_resolved=False`. Otherwise placement_resolved can be True while
the kernel/physical-margin result remains unresolved. The reported
`placement_error_bound` is a floating-point engineering reserve, not directed
interval arithmetic or a world-space accuracy certificate. Invalid inputs and
the kernel's unsupported local precision ranges still raise explicit errors.

The regression covers both operations, independent known cap-distance bounds,
transformed cap/root witnesses, root signs/domain checks, stationary unresolved
cases, snapshot/container/link paths, FCStd restore, lifetime, wrong/selected
representations and large-coordinate precision refusal. The mixed GUI checks a
placed whole-instance region query too. Only the supported native mixed boundary
representation is covered. Placing/grouping live recipe operands can still
violate the kernel's identity-placement gate; static owned snapshots are used
for the multi-level classification test. General placed-operand Booleans remain
required, not implicitly enabled by this consumer API.

Run the documented incremental Implicit/ImplicitScripts builds and the separate
OCCT-8 migration harness. Stage 2 curved operands, Stage 3 conforming meshing/
solid validation and Stage 4 selection/editors/consumer/placement coverage remain
open; passing these property tests does not prove those requirements.
