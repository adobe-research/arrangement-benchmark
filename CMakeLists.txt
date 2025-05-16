cmake_minimum_required(VERSION 3.15)
project(arrangement)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(SKBUILD)
    if(APPLE)
        set(CMAKE_INSTALL_RPATH @loader_path)
    elseif(UNIX)
	set(CMAKE_INSTALL_RPATH $ORIGIN)
    endif()
endif()

include(Eigen3)

option(ARRANGEMENT_WITH_SANITIZERS "Enable sanitizers" OFF)
if (ARRANGEMENT_WITH_SANITIZERS)
    include(sanitizers)
    add_sanitizer_support(address)
endif()

option(ARRANGEMENT_IGL "Enable mesh arrangement (libigl) support" ON)
option(ARRANGEMENT_FAST "Enable fast arrangement (fast_arrangement) support" OFF)
option(ARRANGEMENT_GEOGRAM "Enable geogram support" OFF)

file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB INC_FILES "${PROJECT_SOURCE_DIR}/include/arrangement/*.h")

add_library(arrangement STATIC ${SRC_FILES} ${INC_FILES})
target_link_libraries(arrangement PUBLIC Eigen3::Eigen)
target_compile_features(arrangement PRIVATE cxx_std_20)
target_include_directories(arrangement PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
set_target_properties(arrangement PROPERTIES POSITION_INDEPENDENT_CODE On)

add_library(arrangement::arrangement ALIAS arrangement)

if (ARRANGEMENT_IGL)
    include(libigl)
    target_link_libraries(arrangement PUBLIC igl::core igl_copyleft::cgal)
    target_compile_definitions(arrangement PUBLIC ARRANGEMENT_IGL)
endif()

if (ARRANGEMENT_FAST)
    include(fast_arrangement)
    target_link_libraries(arrangement PUBLIC fast_arrangement::fast_arrangement)
    target_compile_definitions(arrangement PUBLIC ARRANGEMENT_FAST)
endif()

if (ARRANGEMENT_GEOGRAM)
    include(geogram)
    target_link_libraries(arrangement PUBLIC geogram::geogram)
    target_compile_definitions(arrangement PUBLIC ARRANGEMENT_GEOGRAM)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(arrangement PRIVATE
        -Wno-undefined-inline
        -Wno-deprecated-builtins
        -Wno-return-stack-address
    )
endif()

option(ARRANGEMENT_UNIT_TESTS "Build tests" OFF)
if(ARRANGEMENT_UNIT_TESTS)
    include(CTest)
    enable_testing()
    include(Catch2)

    file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
    add_executable(arrangement_tests ${TEST_FILES})
    target_link_libraries(arrangement_tests PRIVATE arrangement::arrangement Catch2::Catch2WithMain)
    target_compile_definitions(arrangement_tests PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
    target_compile_features(arrangement_tests PRIVATE cxx_std_20)

    catch_discover_tests(arrangement_tests)
endif()

option(ARRANGEMENT_EXAMPLES "Build examples" OFF)
if (ARRANGEMENT_EXAMPLES)
    include(cli11)
    file(GLOB EXAMPLE_FILES "${PROJECT_SOURCE_DIR}/app/compute_arrangement.cpp")
    add_executable(compute_arrangement ${EXAMPLE_FILES})
    target_link_libraries(compute_arrangement PRIVATE arrangement::arrangement CLI11::CLI11)
endif()

option(ARRANGEMENT_BUILD_PYTHON_BINDING "Build Python bindings" OFF)
if (ARRANGEMENT_BUILD_PYTHON_BINDING)
    include(nanobind)
    set(PY_SRC_FILE "${PROJECT_SOURCE_DIR}/python/pyarrangement.cpp")
    nanobind_add_module(pyarrangement NB_STATIC STABLE_ABI ${PY_SRC_FILE})
    target_link_libraries(pyarrangement PRIVATE arrangement::arrangement)
    install(TARGETS pyarrangement LIBRARY DESTINATION .)
    install(TARGETS pyarrangement tbb geogram
        COMPONENT Arrangement_Python_Runtime
        RUNTIME DESTINATION ${SKBUILD_PLATLIB_DIR}/arrangement
        LIBRARY DESTINATION ${SKBUILD_PLATLIB_DIR}/arrangement)
    add_library(arrangement::pyarrangement ALIAS pyarrangement)
endif()
