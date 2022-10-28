if(Bink_INCLUDE_DIR AND Bink_LIBRARY)
    set(Bink_FIND_QUIETLY TRUE)
endif()

find_path(Bink_INCLUDE_DIR bink.h)

find_library(Bink_LIBRARY NAMES binkw32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Bink DEFAULT_MSG Bink_LIBRARY Bink_INCLUDE_DIR)

if(Bink_FOUND)
    set(Bink_LIBRARIES ${Bink_LIBRARY})
    set(Bink_INCLUDE_DIRS ${Bink_INCLUDE_DIR})
    if (NOT TARGET Bink::Bink)
        add_library(Bink::Bink UNKNOWN IMPORTED)
        set_target_properties(Bink::Bink PROPERTIES
            IMPORTED_LOCATION "${Bink_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${Bink_INCLUDE_DIR}")
    endif()
endif()
