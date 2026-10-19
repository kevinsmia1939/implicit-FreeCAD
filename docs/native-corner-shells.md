# Corner-aware whole-face attachment and native shells

`BuildNativeShells` accepts regularized corner boundaries. It constructs genuine
field-backed TopoDS faces and shells, not reference triangles or a Part mesh.
The whole mathematical surface, cap attachment and directed trim cycles remain
prerequisites; successful edge incidence alone cannot qualify the input.

## Whole-face membership and component attachment

The stored field and outer box describe the full F=0 set. Fresh fixed-corner
homotopy relates its two-dimensional components to the complete reference;
isolated contacts are tracked separately. Canonical checked uniform-cone
exclusions remove only isolated zeros from native face membership, not a patch
of the two-dimensional zero set. Both signs are removed from the implicit face;
the retained uniform corner remains cap-only.

Components are partitioned only at checked planes that exclude both F and the
nodal-linear reference zero set with the same sign. Proposals bound every
reference node and native seam pole. The separator interpolant now uses the
reference's actual resolution and its fixed exact physical-corner values,
not the requested even resolution or approximate trigonometric corner residuals.
No separator is needed for the default single surface component.

Every component face retains the source exclusions, intersected with its checked
component box. A conservative segment-AABB test rejects stored linear B-spline
seams that could enter an excluded neighborhood. Thus restrictions are preserved
by native component geometry, not just omitted from a display buffer. Complete
directed wire/component assignments and face orientations are retained.

Each native shell must have opposite paired edge uses, cyclic connected vertex
links (no pinches), connected face adjacency and the reference Euler
characteristic. Material-region grouping comes from the attached closed
reference, not component counts or an unvalidated preview mesh.

## Reference settings propagation

Cap attachment now forwards caller resolution/depth/cell limits into the
corner-aware graph correspondence precheck. Previously that precheck silently
used 32 even when callers requested 64. The original CheckCorrespondence entry
point retains its default contract; an explicit overload carries these witness
settings independently of contour tracing and comparison budgets. Failure of a
coarser precheck is not evidence that the requested finer reference failed.

## API and limits

The existing `build_mixed_shells`/`inspect_mixed_shells` APIs expose actual native
field faces, shell ownership, component/material assignments, corner_policy and
reference_resolution. Face-attachment/native-shell/topology flags describe this
implicit-aware construction. Geometry accuracy, solid validity, generic UV
BRepCheck and Part compatibility remain false. The 0.1 mm target is unchanged;
this milestone does NOT certify the complete native boundary's geometric error.

[Corner-aware cap qualification](native-corner-cap-geometry.md) now gates the
later [native solid factory/persistence extension](native-corner-solids.md).
Shell-only reports still do not promote geometric or solid validity. Mesh/coverage
guards remain.

Shell results are owned runtime snapshots. The existing FCStd native property
stores the source boundary; shells can be constructed again from the restored
native boundary without retracing it. This is NOT a new native-shell property
or a direct shell persistence codec. Restored source seam bounds stay unverified.
The default workbench command still creates the existing field-display object.

## Verification and next work

Tests cover both default operations, actual shell/edge/vertex storage, preserved
face restrictions and orientation, independent source immutability, restored
provenance, work/parameter refusal and stripped-restriction rejection. The
disconnected-component fixture additionally exercises caller settings and actual
separator interpolation. FreeCAD tests cover face queries, translated queries,
material classification, runtime ownership, and source FCStd close/reopen.

Next: extend mesh/view support and default workbench exposure, with solid/live
workflow qualification. Curved operands,
snapping, surviving-element history, richer editing and cross-workbench consumers
remain required by the full Stage 2–4 goal.
