#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "coord.h"

typedef struct player {

  coordinates coords;

} player;

player *player_create(level *lv);

char can_move(player *p, level *lv, int dir);


enum {
  FORWARD, BACKWARD, LEFT, RIGHT
};

#endif
