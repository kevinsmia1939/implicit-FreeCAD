# Native corner policy and shared-topology persistence

`BRepClass3d_ImplicitBoxIO` schema 2 extends the existing mesh-free mixed-boundary
record for explicit `CornerPolicy::Regularized`. Schema 1 remains readable, and
ordinary `Reject` boundaries continue emitting byte-compatible schema-1 format.
No change to generic BRepTools/OCAF/STEP implicit-face support is implied.
Schema-2 files require the updated fork; older schema-1-only readers reject
them instead of silently discarding material/contact semantics.

The schema-2 header extends the existing primitive/topology byte counts, face/
seam/root counts and geometric-bound provenance with policy code 1, exact-corner
count, contour-end count, planar-contact count and excluded-contact count.
After the source-index line, each excluded point has one XYZ line before the
existing primitive and shared conventional cap/wire payloads. Classic locale,
max_digits10, finite values, 16 MiB total, <=8 contacts and bounded counts apply.
Unknown policies/schemas, malformed/truncated/trailing input and inconsistent
metadata reject. Write preflight and read publication remain atomic.

## What is checked

Schema 2 recomputes the local mathematical corner germs from the unchanged stored
field/domain. It checks exact lattice identities, the regularized inclusion of
each box corner, excluded-contact identities in canonical order and projected
planar-contact counts. The actual shared graph must contain exactly one vertex
at every retained corner and none at excluded corners. Each split corner has
two native seam ends; uniform-sign corners have none. A three-side native vertex
must coincide exactly with a checked split corner, never merely be nearby.
Ordinary edge roots remain counted separately. Cap planes require exact axis
directions/intercepts for this policy. Existing supported-curve, cap, closed-wire,
shared identity/p-curve and opposite-use graph checks remain in force.

These are local identity/structural checks, not a whole-domain homotopy, solid
validation or fresh tracing certificate. Loading preserves original math and
actual native topology without numerical rebuilding. `SeamBoundVerified` is
false after load. No mesh, proof flags, tracing-resolution diagnostics or new
validity claims are saved. Contact counts are checked mathematical metadata,
not reconstructed trace-cell provenance.

## FreeCAD document path

The existing `PropertyNativeGeometry` / `NativeFeaturePython` owner accepts this
schema through its representation-aware codec. FCStd, XML/content transfer and
immutable undo/redo snapshots retain policy, actual edges/vertices and contacts.
Snapshot naming includes the entire serialized record digest, including policy.
Source objects are not assigned arbitrary `Part::Feature::Shape` values.
Owned snapshots and subobjects retain native topology after document close.

This is a low-level persisted boundary owner, not default-workbench exposure or
a qualified native solid. The workbench's default Add Gyroid remains unchanged.
Native mesh preparation, whole-face
attachment and solid-codec source promotion have explicit
guards until their algorithms support the policy. I/O acceptance cannot enable
those consumers implicitly.
The newer [corner-aware classifier](native-corner-classification.md) now supplies
physical region queries without upgrading the boundary to a solid.
The [stored graph/cap checker](native-corner-attachment.md) now supports this
policy, without relaxing those remaining guards.

## Verification and next gates

Kernel regression covers both operations, scaled/translated and restored-identity
sources, metadata corruption, unchanged output/destination on failure and guarded
consumers. `corner_boundary_smoke.py` now round-trips the new graph/property.
`corner_persistence_smoke.py` exercises real FCStd save/reopen with numerical
construction forbidden, edge/vertex subobjects, ownership, undo/redo, content
transfer, malformed assignment atomicity and unverified restore.
Mapped references survive unchanged FCStd topology; undo restores old identities,
while changed material invalidates them conservatively. This is not tracking
surviving elements through Boolean/parameter edits or completed topology history.

The [corner-aware surface reference](native-corner-topology.md) now resolves.
The [closed corner material reference](native-corner-region.md) now adds caps and
volume grouping. Next implement actual native attachment checks.
Only then qualify default native
solids and expose the complete workflow. All curved-operand, snapping/history,
editing and consumer requirements of Stages 2–4 remain active. Existing build
directories/dependencies are reused; no new packages or system-library changes.
