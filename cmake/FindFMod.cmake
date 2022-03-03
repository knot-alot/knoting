# FMod_INCLUDE_DIR -- include
# FMod_LIBRARY     -- libs

find_path(FMod_INCLUDE_DIR
    NAMES
        "fmod.h"
    HINTS
        ${CMAKE_SOURCE_DIR}/lib/include
        ${FMod_HOME}/include
        $ENV{FMod_HOME}/include
    PATHS
        "$ENV{PROGRAMFILES}"
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
    DOC
        "FMod - Headers"
)

if (WIN32)
    find_library(FMod_LIBRARY
        NAMES
            fmod_vc
        HINTS
            ${CMAKE_SOURCE_DIR}/lib/x86_64/windows
            ${FMod_HOME}
            $ENV{FMod_HOME}
        PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw
            /opt/local
            /opt/csw
            /opt
        PATH_SUFFIXES
            lib
        DOC
            "FMod - Library"
    )

    find_package_handle_standard_args(FMod REQUIRED_VARS FMod_INCLUDE_DIR FMod_LIBRARY)

    mark_as_advanced(FMod_LIBRARY FMod_INCLUDE_DIR FMod_SHARED_LIBRARY)
else()
    find_library(FMod_SHARED_LIBRARY
        NAMES
            fmod
        HINTS
            ${CMAKE_SOURCE_DIR}/lib/x86_64/linux
            ${FMod_HOME}
            $ENV{FMod_HOME}
        PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw
            /opt/local
            /opt/csw
            /opt
        PATH_SUFFIXES
            so
        DOC
            "FMod - Shared Library"
    )

    find_package_handle_standard_args(FMod REQUIRED_VARS FMod_INCLUDE_DIR)

    mark_as_advanced(FMod_INCLUDE_DIR FMod_SHARED_LIBRARY)
endif()

add_library(FMod SHARED IMPORTED)

set_target_properties(FMod
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES
            "${FMod_INCLUDE_DIR}"
         IMPORTED_LINK_INTERFACE_LANGUAGES
             "C++"
)

if(WIN32)
    set_target_properties(FMod
        PROPERTIES
            IMPORTED_IMPLIB
                "${FMod_LIBRARY}"
            IMPORTED_LOCATION
                "${FMod_SHARED_LIBRARY}"
    )
else()
    set_target_properties(FMod
        PROPERTIES
            IMPORTED_LOCATION
                "${FMod_SHARED_LIBRARY}"
    )
endif()
