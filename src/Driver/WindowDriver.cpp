#include <iostream>
#include <cstdlib>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>

#include "DisplayImage.hpp"

#include "WindowManagerGLFW.hpp"
#include "GraphicsContextGLFW.hpp"
#include "RendererOGL.hpp"


namespace pt = boost::property_tree;
namespace po = boost::program_options;


/// \brief Parse the arguments and fill in the property tree for DisplayImage
static void parseArgs( int argc, char* argv[] );

static pt::ptree propTree_;

/// \brief Reference implementation of displaying data to a window using the window manager
int main( int argc, char* argv[] )
{

  bool success( false );

  DisplayImage dispImage;

  // Parse the argument and setup the PropertyTree
  parseArgs( argc, argv );

  // std::cout << "Starting up a window example" << std::endl;

  // std::string filename( "test.json" );

  // // Load the json
  // pt::ptree tree;

  // // Parse the XML into the property tree.
  // pt::read_json(filename, tree);

  // //std::cout << tree << std::endl;

  // std::string name = tree.get< std::string >( "name" );

  // std::cout << "NAME = " << name << std::endl;

  success = dispImage.setInputs( propTree_ );

  if( !success )
  {

    std::cout << "Inputs were not set succesfully" << std::endl;
    return -1;

  }
  else
  {

    std::cout << "Inputs set succesfully" << std::endl;

  }

  success = dispImage.displayImage();

  if( !success )
  {

    std::cout << "Did not succesfully render image or exited with bad code" << std::endl;

  }
  else
  {

    std::cout << "Successfully exiting" << std::endl;

  }


  return 0;

  // bool success( false );

  // GraphicsContextGLFW graphicsContext;
  // WindowManagerGLFW   windowManager;

  // success = graphicsContext.initialize();

  // if( !success )
  // {

  //   std::cerr << "Did not succesfully initialize graphics context" << std::endl;
  //   return -1;

  // }

  // size_t windowIdx;
  // GLFWwindow* pWin;

  // success = windowManager.createWindow( windowIdx, pWin );

  // if( !success )
  // {

  //   std::cerr << "Did not successfully create a window" << std::endl;
  //   return -2;

  // }

  // success = graphicsContext.loadExtensions();

  // if( !success )
  // {

  //   std::cerr << "Did not succesfully load extensions" << std::endl;

  // }


  // // Load Extensions

  // std::cout << "Exiting the program succesfully" << std::endl;
}


/// \brief Parse the arguments and fill in the property tree for DisplayImage
void parseArgs( int argc, char* argv[] )
{

  po::options_description desc( "Inputs" );

  desc.add_options()
    ( "help", "Display help message" )
    ( "filename", po::value< std::string >(), "Filename to display" )
    ( "windowWidth", po::value< unsigned int >(), "Width of the display window" )
    ( "windowHeight", po::value< unsigned int >(), "Height of the display window" )
    ( "fileType", po::value< std::string >(), "Type of the file to display" )
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if( vm.size() == 0 )
  {

    std::cout << desc << std::endl;
    exit( 1 );
  }

  if (vm.find("help") != vm.end() )
  {

    std::cout << desc << "\n";
    exit( 1 );

  }


  if( vm.count( "filename" ) )
  {

    propTree_.put(
                  "filename",
                  vm[ "filename" ].as< std::string >()
                  );

  }


  if( vm.find( "windowWidth" ) != vm.end() )
  {

    propTree_.put(
                  "windowWidth",
                  vm[ "windowWidth" ].as< unsigned int >()
                  );

  }

  if( vm.find( "windowHeight" ) != vm.end() )
  {

    propTree_.put(
                  "windowHeight",
                  vm[ "windowHeight" ].as< unsigned int >()
                  );
  }

  if( vm.find( "fileType" ) != vm.end() )
  {

    propTree_.put(
                  "fileType",
                  vm[ "fileType" ].as< std::string >()
                  );

  }
}