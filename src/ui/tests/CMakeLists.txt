ttb_add_test(
    TARGET_NAME test_elements
    SOURCES test_elements.cpp
)

target_link_libraries( test_elements PRIVATE ttbUi )
