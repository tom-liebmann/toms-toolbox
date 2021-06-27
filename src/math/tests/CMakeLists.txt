ttb_add_test(
    TARGET_NAME test_vector
    SOURCES test_vector.cpp
)

target_link_libraries( test_vector PRIVATE ttbMath )


ttb_add_test(
    TARGET_NAME test_range
    SOURCES test_range.cpp
)

target_link_libraries( test_range PRIVATE ttbMath )
