# Continuing implicit-FreeCAD

Read `docs/HANDOFF.md` before implementation. It is the current development
handoff, including build commands, architecture decisions, progress, toolchain,
known limitations, and the immediate next milestone. Consult `instructions.txt`
for the original brief and `docs/core-integration-plan.md` for the native plan.

- This root is the development umbrella. Native FreeCAD changes belong in the
  `FreeCAD` submodule on `feature/native-implicit`; standalone/addon/docs changes
  belong in this root repository. OCCT extensions belong on
  `feature/implicit-distance`; the user explicitly authorized changes in both
  FreeCAD and OCCT core source.
- Keep field definitions authoritative and generated meshes transient.
- Do not put arbitrary fields into `Part::Feature::Shape` or claim mesh faces
  are native implicit B-Reps. Record approximation and tolerance policies.
- Reuse installed dependencies and the existing build directories. Do not
  install packages or modify system libraries automatically. User approval for
  the existing native build and dependency installation has already been given.
- Build the smallest relevant target. This host has limited available memory;
  use two compile jobs and one link job for FreeCAD.
- Test native changes with the fork's `build-freecad-implicit/bin/FreeCADCmd`.
  The system `FreeCADCmd` does not contain the new native module.
- Update the handoff and roadmap when completing a milestone. Distinguish work
  implemented in the native module from the older standalone/Python prototypes.
- When publishing native changes, push the submodule branch first, then commit
  and push the parent gitlink. Preserve upstream licensing and history.
- The OCCT reference is 8.1 development, while the installed library is 7.9.3.
  Compatibility builds stage only the new extension header and use installed
  headers for existing OCCT types. Never mix general 8.1 headers with 7.9 libs.
