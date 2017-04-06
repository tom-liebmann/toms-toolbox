CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( ttbNet )

# find source files
FILE(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    *.cpp
)

# create executable
ADD_LIBRARY(
    ttbNet
    STATIC ${SOURCE_FILES}
)

TARGET_COMPILE_OPTIONS(
    ttbNet
    PRIVATE -std=c++11
)

IF( WIN32 )
    TARGET_LINK_LIBRARIES(
        ttbNet
        PUBLIC
            wsock32
            ws2_32
    )

    TARGET_COMPILE_DEFINITIONS(
        ttbNet
        PRIVATE WIN32
    )
ENDIF()
