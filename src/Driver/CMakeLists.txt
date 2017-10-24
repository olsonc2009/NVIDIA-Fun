set( SRC_FILES main.cpp )
set( WIN_SRC_FILES WindowDriver.cpp )


include_directories( ${ENGINE_DIR} )


#
# Standard driver
#
add_executable( driver ${SRC_FILES} )

#
# Window Manager Productization
#
add_executable( WinManager ${WIN_SRC_FILES} )

target_link_libraries( WinManager ${ENGINE_TGT} ${GLAD_TGT} )

add_dependencies( WinManager ${ENGINE_TGT} ${GLAD_TGT} )

#
# Install paths
#
install(
  TARGETS driver
  DESTINATION ${BIN_DIR}
  )

