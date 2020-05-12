target_sources(
    ${TARGET_NAME}

    PRIVATE
        BufferedWriter.cpp
        DataCopyReader.cpp
        DataReader.cpp
        DataWriter.cpp
)

add_subdirectory( packets )
