# Exact-node gyroid plane sections

OCCT's plane-rectangle tracer now has an explicit lattice-node mode. It resolves
all six faces of the unchanged default 10 mm, iso-0 gyroid cube, preserving both
one-dimensional section curves and isolated point contacts. This is native
kernel **section computation**, not a native trim B-Rep or completed default
solid workflow. No field, level, phase or rectangle is perturbed.

## What changed

`Settings.AllowExactLatticeNodes` defaults to false. In the new mode:

- A node becomes an exact zero only through the checked quarter-period identity
  in `Geom_ImplicitGyroid::LatticeJet`. Near-zero residuals do not qualify.
- The coordinate-plane map must be exact at that node. A TwoSum residual rejects
  a rounded sum such as `0.1 + 9.9` merely landing on 10.
- All incident cell edges share one grid-node identity. Roots are not welded by
  distance, and a cell counts a corner shared by two of its edges only once.
- A zero endpoint needs a monotonicity bound over the entire edge. Opposite-sign
  harmonic uniqueness is not substituted for an endpoint/tangency proof.
- Two uniformly signed inward cell derivatives prove that only a zero corner,
  not a curve, meets that cell quadrant. Other incident cells can still contain
  the curve. Interior nodes must join with degree two; boundary ends and isolated
  rectangle-corner contacts have distinct, checked cases.
- Bounded odd alternative grids avoid repeatedly sampling unrecognized
  eighth-period zeros. They stay within the same per-axis budget and use the
  same acceptance checks; unresolved cells never become empty by fiat.

The exact-node mode currently requires an effective identity field transform
and coordinate-plane frames. Nonempty exact-identity datum chains work after
primitive I/O. Nonzero transforms and oblique frames reject in this mode; the
existing numeric mode retains its placed/oblique support. Unknown zeros,
grid-aligned tangent/zero edges and exhausted precision/budgets still refuse.

## Curves versus contacts

The result contains `Curves` and a separate `PointContacts` array. Consumers
must not interpret empty curves as an empty intersection without checking points.
A small first-quadrant rectangle at the default origin resolves to one isolated
point and no curves. No zero-length fake curve is manufactured for it.

Each full default cube face has three open curves, two exact corner curve ends
and two isolated planar contacts. Across all six faces, the six split cube
corners each have two curve-end uses and one contact use; the two uniform-cone
corners each have three contact uses and no curve ends. These are section data,
**not yet shared TopoDS_Vertex identities across different faces**. A contact
isolated on one face can belong to a three-dimensional material boundary through
other faces. The [corner material model](native-corner-germs.md) remains necessary
for the eventual cap/solid policy.

`GeometricBound` bounds accepted curve-cell diameters plus coordinate reserve,
in model units; the default target is 0.1 mm. Root brackets are at most tolerance/16.
Exact isolated contacts add no chord error. Residuals have field units and are
diagnostic, not geometric distances. Bounds retain the existing engineering
floating-point reserves, not directed interval arithmetic or solid validation.
Partial results have `Resolved=false` and must not be published as complete trims.

Existing B-Rep builders and the worker protocol keep the default strict mode.
They cannot silently discard the new contacts, reinterpret material or claim a
default-cube native solid merely because its six section queries resolved.

## FreeCAD OCCT-8 API

```python
import ImplicitOCCT as native
face = native.gyroid(10, 0, (0,0,0), (10,10,10))
section = native.trace_gyroid_plane(
    face, (0,0,0), (0,0,1), (1,0,0), (0,10), (0,10),
    distance_tolerance=.1, maximum_cells=512, allow_exact_lattice_nodes=True)
assert section["resolved"]
assert len(section["curves"]) == 3
assert len(section["point_contacts"]) == 2
```

Arguments define the world origin, plane normal, X direction and U/V intervals.
The full rectangle must lie inside the native field's computational domain.
The Python facade accepts keyword settings; the lattice policy requires an
actual bool, not a truthy string. Results contain curve point lists/closed flags,
contacts, resolution, graph/ambiguity diagnostics, exact-node counts, geometric
bound, raw residuals and root-bracket size. Topology/solid validation flags remain
false. Points are not FreeCAD native subelements or Part shapes.

Queries use independently owned mathematical snapshots and survive document
closure or primitive round-trip. Results are transient; no FCStd property or
persistence schema changes, saved proof flags or new GUI commands are introduced.
The default Add Gyroid and the supported native-solid demo remain unchanged.

## Build and verify

Settings/Result gained transient fields. Rebuild TKMesh dependencies, the native
worker and the OCCT-8 FreeCAD consumers together; refreshing Python alone is not
safe across this ABI change. Restart the fork after rebuilding. Reuse existing
build directories/dependencies and do not run runtime/library audits while
replacing their libraries.

```sh
cmake --build build-occt-implicit --target TKMesh --parallel 2
cmake --build build-occt-implicit/native-face-tests \
  --target implicit_occt_native_face_tests implicit_occt_native_worker --parallel 2
cmake --build build-freecad-occt8 \
  --target Implicit ImplicitGui _ImplicitOCCT ImplicitScripts --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests \
  -R '^implicit_occt8_lattice_section$' --output-on-failure
```

Tests cover all six faces against independent complete analytic slice families,
curve/contact incidence, shared interior grid nodes, point-only sections,
reversed coordinate frames, unknown/rounded-affine zeros, budget/type/placement
refusal, source immutability, ownership and restoration. Executed outcomes are in
PROGRESS.md. Next consume this data in canonical box-edge/corner root identity
and cap/implicit cycles, then extend the homotopy/reference, native attachment
and persistence policies before claiming a default native solid. Curved/general
operands and all unfinished Stage 2–4 gates remain required.
