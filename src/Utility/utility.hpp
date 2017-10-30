#ifndef utility_hpp
#define utility_hpp

#include <vector>
#include <string>

///
/// \brief Utility namespace has utility functions that help but aren't really graphics related
///
namespace utility
{

  /// \brief Load a TEXT file to a vector of strings
  bool loadTextFileToVector(
                            const std::string& filename,
                            std::vector< std::string >& outLines
                            );

  /// \brief Take a vector of something and make into a single string
  bool convertVectorToString(
                            const std::vector< std::string >& inputLines,
                            std::string &outputString
                            );

  /// \brief go directly from a file to a singe large string
  bool loadFileIntoString(
                          const std::string filename,
                          std::string& outputString
                          );

  /// \brief Generate vertices, texCoords, and indices that can be used for rendering a texture to the screen, by default flip the texture coordinates for correct visual display
  bool generateScreenModelPackage(
                                 std::vector< float >& vertexPosVec,
                                 std::vector< float >& texCoords,
                                 std::vector< unsigned int >& indices,
                                 bool flipTexCoords = true
                                 );

};

#endif