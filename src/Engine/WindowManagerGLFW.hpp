#ifndef WindowManagerGLFW_hpp
#define WindowManagerGLFW_hpp

#include <map>
#include <string>
#include <vector>

class GraphicsContextGLFW;
class ModelPackage;
class ScreenPackage;
class RendererOGL;

struct GLFWwindow;

///
/// \brief WindowManagerGLFW assumes that there is already a valid context
///        and manages window creation and usage
/// \todo Pass in a window renderer for it to use to render to itself
/// \todo Remove GLFWwindow from forward decleration list in header
///
class WindowManagerGLFW
{

public:

  WindowManagerGLFW( GraphicsContextGLFW* pGraphicsContext = 0 );

  virtual ~WindowManagerGLFW();

  /// \brief Destroy any managed windows
  virtual bool finalize();

  /// \ creates a new window with the specified parameters
  /// \todo Remove GLFWwindow return by reference as it is too specific to this implmentation
  virtual bool createWindow(
                            size_t &retID,
                            GLFWwindow*& pRetWin,
                            const size_t winHeight = 640,
                            const size_t winWidth = 480,
                            const std::string winTitle = "NoTitle",
                            const int openGlMajorVersion = 3,
                            const int openGlMinorVersion = 3
                            );

  /// \brief Make the window index current
  /// \return true if the window exists false otherwise
  ///
  virtual bool grabWindow( size_t windowIdx );

  /// \brief Call glfwSwapBuffer on the window specified
  virtual bool swapBuffers( size_t windowIdx );

  /// \brief Set the input mode on the window
  virtual bool setInputMode( size_t windowIdx, unsigned int mode );

  /// \brief get the window, returns null if it doesn't exist
  GLFWwindow* getWindow( size_t windowIdx );

  /// \brief Render data which should have the dimensions dataDims to the screen, also assumes channel data is interleaved
  virtual bool renderToWindow(
                              size_t windowToRenderTo,
                              const std::vector< float > data,
                              const std::vector< unsigned int > dataDims,
                              unsigned int numDataChannels,
                              RendererOGL *pRenderer
                              );
  //virtual bool renderToWindow( const ModelPackage& modelPackage );

protected:

  std::map< size_t, GLFWwindow* > idToWindowMap_;
  size_t nextWindowID_;

  GraphicsContextGLFW *pGraphicsContext_;

  bool initializedRenderingPackage_;

  ModelPackage* pModelPackage_;

  //
  // Initial size related stuff
  //
  unsigned int renderWidth_;
  unsigned int renderHeight_;
  unsigned int renderChannels_;

  std::vector< float > renderData_;

  // OpenGL Stuff that has to move
  /// \todo Move this OpenGL stuff to RendererOGL
  unsigned int fbo_;
  unsigned int fboTex_;

  size_t renderableIdx_;

};

#endif // WindowManagerGLFW_hpp