# Genuine field-backed native shells

`BRepClass3d_ImplicitBox::BuildNativeShells` now builds actual `TopoDS_Shell`
objects containing conventional cap faces and exact field-backed
`BRep_TImplicitFace` faces. It does **not** turn reference triangles into CAD
faces. Empty field faces are omitted; enclosed components and multiple separated
field components are handled explicitly.

```python
import ImplicitMixed, ImplicitOCCT
obj = ImplicitMixed.demo()
result = ImplicitOCCT.build_mixed_shells(obj.Geometry)
assert result["native_shell_constructed"], result["reason"]
assert result["face_attachment_validated"] and result["topology_validated"]
assert not result["solid_validated"]
handle = result["shell_handle"]
info = ImplicitOCCT.inspect_mixed_shells(handle)
query = ImplicitOCCT.classify_mixed_shells(handle, (0, 0, 2), .1)
```

These are **shells, not validated solids**. No `TopoDS_Solid`, generic Part shape,
manufacturing result or STEP conversion is produced. Geometric/solid validation
remains a separate required stage. New result flags must not be confused with
generic UV-based `BRepCheck` or BOP support.

## Whole-face attachment and exact component domains

The [dual-cap trim attachment](native-trim-attachment.md) must resolve first.
Its entire field/linear reference supplies surface connectivity, component Euler
characteristics, closed-shell attachment and retained-region grouping. This
includes components without any trim boundary, not just the ones visible through
wires. F and the original finite box remain the authoritative definition.

For one zero-surface component, the original field/domain defines the attached
native face directly. For disconnected components, reference component bounds
and assigned native wire-pole bounds only propose axis separators. A separator
is accepted only when:

1. its plane misses every reference zero triangle in that subdomain, using a
   coordinate-reserved projection gap;
2. derivative/Lipschitz/Taylor subdivision excludes F=0 on its entire rectangle;
3. F and L have the same strict sign on that connected rectangle. L is evaluated
   from the ORIGINAL common Freudenthal grid, not an unrelated regridding.

Consequently no zero of (1-t)F+tL crosses an accepted plane. The checked homotopy
component labels remain on their respective sides. Recursive zero-free splitting
gives each new native face an exact field equation and a box domain containing
one whole component. Assigned raw native wires reuse the original shared edge
identities; no new boundary edges are introduced at a zero-free separator.
All assigned wire poles must fit the component domain under coordinate reserves.

This is an explicit axis-separation policy, not a universal component selector.
Overlapping/interlaced component projections, missing zero-free-plane proofs or
exhausted budgets return unresolved. No component is guessed from a mesh centroid
or silently dropped. If the field zero set is empty, the empty field face is
omitted and the retained region can be a full conventional box or empty result.

## Native shell assembly and topology checks

Private copies of conventional caps and the new field faces are grouped by
reference closed-shell identity; shells are associated with retained material
regions. An outer shell plus cavity shell belong to one region, whereas two
disconnected islands have separate regions. Source record, flags, curves,+caches and document properties are not modified.

Validation checks actual native topology, not just reference mesh counts:

- two opposite edge uses between distinct incident faces;
- one incoming/outgoing boundary germ at every wire corner;
- connected cyclic vertex links, retaining multiplicities so a one-edge circle
  with two endpoint germs is not mistaken for an invalid link;
- connected native faces within each shell;
- Euler agreement with the whole-field reference, using V-E+sum(face Euler).
  Cap Euler comes from its planar boundary loops; field-face Euler comes from
  the checked whole-field homotopy, including genus and closed components.

Only then are native shell `Closed` flags set and the owned result published.
`topology_validated=True` refers to these representation-aware attachment and
manifold checks under engineering numerical reserves. It is not a formal
directed-interval certificate or whole-boundary geometric/solid validity.

## FreeCAD ownership and contracts

`build_mixed_shells(boundary_handle, resolution=32, maximum_depth=16,
maximum_cells=4000000, maximum_tests=2000000, maximum_separator_cells=2000000)`
is explicit, synchronous and OCCT-8-only. Successful results contain an owned
`shell_handle` and independently owned `implicit_faces` query handles. They
survive source replacement/document close; a face can outlive its shell capsule.

`face_sources` indexes the INPUT boundary's face list; `face_components` is -1
for conventional caps and a transient component label for generated field faces.
`face_shells`, `shell_regions`, `shell_euler` and `component_domains` expose
attachment diagnostics. These are not persistent element names or full Boolean
history. `inspect_mixed_shells` reports the retained snapshot, never a live rebuild.
`classify_mixed_shells` queries its owned authoritative source region with the
existing physical-distance/witness policy, not generic UV-based solid classification.
Coordinates are local, as for raw mixed boundary handles.

Failed builds return diagnostics with no `shell_handle`; partial counts/domains
must not be consumed as constructed geometry. Successful empty-region builds
can have zero shells/faces. `solid_validated`, `geometry_accuracy_verified`,
`part_compatible` and `generic_brep_check_performed` remain False. The user's
0.1 mm geometric target and existing seam policy are unchanged; this milestone
does not yet establish the missing whole-boundary geometric validation.

Limits from trim attachment remain in force. Separator exclusion has a shared
two-million-cell budget, allowed 1 through ten million; the same maximum depth
is used. At most 128 generated native faces are allowed. Numerical ambiguity and
unsupported separation return unresolved. No dependencies/system libraries change.

Native-shell serialization/FCStd integration is not added yet. Original native
boundary geometry still persists through its existing property/codec; shell
construction is a derived operation. Generic native shell serialization or Part
assignment is not an implicit conversion path. A negative constructor test exposed
an existing Part core bug: `getPyShapes` ran outside `Part.Shape`'s exception
boundary. Moving it inside the existing try/catch makes unsupported capsules and
ordinary invalid objects raise normal Python `TypeError`. Both OCCT ABI builds
exercise that guard; valid/default constructors retain their behavior.

## Evidence and next stage

Three kernel cases cover both operations/open and closed fixtures, real shell
types, field equations/orientation, absence of triangulation-backed fake faces,
source immutability, two-component zero-free splitting and separator exhaustion,
enclosed cavities, full/empty regions and conventional-shell validation where no
implicit face remains. CLI checks owned handles/faces, source classification
equivalence, restore provenance, independent lifetime, multiple regions/cavities,
component domains, negative handles and safe Part rejection.

Next establish whole-boundary geometry validation and construct/group actual
native solids, with explicit validity policies rather than relying on `Closed`
flags. General component restrictions when axis separation fails, curved operands,
native-shell/solid property persistence, consumer/selection/history integration
and every unfinished [Stage 4 gate](stage-4-acceptance.md) remain required.
