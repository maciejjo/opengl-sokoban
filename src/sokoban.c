// Biblioteki standardowe
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


// Zarządzanie shaderami
#include "shader.h"
#include "mesh.h"

// Zamiana stopnie <-> radiany
#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)

// Parametry okna
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

// Liczba pi
# define M_PI		3.14159265358979323846	/* pi */

// Ścieżki do shaderów
const char *vs_filename = "src/shaders/vertex.glsl";
const char *fs_filename = "src/shaders/fragment.glsl";

int main()
{

  // ***** Inicjalizacja GLFW *****
  if(!glfwInit()) {
    fprintf(stderr, "Error initializing glfw\n");
    return EXIT_FAILURE;
  }

  // Utworzenie okienka
  GLFWwindow *window;
  window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Pszepyszna pszygoda pszemka", NULL, NULL);
  if(!window) {
    fprintf(stderr, "Error initializing glfw\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Aktywowanie okienka
  glfwMakeContextCurrent(window);

  // Ustawienie wersji OpenGL'a
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Włącza VSYNC. (Renderowanie przy określonej liczbie klatek na sekundę)
  // Bez tego klatki będą renderowane tak często jak to możliwe, przez co
  // zużycie procesora będzie wynosiło kilkadziesiąt procent.
  // 0 = wyłączony VSYNC, 1 = 60 FPS, 2 = 30 FPS, i tak dalej.
  glfwSwapInterval(1);
  // *****************************

  // Inicjalizacja GLEW.
  // GLEW zarządza funkcjami OpenGL'a.
  glewExperimental = GL_TRUE;
  glewInit();

  // Włączenie Depth testu, czyli sprawdzania, czy dany trójkąt
  // nie jest zasłonięty przez inny. Jeśli jest, to nie jest
  // rysowany.
  glEnable(GL_DEPTH_TEST);

  // Ustawienie koloru czyszczenia ekranu
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


  // ***** Kompilacja shaderów *****
  shader *s = shader_init(vs_filename, fs_filename);
  shader_program_create(s);
  glUseProgram(s->id);
  // ******************************

  /*
  // *****  Ładowanie tekstur *****
  GLuint textures[2];
  glGenTextures(2, textures);

  int width, height;
  unsigned char *image;

  // Zapisujemy teksturę do GL_TEXTURE0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  image = SOIL_load_image("crate.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  // więc tu trzeba ustawić w shaderze parametr tex_crate na 0
  glUniform1i(glGetUniformLocation(s->id, "tex_crate"), 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Drugą teksutrę zapisujemy do GL_TEXTURE1
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  image = SOIL_load_image("shit.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  // więc tu piszemy do shadera tex_shit na 1
  glUniform1i(glGetUniformLocation(s->id, "tex_shit"), 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  */
  // ******************************

  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 45.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);
  GLint uniProj = glGetUniformLocation(s->id, "proj");
  // Zapis do shadera.
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);
  // *********************

  kmVec3 p_eye = { 0.1f, 5.0f, 0.0f };
  kmVec3 p_ctr = { 0.0f, 0.0f, 0.0f };
  kmVec3 p_up  = { 0.0f, 1.0f, 0.0f };
  kmMat4 view;
  kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
  GLint uniView = glGetUniformLocation(s->id, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);


  struct mesh *monkey = mesh_create("mesh/monkey.obj");
  mesh_load_shader(monkey, s);
  mesh_load_mesh(monkey);
  printf("%s\n", monkey->filename);
  printf("Vertex no: %d\n", monkey->v_no);
  printf("Faces no: %d\n", monkey->i_no);

  // Pętla programu
  float angle = 0.0f;
  while(!glfwWindowShouldClose(window)) {

    angle += 1.0f;

    // Czyścimy ekran na zadany kolor
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(monkey->vao);
    //glBindBuffer(GL_ARRAY_BUFFER, monkey->vbo_vert);
    glDrawArrays(GL_TRIANGLES, 0, monkey->v_no);

    kmMat4 model;
    kmMat4 fix_rotation;
    kmMat4Identity(&model);
    kmMat4RotationZ(&fix_rotation, torad(90));
    kmMat4RotationY(&model, torad(angle));
    kmMat4Multiply(&model, &fix_rotation, &model);

    GLint uniModel = glGetUniformLocation(s->id, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);

    // Zamiana przedniego bufora z tylnim
    glfwSwapBuffers(window);

    // Obsługa klawiatury
    glfwPollEvents();
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  }


  glfwTerminate();

  return EXIT_SUCCESS;
}
