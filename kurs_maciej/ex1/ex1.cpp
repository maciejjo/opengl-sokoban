/*****
 * ex1 - przykładowy program nr 1
 * zadanie: utworzenie pustego okna używając freegluta
 *
 *****/

#include <stdlib.h>

// Dołączenie freegluta
#include <GL/freeglut.h>

static void
render_scene()
{
  // Wyczyszczenie ekranu zadanym kolorem
  glClear(GL_COLOR_BUFFER_BIT);
  // Zamiana buforów (double buffering)
  glutSwapBuffers();
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

  // Ustawia stan - OpenGL to maszyna stanów - są to wartości koloru,
  // którym wypełniany jest bufor przed renderowaniem.
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Pętla programu
  glutMainLoop();

  return EXIT_SUCCESS;
}
