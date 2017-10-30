#include "GraphicsContextGLFW.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

/// \brief Constructor, doesn't do anything but set default values
GraphicsContextGLFW::GraphicsContextGLFW()
  : initialized_      ( false )
  , extensionsLoaded_ ( false )
{

}


/// \brief Destructor, calls finalize()
GraphicsContextGLFW::~GraphicsContextGLFW()
{

  finalize();

}


/// \brief Calls glfwInit if it hasn't been called yet
bool
GraphicsContextGLFW::initialize()
{

  //
  // Check that we aren't already initialized
  //
  if( !initialized_ )
    {

      //
      // Check that glfwInit worked
      //
      if( !glfwInit() )
        {

          return false;

        }

      // if( !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) )
      //   {

      //     glfwTerminate();
      //     return false;

      //   }
      //
      // Set that we've been initialized
      //
      initialized_ = true;


      return true;

    }

  return false;

}



/// \brief Calls glfwTerminate() if initialize has been called
bool
GraphicsContextGLFW::finalize()
{

   if( initialized_ )
  {

    glfwTerminate();
    initialized_ = false;
    return true;

  }


  return false;

}


/// \brief Returns if there the class has been initialized yet
bool
GraphicsContextGLFW::isInitialized()
{

  return initialized_;

}


/// \brief Loads OpenGL extensions return false if unsuccesfull
bool
GraphicsContextGLFW::loadExtensions()
{

  if( !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) )
  {

    return false;

  }

  extensionsLoaded_ = true;

  return true;

}


/// \brief Return whether the extensions have been loaded
bool
GraphicsContextGLFW::extensionsLoaded()
{

  return extensionsLoaded_;

}