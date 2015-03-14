SET( TENGOM_PATH "" CACHE PATH "Path of the tengom library" ) 

FIND_PATH(
    TENGOM_INCLUDE_DIR
    NAMES tgCore/Version.hpp
    PATHS ${TENGOM_PATH}
    PATH_SUFFIXES include
    DOC "Tengom include directory"
)

FIND_LIBRARY(
    TENGOM_CORE
    NAMES libtgCore.a
    PATHS ${TENGOM_PATH}
    PATH_SUFFIXES tgCore
)

FIND_LIBRARY(
    TENGOM_MATH
    NAMES libtgMath.a
    PATHS ${TENGOM_PATH}
    PATH_SUFFIXES tgMath
)

FIND_LIBRARY(
    TENGOM_NET
    NAMES libtgNet.a
    PATHS ${TENGOM_PATH}
    PATH_SUFFIXES tgNet
)

SET( TENGOM_FULL
    ${TENGOM_CORE}
    ${TENGOM_NET}
    ${TENGOM_MATH}
)
