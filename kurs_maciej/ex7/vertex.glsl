#version 120

attribute vec3 Position;
//uniform float Scale;
uniform mat4 World;

void main()
{
  gl_Position = World * vec4(Position, 1.0);
}
