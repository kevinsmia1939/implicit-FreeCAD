# Stored cap regions attached to the closed reference

`BRepClass3d_ImplicitBox::CheckCapAttachment` and OCCT-8 FreeCAD's
`ImplicitOCCT.check_mixed_cap_attachment(boundary_handle)` now assign stored
conventional cap regions to the [closed region reference](native-region-topology.md).
This is a numerical cap-region attachment policy, not whole native trim-wire
correspondence or native shell/solid validation.
The [corner-aware path](native-corner-attachment.md) now supports regularized
boundaries with exact nodal corner values and the reference's actual odd grid.

The query first requires independent [stored graph correspondence](native-trim-correspondence.md)
and a resolved whole-box homotopy/closed region reference. Source topology is
validated/private-copied through the explicit codec; source caches, geometry,
stored seam-bound provenance and FreeCAD properties are unchanged.

## Matching policy

Cap reference triangles are grouped by shared-edge connectivity **on each
physical box side**. Every such component must belong to one reference shell
and retained volume region. Stored cap count and reference cap-region count
must agree, but counts alone never assign a match.

For each reference cap component, search triangle centroids for a common retained
point. It must:

- have a strict retained sign for both F and the nodal-linear L, beyond field
  numerical reserves; its position therefore stays retained throughout
  (1-t)F+tL, not just at two separately chosen sample points;
- classify strictly IN one stored conventional planar cap on that same side;
- have a lower distance to **all** stored cap boundary polylines exceeding the
  fresh reference seam bound plus continuous source/reference curve deviation,
  cap p-curve deviation and tight graph matching tolerance.

The seam guard uses freshly traced bounds, never the source's possibly forged
or restored verification flag. Stored graph matching includes cap grouping,
oriented wires and continuous geometry/p-curve agreement with that fresh trace.
Common retained points beyond this boundary uncertainty strip identify the
retained cap components under that planar-boundary policy and the checked
homotopy. Every cap must match bijectively; duplicate or ambiguous assignments
reject. No nearest-cap/nearest-edge pairing, coordinate welding or replacement
geometry is used. Source traversal/provenance indices do not determine identity.

A graph-distance-to-boundary ordering searches deep reference triangles first.
This is only a search heuristic; actual acceptance uses geometric clearance,
conventional classification and F/L sign checks. Thin regions without a suitable
common point can conservatively remain unresolved.

## API and scope

```python
report = ImplicitOCCT.check_mixed_cap_attachment(handle, 32, 16, 4000000, 2000000)
assert report["cap_regions_attached"], report["reason"]
assert not report["trim_wire_attachment_validated"]
assert not report["solid_validated"]
```

`reference_caps`, `reference_shells`, `reference_regions`, `witness_points` and
`boundary_clearance_bounds` follow **stored cap order**, excluding the implicit
face. Reference labels are transient query identities, not persistent names or
Boolean history. `required_clearance` is in model-length units, not field units.
Reference cap/shell/region counts and work counters are diagnostic; partial
arrays on failure must not be used as a complete mapping. An empty cap set can
attach vacuously without validating the implicit face or native solid.

Resolution/depth/homotopy-work defaults remain 32/16/four million with the existing
limits. The final argument independently bounds candidate-centroid tests and
polyline-segment clearance tests, default two million each, allowed 1 through
ten million. Prerequisite graph checks retain their existing fixed defaults
(trace limit 512 and two million comparisons). No new dependencies or system
libraries are needed. Calls are synchronous/explicit; the GUI does not run them
automatically. Invalid settings/representations reject; unresolved prerequisites,
missing witnesses, ambiguity and exhausted budgets return unresolved.

These are engineering floating reserves, not directed intervals or a formal
isotopy certificate. `trim_wire_attachment_validated`, `topology_validated`,
`native_shell_constructed` and `solid_validated` remain False. Mapping retained
cap regions does **not** distinguish every hole/wire on one cap or assign the
entire native implicit face to reference components. The user's 0.1 mm target
and original authoritative fields are unchanged.

## Evidence and next stage

Two new kernel cases cover both operations/open and closed contours, explicit
round-trip provenance and source immutability, actual IN/F-sign witnesses,
multiple cap/material regions, reordered caps and budget/input failures.
The changed-math/forged-bound regression also requires cap attachment to stop at
the failed graph prerequisite. Native CLI covers both contour/operation fixtures,
independent Part cap/edge distances, source-record equality, restore provenance,
retained lifetime and handle/settings guards.

The newer [dual-cap trim attachment](native-trim-attachment.md) now identifies
contour arcs and matches complete directed trim-wire cycles.
Next complete implicit-face component
grouping against this same reference, then construct and validate native shells
and solids. Cap-region bijection alone must not be used to promote a native
compound. Curved operands and all unfinished [Stage 4 gates](stage-4-acceptance.md)
remain required; the full goal is active.
