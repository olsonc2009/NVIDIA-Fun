#include "RenderableOGL.hpp"

#include "glad/glad.h"

/// \brief ctor, just make sure everything is zero'd out
RenderableOGL::RenderableOGL()
{

  vertexArrayObject_  = 0;
  geomBufferID_       = 0;
  textureID_          = 0;
  texCoordBufferID_   = 0;
  indexBufferID_      = 0;
  shaderID_           = 0;

  numElementsToRender_ = 0;
}


/// \brief if anything isn't zerod out then clear out all the OpenGL memory
RenderableOGL::~RenderableOGL()
{

  if( geomBufferID_ != 0 )
  {

    glDeleteBuffers( 1, &geomBufferID_ );

  }

  if( textureID_ != 0 )
  {

    glDeleteTextures( 1, &textureID_ );

  }

  if( texCoordBufferID_ != 0 )
  {

    glDeleteBuffers( 1, &texCoordBufferID_ );

  }

  if( indexBufferID_ != 0 )
  {

    glDeleteBuffers( 1, &indexBufferID_ );

  }

  if( vertexArrayObject_ != 0 )
  {

    glDeleteVertexArrays(1, &vertexArrayObject_);

  }

}