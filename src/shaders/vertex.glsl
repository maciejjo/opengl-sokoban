#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


varying vec3 Color;
varying vec2 Texcoord;

varying vec4 l;
varying vec4 n;
varying vec4 v;


void main()
{

  vec4 lightPos = vec4(0,7,0,1);
  l = normalize(view*lightPos - view*model*vec4(position,1));
  n = normalize(view*model*vec4(normal,0));
  v = normalize(vec4(0,0,0,1) - view*model*vec4(position,1));

  Texcoord = texcoord;
  gl_Position = proj * view * model * vec4(position, 1.0);
  Color = vec3(0,1,0.5);
}
