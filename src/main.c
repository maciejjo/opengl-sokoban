#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <kazmath/kazmath.h>

#define WIN_WIDTH   640
#define WIN_HEIGHT  480

static void
render_scene()
{
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

  /*
  create_vbo();

  if(compile_shaders())
    return EXIT_FAILURE;
  */

  glutMainLoop();

  return EXIT_SUCCESS;
}
