## Arrangement Benechmark

This repo interfaces with the following arrangement algorithms:

* Mesh arrangement [^1]
* Fast and robust mesh arrangement [^2]

[^1]: Zhou, Qingnan, et al. "[Mesh arrangements for solid geometry](https://cims.nyu.edu/gcl/papers/zhou2016mas.pdf)." ACM Transactions on Graphics (TOG) 35.4 (2016): 1-15.
[^2]: Cherchi, Gianmarco, et al. "[Fast and robust mesh arrangements using floating-point arithmetic](https://dl.acm.org/doi/abs/10.1145/3414685.3417818)." ACM Transactions on Graphics (TOG) 39.6 (2020): 1-16.

## Build

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DARRANGEMENT_UNIT_TESTS=ON \
         -DARRANGEMENT_BENCHMARKS=ON
make -j
```

To ensure everything is built correctly, run unit tests:

```sh
./arrangement_tests
```

## Benchmarks

Use the following to run benchmarks:

```sh
$ ./arrangement_tests benchmark
```

Here is the benchmark results on MacBook Pro (Apple M3 max, 128G memory):

```
benchmark name                       samples       iterations    estimated
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
FastArrangement                                100             1     52.6855 s
                                        548.791 ms    545.996 ms     551.25 ms
                                        13.3445 ms    11.5365 ms    17.1973 ms

MeshArrangement                                100             1     1.69776 m
                                         1.01601 s     1.01498 s      1.0172 s
                                        5.65358 ms    4.72342 ms    7.14259 ms
```

## Using the library

```c++
#include <arrangement/Arrangement.h>

arrangement::MatrixFr V; // nx3 Vertex matrix
arrangement::MatrixIr F; // mx3 Face matrix
arrangement::VectorI L;  // mx1 Face labels

// Create a mesh arrangement engine.
auto engine = arrangement::Arrangement::create_mesh_arrangement(V, F, L);

// Alternatively, create a fast mesh arrangement engine.
auto engine = arrangement::Arrangement::create_fast_arrangement(V, F, L);

engine->run();
```

To extract the mesh with all intersections resolved:
```c++
auto out_vertices = engine->get_vertices();
auto out_faces = engine->get_faces();
auto out_labels = engine->get_labels();

// Optional: write the mesh to a file
igl::write_triangle_mesh("output.obj", out_vertices, out_faces);
```

To extract individual cells:
```c++
for (size_t i=0; i<engine->get_num_cells(); i++) {
    auto cell_facets = engine->get_cell_faces(i);
    igl::write_triangle_mesh("cell_" + std::to_string(i) + ".obj", out_vertices, cell_facets);
}
```

### License

TBD.
