# Background source preparation in task panels

Apply and OK in the mixed-boundary and native-solid editors now stage source
tracing and solid construction on the shared native worker. This works with
either value of the persisted BackgroundRecompute flags; task previews do not
change those flags. Direct Python `task.apply()` keeps its synchronous behavior,
using an explicit execute override rather than temporarily changing properties.

ImplicitTaskBatch.PreparedSolidBatch owns the two stages inside the editor's
existing transaction. It preflights the group, cancels superseded ordinary
background requests and rejects another task's pending tokens. Changed sources
enter SourceBatch; native solid targets hold waiting tokens, unavailable geometry
and frozen build settings. Ordinary document recompute cannot start a synchronous
fallback while those tokens are held.

Source jobs capture primitive fields and the original serialized conventional
B-Rep. They do not call Part.Shape.copy(): copying a translated box can flatten
its location and change a coordinate's last bit. The record submission helper
preserves the original snapshot, keeping the existing exact metadata checks.
Serialization and planar-cap display remain caller-thread work.

SourceBatch checks document/transaction, owner/proxy, operands and live recipe
before taking results and around publication. It retains completed private
results until every source succeeds, then publishes the source group. Native
waiting tokens transfer to SolidBatch only after source preparation succeeds.
Solid builds retain their existing all-peer publication guards. Starts retry when
the shared queue is full. There is still one worker and one extra outstanding slot.

Source geometry can be ready while solid validation is still pending; it does
not claim solid validity. A source-group failure clears the entire source group
and waiting native targets. A later solid failure leaves valid source geometry
and unavailable solid output. Cancel rolls the owned edit transaction back.
The source Geometry/fingerprint pair is preserved during Document.Transacting,
so rollback does not erase the fingerprint and numerically retrace unchanged
topology. Normal metadata validation still rejects inconsistent restored data.

Control changes cancel the preview; Apply starts the new settings. OK waits for
both stages before committing. Lost transactions, deletion and document closure
discard work without writing into unrelated edits. Taking over an ordinary
background request cancels that private job; cancelling the task can resume the
prior ordinary recompute through its normal persisted policy.

Task qualification covers same-document primitive gyroid/box source families.
Ordinary property recompute remains opt-in. Default native-solid command exposure,
broader inputs/placements, curved B-Rep operands, snapping/history and consumer
integration remain Stage 2–4 work. The 0.1 mm policy is unchanged; generated meshes
remain transient and independent of native-solid validation.

Final task/shared-batch/runtime checks pass 4/4 (120.40 s). The expanded
boundary/editor/property/runtime run passes 6/6 (198.18 s), including the strict
translated-box editor and ordinary source pipeline. Earlier affected headless
and editor checks are recorded in PROGRESS.md. This increment changes Python
components in the native module; it requires no new kernel build or dependencies.
