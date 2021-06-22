ttb_add_test(
    TARGET_NAME test_vector
    SOURCES test_vector.cpp
)

target_link_libraries( test_vector PRIVATE ttbMath )
