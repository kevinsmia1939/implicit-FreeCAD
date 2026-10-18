# Native boundary edge and vertex selection

Supported mixed gyroid/box objects now expose actual stored edges and vertices
independently of surface triangles. This advances Stage 4 selection, not general
mixed CAD integration or the default primitive's trimming support.

## GUI

Launch `build-freecad-occt8/bin/FreeCAD`, activate **Implicit Modelling**, then
choose **Implicit Modelling → Native boundaries (experimental) → Native boundary
selection demo (experimental)**. This creates the supported open-contour fixture
with its optional surface preview. **Add gyroid** still creates the simpler
10 mm field primitive without native trim topology; it is not silently replaced.

Mixed-boundary View properties **ShowBoundaryEdges** and **ShowVertices** default
to true. Seams are orange, conventional box edges dark, and actual vertices point
markers. Hidden elements have no detail/pick targets. These switches persist
without remeshing; geometry failures remove buffers/nodes and clear selections.
Smooth gyroids have no intrinsic sharp ridges: these are actual trim/cap edges,
not triangle edges. Seams remain tolerance-controlled degree-one B-spline curves.
The surface preview uses analytical field-gradient normals for smooth shading;
cap normals stay separate, preserving the cap/field crease. This changes lighting
only, not mesh positions, silhouette error or solid validity.

| Name | Stored geometry |
|---|---|
| NativeFace1 | Bounded field face, pickable with the optional surface preview |
| CapN | Conventional planar cap |
| SeamN | Legacy subset of implicit trim edges |
| EdgeN | Every unique boundary edge, including conventional box edges |
| VertexN | Actual topological edge endpoints, including closed-edge anchors |

Edges follow compound traversal order. Vertices follow that edge order, adding
each forward edge's first/last endpoints by OCCT identity, never coordinate
welding. Intermediate polyline poles are not extra topological vertices. Closed
edges can reference the same endpoint twice. Seams/edges share Coin nodes:
seam hits retain legacy SeamN names, EdgeN aliases address the same native edge,
and nothing is drawn twice. Other edges and point markers pick as EdgeN/VertexN.
The existing FreeCAD picker prioritizes nearby point/line/face details.

## Native consumer contract

```python
import Implicit
geometry = Implicit.nativeGeometry(obj)
edge_points, segments = geometry.getLinesFromSubElement("Edge", 1)
vertex_points, no_segments = geometry.getLinesFromSubElement("Vertex", 1)
points, no_normals = obj.getSubObject("Vertex1").getPoints(.1)
```

A selected vertex exposes one stored point, no fabricated line or unique surface
normal, and zero-size bounds. Owned selected geometry is pre-placed exactly once
through supported rigid feature/container/App::Link paths and survives document
close. Surface/edge point sampling, nonrigid mutation and unsupported Part
assignment still reject; this is not integrated Draft snapping or a Part shape.

`ImplicitOCCT.inspect_mixed` adds local-coordinate `edge_points`, `vertex_points`,
zero-based `edge_vertices` pairs and `seam_edges` aliases, all derived from actual
topology. The recipe's public inspection copies their containers, so edits cannot
corrupt its display cache or authoritative property. Existing mapped-name rules
apply: unchanged snapshots survive save/copy/undo; changed geometry invalidates
stale references. This is snapshot identity, not edit-stable topology history.

## Persistence precision and verification

The existing conventional B-Rep payload writes vertex coordinates with **15
significant decimal digits**. Vertex positions can therefore change by a few
binary64 units on reopening, even when curve poles restore exactly. We retain
actual stored vertices rather than snapping them to curve poles. Tests require
exact edge/endpoint identities and machine-scale coordinate error reserves (up
to 128 epsilon times coordinate magnitude), not the much looser 0.1 mm target.
No bitwise-exact vertex round-trip claim is made. The solid factory independently
rechecks restored geometry against its requested tolerance.

CLI tests cover both operations/open and closed contours, conventional cap
endpoints, actual versus display-point counts, codec restoration, mapped links,
undo/redo/copy, placed extraction, lifetime and Part rejection. GUI tests cover
real camera/pixel picks, aliases/detail paths, native selected SubObjects,
instance vertex picks, visibility restoration, failure clearing, cache reuse
and the focused workbench command. See PROGRESS.md for executed results.

General curved operands, non-axis-separable restrictions, solid-property
persistence, integrated snapping/history and broader consumers remain required
for the full through-Stage-4 goal.
