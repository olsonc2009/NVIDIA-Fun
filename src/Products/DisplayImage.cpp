#include "DisplayImage.hpp"

#include <iostream>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>


#include "RendererOGL.hpp"
#include "ModelPackage.hpp"
#include "RenderableOGL.hpp"
#include "WindowManagerGLFW.hpp"
#include "GraphicsContextGLFW.hpp"
#include "utility.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
  : pRenderer_        ( 0 )
  , pModelPackage_    ( 0 )
  , pRenderableOGL_   ( 0 )
  , pWindowManager_   ( 0 )
  , pGraphicsContext_ ( 0 )
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

  if( pRenderer_      == 0 )
  {

    delete pRenderer_;
    pRenderer_ = 0;

  }


  if( pModelPackage_  == 0 )
  {

    delete pModelPackage_;
    pModelPackage_ = 0;

  }


  if( pRenderableOGL_ == 0 )
  {

    delete pRenderableOGL_;
    pRenderableOGL_ = 0;

  }

  if( pWindowManager_ == 0 )
  {

    delete pWindowManager_;
    pWindowManager_ = 0;

  }

  if( pGraphicsContext_ )
  {

    delete pGraphicsContext_;
    pGraphicsContext_ = 0;

  }

  renderableIdx_ = 0;

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

  //
  // Load the image from the file into memory
  //
  success = loadImage();

  // Check for errors
  if( !success )
  {

    std::cerr << "Could not load image" << std::endl;
    return false;

  }
  else
  {

    std::cout << "Successfully loaded the image" << std::endl;

  }


  //
  // Setup the window for display
  //
  success = setupWindowForDisplay();

  if( !success )
  {

    std::cerr << "Could not set up the window for display" << std::endl;
    return false;

  }
  else
  {

    std::cout << "Successfully setup the window for display" << std::endl;

  }



  //
  // Setup the model package
  //
  success = setupModelPackage();

  if( !success )
  {

    std::cerr << "Could not set up the model package" << std::endl;
    return false;

  }
  else
  {

    std::cout << "Successfully setup the model package" << std::endl;

  }

  //
  // Call the render loop
  //
  renderLoop();

  // For now return false
  return true;

}


/// \brief The render loop
bool
DisplayImage::renderLoop()
{

  glClearColor( 0.0f, 0.0f, 0.4f, 0.0f );
  glViewport( 0, 0, windowWidth_, windowHeight_ );
  // Do the rendeiring for now in a loop
  do
  {

    glClear( GL_COLOR_BUFFER_BIT );
    //glViewport( 0, 0, 640, 480 );

    pRenderer_->renderId( renderableIdx_ );

    //
    // Blit from the framebuffer to the screen
    //
    // glBindFramebuffer(
    //                   GL_READ_FRAMEBUFFER,
    //                   fbo
    //                   );

    // glBindFramebuffer(
    //                   GL_DRAW_FRAMEBUFFER,
    //                   0
    //                   );

    // glBlitFramebuffer(
    //                   0, 0, 640, 480,
    //                   0, 0, 640, 480,
    //                   GL_COLOR_BUFFER_BIT,
    //                   GL_LINEAR
    //                   );

    // Swap buffers
    pWindowManager_->swapBuffers( windowIdx_ );

    //glfwSwapBuffers( pWin_ );
    glfwPollEvents();

  }
  while(
        glfwGetKey( pWin_, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose( pWin_ ) == 0
        );

}



/// \brief Load the image that was set from inputTree_, return true if succesfull.
/// \todo Implement custom file loader
/// \todo Move this to an algorithm type pattern for testing
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

  if( useImageSizeForWindowSize_ )
  {

    windowWidth_   = image.width();
    windowHeight_  = image.height();
    imageChannels_ = 3; //boost::gil::num_channels< boost::gil::rgb8_image_t >();

  }

  //boost::gil::const_view( image );

  image_.resize( image.width() * image.height() * boost::gil::num_channels< boost::gil::rgb8_image_t >() );

  // For each pixel insert into a storage bin
  // boost::gil::for_each_pixel(
  //                            boost::gil::const_view( image ),
  //                            PixelInserter( &image_ )
  //                            );

  // Loop over all of the pixels in the image and push them into a vector
  // for( size_t row = 0; row < image.height(); ++row )
  // {

    for( size_t col = 0; col < image.width(); ++col )
    {

      //std::cout << ( unsigned int )image_[ ( row * image.width() + col ) * 3 + 2 ] << ", ";
      // image_[ ( image.width() / 2 * image.height() + col ) * 3 + 0 ] = 0;
      // image_[ ( image.width() / 2 * image.height() + col ) * 3 + 1 ] = 0;
      // image_[ ( image.width() / 2 * image.height() + col ) * 3 + 2 ] = 1;

      image_[ ( image.height() / 2 * image.width() + col ) * 3 + 0 ] = 0;
      image_[ ( image.height() / 2 * image.width() + col ) * 3 + 1 ] = 0;
      image_[ ( image.height() / 2 * image.width() + col ) * 3 + 2 ] = 1;

    }

  // }

  return true;

}


/// \brief Get the window setup now that all of the internal vars have been set
bool
DisplayImage::setupWindowForDisplay()
{



  pGraphicsContext_ = new GraphicsContextGLFW();
  pRenderer_        = new RendererOGL();
  pWindowManager_   = new WindowManagerGLFW( pGraphicsContext_ );

  bool success( false );

  success = pGraphicsContext_->initialize();

  if( !success )
  {

    return false;

  }


  success = pWindowManager_->createWindow(
                                          windowIdx_,
                                          pWin_,
                                          windowWidth_,
                                          windowHeight_,
                                          "DisplayImage"
                                          );

  if( !success )
  {
    return false;
  }


  success = pGraphicsContext_->loadExtensions();

  if( !success )
  {

    return false;

  }

  return success;

}



/// \brief Given everything that is loaded into the class memory setup a Model Package for rendering
bool
DisplayImage::setupModelPackage()
{

  bool ret( false );

  pModelPackage_  = new ModelPackage();

  pRenderableOGL_ = new RenderableOGL();

  std::vector< float >        vertexPosVec;
  std::vector< float >        texCoords;
  std::vector< unsigned int > indices;
  std::vector< unsigned int > imageSizeVec;
  std::vector< std::string >  shaderFilenames;

  // Generate what we can for the render package
  utility::generateScreenModelPackage(
                                      vertexPosVec,
                                      texCoords,
                                      indices
                                      );

  // Create the necessary vector for imageSizeVec
  imageSizeVec.push_back( windowWidth_ );
  imageSizeVec.push_back( windowHeight_ );

  // Add the geometry
  pModelPackage_->addGeometry( vertexPosVec );

  // Add the texture to the package
  pModelPackage_->addTexture(
                             image_,
                             texCoords,
                             imageSizeVec,
                             imageChannels_
                             );

  // Add the indices to the package
  pModelPackage_->addIndices( indices );

  shaderFilenames.push_back( "../src/UnitTests/testTexVert.glsl" );
  shaderFilenames.push_back( "../src/UnitTests/testTexFrag.glsl" );

  //
  // Add the shaders to the render package
  //
  ret = pModelPackage_->addRendererFilenames( shaderFilenames );

  // Return if bad
  if( !ret )
  {

    return ret;

  }

  //
  // Create the renderable
  //
  ret = pRenderer_->initializeRenderable( *pModelPackage_, renderableIdx_ );

  // Return if bad
  if( !ret )
  {

    return ret;

  }


  return true;

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

        windowWidth_ = inputTree.get< unsigned int >( "windowWidth" );

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