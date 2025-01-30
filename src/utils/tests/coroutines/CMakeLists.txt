ttb_add_test(
    TARGET_NAME test_exception_injection
    SOURCES test_exception_injection.cpp
    DEPENDENCIES ttb::utils
)

ttb_add_test(
    TARGET_NAME test_copy
    SOURCES test_copy.cpp
    DEPENDENCIES ttb::utils
)
