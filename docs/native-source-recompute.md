# GUI source-to-solid background recompute

The live MixedBoundary owner now has a persisted `BackgroundRecompute` opt-in,
default false. Enable it together with the downstream native solid setting for
property-driven GUI edits:

```python
source.BackgroundRecompute = True
solid.BackgroundRecompute = True
source.Operation = "BoxMinusField"
source.Document.recompute()
```

Changed source recipes queue a private native boundary job and clear Geometry,
seam qualification and same-document native-solid output immediately. Native
solids report pending source preparation and return successfully from recompute.
When the source finishes, guarded publication triggers document recompute;
opted-in solid owners then queue their own fresh builds. Sources or solids with
the flag false retain their synchronous path. Headless execution stays synchronous
even when the flag was saved true. Enabling a flag on unchanged restored topology
does not retrace or revalidate it.

ImplicitMixedRecompute reuses the solid coordinator's timer, deletion/document
observers and undo/redo/abort cancellation. Requests capture owner/proxy/document,
field/box object identities, canonical recipe fingerprint, private primitive face
and box copies, exact worker descriptors and initiating transaction. Equivalent
requests coalesce; changed recipes replace old requests. Dirty operands,
unsupported field definitions/placements or mismatched result descriptors reject
publication. No status/output writes enter unrelated active transactions. Once
the input edit closes, results publish without adding an output transaction.

Failures clear partially published boundary geometry and qualification, invalidate
same-document native-solid consumers and leave an explicit unavailable status.
Repair and recompute retry. Direct source rebuilds invalidate consumers even
without an intervening document recompute pass. Native task-owned batches keep
their own failure/transaction coordinator.

Only the existing primitive gyroid/box source family and same-document pipeline
are qualified here. Task previews with a background-enabled source, default
asynchronous exposure, cross-document scheduling and broader input families
remain open. Serialization, recipe checks, cap reconstruction/tessellation,
result conversion, Coin upload and document recompute remain GUI-thread work.
Field definitions/native topology remain authoritative; all meshes are transient.
The 0.1 mm default and existing numerical/degeneracy policies are unchanged.

See [boundary jobs](native-boundary-jobs.md), [solid recompute](native-solid-recompute.md)
and HANDOFF.md for the backend, builds and qualification results.

Final source GUI/runtime qualification passes 3/3 (143.43 s), including shared
solid handoff, Qt heartbeat, coalescing, transaction/undo/abort guards, direct
repair, early invalid definitions, injected partial publication, failure recovery,
save/reopen with unverified topology, deletion and document closure. Separate
headless and existing property/task compatibility passes 6/6 (226.73 s).
