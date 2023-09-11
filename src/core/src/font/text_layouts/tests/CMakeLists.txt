ttb_add_test(
    TARGET_NAME test_NormalLayout
    SOURCES
        test_NormalLayout.cpp
        TestFontFactory.cpp
    ResultGlyph.cpp
    DEPENDENCIES ttb::core
)

ttb_add_test(
    TARGET_NAME test_BlockLayout
    SOURCES
    test_BlockLayout.cpp
    TestFontFactory.cpp
    ResultGlyph.cpp
    DEPENDENCIES ttb::core
)
