if(TARGET fast_arrangement::fast_arrangement)
    return()
endif()

message(STATUS "Third-party (external): creating target 'fast_arrangement::fast_arrangement'")

include(indirect_predicates)
include(cinolib)
include(Eigen3)
#include(tbb)

include(CPM)
CPMAddPackage(
    NAME fast_arrangement
    GITHUB_REPOSITORY gcherchi/FastAndRobustMeshArrangements
    GIT_TAG bf7eb71da991a61ff5414946a4b2754bbd327e41
    DOWNLOAD_ONLY YES
)

set(TBB_TEST OFF CACHE BOOL " " FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL " " FORCE)
add_subdirectory(${fast_arrangement_SOURCE_DIR}/external/oneTBB)

add_library(fast_arrangement
    ${cinolib_SOURCE_DIR}/external/shewchuk_predicates/shewchuk.c)
target_link_libraries(fast_arrangement
    PUBLIC Eigen3::Eigen indirect_predicates::indirect_predicates tbb)
target_include_directories(fast_arrangement
    PUBLIC
    ${cinolib_SOURCE_DIR}/include
    ${fast_arrangement_SOURCE_DIR}/code
    ${fast_arrangement_SOURCE_DIR}/external/abseil-cpp
    )
#target_compile_options(fast_arrangement PRIVATE -O0)
target_compile_definitions(fast_arrangement PUBLIC CINOLIB_USES_EXACT_PREDICATES)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(fast_arrangement PUBLIC -DIS64BITPLATFORM)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(fast_arrangement PRIVATE
        -Wno-undefined-inline
        -Wno-deprecated-non-prototype
        -Wno-deprecated-builtins
        -Wno-return-stack-address
    )
endif()

add_library(fast_arrangement::fast_arrangement ALIAS fast_arrangement)
