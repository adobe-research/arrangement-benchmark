if (TARGET absl::base)
    return()
endif()

include(CPM)
CPMAddPackage(
    NAME absl
    GIT_REPOSITORY abseil/abseil-cpp
    GIT_TAG 20230802.1
)

include(FetchContent)
FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG 20230802.1
)
