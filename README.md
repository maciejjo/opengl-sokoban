Sokoban
=======

46. Trójwymiarowa wersja gry: „Sokoban”.

Dana jest plansza reprezentująca magazyn, w którym rozmieszczone są skrzynie.
Gracz, który może jedynie popychać te skrzynie musi je umieścić w odpowiednich
miejscach. Przykładową implementację gry. można zobaczyć pod niniejszym
adresem: http://www.pimpernel.com/sokoban . Sposób przeniesienia w 3 wymiar
pozostawiamy studentowi.

Koncepcja:
- plansze ładowane z pliku (format do uzgodnienia),
- kamera skierowana na środek planszy, obracanie kamery myszką,
- przybliżanie/oddalanie na scrollu myszki,
- plansze złożone z obiektów nieprzesuwalnych (mur) i przesuwalnych (skrzynie),
- gracz steruje kulą czy kit tam czym która pcha skrzynie.

TODO:
- menu (GUI/CLI)
- modele (z neta, blender)
- ładowanie modeli (format OBJ)
- plansze (wymyślić format)
- ładowanie planszy
- sterowanie kulą
- sterowanie kamerą
- ...and many more

Biblioteki:
- OpenGL 2.1
- Freeglut3 (albo GLFW)
- GLM
- SOIL
- kazmath (https://github.com/Kazade/kazmath)
- assimp

