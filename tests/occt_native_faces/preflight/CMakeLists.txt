# SPDX-License-Identifier: LGPL-2.1-or-later
cmake_minimum_required(VERSION 3.20)
project(FreeCADOCCTSourcePreflight LANGUAGES NONE)
if(NOT EXISTS "${IMPLICIT_FREECAD_SOURCE}/cMake/FindOCC.cmake")
    message(FATAL_ERROR "Pass the FreeCAD source directory")
endif()
list(PREPEND CMAKE_MODULE_PATH "${IMPLICIT_FREECAD_SOURCE}/cMake")
find_package(OCC REQUIRED)

option(IMPLICIT_OCCT_HEADER_PROBE "Compile the source-build compatibility header regression" OFF)
if(IMPLICIT_OCCT_HEADER_PROBE)
    enable_language(CXX)
    add_executable(implicit_occt_header_probe headers.cxx)
    target_compile_features(implicit_occt_header_probe PRIVATE cxx_std_17)
    target_include_directories(implicit_occt_header_probe SYSTEM PRIVATE "${OCC_INCLUDE_DIR}")
    # The aliases must propagate through toolkit dependencies, not just a
    # one-off include flag on the FreeCAD Part target.
    target_link_libraries(implicit_occt_header_probe PRIVATE TKBRep)
    enable_testing()
    add_test(NAME implicit_occt_header_probe COMMAND implicit_occt_header_probe)
endif()
