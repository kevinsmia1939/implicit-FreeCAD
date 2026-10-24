# Native gyroid / cylindrical patch sections

`BRep_ImplicitCylinderSection::Trace` adds a native OCCT query for a finite open
cylindrical patch intersecting a childless exact gyroid face. FreeCAD exposes it
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
strictly less than a full turn. The conservative oriented square-prism enclosure
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
The result never sets topology or solid validation flags. C++ supports a
caller-thread cancellation checkpoint. Primitive field records stay unchanged.

These curves are tolerance-bearing polylines on a mathematical cylindrical
patch. They are not native trim edges, periodic seam joins, cap regions or
gyroid/cylinder Boolean solids. No GUI Boolean accepts cylindrical operands as
a consequence of this query. Next curved-operand work must join periodic
charts, identify shared roots with planar end caps, construct oriented trims
and retained regions, then qualify shell/solid geometry and persistence.

Kernel tests independently solve
`A*cos(k*z) + B*sin(k*z) + C = 0` at sampled angles and check reference roots
against the output curves. They also check exact cylinder membership, root
bracket widths, empty results, budget exhaustion, rigid placement, cancellation
and invalid/periodic chart rejection. The FreeCAD CLI test covers ownership,
unchanged primitive records, translation, deterministic restored queries and
separate proof flags.

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
