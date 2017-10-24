#include "DisplayImage.hpp"

#include <iostream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

// CPP only namespace shortcut(s)
namespace pt = boost::property_tree;

struct PixelInserter
{

  std::vector<uint8_t>* storage;
  PixelInserter(std::vector<uint8_t>* s) : storage(s) {}

  void operator()(boost::gil::rgb8_pixel_t p) const
  {
          storage->push_back(boost::gil::at_c<0>(p));
          storage->push_back(boost::gil::at_c<1>(p));
          storage->push_back(boost::gil::at_c<2>(p));
  }

};

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

  // reset internal variables to default state
  windowWidth_               = 0;
  windowHeight_              = 0;
  imageFilename_             = "NOT_SPECIFIED";
  fileType_                  = "NOT_SPECIFIED";
  useImageSizeForWindowSize_ = true;

  //
  // Reset the inputTree to a default state
  //
  inputTree_.clear();

  inputTree_.put(
                 "filename",
                 imageFilename_
                 );


  inputTree_.put(
                 "windowWidth",
                 windowWidth_
                 );

  inputTree_.put(
                 "windowHeight",
                 windowHeight_
                 );

  inputTree_.put(
                 "fileType",
                 fileType_
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
/// \todo return something other than false once the implementation is done
bool
DisplayImage::displayImage()
{

  bool success( false );

  // Make sure to validate the input settings
  success = validatePropTree( inputTree_, false );

  // Return if something is wrong
  if( !success )
  {

    return false;

  }

  // Load the image from the file into memory
  loadImage();

  // Do something with the image

  // For now return false
  return false;

}



/// \brief Load the image that was set from inputTree_, return true if succesfull.
/// \todo Implement custom file loader
///        For now we only support PNG through boost, eventually a loader for a custom format will be made
bool
DisplayImage::loadImage()
{

  //
  // Load the image
  //
  boost::gil::rgb8_image_t image;
  boost::gil::png_read_and_convert_image( imageFilename_.c_str(), image );

  std::cout << "Width, Height, Channels = " << image.width() << ", " << image.height() << ", " << boost::gil::num_channels< boost::gil::rgb8_image_t >() << std::endl;

  //boost::gil::const_view( image );

  image_.reserve( image.width() * image.height() * boost::gil::num_channels< boost::gil::rgb8_image_t >() );

  // For each pixel insert into a storage bin
  boost::gil::for_each_pixel(
                             boost::gil::const_view( image ),
                             PixelInserter( &image_ )
                             );

  // Loop over all of the pixels in the image and push them into a vector
  // for( size_t row = 0; row < image.height(); ++row )
  // {

  //   for( size_t col = 0; col < image.width(); ++col )
  //   {

  //     std::cout << ( unsigned int )image_[ ( row * image.width() + col ) * 3 + 2 ] << ", ";

  //   }

  //   std::cout << std::endl;

  // }

  return false;

}



/// \brief Set the inputs using an input tree, validate the inputs before we copy to the internals
bool
DisplayImage::setInputs( const pt::ptree & tree )
{

  //
  // Make sure the inputs are valid and extract what is in there
  //
  bool ret = validatePropTree( tree, true );

  if( !ret )
  {

    return ret;

  }

  //
  // Now that it is validated copy the tree to our internals
  //
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
DisplayImage::validatePropTree(
                               const boost::property_tree::ptree& inputTree,
                               bool extractParameters
                               )
{

  //
  // Check for required filename
  //
  try
  {

    if( inputTree.get< std::string >( "filename" ) == "NOT_SPECIFIED" )
    {

      std::cerr << "filename not set correctly: " << inputTree.get< std::string >( "filename" ) << std::endl;
      return false;

    }

    if( extractParameters )
    {

      imageFilename_ = inputTree.get< std::string >( "filename" );

    }

  }
  catch( pt::ptree_error & err )
  {

    std::cerr << "Error Caught: " << err.what() << std::endl;
    return false;

  }

  //
  // Check on window width, will default to image size if set to 0
  //
  if( inputTree.count( "windowWidth" ) )
  {

    try
    {

      inputTree.get< unsigned int >( "windowWidth" );

      if( extractParameters )
      {

        windowHeight_ = inputTree.get< unsigned int >( "windowWidth" );

        if( windowHeight_ == 0 )
        {

          useImageSizeForWindowSize_ = true;

        }

      }

    }
    catch( pt::ptree_error & err )
    {

      std::cerr << "Error: " << err.what() << std::endl;

    }

  }

  //
  // Cehck on window height, will default to image size if set to 0
  //
  if( inputTree.count( "windowHeight" ) )
  {

    try
    {

      inputTree.get< unsigned int >( "windowHeight" );

      if( extractParameters )
      {

        windowHeight_ = inputTree.get< unsigned int >( "windowHeight" );

        if( windowHeight_ == 0 )
        {

          useImageSizeForWindowSize_ = true;

        }


      }

    }
    catch( pt::ptree_error & err )
    {

      std::cerr << "Error: " << err.what() << std::endl;

    }

  }

  //
  // Check on fileType
  //
  if( inputTree.count( "fileType" ) )
  {

    try
    {

      if( inputTree.get< std::string >( "fileType" ) == "NOT_SPECIFIED" )
      {

        std::cerr << "fileType is NOT_SPECIFIED" << std::endl;
        return false;

      }

      if( extractParameters )
      {

        fileType_ = inputTree.get< std::string >( "fileType" );

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