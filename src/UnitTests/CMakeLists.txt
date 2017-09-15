#
# Add gtest target if it was found
#
if( GTEST_FOUND )

    set( TEST_FILES UnitTest0.cpp )

    add_executable( testSuite ${TEST_FILES} )
    target_link_libraries( testSuite GTest::GTest GTest::Main )

    install(
      TARGETS testSuite
      DESTINATION ${BIN_DIR}
      )

endif()
