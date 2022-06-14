#include "game.h"
#include "display.h"

int Contains (Coordinate *tile, Set current)
{
        int i;
        for (i = 0; i < current.count; i++)
                if (tile->x == current.coordinate[i].x && tile->y == current.coordinate[i].y)
                        return TRUE;
        return FALSE;
}

int IsValid (int aTurn, Coordinate *prev, Coordinate *next, Game *game) // returns val for ok
{
        if (aTurn == TRUE) //alpha's turn 
        {
                if (Contains (prev, game->alpha) == TRUE && prev->x == next->x + 1 && 
                                                           (next->y == prev->y || 
                                                            next->y == prev->y + 1 || 
                                                            prev->y == next->y + 1)) 
                                                            
                return FALSE;
        }
        else 
        {
                if (Contains (prev, game->beta) == TRUE && next->x == prev->x + 1 && 
                                                           (next->y == prev->y || 
                                                            next->y == prev->y + 1 || 
                                                            prev->y == next->y + 1)) 
                return FALSE;
        }

}
Coordinate Front (int aTurn, Coordinate prev)
{
        Coordinate next;
        next.y = prev.y;
        if (aTurn == TRUE) next.x = prev.x + 1;
        else next.x = prev.x - 1;
        return next;
}
Coordinate Left (int aTurn, Coordinate prev)
{
        Coordinate next;
        if (aTurn == TRUE)
        {
                next.x = prev.x - 1;
                next.y = prev.y - 1;
        } else {
                next.x = prev.x + 1;
                next.y = prev.y + 1;
        }
        return next;
}

Coordinate Right (int aTurn, Coordinate prev)
{
        Coordinate next;
        if (aTurn == TRUE)
        {
                next.x = prev.x - 1;
                next.y = prev.y + 1;
        } else {
                next.x = prev.x + 1;
                next.y = prev.y - 1;
        }
        return next;
}

void ModifyValidMoves (int aTurn, Coordinate *prev, Game *game)
{
        Coordinate temp;
        game->valid.count = 0;
        for (temp.x = 1; temp.x <= ROW; temp.x++)
        {
                for (temp.y = 1; temp.y <= COL; temp.y++)
                {

                        if ( IsValid (aTurn, prev, &temp, game) == FALSE && ((Contains (&temp, game->free) == TRUE) ||
                                ((aTurn == TRUE && Contains (&temp, game->beta) == TRUE && Contains (&temp, game->S))||
                                 (aTurn == FALSE && Contains (&temp, game->alpha) == TRUE && Contains (&temp, game->S)))))
                                 {
                                game->valid.coordinate[game->valid.count].x = temp.x;
                                game->valid.coordinate[game->valid.count].y = temp.y;
                                game->valid.count++;

                                 }
                        }
                }

 }


void printValid (Game *game)
{
        for (int i = 0; i < game->valid.count; i++)
                 printf ("[%d] (%d, %d)\n", i + 1, game->valid.coordinate[i].x, game->valid.coordinate[i].y);
        
}

void displayBoard (Game *all_set)
{
        int i;

	LABEL_TOP;
	TOP;
        for (i = 1; i <= ROW ; i++)
        {
                TILE(i, all_set->board[i]);
                if (i < ROW) {
                        PARTITION;
                } else BOTTOM;
        }
}
void initializeBoard (Game *game)
{
        Coordinate position;
        game->alpha.count = 0;
        game->beta.count = 0;
        game->free.count = 0;
        game->S.count = 0;
        for (position.x = 1; position.x <= 7; position.x++)
        {
                for (position.y = 1; position.y <= 5; position.y++)
                {
                        game->board[position.x][position.y] = FREE;
                        if (position.x % 2 == position.y % 2)
                        {       
                                game->S.coordinate[game->S.count].x = position.x;
                                game->S.coordinate[game->S.count].y = position.y;
                                game->S.count++;
                                if (position.x <= 2)
                                {
                                        game->beta.coordinate[game->beta.count].x = position.x;
                                        game->beta.coordinate[game->beta.count].y = position.y;
                                        game->board[game->beta.coordinate[game->beta.count].x][game->beta.coordinate[game->beta.count].y] = BETA_PIECE;
                                        game->beta.count++;
                                }
                                else if (position.x >= 6)
                                {       
                                        game->alpha.coordinate[game->alpha.count].x = position.x;
                                        game->alpha.coordinate[game->alpha.count].y = position.y;
                                        game->board[game->alpha.coordinate[game->alpha.count].x][game->alpha.coordinate[game->alpha.count].y] = ALPHA_PIECE;
                                        game->alpha.count++;
                                }
                                else {
                                        game->free.coordinate[game->free.count].x = position.x;
                                        game->free.coordinate[game->free.count].y = position.y;
                                        game->board[game->free.coordinate[game->free.count].x][game->free.coordinate[game->free.count].y] = FREE;
                                        game->free.count++;
                                }
                        } else {
                                game->free.coordinate[game->free.count].x = position.x;
                                        game->free.coordinate[game->free.count].y = position.y;
                                        game->board[game->free.coordinate[game->free.count].x][game->free.coordinate[game->free.count].y] = FREE;
                                        game->free.count++;
                        }
                }
        }
}



void Add (Coordinate tile, Set *current)
{
        current->coordinate[current->count].x = tile.x;
        current->coordinate[current->count].y = tile.y;
        current->count++;
}

void Remove (Coordinate tile, Set *current)
{
        int i, key;

        for (i = 0; i < current->count; i++)
        {
                 if (tile.x == current->coordinate[i].x && tile.y == current->coordinate[i].y)
                        key = i;
        }
        while (key < current->count)
        {
                current->coordinate[key].x = current->coordinate[key + 1].x;
                current->coordinate[key].y = current->coordinate[key + 1].y;
                key++;
        }
        current->coordinate[key].x = '\0';
        current->coordinate[key].y = '\0';
        current->count--;
}

void Move (Coordinate tile, Set *current, Set *destination)
{
        Remove (tile, current);
        Add (tile, destination);
}

void ModifyBoard (Game *game)
{
        int i, j;
        Coordinate temp;
        for (i = 1; i <= 7; i++){
                for (j = 1; j <= 5; j++){
                        temp.x = i;
                        temp.y = j;
                        if (Contains (&temp, game->beta) == TRUE)
                                game->board[temp.x][temp.y] = BETA_PIECE;
                        else if (Contains (&temp, game->alpha) == TRUE)
                                game->board[temp.x][temp.y] = ALPHA_PIECE;
                        else game->board[temp.x][temp.y] = FREE;
                }
        }
}




// void DisplayValidMoves (Coordinate *current)
// {
//         Coordinate temp, key;

//         for (temp.x = 1; temp.x <= ROW; temp.x)
//         {
//                 for (temp.y = 1; temp.y <= ROW; temp.y)
//                 {

//                         if (isValid (current, temp,  ))
//                 }
//         }
// }
void NextPlayerMove (Coordinate prev, Coordinate next, Game *game)
{
        // game->ok == isValid (prev, next, game);
        // if (game->ok == TRUE)
        // {
        //         Remove (prev, &game->alpha, &game->free, game->board);
        //         Add (next, &game->alpha);
        //         ModifyBoard (game);
        //         game->aTurn = FALSE;
        // }
        // else
        // {
        //         printf ("Invalid move\n");
        // }
        
}
int GameOver (Game *game) //return game state
{
        if (game->alpha.count == 0 || game->beta.count == 0) {
                if (game->alpha.count > 0 && game->beta.count == 0) return ALPHA;
                else return BETA;
        } else return FALSE;
}
