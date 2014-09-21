#ifndef LEVEL_H
#define LEVEL_H

#include "coord.h"

typedef struct level {

  int **map;
  coordinates size;


} level;

level *level_create(const char *filename);

#endif
