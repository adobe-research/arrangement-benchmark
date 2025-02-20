if(TARGET igl::core)
    return()
endif()

message(STATUS "Third-party (external): creating target 'igl::core'")

set(LIBIGL_COPYLEFT_CGAL On CACHE BOOL "Build CGAL support in libigl")

include(CPM)
CPMAddPackage(
    NAME libigl
    GITHUB_REPOSITORY libigl/libigl
    GIT_TAG ba69acc509632c56c738f03440841cc37da50497
)
