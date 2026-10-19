# Corner-aware transient mesh and opt-in live display

Regularized native boundaries now support shared cap constraints and a transient
trim-conforming field mesh candidate. Authoritative field definitions, native
faces/edges/vertices, saved topology and the default workbench command are
unchanged. Display triangles never become native CAD faces.

## Sampling and membership

Prepare meshes only private conventional caps. Canonical samples still come
from native vertex identity plus edge-sample ordinal; adjacent caps and field
trim wires share those nodes. Every native polyline knot remains a constraint,
with opposite cap/wire incidence. The six split corners are shared trim nodes;
the retained uniform corner is cap-only and the excluded uniform corner is absent.
Source caches and original/restored seam provenance are unchanged.

The regularized field extractor uses an odd grid (requested power-of-two minus
one, uniformly per axis), fixes only physical corners with fresh exact lattice
identities, and shares zero roots by grid-node identity. There is no field phase,
isovalue, domain or tolerance perturbation and no distance welding. Grid-corner
roots on the box boundary remain outside the interior extraction; the constrained
collar attaches that interior to the stored native wires.

Each final field triangle must have an AABB disjoint from both canonical uniform-
corner exclusion neighborhoods, with reserves. Cap-only nodes are not mistakenly
removed from caps. Existing triangle-wide untrimmed-field distance bounds,
opposite edge pairing, connected cyclic vertex links and work limits remain.
Reports expose corner_policy and fixed_corner_count; accepted cells is the actual
odd grid size. NativeSurfaceMeshed/MeshClosed do NOT imply coverage or solid
validity: SurfaceCoverageVerified and SolidValidated remain false.

## Exact contact audit broad phase

The initial default audit exhausted its existing work budget. The old X-only
sweep made triangles on an X-constant plane compete with every other triangle
on that plane before checking Y/Z. The audit now uses a balanced three-dimensional
triangle-AABB hierarchy with unique unordered leaf pairs. Min/max envelopes use
the actual binary64 coordinates; no epsilon contact pruning is introduced.

Exact integer triangle predicates, shared-node/common-simplex rules, pair/broad
limits and Limit-as-unresolved behavior are unchanged. BroadTests now counts
node-box and leaf-pair box comparisons. No pairs are skipped merely because they
share vertices. A 4,096-triangle coplanar sparse regression stays within a much
smaller broad budget, then detects an injected unrelated duplicate. Both default
corner candidates audit Clear under the original default limits.

## Live FreeCAD workflow

`ImplicitMixed.create(field, box, operation="Common", corner_policy="Regularized")`
enables the new path explicitly. CornerPolicy is an editable Reject/Regularized
property; legacy documents migrate to Reject. Reject fingerprints retain their
old byte contract so unchanged saved strict topology is not retraced. Regularized
fingerprints include policy, and stored geometry/input checks verify agreement.

The existing `ImplicitSolid.create(source)` view renders transient component-
assigned triangles with field normals and separate cap normals, and retains real
native edge/vertex selection. It still labels display coverage as NOT validated.
The default Add Gyroid command remains the old field-display feature. This is an
opt-in live-workflow extension, not completed default workbench integration.

## Trying the opt-in workflow

Start the fork's `build-freecad-occt8/bin/FreeCAD`, then use its Python console:

```python
import FreeCAD as App, ImplicitMixed, ImplicitSolid
import FreeCADGui as Gui
doc = App.newDocument("NativeCornerGyroid")
field = doc.addObject("Implicit::Feature", "Gyroid")
field.Function, field.Period, field.IsoValue = "Gyroid", 10, 0
field.DomainMin, field.DomainMax = (0, 0, 0), (10, 10, 10)
box = doc.addObject("Part::Box", "Box")
box.Length = box.Width = box.Height = 10
doc.recompute()
source = ImplicitMixed.create(field, box, corner_policy="Regularized")
solid = ImplicitSolid.create(source)
field.ViewObject.Visibility = box.ViewObject.Visibility = source.ViewObject.Visibility = False
Gui.activeDocument().activeView().viewAxonometric()
Gui.activeDocument().activeView().fitAll()
```

The field/box/source remain editable linked tree objects; the result is a native
solid owner, not an ordinary Part shape. This is deliberately opt-in. Display
coverage is not verified, and unsupported parameter/resolution cases must fail
safely rather than silently producing stale surface data.

## Verification and remaining gates

Kernel tests cover shared corner samples, source-cache/record immutability,
both operations, closed candidates, field-distance bounds, domain/contact
exclusion, and exact contact auditing. CLI tests cover both operations, restored
provenance and the diagnostic metadata. The GUI test creates the unchanged
10-mm gyroid/box, renders both operations, performs actual face/vertex ray picks
and semantic subelement selection, tests undo/redo, and restores/re-renders FCStd
without rebuilding the saved solid. A viewport capture is written to
`/tmp/implicit-corner-native-solid.png`.

Corner-aware clipped coverage remains explicitly guarded. The mesh candidate is
not proof of whole-field coverage, isotopy or display fidelity for every field/
parameter change. Next extend and verify that coverage path and broader live/task
editing before changing the default command. Curved operands, snapping/history
and all other unfinished Stage 2–4 gates remain part of the active goal.
