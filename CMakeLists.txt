cmake_minimum_required(VERSION 3.15)
project(arrangement)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(FetchContent)
include(Eigen3)
include(fast_arrangement)
include(libigl)

file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB INC_FILES "${PROJECT_SOURCE_DIR}/include/PyMesh/*.h")

add_library(arrangement STATIC ${SRC_FILES} ${INC_FILES})
target_include_directories(arrangement PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(arrangement PUBLIC igl::core igl_copyleft::cgal
    fast_arrangement::fast_arrangement)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(arrangement PRIVATE
        -Wno-undefined-inline
        -Wno-deprecated-builtins
        -Wno-return-stack-address
    )
endif()
