ttb_add_test(
    TARGET_NAME test_vector
    SOURCES test_vector.cpp
)

if( TARGET test_vector )
    target_link_libraries( test_vector PRIVATE ttbMath )
endif()


ttb_add_test(
    TARGET_NAME test_matrix
    SOURCES test_matrix.cpp
    )

if( TARGET test_matrix )
    target_link_libraries( test_matrix PRIVATE ttbMath )
endif()


ttb_add_test(
    TARGET_NAME test_range
    SOURCES test_range.cpp
)

if( TARGET test_range )
    target_link_libraries( test_range PRIVATE ttbMath )
endif()


ttb_add_test(
    TARGET_NAME test_bezier
    SOURCES test_bezier.cpp
)

if( TARGET test_bezier )
    target_link_libraries( test_bezier PRIVATE ttbMath )
endif()


ttb_add_test(
    TARGET_NAME test_utils
    SOURCES test_utils.cpp
)

if( TARGET test_utils )
    target_link_libraries( test_utils PRIVATE ttbMath )
endif()
