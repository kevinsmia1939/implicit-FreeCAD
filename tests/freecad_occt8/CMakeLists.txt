# SPDX-License-Identifier: LGPL-2.1-or-later
cmake_minimum_required(VERSION 3.20)
project(ImplicitFreeCADOCCT8MigrationTests LANGUAGES NONE)

# Keep the working OCCT 7 umbrella tests untouched. This harness runs only the
# migration binary, including an actual loaded-library audit before smoke tests.
get_filename_component(_root "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(IMPLICIT_MIGRATION_BUILD "" CACHE PATH "Separate FreeCAD OCCT 8 build directory")
set(IMPLICIT_OCCT_LIBRARY_DIR "" CACHE PATH "Expected OCCT 8 shared library directory")
if(NOT EXISTS "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd"
   OR NOT IS_DIRECTORY "${IMPLICIT_OCCT_LIBRARY_DIR}")
    message(FATAL_ERROR "Pass the migration build and expected OCCT 8 library directory")
endif()
set(_scripts "${_root}/FreeCAD/src/Mod/Implicit/Tests")
enable_testing()
add_test(NAME implicit_occt8_runtime
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/occt8_runtime_smoke.py")
set_tests_properties(implicit_occt8_runtime PROPERTIES
    ENVIRONMENT "IMPLICIT_EXPECTED_FREECAD_BUILD=${IMPLICIT_MIGRATION_BUILD};IMPLICIT_EXPECTED_OCCT_LIBRARY_DIR=${IMPLICIT_OCCT_LIBRARY_DIR}"
    PASS_REGULAR_EXPRESSION "native implicit OCCT8 runtime smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_SETUP occt8_runtime TIMEOUT 120)
foreach(_smoke IN ITEMS native mesh linked cancellation ray geometry controlled chart scalar part_linked patch)
    add_test(NAME "implicit_occt8_${_smoke}"
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/${_smoke}_smoke.py")
    set_tests_properties("implicit_occt8_${_smoke}" PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit .*smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
endforeach()
