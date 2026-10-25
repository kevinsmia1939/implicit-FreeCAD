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
add_test(NAME implicit_occt8_cylinder_caps
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/cylinder_caps_smoke.py")
set_tests_properties(implicit_occt8_cylinder_caps PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit cylinder caps smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_cylinder_section
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/cylinder_section_smoke.py")
set_tests_properties(implicit_occt8_cylinder_section PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit cylinder section smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_native_face_snap
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/native_face_snap_smoke.py")
set_tests_properties(implicit_occt8_native_face_snap PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit face snapping smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_native_snap
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/native_snap_smoke.py")
set_tests_properties(implicit_occt8_native_snap PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit snapping smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_operand_instances
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/operand_instances_smoke.py")
set_tests_properties(implicit_occt8_operand_instances PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit operand instances smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 240)
add_test(NAME implicit_occt8_boundary_jobs
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/boundary_jobs_smoke.py")
set_tests_properties(implicit_occt8_boundary_jobs PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit boundary jobs smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_boundary_job_shutdown
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/boundary_job_shutdown_smoke.py")
set_tests_properties(implicit_occt8_boundary_job_shutdown PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit boundary job shutdown smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 45)
add_test(NAME implicit_occt8_surface_jobs
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/surface_jobs_smoke.py")
set_tests_properties(implicit_occt8_surface_jobs PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit surface jobs smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 240)
add_test(NAME implicit_occt8_surface_job_shutdown
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/surface_job_shutdown_smoke.py")
set_tests_properties(implicit_occt8_surface_job_shutdown PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit surface job shutdown smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 30)
add_test(NAME implicit_occt8_solid_batch
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/solid_batch_smoke.py")
set_tests_properties(implicit_occt8_solid_batch PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit solid batch smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_solid_job_shutdown
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/solid_job_shutdown_smoke.py")
set_tests_properties(implicit_occt8_solid_job_shutdown PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit solid job shutdown smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 20)
add_test(NAME implicit_occt8_solid_jobs
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/solid_jobs_smoke.py")
set_tests_properties(implicit_occt8_solid_jobs PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit solid jobs smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 180)
add_test(NAME implicit_occt8_solid_job_publication
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/solid_job_publication_smoke.py")
set_tests_properties(implicit_occt8_solid_job_publication PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit solid job publication smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 120)
add_test(NAME implicit_occt8_source_parameters
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/source_parameters_smoke.py")
set_tests_properties(implicit_occt8_source_parameters PROPERTIES
    PASS_REGULAR_EXPRESSION "native implicit source parameters smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_REQUIRED occt8_runtime TIMEOUT 180)
add_test(NAME implicit_occt8_runtime
    COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCADCmd" "${_scripts}/occt8_runtime_smoke.py")
set_tests_properties(implicit_occt8_runtime PROPERTIES
    ENVIRONMENT "IMPLICIT_EXPECTED_FREECAD_BUILD=${IMPLICIT_MIGRATION_BUILD};IMPLICIT_EXPECTED_OCCT_LIBRARY_DIR=${IMPLICIT_OCCT_LIBRARY_DIR}"
    PASS_REGULAR_EXPRESSION "native implicit OCCT8 runtime smoke test passed"
    FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
    FIXTURES_SETUP occt8_runtime TIMEOUT 120)
foreach(_smoke IN ITEMS native mesh linked cancellation ray geometry controlled chart scalar part_linked patch mixed_box mixed_mesh mixed_audit mixed_coverage mixed_regularity corner_germ lattice_section corner_boundary corner_persistence corner_classification corner_topology corner_region corner_attachment corner_trim corner_shell corner_cap_geometry corner_solid corner_mesh corner_coverage checked_display solid_settings mixed_correspondence mixed_topology mixed_region_topology mixed_cap_attachment mixed_trim_attachment native_shell native_solid native_solid_io native_solid_property live_solid native_property native_reference native_element native_link placed_classification native_topology)
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
option(IMPLICIT_MIGRATION_DRAFT_TESTS "Test built Draft snapping integration (requires Draft and TechDrawGui)" OFF)
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
    add_test(NAME implicit_occt8_gui_live_solid
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/live-solid-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/live-solid-system.cfg"
            "${_scripts}/live_solid_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_live_solid PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit live solid GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_corner_solid
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/corner-solid-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/corner-solid-system.cfg"
            "${_scripts}/corner_solid_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_corner_solid PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit corner solid GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_checked_display
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/checked-display-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/checked-display-system.cfg"
            "${_scripts}/checked_display_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_checked_display PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit checked display GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_async_checked_display
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/async-checked-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/async-checked-system.cfg"
            "${_scripts}/async_checked_display_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_async_checked_display PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit async checked display GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    if(IMPLICIT_MIGRATION_DRAFT_TESTS)
        if(NOT EXISTS "${IMPLICIT_MIGRATION_BUILD}/Mod/Draft/draftguitools/gui_snapper.py"
           OR NOT EXISTS "${IMPLICIT_MIGRATION_BUILD}/Ext/lazy_loader/lazy_loader.py")
            message(FATAL_ERROR "Build Draft and its bundled lazy_loader before enabling Draft GUI checks")
        endif()
        add_test(NAME implicit_occt8_gui_draft_face_snap
            COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
                --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/draft-face-user.cfg"
                --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/draft-face-system.cfg"
                "${_scripts}/draft_face_snap_gui_smoke.py")
        set_tests_properties(implicit_occt8_gui_draft_face_snap PROPERTIES
            PASS_REGULAR_EXPRESSION "native implicit Draft face snapping GUI smoke test passed"
            FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
            FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
        add_test(NAME implicit_occt8_gui_draft_snap
            COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
                --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/draft-snap-user.cfg"
                --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/draft-snap-system.cfg"
                "${_scripts}/draft_snap_gui_smoke.py")
        set_tests_properties(implicit_occt8_gui_draft_snap PROPERTIES
            PASS_REGULAR_EXPRESSION "native implicit Draft snapping GUI smoke test passed"
            FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
            FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    endif()
    add_test(NAME implicit_occt8_gui_operand_instances
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/instances-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/instances-system.cfg"
            "${_scripts}/operand_instances_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_operand_instances PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit operand instances GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 300)
    add_test(NAME implicit_occt8_gui_build_feedback
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/feedback-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/feedback-system.cfg"
            "${_scripts}/build_feedback_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_build_feedback PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit build feedback GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_creation_defaults
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/creation-default-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/creation-default-system.cfg"
            "${_scripts}/gui_creation_defaults_smoke.py")
    set_tests_properties(implicit_occt8_gui_creation_defaults PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit GUI creation defaults smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_task_source_jobs
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/task-source-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/task-source-system.cfg"
            "${_scripts}/task_source_jobs_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_task_source_jobs PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit task source jobs GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_mixed_recompute
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-recompute-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/mixed-recompute-system.cfg"
            "${_scripts}/mixed_recompute_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_mixed_recompute PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit source recompute GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_solid_recompute
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-recompute-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-recompute-system.cfg"
            "${_scripts}/solid_recompute_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_solid_recompute PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit property recompute GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_solid_editor
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-editor-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-editor-system.cfg"
            "${_scripts}/solid_editor_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_solid_editor PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit solid editor GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_source_parameters
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/source-parameters-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/source-parameters-system.cfg"
            "${_scripts}/source_parameters_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_source_parameters PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit source parameters GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_gyroid_create
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/gyroid-create-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/gyroid-create-system.cfg"
            "${_scripts}/gyroid_create_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_gyroid_create PROPERTIES
        PASS_REGULAR_EXPRESSION "native gyroid creation GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_part_boolean
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/part-boolean-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/part-boolean-system.cfg"
            "${_scripts}/part_boolean_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_part_boolean PROPERTIES
        PASS_REGULAR_EXPRESSION "native Part Boolean GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 240)
    add_test(NAME implicit_occt8_gui_solid_create
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-create-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-create-system.cfg"
            "${_scripts}/solid_create_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_solid_create PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit solid creation GUI smoke test passed"
        FAIL_REGULAR_EXPRESSION "Traceback|Exception while processing file"
        FIXTURES_REQUIRED occt8_gui_runtime TIMEOUT 180)
    add_test(NAME implicit_occt8_gui_solid_batch
        COMMAND "${IMPLICIT_MIGRATION_BUILD}/bin/FreeCAD" --safe-mode
            --user-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-batch-user.cfg"
            --system-cfg "${CMAKE_CURRENT_BINARY_DIR}/solid-batch-system.cfg"
            "${_scripts}/solid_batch_gui_smoke.py")
    set_tests_properties(implicit_occt8_gui_solid_batch PROPERTIES
        PASS_REGULAR_EXPRESSION "native implicit solid batch GUI smoke test passed"
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
