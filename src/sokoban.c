#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "shader.h"
#include "mesh.h"
#include "game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

float offset_x, offset_y;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_REPEAT && action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_LEFT:
            offset_x += 0.2f;
            printf("Strzałka w lewo\n");
            break;
        case GLFW_KEY_RIGHT:
            offset_x -= 0.2f;
            printf("Strzałka w prawo\n");
            break;
        case GLFW_KEY_UP:
            offset_y += 0.2f;
            printf("Strzałka w górę\n");
            break;
        case GLFW_KEY_DOWN:
            offset_y -= 0.2f;
            printf("Strzałka w dół\n");
            break;
        default:
            break;
    }
}

int main()
{
  if(!glfwInit()) {
    fprintf(stderr, "Error initializing glfw\n");
    return EXIT_FAILURE;
  }

  GLFWwindow *window;
  window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT,
      "Pszepyszna pszygoda pszemka", NULL, NULL);

  if(!window) {
    fprintf(stderr, "Error initializing glfw\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwSwapInterval(2);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  shader *s = shader_init("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  shader_program_create(s);
  glUseProgram(s->id);

  level *lv = level_create("lvl/lvl2");

  float trans = 2.0f;
  float mid_x = (lv->height -1) * trans  / 2;
  float mid_y = (lv->width -1 ) * trans  / 2;

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 90.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);
  GLint uniProj = glGetUniformLocation(s->id, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);

  printf("Kamera patrzy na %f, %f\n", mid_x, mid_y);

  kmVec3 p_eye = { -5.0f, 10.0f, 0.0f};
  kmVec3 p_ctr = { mid_x, 0.0f, mid_y};
  kmVec3 p_up  = { 0.0f, 1.0f, 0.0f };
  kmMat4 view;
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
  GLint uniView = glGetUniformLocation(s->id, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);

  struct mesh *monkey = mesh_create("mesh/box2.obj");
  mesh_load_shader(monkey, s);
  mesh_load_mesh(monkey);

  struct mesh *ball = mesh_create("mesh/sphere.obj");
  mesh_load_shader(ball, s);
  mesh_load_mesh(ball);

  double x,y;
  double p_x, p_y;

  kmMat4 yaw;
  kmMat4 pitch;
  kmVec3 Axis;
  kmVec3 Diff;

  for(int i =0; i < 5; i++) {
    for(int j =0; j < 5; j++) {
      printf("%d ", lv->map[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  float angle_x= 0.0f;
  float angle_y= 0.0f;
  float sum_angle_x = 0.0f, sum_angle_y = 0.0f;
  float angle= 0.0f;

  while(!glfwWindowShouldClose(window)) {

    kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);

    GLint uniView = glGetUniformLocation(s->id, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);

    p_x = x;
    p_y = y;
    glfwGetCursorPos(window, &x, &y);

    sum_angle_x += angle_x;
    sum_angle_y += angle_y;


    if(p_x > x)
      angle_x= 0.5f * (p_x - x);
    else if(p_x < x)
      angle_x= -0.5f * (x - p_x);
    else if(p_x == x)
      angle_x= 0.0f;

    if(p_y > y && sum_angle_y >= 34.0f)
      angle_y= 0.5f * (p_y - y);
    else if(p_y < y)
      angle_y= -0.5f * (y - p_y);
    else if(p_y == y)
      angle_y= 0.0f;

    printf("%.3f, %.3f\n", sum_angle_x, sum_angle_y);


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(monkey->vao);

    kmVec3Fill(&Axis, 0.0f, 1.0f, 0.0f);

    kmMat4RotationAxisAngle(&yaw, &Axis, torad(angle_x));
    kmVec3Subtract(&Diff, &p_eye, &p_ctr);
    kmVec3MultiplyMat4(&p_eye, &Diff, &yaw);
    kmVec3Add(&p_eye, &p_eye, &p_ctr);

    kmVec4 x_axis = { 1.0f, 0.0f, 0.0f, 0.0f };
    kmVec4MultiplyMat4(&x_axis, &x_axis, &view);
    kmVec3 right_axis = { x_axis.x, x_axis.y, x_axis.z };
    kmVec3Fill(&right_axis, 1.0f, 0.0f, 0.0f);
    /*
    kmVec3 pt, up;
    kmVec3Subtract(&pt, &p_eye, &p_ctr);
    kmVec3Subtract(&up, &p_up, &p_eye);
    kmVec3Mul(&right_axis, &pt, &up);
    */

    kmMat4RotationAxisAngle(&pitch, &right_axis, torad(angle_y));
    kmVec3Subtract(&Diff, &p_eye, &p_ctr);
    kmVec3MultiplyMat4(&p_eye, &Diff, &pitch);
    kmVec3Add(&p_eye, &p_eye, &p_ctr);

    //printf("Camera right: %f %f %f\n", right_axis.x, right_axis.y, right_axis.z);


    angle += 1.0f;


    for(int i = 0; i < lv->height; i++) {
      for(int j = 0; j < lv->width; j++) {


        if(lv->map[i][j] == 1) {
          kmMat4 model;
          kmMat4Identity(&model);
          kmMat4 rotate;
          kmMat4RotationY(&rotate, torad(angle));
          kmMat4Translation(&model, trans*i, 0, trans*j);
          GLint uniModel = glGetUniformLocation(s->id, "model");
          glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);
          glDrawArrays(GL_TRIANGLES, 0, monkey->v_no);
        }
      }
    }

    glBindVertexArray(ball->vao);
    kmMat4 model;
    kmMat4Translation(&model, mid_x + offset_x, 0, mid_y + offset_y);
    GLint uniModel = glGetUniformLocation(s->id, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);
    glDrawArrays(GL_TRIANGLES, 0, ball->v_no);

    glfwSwapBuffers(window);

    glfwPollEvents();

  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
