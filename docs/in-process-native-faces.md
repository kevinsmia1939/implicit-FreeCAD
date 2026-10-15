# In-process OCCT-native faces (experimental)

This increment adds actual native `TopoDS_Face` ownership inside the OCCT 8
FreeCAD process. It reuses the existing OCCT fork's field-face/query/sphere
algorithms; it does not recreate them in Python or turn display triangles into
B-Rep faces. No new OCCT source changes or system dependencies were needed.

## Ownership and ABI boundary

`FreeCAD/src/Mod/Implicit/App/AppNativeOCCT.cpp` builds `_ImplicitOCCT` only with
explicit `FREECAD_OCCT_BUILD_DIR` selection and OCCT 8. `Implicit` depends on that
small target in this configuration. The preserved OCCT 7 build never compiles it.

Import **`ImplicitOCCT`**, not the private extension. The public facade checks
Part's OCCT version, the Part/facade build roots and the extension's origin before
loading C++. The legacy regression confirms that the OCCT 7 process rejects this
import without loading `_ImplicitOCCT` or any OCCT 8 toolkit. These are supported
API guards, not a security boundary against deliberate ctypes/private-DLL loading.

Handles are opaque Python capsules owning native face values and their referenced
geometry/topology. The public API has no mutators or Part conversion. Query,
placement and operation results are immutable snapshots: recomputing a document
or closing it does not invalidate a separately retained handle. They cannot be
pickled or assigned to `Part::Feature::Shape`. Wrong objects are rejected before
native calls. C++ failures become Python exceptions, with no partial result.

Calls currently hold the GIL and run synchronously. This is not an asynchronous
operation scheduler, semantic selection API or dedicated complex-geometry property.

## Python API

All arguments are positional; coordinates are three-element sequences. Model
lengths use the FreeCAD model's millimetres; raw gyroid values remain dimensionless.

| API | Contract |
|---|---|
| `sphere(center, radius, tolerance=.1)` | Exact sphere field, finite enclosing query box; tolerance is not a mesh |
| `gyroid(period, iso, minimum, maximum, tolerance=.1)` | Exact gyroid definition and finite computational domain, not a closed solid |
| `value`, `gradient`, `normal` `(face, point)` | Placed field queries; field value/gradient also exist outside the trim; normal rejects critical points |
| `in_domain(face, point)` | Explicit query-domain/plane-predicate membership |
| `info(face)` | Representation, conservative bounds, stored tolerance; explicitly no UV surface or validated solid |
| `intersect_ray(face, origin, direction, min, max, samples=512, tolerance=0)` | Sampled sign roots with point, residual and geometric bracket width; tangencies/thin features may be missed |
| `translated(face, offset)` | New instance placement; input unchanged; rejects domain/placement overflow |
| `trim(sphere, plane_normal, plane_offset)` | Existing transverse sphere-plane operation, keeping `unit_normal.dot(point) <= offset` |
| `boolean_faces(a, b, operation)` | Existing restricted sphere `common`/`fuse`/`cut`; list of `(face, source_index)` boundaries, not a solid or generic BOP |
| `tessellate(face, tolerance=.1)` | Full sphere/spherical plane-patch only; disposable world-coordinate vertices/normals/zero-based triangles and geometric bound |
| `dumps(face)`, `loads(record)` | Explicit OCCT schema-1 primitive interchange, maximum 64 KiB; not generic B-Rep/STEP/OCAF persistence |

Sphere meshing retains the kernel's 120,000-vertex/240,000-triangle budgets and
geometric interpolation bound. The wrapper applies geometry/instance placement
and accounts for reflection winding; translated and reflected meshes are tested.
No gyroid surface meshing, caps, face-to-face mesh welding or watertight/manufacturing
guarantee is added. See [restricted sphere contracts](native-sphere-operations.md).
For Boolean boundary faces, `value` remains the source primitive's field, not
the final CSG signed distance or a whole-result material classifier.

Primitive records preserve the existing kernel's field/domain/tolerance,
placements and orientation. Trim subtypes and child topology reject with
`NotImplementedError`; they are never reduced to an untrimmed primitive. Unicode,
malformed/trailing/oversized records and unsupported object arguments are checked.
Pickling a handle is deliberately unsupported. The field is authoritative;
meshes and Python memory addresses are not persisted as geometry.

Example in the OCCT 8 Python console:

```python
import ImplicitOCCT as native
face = native.sphere((0, 0, 0), 2, .1)
native.value(face, (2, 0, 0))          # 0
copy = native.loads(native.dumps(face))
patch = native.trim(face, (0, 0, 1), .5)
native.in_domain(patch, (0, 0, 2))    # False
# native.dumps(patch) raises NotImplementedError: trim topology unsupported.
```

## GUI and document persistence

Launch `bash tools/launch_demo.sh --occt8`. In Native Implicit choose
**OCCT in-process sphere (experimental)**, or use:

```python
import ImplicitNative
obj = ImplicitNative.create("Trim", backend="InProcess")
handles = obj.Proxy.native_faces()     # Last successful recompute's snapshots.
```

The existing boundary feature now has an explicit `Backend` enumeration. `Worker`
remains the default, including for old schema-1 documents lacking this property.
`InProcess` uses `ImplicitOCCT`, retaining native handles in a transient proxy
cache. It does not invoke a worker or silently fall back. The new menu command
is registered only when the native API is available. A failed operation clears
both handle and display caches and updates the retained Coin buffers to empty.

This is still an `App::FeaturePython`, **not** `Part::Feature::Shape`. Its editable
primitive operands, operation, tolerance and backend persist in FCStd. Restore
reconstructs the native faces from that recipe; capsules, native child topology
and mesh buffers are not written into the file. Changing parameters produces
new snapshots; previously retained handles remain unchanged. In-process recipes
require the OCCT 8 build, while worker recipes remain usable in either build.

## Build and verification

Reuse the existing build directories, two compile jobs and one link job:

```sh
cmake --build build-freecad-occt8 --target Implicit ImplicitScripts --parallel 2
cmake -S tests/freecad_occt8 -B build-freecad-occt8/migration-tests \
  -DIMPLICIT_MIGRATION_BUILD="$PWD/build-freecad-occt8" \
  -DIMPLICIT_OCCT_LIBRARY_DIR="$PWD/build-occt-implicit/lin64/gcc/lib" \
  -DIMPLICIT_MIGRATION_GUI_TESTS=ON \
  -DIMPLICIT_OCCT_NATIVE_WORKER="$PWD/build-occt-implicit/native-face-tests/implicit_occt_native_worker"
python3 tools/with_xvfb.py ctest --test-dir build-freecad-occt8/migration-tests --output-on-failure

# Preserve and verify the compatibility ABI, including the public import guard:
cmake --build build-freecad-implicit --target Implicit ImplicitScripts --parallel 2
cmake -S . -B build
python3 tools/with_xvfb.py ctest --test-dir build --output-on-failure
```

Results: **20/20** OCCT 8 migration tests (50.92 seconds), **25/25** compatibility/
combined tests (31.38 seconds). The latter includes 24 isolated native OCCT GTest
cases in one entry and the new legacy import guard. These are targeted suites,
not full upstream FreeCAD/OCCT qualification. The GUI native-face test disables
worker subprocesses during creation, edits and save/reload, proves native handle
lifetime across document close, and checks invalid definitions clear the view.

Next: expose native gyroid section/split snapshots and shared-trim topology
without a worker; extend topology-aware persistence and conforming meshing.
General Part consumers, UV-dependent generic algorithms, mixed implicit/B-Rep BOP,
validated solids and STEP export of native implicit faces remain unsupported.
