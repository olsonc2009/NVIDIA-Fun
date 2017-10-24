#
# Add gtest target if it was found
#
if( GTEST_FOUND )

    set( 
        TEST_FILES 
        UnitTest0.cpp 
        UTUtility.cpp
        UTGraphicsContext.cpp
        UTWindowManager.cpp
        UTModelPackage.cpp
        )

    add_executable( testSuite ${TEST_FILES} )
    target_link_libraries( testSuite GTest::GTest GTest::Main )

    include_directories( 
                        ${GLFW_DIRS}/include 
                        ${GLAD_INCLUDE_DIR} 
                        ${UTIL_DIR}
                        ${ENGINE_DIR}
                        )

    install(
      TARGETS testSuite
      DESTINATION ${BIN_DIR}
      )

    target_link_libraries( testSuite glfw ${GLFW_LIBRARIES} ${GLAD_TGT} ${UTIL_TGT} ${ENGINE_TGT} )

    add_dependencies( testSuite ${GLAD_TGT} ${UTIL_TGT} ${ENGINE_TGT} )

endif()
