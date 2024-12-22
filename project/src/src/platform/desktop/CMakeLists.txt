target_sources(
    ${TARGET_NAME}
    PRIVATE
        ApplicationRunner.cpp
        ConnectionManager.cpp
        AdManager.cpp
        SignInManager.cpp
)

target_compile_options( ${TARGET_NAME} PUBLIC -fcoroutines )
