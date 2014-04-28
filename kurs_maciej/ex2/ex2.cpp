/*****
 * ex2 - przykładowy program nr 2
 * zadanie: wyświetlenie pojedyńczego punktu
 *
 *****/

#include <stdlib.h>
#include <stdio.h>
// Dołączenie GLEW
#include <GL/glew.h>
// Dołączenie freegluta
#include <GL/freeglut.h>
#include "math_3d.h"

// Deskryptor VBO
GLuint VBO;

static void
render_scene()
{
  // Wyczyszczenie ekranu zadanym kolorem
  glClear(GL_COLOR_BUFFER_BIT);
  // Atrybuty wierzchołków mają swój indeks. Aby używać ich w shaderach
  // Trzeba je najpierw włączuć w programie. Tutaj używamy tzw "fixed
  // pipeline", gdzie atrybut 0 oznacza pozycję wierzchołka którą
  // załadowaliśmy do VBO. fixed pipe jest używane gdy nie ma zbindowanego
  // żadnego shadera.
  glEnableVertexAttribArray(0);
  // Ponownie bindujemy bufor
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // W tej funkcji precyzjujemy interpretację danych
  glVertexAttribPointer(
      0,        // indeks atrybutu (fixed pipe - więc 0)
      3,        // ilość komponentów w atrybucie (x,y,z)
      GL_FLOAT, // typ komponentów
      GL_FALSE, // czy normalizować dane (nie)
      0,        // przerwy między porcjami danych - stride
      0         // przesunięcie gdzie zaczynają się dane - offset
      );

  // Rysowanie - GL_POINTS to metoda rysowania, rysujemy wierzchołek o id 0,
  // rysujemy 1 wierzchołek
  glDrawArrays(GL_POINTS, 0, 1);
  glDisableVertexAttribArray(0);

  // Zamiana buforów (double buffering)
  glutSwapBuffers();
}

static void
create_vertex_buffer()
{
  Vector3f Vertices;
  Vertices = Vector3f(0.5f, 0.7f, 1.0f);

  // Właściwe generowanie bufora
  // pierwszy arguemnt: ile obiektów, drugi: zmienne gdzie zapisać ich deskryptory
  glGenBuffers(1, &VBO);
  // Zbindowanie deskryptora do targetu - GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Operacja na targecie (nie używamy tu deksryptora
  glBufferData(         // funkcja wpisania danych do bufora
      GL_ARRAY_BUFFER,  // target na jakim operujemy
      sizeof(Vertices), // rozmiar danych
      &Vertices,        // adres danych do wpisania
      GL_STATIC_DRAW    // hint dla OGL co będzie się działo z danymi
      );
  // Zbindowanie 0 oznacza odbindowanie poprzedniego deskryptora
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


static void
init_glut_callbacks()
{
  // Ustawienie callbacku który uruchamia się jednorazowo
  // przy starcie okna freegluta (lub wielorazowo (?))
  glutDisplayFunc(render_scene);
}

int
main(int argc, char **argv)
{
  // Inicjalizacja gluta
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  // GLUT_DOUBLE  - aktywny bufor podwójny
  // GLUT_RGBA    - aktywny bufor kolorów (ekran)
  glutInitWindowSize(800, 600);
  glutCreateWindow("ex1");

  // Własna funkcja inicjalizująca callbacki gluta
  // Callback to funkcja która wykonuje się pod pewnymi warunkami
  init_glut_callbacks();

  // Inicjalizacja GLEW - musi być wykonana po inicie gluta
  GLenum res = glewInit();
  if(res != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
    return EXIT_FAILURE;
  }

  // Ustawia stan - OpenGL to maszyna stanów - są to wartości koloru,
  // którym wypełniany jest bufor przed renderowaniem.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Funkcja tworząca bufor pod 1 wierzchołek
  create_vertex_buffer();

  // Pętla programu
  glutMainLoop();

  return EXIT_SUCCESS;
}
