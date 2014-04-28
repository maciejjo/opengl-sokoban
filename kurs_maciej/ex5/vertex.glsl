#version 120

attribute vec3 Position;
uniform float Scale;

void main()
{
  gl_Position = vec4(Scale * Position.x, Scale * Position.y, Position.z, 1.0);
}
