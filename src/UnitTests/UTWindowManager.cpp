#include "gtest/gtest.h"


#include "GraphicsContextGLFW.hpp"
#include "WindowManagerGLFW.hpp"

/// \brief Create a context and make sure the window manager works as we'd expect it to
TEST( UTWindowManager, ContextCreateWindow )
{

  GraphicsContextGLFW context;
  WindowManagerGLFW winManager;

  //
  // Initialize the context so the window creation will work
  //
  ASSERT_TRUE( context.initialize() );

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

}


/// \brief Test to ensure the window manager fails if there is no context
TEST( UTWindowManager, CreateWindowWithoutContext )
{

  WindowManagerGLFW winManager;

  size_t idx      ( 0 );
  GLFWwindow *pWin( 0 );

  ASSERT_FALSE(
               winManager.createWindow(
                                       idx,
                                       pWin
                                       )
               );

}