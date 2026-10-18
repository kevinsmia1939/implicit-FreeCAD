# SPDX-License-Identifier: LGPL-2.1-or-later
"""User-directory entry point. Requires the implicit-FreeCAD fork runtime."""
import FreeCAD as App

try:
    import ImplicitGui
    import ImplicitWorkbench
    ImplicitWorkbench.register()
except ImportError as error:
    App.Console.PrintWarning(
        "Implicit Modelling requires the implicit-FreeCAD fork executable; "
        "the installed FreeCAD does not contain its native core: " + str(error) + "\n")
