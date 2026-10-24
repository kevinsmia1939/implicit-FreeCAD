# Native dispatch from Part Boolean commands

Part Common and Cut now recognize implicit representations before generic Part
shape conversion. Select a primitive compiled gyroid and a conventional box,
then use Part Common; for Cut select the box first. Settings open before any
geometry work. The default tolerance is 0.1 mm with regularized box corners.

Boundary preparation and solid validation run as two cancellable private worker
jobs. No document geometry or transaction is created while they run. Successful
publication creates a live boundary source and native solid, and hides operands
in one undo transaction. The existing source/solid validation and background
recompute policies remain authoritative. Generated display meshes are transient;
the result has native Geometry rather than Part Shape.

Settings, operand fingerprints, document lifetime and transaction ownership are
checked before publication. Failures leave the task open for correction/retry.
The task never aborts an unrelated transaction. Cancellation discards private
results rather than publishing a partial boundary.

This is restricted dispatch, not general mixed BOP. Native solid/boundary inputs,
field-minus-box, Fuse and subelement operands explicitly reject. Curved operands
and arbitrary field graphs remain unsupported by the existing backend. Ordinary
B-Rep selections keep the existing Part Boolean implementation. No OCCT core
changes are required for this command adapter; earlier native OCCT extensions
still supply the mathematical boundary and validated solid.

The GUI regression is `part_boolean_gui_smoke.py`, registered as
`implicit_occt8_gui_part_boolean` in the OCCT 8 migration harness. Run with:

```sh
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests \
  -R gui_part_boolean --output-on-failure
```

Full Stage 4 still requires broader operands, native-solid Boolean composition,
surviving topology history, additional consumers and their acceptance evidence.
