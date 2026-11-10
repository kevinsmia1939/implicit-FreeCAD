# Cooperative cylinder execution control

The native cylinder requalification and preview paths honor
`BRepClass3d_ImplicitWork::Scope`. The control is installed only on the executing
thread, restored when the scope exits (including exceptions), and never stored
in geometry or saved records. Without a scope, existing synchronous APIs keep
their behavior. Existing settings callbacks remain supported.

OCCT cylinder geometry/volume loops cooperate with the shared control. Stored
record parsing and qualification checks do too; cancellation propagates as
`Standard_AbortiveTransaction` through coherence, masked seam, coverage, cap,
side and combined geometry checks instead of becoming a numerical failure
certificate. A cancelled read preserves its destination graph. Requalification
uses private results and does not promote flags on the raw loaded owner.

The FreeCAD preview reports Meshing, Coverage and ContactAudit phases through
the control. Root generation, incidence/normals/upload checks, whole-surface
distance proofs and exact contact auditing have checkpoints. Contact auditing
accepts an optional native callback, including during coordinate staging,
triangle checks, sorting, the bounded broad phase and before its final verified
flag. The callback is not retained. The existing 0.1 mm target, numerical proofs,
20-million broad-phase budget and native identities remain unchanged.

Native boundary sampling checks cancellation while preparing private buffers and
adjusting indices. It reserves caller capacity and checks once more before
appending points/lines and swapping edge metadata. No callback or allocation
occurs during that final publication, so cancellation cannot expose a partial
boundary cache. The complete preview also checks immediately before returning;
late cancellation discards its private result.

This is execution control for the existing restored/full-disk cylinder path.
The GUI still calls it synchronously. Worker isolation, guarded GUI publication,
coalescing, cancellation on hide/delete/close, live source editing and broader
kernel support remain separate work. Conventional OCCT calls finish their
current call before the next cooperative checkpoint.

The explicit `ImplicitCylinderWorkTests` target links the actual native module
and tests cancellation at early, middle and final checkpoints during restored
read/requalification/volume, preview phases and boundary sampling. It verifies
source-record immutability, raw-owner flags, unchanged destination buffers and
successful work after cancelled scopes. The standalone contact tests also cover
callback cancellation before any certificate is returned.

Build with the existing matched OCCT/FreeCAD build directories (sequentially,
two compile jobs):

```sh
cmake --build build-occt-implicit --target TKTopAlgo --parallel 2
cmake --build build-freecad-occt8 --target ImplicitCylinderWorkTests ImplicitScripts --parallel 2
cmake --build build-freecad-occt8/migration-tests --target implicit_preview_contact_tests --parallel 2
ctest --test-dir build-freecad-occt8/migration-tests -R '^(implicit_preview_contacts|implicit_occt8_cylinder_work)$' --output-on-failure
```

The execution test is excluded from normal application builds and installation;
build it explicitly before running its migration test.
