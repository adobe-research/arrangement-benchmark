#include <arrangement/GeogramArrangement.h>

#include <Eigen/Core>

#include <geogram/basic/attributes.h>
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_surface_intersection.h>

#include <chrono>
#include <iostream>

namespace arrangement {

namespace {

template <typename DerivedV, typename DerivedF, typename DerivedI>
void to_geogram_mesh(Eigen::PlainObjectBase<DerivedV>& V,
    Eigen::PlainObjectBase<DerivedF>& F,
    Eigen::PlainObjectBase<DerivedI>& I,
    GEO::Mesh& M)
{
    M.vertices.create_vertices((int)V.rows());
    for (int i = 0; i < V.rows(); i++) {
        M.vertices.point(i) = GEO::vec3(V(i, 0), V(i, 1), V(i, 2));
    }
    M.facets.create_triangles((int)F.rows());
    for (int i = 0; i < F.rows(); i++) {
        M.facets.set_vertex(i, 0, F(i, 0));
        M.facets.set_vertex(i, 1, F(i, 1));
        M.facets.set_vertex(i, 2, F(i, 2));
    }

    GEO::Attribute<typename DerivedI::Scalar> labels(M.facets.attributes(), "label");
    for (GEO::index_t i : M.facets) {
        labels[i] = I(i);
    }
}

} // namespace

void GeogramArrangement::run()
{
    // Needs to be called once.
    GEO::initialize(GEO::GEOGRAM_INSTALL_ALL);

    auto t_begin = std::chrono::high_resolution_clock::now();

    GEO::Mesh mesh;
    to_geogram_mesh(m_vertices, m_faces, m_in_face_labels, mesh);
    GEO::MeshSurfaceIntersection engine(mesh);
    engine.set_verbose(false);
    engine.set_delaunay(true);
    engine.set_radial_sort(true);
    engine.intersect();
    //engine.remove_external_shell();
    auto t_mid = std::chrono::high_resolution_clock::now();

    m_vertices.resize(mesh.vertices.nb(), 3);
    for (size_t i = 0; i < mesh.vertices.nb(); i++) {
        m_vertices.row(i) << mesh.vertices.point(i)[0], mesh.vertices.point(i)[1],
            mesh.vertices.point(i)[2];
    }

    m_faces.resize(mesh.facets.nb(), 3);
    for (size_t i = 0; i < mesh.facets.nb(); i++) {
        m_faces.row(i) << mesh.facets.vertex(i, 0), mesh.facets.vertex(i, 1),
            mesh.facets.vertex(i, 2);
    }

    GEO::Attribute<typename VectorI::Scalar> labels(mesh.facets.attributes(), "label");
    GEO::Attribute<GEO::index_t> chart(mesh.facets.attributes(), "chart");

    m_out_face_labels.resize(labels.size());
    for (size_t i = 0; i < labels.size(); i++) {
        m_out_face_labels[i] = labels[i];
    }

    m_patches.resize(chart.size());
    size_t num_patches = 0;
    for (size_t i = 0; i < chart.size(); i++) {
        m_patches[i] = chart[i];
        num_patches = std::max(num_patches, (size_t)chart[i] + 1);
    }

    m_cells.resize(num_patches, 2);
    m_cells.setConstant(-1);
    for (size_t i = 0; i < num_patches; i++) {
        m_cells(i, 1) = i;
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    if (m_verbose) {
        std::chrono::duration<double> resolve_time = t_mid - t_begin;
        std::chrono::duration<double> extract_time = t_end - t_mid;
        std::cout << "Arrangement: resolving self-intersection: " << resolve_time.count()
                  << std::endl;
        std::cout << "Arrangement: extracting arrangement: " << extract_time.count() << std::endl;
    }
}

} // namespace arrangement
