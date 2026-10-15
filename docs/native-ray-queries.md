# Native ray–field queries

`Implicit.intersectRay` queries a compiled immutable native field graph, not its
viewport triangles. It works with primitive/JSON graphs and live operations,
including Part signed-distance inputs when the OCCT bridge is enabled.

```python
hits = Implicit.intersectRay(
    feature, (-5, 0, 0), (1, 0, 0), 0, 10, 512, 1e-7
)
for hit in hits:
    print(hit["distance"], hit["point"], hit["residual"], hit["bracket_width"])
```

Arguments are positional: feature, origin, direction, minimum distance, maximum
distance, optional sample count (default 512), optional distance tolerance
(default `1e-7`). Direction is normalized internally. Distances, points and the
distance tolerance use **feature-local model units**. Feature Placement is not
applied; transform world-space rays into that local frame first. Negative
distances are allowed. Inputs must be finite, direction nonzero, interval ordered,
sample count an integer in `1..65536`, and tolerance positive.

The interval is clipped to the feature's effective AABB before sampling. The
domain clips the query only: it does not introduce artificial cap surfaces.
Zero-length intervals report a hit only for an exactly zero field sample.
Inside convention does not change the mathematical zero set.

The clipped interval is divided into the requested number of segments. Exact
zero samples and opposite-sign brackets generate candidates. Bisection refines
each bracket until its width is no larger than the distance tolerance, with at
most 100 iterations. Failure to reach that width (including floating-point
stagnation) raises an error rather than silently accepting an inaccurate result.
Sorted hits within the distance tolerance of the last retained hit are merged.

Each dictionary contains `distance`, `point`, `residual` (absolute field value)
and `bracket_width`. A zero width means an exactly zero sampled/refined value.
The midpoint's geometric error along an isolated continuous sign bracket is at
most half its width; no global closest-point or root completeness claim follows.
The residual is **not** generally a distance: gyroid values are dimensionless.
The feature's classification tolerance is not reused as a ray distance or
residual tolerance. B-Rep field values retain the OCCT distance/classification
tolerance and may have a zero plateau near a boundary.

Tangencies without a sampled zero and multiple crossings between same-sign
samples can be missed. Closely spaced roots can merge. Adjacent exact zero
samples raise an ambiguity error: the sampler cannot distinguish a ray lying
on the surface, a tolerance plateau, or two roots with that sampling. Hard CSG
creases and poorly scaled fields have no special singularity treatment. This
is a bounded sampled numerical query, not certified CAD intersection topology.

Python captures the compiled Shape with the GIL held, releases the GIL for the
numeric query, and reacquires it to create results. The query reads no document
objects or display caches while running. It is synchronous to its caller and
does not yet accept a cancellation token or connect to GUI selection.

`ray_smoke.py` covers analytical roots, a shell's four crossings, gyroid roots,
clipping/no artificial caps, exact endpoints, signed intervals, direction
normalization, placement/sign conventions, sampled versus missed tangencies,
coincident samples and invalid inputs. `part_linked_smoke.py` verifies mixed
Part/implicit roots and invalidation after editing a live Part source.
