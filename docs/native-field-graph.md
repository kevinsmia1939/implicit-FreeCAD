# Native FreeCAD field graph

The in-tree FreeCAD `Implicit` module stores a composable field graph in the
`FieldGraph` property of `Implicit::Feature`. The property is versioned JSON and
is persisted directly in the FreeCAD document. An empty value retains the
single-primitive property interface for compatibility and simple editing.

## Schema 1

Every document has this envelope:

```json
{
  "schema": 1,
  "root": {
    "type": "sphere",
    "center": [0, 0, 0],
    "radius": 10
  }
}
```

Supported nodes are:

| Type | Members | Field semantics |
|---|---|---|
| `sphere` | `center`, `radius` | `length(p-center) - radius` |
| `gyroid` | `period`, `iso` | analytical gyroid field minus `iso` |
| `union` | `left`, `right` | `min(left, right)` |
| `intersection` | `left`, `right` | `max(left, right)` |
| `difference` | `left`, `right` | `max(left, -right)` |
| `translate` | `field`, `offset` | evaluates `field(p-offset)` |
| `offset` | `field`, `amount` | `field(p) - amount` |

The convention is negative-inside unless `InsideNegative` is disabled on the
feature. A positive offset amount expands a negative-inside field. Hard Boolean
boundaries are not differentiable where their operands are equal; evaluation
selects one branch deterministically at such points.

## Python interface

```python
import json
import FreeCAD
import Implicit

doc = FreeCAD.newDocument()
feature = doc.addObject("Implicit::Feature", "NativeField")

graph = {
    "schema": 1,
    "root": {
        "type": "difference",
        "left": {"type": "sphere", "center": [0, 0, 0], "radius": 10},
        "right": {"type": "sphere", "center": [0, 0, 0], "radius": 8},
    },
}

Implicit.setFieldGraph(feature, json.dumps(graph))
doc.recompute()
value = Implicit.evaluate(feature, 9, 0, 0)
classification = Implicit.classify(feature, 9, 0, 0)
```

`setFieldGraph` validates and canonicalizes JSON before changing the feature.
`validateFieldGraph` validates without changing a document, and `getFieldGraph`
returns the canonical stored form.

## Validation limits

Schema 1 rejects unknown members and non-finite parameters. Documents are
limited to 4 MiB, 128 levels of nesting, and 4096 nodes. These limits protect
interactive document loading; they are not intended as modeling limits for the
eventual shared-node/DAG representation.

This graph is authoritative geometry. It is not a `TopoDS_Shape`, and any mesh
generated from it is a cache or conversion result.

## Surface extraction

The native C++ extractor accepts any currently supported graph or primitive:

```python
# Run in the fork's FreeCAD Python environment after creating feature above.
cache = Implicit.tessellate(feature, 24)
vertices = cache["vertices"]    # list of (x, y, z), feature-local coordinates
normals = cache["normals"]      # one normal per shared vertex
triangles = cache["triangles"]  # list of three zero-based vertex indices
grid_samples = cache["sample_count"]  # (resolution + 1)**3
```

The optional resolution defaults to 24 and must be an integer between 2 and 64,
inclusive. This is the cell count along each axis, even for anisotropic domains.
The extractor samples the graph once per grid node, decomposes each cube into
six consistently aligned tetrahedra, and shares roots using global grid-edge
identities. Exact zero-valued grid endpoints share a vertex across all incident
edges. Each other crossing is refined by up to 24 bisection steps. Normals come
from the selected graph gradient; winding follows `InsideNegative`.

The `sample_count` reports grid samples only, excluding root-refinement and
normal evaluations. Root refinement reduces vertex field residuals; the
resolution still controls triangle approximation error. No global geometric
error bound is reported. At critical points a zero gradient produces a zero
normal. Hard Boolean branch boundaries can have discontinuous gradients.

The returned cache is transient: it does not change document properties, is
not serialized, and can be regenerated from the persisted graph and domain.
Extraction uses feature-local coordinates like `evaluate` and `gradient`; a
renderer applies `Placement` once. The extractor does not cap domain boundaries.
A field with no sampled sign change returns empty arrays; that result is not
proof that no surface exists between samples.

This is a uniform-grid baseline for later adaptive extraction. Sampling can miss
thin components, tangencies, and multiple roots along an edge. Connectivity is
regression-tested on enclosed spheres and a hollow sphere, without a general
topology or watertightness guarantee. Vertices and triangles each have a one
million element budget; exceeding either raises an error. Extraction and Python
conversion currently run synchronously with the GIL held. There is no dedicated
native feature view provider yet.
