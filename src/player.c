#include <stdlib.h>
#include <stdio.h>

#include "player.h"

player *player_create(level *lv)
{

  player *p = (player *) malloc(sizeof(player));
  int i, j;

  for(i = 0; i < lv->size.y; i++)
    for(j = 0; j < lv->size.x; j++)
      if(lv->map[i][j] == 2) {
        p->coords.x = i;
        p->coords.y = j;
      }

  return p;

}

char can_move(player *p, level *lv, int dir)
{
  printf("========== checking move...\n");

  int ret = 1, i, j;

  for(i = 0; i < lv->size.y; i++) {
    for(j = 0; j < lv->size.x; j++)
      printf("%d%d %d\t", i, j, lv->map[i][j]);
    printf("\n");
  }

  switch(dir) {

    case FORWARD:

      printf("Player is at %d %d\n", p->coords.y, p->coords.x);
      printf("Player wants %d %d\n", p->coords.y + 1, p->coords.x);

      if(p->coords.y + 1 > lv->size.y) {

        ret = -1;
        printf("Move outside of map\n");

      } else if(lv->map[p->coords.y + 1][p->coords.x] == 1) {

        printf("It's impossible\n");
        ret = 0;

      } else

        printf("It's possible");

      break;


    case BACKWARD:

      printf("Player is at %d %d\n", p->coords.y, p->coords.x);
      printf("Player wants %d %d\n", p->coords.y - 1, p->coords.x);

      if(p->coords.y - 1 < 0) {

        ret = -1;
        printf("Move outside of map\n");

      } else if(lv->map[p->coords.y - 1][p->coords.x] == 1) {

        printf("It's impossible\n");
        ret = 0;

      } else

        printf("It's possible\n");

      break;


    case RIGHT:

      printf("Player is at %d %d\n", p->coords.y, p->coords.x);
      printf("Player wants %d %d\n", p->coords.y, p->coords.x - 1);

      if(p->coords.x - 1 < 0) {

        ret = -1;
        printf("Move outside of map\n");

      } else if(lv->map[p->coords.y][p->coords.x - 1] == 1) {

        ret = 0;

      } else

        printf("It's possible\n");

      break;


    case LEFT:

      printf("Player is at %d %d\n", p->coords.y, p->coords.x);
      printf("Player wants %d %d\n", p->coords.y, p->coords.x + 1);

      if(p->coords.x + 1 > lv->size.x) {

        ret = -1;
        printf("Move outside of map\n");

      } else if(lv->map[p->coords.y][p->coords.x + 1] == 1) {

        printf("It's impossible\n");
        ret = 0;

      } else

        printf("It's possible\n");

      break;


    default:

      printf("Wrong direction\n");

      break;

  }

  return ret;

}


