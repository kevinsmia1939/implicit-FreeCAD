# FreeCAD core-build dependency audit: openSUSE Tumbleweed

Initial audit and latest verification: 2026-10-14. The package checklist below
records what was missing before the user installed dependencies. Configuration
and the `Implicit` and `FreeCADMainCmd` targets now build successfully; no
additional packages are needed for the current native extraction work. The
fork's GUI executable and native view provider now build and pass the isolated
GUI regression test. The combined root suite passes 10/10 tests.

The FreeCAD tree is based on 27.1.0-dev at `d732e2f13a`, with project changes on
`feature/native-implicit`. It requires Qt 6
and defaults to bundled Coin/Pivy, PyCXX, Clipper2, and several other third-party
components. The host has GCC 16.2.1, CMake 4.4.3, Ninja 1.13.2, Python 3.13
development headers, Boost, OCCT 7.9.3 development files, Qt 6 base development
files, ICU, Eigen, yaml-cpp, zlib, zstd, FreeType, Fontconfig, OpenGL/X11
development files, SWIG, and all checked-out submodules.

The official FreeCAD Linux compilation guide lists openSUSE package names, but
its Tumbleweed section still describes the older Qt 5 stack. The checked-out
source is authoritative for this tree and explicitly calls `find_package(Qt6)`.

## Initially missing packages (subsequently installed by the user)

- `libxerces-c-devel`
- `qt6-svg-devel`
- `qt6-tools-devel`
- `python313-pyside6-devel`
- `python313-shiboken6-devel`

Historical installation command (already completed; do not rerun unnecessarily):

```sh
sudo zypper install \
  libxerces-c-devel \
  qt6-svg-devel \
  qt6-tools-devel \
  python313-pyside6-devel \
  python313-shiboken6-devel
```

`qt6-tools-devel` supplies the UiTools and LinguistTools components required by
the current CMake setup. The Python development packages provide the CMake
metadata and headers absent from the installed runtime-only PySide6/Shiboken6
packages.

## Deliberately disabled optional dependencies

A first in-tree `Implicit` module needs App, Gui, Part, OCCT, and the bundled
Coin/Pivy stack. It does not need FEM, SMESH, CAM, Assembly, BIM, Netgen, VTK,
MED, PCL, or developer-wide GoogleTest targets. Disabling those subsystems keeps
the first build focused and avoids installing these currently missing optional
packages:

- `vtk-devel`, `libmed-devel`, `netgen-devel` and MPI/HDF5 variants for FEM/SMESH;
- `python313-pybind11-devel` for CAM/FlatMesh paths;
- `python313-lark` and optional IFC runtime support for BIM;
- `libspnav-devel`, avoided with 3Dconnexion legacy support disabled;
- `doxygen`, required only for generated documentation;
- optional PCL/OpenCV/ODE packages.

## Reproduce the native configuration

Dependency installation and the native build were authorized. The native module
is implemented; configure out of tree with Ninja and constrained job pools:

```sh
cmake -S FreeCAD -B build-freecad-implicit -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DBUILD_GUI=ON \
  -DBUILD_IMPLICIT=ON \
  -DIMPLICIT_OCCT_SOURCE_DIR="$PWD/OCCT" \
  -DBUILD_PART=ON \
  -DBUILD_PART_DESIGN=OFF \
  -DBUILD_FEM=OFF \
  -DBUILD_MESH_PART=OFF \
  -DBUILD_FLAT_MESH=OFF \
  -DBUILD_CAM=OFF \
  -DBUILD_ASSEMBLY=OFF \
  -DBUILD_BIM=OFF \
  -DBUILD_POINTS=OFF \
  -DBUILD_OPENSCAD=OFF \
  -DBUILD_TECHDRAW=OFF \
  -DBUILD_WEB=OFF \
  -DBUILD_HELP=OFF \
  -DBUILD_DRAFT=OFF \
  -DBUILD_IMPORT=OFF \
  -DBUILD_INSPECTION=OFF \
  -DBUILD_JTREADER=OFF \
  -DBUILD_REVERSEENGINEERING=OFF \
  -DBUILD_SPREADSHEET=OFF \
  -DENABLE_DEVELOPER_TESTS=OFF \
  -DFREECAD_USE_3DCONNEXION_LEGACY=OFF \
  -DFREECAD_PARALLEL_COMPILE_JOBS=2 \
  -DFREECAD_PARALLEL_LINK_JOBS=1
```

Build only the needed targets and run the module smoke tests:

```sh
cmake --build build-freecad-implicit --target Implicit ImplicitScripts FreeCADMainCmd --parallel 2
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/native_smoke.py
./build-freecad-implicit/bin/FreeCADCmd FreeCAD/src/Mod/Implicit/Tests/mesh_smoke.py
# Build the fork's GUI and provider when GUI verification is needed:
cmake --build build-freecad-implicit --target ImplicitGui FreeCADMain FreeCADGui_Resources Stylesheets_data --parallel 2
```

This configuration uses GCC 16.2.1, Python 3.13.15, Qt/PySide/Shiboken 6.11.2,
installed OCCT 7.9.3, bundled Coin 4.0.10, Pivy 0.6.11 and PyCXX 7.1.11.
`BUILD_GUI=ON` configures GUI dependencies but building the targets above does
not build the complete GUI. Installed FreeCAD was used for the earlier addon GUI
test. `IMPLICIT_OCCT_SOURCE_DIR` compiles only the fork's new signed-distance
extension with installed OCCT headers/libraries. This does not rebuild all OCCT
toolkits or replace system libraries. Only the new header is staged to avoid an
8.1-source/7.9-library ABI mismatch. No new dependencies are needed for these
increments; Xvfb is already available for isolated GUI testing.

The host has about 44 GiB disk free and roughly 1.3 GiB readily available RAM
plus swap at the latest check. Keep two compile jobs and one link job.
Configuration may reveal another dependency on a different host; report it
instead of installing anything automatically. See [`HANDOFF.md`](HANDOFF.md) for
the full continuation context and unified CTest invocation.

## OCCT 8 source compatibility check

The new OCCT translation unit was compiled against the fork's own headers in a
separate ignored build. Its runtime tests use the installed 7.9.3 compatibility
build described above. To repeat the source-only check without building all
OCCT toolkit dependencies:

```sh
cmake -S OCCT -B build-occt-implicit -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_MODULE_Visualization=OFF \
  -DBUILD_MODULE_ApplicationFramework=OFF \
  -DBUILD_MODULE_DataExchange=OFF \
  -DBUILD_MODULE_Draw=OFF \
  -DUSE_FREETYPE=OFF -DUSE_TBB=OFF
cmake --build build-occt-implicit --parallel 1 \
  --target src/ModelingAlgorithms/TKTopAlgo/CMakeFiles/TKTopAlgo.dir/BRepClass3d/BRepClass3d_SignedDistance.cxx.o
```

GoogleTest 1.18 is available on this host; the umbrella build also runs the new
OCCT GoogleTest source when found. This does not enable the full FreeCAD or OCCT
developer-test suite.

Official references:

- [FreeCAD compile-on-Linux guide](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Compile_on_Linux.md)
- [FreeCAD third-party libraries](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Third_Party_Libraries.md)
- [Current FreeCAD CMake build options](https://github.com/FreeCAD/FreeCAD/blob/main/cMake/FreeCAD_Helpers/InitializeFreeCADBuildOptions.cmake)

