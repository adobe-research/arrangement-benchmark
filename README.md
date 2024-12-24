## Arrangement Benchmark

This repo interfaces with the following arrangement algorithms:

* Mesh arrangement [^1]
* Fast and robust mesh arrangement [^2]

[^1]: Zhou, Qingnan, et al. "[Mesh arrangements for solid geometry](https://cims.nyu.edu/gcl/papers/zhou2016mas.pdf)." ACM Transactions on Graphics (TOG) 35.4 (2016): 1-15.
[^2]: Cherchi, Gianmarco, et al. "[Fast and robust mesh arrangements using floating-point arithmetic](https://dl.acm.org/doi/abs/10.1145/3414685.3417818)." ACM Transactions on Graphics (TOG) 39.6 (2020): 1-16.

## Build

To build the C++ library:

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DARRANGEMENT_UNIT_TESTS=ON \
         -DARRANGEMENT_BENCHMARKS=ON \
         -DARRANGEMENT_EXAMPLES=ON
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
FastArrangement                                100             1     53.7318 s
                                        544.909 ms    543.163 ms    546.648 ms
                                        8.89944 ms    7.82959 ms    10.3866 ms

MeshArrangement                                100             1     1.69022 m
                                         1.01577 s     1.01464 s     1.01707 s
                                        6.19107 ms    5.21611 ms    7.44767 ms

Geogram                                        100             1     1.88286 m
                                         1.05256 s     1.04689 s     1.05797 s
                                        28.2375 ms    24.6452 ms    33.6001 ms
```

## Using the library

```c++
#include <arrangement/Arrangement.h>

arrangement::MatrixFr V = ...; // nx3 Vertex matrix
arrangement::MatrixIr F = ...; // mx3 Face matrix
arrangement::VectorI  L = ...; // mx1 Face labels

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

// Out label for each face is inherited from the parent input face.
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

## Python package

Alternatively, one can install this library as a Python package:

```sh
# From the arrangement root directory
pip install .
```

To use the library:

```python
import arrangement

# Initialize input
V = ... # nx3 Vertex numpy matrix
F = ... # mx3 Face numpy matrix
L = ... # mx1 Face numpy labels

# Create a mesh arrangement engine
engine = arrangement.Arrangement.create_mesh_arrangement(V, F, L)
engine.run()

# Extract the output
out_vertices = engine.vertices
out_faces = engine.faces
out_labels = engine.face_labels
```

You can also invoke a command line script to run the arrangement:

```sh
python -m arrangement --engine mesh -o output.msh input_mesh_1.obj input_mesh_2.ply ...
```

It will compute the arrangement of all input meshes and write the output to `output.msh`.

### License

TBD.
