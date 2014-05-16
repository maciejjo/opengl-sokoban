#version 120

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texcoord;

varying vec3 Color;
varying vec2 Texcoord;
varying vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
  Color = color;
  Texcoord = texcoord;
  Normal = normal;
  gl_Position = proj * view * model * vec4(position, 1.0);
}
