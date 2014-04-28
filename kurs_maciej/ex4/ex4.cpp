#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math_3d.h"
#include "shader_utils.h"

GLuint VBO;

const char *vertex_shader_filename = "vertex.glsl";
const char *fragment_shader_filename = "fragment.glsl";

static void
render_scene()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);

  glutSwapBuffers();
}

static void
init_glut_callbacks()
{
  glutDisplayFunc(render_scene);
}

static void
create_vbo()
{
  Vector3f vertices[3];
  vertices[0] = Vector3f( -1.0f, -1.0f,  0.0f);
  vertices[1] = Vector3f(  1.0f, -1.0f,  0.0f);
  vertices[2] = Vector3f(  0.0f,  1.0f,  0.0f);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

static int
compile_shaders()
{
  GLint link_status = GL_FALSE;

  GLuint vs, fs;
  if((vs = create_shader(vertex_shader_filename, GL_VERTEX_SHADER)) == 0) return -1;
  if((fs = create_shader(fragment_shader_filename, GL_FRAGMENT_SHADER)) == 0) return -1;

  GLuint program = glCreateProgram();

  if(!program) {
    fprintf(stderr, "Error creating shader program\n");
    return -1;
  }

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);

  if(!link_status) {
    fprintf(stderr, "glLinkProgram");
    print_log(program);
    return -1;
  }

  glUseProgram(program);
  return 0;
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
  glutInitWindowSize(640,480);
  glutCreateWindow("ex4");

  init_glut_callbacks();

  int glew_status;
  if((glew_status = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return EXIT_FAILURE;
  }

  printf("GL version: %s\n", glGetString(GL_VERSION));

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  create_vbo();

  if(compile_shaders())
    return EXIT_FAILURE;

  glutMainLoop();

  return EXIT_SUCCESS;
}
