if(TARGET cinolib::cinolib)
    return()
endif()

message(STATUS "Third-party (external): creating target 'cinolib::cinolib'")

include(CPM)
CPMAddPackage(
    NAME cinolib
    GITHUB_REPOSITORY mlivesu/cinolib
    GIT_TAG 8174c95b590d083b3699e4af95901c51344266ec
    DOWNLOAD_ONLY YES
)

add_library(cinolib INTERFACE)
target_include_directories(cinolib INTERFACE ${cinolib_SOURCE_DIR}/include)

add_library(cinolib::cinolib ALIAS cinolib)
