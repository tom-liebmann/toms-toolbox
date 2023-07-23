target_sources(
    ${TARGET_NAME}
    PRIVATE
        ApplicationRunner.cpp
        ConnectionManager.cpp
        AndroidManager.cpp
        AdManager.cpp
        SignInManager.cpp
)

add_subdirectory( android )
