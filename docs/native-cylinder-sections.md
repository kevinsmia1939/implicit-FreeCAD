# Native gyroid / cylindrical patch sections

## Explicit cylindrical field-domain restriction

`BRep_TImplicitCylinderPatch` stores a field zero set restricted to the closed
interior of a finite cylinder. Frame, radius and height limits use field-local
coordinates; both geometry and instance placements move the equation and mask
together. The computational box remains a conservative bound. The full oriented
square-prism enclosure must fit inside it. Existing restrictions and child
topology are refused rather than replaced.

```python
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
restricted = native.restrict_cylinder_local(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0), 2, (-1, 1),
)
assert not native.in_domain(restricted, (1.5, 1.5, 0))
restored = native.loads(native.dumps(restricted))  # Explicit schema 3.
```

`IsInDomain` checks the analytic radial/height predicate after converting the
world point to field-local coordinates. Projection brackets and ray roots must
respect it. Scalar Value still evaluates the authoritative equation; domain
membership is separate. Ordinary UV consumers remain unsupported. Existing
sphere tessellation and box builders reject this subtype rather than ignore its
mask. No cap regions, trim wires or valid solids follow from the storage type.
FreeCAD exposes immutable native-face handles and a `cylinder_restricted` flag,
not Part shapes.

Single-face schema 3 preserves the cylinder predicate, equation, box, tolerance,
both placements and orientation. Primitive schemas 1/2 remain unchanged. Unknown
subtypes, unsupported fields and attached topology reject before writing;
malformed input leaves the destination unchanged. Axis-aligned, oblique and
placed restrictions, EmptyCopy, malformed records, projection/ray exclusion and
safe consumer refusal are covered by native tests.

## Cylindrical intersection queries

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

## Analytic boundary-circle edges

`BuildBoundaryRings` consumes that same native boundary result and constructs
analytic `Geom_Circle` OCCT edges for both height circles. Adjacent arcs reuse
the canonical root vertices and retain their brackets/tolerances. Root-free
circles use two semicircle edges with two shared synthetic vertices, avoiding
duplicate endpoint parameter ambiguity on a single closed edge.

Each arc records the field sign strictly between root brackets. A periodic
circle's root count must be even, consecutive signs must alternate, and an
uncertain sign sample rejects the whole edge batch. Root-free rings require
one consistent sign. Endpoint neighborhoods retain the original numerical
uncertainty; this is not an exact material classification inside those brackets.
Bottom edges are reversed for the outward cap orientation; top edges are forward.
Root IDs refer to ascending geometric parameters independently of edge orientation.

```python
rings = native.build_gyroid_cylinder_rings(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (3, 3 + 2 * math.pi), (0, .5),
)
assert rings["ring_edges_verified"]
bottom, top = rings["boundary_rings"]
```

The C++ result owns actual edges and shared vertices. FreeCAD returns copied
descriptors (ranges, sign, orientation, root IDs, endpoints and tolerance),
not mutable Part shapes. `ring_edges_verified` qualifies the circle partition,
incidence and sign-interval checks only. It does not qualify cap regions, attached
p-curves, cylindrical trim faces, closed shells or Boolean solids. No edges are
published when any interval or incidence check fails. Existing source geometry,
records and proof flags remain unchanged.
Ring coordinate reserves and edge tolerances must also fit the requested
distance tolerance; a resolved section alone does not establish that ring budget.

Tests additionally run OCCT's conventional edge validator, check exact circle
curve types, shared root/adjacent-arc vertex identity, full angular coverage,
dense independent gyroid sign samples, root-free positive/negative circles,
rigid placement and budget failure. Native cap-region construction must still
respect the stricter ring precision guard, including translated-coordinate cases,
connect these circle arcs to planar implicit-section curves and qualify the
retained regions before shell/solid assembly.
Circle signs alone do not classify the disk interior: cap construction must also
find and qualify interior closed implicit contours, including disconnected
regions and holes. Disk-section queries now attach circle crossings by retained
root brackets and accepted cell identities, without welding independently traced
endpoints. Assembly must use the explicit cylinder
restriction and still provide a complete native topology/source family.

## End-cap disk contour graphs

`BRep_ImplicitPlaneSection::TraceGraph` retains accepted Cartesian cells and
shared root indices. `TraceEndCapDisks` uses those cell proofs to constrain
each cap's zero set to a disk. Whole inside/outside cells are handled directly;
cut cells use unambiguous grid-root membership and shared circle-root brackets.
A circle-root bracket must occupy one unique cell. Accepted cut cells have zero
or two crossings, with graph/root incidence checked after assembly. Interior
closed contours are retained even when the circle has no roots.

```python
disks = native.trace_gyroid_cylinder_disks(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (3, 3 + 2 * math.pi), (0, .5),
)
assert disks["disk_contours_verified"]
bottom, top = disks["disks"]
```

The parent `resolved` flag describes the side section; callers must check
`disk_contours_verified`. Each cap has its own curves and global circle-root
endpoint IDs; closed interior curves use `(0,0)`. A failed batch clears both
caps' curves/IDs and resolved flags, including when the side section succeeded.
No native cap-region, trim or solid validity is promoted.

Detailed planar roots use tolerance/4096 brackets and shared height-circle
roots use tolerance/65536. Plane tracing reserves half the requested tolerance
for the disk stage's additional coordinate guards. These are numerical engineering
bounds, not interval certification. The full circumscribed square must resolve;
unknown exterior cells can therefore reject otherwise admissible disks. Circle
roots near Cartesian grid lines may lack a unique cell owner and also reject.
Exact lattice nodes retain the existing identity/coordinate-frame restrictions.

Tests solve the independent analytic equation
`A*sin(k*x) + B*cos(k*x) + C = 0` on sampled cap rows and compare all in-disk
roots against the output contours. Coverage includes a closed interior island
with no circle crossings, exact and nonlattice interior zeros, empty and placed
queries, cancellation, budget failures and genuine interior critical-contact
refusal. Cap-region orientation, nesting and actual edge/face construction are
available through the separate [native cap-region builder](native-cylinder-caps.md);
these disk reports remain polylines with root provenance.

The intersection-query curves are tolerance-bearing polylines on a mathematical cylindrical
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
sources, reconfigure OCCT and both test harnesses first. Existing section
Settings/Result layouts and primitive schemas 1/2 are unchanged; schema 3 adds
the cylinder mask. Cell/disk graphs use new result types. The full native runner
now allows 600 seconds of wall time after a 417.75-second baseline; numerical
and per-operation resource checks are unchanged.
