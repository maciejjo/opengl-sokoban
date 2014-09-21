#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

#include "level.h"

typedef struct game {

  int score;
  int state;
  level current_level;


} game;


#endif
