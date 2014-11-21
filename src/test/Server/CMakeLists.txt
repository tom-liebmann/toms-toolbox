CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

CMAKE_POLICY( SET CMP0015 NEW )

PROJECT( Server )

# find tengom library
SET( TENGOM_PATH "" CACHE PATH "tengom directory" )
INCLUDE_DIRECTORIES( "${TENGOM_PATH}/src" )

# tgNet
FIND_LIBRARY(
    TGNET_STATIC
    NAMES libtgNet.a
    PATHS "${TENGOM_PATH}/build/tgNet" NO_DEFAULT_PATH )
IF( NOT TGNET_STATIC )
    MESSAGE( FATAL_ERROR "Unable to find tgNet!" )
ENDIF()

# add definitions

# platform dependend stuff
IF( WIN32 )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32" )
ENDIF()

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y -pthread -DCLIENT" )
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Bstatic -static-libgcc -static-libstdc++" )

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}" )

# search sources
FILE(
    GLOB_RECURSE
    SOURCE_FILES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    *.cpp *.c
)

# create executable
ADD_EXECUTABLE(
    server
    ${SOURCE_FILES} )

IF( WIN32 )
    TARGET_LINK_LIBRARIES(
        server
        ${TGNET_STATIC}
        wsock32
        ws2_32 )
ELSE()
    TARGET_LINK_LIBRARIES(
        client
        ${TGCORE_STATIC}
        ${TGNET_STATIC}
        ${TGMATH_STATIC}
        ${GLFW_STATIC}
        ${GLEWX_STATIC}
        Xxf86vm
        pthread
        Xrandr
        Xi
        Xcursor
        X11
        Xinerama
        rt
        ${OPENGL_LIBRARIES} )
ENDIF()
