if (TARGET geogram::geogram)
    return()
endif()

set(GEOGRAM_LIB_ONLY "ON" CACHE BOOL " " FORCE)
set(GEOGRAM_WITH_GRAPHICS "OFF" CACHE BOOL " " FORCE)

include(CPM)
CPMAddPackage(
    NAME geogram
    GITHUB_REPOSITORY BrunoLevy/geogram
    GIT_TAG main
)

add_library(geogram::geogram ALIAS geogram)

# Ensure geogram can be correctly installed.
if(SKBUILD)
    if(APPLE)
        set_target_properties(geogram PROPERTIES
            INSTALL_RPATH "@loader_path"
            INSTALL_NAME_DIR "@rpath"
        )
    elseif(UNIX)
        set_target_properties(geogram PROPERTIES
            INSTALL_RPATH "$ORIGIN"
            INSTALL_NAME_DIR "@rpath"
        )
    endif()
endif()
