#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

typedef struct level {

  int **map;
  int width;
  int height;

} level;

typedef struct game {

  int score;
  int state;
  level current_level;


} game;

level *level_create(const char *filename);

#endif
