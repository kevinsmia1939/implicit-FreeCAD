# Restricted native face projection and Draft Near snapping

`BRep_ImplicitTool::Project` now provides a bounded local normal-foot query on
native implicit faces. It never constructs a UV surface. A numerical KKT solve
checks field residual in length units, stationarity and positive numerical tangent
second variation. A line search limits movement from the seed; iterations and
distance are explicit budgets. Acceptance also needs a narrow sign bracket near
the returned point, with a geometric width and stationarity check after refinement.

The bracket must lie wholly in the face's explicit mathematical domain. Its
endpoints check the finite box and convex patch predicates; segment/box tests
also reject crossings through closed domain exclusions, even when both endpoints
are admissible. Nonfinite evaluation/settings throw. Critical points, singular
systems, exhausted precision/budgets and unresolved restrictions return a failed
result with a reason. Source geometry and validation flags remain unchanged.

The public result contains Converged, Point, First/Last bracket endpoints,
Residual, Stationarity, Distance, BracketWidth, Iterations and Reason. Check
Converged before using its point. The query uses ordinary floating reserves and
a numerical local-minimum test; it is not directed interval certification, proof
of global nearestness, root completeness or support for arbitrary added trim
wires. The established native mixed/solid faces use their explicit box/component
domains and exclusions. Generic trim topology and globally constrained surface
nearest queries remain separate work.

`Implicit.snapNative(..., "Near", cursor)` now consumes this query for picked
native mathematical faces. Calculations occur in the selected face's local frame;
the rigid parent/link path is applied once. It requests a tighter local tolerance
than the default 0.1 mm coordinate budget and limits motion to four times that
budget. It emits no candidate on failure. It does not return the display-triangle
hit as a surface snap, nor recertify topology, seam bounds or display quality.

Conventional planar caps inside native owners use a different, trim-aware path.
The plane foot is classified against the actual cap, including holes. If outside,
the closest stored trim-edge point is selected. Only these conventional cap faces
use the ordinary OCCT face classifier; native implicit faces never enter UV-based
classification or Part.getShape.

Draft's existing Near mode now consumes both kinds of candidate with the standard
passive marker and working-plane option. Endpoint/Midpoint/Center behavior on
faces is unchanged. This extends the [vertex/edge snapping integration](draft-native-snapping.md);
it does not implement cross-representation intersection snaps, general face
boundary handling, surviving-reference history, curved mixed operands or complete
Stage 4 integration.

## Build and verification

Reuse existing builds, two compile jobs and one link job. No new dependencies or
system-library changes are required. The new OCCT method/result add API without
changing existing type layouts or record formats. Rebuild matching headers and
the new direct consumer; never mix OCCT 8 headers with the legacy 7.9 library.

```sh
cmake --build build-occt-implicit --target TKBRep --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
cmake --build build-freecad-occt8 --target Implicit Draft --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests \
  -R '^implicit_occt8_(native_face_snap|gui_draft_face_snap|gui_draft_snap)$' --output-on-failure
```

The migration harness retains the previous Draft opt-in and build prerequisites.
Kernel tests compare analytical sphere/gyroid feet, placed/scaled faces, source
immutability, restrictions, thin excluded-box crossings and failure budgets.
FreeCAD tests check both mixed operations, cap distance against independent
conventional OCCT distance queries, holes, rigid paths and retained snapshots.
Actual Draft viewport tests select triangle hits with nonzero field residual and
require a distinct returned point on the mathematical field, the correct marker,
bounded-cap snaps and no Part conversion. The previous Draft Line workflow is
also rerun. Exact outcomes are recorded in HANDOFF and PROGRESS.
