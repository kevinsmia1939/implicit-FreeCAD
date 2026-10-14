cmake_minimum_required(VERSION 3.20)
project(ImplicitFreeCADPrototype VERSION 0.1.0 LANGUAGES CXX)

option(IMPLICIT_BUILD_TESTS "Build the standalone geometry tests" ON)
option(IMPLICIT_BUILD_BENCHMARK "Build the mesh benchmark" ON)
option(IMPLICIT_BUILD_FREECAD_TESTS "Run addon tests when FreeCADCmd is available" ON)
option(IMPLICIT_BUILD_OCCT_PROBE "Build the explicit mesh-only OCCT bridge probe" ON)
set(IMPLICIT_NATIVE_FREECADCMD "" CACHE FILEPATH "FreeCADCmd built with the native Implicit module")

add_library(implicit_geometry
    src/ImplicitSurface.cpp
    src/GyroidSurface.cpp
    src/MarchingTetrahedra.cpp
    src/PlaneIntersection.cpp
)
target_include_directories(implicit_geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(implicit_geometry PUBLIC cxx_std_17)
target_compile_options(implicit_geometry PRIVATE -Wall -Wextra -Wpedantic)

if(IMPLICIT_BUILD_TESTS)
    enable_testing()
    if(IMPLICIT_NATIVE_FREECADCMD)
        if(NOT EXISTS "${IMPLICIT_NATIVE_FREECADCMD}")
            message(FATAL_ERROR "IMPLICIT_NATIVE_FREECADCMD does not exist")
        endif()
        foreach(native_test IN ITEMS native_smoke mesh_smoke)
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

if(IMPLICIT_BUILD_BENCHMARK)
    add_executable(implicit_benchmark benchmark/benchmark.cpp)
    target_link_libraries(implicit_benchmark PRIVATE implicit_geometry)
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

