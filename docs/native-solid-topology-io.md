# Explicit native solid topology persistence

`BRepClass3d_ImplicitSolidIO` adds schema-1 records for the actual supported
`TopoDS_Solid` results. This is the prerequisite for a dedicated FreeCAD solid
geometry property, **not yet that property or FCStd integration**. It does not
serialize a display mesh, Part shape, arbitrary field or a Boolean recipe to run
again on load. Existing boundary-property and generic Part APIs reject these
solid handles rather than silently discarding their result topology.

## What is stored

The restricted factory deliberately shares its conventional caps and wires with
its owned authoritative source boundary. The record reuses that mesh-free
[source topology payload](native-topology-io.md), then explicitly records:

- each result field component's exact box domain and its source-wire indices;
- actual face order/source/component/shell relationships;
- shell order/material-region membership, with material regions defining solids;
- requested tolerance and stored shell Euler metadata as unverified provenance.

Field equations/orientation/tolerance come from the exact source primitive, not
samples or fabricated UV maps. Caps and wires retain their shared native edge/
vertex identity and p-curves. Faces are ordered as source caps then native field
components; each solid contains its explicitly assigned shells, so two islands
remain two solids and a cavity shell remains with its outer material region.
Empty/full and enclosed-component cases are supported too.

Load constructs these saved native faces/shells/solids directly, without tracing,
field extraction, separator searches or numerical Boolean rebuilding. This is
topology reconstruction from a record, not regeneration of geometry from input
parameters. Intermediate data buffers, meshes, pointers and proof flags are not
persisted. Conventional BRep vertex text retains the documented 15-digit
round-trip policy; source field definitions/component domains use their explicit
numeric codecs. No new bitwise-exact vertex-persistence claim follows.

## Structural checks versus numerical validity

The codec verifies supported primitive types, identity placements, cap/source
identity, finite component domains within the source, canonical metadata counts,
unique and complete source-wire assignment, actual parent/child relationships,
opposite paired edge uses between distinct faces and connected shell face graphs.
It checks stored tolerances against the requested tolerance. These are structural
checks, **not** whole-field coverage, topology homotopy, separator validity or
geometric-error certification. Stored Euler metadata is never accepted as proof.

Loading reports `native_solid_constructed=True` because actual native solid
objects exist, but `solid_validated`, `topology_validated`,
`face_attachment_validated` and `geometry_accuracy_verified` are false. Restored
solid/shell Closed flags are false, and source seam bounds remain unverified.
The status is `restored_native_solids_unverified`. Merely having a `solid_handle`
must not be used as a validity test. No generic UV BRepCheck/BOP or ordinary Part
compatibility is implied.

## FreeCAD OCCT-8 API

```python
import ImplicitOCCT as native
result = native.build_mixed_solids(obj.Geometry, .1)
record = native.dumps_mixed_solids(result["solid_handle"])
loaded = native.loads_mixed_solids(record)
assert loaded["native_solid_constructed"] and not loaded["solid_validated"]
```

`inspect_mixed_solids`, field-face queries and source-region classification work
on independently owned loaded snapshots after document closure. Classification
uses the source mathematical region union; it does not certify stored solids or
perform per-solid membership queries.

An explicit source getter supports deliberate new checks/builds:

```python
source = native.source_mixed_solids(loaded["solid_handle"])
fresh = native.build_mixed_solids(source["boundary_handle"], .1)
```

This constructs/checks a **new** result and does not upgrade the old loaded
snapshot's flags. Its numerical settings may produce different component cuts;
it is not yet a non-regenerating verifier of arbitrary saved solid topology.
The source getter is explicitly the original boundary, not the result solids.
Source and solid capsules/record types cannot be interchanged implicitly.

## Failure and resource policy

Reads are complete-record/atomic: unknown versions, truncation, trailing data,
invalid assignments or a locked destination reject without replacing its prior
geometry. Writes preflight into a private record before touching output; invalid
geometry leaves output unchanged. Ordinary stream-write failure is still an I/O
error, not a transactional filesystem operation.

Limits are 32 MiB per solid record, 16 MiB embedded source, 128 result faces/
shells and 500,000 assigned source wires. Wire identity lookup uses a topology
hash map, not quadratic coordinate matching. These are record/count bounds,
not strict allocator/time guarantees or a hardened arbitrary-BRep parser; the
conventional source parser retains its documented limitations.

Tests cover both operations/open and closed contours, shared incidence and source
immutability, separate islands/cavity grouping, canonical round-trips, retained
source triangulation caches versus mesh-free loads, false restored flags,
corrupt metadata/records, locale/stream precision, locked destinations, ownership,
classification and safe Part/boundary-property rejection. See PROGRESS.md for
executed counts and commands.

Next is a dedicated representation-aware solid geometry property/owner with
FCStd transactions/restoration and actual solid subelement dispatch. Remaining
Stage 2–4 curved operands, general component restrictions, snapping/history and
cross-workbench contracts stay in scope; this codec does not complete them.

Verified checkpoint: OCCT `0e6377579d`, FreeCAD `61f412c1fe`; 76 kernel cases,
42/42 OCCT-8 integration tests and 25/25 compatibility tests passed. Reproduce
with the existing native-face harness and `tools/with_xvfb.py ctest` commands in
HANDOFF.md; no additional packages are required.
