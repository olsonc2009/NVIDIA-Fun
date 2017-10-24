#include "RendererOGL.hpp"

#include <iostream>

#include "glad/glad.h"
#include "RenderableOGL.hpp"
#include "utility.hpp"

/// \brief Initialize everything needed, we start nextRenderableId_ at 1 so 0 is considered a bad value
RendererOGL::RendererOGL()
{

  nextRenderableId_ = 1;

}

/// \brief Clean up any leftover memory
RendererOGL::~RendererOGL()
{

  // Clear out all the memory in the map
  for(
      std::map< size_t, RenderableOGL* >::iterator iter = idToRenderableMap_.begin();
      iter != idToRenderableMap_.end();
      ++iter
      )
  {

    delete iter->second;

  }

  idToRenderableMap_.clear();

}


/// \brief Take a model package and create something OpenGL can work with, return of 0 indicates an issue
/// \todo Add lots of error checking
/// \todo Support more than just 2D textures, maybe that should just be something assigned, just generate it elsewhere and pass in the ID
/// \todo Add support for assigning already compiled shaders
/// \todo Add logging to trace through this big process
bool
RendererOGL::initializeRenderable(
                                  const ModelPackage& modelPackage,
                                  size_t &renderableIdx
                                  )
{

  // If we don't have geometry just bail out, we shouldn't be here
  if( !modelPackage.hasGeometry() )
  {

    std::cerr << "Must have geometry for this to work" << std::endl;
    return false;

  }

  //
  // Create the renderable thing
  //
  RenderableOGL *pRenderable = new RenderableOGL();

  // Initialize the VAO
  glGenVertexArrays( 1, &pRenderable->vertexArrayObject_ );
  glBindVertexArray( pRenderable->vertexArrayObject_ );

  // Initialize the vertex buffor object
  glGenBuffers( 1, &pRenderable->geomBufferID_ );
  glBindBuffer( GL_ARRAY_BUFFER, pRenderable->geomBufferID_ );

  // Upload the data to the GPU
  const std::vector< float >& geom( modelPackage.getGeometry() );

  glBufferData(
               GL_ARRAY_BUFFER,
               sizeof( geom[ 0 ] ) * geom.size(),
               &geom[ 0 ],
               GL_STATIC_DRAW
              );


  // Test if we need to do anything with textures
  if( modelPackage.hasTexture() )
  {

    const std::vector< float >&        texCoords( modelPackage.getTextureCoords() );
    const std::vector< float >&        texture  ( modelPackage.getTexture() );
    const std::vector< unsigned int >& texDims  ( modelPackage.getTextureDims() );

    //
    // Make sure we have the right dimensions for what we support now
    //
    if( texDims.size() != 2 )
    {

      delete pRenderable;

      std::cerr << "Only support 2D textures right now" << std::endl;
      return false;

    }

    // Generate / Upload texture coordinats
    glGenBuffers( 1, &pRenderable->texCoordBufferID_ );
    glBindBuffer( GL_ARRAY_BUFFER, pRenderable->texCoordBufferID_ );

    glBufferData(
                 GL_ARRAY_BUFFER,
                 sizeof( texCoords[ 0 ] ) * texCoords.size(),
                 &texCoords[ 0 ],
                 GL_STATIC_DRAW
                 );

    // Generate / Upload the actual texture
    glGenTextures( 1, &pRenderable->textureID_ );
    glBindTexture( GL_TEXTURE_2D, pRenderable->textureID_ );

    // Upload the texture
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 texDims[ 0 ],
                 texDims[ 1 ],
                 0,
                 GL_RGB,
                 GL_FLOAT,
                 &texture[ 0 ]
                 );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  }

  //
  // Test if we have any indices to upload
  //
  if( modelPackage.hasIndices() )
  {

    const std::vector< unsigned int >& indices( modelPackage.getIndices() );

    glGenBuffers( 1, &pRenderable->indexBufferID_ );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pRenderable->indexBufferID_ );

    glBufferData(
                 GL_ELEMENT_ARRAY_BUFFER,
                 sizeof( indices[ 0 ] ) * indices.size(),
                 &indices[ 0 ],
                 GL_STATIC_DRAW
                );

    pRenderable->numElementsToRender_ = indices.size();

  }

  // Check for shaders
  if( modelPackage.hasFiles() )
  {

    /// \todo this needs error checking
    this->createProgram(
                        modelPackage.getRendererFilenames(),
                        pRenderable->shaderID_
                        );

  }


  //
  // Finally store the new container in our map
  //
  idToRenderableMap_[ nextRenderableId_ ] = pRenderable;

  renderableIdx = nextRenderableId_;

  ++nextRenderableId_;

  return true;

}


/// \brief A very simple render routine
bool
RendererOGL::renderId( size_t renderableIdx )
{

  std::map< size_t, RenderableOGL* >::iterator iter = idToRenderableMap_.find( renderableIdx );

  if( iter == idToRenderableMap_.end() )
  {

    std::cerr << "Could not find " << renderableIdx << " in idToRenderableMap_ " << std::endl;
    return false;

  }

  RenderableOGL *pRenderable = iter->second;

  // Call the rendering pipeline here!
  glUseProgram( pRenderable->shaderID_ );

  GLuint TextureID = glGetUniformLocation( pRenderable->shaderID_, "myTextureSampler" );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, pRenderable->textureID_ );
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i( TextureID, 0 );

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer( GL_ARRAY_BUFFER, pRenderable->geomBufferID_);

  glVertexAttribPointer(
                       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                       3,                  // size
                       GL_FLOAT,           // type
                       GL_FALSE,           // normalized?
                       0,                  // stride
                       (void*)0            // array buffer offset
                       );

  // Enable the texture coordinates
  glEnableVertexAttribArray( 1 );
  glBindBuffer( GL_ARRAY_BUFFER, pRenderable->textureID_ );

  glVertexAttribPointer(
                       1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                       2,                  // size
                       GL_FLOAT,           // type
                       GL_FALSE,           // normalized?
                       0,                  // stride
                       (void*)0            // array buffer offset
                       );

  // Bind the element array
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pRenderable->indexBufferID_ );

  // Draw the triangles
  glDrawElements(
                 GL_TRIANGLES,
                 pRenderable->numElementsToRender_,
                 GL_UNSIGNED_INT,
                 (void*)0
                 ); // Starting from vertex 0; 3 vertices total -> 1 triangle

  glDisableVertexAttribArray( 1 );
  glDisableVertexAttribArray( 0 );


}

/// \brief Create a shader program based on the shader files passed in, programID is a reference
bool
RendererOGL::createProgram(
                           const std::vector< std::string > shaderFilenames,
                           unsigned int &programId
                           )
{

  bool ret( false );

  size_t numShaders( shaderFilenames.size() );

  // Create the shader string, intiialize to number of shaders
  std::vector< std::string > shaderStrings( numShaders );

  // Create my temporary shaders
  std::vector< unsigned int > tempShaders;

  ret = createShaders(
                      numShaders,
                      tempShaders
                      );

  if( !ret )
  {

    std::cerr << "Could not create " << numShaders << " shaders";
    return false;

  }


  // Load the shader strings
  for( size_t idx = 0; idx < numShaders; ++idx )
  {

    // Load the shader into a string
    ret = utility::loadFileIntoString(
                                      shaderFilenames[ idx ],
                                      shaderStrings[ idx ]
                                      );

    // error checking
    if( !ret )
    {

      std::cerr << "Could not load " << shaderFilenames[ idx ] << " shader " << std::endl;
      return false;

    }

    // Compile the shader
    ret = compileShader(
                        shaderStrings[ idx ],
                        tempShaders[ idx ]
                        );

    // Error checking
    if( !ret )
    {

      std::cerr << "Could not compile shader " << shaderFilenames[ idx ] << std::endl;
      return false;

    }

  }


  // Create the program that we will be using
  programId = glCreateProgram();

  // Link the shaders
  ret = linkShaders(
                    programId,
                    tempShaders
                    );

  if( !ret )
  {

    std::cerr << "Failed linking the program" << std::endl;
    return false;

  }

  return ret;

}

/// \brief Create the number of shaders specified and return them in the passed in vector.
/// There is an assumed order
///   1. Vertex
///   2. Fragment
///   3. Geometry ( optional )
///   4. Is there more? ( optional )
bool
RendererOGL::createShaders(
                           size_t numShaders,
                           std::vector< unsigned int >& shaderIds
                           )
{

  if( numShaders > 2 || numShaders < 2 )
  {

    std::cerr << "I don't currently support more than 2 shaders i.e. beyond Fragment shaders its a future improvement";
    return false;

  }

  shaderIds.resize( numShaders );

  if( numShaders > 0 )
  {

    shaderIds[ 0 ] = glCreateShader( GL_VERTEX_SHADER );

  }

  if( numShaders > 1 )
  {

    shaderIds[ 1 ] = glCreateShader( GL_FRAGMENT_SHADER );

  }

  return true;

}

/// \brief Compile a string into a shader
/// \todo Add logging to this function
/// \todo Expand to allow for multiple shaders being compiled simultaneously with glShaderSource et al
// static
bool
RendererOGL::compileShader(
                           std::string shaderString,
                           unsigned int shaderId
                           )
{

  int result( 0 );
  int infoLogLength( 0 );

  char const *pSource = shaderString.c_str();

  // Associate the shader and the source
  glShaderSource(
                 shaderId,
                 1,
                 &pSource,
                 NULL
                 );

  // Compile the source
  glCompileShader( shaderId );

  //
  // Check for errors
  //
  glGetShaderiv( shaderId, GL_COMPILE_STATUS, &result );
  glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &infoLogLength );

  if( infoLogLength > 0 )
  {

    std::vector< char > fragShaderErrMsg( infoLogLength + 1 );
    glGetShaderInfoLog( shaderId, infoLogLength, NULL, &fragShaderErrMsg[ 0 ] );
    std::string errString( &fragShaderErrMsg[ 0 ] );

    std::cerr << errString << std::endl;
    return false;

  }

  return true;

}


/// \brief link the shaders to the program ID that is passed in
/// \todo Would it be safer to create the program here as well?
//static
bool
RendererOGL::linkShaders(
                         unsigned int programToLinkToId,
                         std::vector< unsigned int > shadersToLink
                         )
{

  size_t numShaders( shadersToLink.size() );

  //
  // Attach all shaders that are there
  //
  for( size_t idx = 0; idx < numShaders; ++idx )
  {

    glAttachShader( programToLinkToId, shadersToLink[ idx ] );

  }

  // Link everything
  glLinkProgram( programToLinkToId );

  //
  // Check for errors
  //
  int result( 0 );
  int infoLogLength( 0 );
  glGetProgramiv( programToLinkToId, GL_LINK_STATUS, &result );
  glGetProgramiv( programToLinkToId, GL_INFO_LOG_LENGTH, &infoLogLength );

  if( infoLogLength > 0 )
  {

    std::vector< char > fragShaderErrMsg( infoLogLength + 1 );
    glGetShaderInfoLog( programToLinkToId, infoLogLength, NULL, &fragShaderErrMsg[ 0 ] );
    std::string errString( &fragShaderErrMsg[ 0 ] );

    std::cerr << errString << std::endl;
    return false;

  }

  //
  // Clean up after ourselves
  //
  for( size_t idx = 0; idx < numShaders; ++idx )
  {

    glDetachShader( programToLinkToId, shadersToLink[ idx ] );
    glDeleteShader( shadersToLink[ idx ] );

  }

  return true;

}
