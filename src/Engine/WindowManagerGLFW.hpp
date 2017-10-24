#ifndef WindowManagerGLFW_hpp
#define WindowManagerGLFW_hpp

#include <map>
#include <string>

class WindowRendererOGL;

struct GLFWwindow;

///
/// \brief WindowManagerGLFW assumes that there is already a valid context
///        and manages window creation and usage
/// \todo Pass in a window renderer for it to use to render to itself
class WindowManagerGLFW
{

public:

  WindowManagerGLFW( WindowRendererOGL* pWinRenderer = 0 );

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

protected:

  std::map< size_t, GLFWwindow* > idToWindowMap_;
  size_t nextWindowID_;

  WindowRendererOGL *pWinRenderer_;

};

#endif // WindowManagerGLFW_hpp