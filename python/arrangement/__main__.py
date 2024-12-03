#!/usr/bin/env python
"""
Compute the arrangement of a set of meshes.

Example:
  python -m arrangement -o result.msh mesh1.obj mesh2.ply
"""

import argparse
import arrangement
import lagrange
import numpy as np
from pathlib import Path


def parse_args():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawTextHelpFormatter,  # Allows multiline descriptions
    )
    parser.add_argument(
        "-e",
        "--engine",
        help="Arrangement engine",
        default="mesh",
        choices=["mesh", "fast"],
    )
    parser.add_argument(
        "-x", "--export-cells", action="store_true", help="Export cells"
    )
    parser.add_argument("-o", "--output", help="Output file", required=True)
    parser.add_argument("input_meshes", nargs="+", help="Input mesh files")
    return parser.parse_args()


def main():
    args = parse_args()
    meshes = [lagrange.io.load_mesh(f) for f in args.input_meshes]
    for i, mesh in enumerate(meshes):
        mesh.create_attribute("mesh_id", initial_values=np.ones(mesh.num_facets) * i)

    mesh = lagrange.combine_meshes(meshes)

    if args.engine == "mesh":
        engine = arrangement.Arrangement.create_mesh_arrangement(
            mesh.vertices, mesh.facets, np.arange(mesh.num_facets)
        )
    elif args.engine == "fast":
        engine = arrangement.Arrangement.create_fast_arrangement(
            mesh.vertices, mesh.facets, np.arange(mesh.num_facets)
        )
    else:
        raise ValueError(f"Unknown engine: {args.engine}")

    engine.run()

    output_mesh = lagrange.SurfaceMesh()
    output_mesh.add_vertices(engine.vertices)
    output_mesh.add_triangles(engine.faces)

    output_mesh.create_attribute("src_facet_id", initial_values=engine.face_labels)
    output_mesh.create_attribute("patch_id", initial_values=engine.patches)

    lagrange.io.save_mesh(args.output, output_mesh)

    if args.export_cells:
        for i in range(engine.num_cells):
            cell_facets = engine.get_cell_faces(i)

            cell = lagrange.SurfaceMesh()
            cell.add_vertices(engine.vertices)
            cell.add_triangles(cell_facets)

            cell_filename = f"{Path(args.output).stem}_cell_{i}{Path(args.output).suffix}"
            lagrange.io.save_mesh(cell_filename, cell)


if __name__ == "__main__":
    main()
