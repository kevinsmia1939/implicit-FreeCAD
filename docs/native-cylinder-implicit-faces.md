# Cylinder-restricted native implicit faces

`BRepClass3d_ImplicitCylinder::BuildImplicitFaces` now attaches actual native
implicit faces for qualified full-disk axial sheets. The face remains a
`BRep_TImplicitCylinderPatch`, carrying the authoritative field and finite
cylinder predicate. Its boundary uses the original shared field edges. No UV
surface is invented for the field, and no triangles or planar proxies become
implicit geometry. This is a sufficient restricted attachment path, not general
curved Booleans, shell/solid qualification or seamless FreeCAD integration.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
result = native.build_cylinder_implicit_faces(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3),
)
assert result["implicit_faces_attached"], result["attachment_reason"]
assert result["mathematical_correspondence_verified"]
assert not result["solid_validated"]
face = result["implicit_faces"][0]  # Immutable native handle, not Part.Shape.
assert native.info(face)["cylinder_restricted"]
fresh = native.inspect_cylinder_implicit_faces(result["faces_handle"])
```

Use the fork's `build-freecad-occt8/bin/FreeCADCmd`. The owner retains the full
reference, side/cap graph, source field and settings. `implicit_faces` are owned
immutable native-face handles. `sides` and `caps` are detached conventional face
previews; editing them cannot change the graph. Inspection reuses the owned
result rather than rebuilding it. Reports distinguish proxy topology,
mathematical correspondence, native attachment and solid validation. No new
document feature or GUI command is introduced by this API. The defaults for
axial and end-disk covers are 24 binary split levels and 200000 cells per stage.

## Qualification path

The axial sheet check first proves a strict common derivative sign over a
covering of the cylinder. `BuildProjectedRegions` then maps each retained native
boundary wire to a planar reference. Cap seams project affinely; side seams
project to analytic circle arcs after their angular correspondence is checked
strictly monotone. Original vertex identity, not coordinate proximity, determines
proxy endpoint sharing. Directed area/orientation, separate-cycle separation,
bounded nesting and ordinary planar face validation gate the proxy regions.
`WireRegions` maps source wires to reference-region indices. These proxies are
diagnostics/reference topology and never replace the field geometry.

Proxy topology alone leaves `MathematicalCorrespondenceVerified` false. The
current correspondence gate accepts only one complete circular side boundary,
one reference disk and no cap field contours. It independently covers both end
disks with Lipschitz sign cones, requiring opposite strict endpoint signs in
the proven axial direction. The intermediate-value theorem and strict axial
monotonicity give exactly one root on every disk fibre. The zero set is therefore
one connected full-disk graph with the retained circular boundary. Partial
sheets, unproved endpoint covers, folded fields and multiple components refuse
this sufficient path; they remain required by the overall roadmap.

Only after correspondence succeeds does the builder create a native
cylinder-restricted field face and attach its shared trim wire. World settings
are converted to the source field's coordinates, preserving placements and the
original primitive snapshot. Material sign determines face orientation. Effective
edge identities and orientations are checked after attachment, including parent
placement compensation. Failures publish no implicit faces; late cancellation
propagates. The cap/side reference can remain independently qualified.

Derivative/sign cones use analytic bounds with floating-point reserves, not
interval arithmetic. Existing contour and cylinder/chord tolerance policies stay
separate from correspondence. Ordinary BRepCheck validates the conventional
reference faces; it is not promoted into a certificate for custom implicit faces.
Native attachment is checked by the guarded field-domain/component construction
and shared trim incidence. No shell, solid, general export or graph persistence
certificate follows from `FacesAttached`.

Tests cover both material signs, reversed axes, translated sources, immutable
source records, shared effective trim incidence, domain restrictions, partial
sheet refusal and late cancellation. Projected-region tests keep the mathematical
flag false until the independent correspondence gate is applied. The next gates
are partial/multi-sheet component correspondence, shell/material-region assembly,
solid topology/geometry, combined graph persistence and live FreeCAD GUI consumers.
