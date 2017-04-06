CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( toms_toolbox )


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



SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}/include" )

FILE(
    GLOB
    SUBDIRECTORIES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src/ttb"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ttb/*"
)

COPY_HEADER(
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)

FOREACH( SUBDIRECTORY ${SUBDIRECTORIES} )
    ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/src/ttb/${SUBDIRECTORY}" )
ENDFOREACH()



# Export build tree to Targets.cmake file.
EXPORT(
    TARGETS ttbCore ttbMath ttbNet ttbTools
    FILE "${PROJECT_BINARY_DIR}/TTBTargets.cmake"
)

CONFIGURE_FILE(
    TTBConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/TTBConfig.cmake"
)
