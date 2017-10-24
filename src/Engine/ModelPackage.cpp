#include "ModelPackage.hpp"


/// \brief Ctor, calls reset
ModelPackage::ModelPackage()
{

  reset();

}

/// \brief Dtor, calls reset
ModelPackage::~ModelPackage()
{

  reset();

}


/// \brief Add indices to the model
bool
ModelPackage::addIndices( std::vector< unsigned int > indices )
{

  // If we don't have indices allow them to be copied otherwise don't
  if( !hasIndices_ && indices.size() > 0 )
  {

    indices_ = indices;
    hasIndices_ = true;

  }
  else
  {

    return false;

  }

  return true;

}

/// \brief Add filenames to the model package, i.e. shaders or hit programs etc
bool
ModelPackage::addRendererFilenames( std::vector< std::string > filenames )
{

  if( !hasFiles_ && filenames.size() > 0 )
  {

    rendererFilenames_ = filenames;

    hasFiles_ = true;

  }
  else
  {

    return false;

  }

  return true;

}


/// \brief Set everything to a default state, kill everything off that exists
void
ModelPackage::reset()
{

  hasGeometry_ = false;
  hasTexture_  = false;
  hasIndices_  = false;
  hasFiles_    = false;

  pos_              .clear();
  tex_              .clear();
  texCoords_        .clear();
  indices_          .clear();
  rendererFilenames_.clear();

  textureStride_ = 0;

}


///\brief Return if geometry has been set yet
bool
ModelPackage::hasGeometry() const
{

  return hasGeometry_;

}


/// \brief Return if the texture has been set
bool
ModelPackage::hasTexture() const
{

  return hasTexture_;

}


/// \brief Return if indices have been set
bool
ModelPackage::hasIndices() const
{

  return hasIndices_;

}

/// \brief Return whether files are present
bool
ModelPackage::hasFiles() const
{

  return hasFiles_;

}


/// \brief Return the position vector
const std::vector< float >&
ModelPackage::getGeometry() const
{

  return pos_;

}

/// \brief Return the texture vector
const std::vector< float >&
ModelPackage::getTexture() const
{

  return tex_;

}

/// \brief Return the texture coordinates vector
const std::vector< float >&
ModelPackage::getTextureCoords() const
{

  return texCoords_;

}

/// \brief Return the indices vector
const std::vector< unsigned int >&
ModelPackage::getIndices() const
{

  return indices_;

}

/// \brief return the texture stride
size_t
ModelPackage::getTextureStride() const
{

  return textureStride_;

}

/// \brief Return the texture dimensions, the size of the vector dictates its dimensionality
const std::vector< unsigned int >&
ModelPackage::getTextureDims() const
{

  return texSize_;

}

/// \brief Return the renderer filenames we have stored
const std::vector< std::string >&
ModelPackage::getRendererFilenames() const
{

  return rendererFilenames_;

}