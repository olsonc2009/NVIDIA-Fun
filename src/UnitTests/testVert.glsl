// Awesome sauce vertex shader
#version 330 core

layout( location = 0 ) in vec3 vertPosModel;

void main()
{

  gl_Position.xyz = vertPosModel;
  gl_Position.w   = 1.0;

}