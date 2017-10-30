// Awesome fragment shader
#version 330 core

in vec2 uvCoords;

out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{

  color = vec3( 0, 0, 0 );

  //color.rgb = vec3( 0, 0, 0 );
  //color.rg = uvCoords.xy;

  color.rgb = texture( myTextureSampler, uvCoords ).rgb;

}