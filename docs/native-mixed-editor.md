# Native mixed-boundary task editor

The OCCT-8 fork now provides a task panel for supported live native gyroid/box
mixed boundaries. Double-click the result in the tree, or select it and use
FreeCAD's **Toggle Edit Mode** command. The panel edits Common/BoxMinusField,
seam tolerance (explicitly millimetres) and the tracing-cell budget. Field/Box
links are shown read-only; their mathematical definitions remain authoritative.
The tracing-budget selector offers the kernel's supported powers of two from
4 to 512 cells per axis; unsupported restored settings must be corrected before
applying. Exhausting a valid budget is still a numerical preview failure.

To try it in the fork GUI Python console:

```python
import ImplicitMixed
result = ImplicitMixed.demo()
Gui.activeDocument().setEdit(result.Name)
```

Use the wholly OCCT-8 build (`build-freecad-occt8/bin/FreeCAD`), not installed
FreeCAD or the OCCT-7 compatibility build. The native workbench also offers the
gyroid/box demo and selected-operand commands. The default is a caps/seams preview;
the View properties can enable an experimental
[trim-conforming surface candidate](native-conforming-preview.md), with separate
tolerance/budget settings. Neither mode validates CAD solids or general BOP.

## Preview and transaction contract

Changing controls stages values without changing the model. Apply starts one
owned FreeCAD transaction, publishes a native geometry snapshot and updates the
live document dependencies through normal recompute. Repeated previews stay in
that transaction; OK applies/commits, Cancel aborts/restores prior properties and
geometry. A successful edit can be undone/redone normally. Snapshot-local GUI
selections clear when geometry changes; owned snapshots retained by consumers
remain immutable/queryable. Existing mapped-reference invalidation rules apply.

Numerical preview failure clears native geometry and view caches, shows its
reason in the panel and keeps the task open. Correct the settings and Apply
again, or Cancel to restore the prior result. The handled preview invokes the
proxy before normal recompute so its error can be displayed without an uncaught
Python task exception. After failure, the document is still pending recompute;
the absence of Geometry/status message is authoritative, not a claim that a
normal document recompute has already set every native error-status bit.
No failed or partial result is committed by OK.

Task actions never adopt an unrelated active transaction: double-click entry
refuses one, and Apply/normal closing refuse until it is finished. Transactions
are owned by ID and only that ID is committed/aborted. Ordinary reset-edit and
document-close cleanup are tested. Forced framework/Python operations during a
foreign transaction, concurrent external mutations, saving active previews and
deleting individual inputs while editing are not qualified workflows.

## Verification and limits

`mixed_editor_gui_smoke.py` opens the actual task dialog, exercises the normal
Edit command and double-click callback, clicks real Qt Apply/Cancel/OK buttons,
selects native cap geometry during preview, compares region queries, checks
cancel/mapped identity and immutable lifetime, commit/undo/redo, failed-budget
clearing/recovery, reset-edit rollback, unrelated transaction guards, accepted
save/reopen/re-edit and document close with an active preview.
The open-contour/multi-cap fixture also exercises operation preview and cancel
with mapped seam identity restoration.

Run through the separate OCCT-8 migration harness with Xvfb or a desktop display.
This editor covers the supported mixed recipe's three settings, not source-link
replacement, field/DAG editing, uniform physical thickness, arbitrary equations,
placed/curved-operand Booleans, history-based attachment repair or all workbenches.
All unfinished [Stage 2/3/4 acceptance gates](stage-4-acceptance.md) remain in scope.
No new packages/system libraries or OCCT source changes are required.
