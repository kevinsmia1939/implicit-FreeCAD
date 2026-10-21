# Background native-solid jobs

The OCCT-8 FreeCAD module now provides a bounded native executor for restricted
solid construction and its initial transient display candidate. This is the
backend for the asynchronous creation dialog. Existing solid editing/recompute
and checked-display paths remain synchronous.

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
The existing synchronous API behaves unchanged without a scope.

Cancellation is cooperative, not a hard latency guarantee. Opaque conventional
OCCT operations, source serialization/deserialization and some short assembly
steps can run until the next checkpoint. Geometry and tolerance policies are
unchanged; cancellation never publishes a partly validated result.

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
memory budgets remain open. Existing edit/recompute and checked-display
scheduling remain subsequent work. The default Add Gyroid is unchanged.

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
