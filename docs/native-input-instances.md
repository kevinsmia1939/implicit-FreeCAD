# Live mixed input instances

Mixed boundaries can use same-document whole-object App::Link inputs, including
nested links. The gyroid's effective instance matrix must be exactly identity;
box instances and container-owned boxes currently require pure translations.
Root conventional Part shapes retain the existing native box qualification.
This does not add curved operands, arbitrary field transforms, scaled instances,
link arrays or subelement operands.

`ImplicitOperands` uses FreeCAD's `getLinkedObject(..., matrix)` rather than
assuming an App::Link has `getGlobalPlacement()`. It composes the operand's
parent GeoFeatureGroup placements. A linked template's parent container is not
part of the instance path, matching FreeCAD link semantics. LinkTransform decides
whether the template's own placement participates. Effective field placement
must be exactly identity, without tolerance-snapping a small translation.

Box snapshots start from the resolved conventional shape through the shallow
`Part.Shape(shape)` constructor, then replace its location with the effective
translation. The document Shape property is immutable; it is never modified.
`Shape.copy()` is avoided because deep copying can flatten locations and change
coordinate bits. Original B-Rep records continue to enter private native jobs.

Field and Box properties now use App::PropertyLinkGlobal, so a recipe outside an
operand container can reference its children. Legacy dynamic local-scope links
are migrated in place with their values/expressions retained. Direct-root recipe
fingerprints retain their previous format. Instance recipes additionally record
resolved target names and matrices, with signed zero canonicalized. Publication
and task guards therefore reject retargeted links or changed effective poses.

FreeCAD does not normally touch these recipes when a container placement changes.
A document observer marks affected mixed sources touched on parent Placement
changes, and marks mixed sources touched on Group changes. It neither publishes
geometry nor writes status/geometry properties. The ordinary recompute pipeline
then schedules source and solid work. This avoids introducing container backlink
cycles when source objects themselves belong to groups.

Shared-input editing resolves primitive targets. Peer invalidation follows aliases
that share those targets; alias wrappers are recomputed in dependency order after
primitive edits, before task-owned source preparation. Link/source replacement or
placement changes invalidate the editor's captured signature. Save/reopen retains
topology with unverified provenance rather than silently retracing it.

## Native coordinate persistence

The decimal translated instance regression exposed a separate OCCT persistence
bug. TopTools_ShapeSet writes per-shape geometry at 15 significant digits. This
loses some exact binary64 corner coordinates, so reading a newly written native
boundary can fail its exact corner membership checks during solid construction.

The native mixed-boundary writer now uses a private BRepTools_ShapeSet subclass
that writes per-shape geometry with max_digits10. Standard B-Rep format/version,
generic OCCT persistence, topology checks and the 0.1 mm policy are unchanged.
Native record errors also identify the failed invariant. Old records remain
subject to the same validation; damaged coordinates are not guessed or snapped.
The kernel regression checks exact vertex coordinates, deterministic record
round-trip, both operations, solid construction and source immutability.

## Reproduction

Use existing builds with two compile jobs and one link job. Rebuild TKTopAlgo
and the native-face test executable for the codec change; no extension ABI or
general OCCT header changes are involved. Rebuild ImplicitScripts for the FreeCAD
integration. Never replace libraries during runtime tests.

```sh
cmake --build build-occt-implicit --target TKTopAlgo --parallel 2
cmake --build build-occt-implicit/native-face-tests --target implicit_occt_native_face_tests --parallel 2
cmake --build build-freecad-occt8 --target ImplicitScripts --parallel 2
ctest --test-dir build-occt-implicit/native-face-tests --output-on-failure
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests \
  -R '^implicit_occt8_(operand_instances|gui_operand_instances)$' --output-on-failure
```

This increment used CMake/Ninja, GTest/CTest, the fork's FreeCADCmd/GUI, PySide
event-loop checks, Xvfb and GDB exception backtraces to locate the codec failure.
It required no package installation or system-library changes.

New migration tests are `implicit_occt8_operand_instances` and
`implicit_occt8_gui_operand_instances`. The first compares both operations with
an independently located conventional reference and checks link semantics,
migration, shared-target edits, unsupported cases, rollback and unchanged restore.
The GUI test covers selection routing, staged source/solid creation, background
container edits, task editing/cancel and stale creation results. Broader Stage
2–4 gates remain open.
