#pragma once
#include <memory>

#include "EigenTypedef.h"

namespace arrangement {

class Arrangement
{
public:
    typedef std::shared_ptr<Arrangement> Ptr;
    static Ptr create_mesh_arrangement(
        const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels);
    static Ptr create_fast_arrangement(
        const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels);
    static Ptr create_geogram_arrangement(
        const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels);

public:
    /**
     * @brief Constructor
     *
     * @param vertices MatrixFr of size #vertices by 3.
     * @param faces MatrixIr of size #faces by 3.
     * @param face_labels VectorI of size #faces.
     */
    Arrangement(const MatrixFr& vertices, const MatrixIr& faces, const VectorI& face_labels)
        : m_vertices(vertices)
        , m_faces(faces)
        , m_in_face_labels(face_labels)
    {}
    virtual ~Arrangement() = default;

    /**
     * @brief Run the arrangement computation.
     */
    virtual void run() = 0;

    /**
     * @brief Get vertices
     *
     * @note It will return the output vertices after arrangement is run.
     *
     * @return MatrixFr of size #vertices by 3.
     */
    const MatrixFr& get_vertices() const { return m_vertices; }

    /**
     * @brief Get faces
     *
     * @note It will return the output faces after arrangement is run.
     *
     * @return MatrixIr of size #faces by 3.
     */
    const MatrixIr& get_faces() const { return m_faces; }

    /**
     * @brief Get face labels
     *
     * Each output face corresponds to an input face. This vector gives the label
     * of the input face for each output face.
     *
     * @return VectorI of size #faces.
     */
    const VectorI& get_out_face_labels() const { return m_out_face_labels; }

    /**
     * @brief Get the number of cells.
     *
     * @return size_t Number of cells.
     */
    size_t get_num_cells() const
    {
        if (m_cells.rows() > 0)
            return m_cells.maxCoeff() + 1;
        else
            return 0;
    }

    /**
     * @brief Get the faces incident to a given cell.
     *
     * @param cell_id The cell index.
     *
     * @return MatrixIr of size #faces by 3.  Each row gives the vertex indices
     */
    MatrixIr get_cell_faces(const size_t cell_id) const
    {
        const size_t num_faces = m_faces.rows();
        MatrixIr faces(num_faces, 3);
        size_t face_count = 0;
        for (size_t i = 0; i < num_faces; i++) {
            const size_t patch_id = m_patches[i];
            if (m_cells(patch_id, 0) == cell_id) {
                faces.row(face_count) = m_faces.row(i).reverse();
                face_count++;
            } else if (m_cells(patch_id, 1) == cell_id) {
                faces.row(face_count) = m_faces.row(i);
                face_count++;
            }
        }
        faces.conservativeResize(face_count, 3);
        return faces;
    }

    /**
     * @brief Get the per-face cell information.
     *
     * @return MatrixIr of size #faces by 2.  Each row gives the indices of the
     * cells on the positive and negative side of the face.
     */
    const MatrixIr& get_cells() const { return m_cells; }

    /**
     * @brief Get the number of patches.
     *
     * @return size_t Number of patches.
     */
    size_t get_num_patches() const
    {
        if (m_patches.size() > 0)
            return m_patches.maxCoeff() + 1;
        else
            return 0;
    }

    /**
     * @brief Get the patch indices for each face.
     *
     * @return VectorI of size #faces.  Each entry gives the patch index for the
     * corresponding face.
     */
    const VectorI& get_patches() const { return m_patches; }

    /**
     * @brief Get the winding number for each cell.
     *
     * @return MatrixIr of size #cells by 1.  Each entry gives the winding number
     * for the corresponding cell.
     */
    const MatrixIr& get_winding_number() const { return m_winding_number; }

    /**
     * @brief Set verbosity.
     *
     * @param verbose Whether to output progress messages.
     */
    void set_verbose(const bool verbose) { m_verbose = verbose; }

    /**
     * @brief Get verbosity.
     *
     * @return Whether to output progress messages.
     */
    bool get_verbose() const { return m_verbose; }

protected:
    MatrixFr m_vertices;
    MatrixIr m_faces;
    VectorI m_in_face_labels;
    VectorI m_out_face_labels;
    MatrixIr m_cells;
    VectorI m_patches;
    MatrixIr m_winding_number;
    bool m_verbose = false;
};

} // namespace arrangement
