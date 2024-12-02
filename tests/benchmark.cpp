#include "utils.h"

#include <igl/write_triangle_mesh.h>

#include <arrangement/Arrangement.h>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <tuple>

TEST_CASE("benchmark", "[arrangement][!benchmark]")
{
    auto [tet_V, tet_F, tet_L] = generate_tet();
    tet_V = tet_V.rowwise() - tet_V.colwise().mean();
    Eigen::Vector3d axis(1, 2, 3);
    axis.normalize();

    constexpr size_t N = 10;
    arrangement::MatrixFr V(4 * N, 3);
    arrangement::MatrixIr F(4 * N, 3);
    arrangement::VectorI L(4 * N);

    for (size_t i = 0; i < N; i++) {
        Eigen::AngleAxisd rot(i * 2 * M_PI / N, axis);
        V.block(4 * i, 0, 4, 3) = (rot.toRotationMatrix() * tet_V.transpose()).transpose();
        F.block(4 * i, 0, 4, 3) = tet_F.array() + 4 * i;
        L.segment(4 * i, 4) = tet_L;
    }
    igl::write_triangle_mesh("test.obj", V, F);
    {
        auto engine = arrangement::Arrangement::create_fast_arrangement(V, F, L);
        engine->run();
        igl::write_triangle_mesh("test.obj", engine->get_vertices(), engine->get_faces());
    }

    //BENCHMARK("FastArrangement")
    //{
    //    auto engine = arrangement::Arrangement::create_fast_arrangement(V, F, L);
    //    engine->run();
    //    return std::make_tuple(engine->get_vertices(), engine->get_faces());
    //};

    //BENCHMARK("MeshArrangement")
    //{
    //    auto engine = arrangement::Arrangement::create_mesh_arrangement(V, F, L);
    //    engine->run();
    //    return std::make_tuple(engine->get_vertices(), engine->get_faces());
    //};
}
