#version 120

//uniform vec3 triangleColor;
varying vec3 Color;
varying vec2 Texcoord;

uniform sampler2D tex_crate;
uniform sampler2D tex_shit;

void main()
{
  vec4 color = mix(texture2D(tex_crate, Texcoord), texture2D(tex_shit,Texcoord), 0.4);
  gl_FragColor = color * vec4(Color, 1.0);
}
