#pragma once
#include <arrangement/Arrangement.h>

inline auto generate_tet()
{
    arrangement::MatrixFr vertices(4, 3);
    // clang-format off
    vertices <<
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1;
    // clang-format on

    arrangement::MatrixIr faces(4, 3);
    // clang-format off
    faces <<
        0, 2, 1,
        0, 1, 3,
        0, 3, 2,
        1, 2, 3;
    // clang-format on

    arrangement::VectorI face_labels(4);
    face_labels << 0, 1, 2, 3;

    return std::make_tuple(vertices, faces, face_labels);
}

template <typename Derived>
auto concatentate_rows(
    const Eigen::PlainObjectBase<Derived>& A, const Eigen::PlainObjectBase<Derived>& B)
{
    Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, Eigen::Dynamic> C(
        A.rows() + B.rows(), A.cols());
    C << A, B;
    return C;
}
