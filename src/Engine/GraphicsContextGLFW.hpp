#ifndef GraphicsContextGLFW_hpp
#define GraphicsContextGLFW_hpp

/// \brief Context is used to manage the rendering context, this one uses GLFW and eventually glad
/// \todo initialize glad
///
class GraphicsContextGLFW
{

public:

  GraphicsContextGLFW();


  virtual ~GraphicsContextGLFW();


  /// \brief initialize the context making sure we haven't already initialized it
  virtual bool initialize();

  /// \brief If we've been initialized destroy the glfw context cleanly
  virtual bool finalize();

  virtual bool loadExtensions();

  bool isInitialized();

protected:

  bool initialized_;

};

#endif // GraphicsContextGLFW_hpp