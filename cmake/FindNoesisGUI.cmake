# NoesisGUI_INCLUDE_DIR -- include
# NoesisGUI_LIBRARY     -- libs

find_path(NoesisGUI_INCLUDE_DIR
    NAMES
        "NoesisPCH.h"
    HINTS
        ${CMAKE_SOURCE_DIR}/lib/include
        ${NOESIS_HOME}/include
        $ENV{NOESIS_HOME}/include
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
        "Noesis - Headers"
)

if (WIN32)
    find_library(NoesisGUI_LIBRARY
        NAMES
            Noesis
        HINTS
            ${CMAKE_SOURCE_DIR}/lib/x86_64/windows
            ${NOESIS_HOME}
            $ENV{NOESIS_HOME}
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
            "Noesis - Library"
    )

    find_package_handle_standard_args(NoesisGUI REQUIRED_VARS NoesisGUI_INCLUDE_DIR NoesisGUI_LIBRARY)

    mark_as_advanced(NoesisGUI_LIBRARY NoesisGUI_INCLUDE_DIR NoesisGUI_SHARED_LIBRARY)
else()
    find_library(NoesisGUI_SHARED_LIBRARY
        NAMES
            Noesis
        HINTS
            ${CMAKE_SOURCE_DIR}/lib/x86_64/linux
            ${NOESIS_HOME}
            $ENV{NOESIS_HOME}
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
            "Noesis - Shared Library"
    )

    find_package_handle_standard_args(NoesisGUI REQUIRED_VARS NoesisGUI_INCLUDE_DIR)

    mark_as_advanced(NoesisGUI_INCLUDE_DIR NoesisGUI_SHARED_LIBRARY)
endif()

add_library(NoesisGUI SHARED IMPORTED)

set_target_properties(NoesisGUI
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES
            "${NoesisGUI_INCLUDE_DIR}"
         IMPORTED_LINK_INTERFACE_LANGUAGES
             "C++"
)

if(WIN32)
    set_target_properties(NoesisGUI
        PROPERTIES
            IMPORTED_IMPLIB
                "${NoesisGUI_LIBRARY}"
            IMPORTED_LOCATION
                "${NoesisGUI_SHARED_LIBRARY}"
    )
else()
    set_target_properties(NoesisGUI
        PROPERTIES
            IMPORTED_CONFIGURATIONS
                "Debug"
            IMPORTED_LOCATION
                "${NoesisGUI_SHARED_LIBRARY}"
    )
endif()
