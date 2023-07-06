ttb_add_test(
    TARGET_NAME test_vector
    SOURCES test_vector.cpp
    DEPENDENCIES ttb::math
    )


ttb_add_test(
    TARGET_NAME test_matrix
    SOURCES test_matrix.cpp
    DEPENDENCIES ttb::math
    )

ttb_add_test(
    TARGET_NAME test_range
    SOURCES test_range.cpp
    DEPENDENCIES ttb::math
    )

ttb_add_test(
    TARGET_NAME test_bezier
    SOURCES test_bezier.cpp
    DEPENDENCIES ttb::math
)

ttb_add_test(
    TARGET_NAME test_utils
    DEPENDENCIES ttb::math
    SOURCES test_utils.cpp
    )
