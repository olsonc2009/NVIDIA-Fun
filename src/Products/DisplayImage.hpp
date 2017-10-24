#ifndef DisplayImage_hpp
#define DisplayImage_hpp

#include <vector>
#include <string>

#include <boost/property_tree/ptree.hpp>

/// \brief Reference implementation to display an image to the screen using WindowManagerGLFW
/// \todo Add validation for tree settings
class DisplayImage
{

public:

  DisplayImage();
  ~DisplayImage();

  /// \brief Reset the state of this class, cleans up all memory and inputs to prepare for a whole new setup
  void reset();

  /// \brief sets the inputs and validates that at least the correct inputs are in the tree passed in before copying to member function
  bool setInputs( const boost::property_tree::ptree &tree );

  /// \brief Display the image based on the input tree
  bool displayImage();

  /// \brief Return the inputs that are currently in use
  const boost::property_tree::ptree& getInputs() const;

  const std::vector< std::string >& getInputKeys() const;

private:

  static bool validatePropTree( const boost::property_tree::ptree& inputTree );

  boost::property_tree::ptree inputTree_;

  std::vector< std::string > inputKeys_;

};


#endif // DisplayImage_hpp