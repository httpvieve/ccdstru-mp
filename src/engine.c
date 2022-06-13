#include "game.h"
#include "display.h"

void displayBoard (Game *all_set)
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
void initializeBoard (Game *game)
{
        Coordinate position;
        game->alpha.count = 0;
        game->beta.count = 0;
        game->free.count = 0;
        for (position.x = 1; position.x <= 7; position.x++)
        {
                for (position.y = 1; position.y <= 5; position.y++)
                {
                        game->board[position.y][position.x] = FREE;
                        if (position.x % 2 == position.y % 2)
                        {       
                                if (position.x <= 2)
                                {
                                        game->beta.coordinate[game->beta.count].x = position.x;
                                        game->beta.coordinate[game->beta.count].y = position.y;
                                        game->board[game->beta.coordinate[game->beta.count].y][game->beta.coordinate[game->beta.count].x] = BETA_PIECE;
                                        game->beta.count++;
                                }
                                else if (position.x >= 6)
                                {       
                                        game->alpha.coordinate[game->alpha.count].x = position.x;
                                        game->alpha.coordinate[game->alpha.count].y = position.y;
                                        game->board[game->alpha.coordinate[game->alpha.count].y][game->alpha.coordinate[game->alpha.count].x] = ALPHA_PIECE;
                                        game->alpha.count++;
                                }
                                else {
                                        game->free.coordinate[game->free.count].x = position.x;
                                        game->free.coordinate[game->free.count].y = position.y;
                                        game->board[game->free.coordinate[game->free.count].y][game->free.coordinate[game->free.count].x] = FREE;
                                        game->free.count++;
                                }
                        }
                }
        }
}

void Add (Coordinate tile, Set *current)
{
        //board[tile.x][tile.y] = piece;
        current->coordinate[current->count].x = tile.x;
        current->coordinate[current->count].y = tile.y;
        current->count++;
}

void Remove (Coordinate tile, Set *current, Set *free, Board board)
{
        int i, key;
        for (i = 0; i < current->count; i++)
        {
                 if (tile.x == current->coordinate[i].x && tile.y == current->coordinate[i].y)
                        key = i;
        }
        Add (tile, free);
        board[current->coordinate[key].y][current->coordinate[key].x] = FREE;
        current->count--;
        while (key <= current->count)
        {
                current->coordinate[key].x = current->coordinate[key + 1].x;
                current->coordinate[key].y = current->coordinate[key + 1].y;
                key++;
        }
        current->coordinate[key].x = '\0';
        current->coordinate[key].y = '\0';
}
int Contains (Coordinate *tile, Set current)
{
        int i;
        for (i = 0; i < current.count; i++)
                if (tile->x == current.coordinate[i].x && tile->y == current.coordinate[i].y)
                        return TRUE;
        return FALSE;
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
                                game->board[temp.y][temp.x] = BETA_PIECE;
                        else if (Contains (&temp, game->alpha) == TRUE)
                                game->board[temp.y][temp.x] = ALPHA_PIECE;
                        else game->board[temp.y][temp.x] = FREE;
                }
        }
}


int IsValid (Coordinate *prev, Coordinate *next, Game *game) // returns val for ok
{
        if (game->aTurn ==TRUE)
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

void DisplayValidMoves (Coordinate *current)
{
        Coordinate temp, key;

        for (temp.x = 1; temp.x <= ROW; temp.x)
        {
                for (temp.y = 1; temp.y <= ROW; temp.y)
                {

                        if (isValid (current, temp,  ))
                }
        }
}
void NextPlayerMove (Coordinate prev, Coordinate next, Game *game)
{
        game->ok == isValid (prev, next, game);
        if (game->ok == TRUE)
        {
                Remove (prev, &game->alpha, &game->free, game->board);
                Add (next, &game->alpha);
                ModifyBoard (game);
                game->aTurn = FALSE;
        }
        else
        {
                printf ("Invalid move\n");
        }
        
}
int GameOver (Game *game) //return game state
{
        if (game->alpha.count == 0 || game->beta.count == 0) {
                if (game->alpha.count > 0 && game->beta.count == 0) return ALPHA;
                else return BETA;
        } else return FALSE;
}
