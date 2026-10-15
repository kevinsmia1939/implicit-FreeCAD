# Geometric accuracy target

The user selected **0.1 mm geometric tolerance** for the intended manufacturing
workflow and explicitly requested OCCT-native implicit faces. A minimum feature
size has not been specified. This target does not imply that current prototypes
already satisfy a whole-model accuracy or manufacturing acceptance certificate.

- FreeCAD internal length coordinates are treated as millimetres for this
  project. OCCT itself is unit-agnostic; the native-face conformance fixtures
  use the same millimetre convention and a face tolerance of `0.1`.
- Native face tolerance and ray-bracket width are geometric/model-length
  quantities. Tested isolated sign brackets refined to width <=0.1 mm put the
  midpoint within <=0.05 mm of some root in the bracket, subject to continuity
  and ordinary floating-point arithmetic. This does not prove root completeness,
  topology or whole-surface Hausdorff error.
- Sphere signed-distance values have length units. Raw gyroid values and
  level-set widths are dimensionless. **Do not substitute 0.1 for a gyroid field
  tolerance or call a level-set band a 0.1 mm wall.**
- `Implicit::Feature.Tolerance` remains a field classification tolerance;
  existing projection/controlled-extraction/patch field tolerances keep their
  documented meanings. They have not been silently changed to 0.1.
- Fitted-face field/normal sample checks and field-wide Lipschitz bounds do not
  establish a 0.1 mm geometric approximation. A manufacturing bridge must measure
  or conservatively bound geometric error separately, validate boundaries,
  topology and solids, and explicitly reject unmet targets.

The first native OCCT-face layer stores and evaluates the mathematical field
directly; it is not a mesh or B-spline approximation. The new ray tests exercise
the 0.1 mm bracket policy. Generic BOP, trimming, persistence and exchange support
remain separate milestones, not implied by native geometry storage.
