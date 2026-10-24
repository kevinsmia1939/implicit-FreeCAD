# Native gyroid / cylindrical patch sections

`BRep_ImplicitCylinderSection::Trace` adds a native OCCT query for a finite open
cylindrical patch intersecting a childless exact gyroid face. A separate periodic
query also handles one full turn. FreeCAD exposes both modes
through the ABI-checked `ImplicitOCCT.trace_gyroid_cylinder` facade:

```python
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
section = native.trace_gyroid_cylinder(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    2, (.27, 1.1), (-4.2, 3.7), distance_tolerance=.1,
)
if section["resolved"]:
    curves = section["curves"]
```

The frame specifies origin, axis and angle-zero X direction. Radius, heights
and tolerance use model lengths; angles use radians. The angular span must be
strictly less than a full turn in the default open mode. With explicit
`periodic=True`, angle_max must equal `angle_min + 2*pi`. The C++ entry point is
`TracePeriodic`; Settings and Result layouts are unchanged. The conservative oriented square-prism enclosure
must fit entirely inside the field's computational box. This deliberately rejects
some admissible small patches whose larger enclosure leaves that domain.

The chart uses arc length `u = radius * angle` and axial height `v`. Its Jacobian
has orthonormal columns. The gyroid's spatial Hessian bound is augmented by
`LipschitzBound / radius` for chart curvature. Empty-cell Lipschitz tests,
monotone edge-root brackets, regular-cell derivative cones and shared grid-edge
identities govern acceptance. Cell diameter plus the coordinate reserve must
fit the requested geometric tolerance before producing a segment. Field
residual is diagnostic, separate from that model-length bound. Floating-point
reserves are engineering policies, not outward-rounded interval arithmetic.

Near-zero samples, tangencies, insufficient budgets and ambiguous cells remain
unresolved. Callers must inspect `resolved`; partial curves are diagnostic.
The periodic graph indexes each vertical seam edge/root once, reusing its value
and vertex ID at the opposite chart boundary. This uses the cylinder's exact
periodic chart identity, without endpoint proximity welding. Seam points must
have interior incidence; only the two height boundaries can have open ends.
Closed loops repeat exactly the same stored first point. Near-zero seam samples
still remain unresolved. Bounded alternative grids can avoid unresolved sampling
alignments, using identical acceptance tests and the same maximum axis budget.

```python
import math
loop = native.trace_gyroid_cylinder(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (.27, .27 + 2 * math.pi), (-1.7, 1.3), periodic=True,
)
assert loop["resolved"] and loop["periodic"]
assert loop["curves"][0]["closed"]
```

The result never sets topology or solid validation flags. C++ supports a
caller-thread cancellation checkpoint. Primitive field records stay unchanged.

## End-cap root identities

`TracePeriodicBoundary` retains the side trace's actual height-boundary root
identities. Its new BoundaryResult owns canonical `TopoDS_Vertex` handles,
bracket endpoints and unwrapped angular brackets, sorted by bottom/top then
chart angle. `CurveRoots` links every open curve endpoint to a 1-based root ID;
closed curves use `(0,0)`. All roots must have exactly one open endpoint use.
Unresolved or inconsistent results expose no anchors. These IDs are local to
one result, not edit-surviving or persistent topology names.

```python
boundary = native.trace_gyroid_cylinder_boundary(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (3, 3 + 2 * math.pi), (0, .5),
)
assert boundary["resolved"] and boundary["boundary_incidence_verified"]
roots = boundary["boundary_roots"]
endpoints = boundary["curve_boundary_roots"]
```

The FreeCAD API returns copied root descriptors, including vertex tolerance;
it does not expose mutable OCCT vertices. The native C++ result retains the
vertices so future side/end-cap trim builders can share the same handles rather
than reidentify approximate coordinates. The incidence flag qualifies this
root/curve relation only. No planar end-cap region or cylindrical trim edge is
constructed yet, and topology/solid validation remain false.

These curves are tolerance-bearing polylines on a mathematical cylindrical
patch. Periodic graph seam joins are supported, but these are not native trim
edges, cap regions or gyroid/cylinder Boolean solids. No GUI Boolean accepts
cylindrical operands as a consequence of this query. Next curved-operand work must
consume the shared root anchors in planar end-cap construction, construct oriented trims
and retained regions, then qualify shell/solid geometry and persistence.

Kernel tests independently solve
`A*cos(k*z) + B*sin(k*z) + C = 0` at sampled angles and check reference roots
against the output curves. They also check exact cylinder membership, root
bracket widths, empty results, budget exhaustion, rigid placement, cancellation
and invalid chart rejection. Periodic tests check an analytic closed loop,
moving-seam invariance, height-only open endpoints, budget exhaustion and
unresolved zero seam samples. The FreeCAD CLI test covers ownership,
unchanged primitive records, translation, deterministic restored queries and
separate proof flags.
Boundary tests additionally check four distinct ordered cap roots, sign brackets,
vertex-tolerance containment, exact curve-endpoint identity, copied OCCT handle
identity, closed-curve sentinel IDs and empty anchors on unresolved queries.

```sh
cmake --build build-occt-implicit --target TKBRep --parallel 2
cmake --build build-occt-implicit/native-face-tests \
  --target implicit_occt_native_face_tests --parallel 2
build-occt-implicit/native-face-tests/implicit_occt_native_face_tests \
  --gtest_filter='BRepImplicitCylinderSection.*'
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests \
  -R 'cylinder_section|lattice_section' --output-on-failure
```

Reuse the configured OCCT 8 build and matching FreeCAD runtime. After adding
sources, reconfigure OCCT and both test harnesses first. Existing type layouts
and record formats are unchanged; this adds a new query class/API.
