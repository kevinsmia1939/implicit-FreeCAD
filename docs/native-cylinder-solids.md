# Qualified native cylinder material solids

`BRepClass3d_ImplicitCylinder::BuildNativeSolid` now constructs an actual
`TopoDS_Solid` for the qualified full-disk gyroid/cylinder path. Its single shell
contains the retained conventional cylinder, full cap and native implicit face,
sharing their original edge/vertex handles. No mesh is converted into faces and
no artificial field UV surface is introduced. Validity is the explicit
implicit-aware core contract below; it is not a claim that ordinary OCCT
algorithms, Part.Shape serialization or general consumers support this geometry.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
geometry = native.check_cylinder_shell_geometry(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3), distance_tolerance=.1,
)
assert geometry["shell_geometry_validated"], geometry["geometry_reason"]
solid = native.build_cylinder_solid(geometry["geometry_handle"], volume_tolerance=.01)
assert solid["solid_validated"], solid["solid_reason"]
assert solid["solid_constructed"] and solid["solid_shape_type"] == "Solid"
assert solid["volume_lower"] <= solid["volume_upper"]
assert solid["volume_upper"] - solid["volume_lower"] <= .01
point = native.classify_cylinder_material(solid["solid_handle"], (0, 0, -.5))
fresh = native.inspect_cylinder_solid(solid["solid_handle"])
```

Use the fork's `build-freecad-occt8/bin/FreeCADCmd`. The immutable solid owner
retains the complete source/reference graph and native shape. Reports expose
native implicit-face handles and detached conventional face previews. Report or
preview edits do not mutate that owner; inspection does not rebuild. No arbitrary
field is put in a Part::Feature Shape, and no new document/GUI workflow or
document persistence is supplied by this construction API. Separate
[explicit cylinder graph IO](native-cylinder-persistence.md) restores topology
with geometry/volume/solid qualification cleared. The explicit
`solid_generic_occt_supported` flag remains false.

## Construction gates

Construction accepts an unchanged qualified ShellGeometry reference. Earlier
gates establish an exact full-disk graph, selected material orientation, closed
shell topology, coherent stored tolerances and bidirectional boundary error
within the requested model-length target. The graph and strict end-disk signs
give one connected material region with positive thickness on every disk fibre.
The volume gate below additionally requires a bounded positive volume interval
strictly below the full cylinder volume. Only then does BRep_Builder create a
solid and attach the existing closed shell. Failure returns no solid; source
geometry diagnostics remain owned. Cancellation propagates, including just
before construction. Cached report dictionaries are not construction authority.

`solid_validated` records this implicit-aware topology/geometry/material contract.
It does not invoke ordinary BRepCheck for the custom implicit face and does not
enable unsupported generic Boolean, meshing, export or mass-property paths.
Point classification reuses the authoritative mathematical material recipe and
keeps its spatial tolerance, UNKNOWN and approximation-clearance contracts.

## Bounded mathematical volume

```python
volume = native.bound_cylinder_material_volume(
    solid["solid_handle"], volume_tolerance=.02,
    maximum_depth=24, maximum_cells=200000, maximum_root_tests=10000000,
)
assert volume["volume_verified"]
assert volume["volume_kind"] == "mathematical_material"
```

Volume tolerance is an absolute interval width in model length cubed (mm³ in the
normal FreeCAD convention). It is independent of the 0.1 mm geometric target.
The C++ `VolumeSettings` carries this tolerance, bounded work settings and an
optional cooperative checkpoint that is not retained in results.

The integrator partitions the cylinder's disk in local orthonormal coordinates.
Rectangles wholly inside have bounded full area; strictly disjoint rectangles
contribute zero; circle-crossing rectangles retain conservative area intervals.
For each potentially nonempty rectangle, a base point inside the disk and
gradient/Hessian Taylor cones qualify lower/upper root heights over the entire
rectangle. Only proved field signs update these heights. The existing global
end-disk correspondence supplies valid fallback endpoints. These height bounds
give selected-material thickness bounds, which combine with area bounds into
volume intervals.

The largest-uncertainty cell is split first. Explicit scalar, gradient, area and
accumulation reserves enlarge bounds; the full cylinder volume supplies an
independent upper bound with reserve. A pass requires the final interval width
to fit the request. Cell, root-work, depth and precision failures refuse and
clear output bounds. This is analytic numerical engineering with floating-point
reserves, not interval arithmetic or a mesh-based estimate. It bounds the
mathematical material volume; it is not generic BRepGProp evaluation of the
approximate stored boundary. An overly broad interval can pass its requested
width yet fail the positive/partial-volume solid gate.

Defaults are 0.01 volume units, 24 binary levels, 200000 cells and 10000000 root
tests; maximum limits are 32 levels, 2000000 cells and 50000000 root tests.
Geometry or qualified solid owners can be queried. Failed solid owners refuse
material/volume queries rather than silently using a failed shape.

Tests verify both material signs against independent analytic-root polar
quadrature at two resolutions, complementary volume enclosure, actual Solid
type/closed shell/implicit-face membership, mathematical classification,
translated/reversed frames, source immutability, all work budgets, overly broad
interval refusal and late cancellation. Partial/multi-sheet geometry,
trim-conforming display caches, broad BRep dispatch/history, document persistence
and live FreeCAD source/property/GUI consumers remain required by Stage 4.
