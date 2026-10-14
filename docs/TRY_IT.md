# Try the native build

On this development machine the fork is already built. From the project root:

```sh
bash tools/launch_demo.sh
```

This starts `build-freecad-implicit/bin/FreeCAD`, activates the **Native
Implicit** workbench and creates four examples. Safe mode gives the experiment
temporary preferences and isolates installed addons. The ordinary system
`FreeCAD` launcher does not contain the native module.

## What to try

1. Select **1 - Gyroid** in the tree. In **Data**, change `Period` from 8 to 6,
   then change `IsoValue` from 0.17 to 0.4. Recompute if the document is marked
   for recompute (Edit > Refresh, or `App.ActiveDocument.recompute()` in the
   Python console). The bounded mathematical surface should update.
2. Select **3 - Sphere** and change **Data > Radius** from 6 to 4.
3. For any example, use **View > Resolution** (try 16 then 32), toggle
   `AdaptiveSampling`, and change `DisplayMode` or `SurfaceColor`. Adaptive
   sampling rounds resolution up to a power of two. Rotating the camera should
   not increase `CacheBuildCount`; changing geometry should.
4. Save as an `.FCStd` document and reopen it using the same native executable.
   Geometry and display settings should be restored.

Examples 2 and 4 use composed graphs. Their `FieldGraph` takes precedence over
the primitive `Period`, `Radius`, and `IsoValue` properties. The workbench's
sphere and gyroid commands now create editable primitives directly.

To thicken the capped band, open **View > Panels > Python console** and run:

```python
import json, Implicit
obj = App.ActiveDocument.getObject("GyroidBand")
graph = json.loads(obj.FieldGraph)
graph["root"]["left"]["halfWidth"] = 0.45
Implicit.setFieldGraph(obj, json.dumps(graph))
App.ActiveDocument.recompute()
```

Band width is a dimensionless level-set parameter for the gyroid; it does not
specify uniform physical wall thickness.

For Boolean commands, select two native fields or single-solid Part features,
then choose union/intersection/difference in Native Implicit. Results have live
`Left`/`Right` links; source edits propagate on recompute. Differing placements
and inside conventions are supported. The demo examples are spaced apart, so
move two spheres into overlap for a visibly interesting Boolean result.

The **Live B-Rep field** command takes one single-solid Part object. The native
Part/OCCT bridge is required. Build it with the existing configured dependencies:

```sh
cmake --build build-freecad-implicit --target Part PartGui PartScripts Implicit ImplicitGui --parallel 2
```

Recompute after changing the Part source's dimensions or placement. Mixed
results are still implicit fields, not Part solids. STEP export and native OCCT
implicit-face Booleans are still planned. Restart an already-open development
GUI after rebuilding; an existing process does not load the new implementation.

To try a prepared live workflow after building Part/PartGui:

```sh
bash tools/launch_demo.sh --live-part
```

Only the mixed result is visible. Edit `Band.Amount` (dimensionless),
`Gyroid.Period`, or the hidden Part box's dimensions/Placement, then recompute.
The result follows those inputs. You can inspect `Left`/`Right` in Data and save
the document with all dependencies intact.

## Automated checks

```sh
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

The GUI regression exits automatically; the interactive demo stays open.
