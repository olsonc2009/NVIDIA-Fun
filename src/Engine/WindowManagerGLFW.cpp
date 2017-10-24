#include "WindowManagerGLFW.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


/// \brief Constructor, does nothing until initialize
WindowManagerGLFW::WindowManagerGLFW( WindowRendererOGL* pWinRenderer )
  : nextWindowID_( 0 )
{

}


/// \brief Destructor, calls finalize
WindowManagerGLFW::~WindowManagerGLFW()
{

  finalize();

}

///
/// \brief Create a new managed window with the specified parameters
///
/// \param size_t &retId windowID that will be returned if the function is succesful
/// \todo finish documentation
bool
WindowManagerGLFW::createWindow(
                                size_t &retID,
                                GLFWwindow*& pRetWin,
                                const size_t winHeight,
                                const size_t winWidth,
                                const std::string winTitle,
                                const int openGlMajorVersion,
                                const int openGlMinorVersion
                                )
{

  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, openGlMajorVersion );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, openGlMinorVersion );
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* pWindow = glfwCreateWindow(
                                         winHeight,
                                         winWidth,
                                         winTitle.c_str(),
                                         NULL,  // GLFWmonitor* monitor
                                         NULL   // GLFWwindow* share
                                         );

  if( pWindow )
  {

    //
    // Add the window to our map and assign the return variables
    //
    idToWindowMap_[ nextWindowID_ ] = pWindow;
    retID = nextWindowID_;

    pRetWin = pWindow;

    //
    // Increment our ID to the next one
    //
    ++nextWindowID_;

    //
    // Set the swap interval to 0 so we render as fast as possible
    //
    if( this->grabWindow( retID ) )
      {
        glfwSwapInterval( 0 );
      }

    return true;

  }

  retID = 0;
  return false;

}

/// \brief Make the window index current
/// \return true if the window exists false otherwise
///
bool
WindowManagerGLFW::grabWindow( size_t windowIdx )
{

  if( idToWindowMap_.find( windowIdx ) != idToWindowMap_.end() )
  {

    glfwMakeContextCurrent( idToWindowMap_[ windowIdx ] );

  }
  else
  {

    return false;

  }

  return true;

}

/// \brief Destroy any managed windows
bool
WindowManagerGLFW::finalize()
{

  //
  // Destroy any windows we have tracked
  //
  for( std::map< size_t, GLFWwindow* >::iterator iter = idToWindowMap_.begin(); iter != idToWindowMap_.end(); ++iter )
    {

      glfwDestroyWindow( iter->second );

    }

  //
  // Clear the map now that we are done with it
  //
  idToWindowMap_.clear();

  return true;

}

bool
WindowManagerGLFW::swapBuffers( size_t windowIdx )
{

  GLFWwindow* pRet = getWindow( windowIdx );

  if( pRet == 0 )
  {

    return false;

  }

  glfwSwapBuffers( pRet );

  return true;

}

bool
WindowManagerGLFW::setInputMode( size_t windowIdx, unsigned int mode )
{

  GLFWwindow* pWindow = getWindow( windowIdx );

  if( pWindow == 0 )
  {
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(
                    pWindow,
                    mode,
                    GL_TRUE
                    );

}


/// \brief Returns a GLFWwindow pointer if the it exists, if it doesn't it returns a null pointer so make sure to check for that!
GLFWwindow*
WindowManagerGLFW::getWindow( size_t windowIdx )
{

  std::map< size_t, GLFWwindow* >::iterator iter = idToWindowMap_.find( windowIdx );

  if( iter != idToWindowMap_.end() )
  {

    return iter->second;

  }

  return 0;

}

