import FreeCADGui as Gui


class CreateImplicitGyroidCommand:
    def GetResources(self):
        return {
            "MenuText": "Implicit Gyroid",
            "ToolTip": "Create an editable bounded implicit gyroid",
            "Pixmap": "",
        }

    def Activated(self):
        from ImplicitGyroid.freecad_feature import create_gyroid
        create_gyroid()

    def IsActive(self):
        return True


class ImplicitGyroidWorkbench(Gui.Workbench):
    MenuText = "Implicit Geometry"
    ToolTip = "Experimental implicit-surface visualization"

    def Initialize(self):
        Gui.addCommand("CreateImplicitGyroid", CreateImplicitGyroidCommand())
        self.appendToolbar("Implicit Geometry", ["CreateImplicitGyroid"])
        self.appendMenu("Implicit Geometry", ["CreateImplicitGyroid"])

    def GetClassName(self):
        return "Gui::PythonWorkbench"


Gui.addWorkbench(ImplicitGyroidWorkbench())

