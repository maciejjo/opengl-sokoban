#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <kazmath/kazmath.h>

#include "shader_utils.h"

#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

const char *vs_filename = "src/shaders/vertex.glsl";
const char *fs_filename = "src/shaders/fragment.glsl";

int main()
{


  // Init GLFW
  GLFWwindow *window;

  if(!glfwInit()) {
    fprintf(stderr, "Error initializing glfw\n");
    return EXIT_FAILURE;
  }

  window = glfwCreateWindow(800, 600, "Pszepyszna pszygoda pszemka", NULL, NULL);
  if(!window) {
    fprintf(stderr, "Error initializing glfw\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // Set vsync rate
  glfwSwapInterval(1);

  // Init GLEW
  glewExperimental = GL_TRUE;
  glewInit();

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Create VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Fill VBO
  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create EBO
  GLuint ebo;
  glGenBuffers(1, &ebo);

  // Fill EBO
  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0,
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  GLuint vs, fs;
  if((vs = create_shader(vs_filename, GL_VERTEX_SHADER)) == 0) return EXIT_FAILURE;
  if((fs = create_shader(fs_filename, GL_FRAGMENT_SHADER)) == 0) return EXIT_FAILURE;
  GLuint shader_program = glCreateProgram();

  if(!shader_program) {
    fprintf(stderr, "Error creating shader shader_program\n");
    return -1;
  }

  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  // *** Specify layout of vertex data **
  GLint posAttrib = glGetAttribLocation(shader_program, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

  GLint colAttrib = glGetAttribLocation(shader_program, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void *)(3 * sizeof(float)));

  GLint texAttrib = glGetAttribLocation(shader_program, "texcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void *) (6 * sizeof(float)));
  // ***********************************

  // ********* Load textures ***********
  GLuint textures[2];
  glGenTextures(2, textures);

  int width, height;
  unsigned char *image;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  image = SOIL_load_image("crate.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shader_program, "tex_crate"), 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  image = SOIL_load_image("shit.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shader_program, "tex_shit"), 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // ***********************************

  GLint uniModel = glGetUniformLocation(shader_program, "model");

  kmMat4 view;
  kmVec3 p_eye = { 5.2f, 1.2f, 2.2f };
  kmVec3 p_ctr = { 0.0f, 0.0f, 0.0f };
  kmVec3 p_up  = { 0.0f, 0.0f, 1.0f };
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
  GLint uniView = glGetUniformLocation(shader_program, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);
 

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 45.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 10.0f);
  GLint uniProj = glGetUniformLocation(shader_program, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);


  while(!glfwWindowShouldClose(window)) {

    // Set bg color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    kmMat4 model;
    kmMat4Identity(&model);
    kmMat4RotationZ(&model, 10 * torad((float) clock() / (float) CLOCKS_PER_SEC * 180.0f));
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);


    glDrawArrays(GL_TRIANGLES, 0, 36);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 36, 6);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    kmMat4 tmp;
    kmMat4Translation(&tmp, 0.0f, 0.0f, -1.0f);
    kmMat4Multiply(&model, &model, &tmp);
    kmMat4Scaling(&tmp, 1.0f, 1.0f, -1.0f);
    kmMat4Multiply(&model, &model, &tmp);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisable(GL_STENCIL_TEST);


    glfwSwapBuffers(window);
    glfwPollEvents();


    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
