// SPDX-License-Identifier: LGPL-2.1-or-later
// OCCT 8-only subprocess. Never load this kernel into the OCCT 7 FreeCAD GUI.
#include <BRep_ImplicitSphereOps.hxx>
#include <BRep_TImplicitSpherePatch.hxx>
#include <BRep_ImplicitTool.hxx>
#include <BRep_Builder.hxx>
#include <Standard_Failure.hxx>
#include <Poly_Triangle.hxx>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>

namespace {
double number(const char* text)
{
    std::istringstream in(text);
    in.imbue(std::locale::classic());
    double result;
    if (!(in >> result) || !std::isfinite(result))
        throw std::runtime_error("Non-finite or invalid numeric argument");
    in >> std::ws;
    if (!in.eof()) throw std::runtime_error("Trailing numeric argument data");
    return result;
}
TopoDS_Face sphere(const gp_Pnt& center, double radius, double tolerance)
{
    TopoDS_Face face;
    const double extent = radius + tolerance;
    BRep_Builder().MakeImplicitFace(face, new Geom_ImplicitSphere(center, radius),
        center.Translated(gp_Vec(-extent, -extent, -extent)),
        center.Translated(gp_Vec(extent, extent, extent)), TopLoc_Location(), tolerance);
    return face;
}
void point(std::ostream& out, const gp_XYZ& p)
{
    out << '[' << p.X() << ',' << p.Y() << ',' << p.Z() << ']';
}
}
int main(int argc, char** argv)
{
    try {
        if (argc != 11)
            throw std::runtime_error("Usage: worker sphere|trim|common|fuse|cut tolerance ax ay az radius bx by bz radiusOrPlaneOffset");
        using Ops = BRep_ImplicitSphereOps;
        const std::string mode(argv[1]);
        const double tolerance = number(argv[2]);
        const gp_Pnt a(number(argv[3]), number(argv[4]), number(argv[5]));
        const double ar = number(argv[6]);
        const gp_Pnt b(number(argv[7]), number(argv[8]), number(argv[9]));
        const double br = number(argv[10]);
        const auto left = sphere(a, ar, tolerance);
        std::vector<TopoDS_Face> faces;
        std::vector<int> sources;
        if (mode == "sphere") { faces = {left}; sources = {0}; }
        else if (mode == "trim") {
            const double length = std::hypot(b.X(), b.Y(), b.Z());
            if (!std::isfinite(length) || length <= 0)
                throw std::runtime_error("Plane normal must be finite and nonzero");
            const gp_Dir normal(b.X()/length, b.Y()/length, b.Z()/length);
            faces = {Ops::Trim(left, gp_Pln(gp_Pnt(gp_XYZ(normal.X(), normal.Y(), normal.Z())*br), normal))};
            sources = {0};
        } else {
            Ops::Operation op;
            if (mode == "common") op = Ops::Operation::Common;
            else if (mode == "fuse") op = Ops::Operation::Fuse;
            else if (mode == "cut") op = Ops::Operation::Cut;
            else throw std::runtime_error("Unknown operation");
            const auto result = Ops::Boolean(left, sphere(b, br, tolerance), op);
            faces = result.Faces;
            sources = result.Sources;
        }
        // Buffer the complete response: a failed operation emits no partial JSON.
        std::ostringstream out;
        out.imbue(std::locale::classic());
        out << std::setprecision(17);
        out << "{\"schema\":1,\"backend\":\"OCCT-8.1-native-sphere\",\"solid_validated\":false,\"faces\":[";
        for (size_t f = 0; f < faces.size(); ++f) {
            if (f) out << ',';
            const auto data = occ::down_cast<BRep_TImplicitFace>(faces[f].TShape());
            const auto mesh = Ops::Tessellate(faces[f], tolerance);
            out << "{\"source\":" << sources[f] << ",\"reversed\":"
                << (faces[f].Orientation() == TopAbs_REVERSED ? "true" : "false")
                << ",\"trimmed\":" << (data->IsKind(STANDARD_TYPE(BRep_TImplicitSpherePatch)) ? "true" : "false")
                << ",\"geometric_bound\":" << mesh.GeometricBound << ",\"vertices\":[";
            for (int i = 1; i <= mesh.Triangulation->NbNodes(); ++i) {
                if (i > 1) out << ',';
                point(out, mesh.Triangulation->Node(i).XYZ());
            }
            out << "],\"normals\":[";
            for (int i = 1; i <= mesh.Triangulation->NbNodes(); ++i) {
                if (i > 1) out << ',';
                point(out, BRep_ImplicitTool::Normal(faces[f], mesh.Triangulation->Node(i)).XYZ());
            }
            out << "],\"triangles\":[";
            for (int i = 1; i <= mesh.Triangulation->NbTriangles(); ++i) {
                if (i > 1) out << ',';
                int x,y,z;
                mesh.Triangulation->Triangle(i).Get(x,y,z);
                out << '[' << x-1 << ',' << y-1 << ',' << z-1 << ']';
            }
            out << "]}";
        }
        out << "]}\n";
        std::cout << out.str();
        return std::cout ? 0 : 2;
    } catch (const Standard_Failure& error) {
        std::cerr << error.what() << '\n';
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
    }
    return 1;
}
