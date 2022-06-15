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

Coordinate Front (int aTurn, Coordinate prev)
{
        Coordinate next;
        next.y = prev.y;
        if (aTurn == TRUE) next.x = prev.x - 1;
        else next.x = prev.x + 1;
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

void printValid (Game *game)
{
        for (int i = 0; i < game->valid.count; i++)
                 printf ("[%d] (%d, %d)\n", i + 1, game->valid.coordinate[i].x, game->valid.coordinate[i].y);
        
}

void displayBoard (Game *game)
{
        int i, j;
        system("cls");
	LABEL_TOP;
	TOP;
        if (game->aTurn == TRUE)
                        {
                                for (i = 1; i <= ROW ; i++)
                                {

                                TILE(i, game->board[i]);
                                if (i < ROW) 
                                PARTITION;
                                }
         } else {
                                for (j = ROW; j >= 1; j--)
                                {

                                TILE(j, game->board[j]);
                                if (j > 1) 
                                PARTITION;
                                }
                        }
        BOTTOM;
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
       // game->ok = !game->ok;
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

void Move (Coordinate tile, Set *current, Set *destination) //add modification to board or just transfer stuff
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

Set ModifyValid (Game *game, int aTurn, Coordinate prev)
{
	Set temp, all;
	int i;
		temp.count = 0;
		all.coordinate[0] = Left (aTurn, prev);
		all.coordinate[1] = Front (aTurn, prev);
		all.coordinate[2] = Right (aTurn, prev);
	
	for (i = 0; i < 3; i++)
	{
		if (Contains (&all.coordinate[i], game->free) == TRUE)
			temp.coordinate[temp.count++] = all.coordinate[i]; 
		switch (aTurn)
		{
			case TRUE:
				if (Contains (&all.coordinate[i], game->beta) == TRUE && Contains (&all.coordinate[i], game->S))
					temp.coordinate[temp.count++] = all.coordinate[i]; 
			break;
			case FALSE:
				if (Contains (&all.coordinate[i], game->alpha) == TRUE && Contains (&all.coordinate[i], game->S))
					temp.coordinate[temp.count++] = all.coordinate[i]; 
			break;
		}
	}
        //for (i = 0; i < temp.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, temp.coordinate[i].x, temp.coordinate[i].y);
    
	return temp;
}

void ElimProcess (Game *game, Coordinate prev, Coordinate next, int aTurn)
{
        switch (aTurn)
	{
	case TRUE:
                if (Contains (&next, game->free) == TRUE)
                        {
                                
                        Move (prev, &game->alpha, &game->free);
                        Move (next, &game->free, &game->alpha);
                 }
		if (Contains (&next, game->beta) == TRUE && Contains (&prev, game->S))
                        {
                        Move (next, &game->beta, &game->free);
                        Move (prev, &game->alpha, &game->free);
                        Move (next, &game->free, &game->alpha);
                        //eat remarks
                } 
	break;
	case FALSE:

                if (Contains (&next, game->free) == TRUE)
                {
                        Move (prev, &game->beta, &game->free);
                        Move (next, &game->free, &game->beta);
                }
                if (Contains (&next, game->alpha) == TRUE && Contains (&prev, game->S))
                        {
                        Move (next, &game->alpha,&game->free);
                        Move (prev, &game->beta, &game->free);
                        Move (next, &game->free, &game->beta);
                }
	break;
	}
        ModifyBoard (game);
        aTurn = !aTurn;
}

