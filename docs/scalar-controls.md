# Scalar controls and smooth field composition

The native schema-1 graph now supports four additional node types. They reuse
the existing immutable fields, strict validation, document persistence, native
gradients, compiled caches and extraction APIs. This is an additive field-node
increment, not a shared-node/DAG schema or dimension-checking type system.

| Node | Members | Equation | Conservative Lipschitz bound |
|---|---|---|---|
| `constant` | `value` | constant scalar | 0 |
| `linear` | `slope` (3-vector), `intercept` | `dot(slope,p)+intercept` | length of slope |
| `variable_band` | `field`, `width` (both field nodes) | `abs(F(p))-W(p)` | `L_F + L_W` |
| `smooth_union` | `left`, `right`, positive `blend` | polynomial smooth minimum below | `max(L_left,L_right)` |

For smooth union let `a=left(p)`, `b=right(p)`, `k=blend` and
`h=clamp(0.5+0.5*(b-a)/k,0,1)`. The result is
`lerp(b,a,h)-k*h*(1-h)`. Its gradient is the convex combination
`h*gradient(left)+(1-h)*gradient(right)`, including cancellation of the
weight's derivative. This smooths the min transition but does **not** preserve
an exact signed-distance property. Non-smooth operands can still contain
creases away from the blend transition.

Example: a gyroid level-set band that grows along local x:

```python
graph = {
    "schema": 1,
    "root": {
        "type": "variable_band",
        "field": {"type": "gyroid", "period": 10, "iso": 0},
        "width": {"type": "linear", "slope": [0.01, 0, 0], "intercept": 0.3},
    },
}
Implicit.setFieldGraph(feature, json.dumps(graph))
feature.Document.recompute()
```

The negative-inside set is `abs(F)<=W`. Negative width values yield no interior
at that location, rather than silently taking their absolute value or clamping
them. A zero width is a zero-thickness set; extraction may miss it. The gradient
subtracts `gradient(W)` and changes the sign of `gradient(F)` according to F.
At `F=0` the absolute-value derivative is undefined; a deterministic positive
branch is used, as with the fixed-band node.

All coefficients must be finite. Width/intercept/blend use field units and slope
uses field units per local model length. Operands of a smooth union and its
blend must have compatible scalar units; width must match its input's units.
**The graph does not enforce those dimensional contracts yet.** Raw gyroid
values/widths are dimensionless and do not imply millimetre wall thickness.
Do not smooth-blend distance-valued and dimensionless fields as though their
amplitudes were physically interchangeable. Even for distance-valued inputs,
the smooth result is generally not an exact distance field.

Analytical derivatives and conservative bounds feed existing projection, chart,
quality and controlled extraction APIs. An unbounded child still disables
finite-bound-controlled extraction. Generated meshes remain transient; no new
native OCCT geometry or manufacturing representation is implied.

`scalar_smoke.py` verifies equations, analytic gradients against finite
differences on both band branches and smooth regions, negative-width behavior,
smooth-field Lipschitz samples, controlled extraction, strict invalid-node
rejection, and FCStd round-trip values/graphs. Interactive graph editing,
automatic unit checking, shared-node persistence and spatially varying physical
thickness are separate future milestones.
