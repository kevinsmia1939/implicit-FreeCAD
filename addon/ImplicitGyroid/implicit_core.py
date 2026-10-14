"""Dependency-free gyroid evaluation and visualization mesh extraction."""

from dataclasses import dataclass
import math


@dataclass(frozen=True)
class Bounds:
    minimum: tuple
    maximum: tuple


@dataclass
class MeshData:
    vertices: list
    normals: list
    triangles: list


class Gyroid:
    def __init__(self, period, level=0.0):
        if not math.isfinite(period) or period <= 0.0:
            raise ValueError("period must be finite and positive")
        if not math.isfinite(level):
            raise ValueError("level must be finite")
        self.period = float(period)
        self.level = float(level)
        self.wave_number = 2.0 * math.pi / self.period

    def evaluate(self, point):
        x, y, z = (self.wave_number * value for value in point)
        return (
            math.sin(x) * math.cos(y)
            + math.sin(y) * math.cos(z)
            + math.sin(z) * math.cos(x)
            - self.level
        )

    def gradient(self, point):
        x, y, z = (self.wave_number * value for value in point)
        k = self.wave_number
        return (
            k * (math.cos(x) * math.cos(y) - math.sin(z) * math.sin(x)),
            k * (math.cos(y) * math.cos(z) - math.sin(x) * math.sin(y)),
            k * (math.cos(z) * math.cos(x) - math.sin(y) * math.sin(z)),
        )

    def normal(self, point):
        gradient = self.gradient(point)
        length = math.sqrt(sum(value * value for value in gradient))
        if length <= 1.0e-14:
            return (0.0, 0.0, 1.0)
        return tuple(value / length for value in gradient)


_CORNERS = (
    (0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0),
    (0, 0, 1), (1, 0, 1), (1, 1, 1), (0, 1, 1),
)
_TETRAHEDRA = (
    (0, 5, 1, 6), (0, 1, 2, 6), (0, 2, 3, 6),
    (0, 3, 7, 6), (0, 7, 4, 6), (0, 4, 5, 6),
)


def _subtract(a, b):
    return tuple(x - y for x, y in zip(a, b))


def _cross(a, b):
    return (
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    )


def _dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def _interpolate(a, b, fa, fb):
    denominator = fa - fb
    amount = 0.5 if abs(denominator) < 1.0e-15 else fa / denominator
    return tuple(x + amount * (y - x) for x, y in zip(a, b))


def _add_triangle(surface, mesh, a, b, c):
    normals = [surface.normal(point) for point in (a, b, c)]
    face_normal = _cross(_subtract(b, a), _subtract(c, a))
    average_normal = tuple(sum(values) for values in zip(*normals))
    if _dot(face_normal, average_normal) < 0.0:
        b, c = c, b
        normals[1], normals[2] = normals[2], normals[1]
    first = len(mesh.vertices)
    mesh.vertices.extend((a, b, c))
    mesh.normals.extend(normals)
    mesh.triangles.append((first, first + 1, first + 2))


def _polygonise_tetrahedron(surface, points, values, mesh):
    inside = [index for index, value in enumerate(values) if value <= 0.0]
    outside = [index for index, value in enumerate(values) if value > 0.0]
    if not inside or not outside:
        return
    if len(inside) in (1, 3):
        inverted = len(inside) == 3
        singleton = outside[0] if inverted else inside[0]
        others = inside if inverted else outside
        vertices = [
            _interpolate(points[singleton], points[index],
                         values[singleton], values[index])
            for index in others
        ]
        if inverted:
            vertices[1], vertices[2] = vertices[2], vertices[1]
        _add_triangle(surface, mesh, *vertices)
        return

    a = _interpolate(points[inside[0]], points[outside[0]], values[inside[0]], values[outside[0]])
    b = _interpolate(points[inside[0]], points[outside[1]], values[inside[0]], values[outside[1]])
    c = _interpolate(points[inside[1]], points[outside[0]], values[inside[1]], values[outside[0]])
    d = _interpolate(points[inside[1]], points[outside[1]], values[inside[1]], values[outside[1]])
    _add_triangle(surface, mesh, a, b, c)
    _add_triangle(surface, mesh, b, d, c)


def extract_mesh(surface, bounds, resolution):
    """Extract an indexed triangle soup; resolution is cells per axis."""
    if resolution < 1:
        raise ValueError("resolution must be positive")
    minimum, maximum = bounds.minimum, bounds.maximum
    if any(high <= low for low, high in zip(minimum, maximum)):
        raise ValueError("bounds must be non-empty")
    steps = tuple((high - low) / resolution for low, high in zip(minimum, maximum))

    def point(x, y, z):
        return tuple(minimum[axis] + steps[axis] * index
                     for axis, index in enumerate((x, y, z)))

    width = resolution + 1
    values = [0.0] * (width ** 3)

    def offset(x, y, z):
        return z * width * width + y * width + x

    for z in range(width):
        for y in range(width):
            for x in range(width):
                values[offset(x, y, z)] = surface.evaluate(point(x, y, z))

    mesh = MeshData([], [], [])
    for z in range(resolution):
        for y in range(resolution):
            for x in range(resolution):
                cube_points = [point(x + dx, y + dy, z + dz) for dx, dy, dz in _CORNERS]
                cube_values = [values[offset(x + dx, y + dy, z + dz)] for dx, dy, dz in _CORNERS]
                for tetrahedron in _TETRAHEDRA:
                    _polygonise_tetrahedron(
                        surface,
                        [cube_points[index] for index in tetrahedron],
                        [cube_values[index] for index in tetrahedron],
                        mesh,
                    )
    return mesh

