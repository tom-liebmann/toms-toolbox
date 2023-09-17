ttb_add_test(
    TARGET_NAME test_Group
    SOURCES
      test_Group.cpp
      Approx.cpp
      TestElement.cpp
    DEPENDENCIES ttb::ui
)

ttb_add_test(
    TARGET_NAME test_Linear
    SOURCES
    test_Linear.cpp
    Approx.cpp
    TestElement.cpp
    DEPENDENCIES ttb::ui
)
