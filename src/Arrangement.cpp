#include <PyMesh/Arrangement.h>
#include <PyMesh/FastArrangement.h>
#include <PyMesh/MeshArrangement.h>

using namespace PyMesh;

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
