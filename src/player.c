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

/* 
 * OBSLUGA RUCHU by PaQu
 */

#define X p->coords.x
#define Y p->coords.y

int move(player *p, 	/* struktura plyera z informacja o obecnej pozycji playera  */
	 level *lv, 	/* informacje o planszy, gdzie co sie znajduje*/
	 int x,int y	/* w ktora strone sie poruszamy */
	) 
{
	int i,j;	
	for(i = 0; i < lv->size.y; i++) {
    		for(j = 0; j < lv->size.x; j++)
     			 printf("%d%d %d\t", i, j, lv->map[i][j]);
    	printf("\n");
  	}


	printf("Moja pozycja teraz %d,%d \t Moja pozycja po ruchu %d,%d \n",
		p->coords.y,p->coords.x,p->coords.y+y,p->coords.x+x);
	if(0==lv->map[Y+y][X+x] || 2==lv->map[Y+y][X+x]){
		lv->map[Y][X]=0;
		p->coords.x+=x;
		p->coords.y+=y;
		lv->map[Y][X]=2;
	}else if( 1==lv->map[Y+y][X+x]){
		printf("Wall \n");
	}else if( 3==lv->map[Y+y][X+x]){
		if(1 == y){
			if(0==lv->map[Y+y+1][X+x]|| 2==lv->map[Y+y+1][X+x]){
				lv->map[Y+y+1][X+x]=3;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else if( 4 == lv->map[Y+y+1][X+x]){
				lv->map[Y+y+1][X]=5;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;			
			}else printf("Nothing to do\n");
		}else if( -1 == y){
			if(0==lv->map[Y+y-1][X+x] || 2==lv->map[Y+y-1][X+x]){
				lv->map[Y+y-1][X+x]=3;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;	
			}else if( 4 == lv->map[Y+y-1][X+x]){
				lv->map[Y+y-1][X+x]=5;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else printf("Nothing to do\n");
		}else if( 1 == x){
			if(0==lv->map[Y+y][X+x+1] || 2==lv->map[Y+y][X+x+1]){
				lv->map[Y+y][X+x+1]=3;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else if( 4 == lv->map[Y+y][X+x+1]){
				lv->map[Y+y][X+x+1]=5;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else printf("Nothing to do\n");
		}else if( -1 == x){
			if(0==lv->map[Y+y][X+x-1] || 2==lv->map[Y+y][X+x-1]){
				lv->map[Y+y][X+x-1]=3;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else if( 4 == lv->map[Y+y][X+x-1]){
				lv->map[Y+y][X+x-1]=5;
				lv->map[Y+y][X+x]=2;
				lv->map[Y][X]=0;
				p->coords.x+=x;
				p->coords.y+=y;
			}else printf("Nothing to do\n");
		}
	}else if( 5 == lv->map[Y+y][X+x]){
		printf("Box on slot \n");
	}

	for(i = 0; i < lv->size.y; i++) {
    		for(j = 0; j < lv->size.x; j++)
     			 printf("%d%d %d\t", i, j, lv->map[i][j]);
    	printf("\n");
  	}

return 1; 
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
/*
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
*/
      break;


    case BACKWARD:
/*
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
*/
      break;


    case RIGHT:
/*
      printf("Player is at %d %d\n", p->coords.y, p->coords.x);
      printf("Player wants %d %d\n", p->coords.y, p->coords.x - 1);

      if(p->coords.x - 1 < 0) {

        ret = -1;
        printf("Move outside of map\n");

      } else if(lv->map[p->coords.y][p->coords.x - 1] == 1) {

        ret = 0;

      } else

        printf("It's possible\n");
*/
      break;


    case LEFT:
/*
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
*/
      break;


    default:

      printf("Wrong direction\n");

      break;

  }

  return ret;

}


