# Experimental background viewport meshing

Native `Implicit::Feature` view providers offer an opt-in `BackgroundMeshing`
property. The default remains synchronous for compatibility and reproducible
reference tests. No extra system packages are required: the implementation uses
the Qt Concurrent component already configured by FreeCAD.

## Try it

Select a native implicit object, open **View > Meshing**, and enable
`BackgroundMeshing`. Change a geometry parameter and recompute, or increase
`Resolution`. Alternatively, use the fork's Python console:

```python
obj = App.ActiveDocument.getObject("MixedIntersection")
obj.ViewObject.BackgroundMeshing = True
obj.ViewObject.Resolution = 32
App.ActiveDocument.recompute()
```

Use `CancelMeshing = True` to cancel current background work. The checkbox resets
automatically. Changing a parameter/resolution again requests a new mesh; camera
motion and ordinary primitive Placement changes do not request extraction.
The mathematical field remains usable when display extraction is cancelled.

View diagnostics:

| Property | Meaning |
|---|---|
| `Meshing` | Background work remains outstanding, including a cancelled job that has not finished |
| `MeshStatus` | Ready, Queued, Meshing, Hidden, Cancelled, Waiting for recompute, or an error message |
| `MeshRequestId` | Latest request/invalidation generation |
| `DisplayedRequestId` | Uploaded generation, or zero if no current mesh is displayed |
| `MeshJobCount` | Background jobs dispatched this session; rapid edits can coalesce into fewer jobs |
| `CacheBuildCount` | Coin buffer uploads, not worker invocations |

An empty successful extraction can also be `Ready` with zero triangles. That is
not proof that no sub-grid surface exists. Diagnostics and meshes are transient;
the enabled mode and ordinary display settings persist in FCStd.

## Ownership and scheduling contract

1. The GUI thread captures an immutable native `Shape`. Document dependencies,
   B-Rep serialization/validation, and field compilation remain on that thread.
2. One module-local worker pool runs at most one extraction at a time. Each view
   provider owns at most one dispatched job and one replaceable pending request.
   New requests cancel the dispatched job cooperatively and replace the pending
   snapshot rather than appending a job for every edit.
3. Workers touch only copied immutable fields, local extraction buffers and an
   atomic cancellation flag. They do not access FreeCAD documents, Python, Qt
   widgets or Coin nodes. B-Rep field evaluators retain their existing mutex.
4. Finished results are delivered on the GUI thread. Only the current generation
   may populate the native display cache and retained Coin buffers. The feature
   also checks snapshot identity/domain/convention before accepting a mesh.
   Completed results are consumed even when obsolete, so the future watcher
   does not retain a discarded large mesh after cancellation or an input error.
5. Definition edits clear obsolete display data and wait for recompute. Failed
   inputs cancel pending work and cannot resurrect an old mesh. Hidden views
   stop background requests; showing them reuses a valid cache or requests work.
6. Deletion, document close and restoration cancel outstanding jobs. The queued
   callback context is destroyed with the provider; workers own their snapshots
   and never dereference a deleted provider. Closing a document does not wait on
   its job. Worker-pool shutdown may wait for cooperative cancellation to finish.

Both uniform extraction and the existing octree-pruned path are supported.
Adaptive view resolution still rounds up to a power of two in `[2,64]`, and the
one-million vertex/triangle limits still apply. Synchronous fallback shares the
completed native mesh cache; ordinary camera motion reuses retained nodes.

## Limits

This moves extraction off the GUI thread, not the entire recompute workflow.
Large Part recomputes, B-Rep snapshots/validation, graph compilation and Coin
buffer upload can still block it. Cancellation cannot interrupt the middle of a
single OCCT distance query. There is no global queued-snapshot/cache memory
budget, thread-priority policy, progress percentage or GPU backend yet.

Bounds on concurrent jobs do not constitute a complete memory budget: multiple
objects can each retain a pending snapshot and display cache. Coalescing avoids
unbounded queues per edit, but many large objects still consume memory. The
geometric fidelity/topology limitations of sampled extraction are unchanged;
background meshing does not create manufacturing solids or STEP geometry.

## Regression coverage

`FreeCAD/src/Mod/Implicit/Tests/async_gui_smoke.py` checks main-loop heartbeat
during extraction, rapid-edit coalescing, final sphere coordinates and triangle
counts, effective-resolution cache reuse, synchronous fallback, tolerance/error
recovery, cancellation, hidden views, retained nodes, stale-result suppression,
deletion/close while work is outstanding, and FCStd restore. With live Part tests
enabled, it also checks B-Rep source edits and source deletion.

Run after building `ImplicitGui` (plus Part/PartGui/PartScripts for Part coverage):

```sh
python3 tools/with_xvfb.py ctest --test-dir build -R implicit_async_gui --output-on-failure
```
