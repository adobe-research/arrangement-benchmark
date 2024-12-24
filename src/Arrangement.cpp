#include <arrangement/Arrangement.h>
#include <arrangement/FastArrangement.h>
#include <arrangement/MeshArrangement.h>
#include <arrangement/GeogramArrangement.h>

using namespace arrangement;

Arrangement::Ptr Arrangement::create_mesh_arrangement(
    const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels)
{
    return std::make_shared<MeshArrangement>(vertices, faces, face_labels);
}

Arrangement::Ptr Arrangement::create_fast_arrangement(
    const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels)
{
    return std::make_shared<FastArrangement>(vertices, faces, face_labels);
}

Arrangement::Ptr Arrangement::create_geogram_arrangement(
    const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels)
{
    return std::make_shared<GeogramArrangement>(vertices, faces, face_labels);
}
