CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( tgCore )

# GLFW
SET( GLFW_PATH "" CACHE PATH "GLFW directory" )
INCLUDE_DIRECTORIES( "${GLFW_PATH}/include" )

FIND_LIBRARY(
    GLFW_STATIC
    NAMES libglfw3.a
    PATHS "${GLFW_PATH}/src" NO_DEFAULT_PATH )

IF( NOT GLFW_STATIC )
    MESSAGE( FATAL_ERROR "GLFW not found!" )
ENDIF()

# OpenGL
FIND_PACKAGE( OpenGL )

IF( NOT OPENGL_FOUND )
    MESSAGE( SEND_ERROR "OpenGL not found!" )
ENDIF()

# platform dependend stuff
IF( WIN32 )
    # glew
    SET( GLEW_PATH "" CACHE PATH "GLEW directory" )
    INCLUDE_DIRECTORIES( "${GLEW_PATH}/include" )

    FIND_LIBRARY(
        GLEWX_STATIC
        NAMES libglew32mx.a
        PATHS "${GLEW_PATH}/lib/" NO_DEFAULT_PATH)

    IF( NOT GLEWX_STATIC )
        MESSAGE( FATAL_ERROR "glewX not found!" )
    ENDIF()

    FIND_LIBRARY(
        GLEW_STATIC
        NAMES libglew32.a
        PATHS "${GLEW_PATH}/lib/" NO_DEFAULT_PATH)

    IF( NOT GLEW_STATIC )
        MESSAGE( FATAL_ERROR "glew not found!" )
    ENDIF()

    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32 -DGLEW_STATIC" )
ENDIF()

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y" )
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Bstatic -static-libgcc -static-libstdc++" )

# find source files
FILE(
    GLOB_RECURSE
    SOURCE_FILES
    ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}"
    *.cpp )

# create executable

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/../" )

ADD_LIBRARY( tgCore STATIC ${SOURCE_FILES} )

IF( WIN32 )
    TARGET_LINK_LIBRARIES(
        tgCore
        ${GLFW_STATIC}
        ${GLEW_STATIC}
        ${OPENGL_LIBRARIES} )
ELSE()
    TARGET_LINK_LIBRARIES(
        tgCore
        ${GLFW_STATIC}
        ${OPENGL_LIBRARIES} )
ENDIF()
