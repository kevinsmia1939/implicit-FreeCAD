# SPDX-License-Identifier: LGPL-2.1-or-later
cmake_minimum_required(VERSION 3.20)
project(ImplicitOCCTNativeFaces LANGUAGES CXX)

# Deliberately separate from the umbrella/FreeCAD OCCT 7.9 compatibility build.
set(IMPLICIT_OCCT_NATIVE_BUILD "" CACHE PATH "Configured OCCT 8.1 source build")
if(NOT EXISTS "${IMPLICIT_OCCT_NATIVE_BUILD}/OpenCASCADETargets.cmake"
   OR NOT EXISTS "${IMPLICIT_OCCT_NATIVE_BUILD}/include/opencascade/BRep_TImplicitFace.hxx")
    message(FATAL_ERROR "Configure the OCCT fork build including native implicit faces first")
endif()
include("${IMPLICIT_OCCT_NATIVE_BUILD}/OpenCASCADETargets.cmake")
find_package(GTest REQUIRED)
add_executable(implicit_occt_native_face_tests
    ../../OCCT/src/ModelingAlgorithms/TKMesh/GTests/BRepMesh_ImplicitBoundary_Test.cxx
    ../../OCCT/src/ModelingAlgorithms/TKTopAlgo/GTests/BRepClass3d_ImplicitBox_Test.cxx
    ../../OCCT/src/ModelingData/TKBRep/GTests/BRep_ImplicitPlaneSection_Test.cxx
    ../../OCCT/src/ModelingData/TKBRep/GTests/BRep_ImplicitSphereOps_Test.cxx
    ../../OCCT/src/ModelingData/TKBRep/GTests/BRep_ImplicitFace_Test.cxx)
target_compile_features(implicit_occt_native_face_tests PRIVATE cxx_std_17)
target_include_directories(implicit_occt_native_face_tests SYSTEM PRIVATE
    "${IMPLICIT_OCCT_NATIVE_BUILD}/include/opencascade")
target_link_libraries(implicit_occt_native_face_tests PRIVATE TKPrim TKMesh GTest::gtest_main)
add_executable(implicit_occt_native_worker ../../tools/occt_native_worker.cpp)
target_compile_features(implicit_occt_native_worker PRIVATE cxx_std_17)
target_include_directories(implicit_occt_native_worker SYSTEM PRIVATE
    "${IMPLICIT_OCCT_NATIVE_BUILD}/include/opencascade")
target_link_libraries(implicit_occt_native_worker PRIVATE TKBRep)
enable_testing()
add_test(NAME implicit_occt_native_faces COMMAND implicit_occt_native_face_tests)
set_tests_properties(implicit_occt_native_faces PROPERTIES TIMEOUT 120)
