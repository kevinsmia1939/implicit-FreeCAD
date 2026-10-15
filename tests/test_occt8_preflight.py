# SPDX-License-Identifier: LGPL-2.1-or-later
"""Check strict source-build selection; never compile/install or mix OCCT ABIs."""
from pathlib import Path
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[1]
native = Path(sys.argv[1]).resolve()
with tempfile.TemporaryDirectory(prefix="implicit-occt8-preflight-") as temporary:
    for name, source in (("actual", native), ("invalid", Path(temporary) / "absent")):
        run = subprocess.run(["cmake", "-S", str(root / "tests/occt_native_faces/preflight"),
                              "-B", str(Path(temporary) / name),
                              "-DIMPLICIT_FREECAD_SOURCE=" + str(root / "FreeCAD"),
                              "-DFREECAD_OCCT_BUILD_DIR=" + str(source)],
                             capture_output=True, text=True, timeout=30)
        output = run.stdout + run.stderr
        if name == "invalid":
            assert run.returncode and "Explicit OCCT source build is not configured" in output, output
        elif run.returncode:
            assert "Explicit OCCT source build is not ready for FreeCAD" in output, output
            assert "System-library fallback is" in output and "forbidden" in output, output
            assert "found version \"8." in output, output
        else:
            assert "Using explicit OCCT build-tree targets only" in output, output
            print("OCCT dependencies are built; FreeCAD source/API/runtime migration is still unverified")
print("strict OCCT source-build preflight test passed")
