if(TARGET CLI11::CLI11)
    return()
endif()

message(STATUS "Third-party (external): creating target 'CLI11::CLI11'")

include(CPM)
CPMAddPackage(
    NAME cli11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.4.2
)
