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
        output_mesh.create_attribute("label", initial_values=engine.face_labels)

        cells = []
        for ci in range(engine.num_cells):
            faces = engine.get_cell_faces(ci)
            cell = lagrange.SurfaceMesh()
            cell.add_vertices(engine.vertices)
            cell.add_triangles(faces)
            cells.append(cell)

        return output_mesh, cells


    def test_geogram_arrangement(self, tet):
        r, cells = self.compute_arrangement(tet, "geogram")
        lagrange.io.save_mesh("debug.obj", r)

        tet2 = tet.clone()
        tet2.vertices = tet.vertices + [10.2, 0.2, 0.2]

        mesh = lagrange.combine_meshes([tet, tet2])
        r, cells = self.compute_arrangement(mesh, "geogram")
        lagrange.io.save_mesh("debug2.obj", r)

        for i, cell in enumerate(cells):
            print(cell.facets)
            lagrange.io.save_mesh(f"cell_{i}.msh", cell)
