// SPDX-License-Identifier: LGPL-2.1-or-later
#include <BRepClass3d_SignedDistance.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepTools.hxx>
#include <Standard_Failure.hxx>
#include <TopExp_Explorer.hxx>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}
void near(double actual, double expected)
{
    require(std::abs(actual - expected) < 2e-6, "incorrect signed distance");
}
int main(int argc, char** argv)
{
    try {
        auto box = BRepPrimAPI_MakeBox(gp_Pnt(-1, -1, -1), gp_Pnt(1, 1, 1)).Shape();
        BRepClass3d_SignedDistance distance(box);
        near(distance.Value(gp_Pnt(0, 0, 0)), -1);
        near(distance.Value(gp_Pnt(0.5, 0, 0)), -0.5);
        near(distance.Value(gp_Pnt(1, 0, 0)), 0);
        near(distance.Value(gp_Pnt(2, 2, 2)), std::sqrt(3.0));
        near(distance.Value(gp_Pnt(1 + 1e-8, 0, 0)), 0);
        auto sphere = BRepPrimAPI_MakeSphere(3).Shape();
        auto shell = BRepAlgoAPI_Cut(sphere, BRepPrimAPI_MakeSphere(1).Shape()).Shape();
        // Cut can return a one-solid compound; explicitly select its solid.
        TopExp_Explorer solid(shell, TopAbs_SOLID);
        require(solid.More(), "hollow sphere has no solid");
        BRepClass3d_SignedDistance hollow(solid.Current());
        near(hollow.Value(gp_Pnt(0, 0, 0)), 1);
        near(hollow.Value(gp_Pnt(2, 0, 0)), -1);
        near(hollow.Value(gp_Pnt(4, 0, 0)), 1);
        for (const auto& invalid : {TopoDS_Shape(), box.Reversed()}) {
            bool failed = false;
            try { BRepClass3d_SignedDistance rejected(invalid); }
            catch (const Standard_Failure&) { failed = true; }
            require(failed, "accepted invalid solid");
        }
        bool failed = false;
        try { distance.Value(gp_Pnt(std::numeric_limits<double>::infinity(), 0, 0)); }
        catch (const Standard_Failure&) { failed = true; }
        require(failed, "accepted non-finite point");
        if (argc > 1) require(BRepTools::Write(box, argv[1]), "could not write B-Rep fixture");
        std::cout << "OCCT signed distance tests passed\n";
    }
    catch (const Standard_Failure& error) {
        std::cerr << error.GetMessageString() << '\n';
        return 1;
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
