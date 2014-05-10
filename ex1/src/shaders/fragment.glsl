#version 120

//uniform vec3 triangleColor;
varying vec3 Color;
varying vec2 Texcoord;
varying vec3 Normal;

uniform sampler2D tex_crate;
uniform sampler2D tex_shit;

void main()
{
   //texture2D(tex_crate, Texcoord);
  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
