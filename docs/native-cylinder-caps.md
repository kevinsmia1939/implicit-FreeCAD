# Native planar cylinder cap regions

`BRepClass3d_ImplicitCylinder::BuildCaps` constructs actual planar OCCT faces
for the retained field material on both cylinder end disks. The default selects
F<=0; `negative=False` selects F>=0. This is the cap portion of a future mixed
cylinder Boolean. No cylindrical side faces, field-face assembly or solid is
created by this API.

```python
import math
import ImplicitOCCT as native
field = native.gyroid(10, .21, (-10, -10, -10), (10, 10, 10))
caps = native.build_cylinder_caps(
    field, (0, 0, 0), (0, 0, 1), (1, 0, 0),
    .5, (3, 3 + 2 * math.pi), (0, .5), negative=True,
)
assert caps["cap_topology_validated"]
assert not caps["solid_validated"]
bottom_faces, top_faces = caps["caps"]
```

The frame and dimensions use the same world-coordinate convention as the
[cylinder section queries](native-cylinder-sections.md). The 0.1 mm default
contour target, primitive/placement support and conservative tracing refusals
remain unchanged. Source masks or attached topology that the section query does
not support are still rejected.

`BuildCapSkeleton` computes one coordinated disk/ring result. Circle arcs and
field seams reuse its actual OCCT root vertices. Field seams are degree-1
B-splines following the accepted disk graph, with the recorded contour-distance
bound. Their poles are not separate topological vertices or mesh faces. Circle
edges remain analytic. Both curve kinds have matching p-curves on genuine
planar surfaces; no UV parameterization is invented for the implicit field.

Gradient orientation is checked against Hessian/coordinate reserves. The retained
material stays to the left in each cap's outward frame. Directed vertex incidence
must form cycles. Finite signed-area bounds, nesting parity and conventional
BRepCheck face validation gate region publication. Closed interior contours
produce holes or islands as appropriate. Full and empty cap sets are supported.
All geometry remains numerical/tolerance controlled; the flags are engineering
validation rather than interval certification or a general mixed-BOP guarantee.

Nesting tests are bounded by `maximum_nesting_tests` (default 2,000,000; allowed
1..10,000,000). Cancellation checkpoints cover tracing, edge/cycle assembly and
nesting. Any failed cap invalidates the whole face/field-edge batch, including
an earlier successful cap. The report's `reason` identifies refusal. Boundary
qualification, cap topology, contour distance and solid validity remain separate.

The immutable `caps_handle` retains the shared native graph, source equation and
canonical settings. Callback pointers are never retained. `caps` contains detached
ordinary Part.Face previews obtained through explicit B-Rep copies; editing them
does not alter the owner. `inspect_cylinder_caps(handle)` reads the owner without
rebuilding contours or caps. `field_record` and `input_settings` preserve source
provenance. These previews must not be used to claim a completed implicit solid.

`cap_areas` and `area_error_estimates` use adaptive OCCT integration at 1e-10
relative tolerance. Nonadaptive integration can underresolve many-knot seams.
These values describe the constructed cap faces, independently of the linear
contour approximation target; they do not certify exact mathematical material
area or volume.

Native tests independently check field-sign classification away from the contour
uncertainty band, shared OCCT vertex identity, plane normals, actual BRepCheck
validity, holes/islands and complementary area sums against pi*radius^2.
Additional cases cover full/empty caps, rigid placement, cancellation, critical
contacts, nesting exhaustion and nonfinite area refusal. FreeCAD tests cover
source/owner lifetime, preview mutation, precise inspection and typed policies.

```sh
cmake --build build-occt-implicit --target TKTopAlgo --parallel 2
cmake --build build-occt-implicit/native-face-tests \
  --target implicit_occt_native_face_tests --parallel 2
build-occt-implicit/native-face-tests/implicit_occt_native_face_tests \
  --gtest_filter='BRepClass3dImplicitCylinder.*'
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests \
  -R cylinder_caps --output-on-failure
```

Reconfigure the existing OCCT source build and both test harnesses when adding
these files. Reuse matching OCCT 8/FreeCAD binaries; no system library installation
is required. Next gates are cylinder-side retained regions and p-curves, shared
implicit-face seams/components, closed shell/solid validation and full native
persistence plus live FreeCAD consumers.
