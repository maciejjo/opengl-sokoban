#include "game.h"

level *level_create(const char *filename) {

  level *lv = malloc(sizeof(level));
  FILE *map_file = fopen(filename, "rt");
  char line[256];
  char line_num = 1;
  int i, j;

  if(fgets(line, 256, map_file) != NULL) {
    if(sscanf(line, "%d %d", &lv->width, &lv->height) != 2) {
      fprintf(stderr, "Error in %s level file on line %d\n",
          filename, line_num);
    }
  }

  lv->map = malloc(lv->height * sizeof(int *));

  for(i = 0; i < lv->height; i++) {

    lv->map[i] = malloc(lv->width * sizeof(int));

    for(j = 0; j < lv->width; j++) {


      line_num++;

      if(fscanf(map_file, "%d", &lv->map[i][j]) != 1)
      fprintf(stderr, "Error in %s level file on line %d\n",
          filename, line_num);

    }
  }

  return lv;

}
