# Native mixed-box topology persistence

Explicit schema-1 `BRepClass3d_ImplicitBoxIO` preserves the authoritative gyroid
and the **shared cap/seam/wire topology** of supported mixed boundaries. This
advances the active [through-Stage-4 goal](stage-4-acceptance.md); it does not
complete conforming meshing, solid validation or general curved B-Rep support.

This is not a recipe rebuild, mesh conversion, fake UV surface or generic
BRepTools/STEP/OCAF support for implicit faces. The newer
[native FreeCAD geometry property](native-geometry-property.md) uses this codec
for supported mixed topology in FCStd. Unchanged inputs restore without
numerical rebuilding; field graphs and sphere prototypes remain separate paths.

## Representation and limits

One record combines:

- The existing explicit primitive record: gyroid parameters, finite box domain,
  native face tolerance and orientation (the Boolean inside convention).
- A conventional BRepTools version-3 payload containing the planar caps and an
  extra compound holding the implicit face's trim wires. Edges/vertices are
  serialized in **one shared graph**, preserving identity and planar p-curves.
  No implicit face is passed to the generic BRepTools writer.
- Cap source traversal indices, seam/root counts and stored seam-bound metadata.

On loading, a real `BRep_TImplicitFace` is reconstructed from its authoritative
primitive, then the stored wires are attached. Caps and native face share the
loaded edges, not duplicated or coordinate-welded substitutes. Preview polylines
come from stored degree-1 B-spline geometry; sections are not retraced and the
original cell-resolution diagnostics/certificates are not invented.

Only the current identity-placed primitive gyroid/axis-aligned-box result is
supported. Records are limited to **16 MiB**, 128 faces, 500,000 edges and 300,000
poles per curve. Primitive records remain <=64 KiB. Unknown schemas, truncated,
malformed, oversized or trailing records reject. Read failure leaves the
destination unchanged; locked destinations reject. Unsupported/invalid writes
preflight before touching the output stream; an actual output I/O failure can
still leave partial bytes. No new dependencies or system installation.

The codec validates supported primitive/curve types, domain-bounded finite
poles, vertex/curve endpoints, connected closed wires, planar cap validity and
orientation, two opposite uses per edge, one cap use per native seam, and shared
box-edge root counts. It does **not** re-run the numerical tracing proof,
certify arbitrary nested topology, establish component/nonempty counts or
validate a whole native solid. Size bounds are not a hardened security parser
for hostile conventional BRepTools payloads; use normal trusted CAD-file handling.

## Accuracy and transient state

`seam_geometric_bound` survives as **stored provenance**. A freshly traced result
has `seam_bound_verified=True`; a restored result always has
`seam_bound_verified=False`, even after saving/loading it again. A producer's
claimed number is not converted into a new geometric certificate. All results
still have `solid_validated=False`. No manufacturing or 0.1 mm whole-model claim.

Meshes/polygons are removed from a deep copy during writing and discarded on
reading, without clearing the source's caches. Runtime locked/checked/modified/
convex flags are normalized on copies, not restored as source validity evidence.
Primitive loading can produce nonempty identity-location chains; the codec
canonicalizes only matrices whose coefficients are **exactly identity**, not
near-identity transforms. Nonidentity placements remain unsupported.

## FreeCAD API

Use the wholly OCCT-8-linked fork:

```python
import ImplicitMixed
import ImplicitOCCT as native
obj = ImplicitMixed.demo(open_contours=True)
snapshot = obj.Proxy.native_boundary()
record = native.dumps_mixed(snapshot["boundary_handle"])
restored = native.loads_mixed(record)
query = native.classify_mixed(restored["boundary_handle"], (0, 0, 0), .1)
assert restored["seam_bound_verified"] is False
assert restored["solid_validated"] is False
```

Restored dictionary fields match the mixed-boundary API: opaque owning
`boundary_handle`, immutable `implicit_face`, independent conventional Part
`planar_caps`, `cap_sources`, seam polylines/count, box-edge-root count, operation,
inside convention and accuracy metadata. Exported Part cap copies do not share
identity with the opaque compound and must not be sewn into a purported
implicit Part solid. Retained handles survive operand edits and document close.

The primitive-only `native.dumps(face)` still rejects trimmed/child topology.
Use the explicit `dumps_mixed` API for this supported hybrid representation.
Ordinary Part assignment and generic implicit-face BRepTools/STEP persistence
remain guarded; no automatic fallback or system OCCT-7 import path is added.

## Verification

Kernel tests cover both Boolean directions and open/closed contour graphs,
classification equivalence, shared opposite seam uses and planar p-curves,
write-load-write-load preservation, malformed input/destination atomicity,
locked rejection and source-mesh preservation versus restored-mesh omission.
CLI/GUI tests cover public APIs, restored caps/queries, accuracy-flag downgrade,
bad records, retention after document close and no Python recipe rebuild.
Build/test commands remain those in [native-mixed-box.md](native-mixed-box.md).

Verified heads: OCCT `aa6ced5047`, FreeCAD `eacfd7c7e6`; 35/35 kernel cases,
22/22 migration tests (46.09 s), 25/25 compatibility tests (28.08 s).
