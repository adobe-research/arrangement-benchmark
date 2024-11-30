if(TARGET igl::core)
    return()
endif()

message(STATUS "Third-party (external): creating target 'igl::core'")

set(LIBIGL_COPYLEFT_CGAL On CACHE BOOL "Build CGAL support in libigl")

include(CPM)
CPMAddPackage(
    NAME libigl
    GITHUB_REPOSITORY libigl/libigl
    GIT_TAG 5067c8b7eb85343af8f53cdeb0bfd1957ad59e04
)
