# Implicit Modelling workbench

Launch `build-freecad-occt8/bin/FreeCAD` and choose **Implicit Modelling**
from the workbench selector. The turquoise toolbar icon now opens **Add native
gyroid solid (10 mm cube)**. Set dimensions/period/level and build controls,
then press OK. Boundary tracing and solid validation run as cancellable private
jobs. Successful creation selects a **Gyroid solid** with native faces, edges
and vertices. Its boundary source, editable Gyroid field and expression-driven
GyroidDomain box are created in the same undo step and hidden. Cancel or failure
creates no partial objects. The older **Native Implicit** workbench remains.

The default geometric tolerance is 0.1 mm with regularized isolated corners.
Independent geometry depth defaults to 18, retaining topology depth 16; larger
or harder models can still exhaust explicit budgets. These are restricted
gyroid/box native solids, not wall-thickness gyroids or general mixed BOP.
The solid uses a native Geometry property; display meshes are transient and
display qualification remains separate from solid validation.

Select the hidden Gyroid field in the tree and edit its
Length/Width/Height, Period and IsoValue properties. Box dimensions follow the
field expressions and source/solid background recompute follows those changes.
Unsupported inputs fail explicitly. Saved native topology is restored with its
validation flags unverified; restore does not silently recertify it.

The menu retains **Add gyroid (10 mm cube)** (`Implicit_Gyroid`) for the older
field-display workflow described below. Its command identifier remains stable.

On the OCCT 8 build, **Native boundaries (experimental)** also provides a
[native edge/vertex selection demo](native-edge-vertex-selection.md) with the
surface preview enabled, plus the supported selected-operand mixed commands.
The demo is separate from the default primitive below.

**Native solids (experimental)** now provides a
[live native-solid demo and selected-boundary build command](live-native-solids.md).
These use actual solid/component topology, source-driven recompute and semantic
selection. Standard Part Common and box-first Cut also provide restricted native
dispatch; see [Part Boolean workflow](native-part-booleans.md).

Select Gyroid in the tree and use the Data property editor:

- **Gyroid dimensions:** Length, Width, Height, initially 10 mm each.
- **Field:** Period (initially 10 mm in internal units), IsoValue (initially 0).
- **Placement:** translation and rotation.
- **Domain:** DomainMin is the local origin; DomainMax is expression-driven by
  the three dimensions. Edit dimensions rather than replacing these expressions.

The View tab exposes resolution, color and display settings. Parameter edits
recompute the native compiled field and its disposable viewport mesh. FCStd
retains the parameters and dimensional expressions. Creation is transactional;
undo/redo uses the normal document setting.

This is a bounded implicit **surface**, not a wall-thickness gyroid or a validated
closed Part solid. The authoritative object is `Implicit::Feature` from the
fork's C++ field module. It is not the old addon, nor is a mesh assigned to Shape.
This command does not run mixed-box tracing or the separate native-solid factory.

## User workbench directory

`workbench/ImplicitModelling/InitGui.py` is the lightweight user entry point.
It is also deployed locally at
`/home/kevin/.local/share/FreeCAD/v27-1/Mod/ImplicitModelling/InitGui.py`.
The fork registers the same workbench in-tree; registration is idempotent.
The user entry point alone cannot give an ordinary FreeCAD installation the
native compiled module. It reports a warning when the fork core is unavailable.

No new dependencies or C++ rebuild are required. Refresh `ImplicitScripts`
and the InitGui resource in the existing build after editing these scripts.
