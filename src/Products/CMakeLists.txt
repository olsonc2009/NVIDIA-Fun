#
# Set up the products to be built
#

find_package( Boost )
find_package( PNG )

set( DISPLAY_TGT display )

set( DISPLAY_SRC DisplayImage.cpp )

add_definitions( ${PNG_DEFINITIONS} )

include_directories( 
                    ${Boost_INCLUDE_DIRS} 
                    ${PNG_INCLUDE_DIRS}
                    ${UTIL_DIR}
                    ${ENGINE_DIR}
                    )

add_library( ${DISPLAY_TGT} SHARED ${DISPLAY_SRC} )

target_link_libraries( ${DISPLAY_TGT} ${PNG_LIBRARIES} ${ENGINE_TGT} )

add_dependencies( ${DISPLAY_TGT} ${ENGINE_TGT} )

set( DISPLAY_TGT ${DISPLAY_TGT} PARENT_SCOPE )
