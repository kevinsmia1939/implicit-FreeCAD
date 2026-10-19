# Live native solids in FreeCAD

The OCCT-8 fork now connects the [native solid property](native-solid-property.md)
to a live source feature and a component-aware viewport. This is a restricted
native gyroid/axis-aligned-box workflow, not generic Part BOP or completion of
Stages 2–4. Mathematical field definitions and actual native result topology
remain authoritative; all display triangles are disposable.

## Try it

Launch the fork's `build-freecad-occt8/bin/FreeCAD`, choose **Implicit Modelling**,
then **Native solids (experimental) → Live native solid demo**. Alternatively,
select an existing supported native mixed-boundary object and choose **Build
live native solid from boundary**. The default Add Gyroid icon is unchanged:
its 10 mm field preview is not automatically promoted to a trimmed solid.

Python console:

```python
import ImplicitSolid
solid = ImplicitSolid.demo()
# Or: solid = ImplicitSolid.create(boundary, tolerance=.1, topology_resolution=32)
```

The new object is `Implicit::NativeSolidFeaturePython`, with its own typed
`Geometry` property, a live `Source`, 0.1 mm default `GeometricTolerance`,
topology resolution/depth and independent work budgets. Edit these through the
property editor and recompute. SolidCount, ShellCount, NativeFaceCount,
SolidValidated and BackendStatus are read-only diagnostics. Source operation or
geometry changes build a new checked result. Unresolved builds clear geometry,
counts and the entire view rather than leaving a valid-looking old solid.

## Coordinate and dependency contract

Geometry is stored in the source boundary's local coordinates. `Source` is an
`App::PropertyLinkGlobal`: putting the result into App::Part must not adopt and
move its upstream Boolean operands. Ordinary local-scope links did exactly that
in the first container test and invalidated the source's identity-input contract.

By default FollowSourcePlacement maps the source's global pose into the output
container's coordinates on recompute. Disable it to position the output
independently. Placement changes do not alter the authoritative local geometry
or rebuild its display mesh. Rigid output App::Link instances use the native
owner's standard subobject extraction; no Part shape or twice-applied pose is
introduced. Link arrays, nonrigid transforms, source App::Link dispatch, general
placed/curved Boolean operands and automatic external-file loading are not
qualified by this increment. The source mixed-boundary algorithm retains its
existing operand restrictions; grouping/moving the source's own operands is a
separate remaining gate.

## Persistence and validation

The property stores actual native solids in `.nativesolid` FCStd payloads. The
proxy stores source/settings fingerprints only to identify unchanged inputs,
never as substitute geometry. Unchanged recompute compares the embedded source
record/tolerance with the live input and reuses the actual result. Save/reopen
does not numerically rebuild; loaded qualification flags remain false even
after an unchanged recompute. The visible SolidValidated diagnostic reflects
the owned snapshot, not a saved Boolean or the presence of a mesh.

For an explicit **new** checked build:

```python
solid.Proxy.rebuild(solid)
solid.Document.recompute()
```

This does not certify or mutate a previously retained loaded snapshot. Corrupt
payloads or a readable result inconsistent with unchanged source/settings require
explicit repair; there is no silent numerical-recipe fallback. Runtime undo/redo
retains immutable snapshot ownership. Source errors skipped by Python recompute
also clear the solid via the C++ owner invalidation hook.

## Viewport and selection

The renderer builds a genuine Solid/Shell/Face scene hierarchy from the actual
result's material/shell/face relations. NativeFaceN refers to each result field
component, not the source's aggregate field face. CapN uses the result's explicit
source-cap index. FaceN, ShellN and SolidN detail paths select their whole semantic
nodes, not individual display triangles. Actual EdgeN/VertexN buffers come from
result ComplexGeoData; SeamN aliases actual edges by OCCT identity, never by
position or the source compound's edge order. Mapped references reject stale
snapshot identities. Surviving-element Boolean history is still unfinished.

An optional constrained surface candidate comes from the owned mathematical
source. Every triangle must fit exactly one saved actual component domain,
allowing only a machine-scale coordinate reserve; crossings, overlapping-domain
ambiguity or missing component buffers refuse the field preview. This is
display assignment, NOT field coverage or a numerical revalidation of saved
component cuts. Preview failures keep authoritative geometry and show native
caps/edges/vertices with an explicit status. They do not downgrade/upgrade the
solid's independent validation flag. Separate cap normals retain sharp trims;
analytical field normals provide smooth shading. Each face compacts existing
vertex indices without coordinate welding or changing the mesh.

View properties control field surface, boundary edges and vertex markers,
surface tolerance and meshing budget. Camera changes do not regenerate anything;
edge/vertex visibility changes reuse the candidate. Native geometry failure
clears buffers, aliases, selections and cached candidates. FCStd stores display
settings, never mesh buffers. Meshing/building remains synchronous.

## Verification and remaining gates

CLI tests cover live operations, source/result consistency, budgets, explicit
repair, unverified restore with building forbidden, corruption, undo/redo,
independent/followed/container placement and skipped-execution upstream failures.
GUI tests cover actual component-face ray hits, camera-based edge/vertex picks,
semantic/mapped selection, rigid output links, cache/visibility/error handling,
both operations, save/reopen with building forbidden and a two-component result.
Viewport captures are inspected; Xvfb/software OpenGL is not a physical GPU or
interactive-performance qualification.

```sh
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests \
  -R 'implicit_occt8_(live_solid|gui_live_solid)$' --output-on-failure
```

Remaining requirements include native default-gyroid trim degeneracies, curved
operands/general field restrictions, integrated snapping, surviving-element
history, task-panel/source-link editing, broader cross-workbench dispatch and
full end-to-end placement/consumer qualification. This restricted workflow does
not complete Stage 4. See stage-4-acceptance.md and PROGRESS.md.

The later [corner-aware native solid extension](native-corner-solids.md) now
handles the unchanged default cube's trim corners, with [opt-in live display](native-corner-mesh.md)
and a separate [clipped-proximity checker](native-corner-coverage.md). Fresh
GUI-buffer and broader parameter/task qualification still precede switching
the default primitive; the other integration requirements above remain open.
