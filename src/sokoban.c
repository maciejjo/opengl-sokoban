#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "shader.h"
#include "mesh.h"
#include "game.h"

# define M_PI		3.14159265358979323846
#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)

#define WIN_WIDTH 1366
#define WIN_HEIGHT 768

int main()
{
  if(!glfwInit()) {
    fprintf(stderr, "Error initializing glfw\n");
    return EXIT_FAILURE;
  }

  GLFWwindow *window;
  window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Pszepyszna pszygoda pszemka", NULL, NULL);
  if(!window) {
    fprintf(stderr, "Error initializing glfw\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  shader *s = shader_init("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  shader_program_create(s);
  glUseProgram(s->id);

  level *lv = level_create("lvl/lvl1");

  float trans = 2.5;
  float mid_x = (lv->height -1) * trans  / 2;
  float mid_y = (lv->width -1 ) * trans  / 2;

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 90.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);
  GLint uniProj = glGetUniformLocation(s->id, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);

  printf("Kamera patrzy na %f, %f\n", mid_x, mid_y);

  kmVec3 p_eye = { 0.0f, 10.0f, 0.0f};
  kmVec3 p_ctr = { mid_x, 0.0f, mid_y};
  kmVec3 p_up  = { 0.0f, 1.0f, 0.0f };
  kmMat4 view;
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
  GLint uniView = glGetUniformLocation(s->id, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);

  struct mesh *monkey = mesh_create("mesh/box2.obj");
  mesh_load_shader(monkey, s);
  mesh_load_mesh(monkey);

  double x,y;
  double p_x, p_y;

    kmMat4 yaw;
    kmVec3 Axis;
    kmVec3 Diff;

  float angle_x= 0.0f;
  float angle_y= 0.0f;
  while(!glfwWindowShouldClose(window)) {

    p_x = x;
    p_y = y;
    glfwGetCursorPos(window, &x, &y);


    if(p_x > x)
      angle_x= 0.5f * (p_x - x);
    else if(p_x < x)
      angle_x= -0.5f * (x - p_x);
    else if(p_x == x)
      angle_x= 0.0f;

    if(p_y > y)
      angle_y= 0.5f * (p_y - y);
    else if(p_y < y)
      angle_y= -0.5f * (y - p_y);
    else if(p_y == y)
      angle_y= 0.0f;



    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(monkey->vao);


    kmVec3Fill(&Axis, 0.0f, 1.0f, 0.0f);
    kmVec3Subtract(&Diff, &p_eye, &p_ctr);
    //kmVec3Mul(&Axis, &Axis, &Diff);
    kmMat4RotationAxisAngle(&yaw, &Axis, torad(angle_x));
    //kmMat4RotationY(&yaw, torad(angle_frame));
    kmVec3MultiplyMat4(&p_eye, &Diff, &yaw);
    kmVec3Add(&p_eye, &p_eye, &p_ctr);
    kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
    GLint uniView = glGetUniformLocation(s->id, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);


    for(int i = 0; i < lv->height; i++) {
      for(int j = 0; j < lv->width; j++) {


        kmMat4 model;
        kmMat4Identity(&model);
        kmMat4 rotatez;
        kmMat4RotationX(&rotatez, torad(90));
        kmMat4Translation(&model, trans*i, 0, trans*j);
        kmMat4Multiply(&model, &model, &rotatez);
        GLint uniModel = glGetUniformLocation(s->id, "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);
        glDrawArrays(GL_TRIANGLES, 0, monkey->v_no);
      }
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
