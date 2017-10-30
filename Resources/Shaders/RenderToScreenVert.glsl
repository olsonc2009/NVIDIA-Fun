// Awesome sauce vertex shader
#version 330 core

layout( location = 0 ) in vec3 vertPosModel;
layout( location = 1 ) in vec2 texCoords;

out vec2 uvCoords;

void main()
{

  gl_Position.xyz = vertPosModel;
  gl_Position.w   = 1.0;

  uvCoords = texCoords;

}