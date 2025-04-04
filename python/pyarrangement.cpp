#include <arrangement/Arrangement.h>

#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

NB_MODULE(pyarrangement, m)
{
    nb::class_<arrangement::Arrangement>(m, "Arrangement")
        .def_static("create_mesh_arrangement", &arrangement::Arrangement::create_mesh_arrangement)
        .def_static("create_fast_arrangement", &arrangement::Arrangement::create_fast_arrangement)
        .def_static(
            "create_geogram_arrangement", &arrangement::Arrangement::create_geogram_arrangement)
        .def("run", &arrangement::Arrangement::run)
        .def_prop_ro(
            "vertices", &arrangement::Arrangement::get_vertices, nb::rv_policy::reference_internal)
        .def_prop_ro(
            "faces", &arrangement::Arrangement::get_faces, nb::rv_policy::reference_internal)
        .def_prop_ro("face_labels",
            &arrangement::Arrangement::get_out_face_labels,
            nb::rv_policy::reference_internal)
        .def_prop_ro("num_cells", &arrangement::Arrangement::get_num_cells)
        .def("get_cell_faces", &arrangement::Arrangement::get_cell_faces)
        .def_prop_ro("num_patches", &arrangement::Arrangement::get_num_patches)
        .def_prop_ro(
            "patches", &arrangement::Arrangement::get_patches, nb::rv_policy::reference_internal)
        .def_prop_ro(
            "cells", &arrangement::Arrangement::get_cells, nb::rv_policy::reference_internal)
        .def_prop_ro("winding_number", &arrangement::Arrangement::get_winding_number)
        .def_prop_rw("verbose",
            &arrangement::Arrangement::get_verbose,
            &arrangement::Arrangement::set_verbose);
}
