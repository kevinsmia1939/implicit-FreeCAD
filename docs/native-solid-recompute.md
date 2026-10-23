# Background property recompute

The native solid owner has a persisted `BackgroundRecompute` property. It defaults
to false, preserving synchronous console and existing document behavior. Enable
it explicitly for GUI property edits:

```python
solid.BackgroundRecompute = True
solid.MaximumSegmentTests = 7999999
solid.Document.recompute()
print(solid.BackendStatus)
```

Changed source/build descriptors queue a private native solid job and immediately
clear Geometry and SolidValidated. Recompute returns while the worker builds.
Consumers therefore see unavailable geometry until a fresh validated result is
published; they must not reuse the preceding solid. Repeated recompute with the
same descriptor coalesces; changed descriptors replace/cancel older jobs.
Publication recomputes downstream objects on the GUI thread. Initial display
buffers feed the existing view cache, and checked mode remains independently opt-in.

ImplicitSolidRecompute records the owner/proxy/document, input record/settings,
live source signature and initiating transaction. It shares the bounded worker
with task and display jobs. It retries when the native queue is full. It defers
all document property writes while an unrelated transaction is active. A closed
input transaction permits publication when no transaction is active; no extra
output transaction is opened. Undo/redo/abort cancel pending requests without
writing over the restored state. Deletion/document closure also discard jobs.
Task-panel batches retain their existing transaction ownership and reject owners
already busy with ordinary background recompute.

Changed or dirty source inputs reject the pending result. Direct rebuilds also
reject unrecomputed mixed recipes before reusing cached geometry. Failures clear output
and leave an explicit status; repair followed by recompute starts a new request.
Restored valid topology retains its independent unverified qualification status;
merely enabling background recompute does not certify it. Headless execution
continues synchronously even when the persisted setting is true.

Source tracing now has its own [GUI opt-in](native-source-recompute.md); linked
native solids wait for pending source geometry before queuing their builds.
Serialization, result conversion, component assignment, Coin upload and downstream
recompute still run on the caller/GUI thread. This is an opt-in foundation for ordinary rebuild scheduling, not a
fully asynchronous document engine. Automatic/default exposure, broader live
source families, cross-document consumer scheduling and global memory budgets
remain open, alongside the wider Stage 2–4 geometry/integration requirements.

The actual GUI test covers heartbeat/coalescing, downstream expression updates,
undo/redo/abort, foreign-transaction deferral, dirty/stale recipes, work failure
and repair, injected partial-publication failure, save/reopen, deletion and close.
Final GUI/runtime checks pass 3/3 (130.25 s). Separate editor/batch and headless
compatibility results are recorded in PROGRESS.md.
