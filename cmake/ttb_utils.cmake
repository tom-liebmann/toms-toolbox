include_guard( GLOBAL )

macro( ttb_define_map MAP_NAME )
    set( _ITEMS "${ARGN}" )
    while( _ITEMS )
        list( POP_FRONT _ITEMS _KEY _VALUE )
        set( "${MAP_NAME}_${_KEY}" "${_VALUE}" )
    endwhile()
endmacro()

function( ttb_define_target_property )
    cmake_parse_arguments( ARGS
        "REQUIRED"                              # Options
        "TARGET;PROPERTY;BRIEF_DOCS;FULL_DOCS"  # One value keywords
        ""                                      # Multi value keywords
        ${ARGN}
    )

    define_property(
        TARGET PROPERTY "${ARGS_PROPERTY}"
        BRIEF_DOCS "${ARGS_BRIEF_DOCS}"
        FULL_DOCS "${ARGS_FULL_DOCS}"
    )

    if( "${ARGS_REQUIRED}" )
        add_custom_target(
            check_property_${ARGS_PROPERTY}_${ARGS_TARGET}
            COMMAND $<IF:$<BOOL:$<TARGET_PROPERTY:${ARGS_TARGET},${ARGS_PROPERTY}>>,true,false> || (echo "Missing property ${ARGS_PROPERTY} for target ${ARGS_TARGET}" && false)
        )
        add_dependencies( ${ARGS_TARGET} check_property_${ARGS_PROPERTY}_${ARGS_TARGET})
    endif()
endfunction()
