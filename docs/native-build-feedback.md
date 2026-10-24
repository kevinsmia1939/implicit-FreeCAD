# Live native build feedback

Mixed boundary sources and native solids expose a read-only `BuildFeedback`
property in the View tab, under Native build. Their tree icons carry a matching
badge. The feedback describes the current owner and its inputs:

| State | Meaning |
| --- | --- |
| Pending | A native build is active; output may be unavailable. |
| Failed | Build/restore failed, or the required source is unavailable. |
| Changed | Stored geometry exists, but inputs are touched or invalid. |
| Empty | No geometry or active build; recompute is required. |
| Unverified | Geometry exists without current boundary/solid verification. |
| Ready | Boundary seam verification or native solid validation is current. |

Ready does not certify the display mesh. For a boundary it also does not imply
solid validity. Display checking and native solid validation retain their
separate properties and acceptance gates. Loading saved topology does not turn
it into newly validated geometry.

`Implicit::PropertyBuildFeedback` is a dedicated Python-object property with a
normal property-editor item. Its value calculates text when read. A GUI timer
emits icon-change notifications and repaints property views when the text changes;
it never assigns a status string, recomputes the document or changes proof flags.
This matters because even transient property writes can enter an unrelated
active undo transaction. The property is transient, output, read-only and marked
NoModify/NoRecompute. The feedback value resolves its owner by document identity
and object name without retaining the document or view. Runtime feedback is
recreated on attachment rather than serialized as model authority.

Geometry refresh preserves whole-object selection, so the property panel stays
attached during failure and repair. Snapshot-local edge/face/vertex selections,
including link instances, are discarded before old indices can be reused.

The implementation lives in `ImplicitBuildFeedback.py`, the dedicated App
property class and `NativeBoundaryView`. It applies to mixed sources and native
solids, including restored objects, rather than changing the older field display
or treating its mesh as a native B-Rep.

The migration harness test is `implicit_occt8_gui_build_feedback`; use the
existing OCCT 8 FreeCAD build and Xvfb wrapper. Build only `Implicit` and
`ImplicitScripts` after modifying this code, with two compile jobs and one link
job. Do not rebuild libraries while GUI tests are running.
