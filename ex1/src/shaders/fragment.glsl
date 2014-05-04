#version 120

//uniform vec3 triangleColor;
varying vec3 Color;
varying vec2 Texcoord;

uniform sampler2D tex_crate;
uniform sampler2D tex_shit;

void main()
{
  vec4 color = texture2D(tex_crate, Texcoord);
  gl_FragColor = color * vec4(Color, 1.0);
}
