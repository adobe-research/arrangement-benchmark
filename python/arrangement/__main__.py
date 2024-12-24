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
import json


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
        choices=["mesh", "fast", "geogram"],
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
    elif args.engine == "geogram":
        engine = arrangement.Arrangement.create_geogram_arrangement(
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

    cell_data = engine.cells
    patches = engine.patches
    parent_facets = engine.face_labels
    cell_info = []

    # Dump cell information to JSON.
    for i in range(engine.num_cells):
        cell_ids = cell_data[patches]
        active_facets = (cell_ids == i).any(axis=1)
        active_orientations = np.zeros(output_mesh.num_facets, dtype=int)
        active_orientations[cell_ids[:, 0] == i] = -1
        active_orientations[cell_ids[:, 1] == i] = 1

        active_parent_facets = parent_facets[active_facets]
        active_orientations = active_orientations[active_facets]
        active_facets = np.where(active_facets)[0]
        cell_info.append(
            {
                "facet_ids": active_facets.tolist(),
                "facet_orientations": active_orientations.tolist(),
                "parent_facet_ids": active_parent_facets.tolist(),
            }
        )

    output_dir = Path(args.output).parent
    with open(output_dir / f"{Path(args.output).stem}_cells.json", "w") as f:
        json.dump(cell_info, f, indent=4)

    cells = []
    for i in range(engine.num_cells):
        cell_facets = engine.get_cell_faces(i)

        cell = lagrange.SurfaceMesh()
        cell.add_vertices(engine.vertices)
        cell.add_triangles(cell_facets)
        cell.create_attribute("cell_id", initial_values=np.ones(cell.num_facets) * i)

        cells.append(cell)

        if args.export_cells:
            cell_file = output_dir / f"{Path(args.output).stem}_cell_{i}.msh"
            lagrange.io.save_mesh(cell_file, cell)

    cells = lagrange.combine_meshes(cells)
    lagrange.io.save_mesh(output_dir / f"{Path(args.output).stem}_cells.msh", cells)


if __name__ == "__main__":
    main()
