# Regularized corner-aware mathematical region queries

`BRepClass3d_ImplicitBox::Classify` now accepts explicit regularized-corner
boundaries, including schema-2/property-restored snapshots. Common queries the
closure of the box's three-dimensional `F<0` material; BoxMinusField uses `F>0`.
This differs from the old closed inequalities at isolated contacts. The field,
isovalue, phase and box are unchanged. This is not stored-topology or solid
validation, nor generic Part dispatch.

The existing conservative classifier remains the basis: strict field signs give
physical separation bounds, retained strict-sign cap points give actual boundary
witnesses, and an in-box opposite-sign bracket contains a material-boundary point.
Those witnesses also apply to closure-of-interior material. No raw field residual
is interpreted as distance. Legacy `Reject` policy behavior is unchanged.

## Exact corners and excluded neighborhoods

Local corner germs are checked afresh from the authoritative field/domain, not
trusted from saved metadata or a mesh. A split cone or uniformly retained cone
has strict material points arbitrarily close inside the box. Its exact corner
lies in the regularized material and, being on the box boundary, is an actual
boundary witness. Isolated closed-set contacts are never used as such witnesses.

At an excluded corner `a`, choose `G=F` for Common and `G=-F` for subtraction.
The exact lattice identity gives `G(a)=0`. All three inward coordinate
derivatives are positive. If their minimum is `m` and `H` bounds the field's
Hessian operator norm, Taylor's inequality for an inward displacement gives:

```
G(a+d) >= m*||d|| - H*||d||^2/2
```

Thus the clipped ball of radius `m/H` has strictly excluded sign except at its
anchor; there is no regularized material anywhere in that ball. The implementation
subtracts gradient/coordinate reserves, limits the radius to one eighth of the
smallest box span and subtracts query/anchor arithmetic reserves. Consequently
`radius - distance(point,a)` gives a conservative region/boundary-distance lower
bound, even for query points outside the box. This is NOT a nearest-point
estimate or an extension of the smaller coordinate-monotone germ cube.

For the default cube at 0.1 mm tolerance, the isolated Common contact at the
origin and subtraction contact at `(10,10,10)` classify Outside, not NearBoundary.
The other seven corners are actual boundary witnesses for each operation. If
the separation bound is insufficient and no genuine witness is found, the answer
remains Unresolved. A larger tolerance does not turn an excluded point itself
into a witness. Bounds use ordinary floating-point engineering reserves, not
directed intervals or a whole-model error/topology certificate.

## FreeCAD ownership and API

`ImplicitOCCT.classify_mixed(handle, point, tolerance=.1)` now reports
`corner_policy` as `reject` or `regularized`, in addition to the existing physical
bounds/witnesses. Classification preserves original/restored seam provenance and
never sets solid validity. No new persistence schema or C++ result-layout change.

`Implicit.classifyNative` on whole ComplexGeoData snapshots uses the same region
through existing rigid-placement precision handling. Container/rigid App::Link
and restored FCStd ownership are tested. Selected edges/vertices are not regions;
the existing selected-element rejection remains. The workbench default gyroid
and display mesh are unchanged.

Kernel tests exercise all eight corners for both operations, strict interior/
exterior points, independent in-box sign brackets, outside-box exclusion,
insufficient-separation Unresolved results, physical scale/periodic translation,
schema-2 restore and source immutability. Native CLI tests additionally cover
typed properties, placed containers/links, FCStd restore and retained lifetime.

The [whole-box corner homotopy/surface reference](native-corner-topology.md) now
resolves the default surface and contacts. The [closed material reference](native-corner-region.md)
now supplies caps and strict-volume grouping. The [graph/cap checker](native-corner-attachment.md)
now attaches cap regions, and [complete trim cycles](native-corner-trim.md) attach too.
[Whole restricted-face/native shell attachment](native-corner-shells.md) now exists.
Next: qualified default native solids and GUI exposure. Mesh Prepare,
clipped coverage and solid factories still guard
this policy. General curved operands, snapping/history, richer editing and
cross-workbench dispatch remain part of the unfinished Stage 2–4 requirements.
The later [cap geometry extension](native-corner-cap-geometry.md) supplies separate
directed distance bounds without native-solid promotion.
