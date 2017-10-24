#
# Set up for building the engine
#
set( ENGINE_TGT engine )
set( 
	ENGINE_SRC 
	GraphicsContextGLFW.cpp
	WindowManagerGLFW.cpp
	RendererOGL.cpp
	RenderableOGL.cpp
	ModelPackage.cpp
	)

include_directories( 
	                ${GLFW_DIRS}/include 
	                ${GLAD_INCLUDE_DIR} 
	                ${UTIL_DIR}
	                )

add_library( ${ENGINE_TGT} ${ENGINE_SRC} )
add_dependencies( ${ENGINE_TGT} ${GLAD_TGT} ${UTIL_TGT} )

set( ENGINE_TGT ${ENGINE_TGT} PARENT_SCOPE )