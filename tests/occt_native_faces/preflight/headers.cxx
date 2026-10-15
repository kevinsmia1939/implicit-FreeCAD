// SPDX-License-Identifier: LGPL-2.1-or-later
#include <Standard_Version.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <NCollection_List.hxx>
#include <type_traits>

static_assert(OCC_VERSION_MAJOR >= 8, "This regression must not fall back to OCCT 7");
static_assert(std::is_same_v<TopTools_ListOfShape, NCollection_List<TopoDS_Shape>>);

int main()
{
    TopTools_ListOfShape shapes;
    shapes.Append(TopoDS_Shape());
    TopTools_IndexedMapOfShape indexed;
    TColStd_SequenceOfInteger values;
    values.Append(42);
    return shapes.Extent() == 1 && indexed.IsEmpty() && values.First() == 42 ? 0 : 1;
}
