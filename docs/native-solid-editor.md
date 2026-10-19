# Live native-solid task editor and independent geometry depth

Double-click a live native solid in the OCCT-8 fork to open its task panel.
Apply previews changes in one owned transaction; OK commits, Cancel or ResetEdit
rolls back. The task exposes solid tolerance, topology resolution/depth, geometry
depth, work budgets and followed placement. For a same-document live mixed source,
it also stages Common/BoxMinusField. That operation edits the shared source and
therefore affects every dependent result; the panel explicitly warns about this.
Stored boundary snapshots and external sources do not acquire an operation editor.
Source-link replacement and general field/dimension editing are separate gates.

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

This is not completed Stage 4. The default Add Gyroid command is unchanged.
Broader field/parameter/source-link editing, responsive scheduling/cancellation,
curved conventional operands, snapping, surviving-element history and consumer/
placement integration remain part of the active acceptance requirements.
