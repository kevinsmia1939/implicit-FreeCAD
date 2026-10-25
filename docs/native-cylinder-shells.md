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
