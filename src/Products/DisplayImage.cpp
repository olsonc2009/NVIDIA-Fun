#include "DisplayImage.hpp"

#include <iostream>

// CPP only namespace shortcut(s)
namespace pt = boost::property_tree;

/// \brief Ctor, calls reset
DisplayImage::DisplayImage()
{

  reset();

}


/// \brief Dtor, currently does absolutely nothing
DisplayImage::~DisplayImage()
{


}


/// \brief Reset everything to default values
void
DisplayImage::reset()
{

  //
  // Reset the inputTree to a default state
  //
  inputTree_.clear();

  inputTree_.push_back(
                       pt::ptree::value_type(
                                             "filename",
                                             pt::ptree( "NOT_SPECIFIED" )
                                             )
                       );


  inputTree_.push_back(
                       pt::ptree::value_type(
                                             "windowWidth",
                                             pt::ptree( "640" )
                                             )
                       );

  inputTree_.push_back(
                       pt::ptree::value_type(
                                             "windowHeight",
                                             pt::ptree( "640" )
                                             )
                       );

  inputTree_.push_back(
                       pt::ptree::value_type(
                                             "fileType",
                                             pt::ptree( "NOT_SPECIFIED" )
                                             )
                       );

  //
  // Set the input keys of interest
  //
  inputKeys_.clear();
  inputKeys_.push_back( "filename"     );
  inputKeys_.push_back( "windowWidth"  );
  inputKeys_.push_back( "windowHeight" );
  inputKeys_.push_back( "fileType"     );

}

/// \brief Display the image based on the input tree
bool
DisplayImage::displayImage()
{

  bool success( false );

  // Make sure to validate the input settings
  success = validatePropTree( inputTree_ );

  // Return if something is wrong
  if( !success )
  {

    return false;

  }

  // Do something with the image

}



/// \brief Set the inputs using an input tree
bool
DisplayImage::setInputs( const pt::ptree & tree )
{

  bool ret = validatePropTree( tree );

  if( !ret )
  {

    return ret;

  }


  inputTree_ = tree;

  return ret;

}


/// \brief Return the current input tree
const pt::ptree&
DisplayImage::getInputs() const
{

  return inputTree_;

}


/// \brief Ensure that the property tree has valid parameters for our use
/// \todo Ensure the the filename specified can be opened
bool
DisplayImage::validatePropTree( const boost::property_tree::ptree& inputTree )
{

  try
  {

    inputTree.get< std::string >( "filename" ) != "NOT_SPECIFIED";

  }
  catch( pt::ptree_error & err )
  {

    std::cerr << "Error Caught: " << err.what() << std::endl;
    return false;

  }

  if( inputTree.count( "windowWidth" ) )
  {

    try
    {

      inputTree.get< unsigned int >( "windowWidth" );

    }
    catch( pt::ptree_error & err )
    {

      std::cerr << "Error: " << err.what() << std::endl;

    }

  }

  if( inputTree.count( "windowHeight" ) )
  {

    try
    {

      inputTree.get< unsigned int >( "windowHeight" );

    }
    catch( pt::ptree_error & err )
    {

      std::cerr << "Error: " << err.what() << std::endl;

    }

  }

  if( inputTree.count( "fileType" ) )
  {

    try
    {

      if( inputTree.get< std::string >( "fileType" ) != "NOT_SPECIFIED" )
      {

        std::cerr << "fileType is NOT_SPECIFIED" << std::endl;
        return false;

      }

    }
    catch( pt::ptree_error & err )
    {

      std::cerr << "Error: " << err.what() << std::endl;

    }

  }



  return true;

}


/// \brief Return the keys that are used by this module
const std::vector< std::string >&
DisplayImage::getInputKeys() const
{

  return inputKeys_;

}