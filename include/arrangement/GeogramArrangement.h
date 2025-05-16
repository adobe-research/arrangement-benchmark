#pragma once

#ifdef ARRANGEMENT_GEOGRAM

#include "Arrangement.h"

namespace arrangement {

class GeogramArrangement final : public Arrangement
{
public:
    using Base = Arrangement;

public:
    GeogramArrangement(const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels)
        : Base(vertices, faces, face_labels)
    {}
    ~GeogramArrangement() = default;

    void run() override;

private:
    using Base::m_cells;
    using Base::m_faces;
    using Base::m_in_face_labels;
    using Base::m_out_face_labels;
    using Base::m_patches;
    using Base::m_vertices;
    using Base::m_winding_number;
};

} // namespace arrangement

#endif // ARRANGEMENT_GEOGRAM
