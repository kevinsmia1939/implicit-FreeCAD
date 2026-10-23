# Live native-solid task editor and independent geometry depth

## Create with chosen build settings

Select a supported native mixed boundary, then choose **Implicit Modelling →
Native solids (experimental) → Build live native solid from boundary**. The
creation task lets you choose tolerance, topology resolution, independent
geometry depth and all work budgets before the first build. Opening the task
does not create geometry or an object. Cancel closes it without document changes.

OK submits a background native build and initial display extraction. A GUI timer
reports the current phase while keeping Cancel available. No owner or transaction
is held during numerical construction. A failed build keeps the task open with
the failure reason, so settings can be corrected. Cancel or document closure
cancels outstanding work. Changed inputs/settings and unrelated transactions
prevent stale-result publication. A ready result is published atomically.
Successful creation optionally hides the boundary in the same undo transaction;
undo/redo restores both the result and source visibility. The shifted 20-mm
fixture succeeds with geometry depth 18 and the unchanged 0.1-mm tolerance.
More depth or work does not guarantee a supported result. The initial candidate
is reused without GUI-thread remeshing. Console creation stays synchronous;
new GUI owners enable background property recompute and display extraction uses
background jobs. See [job contracts](native-solid-jobs.md).

The mixed-boundary GUI commands now offer their own cancellable creation task;
new GUI sources also enable background recompute. Legacy source flags remain
unchanged when creating a new solid. See [creation defaults](native-gui-creation.md).

The console factory retains its existing positional arguments and additionally
accepts a validated dictionary of integer work/depth limits:

```python
solid = ImplicitSolid.create(
    boundary, tolerance=.1, maximum_geometry_depth=18,
    build_limits={"MaximumGeometryCells": 2000000},
    hide_source=True,
)
```

Allowed dictionary keys are MaximumDepth, MaximumCells, MaximumTests,
MaximumSeparatorCells, MaximumGeometryCells and MaximumSegmentTests. Independent
geometry depth remains its dedicated argument. Unknown keys, noninteger values
and values outside these limits reject before document mutation:

| Limit | Default | Allowed range |
|---|---:|---:|
| MaximumDepth | 16 | 0–16 |
| MaximumCells | 4,000,000 | 1–10,000,000 |
| MaximumTests | 2,000,000 | 1–10,000,000 |
| MaximumSeparatorCells | 2,000,000 | 1–10,000,000 |
| MaximumGeometryCells | 2,000,000 | 1–10,000,000 |
| MaximumSegmentTests | 8,000,000 | 1–50,000,000 |

## Edit an existing native solid

Double-click a live native solid in the OCCT-8 fork to open its task panel.
Apply previews changes in one owned transaction; OK commits, Cancel or ResetEdit
rolls back. The task exposes solid tolerance, topology resolution/depth, geometry
depth, work budgets and followed placement. For a same-document live mixed source,
it also stages Common/BoxMinusField. That operation edits the shared source and
therefore affects every dependent result; the panel explicitly warns about this.
Stored boundary snapshots and external sources do not acquire an operation editor.
Primitive source parameters are now staged as described below. Source-link
replacement and general field-graph editing remain separate gates.

GUI Apply/OK now prepare changed source groups, then build affected native-solid
peers in background batches with phase reporting. Each group publishes only after
all its results succeed. OK waits for successful completion;
Cancel discards pending work and rolls back. Changed controls/inputs or loss of
the transaction prevent stale publication. Initial default candidates are reused.
Serialization, planar-cap display, result conversion and Coin upload still run
on the GUI thread. Cancel preserves the journaled geometry/fingerprint pair,
avoiding a numerical retrace of unchanged restored topology. Direct Python
task.apply() remains synchronous. See [staged task contracts](native-task-sources.md).

The panel reuses the mixed-boundary task's owned-transaction/button lifecycle,
with solid-specific staged controls and validation. It does not adopt or close
unrelated transactions. Previews execute the source/result explicitly before
normal dependency recompute, so an expected numerical failure is caught in the
panel rather than producing an uncaught document-recompute traceback. Failed
solid construction clears geometry, counts, buffers and checked display state;
the task stays open for repair or Cancel. Cancel restores both source and solid
changes through the document transaction. The Tasks view is shown explicitly;
GUI tests wait for its animated overlay before requiring real visible buttons.
The form wraps labels above controls and uses FreeCAD's existing task scroll
area. A GUI-core fix grows/reanchors active task overlays to their child minimum
size and refreshes them after delayed minimum-size updates. Without this, Qt
expanded the task child beyond a restored narrow overlay and clipped Cancel.
Inactive overlays keep their existing size policy. Tests require the entire
action buttons and selected depth entry to be in their actual visible regions,
not merely flagged visible in Qt.

## Shared primitive source parameters

For a same-document, identity-placed primitive gyroid and a conventional box,
the task stages Period, IsoValue and Length/Width/Height. Dimensions are editable
only for a pure-translated `Part::Box`; other conventional box snapshots expose
read-only dimensions. An optional fit-domain control updates the authoritative
field domain to the planned box bounds. The task does not introduce duplicate
field definitions or backwards property links that create dependency cycles.

The focused workbench's Length/Width/Height properties and canonical DomainMax
expressions are preserved: fitting updates their length inputs, not the saved
expressions. Ordinary fields without these aliases use direct domain properties.
Expression-driven scalar/dimension controls are disabled; custom domain
expressions disable fitting and are never replaced. Untouched rounded controls
retain original binary64 input values, including the solid tolerance. This
prevents opening/applying a task from silently changing saved input precision.

Edits use the task's existing owned transaction. Before modifying shared math,
all live mixed sources sharing the field or box and their native solid results
are invalidated. All these same-document native peers are explicitly rebuilt;
failed geometry previews cannot leave the old native results displayed as valid.
Errors remain in the task for repair or Cancel. Undo/redo and cancellation restore
the shared source parameters and dependent geometry together. Changes to observed
source inputs outside the task require closing/reopening it; cross-document
shared-input changes reject before mutation. This is not general transactional
support for arbitrary consumers or cross-document dependency graphs.

The successful 10-mm to 5-mm unit-cell edit retains the 0.1-mm target. Exposing a
level or dimension control does not qualify every value: native topology,
regularity, physical tolerance and work-budget checks still apply. In particular,
the critical gyroid level 1.5 rejects and clears dependent geometry rather than
guessing a valid solid. Field graphs, transformed fields and source-link
replacement are outside this primitive editor increment. The default Add Gyroid
command remains unchanged.

## Independent geometry checking

`MaximumDepth` still controls topology/reference checks (up to 16).
`MaximumGeometryDepth` is now a separate integer property:

- `-1` inherits `MaximumDepth`, preserving the old default behavior.
- `1..24` selects the native cap-geometry check depth independently.
- Other values reject; no silent clamping or tolerance relaxation occurs.

```python
import ImplicitSolid
solid = ImplicitSolid.create(source, maximum_geometry_depth=18)
# Or edit an existing live feature, then recompute:
solid.MaximumGeometryDepth = 18
solid.Document.recompute()
```

The geometry depth is not a meshing resolution, physical tolerance or wall
thickness. More depth does not promise success: work/precision/topology limits
and unsupported source cases still fail explicitly. The tested shifted 20-mm
unit cell remains at 0.1 mm, with topology depth 16 and geometry depth 18; shared
default depth fails. Creation failure aborts the whole transaction and leaves
no half-built owner. Creation now checks the initial proxy before normal recompute
so such expected failures remain catchable without a recompute traceback.

## Migration and persistence

Older schema-1 live features missing the property migrate to `-1`. Their original
eight-setting fingerprint and native call prefix remain byte-compatible. Only a
non-default independent depth extends the descriptor/call with a ninth setting.
Unchanged legacy and new FCStd files restore actual topology without rebuilding;
restored native-solid validation stays false. Tests inspect the legacy fixture's
Document.xml to ensure the property really was absent, not merely defaulted in
an already-updated file. Explicit changed settings build a new checked result;
they never certify a retained loaded snapshot in place.

## Translated conventional boxes

The corner-aware OCCT input recognizer now accepts **exact pure translations**
on conventional box topology. Every location must have an exactly identity
linear part and finite translation. `BRep_Tool`/adaptors supply actual world-space
vertices, lines and planes. All existing exact corner, line/plane intercept,
orientation, topology, coordinate-precision, field-domain and corner-germ checks
remain. The operand is not snapped, baked, welded or modified.

For both operations, a located box produces the same canonical topology record
as the equivalent box built directly at those coordinates. Native solid geometry
checks pass at 0.1 mm; source location identity/free flag and absent triangulations
are preserved. Rotations, scaling and placed implicit fields remain rejected;
arbitrary container/link/input transforms and curved operands are not qualified
by this restricted translation extension. Output placement remains a separate
existing contract.

## Verification and remaining work

Kernel tests compare actual topology records, both operations, native solid bounds,
source immutability and unsupported transforms. CLI tests cover exact legacy
fingerprints, a genuinely missing property, forwarding of independent depth,
invalid settings, atomic creation failure, a shifted larger Part::Box and unchanged
FCStd restore with native building forbidden. GUI tests cover stored read-only
sources, staging, real Apply/OK/Cancel, operation/depth undo/redo, budget failure
and repair, ResetEdit, unrelated transaction protection, checked-display invalidation
and restore. Full results are recorded in PROGRESS.md.
A real GUI capture after Apply is saved as `/tmp/implicit-solid-editor.png`.
The parameter-edit capture is `/tmp/implicit-source-parameters-editor.png`.
The parameter test scrolls the full control rectangle: Qt's ensureWidgetVisible
can use a spin box's input-method cursor rectangle, which does not guarantee the
entire control is inside the viewport. The final test checks that containment
and all standard task buttons, and the capture is inspected visually.
Creation tests also cover no build on open/Cancel, invalid inputs/limits, atomic
budget failure/retry, the shifted 20-mm result, source visibility undo/redo,
unchanged FCStd restore and document closure while staging. Captures are
`/tmp/implicit-solid-creation-settings.png` and `/tmp/implicit-solid-created.png`.

This is not completed Stage 4. The default Add Gyroid command is unchanged.
Broader field-graph/source-link editing and responsive scheduling/cancellation,
curved conventional operands, snapping, surviving-element history and consumer/
placement integration remain part of the active acceptance requirements.
