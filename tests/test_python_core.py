import importlib.util
import math
from pathlib import Path
import unittest


MODULE_PATH = Path(__file__).parents[1] / "addon" / "ImplicitGyroid" / "implicit_core.py"
SPEC = importlib.util.spec_from_file_location("implicit_core", MODULE_PATH)
implicit_core = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(implicit_core)


class GyroidCoreTests(unittest.TestCase):
    def test_origin_and_analytical_gradient(self):
        surface = implicit_core.Gyroid(2.0 * math.pi)
        self.assertAlmostEqual(surface.evaluate((0.0, 0.0, 0.0)), 0.0)
        self.assertEqual(surface.gradient((0.0, 0.0, 0.0)), (1.0, 1.0, 1.0))

    def test_mesh_stays_in_bounds(self):
        surface = implicit_core.Gyroid(1.0)
        bounds = implicit_core.Bounds((0.0, 0.0, 0.0), (1.0, 1.0, 1.0))
        mesh = implicit_core.extract_mesh(surface, bounds, 5)
        self.assertGreater(len(mesh.triangles), 0)
        self.assertEqual(len(mesh.vertices), len(mesh.normals))
        self.assertEqual(len(mesh.vertices), 3 * len(mesh.triangles))
        for vertex in mesh.vertices:
            self.assertTrue(all(-1.0e-12 <= value <= 1.0 + 1.0e-12 for value in vertex))

    def test_invalid_inputs(self):
        with self.assertRaises(ValueError):
            implicit_core.Gyroid(0.0)
        with self.assertRaises(ValueError):
            implicit_core.extract_mesh(implicit_core.Gyroid(1.0),
                                       implicit_core.Bounds((0, 0, 0), (1, 1, 1)), 0)


if __name__ == "__main__":
    unittest.main()

