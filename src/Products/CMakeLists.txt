#
# Set up the products to be built
#

find_package( Boost )

set( DISPLAY_TGT display )

set( DISPLAY_SRC DisplayImage.cpp )

include_directories( ${Boost_INCLUDE_DIRS} )

add_library( ${DISPLAY_TGT} SHARED ${DISPLAY_SRC} )

set( DISPLAY_TGT ${DISPLAY_TGT} PARENT_SCOPE )