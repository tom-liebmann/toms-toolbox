ttb_add_test(
    TARGET_NAME test_Pointer
    SOURCES test_Pointer.cpp
    DEPENDENCIES ttb::utils
)

ttb_add_test(
    TARGET_NAME test_Reference
    SOURCES test_Reference.cpp
    DEPENDENCIES ttb::utils
)
