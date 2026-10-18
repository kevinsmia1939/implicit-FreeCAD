# Native mixed-region classification

This advances the active through-Stage-4 goal's field-aware classification gate.
It does **not** finish Stage 3 native-solid validation or conforming meshing,
nor Stage 2 curved operands or Stage 4 FreeCAD property/persistence/selection.

The newer [native geometry consumer](native-geometry-property.md) adds
`Implicit.classifyNative` for whole local/pre-placed ComplexGeoData snapshots.
It preserves the mathematical-region contract while shrinking separation bounds,
expanding witness bounds and reserving placement precision. Selected boundary
elements reject; large-coordinate precision failures remain unresolved. The
local handle API below is unchanged. Native property/persistence and partial
cap/seam selection now exist, but the full Stage 2/3/4 gates are not complete.

`BRepClass3d_ImplicitBox::Classify` queries the authoritative mathematical region
of an unchanged native gyroid/box boundary. It does not classify the display
mesh, approximate cap polygons, or a purported ordinary Part solid. Common
uses `box AND F<=0`; BoxMinusField uses `box AND F>=0`, determined by the owned
implicit face's orientation. The box and field remain immutable snapshots.
Explicit [regularized corner policy](native-corner-classification.md) instead
uses closure of the box's strict-sign three-dimensional material. Fresh corner
identities distinguish retained boundary corners from excluded closed-set
contacts; Taylor exclusion supplies physical lower bounds around the latter.

## Contract

Input tolerance is a **model length**, default 0.1 mm. Raw gyroid residuals are
dimensionless; they are never compared directly with that length tolerance.

| State | Required evidence |
|---|---|
| `inside` | The point is in both mathematical constraints, with a conservative boundary-distance lower bound greater than the requested tolerance |
| `outside` | A violated constraint gives a conservative region/boundary-distance lower bound greater than the tolerance |
| `near_boundary` | A retained strict-sign cap point, a checked retained corner, or a sign bracket entirely within the box proves a boundary point exists within the tolerance ball |
| `unresolved` | Neither separation nor a boundary witness was established; no guessed ON result |

Field distance lower bounds use global Lipschitz exclusion and a local Taylor
bound with the existing gyroid Hessian operator bound. Solving
`abs(F)-reserve = gradient_bound*r + H*r*r/2` gives another exclusion radius,
not a signed-distance approximation. The maximum of these valid lower bounds
is used. Inside an intersection, take the minimum constraint-boundary distance;
outside, take the maximum lower bound among violated constraints.

Near a field surface, the normalized gradient defines a search line clipped
to the box and the tolerance ball. Opposite, reserve-separated signs establish
existence by continuity. Bisection retains the sign bracket; uncertain midpoint
signs are not collapsed to an allegedly exact root. Successful root witnesses
have bracket width <= tolerance/256. A zero/critical gradient without a cap
witness remains unresolved when distance exclusion cannot settle the query.

This is the established ordinary floating-point/reserve policy, **not directed
interval arithmetic**. Finite points, positive tolerances and trigonometric
phase below 1e6 are required. Tolerances below coordinate precision reject.
No global closest point, nonempty/component count, tangent resolution, topology
validation or TopoDS_Solid claim follows from the classification result.

## FreeCAD API

Use the wholly OCCT-8-linked fork:

```python
import ImplicitMixed
import ImplicitOCCT as native
obj = ImplicitMixed.demo()
query = obj.Proxy.classify((0, 0, 2), .1)
snapshot = obj.Proxy.native_boundary()
query = native.classify_mixed(snapshot["boundary_handle"], (-.33, 0, 2), .1)
```

The returned dictionary contains `state`, `distance_lower_bound`,
`distance_tolerance`, `has_boundary_witness`, and `solid_validated=False`.
The native handle API also reports `corner_policy` explicitly.
If a witness exists it adds `witness_first`, `witness_last`,
`witness_distance_upper_bound`, and `witness_bracket_width`. Cap witnesses repeat
one point. Root witnesses give a segment containing a zero, not a guessed exact
surface point. Bounds/witnesses are lengths and coordinates in the native
snapshot's model frame. No implicit consumer dispatch into Part is added.

The feature method queries its last successful recompute. After failed execution
there is no cached region and it raises. An independently retained snapshot
remains queryable after edits/document close and always describes the old region.
Queries do not alter document state or regenerate display meshes. Handles and
witnesses are transient; FCStd still stores the existing recipe, not native
topology. Queries are synchronous; this does not supply whole-workflow scheduling.

## Verification

Kernel tests cover both Boolean directions, inside/outside separation, cap
witnesses from either side, root brackets and domain clipping, invalid inputs,
empty boundary rejection, critical unresolved behavior and coordinate scaling.
The scaling case keeps field values identical while changing the geometric
answer at the same millimetre tolerance, guarding against residual-as-distance.
CLI/GUI tests cover feature calls, operation edits, restore, failed-cache
invalidation, wrong-handle rejection and retained snapshot lifetime.

Verified heads: OCCT `1ba27f551a`, FreeCAD `256ccafef0`; 33/33 kernel cases,
22/22 migration tests (48.32 s), 25/25 compatibility tests (27.63 s).

Remaining requirements are tracked without reducing scope in
[stage-4-acceptance.md](stage-4-acceptance.md).
