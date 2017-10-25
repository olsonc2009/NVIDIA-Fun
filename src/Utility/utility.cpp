#include "utility.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace utility
{

  /// \brief Load a text file into the reference vector, clears vector initial contents
  bool
  loadTextFileToVector(
                      const std::string& filename,
                      std::vector< std::string >& outLines
                      )
  {

    // Clear the vector
    outLines.clear();

    //
    // Open the file
    //
    std::ifstream inFile( filename.c_str() );

    // Check for error condition
    if( !inFile.is_open() )
    {

      std::cerr << "Could not open filename " << filename << std::endl;
      return false;

    }

    //
    // Read the file
    //
    std::string line;

    while( std::getline( inFile, line ) )
    {

      outLines.push_back( line );

    }

    return true;

  }

  /// \brief Take a vector of something and make into a single string
  /// \todo templatize this!
  /// \todo add a user specifiable delimeter
  bool convertVectorToString(
                            const std::vector< std::string >& inputLines,
                            std::string &outputString
                            )
  {

    outputString = "";

    if( inputLines.size() == 0 )
    {

      outputString = "";
      return false;

    }

    const size_t numLines( inputLines.size() );

    for( size_t idx = 0; idx < numLines; ++idx )
    {

      outputString += inputLines[ idx ] + "\n";

    }

    return true;

  }

  /// \brief go directly from a file to a singe large string
  bool
  loadFileIntoString(
                     const std::string filename,
                     std::string& outputString
                     )
  {

    std::vector< std::string > lines;

    bool ret( false );

    // Load the file into a vector of strings
    ret = loadTextFileToVector(
                               filename,
                               lines
                               );

    // Check for errors
    if( !ret )
    {
      return ret;
    }

    // stringify the vector
    ret = convertVectorToString(
                                lines,
                                outputString
                                );

    // Return whatever happened
    return ret;

  }

  /// \brief Generate vertices, texCoords, and indices that can be used for rendering a texture to the screen, by default flip the texture coordinates for correct visual display
  bool generateScreenModelPackage(
                                  std::vector< float >& vertexPosVec,
                                  std::vector< float >& texCoords,
                                  std::vector< unsigned int >& indices,
                                  bool flipTexCoords
                                  )
  {

    vertexPosVec.clear();
    texCoords   .clear();
    indices     .clear();

    //
    // 4 points of the square
    //

    // Point 0
    vertexPosVec.push_back( -1.0f );
    vertexPosVec.push_back( -1.0f );
    vertexPosVec.push_back( 0.0f );

    if( !flipTexCoords )
    {

      texCoords   .push_back( 0.0f );
      texCoords   .push_back( 0.0f );

    }
    else
    {

      texCoords   .push_back( 0.0f );
      texCoords   .push_back( 1.0f );

    }


    // Point 1
    vertexPosVec.push_back( -1.0f );
    vertexPosVec.push_back( 1.0f );
    vertexPosVec.push_back( 0.0f );

    if( !flipTexCoords )
    {

      texCoords   .push_back( 0.0f );
      texCoords   .push_back( 1.0f );

    }
    else
    {

      texCoords   .push_back( 0.0f );
      texCoords   .push_back( 0.0f );

    }

    // Point 2
    vertexPosVec.push_back( 1.0f );
    vertexPosVec.push_back( 1.0f );
    vertexPosVec.push_back( 0.0f );

    if( !flipTexCoords )
    {

      texCoords   .push_back( 1.0f );
      texCoords   .push_back( 1.0f );

    }
    else
    {

      texCoords   .push_back( 1.0f );
      texCoords   .push_back( 0.0f );

    }

    // Point 3
    vertexPosVec.push_back( 1.0f );
    vertexPosVec.push_back( -1.0f );
    vertexPosVec.push_back( 0.0f );

    if( !flipTexCoords )
    {

      texCoords   .push_back( 1.0f );
      texCoords   .push_back( 0.0f );

    }
    else
    {

      texCoords   .push_back( 1.0f );
      texCoords   .push_back( 1.0f );

    }

    // Indices for drawing triangles
    indices.push_back( 0 );
    indices.push_back( 2 );
    indices.push_back( 1 );
    indices.push_back( 0 );
    indices.push_back( 3 );
    indices.push_back( 2 );

  }


};