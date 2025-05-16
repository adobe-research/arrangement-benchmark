#include <arrangement/Arrangement.h>
#include <arrangement/EigenTypedef.h>

#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>

#include <CLI/CLI.hpp>

#include <exception>
#include <string>

int main(int argc, char** argv)
{
    struct
    {
        std::string engine = "fast";
        std::string input_mesh;
        std::string output_mesh;
    } args;

    CLI::App app{"Compute arrangement"};
    app.add_option("--engine", args.engine, "Engine to use (fast, mesh)");
    app.add_option("input_mesh", args.input_mesh, "Input mesh file")->required();
    app.add_option("output_mesh", args.output_mesh, "Output mesh file")->required();
    CLI11_PARSE(app, argc, argv);

    arrangement::MatrixFr vertices;
    arrangement::MatrixIr faces;
    arrangement::VectorI face_labels;

    igl::read_triangle_mesh(args.input_mesh, vertices, faces);
    face_labels.resize(faces.rows());
    face_labels.setZero();

    std::string output_basename = args.output_mesh.substr(0, args.output_mesh.find_last_of('.'));

    arrangement::Arrangement::Ptr engine;
    if (args.engine == "fast") {
        engine = arrangement::Arrangement::create_fast_arrangement(vertices, faces, face_labels);
    } else if (args.engine == "mesh") {
        engine = arrangement::Arrangement::create_mesh_arrangement(vertices, faces, face_labels);
    } else {
        throw std::runtime_error("Unknown engine: " + args.engine);
    }
    if (engine == nullptr) {
        throw std::runtime_error("Failed to create arrangement engine");
    }

    engine->run();

    const auto& V = engine->get_vertices();
    const auto& F = engine->get_faces();

    igl::write_triangle_mesh(args.output_mesh, V, F);

    for (size_t i = 0; i < engine->get_num_cells(); i++) {
        std::string cell_mesh_file = output_basename + "_cell_" + std::to_string(i) + ".obj";
        const auto cell_faces = engine->get_cell_faces(i);
        igl::write_triangle_mesh(cell_mesh_file, V, cell_faces);
    }

    return 0;
}
