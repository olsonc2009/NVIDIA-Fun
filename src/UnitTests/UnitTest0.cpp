#include <map>
#include <string>
#include <fstream>
#include <vector>

#include "gtest/gtest.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "utility.hpp"

#include "GraphicsContextGLFW.hpp"
#include "WindowManagerGLFW.hpp"
#include "RendererOGL.hpp"

class WindowRendererOGL
{

public:

  WindowRendererOGL( size_t renderHeight, size_t renderWidth)
    :
    renderHeight_( renderHeight )
    , renderWidth_( renderWidth )
  {}
  virtual ~WindowRendererOGL() {}

  virtual void initialize()
  {



  }

protected:

  size_t renderHeight_;
  size_t renderWidth_;
  unsigned int winFboID_;
  unsigned int winTextureID_;

};


/// \brief Actually render a triangle
/// \todo Figure out where the right place is to initialize glew / glad as it looks like it needs a window from GLFW to work correctly
///
TEST( UnitTest0, RenderTriangle )
{

  GraphicsContextGLFW context;
  WindowManagerGLFW winManager;
  RendererOGL renderer;

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


  // initialize glad
  if( !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) )
  {

    glfwTerminate();
    FAIL() << "Didn't initialize glad correctly" << std::endl;
  }

  //
  // Now we need to render something to the FBO, in this case that something will be a texture
  //
  static const GLfloat g_vertex_buffer_data[] = {
     -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
  };

  // Ensure we can capture the escape key being pressed below
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  winManager.setInputMode( idx, GLFW_STICKY_KEYS );

  //glfwMakeContextCurrent(pWin);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // Link the shader program
  std::vector< std::string > shaderFilenames;
  shaderFilenames.push_back( "../src/UnitTests/testVert.glsl" );
  shaderFilenames.push_back( "../src/UnitTests/testFrag.glsl" );
  unsigned int programId( 0 );

  ASSERT_TRUE(
              renderer.createProgram(
                                     shaderFilenames,
                                     programId
                                     )
              );

  std::cout << "Staring renderer loop" << std::endl;

  // Do the rendeiring for now in a loop
  // do
  {

    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram( programId );

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                         0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                         3,                  // size
                         GL_FLOAT,           // type
                         GL_FALSE,           // normalized?
                         0,                  // stride
                         (void*)0            // array buffer offset
                         );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    // Swap buffers
    ASSERT_TRUE( winManager.swapBuffers( idx ) ); //
    //glfwSwapBuffers(pWin);
    glfwPollEvents();

  }
  // while(
  //       glfwGetKey(pWin, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
  //       glfwWindowShouldClose(pWin) == 0
  //       );

}



/// \brief Actually render something
/// \todo Figure out where the right place is to initialize glew / glad as it looks like it needs a window from GLFW to work correctly
///
TEST( UnitTest0, RenderTexture )
{

  GraphicsContextGLFW context;
  WindowManagerGLFW winManager;
  RendererOGL renderer;
  ModelPackage modelPackage;

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


  // initialize glad
  if( !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) )
  {

    glfwTerminate();
    FAIL() << "Didn't initialize glad correctly" << std::endl;
  }



  //
  // INITIALIZE
  //
  unsigned int renderHeight( 640 );
  unsigned int renderWidth( 480 );

  unsigned int fbo( 0 );
  unsigned int fboTex( 0 );
  unsigned int toRenderTex( 0 );
  unsigned int vbo( 0 );
  unsigned int vao( 0 );
  unsigned int renderBuffer( 0 );

  // Generate the texture to render
  std::vector< float > testTexture( renderHeight * renderWidth * 4 );

  //for( size_t hIdx = 0; hIdx < renderHeight; ++hIdx )
  {

    for( size_t wIdx = 0; wIdx < renderWidth; ++wIdx )
    {

      testTexture[ ( renderHeight / 2 * renderWidth + wIdx ) * 4 ] = 1;

    }

  }
  //
  // Generate and bind the fbo
  //
  glGenFramebuffers( 1, &fbo );
  glBindFramebuffer( GL_FRAMEBUFFER, fbo );

  //
  // Generate and bind a texture
  //
  glGenTextures( 1, &fboTex );
  glBindTexture( GL_TEXTURE_2D, fboTex );

  //
  // Assign memory to the texture
  //
  glTexImage2D(
               GL_TEXTURE_2D,
               0,
               GL_RGB,
               renderWidth,
               renderHeight,
               0,
               GL_RGB,
               GL_FLOAT,
               0
               );

  //
  // Attach the texture to the framebuffer
  //
  glFramebufferTexture2D(
                         GL_FRAMEBUFFER,           // Attach to the framebuffer currently bound
                         GL_COLOR_ATTACHMENT0,     // Target 0
                         GL_TEXTURE_2D,            // Type of texture we are attaching
                         fboTex,                   // texture ID to attach
                         0                         // MipMap level
                         );

  //
  // Set the list of drawable buffers for the shaders that are sure to come
  //
  GLenum drawBuffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };

  glDrawBuffers( 1, drawBuffers );

  //
  // Check for issues
  //
  if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
  {

    std::cout << "Bad juju on the framebuffer" << std::endl;
    FAIL() << " frame buffer wasn't set up correctly";

  }

  // Create the texture for the triangle!
  // glGenTextures( 1, &toRenderTex );
  // glBindTexture( GL_TEXTURE_2D, toRenderTex );

  // glTexImage2D(
  //              GL_TEXTURE_2D,
  //              0,
  //              GL_RGBA,
  //              renderWidth,
  //              renderHeight,
  //              0,
  //              GL_RGBA,
  //              GL_FLOAT,
  //              &testTexture[ 0 ]
  //              );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Set up the triangles that are to be drawn
  // Ensure we can capture the escape key being pressed below
  winManager.setInputMode( idx, GLFW_STICKY_KEYS );

  //glfwMakeContextCurrent(pWin);

  //
  // Create a vertex array object
  //
  GLuint vertexArrayID;
  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);


  //
  // 4 points of the square
  //
  std::vector< float > vertexPosVec;
  std::vector< float > texCoords;

  // Point 0
  vertexPosVec.push_back( -1.0f );
  vertexPosVec.push_back( -1.0f );
  vertexPosVec.push_back( 0.0f );
  texCoords   .push_back( 0.0f );
  texCoords   .push_back( 0.0f );

  // Point 1
  vertexPosVec.push_back( -1.0f );
  vertexPosVec.push_back( 1.0f );
  vertexPosVec.push_back( 0.0f );
  texCoords   .push_back( 0.0f );
  texCoords   .push_back( 1.0f );

  // Point 2
  vertexPosVec.push_back( 1.0f );
  vertexPosVec.push_back( 1.0f );
  vertexPosVec.push_back( 0.0f );
  texCoords   .push_back( 1.0f );
  texCoords   .push_back( 1.0f );

  // Point 3
  vertexPosVec.push_back( 1.0f );
  vertexPosVec.push_back( -1.0f );
  vertexPosVec.push_back( 0.0f );
  texCoords   .push_back( 1.0f );
  texCoords   .push_back( 0.0f );

  // Indices for drawing triangles
  std::vector< unsigned int > indices;
  indices.push_back( 0 );
  indices.push_back( 2 );
  indices.push_back( 1 );
  indices.push_back( 0 );
  indices.push_back( 3 );
  indices.push_back( 2 );

  // Link the shader program
  std::vector< std::string > shaderFilenames;
  shaderFilenames.push_back( "../src/UnitTests/testTexVert.glsl" );
  shaderFilenames.push_back( "../src/UnitTests/testTexFrag.glsl" );
  unsigned int programId( 0 );

  // ASSERT_TRUE(
  //             renderer.createProgram(
  //                                    shaderFilenames,
  //                                    programId
  //                                    )
  //             );

  std::vector< unsigned int > texSize;
  texSize.push_back( 640 );
  texSize.push_back( 480 );

  modelPackage.addGeometry( vertexPosVec );
  modelPackage.addIndices( indices );
  modelPackage.addTexture(
                          testTexture,
                          texCoords,
                          texSize,
                          3
                          );

  modelPackage.addRendererFilenames( shaderFilenames );


  size_t renderableIdx( 0 );

  ASSERT_TRUE(
              renderer.initializeRenderable(
                                            modelPackage,
                                            renderableIdx
                                            )
              );

  ASSERT_NE( 0, renderableIdx );

  // Buffer identifiers
  // GLuint vertexBuffer;
  // GLuint indexBuffer;
  // GLuint texCoordBuffer;

  // // Generate 1 buffer, put the resulting identifier in vertexbuffer
  // glGenBuffers(1, &vertexBuffer);

  // // The following commands will talk about our 'vertexbuffer' buffer
  // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  // // Give our vertices to OpenGL.
  // glBufferData(
  //              GL_ARRAY_BUFFER,
  //              sizeof( vertexPosVec[ 0 ] ) * vertexPosVec.size(),
  //              &vertexPosVec[ 0 ],
  //              GL_STATIC_DRAW
  //              );

  // // Generate a texture coordinate buffer
  // glGenBuffers( 1, &texCoordBuffer );

  // glBindBuffer( GL_ARRAY_BUFFER, texCoordBuffer );

  // // Upload the texture coordinates
  // glBufferData(
  //              GL_ARRAY_BUFFER,
  //              sizeof( texCoords[ 0 ] ) * texCoords.size(),
  //              &texCoords[ 0 ],
  //              GL_STATIC_DRAW
  //              );

  // // Generate a buffer
  // glGenBuffers( 1, &indexBuffer );

  // // Bind our new index buffer
  // glBindBuffer(
  //              GL_ELEMENT_ARRAY_BUFFER,
  //              indexBuffer
  //              );

  // // Buffer the indices
  // glBufferData(
  //              GL_ELEMENT_ARRAY_BUFFER,
  //              indices.size() * sizeof(unsigned int),
  //              &indices[0],
  //              GL_STATIC_DRAW
  //              );

  std::cout << "Staring renderer loop" << std::endl;


  {
    GLenum err = glGetError();
    if( err != GL_NO_ERROR )
    {

      FAIL() << err;

    }
  }
  // Do the rendeiring for now in a loop
  do
  {

    glClear( GL_COLOR_BUFFER_BIT );
    //glViewport( 0, 0, 640, 480 );

    renderer.renderId( renderableIdx );

    //
    // Blit from the framebuffer to the screen
    //
    glBindFramebuffer(
                      GL_READ_FRAMEBUFFER,
                      fbo
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

    // Swap buffers
    ASSERT_TRUE( winManager.swapBuffers( idx ) ); //
    //glfwSwapBuffers(pWin);
    glfwPollEvents();

  }
  while(
        glfwGetKey(pWin, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(pWin) == 0
        );

}