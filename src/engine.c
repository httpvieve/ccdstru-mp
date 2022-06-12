#include "game.h"
#include "display.h"

void display_board (Data *all_set)
{

	LABEL_TOP;
	TOP;
	TILE(1, all_set->board[1]);
	PARTITION;
	TILE(2, all_set->board[2]);
	PARTITION;
	TILE(3, all_set->board[3]);
	PARTITION;
	TILE(4, all_set->board[4]);
	PARTITION;
	TILE(5, all_set->board[5]);
	BOTTOM;


}
void init_board (Data *all_set)
{
        
        Coordinate position;
        all_set->Y.count = 0;
        all_set->E.count = 0;
        all_set->S.count = 0;
        for (position.x = 1; position.x <= 7; position.x++)
        {
                for (position.y = 1; position.y <= 5; position.y++)
                {
                        all_set->board[position.y][position.x] = ' ';
                        if (position.x % 2 == position.y % 2)
                        {       
                                all_set->S.coordinate[all_set->S.count].x = position.x;
                                all_set->S.coordinate[all_set->S.count].y = position.y;
                                all_set->S.count++;
                                
                                if (position.x <= 2)
                                {
                                        all_set->Y.coordinate[all_set->Y.count].x = position.x;
                                        all_set->Y.coordinate[all_set->Y.count].y = position.y;
                                        all_set->board[position.y][position.x] = 'o';
                                        all_set->Y.count++;
                                }
                                if (position.x >= 6)
                                {       
                                        all_set->E.coordinate[all_set->E.count].x = position.x;
                                        all_set->E.coordinate[all_set->E.count].y = position.y;
                                        all_set->board[position.y][position.x] = 'x';
                                        all_set->E.count++;
                                }
                        }
                }
        }
}


int main ()
{
        Data game, *all_set;
        all_set = &game;
        init_board (all_set);
        for ( int i = 0; i < MAX_PAIR; i++)
        
                 printf ("Tile %d is (%d, %d)\n", i + 1, game.S.coordinate[i].x, game.S.coordinate[i].y);
        
        display_board (all_set);

        return 0;
}
