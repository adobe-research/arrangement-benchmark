import pytest
import lagrange
import arrangement
import numpy as np


@pytest.fixture
def tet():
    mesh = lagrange.SurfaceMesh()
    mesh.add_vertex([0, 0, 0])
    mesh.add_vertex([1, 0, 0])
    mesh.add_vertex([0, 1, 0])
    mesh.add_vertex([0, 0, 1])
    mesh.add_triangle(0, 2, 1)
    mesh.add_triangle(0, 3, 2)
    mesh.add_triangle(0, 1, 3)
    mesh.add_triangle(1, 2, 3)
    return mesh


class TestArrangement:
    def compute_arrangement(self, mesh, engine_name):
        match engine_name:
            case "mesh":
                engine = arrangement.Arrangement.create_mesh_arrangement(
                    mesh.vertices, mesh.facets, np.arange(mesh.num_facets)
                )
            case "fast":
                engine = arrangement.Arrangement.create_fast_arrangement(
                    mesh.vertices, mesh.facets, np.arange(mesh.num_facets)
                )
            case "geogram":
                engine = arrangement.Arrangement.create_geogram_arrangement(
                    mesh.vertices, mesh.facets, np.arange(mesh.num_facets)
                )

        engine.run()

        output_mesh = lagrange.SurfaceMesh()
        output_mesh.add_vertices(engine.vertices)
        output_mesh.add_triangles(engine.faces)
        output_mesh.create_attribute(
            "label", element="Facet", initial_values=engine.face_labels
        )

        cells = []
        for ci in range(engine.num_cells):
            faces = engine.get_cell_faces(ci)
            cell = lagrange.SurfaceMesh()
            cell.add_vertices(engine.vertices)
            cell.add_triangles(faces)
            cells.append(cell)

        if engine_name == "mesh":
            winding_number = engine.winding_number
            assert len(winding_number) == len(output_mesh.facets)
            assert winding_number.shape[1] == 2
            assert np.all(np.absolute(winding_number[:, 0] - winding_number[:, 1]) == 1)

        return output_mesh, cells

    def test_mesh_arrangement(self, tet):
        r, cells = self.compute_arrangement(tet, "mesh")
        assert len(cells) == 2

        tet2 = tet.clone()
        tet2.vertices = tet.vertices + [0.2, 0.2, 0.2]

        mesh = lagrange.combine_meshes([tet, tet2])
        r, cells = self.compute_arrangement(mesh, "mesh")
        assert len(cells) == 4

    def test_fast_arrangement(self, tet):
        r, cells = self.compute_arrangement(tet, "fast")
        assert len(cells) == 2

        tet2 = tet.clone()
        tet2.vertices = tet.vertices + [0.2, 0.2, 0.2]

        mesh = lagrange.combine_meshes([tet, tet2])
        r, cells = self.compute_arrangement(mesh, "fast")
        assert len(cells) == 4

    def test_geogram_arrangement(self, tet):
        r, cells = self.compute_arrangement(tet, "geogram")
        assert len(cells) == 2

        tet2 = tet.clone()
        tet2.vertices = tet.vertices + [0.2, 0.2, 0.2]

        mesh = lagrange.combine_meshes([tet, tet2])
        r, cells = self.compute_arrangement(mesh, "geogram")
        assert len(cells) == 4
