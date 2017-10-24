set( SRC_FILES main.cpp )

add_executable( driver ${SRC_FILES} )

install(
  TARGETS driver
  DESTINATION ${BIN_DIR}
  )
