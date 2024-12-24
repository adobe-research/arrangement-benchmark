if (TARGET absl::base)
    return()
endif()

include(CPM)
CPMAddPackage(
    NAME absl
    GIT_REPOSITORY abseil/abseil-cpp
    GIT_TAG 20230802.1
)

