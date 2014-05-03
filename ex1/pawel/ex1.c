// Biblioteki standardowe
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// Biblioteki dodatkowe
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <kazmath/kazmath.h>
#include <kazmath/vec4.h>

// Zarządzanie shaderami
#include "shader_utils.h"

// Zamiana stopnie <-> radiany
#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)

// Parametry okna
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Ścieżki do shaderów
const char *vs_filename = "vertex.glsl";
const char *fs_filename = "fragment.glsl";

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

  // Tu się zaczynają ciekawe rzeczy.

  // Na wstępie: W OpenGlu jak się chce coś zapisać na karcie graficznej,
  // to trzeba to robić przez tzw. obiekty. Wszystkie funkcje które się
  // zaczynają od glGen* służą do tworzenia takich obiektów. Działa to
  // jak otwieranie plików open'em - dostaje się deskryptor, którego się
  // potem używa do odwołania się do danego obiektu.

  // VAO to Vertex Array Object. Działa to tak, że zapamiętują się w nim
  // powiązania między wierzchołkami, a ich atrybutami (które są za chwilę)
  // Dlatego wystarczy aktywować VAO, wczytać atrybuty wierzchołków, potem można
  // aktywować inny vao, wczytać atrybuty innego modelu/czegokolwiek, i jak się
  // aktywuje znowu ten pierwszy, to te atrybuty wczytane wcześniej będą ciągle
  // wczytane.

  // Typy zmiennych w OGL'u nazywają się zawsze GLtyp. Tutaj jest GLuint czyli
  // unsigned int. Do niego chcemy uzyskać deskryptor obiektu.
  GLuint vao;
  // 1 oznacza, że chcemy stworzyć 1 obiekt. Do tego podajemy adres gdzie ma się
  // zapisać deskryptor. Czyli adres zmiennej vao.
  glGenVertexArrays(1, &vao);
  // W OpenGLu żeby używać jakiegoś obiektu, trzeba go zbindować (używając
  // deskryptora).
  glBindVertexArray(vao);

  // VBO to Vertex Buffer Object - czyli obiekt w którym się trzyma wierzchołki
  // na karcie graficznej. Nie trzyma się tam tylko współrzędnych wierzchołka,
  // ale też kolor, współrzędne textur i cokolwiek związane z wierzchołkiem.
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Tablica wierzchołków. Do VBO trzeba pisać z płaskiej tablicy. Więc po kolei
  // w rzędach podane są wierzchołki. Schemat jest następujący:
  // X      Y      Z     R     G     B     Xt    Yt
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

  unsigned int level_layout[5][5] = {
    { 1, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 1, 0, 0, 1 },
    { 1, 1, 1, 1, 1 },
  };


  // Żeby pisać do bufora, trzeba go zbindować. Bindujemy do targetu GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Właściwy zapis danych na GPU. Tutaj nie używamy deskryptora bufora, tylko
  // zapisujemy do obecnie zbindowanego bufora do GL_ARRAY_BUFFER.
  // dalej podajemy rozmiar danych, adres danych, i rodzaj rysowania
  // (do optymalizacji pamięci)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  // ***** Kompilacja shaderów *****
  GLuint vs, fs, shader_program;
  if((vs = create_shader(vs_filename, GL_VERTEX_SHADER)) == 0) return EXIT_FAILURE;
  if((fs = create_shader(fs_filename, GL_FRAGMENT_SHADER)) == 0) return EXIT_FAILURE;
  shader_program = glCreateProgram();

  if(!shader_program) {
    fprintf(stderr, "Error creating shader shader_program\n");
    return -1;
  }

  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);
  glUseProgram(shader_program);
  // ******************************

  // ***** Informacja dla shadera jak są ułożone dane w buforze *****
  GLint posAttrib = glGetAttribLocation(shader_program, "position");
  glEnableVertexAttribArray(posAttrib);
  // Tu precyzujemy ułożenie danych. Podajemy jaki atrybut (position), ile
  // ma danych (3 - x,y,z), jakiego typu (float), czy trzeba je normalizować
  // (GL_FALSE - czyli nie), ile zajmuje 1 wiersz danych (8, bo są x,y,z,r,g,b,xt,yt)
  // jakie jest przesunięcie względem początku wiersza (0, bo x,y,z są na początku)
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

  GLint colAttrib = glGetAttribLocation(shader_program, "color");
  glEnableVertexAttribArray(colAttrib);
  // Tu zmienione przesunięcie, bo r,g,b są po 3 wartościach: x,y,z
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void *)(3 * sizeof(float)));

  GLint texAttrib = glGetAttribLocation(shader_program, "texcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void *) (6 * sizeof(float)));
  // ***************************************************************

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
  glUniform1i(glGetUniformLocation(shader_program, "tex_crate"), 0);

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
  glUniform1i(glGetUniformLocation(shader_program, "tex_shit"), 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // ******************************



  // ***** Matematyka *****
  // Zapisujemy do shadera macierze przez które będziemy mnożyć wierzchołki.
  // Są to M - model, V - view, P - projection. Pomnożenie przez M przekłada
  // ze współrzędnych modelu na współrzędne świata (obracanie, skalowanie,
  // przesuwanie). V obraca i przesuwa świat żeby znalazł się przed kamerą.
  // P tworzy perspektywę. Po pomnożeniu przez P jest wykonywane podzielenie
  // wszystkich współrzędnych przez czwartą współrzędną, co daje nam NDC,
  // czyli Normalized Device Coordinates.

  // Nie modyfikujemy współrzędnych modelu - więc do macierzy M
  // przypisujemy macierz jednostkową.

  // Funkcja LookAt tworzy za nas macierz V, trzeba tylko podać
  // współrzędne oka, punktu na który patrzymy, i wektor wyznaczający
  // górę.


  // Funkcja PerspectiveProjection tworzy macierz P. Przyjmuje
  // kąt widzenia, aspect ratio, oraz położenie płaszczyzn
  // przycinania z_near i z_far.
  kmMat4 proj;
  kmMat4PerspectiveProjection(&proj, 45.0f, (float) WIN_WIDTH / (float) WIN_HEIGHT, 1.0f, 100.0f);
  GLint uniProj = glGetUniformLocation(shader_program, "proj");
  // Zapis do shadera.
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &proj.mat[0]);
  // *********************

  // Ustawienie koloru czyszczenia ekranu
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    kmVec3 p_eye = { 8.0f, 3.0f, 8.0f };
    kmVec3 p_ctr = { 3.0f, 0.0f, 3.0f };
    kmVec3 p_up  = { 0.0f, 1.0f, 0.0f };

  // Pętla programu
  while(!glfwWindowShouldClose(window)) {

    // Czyścimy ekran na zadany kolor
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < 5; i++) {
      for(int j = 0; j < 5; j++) {
        if(level_layout[i][j]) {
          kmMat4 model;
          kmMat4Identity(&model);
          GLint uniModel = glGetUniformLocation(shader_program, "model");
          kmMat4Translation(&model, (float) i, 0.0f, (float) j);
          // Zapis do shadera.
          glUniformMatrix4fv(uniModel, 1, GL_FALSE, &model.mat[0]);
          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
      }
    }

    kmMat4 view;
    kmMat4 rotation;
    kmMat4RotationYawPitchRoll(&rotation, 0.0f, torad(1.0f), 0.0f);
    kmVec3MultiplyMat4(&p_eye, &p_eye, &rotation);
    kmMat4LookAt(&view, &p_eye, &p_ctr, &p_up);
    GLint uniView = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view.mat[0]);



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
