# Restored cylinder material queries

Implemented in OCCT TKTopAlgo and the compiled FreeCAD native bridge. Native
cylinder tests pass 23/23; selected CLI/GUI checks pass 14/14. This is separate
from the older standalone or Python field prototypes.

`ImplicitOCCT.classify_cylinder_material(owner, point, point_tolerance=1.e-7)`
accepts original qualified cylinder geometry/solids, freshly qualified restored
geometry, and qualified restored solids. A ready `NativeCylinderFeature.Geometry`
is such an owned solid capsule. Raw FCStd/graph restoration and failed geometry
or solid qualification cannot authorize a query. Changing a returned report
dictionary does not change the underlying qualification state.

OCCT `BRepClass3d_ImplicitCylinderIO::ClassifyMaterial` takes a verified stored
`Geometry`, recomputes the axial derivative cover over the actual finite cylinder,
and calls the same private spatial classifier used by construction geometry.
It evaluates the authoritative gyroid field and cylinder domain. It neither
retraces a contour nor rebuilds the stored native shape, and it stores no query
results, callbacks or new certificates in the source owner.

The result has `mathematical_state` IN, OUT, ON or UNKNOWN, spatial
`distance_lower_bound`/`distance_upper_bound`, the field value at the domain
projection, and `boundary_allowance_separated`. The latter records clearance
beyond the qualified native boundary approximation allowance. The query
tolerance is a model length, not a dimensionless field-value threshold. UNKNOWN
is a valid result when the distance bounds straddle that tolerance. Invalid
coordinates/tolerances refuse; cancellation propagates. Generic OCCT solid
classification remains unsupported.

An FCStd load starts Unverified. Explicit geometry and solid requalification
enable the query; undoing that transaction restores the unverified owner and
revokes query access. Owned capsules retain their immutable graph independently
of document/view buffers. This increment does not add automatic background
requalification, editable field/operand dependencies or general Boolean dispatch.
