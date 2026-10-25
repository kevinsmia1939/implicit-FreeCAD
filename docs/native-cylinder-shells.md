# Native cylinder material shells

`BRepClass3d_ImplicitCylinder::BuildNativeShell` constructs an actual closed
`TopoDS_Shell` for the qualified full-disk axial-sheet path. It reuses the
retained planar cap, cylindrical side and native implicit face with shared edge
and vertex handles. The field remains authoritative. Topological closure does
not qualify shell geometry, a solid, volume, export or general OCCT consumers.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
shell = native.build_cylinder_shell(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3),
)
assert shell["shell_topology_validated"], shell["shell_reason"]
assert shell["shell_closed"] and shell["shell_euler"] == 2
assert not shell["shell_geometry_validated"]
assert not shell["solid_validated"]
fresh = native.inspect_cylinder_shell(shell["shell_handle"])
```

Use `build-freecad-occt8/bin/FreeCADCmd`. The immutable owner retains the shell
and its complete source/reference graph. Native implicit-face handles and
detached conventional side/cap previews remain available in the report. Editing
previews or report flags does not alter the owner. Inspection does not rebuild.
The shell is not passed through ordinary BRep serialization or exposed as an
arbitrary Part.Shape. No GUI command or document property is added by this API.

The correspondence gate proves one root per disk fibre with strict endpoint
signs. Material sign and axial direction select exactly one full cap; the other
cap is empty. The builder requires one retained cylinder face and one qualified
implicit face. Each of these generated faces has one disk-cell wire in the
native decomposition, including the cylinder's explicitly doubled seam use.

The topology checker verifies every edge has one forward and one reverse use.
Two uses on the same face are accepted only for a conventional surface edge
recognized as a genuine paired-p-curve seam. All other shared edges connect
different faces. It verifies the traversal joins of the private builders'
ordered wires, rather than assuming that arbitrary child lists are ordered.
Vertex links use distinct endpoint germs, including both germs of closed edges,
and must be single cycles. Face adjacency must be connected. The resulting
decomposition must have Euler characteristic 2 before the shell is marked
closed. This handles seam vertices that occur twice in one face wire; the older
box-shell validator's one-corner-per-vertex contract is not silently relaxed.

For the present qualified case there are three vertices, four edges and three
faces. The output's topology counters describe the retained native graph.
Failure publishes no shell or output face list, though the independently
qualified source faces may remain. Cancellation propagates. The function does
not accept externally supplied/mutated shells or use cached flags as proof.

Tests cover both material signs, both axial directions, actual implicit-face
membership in the shell, expected native counts, translation, partial-sheet
refusal and late cancellation. FreeCAD tests cover immutable ownership, detached
preview mutation, inspection, explicit geometry/solid false flags and wrong
owner rejection. Partial/multi-sheet correspondence, solid geometry/volume,
combined graph persistence, classification dispatch and live GUI consumers remain
required by the full roadmap.

## Trim tolerance audit

```python
audit = native.check_cylinder_shell_tolerance(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3), distance_tolerance=.05,
)
assert audit["trim_tolerance_verified"], audit["trim_tolerance_reason"]
assert audit["field_wire_bound"] <= .05
assert audit["maximum_face_tolerance"] <= .05
assert not audit["shell_geometry_validated"] and not audit["solid_validated"]
fresh = native.inspect_cylinder_shell_tolerance(audit["tolerance_handle"])
```

The C++ `CheckShellTolerance` result retains the qualified shell and reports
contour, cylinder/chord, combined field-wire and maximum declared face/edge/vertex
tolerances. It requires the sum of contour and chord allowances to fit the trim
request. Each declared tolerance must also fit, the native field-face tolerance
must cover the combined field-wire allowance, and the cylindrical field-edge
tolerance must cover the chord allowance. Invalid source qualification and late
cancellation retain the existing refusal/propagation policy.

`BuildImplicitFaces` now gives the generated field face an explicit trim tolerance
after its combined bound fits. This updates only the new restricted face TShape;
the primitive field's original numerical tolerance and source record remain
unchanged. For example, a 0.05 query can produce a 0.05 trimmed face from a source
with tolerance 1e-6 or 0.5 without changing that source. The mathematical equation
is unchanged; the generated trim has an independently declared approximation
policy. `restrict_cylinder_local` alone retains its earlier source-tolerance
contract.

This audit proves coherence of the retained bounds and declarations under the
existing floating-reserve policy. It does not supply the missing global
bidirectional shell-boundary correspondence, solid material classification,
volume or general consumer support. The geometry and solid flags remain false.
