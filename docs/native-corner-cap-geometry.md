# Corner-aware native cap geometry

`CheckCapGeometry` supports regularized corner boundaries. It retains the existing
whole-domain directed distance checks, lifted p-curve/3-D edge/vertex deviation
checks, spatial index, finite-domain projections and failure budgets. It neither
trusts stored seam proof flags nor promotes topology/native-solid validity.

## Mathematical cap membership

On each physical box side the cap is the closure of its strictly retained
two-dimensional area. An isolated planar zero with two unretained inward
directions is not such a cap point. Split corners can be isolated on one side
yet belong to other caps and the implicit face. The retained uniform 3-D corner
is cap-only; the excluded uniform corner is not a material boundary point.
Discarding an isolated planar contact does not remove the corresponding point
from another boundary face that actually owns it.

For a checked exact lattice corner, both signed free-coordinate derivatives
must be strictly unretained throughout the convex hull of corner and candidate
cell before a mathematical-to-cap cell can be excluded. The global Hessian
bound, maximum corner-to-cell radius and floating-point reserves bound their
variation. This is a whole-cell monotonicity argument, not same-sign-corner
sampling. It is not limited to the smaller stored corner neighborhood; the new
cell's full hull receives its own derivative bound. Native candidate cells are
NEVER discarded by this rule: wrong material near an excluded zero must fail.

Native-to-mathematical witnesses use strict retained projected points, or exact
corners with a proved strictly retained inward ray on that same physical side.
Continuity gives retained side area approaching such a corner. A corner isolated
from retained side area cannot be a witness, even when its raw field value is
exactly zero. All witness bounds use Euclidean distances plus reserves, never
field residuals as millimetres. Bounds are numerical engineering bounds, not
directed interval certificates.

## Diagnostics and API

`ImplicitOCCT.check_mixed_cap_geometry(handle, tolerance=.1, maximum_depth=16,
maximum_cells=2000000, maximum_segment_tests=8000000)` validates the stored
source cap candidates. It reports cap_geometry_resolved, both directed bounds,
edge_vertex_deviation, boundary_bound, cells/segment work, corner_policy and
unresolved side/direction/bounds. The counters regularized_empty_cells and
corner_witnesses describe the proved exclusions and admitted corner-witness
candidates, not mesh triangles or native validity flags. Default caps may use
strict interior witnesses without using any corner witness.

The C++ entry point still accepts separate candidate caps for independent
missing/extra/displaced geometry tests. A tiny redundant patch within tolerance
of a retained corner can correctly pass geometric proximity while failing native
topology requirements. A comparable patch near the excluded corner must fail.
Thus cap_geometry_resolved is deliberately NOT topology_validated or
solid_validated; both flags remain false on this diagnostic.

## Scope, persistence and budgets

The field, outer domain, native trim topology, source/restored provenance and
default workbench command are unchanged. Results are transient diagnostics, not
new persistence metadata. The existing immutable source property remains owned
after document close. BuildNativeSolids still explicitly rejects the corner
policy pending integration/qualification of native solid results and consumers.
Mesh/coverage and corner-native-solid serialization are not enabled here.

The physical target stays 0.1 mm. The shifted 20-mm fixture exhausts default
depth 16 and resolves with an explicit depth-18 request; the 10-mm default uses
depth 16. No production default or error threshold was increased. Future solid
integration needs independent geometry-depth settings: currently its shared
depth parameter also feeds graph/reference checks capped at 16. Do not enlarge
the physical tolerance just to fit a larger model into a smaller work budget.

## Verification and next work

Kernel tests cover both default operations, both directed bounds, shifted/scaled
cases at an absolute 0.1-mm target, retained/excluded corner patches with actual
plane/B-spline geometry, missing/opposite-material caps, restore/provenance,
budgets and source immutability. FreeCAD tests exercise the diagnostic, owned
source after close, restored geometry and explicit validity/refusal flags.

Next integrate the new cap qualification into corner-aware native solid
construction and persistence, preserving restricted face membership and material
grouping. Then extend the mesh/view path and expose qualified defaults in the
workbench. Curved operands, snapping/history, richer editing and cross-workbench
contracts remain part of the active full Stage 2–4 goal.
