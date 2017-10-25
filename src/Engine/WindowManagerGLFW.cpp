#include "WindowManagerGLFW.hpp"

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GraphicsContextGLFW.hpp"
#include "ScreenPackage.hpp"
#include "RendererOGL.hpp"
#include "utility.hpp"

/// \brief Constructor, does nothing until initialize
WindowManagerGLFW::WindowManagerGLFW( GraphicsContextGLFW* pGraphicsContext )
  : nextWindowID_               ( 0 )
  , pGraphicsContext_           ( pGraphicsContext )
  , initializedRenderingPackage_( false )
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

  // If a graphics context wasn't passed in create one
  if( pGraphicsContext_ == 0 )
  {

    pGraphicsContext_ = new GraphicsContextGLFW();

  }

  //
  // Check if our graphics context has been initialized
  //
  if( !pGraphicsContext_->isInitialized() )
  {

    pGraphicsContext_->initialize();

  }


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

    if( &pRetWin != 0 )
    {

      pRetWin = pWindow;

    }

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


/// \brief Render data to the screen, it should be of size dataDims and have dataDims.size() channels
/// \todo Support other than 3 channels of data
/// \todo Add logging to this alongside cerr
/// \todo Move OpenGL init stuff out of here to RendererOGL
bool
WindowManagerGLFW::renderToWindow(
                                  size_t windowToRenderTo,
                                  const std::vector< float > data,
                                  const std::vector< unsigned int > dataDims,
                                  unsigned int numDataChannels,
                                  RendererOGL* pRenderer
                                  )
{

  bool ret( false );

  // Check for valid dimension sizes
  if( dataDims.size() != 2 )
  {

    std::cerr << "WindowManagerGLFW does not support anything except 2 dimensional data for rendering to screen" << std::endl;
    return false;

  }

  if( dataDims[ 0 ] * dataDims[ 1 ] * numDataChannels != data.size() )
  {

    std::cerr << "data.size() != dataDimensions" << std::endl;
    return false;

  }

  // Check for valid channels of datas
  if( numDataChannels != 3 )
  {

    std::cerr << "WindowManagerGLFW does not support anything but 3 channel data to render" << std::endl;

  }

  //
  // Now that we've checked for input issues, start doing something useful
  //

  //
  // If we haven't initialized a rendering package go ahead and store what we need
  // and initialize the OpenGL related stuff
  //
  if( !initializedRenderingPackage_ )
  {

    // Store attributes of render data and the data itself
    renderWidth_    = dataDims[ 0 ];
    renderHeight_   = dataDims[ 1 ];
    renderChannels_ = numDataChannels;

    renderData_ = data;

    initializedRenderingPackage_ = true;

    //------------------------------------------------------------
    // OpenGL Offscreen FBO init
    //

    //
    // Generate and bind the fbo
    //
    glGenFramebuffers( 1, &fbo_ );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_ );

    //
    // Generate and bind a texture
    //
    glGenTextures( 1, &fboTex_ );
    glBindTexture( GL_TEXTURE_2D, fboTex_ );

    //
    // Assign memory to the texture
    //
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 renderWidth_,
                 renderHeight_,
                 0,
                 GL_RGB,
                 GL_FLOAT,
                 0
                 );

    /// \todo figure out if these tex parameters are really necessary, I thought there was a new way to do this in modern opengl
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //
    // Attach the texture to the framebuffer
    //
    glFramebufferTexture2D(
                           GL_FRAMEBUFFER,           // Attach to the framebuffer currently bound
                           GL_COLOR_ATTACHMENT0,     // Target 0
                           GL_TEXTURE_2D,            // Type of texture we are attaching
                           fboTex_,                   // texture ID to attach
                           0                         // MipMap level
                           );

    //
    // Set the list of drawable buffers for the shaders that are sure to come
    //
    GLenum drawBuffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };

    glDrawBuffers( 1, drawBuffers );

    //
    // Check for issues
    /// \todo clean up if we fail
    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {

      std::cerr << "Bad juju on the framebuffer" << std::endl;
      return false;

    }

    //
    // End OpenGL Offscreen FBO init
    //------------------------------------------------------------



    //------------------------------------------------------------
    // Initialize the model package
    //

    pModelPackage_ = new ModelPackage();

    std::vector< float >        vertexPosVec;
    std::vector< float >        texCoords;
    std::vector< unsigned int > indices;
    std::vector< unsigned int > imageSizeVec;
    std::vector< std::string >  shaderFilenames;

    // Generate what we can for the render package
    utility::generateScreenModelPackage(
                                        vertexPosVec,
                                        texCoords,
                                        indices
                                        );

      // Create the necessary vector for imageSizeVec
    imageSizeVec.push_back( renderWidth_ );
    imageSizeVec.push_back( renderHeight_ );

    // Add the geometry
    pModelPackage_->addGeometry( vertexPosVec );

    // Add the texture to the package
    pModelPackage_->addTexture(
                               renderData_,
                               texCoords,
                               imageSizeVec,
                               numDataChannels
                               );

    // Add the indices to the package
    pModelPackage_->addIndices( indices );

    shaderFilenames.push_back( "../src/UnitTests/testTexVert.glsl" );
    shaderFilenames.push_back( "../src/UnitTests/testTexFrag.glsl" );

    //
    // Add the shaders to the render package
    //
    ret = pModelPackage_->addRendererFilenames( shaderFilenames );

    // Return if bad
    if( !ret )
    {

      std::cerr << "Did not add renderer filenames succesfully" << std::endl;
      return ret;

    }

    //
    // Create the renderable
    //
    ret = pRenderer->initializeRenderable( *pModelPackage_, renderableIdx_ );

    // Return if bad
    if( !ret )
    {

      std::cerr << "Something went wrong initializng the renderable" << std::endl;
      return ret;

    }

    //
    // End Initialize the model package
    //------------------------------------------------------------

  }
  else if( dataDims[ 0 ] != renderWidth_ || dataDims[ 1 ] != renderHeight_ )
  {

    std::cerr << "Data dimensions changed, not supported yet" << std::endl;
    return false;

  }

  //---------------------------------------------------
  // Go about rendering the damn thing
  //

  // Make the window requested active
  this->grabWindow( windowToRenderTo );

  glViewport( 0, 0, renderWidth_, renderHeight_ );

  // Bind the FBO we are going to render to
  glBindFramebuffer( GL_FRAMEBUFFER, fbo_ );

  // Have the renderer do something cool
  pRenderer->renderId( renderableIdx_ );

  // Blit the framebuffer from our FBO to the screen requested
  glBindFramebuffer(
                    GL_READ_FRAMEBUFFER,
                    fbo_
                    );

  glBindFramebuffer(
                    GL_DRAW_FRAMEBUFFER,
                    0
                    );

  glBlitFramebuffer(
                    0, 0, 640, 480,
                    0, 0, 640, 480,
                    GL_COLOR_BUFFER_BIT,
                    GL_LINEAR
                    );

}

