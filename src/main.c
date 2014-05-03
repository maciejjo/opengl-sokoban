#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <kazmath/kazmath.h>

#include "shader_utils.h"

#define WIN_WIDTH   640
#define WIN_HEIGHT  480

GLuint VBO;
GLuint IBO;
GLuint World_Location;

const char *vs_filename = "src/shaders/vertex.glsl";
const char *fs_filename = "src/shaders/fragment.glsl";

static void
render_scene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();

  static float scale = 0.0f;
  scale += 0.5f;

  kmMat4 model;
  kmMat4Identity(&model);
  kmMat4RotationY(&model, scale);
  kmMat4Translation(&model, 0.0f, 0.0f, 3.0f);

  kmMat4 view;
  kmVec3 p_eye = {  0.0f,  0.0f, -3.0f };
  kmVec3 p_ctr = {  0.0f,  0.0f,  2.0f };
  kmVec3 p_up  = {  0.0f,  1.0f,  0.0f };
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 60.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);

  kmMat4Multiply(&model, &model, &view);
  kmMat4Multiply(&model, &model, &proj);

  glUniformMatrix4fv(World_Location, 1, GL_TRUE, &model.mat[0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

static void
create_vbo()
{

  kmVec3 vertices[3] = {
    { -1.0f, -1.0f,  0.0f },
    {  0.0f, -1.0f,  1.0f },
    {  0.0f,  1.0f,  0.0f },
  };

  unsigned int indices[] =
  {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2,
  };

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

static int
compile_shaders()
{
  GLint link_status = GL_FALSE;

  GLuint vs, fs;

  if((vs = create_shader(vs_filename, GL_VERTEX_SHADER)) == 0) return -1;
  if((fs = create_shader(fs_filename, GL_FRAGMENT_SHADER)) == 0) return -1;

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

  World_Location = glGetUniformLocation(program, "World");
  assert(World_Location != 0xFFFFFFFF);

  return 0;
}


static void
init_glut_callbacks()
{
  glutDisplayFunc(render_scene);
  glutIdleFunc(render_scene);
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow("sokoban: pszepyszna pszygoda pszemka");

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
