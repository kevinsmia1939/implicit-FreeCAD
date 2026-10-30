# Native cylinder document geometry

`Implicit::NativeCylinderFeature` owns an
`Implicit::PropertyNativeCylinderGeometry`, backed by actual native topology.
It derives from `App::GeoFeature`, not `Part::Feature`; there is no arbitrary
implicit field in a Part Shape. An [opt-in surface preview](native-cylinder-preview.md)
now renders this geometry with native picks; live source editing remains open.

Use the fork's OCCT 8 runtime. Given a qualified restored-solid result from
[cylinder persistence](native-cylinder-persistence.md):

```python
import FreeCAD as App
import Implicit
import ImplicitOCCT as native

doc = App.newDocument("Cylinder")
obj = doc.addObject("Implicit::NativeCylinderFeature", "Cylinder")
obj.Geometry = solid["qualified_stored_solid_handle"]
assert obj.GeometryState == "Ready"
doc.saveAs("/tmp/Cylinder.FCStd")
```

The property accepts a qualified restored-solid owner, a bounded explicit graph
record (which becomes unverified), or None. Invalid assignments are staged and
reject before altering the current graph. In-memory Copy/Paste and undo/redo
share immutable owners. Face/edge/vertex counts and native subelement segments
are available to core geometry consumers only after qualification; persistent
edit-surviving element history remains open. Identity-placed native subobjects
and preview viewport picking are now supported separately.

FCStd stores the actual graph in a `.nativecylinder` payload, without a mesh or
cached validity flags. Save/reopen always yields `GeometryState == "Unverified"`.
The getter then returns a typed stored owner, so explicit requalification uses:

```python
geometry = native.qualify_stored_cylinder_geometry(obj.Geometry)
solid = native.qualify_stored_cylinder_solid(geometry["stored_geometry_handle"])
assert solid["solid_validated"], solid["solid_reason"]
obj.Geometry = solid["qualified_stored_solid_handle"]
```

Restore does not rerun field covers or volume integration. Ordinary recompute
does not promote an unverified graph. The derived state label is transient;
saved Ready text cannot override restored qualification. Compressed property
transfer and inline Base64 XML follow the same policy. Inline payloads have an
explicit bounded byte length, and unknown property schemas reject.

GUI tests cover native tree ownership and the same save/reopen/requalify/undo/redo
lifecycle, plus opt-in preview rendering and native face/edge/vertex hits.
Generic surface extraction still refuses because global preview surface-distance
accuracy is not certified. Nonidentity geometry
transforms also refuse pending a representation-aware placement contract.

Next are global display certification, background cancellable requalification
with atomic publication, broader semantic consumers and live field/operand parameter
editing. Restored-owner material queries, broader mixed Booleans and all other
unfinished Stage 2–4 acceptance gates remain open.

## Native boundary sampling

```python
snapshot = Implicit.nativeGeometry(obj)
points, segments = snapshot.getLines(.01)
vertices, normals = snapshot.getPoints(.01)
edge_points, edge_lines = snapshot.getLinesFromSubElement("Edge", 1)
```

These are owned immutable geometry snapshots, not live document references.
They expose native Solid/Shell/Face/Edge/Vertex counts. getPoints returns only
actual CAD vertices, with no invented mesh vertices or surface normals.
getLines shares native endpoint indices across edges, retaining degree-one
spline poles and native line endpoints. Analytic circles are sampled with a
whole-arc chord bound `R * angle**2 / (8 * N**2)`, plus actual endpoint mismatch
and floating reserves. The per-subelement line API uses .01 model units.
Buffers are transient and never serialized; the authoritative graph is unchanged.

The requested line accuracy is relative to the stored native curves. Their
existing field-intersection approximation is separate, as is any future GPU
upload displacement. It is not a total mathematical display-error certificate.
Precision and a one-million-node limit can refuse sampling. Unverified snapshots
have zero available-element counts and refuse line/point extraction. Generic
surface extraction continues to refuse. This provides shared boundaries for
the separate surface-preview stage. The line API alone does not render a surface
or define viewport selection behavior; see the preview contract for those.
