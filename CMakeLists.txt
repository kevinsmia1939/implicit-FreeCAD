cmake_minimum_required(VERSION 3.20)
project(ImplicitFreeCADPrototype VERSION 0.1.0 LANGUAGES CXX)

option(IMPLICIT_BUILD_TESTS "Build the standalone geometry tests" ON)
option(IMPLICIT_BUILD_BENCHMARK "Build the mesh benchmark" ON)
option(IMPLICIT_BUILD_FREECAD_TESTS "Run addon tests when FreeCADCmd is available" ON)
option(IMPLICIT_BUILD_OCCT_PROBE "Build the explicit mesh-only OCCT bridge probe" ON)
option(IMPLICIT_BUILD_OCCT_DISTANCE "Build OCCT fork signed-distance extension and tests" OFF)
set(IMPLICIT_NATIVE_FREECADCMD "" CACHE FILEPATH "FreeCADCmd built with the native Implicit module")
set(IMPLICIT_NATIVE_FREECADGUI "" CACHE FILEPATH "FreeCAD GUI built with ImplicitGui")
option(IMPLICIT_NATIVE_PART_TESTS "Test live Part inputs (requires built native Part bridge)" OFF)

add_library(implicit_geometry
    src/ImplicitSurface.cpp
    src/GyroidSurface.cpp
    src/MarchingTetrahedra.cpp
    src/PlaneIntersection.cpp
    src/CylinderIntersection.cpp
)
target_include_directories(implicit_geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(implicit_geometry PUBLIC cxx_std_17)
target_compile_options(implicit_geometry PRIVATE -Wall -Wextra -Wpedantic)

if(IMPLICIT_BUILD_TESTS)
    enable_testing()
    if(IMPLICIT_NATIVE_FREECADGUI)
        if(NOT EXISTS "${IMPLICIT_NATIVE_FREECADGUI}")
            message(FATAL_ERROR "IMPLICIT_NATIVE_FREECADGUI does not exist")
        endif()
        add_test(NAME implicit_native_gui COMMAND "${IMPLICIT_NATIVE_FREECADGUI}"
            --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/implicit-gui-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/implicit-gui-system.cfg"
            "${CMAKE_CURRENT_SOURCE_DIR}/FreeCAD/src/Mod/Implicit/Tests/gui_smoke.py")
        set_tests_properties(implicit_native_gui PROPERTIES
            PASS_REGULAR_EXPRESSION "native implicit GUI smoke test passed"
            FAIL_REGULAR_EXPRESSION "Traceback" TIMEOUT 180)
        if(IMPLICIT_NATIVE_PART_TESTS)
            set_tests_properties(implicit_native_gui PROPERTIES
                ENVIRONMENT "IMPLICIT_TEST_PART_BRIDGE=1")
        endif()
        add_test(NAME implicit_async_gui COMMAND "${IMPLICIT_NATIVE_FREECADGUI}"
            --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/implicit-async-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/implicit-async-system.cfg"
            "${CMAKE_CURRENT_SOURCE_DIR}/FreeCAD/src/Mod/Implicit/Tests/async_gui_smoke.py")
        set_tests_properties(implicit_async_gui PROPERTIES
            PASS_REGULAR_EXPRESSION "native implicit async GUI smoke test passed"
            FAIL_REGULAR_EXPRESSION "Traceback" TIMEOUT 180)
        if(IMPLICIT_NATIVE_PART_TESTS)
            set_tests_properties(implicit_async_gui PROPERTIES
                ENVIRONMENT "IMPLICIT_TEST_PART_BRIDGE=1")
        endif()
    endif()
    if(IMPLICIT_NATIVE_FREECADCMD)
        if(NOT EXISTS "${IMPLICIT_NATIVE_FREECADCMD}")
            message(FATAL_ERROR "IMPLICIT_NATIVE_FREECADCMD does not exist")
        endif()
        set(_native_tests native_smoke mesh_smoke linked_smoke cancellation_smoke)
        if(IMPLICIT_NATIVE_PART_TESTS)
            list(APPEND _native_tests part_linked_smoke)
        endif()
        foreach(native_test IN LISTS _native_tests)
            add_test(NAME implicit_${native_test}
                COMMAND "${IMPLICIT_NATIVE_FREECADCMD}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/FreeCAD/src/Mod/Implicit/Tests/${native_test}.py")
            set_tests_properties(implicit_${native_test} PROPERTIES
                PASS_REGULAR_EXPRESSION "native implicit .*smoke test passed"
                FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
                TIMEOUT 120)
        endforeach()
    endif()
    add_executable(implicit_tests tests/test_geometry.cpp)
    target_link_libraries(implicit_tests PRIVATE implicit_geometry)
    add_test(NAME implicit_geometry_tests COMMAND implicit_tests)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME implicit_python_core_tests
                 COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_python_core.py)
    endif()
    if(IMPLICIT_BUILD_FREECAD_TESTS)
        find_program(FREECADCMD_EXECUTABLE NAMES FreeCADCmd freecadcmd)
        if(FREECADCMD_EXECUTABLE)
            add_test(NAME implicit_freecad_smoke
                     COMMAND ${FREECADCMD_EXECUTABLE}
                             -P ${CMAKE_CURRENT_SOURCE_DIR}/addon
                             ${CMAKE_CURRENT_SOURCE_DIR}/tests/freecad_smoke.py)
            set_tests_properties(implicit_freecad_smoke PROPERTIES TIMEOUT 60)
        endif()
    endif()
endif()

if(IMPLICIT_BUILD_OCCT_DISTANCE)
    find_package(OpenCASCADE REQUIRED COMPONENTS FoundationClasses ModelingData ModelingAlgorithms)
    set(_occt_field_dir "${CMAKE_CURRENT_SOURCE_DIR}/OCCT/src/ModelingAlgorithms/TKTopAlgo/BRepClass3d")
    add_library(occt_implicit_distance STATIC "${_occt_field_dir}/BRepClass3d_SignedDistance.cxx")
    target_compile_features(occt_implicit_distance PUBLIC cxx_std_17)
    # Stage only the new header: never mix the fork's OCCT 8 headers with
    # installed OCCT 7 libraries and their different ABI.
    configure_file("${_occt_field_dir}/BRepClass3d_SignedDistance.hxx"
        "${CMAKE_CURRENT_BINARY_DIR}/occt-implicit-include/BRepClass3d_SignedDistance.hxx" COPYONLY)
    target_include_directories(occt_implicit_distance PUBLIC
        "${CMAKE_CURRENT_BINARY_DIR}/occt-implicit-include")
    target_include_directories(occt_implicit_distance SYSTEM PUBLIC ${OpenCASCADE_INCLUDE_DIR})
    target_link_libraries(occt_implicit_distance PUBLIC
        ${OpenCASCADE_FoundationClasses_LIBRARIES} ${OpenCASCADE_ModelingData_LIBRARIES}
        ${OpenCASCADE_ModelingAlgorithms_LIBRARIES})
    if(IMPLICIT_BUILD_TESTS)
        add_executable(implicit_occt_distance_tests tests/test_occt_distance.cpp)
        target_link_libraries(implicit_occt_distance_tests PRIVATE occt_implicit_distance)
        add_test(NAME implicit_occt_signed_distance COMMAND implicit_occt_distance_tests
            "${CMAKE_CURRENT_BINARY_DIR}/implicit-box.brep")
        set_tests_properties(implicit_occt_signed_distance PROPERTIES FIXTURES_SETUP brep_fixture)
        find_package(GTest QUIET)
        if(GTest_FOUND)
            add_executable(implicit_occt_gtests
                "${CMAKE_CURRENT_SOURCE_DIR}/OCCT/src/ModelingAlgorithms/TKTopAlgo/GTests/BRepClass3d_SignedDistance_Test.cxx")
            target_link_libraries(implicit_occt_gtests PRIVATE occt_implicit_distance GTest::gtest_main)
            add_test(NAME implicit_occt_gtests COMMAND implicit_occt_gtests)
        endif()
        if(IMPLICIT_NATIVE_FREECADCMD)
            add_test(NAME implicit_mixed_smoke COMMAND "${IMPLICIT_NATIVE_FREECADCMD}"
                "${CMAKE_CURRENT_SOURCE_DIR}/FreeCAD/src/Mod/Implicit/Tests/mixed_smoke.py")
            set_tests_properties(implicit_mixed_smoke PROPERTIES FIXTURES_REQUIRED brep_fixture
                ENVIRONMENT "IMPLICIT_BREP_FIXTURE=${CMAKE_CURRENT_BINARY_DIR}/implicit-box.brep"
                PASS_REGULAR_EXPRESSION "native implicit mixed smoke test passed"
                FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file" TIMEOUT 180)
        endif()
    endif()
endif()

if(IMPLICIT_BUILD_BENCHMARK)
    add_executable(implicit_benchmark benchmark/benchmark.cpp)
    target_link_libraries(implicit_benchmark PRIVATE implicit_geometry)
    add_executable(implicit_intersection_benchmark benchmark/intersections.cpp)
    target_link_libraries(implicit_intersection_benchmark PRIVATE implicit_geometry)
endif()

if(IMPLICIT_BUILD_OCCT_PROBE)
    find_package(OpenCASCADE QUIET COMPONENTS FoundationClasses ModelingData)
    if(OpenCASCADE_FOUND)
        add_executable(implicit_occt_bridge_probe tests/occt_bridge_probe.cpp)
        target_include_directories(implicit_occt_bridge_probe PRIVATE ${OpenCASCADE_INCLUDE_DIR})
        target_link_libraries(implicit_occt_bridge_probe PRIVATE
            implicit_geometry
            ${OpenCASCADE_FoundationClasses_LIBRARIES}
            ${OpenCASCADE_ModelingData_LIBRARIES}
        )
        if(IMPLICIT_BUILD_TESTS)
            add_test(NAME implicit_occt_bridge_probe COMMAND implicit_occt_bridge_probe)
        endif()
    endif()
endif()

