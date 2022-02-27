ttb_add_test(
    TARGET_NAME test_vector
    SOURCES test_vector.cpp
    DEPENDENCIES ttbMath
    )


ttb_add_test(
    TARGET_NAME test_matrix
    SOURCES test_matrix.cpp
    DEPENDENCIES ttbMath
    )

ttb_add_test(
    TARGET_NAME test_range
    SOURCES test_range.cpp
    DEPENDENCIES ttbMath
    )

ttb_add_test(
    TARGET_NAME test_bezier
    SOURCES test_bezier.cpp
    DEPENDENCIES ttbMath
)

ttb_add_test(
    TARGET_NAME test_utils
    DEPENDENCIES ttbMath
    SOURCES test_utils.cpp
    )
