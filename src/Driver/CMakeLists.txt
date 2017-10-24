set( SRC_FILES main.cpp )
set( WIN_SRC_FILES WindowDriver.cpp )

find_package( Boost REQUIRED program_options )

include_directories( 
					${ENGINE_DIR} 
					${Boost_INCLUDE_DIRS}
					${PRODUCT_DIR}
					)



#
# Standard driver
#
add_executable( driver ${SRC_FILES} )

#
# Window Manager Productization
#
add_executable( WinManager ${WIN_SRC_FILES} )

target_link_libraries( 
					  WinManager 
					  ${ENGINE_TGT} 
					  ${GLAD_TGT} 
					  ${Boost_LIBRARIES} 
					  ${DISPLAY_TGT} 
					  )

add_dependencies( WinManager ${ENGINE_TGT} ${GLAD_TGT} ${DISPLAY_TGT} )

#
# Install paths
#
install(
  TARGETS driver
  DESTINATION ${BIN_DIR}
  )

