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
