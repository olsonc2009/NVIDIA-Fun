#ifndef RenderableOGL_hpp
#define RenderableOGL_hpp


/// \brief Containter to hold onto all of the openGL crap
class RenderableOGL
{

public:

  RenderableOGL();
  ~RenderableOGL();

  unsigned int vertexArrayObject_;
  unsigned int geomBufferID_;
  unsigned int textureID_;
  unsigned int texCoordBufferID_;
  unsigned int indexBufferID_;
  unsigned int shaderID_;

  unsigned int numElementsToRender_;

};


#endif // RenderableOGL_hpp