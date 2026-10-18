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
foreach(_smoke IN ITEMS native mesh linked cancellation ray geometry controlled chart scalar part_linked patch mixed_box mixed_mesh mixed_audit mixed_coverage mixed_regularity mixed_correspondence mixed_topology mixed_region_topology mixed_cap_attachment mixed_trim_attachment native_shell native_solid native_property native_reference native_element native_link placed_classification)
    add_test(NAME "implicit_occt8_${_smoke}"
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/${_smoke}_smoke.py")
    set_tests_properties("implicit_occt8_${_smoke}" PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit .*smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
endforeach()
add_test(NAME implicit_occt8_native_faces
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/native_face_smoke.py")
set_tests_properties(implicit_occt8_native_faces PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit OCCT face smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)

option(IMPLICIT_MIGRATION_GUI_TESTS "Test the built OCCT 8 GUI (requires DISPLAY or Xvfb)" OFF)
set(IMPLICIT_OCCT_NATIVE_WORKER "" CACHE FILEPATH "Built native OCCT worker for GUI recipe tests")
if(IMPLICIT_MIGRATION_GUI_TESTS)
    if(NOT EXISTS "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD"
       OR NOT EXISTS "${IMPLICIT_OCCT_NATIVE_WORKER}")
        message(FATAL_ERROR "Build the migration GUI and pass IMPLICIT_OCCT_NATIVE_WORKER")
    endif()
    foreach(_gui IN ITEMS runtime sync async boundary)
        if(_gui STREQUAL runtime)
            set(_script occt8_gui_runtime_smoke.py)
            set(_success "native implicit OCCT8 GUI runtime smoke test passed")
        elseif(_gui STREQUAL sync)
            set(_script gui_smoke.py)
            set(_success "native implicit GUI smoke test passed")
        elseif(_gui STREQUAL async)
            set(_script async_gui_smoke.py)
            set(_success "native implicit async GUI smoke test passed")
        else()
            set(_script native_boundary_gui_smoke.py)
            set(_success "native OCCT boundary GUI smoke test passed")
        endif()
        add_test(NAME "implicit_occt8_gui_${_gui}"
            COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
                --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/gui-${_gui}-user.cfg"
                --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/gui-${_gui}-system.cfg"
                "${_scripts}/${_script}")
        set_tests_properties("implicit_occt8_gui_${_gui}" PROPERTIES
            ENVIRONMENT "IMPLICIT_EXPECTED_FREECAD_BUILD=${IMPLICIT_MIGRATION_BUILD};IMPLICIT_EXPECTED_OCCT_LIBRARY_DIR=${IMPLICIT_OCCT_LIBRARY_DIR};IMPLICIT_TEST_PART_BRIDGE=1;IMPLICIT_OCCT_NATIVE_WORKER=${IMPLICIT_OCCT_NATIVE_WORKER}"
            PASS_REGULAR_EXPRESSION "${_success}"
            FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
            TIMEOUT 180)
        if(_gui STREQUAL runtime)
            set_tests_properties("implicit_occt8_gui_${_gui}" PROPERTIES
                FIXTURES_REQUIRED occt8_runtime FIXTURES_SETUP occt8_gui_runtime)
        else()
            set_tests_properties("implicit_occt8_gui_${_gui}" PROPERTIES
                FIXTURES_REQUIRED occt8_gui_runtime)
        endif()
    endforeach()
    add_test(NAME implicit_occt8_gui_native_faces
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/native-face-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/native-face-system.cfg"
            "${_scripts}/native_face_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_native_faces PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit in-process face GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_mixed_box
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-box-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-box-system.cfg"
            "${_scripts}/mixed_box_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_mixed_box PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit mixed box GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_mixed_editor
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-editor-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-editor-system.cfg"
            "${_scripts}/mixed_editor_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_mixed_editor PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit mixed editor GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_mixed_surface
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-surface-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-surface-system.cfg"
            "${_scripts}/mixed_surface_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_mixed_surface PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit mixed surface GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    foreach(_demo IN ITEMS graph live)
        set(_demo_flags "")
        if(_demo STREQUAL live)
            set(_demo_flags --live-part --background)
            set(_success "live Part implicit demo verified")
        else()
            set(_success "interactive native demo verified")
        endif()
        add_test(NAME "implicit_occt8_demo_${_demo}"
            COMMAND bash "${_root}/tools/launch_demo.sh" --occt8 ${_demo_flags}
                --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/demo-${_demo}-user.cfg"
                --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/demo-${_demo}-system.cfg")
        set_tests_properties("implicit_occt8_demo_${_demo}" PROPERTIES
            ENVIRONMENT "IMPLICIT_DEMO_CHECK=1;IMPLICIT_OCCT8_GUI=${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD"
            PASS_REGULAR_EXPRESSION "${_success}"
            FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
            FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    endforeach()
endif()
