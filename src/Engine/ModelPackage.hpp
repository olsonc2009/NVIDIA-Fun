#ifndef ModelPackage_hpp
#define ModelPackage_hpp

#include <iostream>  // Weird that this is needed for size_t
#include <vector>
#include <string>

/// \brief This is a container class for rendering something.
/// I'd really like it to work for everything, i.e. terrain and models but I'm
/// not sure that is possible with its current make up, will have to keep
/// looking into it I suppose
class ModelPackage
{

public:

  ModelPackage();
  virtual ~ModelPackage();

  /// \brief Allocate the positions to the model package
  template < class T >
  bool addGeometry( std::vector< T > positionsModelSpace );

  /// \brief Give both the texture and texture coordinates for the model
  template < class T >
  bool addTexture(
                  std::vector< T > texture,
                  std::vector< float > textureCoords,
                  std::vector< unsigned int > textureSize,
                  size_t textureStride
                  );

  /// \brief Add the texture coordinates to the model package
  //bool addTextureCoordinates( std::vector< float > texCoords );

  // template < class T, size_t N >
  // bool addColors(
  //                std::vector< T > colors
  //                );

  /// \brief Add the indices that will make up whatever is to be drawn
  bool addIndices( std::vector< unsigned int > indices );

  /// \brief Add filenames to the model package, i.e. shaders or hit programs etc
  bool addRendererFilenames( std::vector< std::string > filenames );

  virtual bool hasGeometry() const;
  virtual bool hasTexture() const;
  virtual bool hasIndices() const;
  virtual bool hasFiles() const;

  virtual const std::vector< float >& getGeometry() const ;
  virtual const std::vector< float >& getTexture() const;
  virtual const std::vector< float >& getTextureCoords() const;
  virtual const std::vector< unsigned int >& getIndices() const;
  virtual size_t getTextureStride() const;
  virtual const std::vector< unsigned int >& getTextureDims() const;
  virtual const std::vector< std::string >& getRendererFilenames() const;

  virtual void reset();

protected:

  std::vector< float >          pos_;        ///< 3-Space positions in model coordinates
  std::vector< float >          tex_;        ///< The actual texture
  std::vector< unsigned int >   texSize_;    ///< the size of the texture, the dimension is determined by the entries in the vector
  std::vector< float >          texCoords_;  ///< texture coordinates
  std::vector< unsigned int >   indices_;    ///< indices for making the shapes of the model
  std::vector< std::string >    rendererFilenames_; ///< Filenames for the renderer to take into account i.e. Shaders or Programs

  size_t textureStride_;                     ///< Dimension of the texture i.e. RGB = 3 RGBA = 4

  bool hasGeometry_;
  bool hasTexture_;
  bool hasIndices_;
  bool hasFiles_;
//  bool hasColors_;
//  bool hasNormals_;

};



/// \brief Allocate the positions to the model package
template < class T >
bool
ModelPackage::addGeometry( std::vector< T > positionsModelSpace )
{

  if( !hasGeometry_ && positionsModelSpace.size() > 0 )
  {

    pos_.resize( positionsModelSpace.size() );

    std::copy( positionsModelSpace.begin(), positionsModelSpace.end(), pos_.begin() );
    hasGeometry_ = true;

  }
  else
  {

    return false;

  }

  return true;

}



/// \brief Give both the texture and texture coordinates for the model
/// \todo Could add a check that textureCoords and number of positions match up
template < class T >
bool
ModelPackage::addTexture(
                         std::vector< T > texture,
                         std::vector< float > textureCoords,
                         std::vector< unsigned int > textureSize,
                         size_t textureStride
                         )
{

  if( !hasTexture_ && texture.size() > 0 )
  {

    tex_.resize( texture.size() );

    for( size_t idx = 0; idx < tex_.size(); ++idx )
    {

      tex_[ idx ] = static_cast< float >( texture[ idx ] );

    }
    //std::copy( texture.begin(), texture.end(), tex_.begin() );

    // Set everything we need
    texCoords_     = textureCoords;
    textureStride_ = textureStride;
    texSize_       = textureSize;

    hasTexture_ = true;

  }
  else
  {

    return false;

  }

  return true;

}


#endif // ModelPackage_hpp