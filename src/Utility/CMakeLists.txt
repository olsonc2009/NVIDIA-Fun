#
# Build any utilities that we need
#

# setup GLAD
set( GLAD_TGT glad )
set( GLAD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c )
set( GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad/include )

#
# Setup utility library
#
set( UTIL_TGT utility )
set( UTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/utility.cpp )


# Build GLAD
include_directories( ${GLAD_INCLUDE_DIR} )
add_library( ${GLAD_TGT} SHARED ${GLAD_SRC} )

# Build utility library
add_library( ${UTIL_TGT} SHARED ${UTIL_SRC} )

#
# Set these variables to parent scope so everyone else can use them
#
set( GLAD_TGT ${GLAD_TGT} PARENT_SCOPE )
set( UTIL_TGT ${UTIL_TGT} PARENT_SCOPE )
set( GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} PARENT_SCOPE )