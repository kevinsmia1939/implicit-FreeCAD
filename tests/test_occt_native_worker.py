# SPDX-License-Identifier: LGPL-2.1-or-later
"""Protocol/operation regression for the isolated OCCT 8 process."""
import json
import math
import subprocess
import sys

worker = sys.argv[1]


def run(mode, a=(0, 0, 0, 2), b=(2, 0, 0, 2), tolerance=.1):
    return subprocess.run([worker, mode] + [str(v) for v in (tolerance, *a, *b)],
                          capture_output=True, text=True, timeout=30)


for mode, count in (("sphere", 1), ("trim", 1), ("common", 2), ("fuse", 2), ("cut", 2)):
    result = run(mode, b=(0, 0, 1, .5) if mode == "trim" else (2, 0, 0, 2))
    assert result.returncode == 0, result.stderr
    data = json.loads(result.stdout)
    assert data["schema"] == 1 and data["solid_validated"] is False
    assert data["backend"] == "OCCT-8.1-native-sphere"
    assert len(data["faces"]) == count
    for face in data["faces"]:
        assert 0 < face["geometric_bound"] <= .1
        assert len(face["vertices"]) == len(face["normals"]) <= 120000
        assert len(face["triangles"]) <= 240000
        assert face["trimmed"] == (mode != "sphere")
        for vertex, normal in zip(face["vertices"], face["normals"]):
            assert all(math.isfinite(v) for v in vertex + normal)
            assert abs(sum(v*v for v in normal) - 1) < 1e-12
        for triangle in face["triangles"]:
            assert all(isinstance(i, int) and 0 <= i < len(face["vertices"]) for i in triangle)
    if mode == "cut":
        assert data["faces"][1]["reversed"] is True

for result in (run("invalid"), run("trim", b=(0, 0, 0, 1)),
               run("common", b=(4, 0, 0, 2)), run("sphere", tolerance=0),
               run("sphere", tolerance=1e-15), run("sphere", a=(0, 0, 0, -1)),
               run("sphere", a=(float("nan"), 0, 0, 2))):
    assert result.returncode != 0
    assert not result.stdout, "failed request emitted a partial response"
    assert result.stderr

empty = run("common", b=(10, 0, 0, 2))
assert empty.returncode == 0 and not json.loads(empty.stdout)["faces"]
print("OCCT native worker protocol test passed")

def section(tolerance=.1, iso=.21, budget=512):
    return subprocess.run([worker, "section", str(tolerance), "10", str(iso),
                           "0", "0", "1", ".37", "8", str(budget)],
                          capture_output=True, text=True, timeout=30)

resolved = section()
assert resolved.returncode == 0, resolved.stderr
data = json.loads(resolved.stdout)
assert data["resolved"] is True and data["curves"]
assert data["backend"] == "OCCT-8.1-native-section"
assert 0 < data["geometric_bound"] <= .1
assert data["maximum_root_bracket"] <= .1/16
assert data["graph_defects"] == 0 and data["unresolved_cells"] == 0
for curve in data["curves"]:
    assert len(curve["points"]) >= 2
    assert all(abs(p[2] - .37) < 1e-14 and all(math.isfinite(v) for v in p) for p in curve["points"])
unresolved = json.loads(section(budget=4).stdout)
assert unresolved["resolved"] is False and unresolved["unresolved_cells"] > 0
empty = json.loads(section(iso=3).stdout)
assert empty["resolved"] is True and not empty["curves"]
invalid = section(budget=513)
assert invalid.returncode and not invalid.stdout
print("OCCT native section protocol test passed")
