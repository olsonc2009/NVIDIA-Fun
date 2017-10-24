#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "utility.hpp"

/// \todo Create a load shaders routine
TEST( UTUtility, LoadTextFromFile_Nominal )
{

  //
  // Load a test shader
  /// \todo This path needs to change or something
  const std::string filename( "../src/UnitTests/testVert.glsl" );

  std::vector< std::string > lines;

  ASSERT_TRUE(
              utility::loadTextFileToVector(
                                            filename,
                                            lines
                                            )
              );

  ASSERT_EQ( 12, lines.size() );
  ASSERT_EQ( std::string( "// Awesome sauce vertex shader" ), lines[ 0 ] );
  ASSERT_EQ( std::string( "" ), lines[ 7 ] );
  ASSERT_EQ( std::string( "  gl_Position.w   = 1.0;" ), lines[ 9 ] );

}


TEST( UTUtility, ConvertVectorToString_Nominal )
{


  std::vector< std::string > input;
  std::string outString;

  input.push_back( "1" );
  input.push_back( "2" );

  ASSERT_TRUE(
              utility::convertVectorToString(
                                              input,
                                              outString
                                              )
              );

  ASSERT_EQ( "1\n2\n", outString );

}


TEST( UTUtility, ConvertVectorToString_Failure )
{


  std::vector< std::string > input;
  std::string outString;



  ASSERT_FALSE(
              utility::convertVectorToString(
                                              input,
                                              outString
                                              )
              );

  ASSERT_EQ( "", outString );

}