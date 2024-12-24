#include "utils.h"

#include <arrangement/Arrangement.h>

#include <igl/write_triangle_mesh.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <tuple>

auto concatenate_mesh(const arrangement::MatrixFr& V1,
    const arrangement::MatrixIr& F1,
    const arrangement::VectorI& L1,
    const arrangement::MatrixFr& V2,
    const arrangement::MatrixIr& F2,
    const arrangement::VectorI& L2)
{
    auto V = concatentate_rows(V1, V2);
    auto F = concatentate_rows(F1, (F2.array() + V1.rows()).matrix().eval());
    auto L = concatentate_rows(L1, L2);
    return std::make_tuple(V, F, L);
}

TEST_CASE("FastArrangement", "[arrangement]")
{
    auto [V, F, L] = generate_tet();

    SECTION("Simple")
    {
        auto engine = arrangement::Arrangement::create_fast_arrangement(V, F, L);
        engine->run();

        // +1 because arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 1 + 1);
        REQUIRE(engine->get_num_patches() == 1);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 4);
    }

    SECTION("Disjoint tets")
    {
        auto V2 = (V.array() + 10).matrix().eval();
        auto F2 = F;
        auto L2 = (L.array() + F.rows()).matrix().eval();

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_fast_arrangement(V3, F3, L3);
        engine->run();

        // +1 because arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);
        REQUIRE(engine->get_num_patches() == 2);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        // igl::write_triangle_mesh("debug.obj", vertices, faces);

        REQUIRE(vertices.rows() == 8);
        REQUIRE(faces.rows() == 8);
    }

    SECTION("exactly duplicate triangle")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0,
            1, 0, 0,
            0, 1, 0;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_fast_arrangement(V3, F3, L3);
        engine->run();

        // +1 because arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 1 + 1);
        REQUIRE(engine->get_num_patches() == 1);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 4);
    }

    SECTION("triangle-tet")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0.5,
            1, 0, 0.5,
            0, 1, 0.5;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_fast_arrangement(V3, F3, L3);
        engine->run();

        // +1 because arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);
        REQUIRE(engine->get_num_patches() == 4);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 9);
        REQUIRE(faces.rows() == 13);
    }
}

TEST_CASE("MeshArrangement", "[arrangement]")
{
    auto [V, F, L] = generate_tet();

    SECTION("Simple")
    {
        auto engine = arrangement::Arrangement::create_mesh_arrangement(V, F, L);
        engine->run();

        // +1 because mesh arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 1 + 1);
        REQUIRE(engine->get_num_patches() == 1);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 4);
    }

    SECTION("Disjoint tets")
    {
        auto V2 = (V.array() + 10).matrix().eval();
        auto F2 = F;
        auto L2 = (L.array() + F.rows()).matrix().eval();

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_mesh_arrangement(V3, F3, L3);
        engine->run();

        // +1 because mesh arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);
        REQUIRE(engine->get_num_patches() == 2);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 8);
        REQUIRE(faces.rows() == 8);
    }

    SECTION("exactly duplicate triangle")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0,
            1, 0, 0,
            0, 1, 0;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_mesh_arrangement(V3, F3, L3);
        engine->run();

        // Mesh arrangement keeps degenerate cells.
        // +1 because mesh arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);
        REQUIRE(engine->get_num_patches() == 3);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 5);
    }

    SECTION("triangle-tet")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0.5,
            1, 0, 0.5,
            0, 1, 0.5;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_mesh_arrangement(V3, F3, L3);
        engine->run();

        // +1 because mesh arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);
        REQUIRE(engine->get_num_patches() == 4);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 9);
        REQUIRE(faces.rows() == 13);

        auto& labels = engine->get_out_face_labels();
        REQUIRE(labels.minCoeff() == 0);
        REQUIRE(labels.maxCoeff() == 4);
    }
}

TEST_CASE("GeogramArrangement", "[arrangement]")
{
    auto [V, F, L] = generate_tet();

    SECTION("Simple")
    {
        auto engine = arrangement::Arrangement::create_geogram_arrangement(V, F, L);
        engine->run();

        // +1 because geogram arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 1 + 1);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 4 * 2); // all faces are duplicated in geogram's output
    }

    //SECTION("Disjoint tets")
    //{
    //    auto V2 = (V.array() + 10).matrix().eval();
    //    auto F2 = F;
    //    auto L2 = (L.array() + F.rows()).matrix().eval();

    //    auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

    //    auto engine = arrangement::Arrangement::create_geogram_arrangement(V3, F3, L3);
    //    engine->run();

    //    // +2 because geogram arrangments keeps the ambient cell per component.
    //    REQUIRE(engine->get_num_cells() == 2 + 2);

    //    auto& vertices = engine->get_vertices();
    //    auto& faces = engine->get_faces();
    //    igl::write_triangle_mesh("debug.obj", vertices, faces);

    //    for (int i=0; i<engine->get_num_cells(); i++) {
    //        auto cell_faces = engine->get_cell_faces(i);
    //        igl::write_triangle_mesh("debug_" + std::to_string(i) + ".obj", vertices, cell_faces);
    //    }

    //    REQUIRE(vertices.rows() == 8);
    //    REQUIRE(faces.rows() == 8 * 2); // all faces are duplicated in geogram's output
    //}

    SECTION("exactly duplicate triangle")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0,
            1, 0, 0,
            0, 1, 0;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_geogram_arrangement(V3, F3, L3);
        engine->run();

        // Geogram arrangement keeps degenerate cells.
        REQUIRE(engine->get_num_cells() == 2);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 4);
        REQUIRE(faces.rows() == 4 * 2); // all faces are duplicated in geogram's output
    }

    SECTION("triangle-tet")
    {
        arrangement::MatrixFr V2(3, 3);
        // clang-format off
        V2 <<
            0, 0, 0.5,
            1, 0, 0.5,
            0, 1, 0.5;
        // clang-format on

        arrangement::MatrixIr F2(1, 3);
        F2 << 0, 1, 2;

        arrangement::VectorI L2(1);
        L2 << 4;

        auto [V3, F3, L3] = concatenate_mesh(V, F, L, V2, F2, L2);

        auto engine = arrangement::Arrangement::create_geogram_arrangement(V3, F3, L3);
        engine->run();

        // +1 because geogram arrangments keeps the ambient cell.
        REQUIRE(engine->get_num_cells() == 2 + 1);

        auto& vertices = engine->get_vertices();
        auto& faces = engine->get_faces();

        REQUIRE(vertices.rows() == 9);
        REQUIRE(faces.rows() == 13 * 2); // all faces are duplicated in geogram's output

        auto& labels = engine->get_out_face_labels();
        REQUIRE(labels.minCoeff() == 0);
        REQUIRE(labels.maxCoeff() == 4);
    }
}
