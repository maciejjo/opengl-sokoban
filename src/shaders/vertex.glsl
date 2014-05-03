#version 120

attribute vec3 Position;
//uniform float Scale;
uniform mat4 World;
varying vec4 Color;

void main()
{
  gl_Position = World * vec4(Position, 1.0);
  Color = vec4(clamp(Position, 0.2, 1.0), 1.0);
}
