# Implicit Modelling workbench

Launch `build-freecad-occt8/bin/FreeCAD` and choose **Implicit Modelling**
from the workbench selector. Click the turquoise **Add gyroid (10 mm cube)**
icon. A document is created if needed, and a selected **Gyroid** object appears
in the tree. The older **Native Implicit** experimental workbench remains.

On the OCCT 8 build, **Native boundaries (experimental)** also provides a
[native edge/vertex selection demo](native-edge-vertex-selection.md) with the
surface preview enabled, plus the supported selected-operand mixed commands.
The demo is separate from the default primitive below.

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
This command does not run mixed-box tracing or the pending native-solid factory.

## User workbench directory

`workbench/ImplicitModelling/InitGui.py` is the lightweight user entry point.
It is also deployed locally at
`/home/kevin/.local/share/FreeCAD/v27-1/Mod/ImplicitModelling/InitGui.py`.
The fork registers the same workbench in-tree; registration is idempotent.
The user entry point alone cannot give an ordinary FreeCAD installation the
native compiled module. It reports a warning when the fork core is unavailable.

No new dependencies or C++ rebuild are required. Refresh `ImplicitScripts`
and the InitGui resource in the existing build after editing these scripts.
