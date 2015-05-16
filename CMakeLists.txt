CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( tengom )

FUNCTION( COPY_HEADER SOURCE DESTINATION )
    FILE(
        GLOB
        CHILDREN
        RELATIVE ${SOURCE}
        ${SOURCE}/*
    )

    FOREACH( CHILD ${CHILDREN} )
        IF( IS_DIRECTORY ${SOURCE}/${CHILD} )
            COPY_HEADER(
                ${SOURCE}/${CHILD}
                ${DESTINATION}/${CHILD}
            )
        ELSE()
            IF( ${SOURCE}/${CHILD} MATCHES "^.*[.]hpp$")
                FILE(
                    COPY
                    ${SOURCE}/${CHILD}
                    DESTINATION ${DESTINATION}
                )
            ENDIF()
        ENDIF()
    ENDFOREACH()
ENDFUNCTION( COPY_HEADER )

FUNCTION( ADD_SUBLIBRARY LIB_NAME )
    # build library
    ADD_SUBDIRECTORY( ${LIB_NAME} )

    # copy header files
    COPY_HEADER(
        ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )
ENDFUNCTION( ADD_SUBLIBRARY )

SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

# All sublibraries to build
SET( SUBLIBRARIES "tgCore" "tgMath" "tgNet" )

# Copy header files of sublibraries into a single include directory.
FOREACH( SUBLIBRARY ${SUBLIBRARIES} )
    COPY_HEADER(
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${SUBLIBRARY}"
        "${CMAKE_CURRENT_BINARY_DIR}/include/${SUBLIBRARY}"
    )
ENDFOREACH()

# Set compilation flags used by all libraries.
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
IF( WIN32 )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32" )
ENDIF()

# Add sublibraries.
FOREACH( SUBLIBRARY ${SUBLIBRARIES} )
    ADD_SUBDIRECTORY( "src/${SUBLIBRARY}" )
ENDFOREACH()

# Export build tree to Targets.cmake file.
SET( CONF_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include" )

EXPORT(
    TARGETS ${SUBLIBRARIES}
    FILE "${PROJECT_BINARY_DIR}/TengomTargets.cmake"
)

CONFIGURE_FILE(
    TengomConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/TengomConfig.cmake"
)
