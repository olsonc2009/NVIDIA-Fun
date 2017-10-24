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

};