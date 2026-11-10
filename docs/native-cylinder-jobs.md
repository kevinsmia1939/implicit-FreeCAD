# Native cylinder background jobs

`Implicit.submitCylinderJob(featureOrRecord, resolution=16)` freezes a dedicated
cylinder geometry property's explicit graph record, or accepts such a record
string directly. `Implicit.takeCylinderJob(job)` consumes a ready result once,
returning `geometry_handle`, `preview`, `input_record` and `resolution`.
Poll/cancel with `ImplicitOCCT.native_job_status` and `cancel_native_job`.
Wrong result-kind consumers refuse without consuming the job.

The worker parses a private graph, freshly qualifies geometry and material
volume, constructs the qualified restored solid owner, and generates the
trim-conforming preview with distance/contact results. It receives only owned
strings and scalar settings. Python, document objects and Coin nodes stay on the
calling thread. Neither job completion nor taking a result changes the source
feature or promotes its raw restoration state. Adoption is a separate explicit
document operation. A malformed graph fails in the worker; invalid record size
or resolution refuses before submission.

`ImplicitNativeWork` is a shared native library used by both `Implicit` and
`_ImplicitOCCT`. It owns the existing single worker, with at most one additional
outstanding request. Box boundary, solid and surface jobs and cylinder jobs
therefore share capacity. Existing box scheduling semantics are preserved.
Model builds cancel new cylinder presentation jobs to avoid being held behind
preview work. Requests use the cooperative OCCT scope; cancellation discards
private results. Dropping a job handle cancels it, including a queued payload.
Normal shutdown cancels outstanding work and joins the worker.

The tests check Python responsiveness, explicit adoption and undo/redo, frozen
inputs across edits, document deletion, both material signs, native qualification,
matching uploaded preview buffers, single-consumption/kind guards, shared queue
capacity, model-build priority, queued/running/ready cancellation, malformed
inputs and shutdown with outstanding work. Existing worker families must also
be tested after changes to the shared scheduler/library.

The cylinder view still uses the synchronous API. Next implement GUI polling,
source/settings/generation guards, coalescing and cancellation on hide, deletion
and closure. A GUI controller must compare the returned record/settings with
its live source before adopting geometry or publishing buffers. It must clear
stale evidence on failure/cancellation and keep all Coin/document operations on
the GUI thread. This backend does not complete Stage 4 or general mixed Boolean
and consumer support.

Build the existing `Implicit` and `ImplicitScripts` targets with two jobs. The
native shared library is a dependency and is installed alongside the module.
Migration tests `implicit_occt8_cylinder_jobs` and
`implicit_occt8_cylinder_job_shutdown` run in the matched fork's FreeCADCmd.
