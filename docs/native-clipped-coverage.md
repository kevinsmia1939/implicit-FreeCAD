# Whole-domain clipped-boundary proximity

The OCCT-8 native module now checks both directed distances between a transient
mixed mesh and the mathematical boundary of `box AND F<=0` (Common) or
`box AND F>=0` (BoxMinusField). The default physical target is **0.1 mm**.
This advances Stage 3 proximity validation. It does not prove topological
equivalence, component preservation, native-solid validity or Stage 4 completion.

```python
import ImplicitOCCT as native
report = native.check_mixed_coverage(obj.Geometry)
assert report["boundary_proximity_resolved"]
```

The explicit API privately regenerates a candidate and validates a private copy
of the supported native gyroid/axis-box definition. It neither trusts mesh
metadata nor changes source topology, flags, caches, geometry properties or GUI
buffers. Saved seam-bound provenance remains unchanged, including unverified
restored bounds. This is not a recipe/topology rebuild or a Part shape conversion.

## What is checked

For mathematical-boundary-to-mesh distance, the entire closed box is adaptively
partitioned, not sampled at a fixed set of vertices. Every volume cell must be:

- excluded from F=0 by a Lipschitz/Taylor variation bound; or
- wholly within the tolerance of an explicit point on an implicit mesh triangle.

Each of the six box faces is partitioned similarly. Face cells are excluded
only if their entire field-sign interval is outside the retained region;
otherwise they must be within tolerance of cap triangles on that same plane.
This also covers face/edge/corner limits. Possible zero cells are not discarded
just because corners have the same sign. A triangle BVH supplies upper distances
to explicit convex triangle points. Its box lower bounds are used only for
conservative pruning, not as nearest-point certificates.

For mesh-to-mathematical-boundary distance, all cap/implicit triangles are
checked adaptively. Correct-sign planar patches can be accepted directly.
Interior patches may use a triangle-wide normal-line root witness only when
the ENTIRE witness slab stays inside the box. Otherwise a local patch uses a
clipped sign bracket from the native region classifier or an explicit retained
point on a true box side, then adds the patch radius to its witness distance.
Retained cap witnesses use projected gradients and in-plane coordinate directions;
a 3D gradient alone can leave the box and miss a nearby planar witness. Failed
patches subdivide without changing the mesh. No untrimmed-distance flag or raw
field residual is accepted as physical distance to the clipped boundary.

For a spatial cell of center `p` and radius `r`, the field variation is bounded
by the minimum of `L*r` and `|gradient(p)|*r + H*r*r/2`, with numerical reserves;
the gyroid has `H=4*(2*pi/Period)^2`. If the center-to-mesh upper distance plus
`r` meets tolerance, every possible boundary point in that cell is covered.
Reverse patch witnesses use the analogous triangle-radius inequality or the
normal-line quadratic bound described in native-conforming-preview.md.

These are ordinary floating-point bounds with conservative engineering reserves,
**not directed interval arithmetic or an exact implicit-geometry certificate**.
They are independent of the exact stored-coordinate triangle-contact predicates.

## API and result contract

```python
report = native.check_mixed_coverage(
    obj.Geometry, .1, 256, 14, 2000000, 1000000)
```

Arguments: immutable mixed-boundary handle, tolerance (mm), maximum candidate
grid cells, maximum checking depth, spatial-cell budget, triangle-patch budget.
The kernel counterpart is `BRepMesh_ImplicitBoundary::CheckCoverage(boundary,
mesh, tolerance, depth, cells, patches)`; it can check supplied transient buffers
and ignores their provenance/validation flags.

- `boundary_proximity_resolved`: True ONLY after both directed checks finish.
- `boundary_to_mesh_bound`, `mesh_to_boundary_bound`: global upper bounds ONLY
  when resolved; otherwise these are partial diagnostics, not certificates.
- `spatial_cells`, `triangle_patches`: checking work performed.
- `unresolved_triangle`: caps-first zero-based root triangle, or -1 for the
  forward spatial check. `unresolved_minimum` / `unresolved_maximum` identify
  the failed spatial cell or triangle patch; meaningful only if unresolved.
- `distance_tolerance`, `cells`: requested tolerance and accepted mesh grid.
- `contact_audit_performed=False`, `topology_validated=False`,
  `solid_validated=False`, always. Run the separate contact audit if required.

Depth is 1..20 (default 14); spatial budget 1..10,000,000 (default 2,000,000);
patch budget 1..4,000,000 (default 1,000,000). Candidate extraction retains its
own limits. Work/depth exhaustion returns unresolved, never success. Invalid
settings, nonfinite/out-of-box mesh nodes, wrong cap planes/tags and invalid
indices reject. Coordinate/trigonometric/derivative precision guards apply;
unsupported numerical ranges reject instead of silently weakening the bound.
The limits do not promise exact peak allocation or runtime.

## Evidence and next gates

Kernel tests cover both operations and closed/open contours; independent
harmonic z roots and raw-equation retained-cap samples are checked against a
separate brute-force triangle-distance reference. Negative cases remove all
field/cap geometry, remove ONE disconnected field component while leaving the
other, insert an extra triangle deep inside the volume, forge metadata and
exhaust depth/spatial/patch budgets. The component and extraneous-face cases
ensure the check is not merely a mesh residual or a one-direction test.
The native CLI additionally checks immutable snapshots, restore with stored
seam bounds unverified, lifetime after document close, limits and bad inputs.

A resolved proximity bound plus a clear mesh-contact audit is still NOT proof
that the native boundary has the correct topology. Features smaller than the
tolerance can be missed while two point sets remain within tolerance; no minimum
feature size/reach or isotopy certificate has been established. Regularity,
stratified trim correspondence, native-shell/solid construction and implicit-aware
topology/solid validation remain required. Broader curved operands and unfinished
FreeCAD history/snapping/consumer gates remain in the active Stage 4 goal.

Final checkpoint evidence: 45 kernel cases (37.18 seconds), 32/32 OCCT-8
migration tests (182.37 seconds) and 25/25 compatibility tests (69.67 seconds).
These are regression timings under concurrent test load, not performance claims.

Reproduce using existing builds:

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests -R 'implicit_occt8_mixed_coverage$' --output-on-failure
```

Checks are explicit and synchronous; GUI previews do not automatically run them
or persist a validation claim. Use the wholly OCCT-8 FreeCAD build. No new
dependencies or system-library changes are required.
