# Checked local gyroid corner models

This kernel increment supplies a necessary local model for native trimming of
the unchanged default gyroid: period 10 mm, iso 0, domain `[0,10]^3`. It does
**not** enable that primitive's full native trim/solid workflow yet. Existing
strict regularity, trimming, correspondence and solid-validation gates are
unchanged. No field phase, level or domain is shifted to avoid zeros.

## Why the corners matter

At all eight default corners, the mathematical field is zero and its gradient
has three positive coordinate components. Relative to the inward box directions:

| Corner | Local field behavior | Closed inequalities | Regularized material |
|---|---|---|---|
| `(0,0,0)` | F strictly positive away from the corner | Both operations retain the zero point | Common drops the isolated contact; box-minus-field retains it |
| `(10,10,10)` | F strictly negative away from the corner | Both operations retain the zero point | Common retains it; box-minus-field drops the isolated contact |
| Other six | Positive and negative material cones meet | Both retain the corner | Both retain it as closure of interior material |

Here regularization means closure of the three-dimensional interior of
`box AND F<=0` or `box AND F>=0`, not discarding small features by tolerance.
These distinctions are returned explicitly. They do not change the source field,
the existing mathematical-region classifier or current Boolean semantics.
Future construction must record and consume a deliberate policy; it must not
silently remove the isolated contacts or claim they are ordinary solid faces.

## Exact lattice recognition

`Geom_ImplicitGyroid::LatticeJet` recognizes a coordinate only when the stored
binary-rational numbers satisfy `4*x = n*period` exactly for a bounded integer n.
It first normalizes by an exact power of two, then uses an FMA residual. A nearby
coordinate is not accepted merely because division rounds near an integer;
even a nextafter displacement rejects. Normalization avoids underflow falsely
turning a nonzero residual into an identity. Failed recognition leaves output
parameters unchanged.

The quarter-period sine/cosine factors are then integers. Gradient coefficients
use the existing numerical wave number and their error reserves. This predicate
does not change general Value/Gradient evaluation or promise bitwise zero from
the ordinary trigonometric evaluator. A zero is accepted only for a recognized
lattice point with an exactly matching level, not a small floating residual.

## Whole-neighborhood bounds

`BRepClass3d_ImplicitBox::CheckCornerGerms` examines the computational box domain
of an identity-placed native gyroid face; stored trim wires are not inspected.
For each corner it either:

- excludes F=0 throughout a small inward box using a field/Lipschitz margin;
- recognizes an exact zero and bounds each inward coordinate derivative away
  from zero throughout the neighborhood using the global Hessian bound;
- reports unresolved for an unrecognized near-zero, tangent coordinate direction
  or insufficient coordinate/derivative precision.

Uniform derivative signs distinguish positive/negative cones from split cones.
They also support local field-sign bounds by integration along rays from the
known zero anchor. Radius is a maximum neighborhood edge length in model units,
**not** an approximation error, distance tolerance or minimum-feature certificate.
Neighborhoods stay within the original box, are disjoint across corners, and
endpoints round toward the anchor when needed to respect the requested radius.
Bounds use engineering floating-point reserves, not directed intervals.

Exact identity matrices are accepted even when primitive I/O reconstructs a
nonempty location chain. No topology is mutated to normalize it; even a 1e-18
translation rejects. General placements/custom fields remain unsupported here.

## FreeCAD OCCT-8 API

```python
import ImplicitOCCT as native
face = native.gyroid(10, 0, (0,0,0), (10,10,10))
report = native.check_box_corners(face, .1)
assert report["corner_germs_resolved"]
assert report["exact_zero_count"] == 8 and report["split_count"] == 6
query = native.classify_box_corner(face, 0, (0,0,0), "common", .1)
assert query["resolved"] and query["on_field"]
assert query["closed_retained"] and not query["regularized_retained"]
```

Corner indices are zero-based high-side bit masks: X=1, Y=2, Z=4, **not** native
VertexN numbering. Reports include anchors, neighborhoods, inward derivative
bounds, recognized-zero status, radius and reasons. ExactZeroCount counts known
identities even when a tangent direction prevents a resolved local model.

Local membership is NOT whole-solid physical-distance classification. Outside
the checked neighborhood or at an uncertain local field sign it returns
`resolved=False`; default false membership flags must not be interpreted as a
proof of exclusion. No new native shape, validity flag or topology payload is
published. Reports are derived afresh from owned math, including after primitive
round-trip or document close. Existing `classify_mixed` remains separate.

## Remaining integration

Next extend plane-rectangle tracing and box-edge roots to consume exact corner
identities, sharing the original corner vertex rather than inventing one root
per edge or coordinate-welding. Cap/implicit cycles must use the local material
policy consistently. Project each checked derivative cone onto the two free
directions of a box face: opposite signs imply an entering contour, while equal
signs imply only an isolated face contact. A corner retained by three-dimensional
regularization need not produce a retained cap region on all incident faces.
For the default cube this predicts two face-contour endpoint uses at each split
corner and none at the two uniform cones; it is a next tracing acceptance case,
not implemented contour connectivity. Then extend the nodal-linear/homotopy reference for exact
corner zeros and attach actual trims/caps/shells to it. Existing checks must not
be bypassed merely because eight local neighborhoods resolved. General tangent
corners, default grid-node degeneracies, curved operands and every unfinished
Stage 2–4 gate remain required.

Three kernel cases test exact recognition/output atomicity, default/scaled/
period-translated corners, independent neighborhood derivative samples, both
material conventions, source immutability, round-trip identities and explicit
precision/type/near-zero/tangent refusals. The FreeCAD test reproduces the default
parameters, exercises owned APIs and proves that whole-box trimming still rejects.
Executed counts and build/test logs are in PROGRESS.md.
