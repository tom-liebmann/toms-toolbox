ttb_add_test(
    TARGET_NAME test_Group
    SOURCES
      test_Group.cpp
      Approx.cpp
      TestElement.cpp
    DEPENDENCIES ttb::ui
)
