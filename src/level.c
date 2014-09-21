#include <stdio.h>
#include <stdlib.h>

#include "level.h"


level *level_create(const char *filename) {

  level *lv = (level *) malloc(sizeof(level));
  FILE *map_file = fopen(filename, "rt");
  char line[256];
  char line_num = 1;
  int i, j;

  if(fgets(line, 256, map_file) != NULL) {
    if(sscanf(line, "%d %d", &lv->size.x, &lv->size.y) != 2) {
      fprintf(stderr, "Error in %s level file on line %d\n",
          filename, line_num);
    }
  }

  lv->map = (int **) malloc(lv->size.y * sizeof(int *));

  for(i = 0; i < lv->size.y; i++) {

    lv->map[i] = (int *) malloc(lv->size.x * sizeof(int));

    for(j = 0; j < lv->size.x; j++) {

      line_num++;

      if(fscanf(map_file, "%d", &lv->map[i][j]) != 1)
        fprintf(stderr, "Error in %s level file on line %d\n",
            filename, line_num);

    }
  }

  return lv;

}
