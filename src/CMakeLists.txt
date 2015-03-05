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
        ${CMAKE_CURRENT_BINARY_DIR}/include/${LIB_NAME}
    )
ENDFUNCTION( ADD_SUBLIBRARY )

ADD_SUBLIBRARY( "tgCore" )
ADD_SUBLIBRARY( "tgMath" )
ADD_SUBLIBRARY( "tgNet" )
