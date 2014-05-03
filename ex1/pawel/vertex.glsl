#version 120

attribute vec3 position;
attribute vec3 color;
attribute vec2 texcoord;

varying vec3 Color;
varying vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
  Color = color;
  Texcoord = texcoord;
  gl_Position = proj * view * model * vec4(position, 1.0);
}