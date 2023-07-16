ttb_add_test(
    TARGET_NAME test_color
    SOURCES test_color.cpp
    DEPENDENCIES ttb::utils
    )

ttb_add_test(
    TARGET_NAME test_coroutine
    SOURCES test_coroutine.cpp
    DEPENDENCIES ttb::utils
)

