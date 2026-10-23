# Background native-solid jobs

The OCCT-8 FreeCAD module now provides a bounded native executor for restricted
solid construction and its initial transient display candidate. This is the
backend for the asynchronous creation dialog and solid-editor previews.
Checked and unchecked view cache misses also use this worker. Ordinary GUI
property rebuilds can now opt into [background recompute](native-solid-recompute.md).
Its default remains synchronous; source preparation is still synchronous.

## Execution and ownership

`ImplicitOCCT.start_solid_job(boundary, *settings)` accepts the same setting
prefix as `build_mixed_solids`. Submission serializes the immutable source on
the caller thread. A single worker reads a private copy, constructs and freshly
validates actual native solids, then extracts the initial display candidate at
0.1 mm with the existing default cell limit. Neither document objects, Python
references nor Coin nodes enter the worker.

`solid_job_status(job)` reports state, phase, checkpoint count and failure text.
Phases are descriptive, not percentages, and may repeat through nested calls.
`cancel_solid_job(job)` requests cooperative cancellation; queued and ready
results are discarded immediately. Dropping the capsule also cancels. Normal
process shutdown cancels outstanding work and joins the worker.

At most two requests are outstanding (one executing and one queued); further
submissions reject. This is not a global memory budget: callers can retain
completed handles/results. Existing per-algorithm work limits still apply.

`take_solid_job(job)` consumes a ready result once. It returns the fresh native
solid report/handle, exact submitted `input_record` and `input_settings`, and
`surface`/`surface_error`. Display failure can coexist with a validated native
solid. Candidate display flags are not checked-display, coverage or solid proof.
No serialization/restoration is used to manufacture fresh validation flags.
Python result conversion still happens on the calling thread.

## Cancellation in OCCT

`BRepClass3d_ImplicitWork::Scope` installs a thread-local optional control.
Nested scopes restore the previous control; unrelated threads are unaffected.
Checkpoints in topology, cap/trim attachment, shell construction, geometry
validation and meshing throw `Standard_AbortiveTransaction` on cancellation.
Clipped coverage and exact contact auditing now expose their own phases and
checkpoints, including candidate preparation and recursive spatial traversal.
The existing synchronous API behaves unchanged without a scope.

Cancellation is cooperative, not a hard latency guarantee. Opaque conventional
OCCT operations, source serialization/deserialization and some short assembly
steps can run until the next checkpoint. Geometry and tolerance policies are
unchanged; cancellation never publishes a partly validated result.

## Display-only native jobs

`start_surface_job(boundary, tolerance=.1, cells=256, nodes=200000,
triangles=400000, checked=False, depth=14, spatial=2000000, patches=1000000,
pairs=2000000, broad=50000000)` extracts a private display candidate. With
checked=True it runs the same native binary32 displacement, clipped proximity
and exact contact calculation as `tessellate_mixed_checked`. It does not build
or return native solids.

Use `native_job_status` and `cancel_native_job` for either solid or display jobs.
`take_surface_job` consumes a ready display result once, returning `surface`,
`input_record` and a ten-element `input_settings` tuple in the argument order
above (excluding the boundary). Wrong-kind take calls fail without consuming.
Both kinds share the existing executor and queue capacity. Dropping handles or
normal process shutdown cancels outstanding work. A checked failure returns no
candidate; unchanged geometry/mesh proof flags are separate from display checks.

Python report conversion and analytic normal extraction occur when taking the
result on the caller thread. Source serialization also runs on that thread.
ImplicitSurfaceJobs binds these descriptors to live views with generation/source/
settings guards and publishes checked status only after successful scene upload.
It coalesces requests, cancels detached/deleted/hidden views and yields to pending
solid batches. Only one display job is outstanding; further descriptors wait.
The checked-mode setting is part of the job settings and view key, so switching
modes rejects the former generation. Cached/initial candidates remain reusable
and cancel superseded requests. Unchecked results never publish checked status.

## Publication prerequisite and next GUI work

The internal `ImplicitSolid._create(..., completed=result)` path performs an
atomic main-thread owner transaction without rebuilding a matching result.
It rejects touched/invalid source dependencies, changed records/settings,
unvalidated or mismatched native handles and unrelated active transactions.
Undo/redo retain the actual native topology. The public synchronous `create`
signature remains unchanged.

The creation task submits and polls via a 50-ms GUI timer. Cancel/form destruction
and document closure cancel outstanding jobs; no document transaction is held
while the worker runs. Changed settings, dirty inputs, changed source records,
missing source/document identity and active external transactions prevent
publication. Failures leave the task repairable. The view consumes the initial
candidate without synchronous remeshing, retaining the existing component
assignment and atomic scene-upload checks. These are still main-thread operations,
as are result conversion and source serialization; whole-workflow latency and
memory budgets remain open. Ordinary recompute/source preparation remain
subsequent work. The default Add Gyroid is unchanged.

## Shared-solid editor batches

GUI Apply/OK use `ImplicitSolidBatch.SolidBatch` inside the editor's owned
transaction. Boundary/source preparation is still synchronous. Every affected
native solid is invalidated before worker submission. Jobs run sequentially;
fresh results are retained until the whole group succeeds. Publication checks
source identity, serialized inputs, live recipe fingerprints, dirty dependency
leaves, settings and transaction ownership, then checks again after publication.
Pending owners suppress ordinary synchronous solid execution during recompute.
They expose no old geometry or validity flags while pending.

Cancel discards private work and the editor rolls back its transaction. Lost
transactions never authorize mutation inside an unrelated transaction. Deleted
owners and changed controls/inputs cancel the group; failures remain repairable.
The group retains completed results in memory until publication: only worker
concurrency and per-job work are bounded, not total document memory. Initial
default display candidates are reused; checked and unchecked regeneration use
background jobs. Normal console/task.apply() and
ordinary property edits retain synchronous execution by default outside a pending
batch; BackgroundRecompute explicitly enables the separate GUI coordinator.

## Tests

- Kernel: nested/thread-local control, cancellation during topology/geometry/
  meshing, source immutability and successful retry.
- Runtime: Python heartbeat, synchronous/async topology equivalence, bounded
  queue, queued/running/ready cancellation, dropped handles, geometry/meshing
  phase cancellation, failed-budget retry and independent source lifetime.
- Publication: no numerical rebuild, dirty dependencies with unchanged stored
  geometry, mismatched settings, external transactions, rollback and undo/redo.
- Shutdown: normal process exit with a running and a queued job, under timeout.
- Existing checked-display regression checks the factored mesh-report path.

See `PROGRESS.md` for executed results. These checks do not close the remaining
Stage 2–4 curved-operand, snapping/history, consumer or end-to-end GUI gates.
