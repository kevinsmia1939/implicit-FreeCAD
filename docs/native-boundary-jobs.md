# Native boundary-preparation jobs

The OCCT 8 FreeCAD module can now prepare supported gyroid/box boundaries on the
same bounded native worker used for solids and transient display. This is the
backend for moving source tracing out of future GUI recompute/task workflows;
ordinary source recompute can now opt into the
[GUI source-to-solid coordinator](native-source-recompute.md). Both task editors
also adopt this backend through [staged preparation](native-task-sources.md).
New GUI owners now enable ordinary background recompute; everyday native-solid
primitive exposure remains subsequent integration work.

```python
job = native.start_boundary_job(field, box_shape, "common", .1, 512, "regularized")
status = native.native_job_status(job)
# Poll from the GUI event loop; consume only after state == "ready".
boundary = native.take_boundary_job(job)
```

Arguments are a native primitive face, conventional `Part.Shape`, operation
(`common` or `box_minus_field`), tolerance (default 0.1 mm), maximum section cells
(default 512, power of two 4..512) and corner policy (`reject` by default or
`regularized`). The existing mixed-box geometry restrictions, degeneracy rules
and error budgets apply unchanged. Curved/arbitrarily placed B-Rep operands are
not supported by this factory.

Submission serializes the primitive field and conventional B-Rep on the caller
thread, bounded to 64 KiB and 4 MiB respectively. The worker decodes private
geometry and constructs actual shared native trim topology. No document object,
Python reference, mutable Part object or Coin node enters the worker. Taking
returns the ordinary mixed-boundary report plus `input_field`, `input_box` and
`input_settings` (operation, tolerance, cells, corner policy). GUI publication must
compare these descriptors with live inputs and enforce owner/generation/transaction
guards. Report conversion and cap reconstruction still run on the caller thread.

`start_boundary_job_from_record(field, box_record, operation, tolerance, cells,
corner_policy)` accepts an already captured conventional B-Rep string with the
same defaults. GUI coordinators use this helper to preserve the original record
without Part.Shape.copy() or a second encoding step; exact translated-box
metadata stays unchanged.

The result is a source boundary, with fresh seam qualification where the kernel
resolves it; it is not a validated native solid or a display-coverage proof.
Restoring serialized topology retains the existing unverified provenance policy.
Wrong-kind take APIs reject before consumption. Successful take is one-shot;
failure/cancellation expose no result. Source mutation/deletion cannot alter a
submitted record. One worker and one extra outstanding slot are shared across
all three job kinds. Capsule destruction and normal executor shutdown cancel work
and join the worker.

Primitive snapshot decoding preserves empty identity locations for exactly
identity input matrices. Nearby nonzero placements are not snapped. This lets
strict mixed construction accept an identity primitive after a private round trip
without broadening its placement policy or changing the record schema.

OCCT plane-section settings have an optional synchronous WorkCheckpoint callback,
called during sampling, edge solving and curve assembly. It is never retained in
results. This avoids a lower-level TKBRep dependency on TKTopAlgo's work control.
Mixed boundary construction connects it to the existing thread-local scope and
adds root-isolation/side checkpoints. Exceptions propagate cancellation without
returning partial topology. Decoding and opaque conventional OCCT calls may run
until the next checkpoint; no hard cancellation latency is promised.

The settings extension changes the fork's extension ABI. Rebuild direct section
consumers with matching headers/libraries. The active OCCT 8 libraries, FreeCAD
extension and native test/worker consumers were rebuilt together; the OCCT 7
legacy build and installed libraries were left untouched. See HANDOFF.md and
PROGRESS.md for qualification results and the remaining GUI integration work.

Rebuild with the existing configured OCCT 8 trees (two compile jobs and the
FreeCAD build's single-link pool):

```sh
cmake --build build-occt-implicit --target TKBRep TKTopAlgo --parallel 2
cmake --build build-freecad-occt8 --target _ImplicitOCCT ImplicitScripts --parallel 2
cmake --build build-occt-implicit/native-face-tests --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
ctest --test-dir build-freecad-occt8/migration-tests -R '^implicit_occt8_boundary_(jobs|job_shutdown)$' --output-on-failure
```

Finish builds before starting runtime tests. Do not substitute the system
FreeCADCmd or mix OCCT 7 compatibility headers/libraries into this runtime.

Final qualification: 118/118 kernel tests (321.56 s), boundary/shutdown/runtime
3/3 (5.91 s), and existing worker/shutdown/runtime/checked-display GUI 7/7
(186.74 s). The latter includes actual GUI save/reopen and checked-buffer upload
qualification. GUI source-job adoption is now available through the opt-in
source-to-solid coordinator and staged task preparation.
