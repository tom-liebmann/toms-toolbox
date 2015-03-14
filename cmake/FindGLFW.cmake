FIND_PATH(
    GLFW_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    DOC "GLFW include directory"
)

FIND_LIBRARY(
    GLFW_LIBRARY
    NAMES libglfw3.a
)
