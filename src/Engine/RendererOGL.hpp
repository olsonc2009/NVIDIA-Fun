#ifndef RendererOGL_hpp
#define RendererOGL_hpp

#include <vector>
#include <string>
#include <map>

#include "ModelPackage.hpp"

class RenderableOGL;

/// \brief Takes care of rendering everything in the scene, all OpenGL calls should be here
class RendererOGL
{

public:

  RendererOGL();
  virtual ~RendererOGL();

  virtual bool initializeRenderable(
                                      const ModelPackage& modelPackage,
                                      size_t &renderableIdx
                                      );

  virtual bool renderId( size_t renderableIdx );

  virtual bool createProgram(
                              const std::vector< std::string > shaderFilenames,
                              unsigned int &programId
                              );


  /// \brief Create the number of shaders specified and return them in the passed in vector.
  /// There is an assumed order
  ///   1. Vertex
  ///   2. Fragment
  ///   3. Geometry ( optional ) NOT SUPPORTED YET
  ///   4. Is there more? ( optional ) NOT SUPPORTED YET
  static bool createShaders(
                            size_t numShaders,
                            std::vector< unsigned int >& shaderIds
                            );

  /// \brief Given a string and a shader ID compile the shader and check for errors true is a good compile false means errors
  static bool compileShader(
                            std::string shaderString,
                            unsigned int shaderId
                            );

  /// \brief link the shaders to the program ID that is passed in, right now you must have just called createProgram prior to calling this
  static bool linkShaders(
                          unsigned int programToLinkToId,
                          std::vector< unsigned int > shadersToLink
                          );

protected:

  std::map< size_t, RenderableOGL* > idToRenderableMap_;

  size_t nextRenderableId_;

};

#endif //RendererOGL_hpp