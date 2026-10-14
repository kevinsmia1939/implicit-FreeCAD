# FreeCAD core-build dependency audit: openSUSE Tumbleweed

Audit date: 2026-10-14. No package installation, FreeCAD configuration, or
FreeCAD compilation was performed.

The checked-out FreeCAD tree is 27.1.0-dev at `d732e2f13a`. It requires Qt 6
and defaults to bundled Coin/Pivy, PyCXX, Clipper2, and several other third-party
components. The host has GCC 16.2.1, CMake 4.4.3, Ninja 1.13.2, Python 3.13
development headers, Boost, OCCT 7.9.3 development files, Qt 6 base development
files, ICU, Eigen, yaml-cpp, zlib, zstd, FreeType, Fontconfig, OpenGL/X11
development files, SWIG, and all checked-out submodules.

The official FreeCAD Linux compilation guide lists openSUSE package names, but
its Tumbleweed section still describes the older Qt 5 stack. The checked-out
source is authoritative for this tree and explicitly calls `find_package(Qt6)`.

## Missing mandatory packages for a reduced Implicit-module build

- `libxerces-c-devel`
- `qt6-svg-devel`
- `qt6-tools-devel`
- `python313-pyside6-devel`
- `python313-shiboken6-devel`

Installation command, to be run only after approval:

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

## Proposed first configuration

After dependency approval and after adding the module skeleton, configure an
out-of-tree development build with Ninja and a constrained parallel job pool:

```sh
cmake -S FreeCAD -B build-freecad-implicit -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DBUILD_GUI=ON \
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
  -DENABLE_DEVELOPER_TESTS=OFF \
  -DFREECAD_USE_3DCONNEXION_LEGACY=OFF \
  -DFREECAD_PARALLEL_COMPILE_JOBS=2 \
  -DFREECAD_PARALLEL_LINK_JOBS=1
```

The host currently has about 45 GiB free and roughly 2 GiB readily available
RAM plus swap. Two compile jobs and one link job are therefore safer than using
all CPU cores. Configuration may reveal an additional package requirement due
to the development snapshot; if so, stop and report it rather than installing
anything automatically.

Official references:

- [FreeCAD compile-on-Linux guide](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Compile_on_Linux.md)
- [FreeCAD third-party libraries](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Third_Party_Libraries.md)
- [Current FreeCAD CMake build options](https://github.com/FreeCAD/FreeCAD/blob/main/cMake/FreeCAD_Helpers/InitializeFreeCADBuildOptions.cmake)

