#include <iostream>

#include "WindowManagerGLFW.hpp"
#include "GraphicsContextGLFW.hpp"
#include "RendererOGL.hpp"

int main( int argc, char* argv[] )
{

  std::cout << "Starting up a window example" << std::endl;


  GraphicsContextGLFW graphicsContext;
  WindowManagerGLFW   windowManager;

  graphicsContext.initialize();

  size_t windowIdx;
  GLFWwindow* pWin;

  windowManager.createWindow( windowIdx, pWin );

}