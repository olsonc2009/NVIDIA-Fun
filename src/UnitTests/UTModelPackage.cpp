#include "gtest/gtest.h"

#include "ModelPackage.hpp"


/// \brief Test the geometry copy function with floats
TEST( UTModelPackage, Geometry )
{


  ModelPackage modelPackage;

  std::vector< float > pos;

  pos.push_back( 1.0f );
  pos.push_back( -1.0f );
  pos.push_back( 0.0f );

  modelPackage.addGeometry( pos );

  ASSERT_TRUE( modelPackage.hasGeometry() );

  ASSERT_EQ(  1.0f, modelPackage.getGeometry()[ 0 ] );
  ASSERT_EQ( -1.0f, modelPackage.getGeometry()[ 1 ] );
  ASSERT_EQ(  0.0f, modelPackage.getGeometry()[ 2 ] );

}

/// \brief Test the texture copy function with floats
TEST( UTModelPackage, Texture )
{


  ModelPackage modelPackage;

  std::vector< float > tex;
  std::vector< float > texCoords;
  std::vector< unsigned int > dims;

  tex.push_back( 1.0f );
  tex.push_back( -1.0f );

  texCoords.push_back( 0.0f );
  texCoords.push_back( 2.0f );

  dims.push_back( 1 );
  dims.push_back( 1 );


  modelPackage.addTexture(
                          tex,
                          texCoords,
                          dims,
                          3
                          );

  ASSERT_TRUE( modelPackage.hasTexture() );

  ASSERT_EQ(  1.0f, modelPackage.getTexture()[ 0 ] );
  ASSERT_EQ( -1.0f, modelPackage.getTexture()[ 1 ] );

  ASSERT_EQ(  0.0f, modelPackage.getTextureCoords()[ 0 ] );
  ASSERT_EQ(  2.0f, modelPackage.getTextureCoords()[ 1 ] );

  ASSERT_EQ( 3, modelPackage.getTextureStride() );

}


/// \brief Test the texture copy function with floats
TEST( UTModelPackage, Indices )
{

  ModelPackage modelPackage;

  std::vector< unsigned int > indices;

  indices.push_back( 1 );
  indices.push_back( 10 );

  modelPackage.addIndices( indices );

  ASSERT_TRUE( modelPackage.hasIndices() );

  ASSERT_EQ( 1 , modelPackage.getIndices()[ 0 ] );
  ASSERT_EQ( 10, modelPackage.getIndices()[ 1 ] );

}


/// \brief Test the filenames copy function with files
TEST( UTModelPackage, Filenames )
{

  ModelPackage modelPackage;

  std::vector< std::string > filenames;
  filenames.push_back( "shader" );

  ASSERT_TRUE( modelPackage.addRendererFilenames( filenames ) );

  ASSERT_TRUE( modelPackage.hasFiles() );

  ASSERT_EQ( std::string( "shader" ), modelPackage.getRendererFilenames()[ 0 ] );

}
