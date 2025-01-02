target_sources(
    ${TARGET_NAME}
    PRIVATE
        CoroApp.cpp
        Application.cpp
        ApplicationProperties.cpp
)

add_subdirectory( platform )
