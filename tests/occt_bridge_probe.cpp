#include "implicit/GyroidSurface.h"
#include "implicit/MarchingTetrahedra.h"

#include <BRepAdaptor_Surface.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangle.hxx>
#include <Poly_Triangulation.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Face.hxx>

#include <cmath>
#include <cstdlib>
#include <iostream>

int main()
{
    constexpr double period = 10.0;
    implicit::GyroidSurface surface(period, 0.0, {{0, 0, 0}, {period, period, period}});
    const implicit::TriangleMesh mesh =
        implicit::MarchingTetrahedra::extract(surface, {8, 8, 8});
    if (mesh.vertices.empty() || mesh.triangles.empty()) {
        std::cerr << "implicit extraction returned an empty mesh\n";
        return EXIT_FAILURE;
    }

    Handle(Poly_Triangulation) triangulation =
        new Poly_Triangulation(static_cast<int>(mesh.vertices.size()),
                               static_cast<int>(mesh.triangles.size()), false);
    for (std::size_t index = 0; index < mesh.vertices.size(); ++index) {
        const auto& point = mesh.vertices[index];
        triangulation->SetNode(static_cast<int>(index + 1), gp_Pnt(point.x, point.y, point.z));
    }
    for (std::size_t index = 0; index < mesh.triangles.size(); ++index) {
        const auto& triangle = mesh.triangles[index];
        triangulation->SetTriangle(static_cast<int>(index + 1),
                                   Poly_Triangle(static_cast<int>(triangle[0] + 1),
                                                 static_cast<int>(triangle[1] + 1),
                                                 static_cast<int>(triangle[2] + 1)));
    }

    BRep_Builder builder;
    TopoDS_Face meshFace;
    builder.MakeFace(meshFace, triangulation);
    TopLoc_Location location;
    const Handle(Poly_Triangulation) stored = BRep_Tool::Triangulation(meshFace, location);
    const Handle(Geom_Surface) geometry = BRep_Tool::Surface(meshFace);
    if (stored.IsNull() || stored->NbTriangles() != triangulation->NbTriangles()) {
        std::cerr << "OCCT did not retain the visualization triangulation\n";
        return EXIT_FAILURE;
    }
    if (!geometry.IsNull()) {
        std::cerr << "triangulation-only face unexpectedly acquired surface geometry\n";
        return EXIT_FAILURE;
    }

    // BRepAdaptor_Surface intentionally has no usable surface when BRep_Tool
    // returns null. Constructing it is safe, but parametric evaluation is not.
    BRepAdaptor_Surface adaptor(meshFace);
    (void)adaptor;
    std::cout << "OCCT bridge probe passed: mesh face has " << stored->NbNodes()
              << " nodes and " << stored->NbTriangles()
              << " triangles, but no Geom_Surface\n";
    return EXIT_SUCCESS;
}

