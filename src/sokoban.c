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

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

const char *vs_filename = "src/shaders/vertex.glsl";
const char *fs_filename = "src/shaders/fragment.glsl";

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

  shader *s = shader_init(vs_filename, fs_filename);
  shader_program_create(s);
  glUseProgram(s->id);

  level *lv = level_create("lvl/lvl1");

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 90.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);
  GLint uniProj = glGetUniformLocation(s->id, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);

  kmVec3 p_eye = { 0.0f, 5.0f, 15.0f };
  kmVec3 p_ctr = { (3.0f * lv->height)/2.0f, 1.5f, (3.0f * lv->width)/2.0f};
  kmVec3 p_up  = { 0.0f, 1.0f, 0.0f };
  kmMat4 view;
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
  GLint uniView = glGetUniformLocation(s->id, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);

  struct mesh *monkey = mesh_create("mesh/monkey.obj");
  mesh_load_shader(monkey, s);
  mesh_load_mesh(monkey);


  float angle_frame = 1.0f;
  float angle = 0.0f;
  while(!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(monkey->vao);

    angle += 1.1f;

    kmMat4 yaw;
    kmMat4RotationY(&yaw, torad(angle_frame));
    //kmMat4Multiply(&view, &view, &yaw);
    kmVec3MultiplyMat4(&p_eye, &p_eye, &yaw);
    kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
    GLint uniView = glGetUniformLocation(s->id, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);

    for(int i = 0; i < lv->height; i++) {
      for(int j = 0; j < lv->width; j++) {


        kmMat4 model;
        kmMat4Identity(&model);
        kmMat4 rotate;
        kmMat4RotationY(&rotate, torad(angle));
        kmMat4Translation(&model, 2.5*i, 0, 2.5*j);
        kmMat4Multiply(&model, &model, &rotate);
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
