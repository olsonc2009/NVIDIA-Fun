#include "gtest/gtest.h"


#include "RendererOGL.hpp"
#include "WindowManagerGLFW.hpp"

/// \brief Create a context and make sure the window manager works as we'd expect it to
TEST( UTWindowManager, ContextCreateWindow )
{

  WindowManagerGLFW winManager;

  size_t idx      ( 0 );
  GLFWwindow *pWin( 0 );

  //
  // Create a window
  //
  ASSERT_TRUE(
              winManager.createWindow(
                                      idx,
                                      pWin
                                      )
              );

  //
  // Make sure the window was assigned
  //
  ASSERT_NE( 0, reinterpret_cast< size_t >( pWin ) );

  ASSERT_TRUE( winManager.finalize() );

}



class UTWindowManager_Param : public ::testing::TestWithParam< std::pair< unsigned int, unsigned int > >
{

public:


};


TEST_P( UTWindowManager_Param, TestRenderings )
{

  std::pair< unsigned int, unsigned int > renderLocation = GetParam();

  WindowManagerGLFW winManager;
  RendererOGL renderer;

  size_t idx      ( 0 );
  GLFWwindow *pWin( 0 );

  unsigned int width    ( 640 );
  unsigned int height   ( 480 );
  unsigned int channels ( 3 );

  std::vector< float > data( width * height * channels, 0.0f );

  size_t bIdx( ( renderLocation.second * width + renderLocation.first ) * 3 );

  data[ bIdx + 0 ] = .10f;
  data[ bIdx + 1 ] = .20f;
  data[ bIdx + 2 ] = 3.0f;

  std::vector< unsigned int > dataDims;
  dataDims.push_back( width );
  dataDims.push_back( height );

  //
  // Create a window
  //
  ASSERT_TRUE(
              winManager.createWindow(
                                      idx,
                                      pWin,
                                      640,
                                      480,
                                      "TestRenders"
                                      )
              );

  ASSERT_TRUE(
              winManager.renderToWindow(
                                        idx,
                                        data,
                                        dataDims,
                                        channels,
                                        &renderer,
                                        "../Resources/Shaders/"
                                        )
              );

  std::vector< float > outData;
  std::vector< unsigned int > outDataDims;
  unsigned int outChannels;

  ASSERT_TRUE(
              winManager.retrieveRenderedData(
                                              outData,
                                              outDataDims,
                                              outChannels
                                              )
              );

  // Test output dimensions
  ASSERT_EQ( 2       , outDataDims.size() );
  ASSERT_EQ( width   , outDataDims[ 0 ]   );
  ASSERT_EQ( height  , outDataDims[ 1 ]   );
  ASSERT_EQ( channels, outChannels        );

  // Test output data
  ASSERT_EQ( data.size(), outData.size() );

  for( size_t row = 0; row < outDataDims[ 1 ]; ++row )
  {

    for( size_t col = 0; col < outDataDims[ 0 ]; ++col )
    {

      size_t baseIdx( ( row * outDataDims[ 0 ] + col ) * 3 );

      if( data[ baseIdx ] != outData[ baseIdx ] )
      {

        FAIL() << row << ", " << col << " red truth = " << data[ baseIdx ] << " found " << outData[ baseIdx ] << std::endl;

      }

      if( data[ baseIdx + 1 ] != outData[ baseIdx + 1 ] )
      {

        FAIL() << row << ", " << col << " green truth = " << data[ baseIdx + 1 ] << " found " << outData[ baseIdx + 1 ] << std::endl;

      }

      if( data[ baseIdx + 2 ] != outData[ baseIdx + 2 ] )
      {

        FAIL() << row << ", " << col << " blue truth = " << data[ baseIdx + 2 ] << " found " << outData[ baseIdx + 2 ] << std::endl;

      }

    }

  }
}


INSTANTIATE_TEST_CASE_P(
                        TestRenderings,
                        UTWindowManager_Param,
                        ::testing::Values(
                                          std::pair< unsigned int, unsigned int >( 40, 30 ),
                                          std::pair< unsigned int, unsigned int >( 30, 40 ),
                                          std::pair< unsigned int, unsigned int >( 100, 221 )
                                          )
                        );