# Native FreeCAD field graph

The in-tree FreeCAD `Implicit` module stores a composable field graph in the
`FieldGraph` property of `Implicit::Feature`. The property is versioned JSON and
is persisted directly in the FreeCAD document. In `Operation=Definition`, an
empty value uses the single-primitive properties. Other operations use live
document links, not `FieldGraph`; the JSON schema remains 1 while new features
have document `SchemaVersion=3`.

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
| `box` | `center`, `halfSize` | axis-aligned box signed distance |
| `band` | `field`, `halfWidth` | `abs(field)-halfWidth`; width has input field units |
| `brep` | `data`, `tolerance` | numerical signed distance to an ASCII BRep solid snapshot; requires OCCT extension build |
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

`Implicit.project` now exposes local stationary projection with convergence and
failure diagnostics. `Implicit.inspectTessellation` measures sampled residuals
and edge topology on a fresh transient extraction. See
[projection and quality](native-projection-and-quality.md) for signatures and
accuracy contracts; neither API certifies a closest point or manufacturing solid.

Native `Implicit.intersectRay(feature, origin, direction, minDistance,
maxDistance, samples=512, distanceTolerance=1e-7)` now queries compiled fields
directly. See [native ray queries](native-ray-queries.md) for local-coordinate,
distance/residual, domain-clipping and sampled-root limitations.

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

This uniform-grid baseline is also the reference for octree-pruned sampling. Sampling can miss
thin components, tangencies, and multiple roots along an edge. Connectivity is
regression-tested on enclosed spheres and a hollow sphere, without a general
topology or watertightness guarantee. Vertices and triangles each have a one
million element budget; exceeding either raises an error. Native Python
extraction releases the GIL after capturing immutable geometry; argument parsing
and Python-result conversion still hold it. Python tessellation calls remain
synchronous. The native `ImplicitGui::ViewProviderShape` also
offers opt-in `BackgroundMeshing` with bounded/coalesced jobs, cancellation and
generation-checked uploads; see [the background contract](background-meshing.md).
The provider consumes and caches the extractors; view properties control resolution,
adaptive sampling, color and shaded/wire/flat-line modes. Adaptive display
rounds the requested resolution up to the next power of two. Workbench Boolean
commands create live links, including direct single-solid Part operands when
the Part/OCCT bridge is built.

## Adaptive sampling and batches

`Implicit.tessellateAdaptive(feature, 32)` uses Lipschitz bounds to reject empty
octree blocks. Resolution must be a power of two in `[2,64]`. It returns the same
mesh fields plus `pruned_blocks`; `sample_count` counts evaluated grid nodes,
excluding root refinement. Uniform extraction also returns `pruned_blocks=0`.
Surface cells all reach the finest grid level, preserving common edge keys and
avoiding mixed-level seams. The sample storage remains dense. Fields without a
finite conservative bound (currently B-Rep fields) fall back to complete
sampling. This does not guarantee detection of all sub-grid geometry or bound
triangle-interior approximation error.

`Implicit.evaluateBatch(feature, [(x,y,z), ...])` returns scalar values and uses
one compiled graph for the call. Input points must have three finite numeric
coordinates. The batch limit is one million points. Compiled field graphs are
cached on the feature and invalidated on relevant property changes. Its numeric
evaluation loop releases the GIL; conversion into/out of Python does not.

## Live document composition

```python
left = doc.addObject("Implicit::Feature", "EditableSphere")
left.Function = "Sphere"
left.Radius = 5
right = doc.addObject("Implicit::Feature", "OtherSphere")
right.Function = "Sphere"
right.Radius = 3
right.Placement.Base = (4, 0, 0)
result = doc.addObject("Implicit::Feature", "Cut")
result.Operation = "Difference"
result.Left, result.Right = left, right
doc.recompute()
left.Radius = 6
doc.recompute()  # propagates through persistent PropertyLink dependencies
```

Operations are `Definition`, `Union`, `Intersection`, `Difference`, `Band`,
`Offset`, and `BRep`. `Band`/`Offset` use `Left` and `Amount`, in input field
units. `BRep` uses a single-solid Part feature as `Left`; binary operations also
accept Part features directly. This requires `BUILD_PART` and
`IMPLICIT_OCCT_SOURCE_DIR`; JSON snapshot support does not require Part.

Operands' global placements are mapped into the result's parent frame; the
result's own Placement is an additional transform of the assembled field.
Gradients are rotated back, and positive-inside operands are sign-normalized. Recompute
after edits before querying results. Native links share immutable child fields
instead of duplicating JSON. This is a document-level DAG, not a new JSON DAG
format or dedicated geometry property. Missing/failed sources are errors, not
usable stale geometry; cycles and excessive native dependency depth are rejected.
Skipped downstream features are invalidated after document recompute when a
source fails; `StatusMessage` reports the reason. Use `feature.isValid()` for
validity checks, not a search for an `Error` string in Python's `State` list.

`AutoDomain=True` uses the transformed union of implicit operand domains, or
padded transformed B-Rep bounds. It is conservative for all binary operations.
`Implicit.bounds(feature)` returns the effective local evaluation domain.
`DomainMin`/`DomainMax` are the manual override when `AutoDomain=False`, and
always apply to definitions. A sampling domain does not cap a field or restrict
the operand's mathematical sign set. Expanded offsets may need a larger manual
domain: field units are not universally physical length, so no automatic offset
padding is inferred. Changes to enclosing document containers, App::Link
instances, and cross-document links are not yet broadly regression-tested.

## Python extraction cancellation

```python
token = Implicit.newCancellationToken()
# Another Python thread may call Implicit.cancel(token).
mesh = Implicit.tessellate(feature, 64, token)
```

Both extractors accept the optional third token (or `None`). Tokens are one-shot
and atomic; cancellation raises `RuntimeError` containing `cancelled`, returning
no partial mesh. Checks occur during sampling and cell/root processing. A single
expensive OCCT evaluation is not interruptible mid-query. Python-result
conversion is not cancellable. Background code should signal only the token;
FreeCAD document and GUI APIs remain main-thread operations. The API does not
make arbitrary concurrent document mutation safe.

## Bounded bands and B-Rep snapshots

A capped gyroid visualization region can be defined as:

```json
{"schema":1,"root":{"type":"intersection",
  "left":{"type":"band","halfWidth":0.3,
    "field":{"type":"gyroid","period":10,"iso":0}},
  "right":{"type":"box","center":[0,0,0],"halfSize":[8,8,8]}}}
```

Set the computational domain larger than the box. These box boundaries supply
cap surfaces; clipping to the computational domain alone does not. A gyroid
band width is dimensionless and does not prescribe uniform physical thickness.

With `IMPLICIT_OCCT_SOURCE_DIR` configured, a `brep` node stores `data` as ASCII
BRep text and a positive model-length `tolerance`. Only valid single bounded
outward-oriented solids are accepted. JSON snapshots never track source edits.
The workbench's **Live B-Rep field** command instead stores a Part source link;
recompute captures a new isolated BRep evaluator and updates automatic bounds.
The source must contain a genuine single solid, not an arbitrary compound.
Mixed field Booleans
produce implicit fields, not `TopoDS_Shape` results. A node's numerical distance
and finite-difference gradients can be expensive. Combining distance and gyroid
fields preserves sign-set operations but gives no common physical distance unit.
