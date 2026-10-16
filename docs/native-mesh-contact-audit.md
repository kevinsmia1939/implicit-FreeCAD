# Exact transient mesh contact audit

The OCCT-8 native module now provides a separate, explicit audit of triangle
contacts in mixed mesh candidates. This advances the Stage 3 geometric checks;
it does not prove implicit-surface coverage, clipped-field approximation,
native-solid validity or completion of Stage 4.

```python
import ImplicitOCCT as native
report = native.audit_mixed_mesh(obj.Geometry, .1, 256, 2000000, 50000000)
```

Arguments are the immutable native mixed-boundary handle, physical meshing
tolerance, maximum longest-axis cells, maximum exact pair tests and maximum
broad-phase tests. The function generates a private transient candidate using
the existing mesher and audits **all cap and implicit triangles**, including
pairs sharing nodes/edges. It does not reuse or certify a mutable GUI buffer.
It does not modify/persist native geometry, display buffers, source caches or
any validation flag on the document. Generation and audit are synchronous.

## Report

- `state="clear"`, `triangle_contacts_clear=True`: every pair was culled by
  disjoint coordinate bounds or has an exact separating-plane witness proving
  it meets only at its shared node-ID simplex, if any.
- `state="contact_or_unresolved"`, clearance False: an improper contact,
  duplicate/degenerate triangle or lack of a separating witness was found.
  This is NOT a promised intersection point/curve or automatic repair.
- `state="limit"`, clearance False: work/arithmetic limits prevented a complete
  audit. A partial successful scan never establishes clearance.
- `first_triangle`, `second_triangle`: diagnostic zero-based triangle IDs.
  Caps precede implicit triangles; -1 means none (or no second triangle for a
  degenerate face). Counts are `cap_triangle_count`, `implicit_triangle_count`.
- `pair_tests`, `broad_tests`: work performed. `distance_tolerance`, `cells`
  and `node_count` identify generation settings/size, not a persistent certificate.
- `surface_coverage_verified=False`, `solid_validated=False`, always.

The candidate's separate paired-edge/cyclic-link checks still matter. A contact
audit alone says nothing about closedness, orientation, topology completeness,
field classification or correspondence to the intended mathematical region.
In particular, unrelated coincident vertices are not silently welded. Shared
node IDs permit only their common vertex/edge, never arbitrary overlap.

## Kernel implementation

`TKMesh/BRepMesh_ImplicitBoundary::Audit(Result, pairLimit, broadLimit)` also
accepts native transient buffers directly. It ignores untrusted metadata flags
and checks geometry/index/size validity. Nonfinite points, invalid indices,
empty input and invalid limits reject. Degenerate triangles cannot pass.

The broad phase is a bounded X-axis sweep with exact binary64 coordinate-box
comparisons. Overlapping boxes use integer separating-axis/plane predicates.
All nonzero binary64 coordinates are decomposed with `frexp` into an integer
mantissa and binary exponent, then shifted to one common power-of-two scale.
Signed arbitrary-width integers, implemented locally with 32-bit words and
64-bit carries, evaluate vector differences, cross products and dot products.
No floating epsilon, rounded normal, tolerance welding or external arithmetic
package is involved in these predicates. No new dependency was introduced.

Candidate axes include face normals, edge cross products and in-plane edge
normals for coplanar triangles. Any accepted separator is checked exactly. For
triangles sharing a vertex or edge, the separating plane must contain the
ENTIRE common simplex; at least one triangle's nonshared vertices must lie
strictly on its side. This proves intersection is confined to the common
simplex. Simply skipping adjacent pairs would miss same-side overlaps and
crossings beyond a common vertex. The new negative tests caught and fixed an
initial omission of the whole-common-edge requirement.

Failure to find a separating witness is conservative: it does not invent
clearance or claim a resolved intersection. The audit is exact for the stored
binary64 mesh, not exact implicit geometry or a physical clearance tolerance.
It is not a minimum-separation or near-contact-distance calculation.

Resource limits: at most 1,000,000 nodes / 2,000,000 triangles; nonzero input
coordinate binary-exponent spread at most 512. Maximum requested pair tests
must be 1..10,000,000 and broad tests 1..100,000,000. Defaults are 2,000,000 /
50,000,000. Exponent or exhausted work limits return unresolved. These are not
exact allocation/runtime guarantees; long overlapping AABBs can hit the sweep
budget even when triangles are geometrically separate. A hierarchy/back-end
optimization and cancellation remain future work.

## Evidence and remaining gates

Two new kernel cases cover valid adjacency, crossings beyond shared vertices,
coplanar same-side shared-edge overlap, duplicate faces, unrelated coincident
nodes, tiny nonzero separation, overlapping-AABB oblique separation at binary
scales 2^-100 / 1 / 2^100, winding reversal, arithmetic/work limits and bad
indices. The native iso-0 fixture passes 906,095 exact pair tests. The existing
40 kernel cases remain in the executable (42 total).
Near-parallel planes with overlapping boxes additionally exercise tiny exact
gaps and crossings, rather than relying only on AABB culling. GCC UBSan passes
this predicate case; only the audit/test source is instrumented, not the other
OCCT libraries. Final regression: 42 kernel cases (20.03 seconds), 31/31
migration (121.03 seconds), 25/25 compatibility tests (45.43 seconds).

Reproduce with the existing configured builds:

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests -R 'implicit_occt8_mixed_audit$' --output-on-failure
```

The targeted sanitizer command uses the existing compiler/GTest/OCCT libraries:

```sh
c++ -std=c++17 -O1 -g -fsanitize=undefined -fno-sanitize-recover=all \
  -I build-occt-implicit/include/opencascade \
  OCCT/src/ModelingAlgorithms/TKMesh/BRepMesh/BRepMesh_ImplicitBoundaryAudit.cxx \
  OCCT/src/ModelingAlgorithms/TKMesh/GTests/BRepMesh_ImplicitBoundary_Test.cxx \
  -L build-occt-implicit/lin64/gcc/lib \
  -Wl,-rpath,"$PWD/build-occt-implicit/lin64/gcc/lib" \
  -lTKMesh -lTKPrim -lTKTopAlgo -lTKBRep -lTKGeomBase -lTKG3d -lTKMath \
  -lTKernel -lgtest_main -lgtest -pthread -o /tmp/implicit-contact-ubsan
/tmp/implicit-contact-ubsan --gtest_filter='BRepMesh_ImplicitBoundary.ExactTriangle*'
```

`mixed_audit_smoke.py` tests both operations with closed/open contour fixtures,
clearance reports, source immutability, limited audits, wrong handles and
retained lifetime. All four fixture audits return clear. This is targeted
evidence, not arbitrary-field/operand qualification. The ordinary surface
preview still does NOT automatically run the audit or claim embeddedness.

Next: coverage of the authoritative zero set, clipped-boundary consistency and
implicit-aware native-solid validation. These cannot be replaced by this mesh
audit. Curved operands, history/snapping and broader FreeCAD consumers remain
within the active through-Stage-4 goal.

A newer [clipped-boundary proximity check](native-clipped-coverage.md) now
partitions the full box/faces and checks reverse clipped witnesses. It addresses
two-sided numerical proximity, not topology equivalence or native-solid validity.
