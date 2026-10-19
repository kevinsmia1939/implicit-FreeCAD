# Corner-aware native solids and topology persistence

The regularized gyroid/box path now constructs actual field-backed TopoDS solids
after fresh whole-face/shell/topology attachment, directed cap geometry and stored
face/edge/vertex tolerance checks. Both default operations produce one solid,
one shell, 13 faces, 30 edges and 19 vertices, with boundary Euler -2. Neither
isolated zero contact belongs to the implicit face; the retained uniform corner
remains cap-only. Field/domain definitions and the 0.1-mm target are unchanged.

Validation is the existing restricted implicit-aware numerical policy, not
generic UV BRepCheck/BOP, directed interval certification, Part compatibility or
manufacturing exchange. No reference triangle is a CAD face. Cap coverage alone
cannot enable construction: the fresh whole-surface, component/material, trim,
oriented incidence, vertex-link and shell checks remain prerequisites.

## Independent geometry depth

The original C++ BuildNativeSolids entry point retains its signature and shared
depth semantics. An overload adds a final geometry depth (1..24); topology and
reference depth remain independently limited to 16. The FreeCAD binding appends
`maximum_geometry_depth=-1`, where -1 inherits the existing reference depth.

The shifted 20-mm fixture at the same physical 0.1-mm target exhausts geometry
depth 16 and passes with geometry depth 18, while its reference still uses depth
16 and the actual 31-cell grid. Tolerance is not enlarged to fit a smaller work
budget. Existing calls and defaults are unchanged; geometry/tolerance/resource
failure does not publish a constructed/validated solid handle.

## Solid codec schemas 1 and 2

Strict sources continue writing `OCCT_IMPLICIT_SOLIDS 1`. Regularized sources
write schema 2. Readers accept both and require outer version/source-policy
agreement. A schema-2 record carries the existing authoritative schema-2 source
boundary, native component boxes, source-wire assignments, face/shell/material
relations and requested tolerance. Actual caps/shared edges/wires come from the
stored source topology, not numerical Boolean recipe reconstruction.

Both source and component face restrictions are checked. Source restrictions
must equal fresh canonical corner exclusions; each actual component restriction
must equal the source restriction intersected with its component box. Readers
derive this same canonical restriction when instantiating component faces, so
serialization cannot quietly fill in an excluded contact. Field type/parameters,
orientation, domains, tolerance, shared topology and material grouping checks
remain. Arbitrary caller masks are not admitted as valid factory geometry.

Restored solids are constructed but unverified: validation, whole-face/topology,
geometry proof flags and shell/solid Closed flags are not restored. Saved work
counters/reference resolution are not proof metadata. Source seam bounds remain
unverified. Explicit rebuilding from `source_mixed_solids` performs fresh checks
without changing the restored object's flags. Re-saving restored topology is
supported; malformed/version-spoofed input is rejected atomically, and writer
preflight refuses lost component restrictions before touching its output stream.

## FreeCAD ownership and workflow scope

The existing `Implicit::NativeSolidFeaturePython` and native solid geometry
property now own this corner-aware topology. Standard subelement access, runtime
undo/redo, FCStd close/reopen and owned snapshots use the actual native solids.
FCStd restore preserves topology and drops proof flags; it does not rerun the
Boolean recipe. Embedded schema-2 bytes participate in existing conservative
snapshot-digest naming, not surviving-element history.

The default workbench command is unchanged. The later [mesh/live-view extension](native-corner-mesh.md)
supports opt-in transient rendering; clipped coverage remains guarded. This milestone alone does not claim a new
default smooth/crisp GUI result or generic Part consumer support. Source-linked
live-solid/task workflows need corner-specific GUI qualification before default
exposure. Curved operands, snapping/history, richer editing and all other
unfinished Stage 2–4 gates remain active.

## Verification and next milestone

Kernel tests cover both operations, actual solid shape/Closed state, 0.1-mm
bounds, stored tolerances, shared native counts, restricted face queries, schema-2
round trips, unverified restoration, atomic version-spoof refusal and restriction-
loss preflight, plus independent depth/tolerance/resource failure. FreeCAD tests
cover source/classification consistency, explicit recheck, subelements, native
solid property undo/redo, FCStd ownership and larger-model geometry depth.

Next extend corner-aware clipped coverage and broader live property/task/selection
qualification, then switch the default workbench command
only after those gates pass. This is not completion of the full Stage 4 goal.
