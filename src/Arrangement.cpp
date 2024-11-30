#include <Arrangement/Arrangement.h>
#include <Arrangement/FastArrangement.h>
#include <Arrangement/MeshArrangement.h>

using namespace Arrangement;

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
