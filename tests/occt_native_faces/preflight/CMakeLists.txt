# SPDX-License-Identifier: LGPL-2.1-or-later
cmake_minimum_required(VERSION 3.20)
project(FreeCADOCCTSourcePreflight LANGUAGES NONE)
if(NOT EXISTS "${IMPLICIT_FREECAD_SOURCE}/cMake/FindOCC.cmake")
    message(FATAL_ERROR "Pass the FreeCAD source directory")
endif()
list(PREPEND CMAKE_MODULE_PATH "${IMPLICIT_FREECAD_SOURCE}/cMake")
find_package(OCC REQUIRED)
