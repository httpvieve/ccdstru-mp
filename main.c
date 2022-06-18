/**
 * Programmed by:     Genevieve S. Balestramon
 * Last modified:     Jun 15, 2022
 * Version:           1.0
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define TRUE 1
#define FALSE 0

#define COL 5
#define ROW 7

#define FREE ' '
#define BETA_PIECE 'o'
#define ALPHA_PIECE 'x'

#define RED "\033[1;31m"
#define PURPLE "\033[0;34m"
#define GREEN "\033[0;32m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

#define BETA_TURN printf ("%s%s%s\n", PURPLE, "[Beta's turn]", RESET);
#define BETA_WIN printf ("%s%s%s\n", PURPLE, "\tPlayer Beta wins the game!\n", RESET);
#define ALPHA_TURN printf ("%s%s%s\n", CYAN, "[Alpha's turn]", RESET);
#define ALPHA_WIN printf ("%s%s%s\n", CYAN, "\tPlayer Alpha wins the game!\n", RESET);
#define GAME_OVER printf ("\t\tGAME OVER");
  
#define LABEL_TOP printf("\n     1   2   3   4   5\n");
#define TOP printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 187);
#define TILE(label, p) printf (" %d %c %c %c %c %c %c %c %c %c %c %c\n", label, 186, p[1], 186, p[2], 186, p[3], 186, p[4], 186, p[5], 186);
#define PARTITION printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 204, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 185);
#define BOTTOM printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 200, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 188);

typedef char Board[ROW + 1][COL + 1];

typedef struct {
        int x;
        int y;
} Coordinate;

typedef struct {        
        int count;
        Coordinate coordinate[ROW * COL];
} Set;

typedef struct {
        Set alpha, beta, free, valid, S, Y, E; 
        Coordinate prev, next;
        Board board;
        int over, ok, aTurn;
} Game;

enum WinningState {
        ALPHA_WINS_PAWNS_OUT = 5, ALPHA_WINS_ON_BASE = 7,
        BETA_WINS_PAWNS_OUT = 6, BETA_WINS_ON_BASE = 8,
        STALEMATE = 10,
};

Coordinate GetMove (Set avail);
Coordinate Left (int aTurn, Coordinate prev);
Coordinate Front (int aTurn, Coordinate prev);
Coordinate Right (int aTurn, Coordinate prev);

Set AvailableMoves (Set current, Game *game);
Set ModifyValid (Game *game, int aTurn, Coordinate prev);

int Contains (Coordinate *tile, Set current);
int GameOver (Game *game);

void Divider();
void NextPlayerMove (Coordinate prev, Game *game);
void ModifyBoard (Game *game);
void DisplayBoard (Game *game);
void InitializeBoard (Game *game);
void DeclareWinner(int over, Game *game);
void Add (Coordinate tile, Set *current);
void Remove (Coordinate tile, Set *current);
void Move (Coordinate tile, Set *current, Set *destination);
void EliminationProcess (Game *game, Coordinate prev, Coordinate next, int aTurn);

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

Coordinate Front (int aTurn, Coordinate prev)
{
        Coordinate next;
        next.y = prev.y; //y coordinate is the same since piece only moves forward
        
        if (aTurn == TRUE) next.x = prev.x - 1; 
        else next.x = prev.x + 1; 
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

Coordinate GetMove (Set avail)
{
        Coordinate tile; // temporary holder which will be returned by GetMove()
        int i;

        for (i = 0; i < avail.count; i++) printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y); //prints all the available coordinates modified by ModifyValid()
        
	printf ("Enter choice: ");
        scanf ("%d", &i); //asks user to prompt for input 
        
	while (i < 1 || i > avail.count)
        {
                printf ("%sInvalid input.%s", RED, RESET);
                printf ("\nPlease enter a valid choice: \n");
                for (i = 0; i < avail.count; i++) printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
                scanf ("%d", &i);
        } // loops until prompted choice contains a valid move 
        tile = avail.coordinate[i - 1]; // decrements since array indices usually starts at zero
        printf ("You have selected (%d, %d).\n", tile.x, tile.y); //displays selected move
        
        return tile;
}

Set AvailableMoves (Set current, Game *game) //alternatively used to store coordinates of present pieces
{
        Set temp, holder; 
        int i;
        holder.count = 0;
        for (i = 0; i < current.count; i++) //loops for all pieces
        {
                temp = ModifyValid (game, game->aTurn, current.coordinate[i]); // holds a structure which contains moves which varies from left, right, front that are valid
                if (temp.count > 0) holder.coordinate[holder.count++] = current.coordinate[i]; // if a piece has a count > 0, then its coordinates will be stored in holder 
        }
        return holder; // returns a structure which holds all the total number and coordinates pieces with valid moves
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
		if (Contains (&all.coordinate[i], game->free) == TRUE) temp.coordinate[temp.count++] = all.coordinate[i]; 
		switch (aTurn)
		{
			case TRUE:
				if (Contains (&all.coordinate[i], game->beta) == TRUE 
                                    && Contains (&all.coordinate[i], game->S) == TRUE)
					temp.coordinate[temp.count++] = all.coordinate[i]; 
			break;
			case FALSE:
				if (Contains (&all.coordinate[i], game->alpha) == TRUE 
                                     && Contains (&all.coordinate[i], game->S) == TRUE)
					temp.coordinate[temp.count++] = all.coordinate[i]; 
			break;
		}
	}
	return temp;
}

int GameOver (Game *game) //return game state
{
        Set temp_alpha, temp_beta;
        int in_setY =  0, in_setE = 0, i;

        temp_alpha = AvailableMoves (game->alpha, game);
        temp_beta = AvailableMoves (game->beta, game);

        for (i = 0; i < game->alpha.count; i++) if (Contains (&game->alpha.coordinate[i], game->E) == TRUE) in_setE++;
        for (i = 0; i < game->beta.count; i++) if (Contains (&game->beta.coordinate[i], game->Y) == TRUE) in_setY++;

        if (game->alpha.count - in_setE == 0) return ALPHA_WINS_ON_BASE;
        else if (game->beta.count - in_setY == 0) return BETA_WINS_ON_BASE;
        else if ((temp_alpha.count == 0 && game->beta.count > in_setY ) 
              || (temp_beta.count == 0  && game->alpha.count > in_setE )) return STALEMATE;
        else if (game->alpha.count > 0 && game->beta.count == 0) return ALPHA_WINS_PAWNS_OUT;
        else if (game->beta.count > 0 && game->alpha.count == 0) return BETA_WINS_PAWNS_OUT;
        else return FALSE;
}

int Contains (Coordinate *tile, Set current)
{
        int i;
        for (i = 0; i < current.count; i++)
                if (tile->x == current.coordinate[i].x 
                 && tile->y == current.coordinate[i].y)
                        return TRUE;  
        return FALSE;
}

void Divider()
{
	int i;
	printf ("%s", RED);
	for (i = 0; i < 42; i++) if (i == 41) printf("\n");
                                 else printf("=");
        GAME_OVER;
        for (i = 0; i < 42; i++) if (i == 41) printf("\n%s", RESET);
                                 else printf("=");
} 

void NextPlayerMove (Coordinate prev, Game *game)
{
        DisplayBoard (game);
        if (game->aTurn)
        {
                ALPHA_TURN;
                game->valid = AvailableMoves (game->alpha, game);
        } else {
                BETA_TURN;
                game->valid = AvailableMoves (game->beta, game);
        }

        if (game->valid.count == 0)
                game->ok = !game->ok;
        else {
                printf (" Please select a piece to move: \n");
                prev = GetMove (game->valid);
                printf (" Available moves:\n");
                game->valid = ModifyValid (game, game->aTurn, prev);
                game->next = GetMove (game->valid);
                EliminationProcess (game, prev, game->next, game->aTurn);
                DisplayBoard(game);
                printf ("%s \nSuccessfully moved to (%d, %d)! \n%s", RED, game->next.x, game->next.y, RESET);
                printf ("Press any key to continue...");
                getch();
                
                game->aTurn = !game->aTurn;
                game->ok = !game->ok;
        }
}

void ModifyBoard (Game *game)
{
        int i, j;
        Coordinate temp;
        for (i = 1; i <= 7; i++)
		{
                for (j = 1; j <= 5; j++)
				{
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

void DisplayBoard (Game *game)
{
        int i, j;
        system("cls");
	LABEL_TOP;
	TOP;
        if (game->aTurn == TRUE) {
                for (i = 1; i <= ROW ; i++) {
                TILE(i, game->board[i]);
                if (i < ROW) PARTITION;
                }
        } else {
                for (j = ROW; j >= 1; j--) {
                TILE(j, game->board[j]);
                if (j > 1) PARTITION;
                }
        } BOTTOM;
}

void InitializeBoard (Game *game)
{
        Coordinate position;
        game->alpha.count = 0;
        game->beta.count = 0;
        game->free.count = 0;
        game->S.count = 0;
        game->E.count = 0;
        game->Y.count = 0;
        
        for (position.x = 1; position.x <= 7; position.x++)
        {
                for (position.y = 1; position.y <= 5; position.y++)
                {
                        game->board[position.x][position.y] = FREE;
                        if (position.x % 2 == position.y % 2)
                        {       
                                game->S.coordinate[game->S.count++] = position;
                                if (position.x <= 2)
                                {
                                        game->E.coordinate[game->E.count++] = position;
                                        game->beta.coordinate[game->beta.count++] = position;
                                        game->board[position.x][position.y] = BETA_PIECE;
                                }
                                else if (position.x >= 6)
                                {       
                                        game->Y.coordinate[game->Y.count++] = position;
                                        game->alpha.coordinate[game->alpha.count++] = position;
                                        game->board[position.x][position.y] = ALPHA_PIECE;
                                }
                                else {
                                        game->free.coordinate[game->free.count++] = position;
                                        game->board[position.x][position.y] = FREE;
                                }
                        } else {
                                game->free.coordinate[game->free.count++] = position;
                                game->board[position.x][position.y] = FREE;
                        }
                }
        }
}

void DeclareWinner(int over, Game *game)
{
        switch (over)
        {
        case ALPHA_WINS_ON_BASE:
                printf ("Player Beta has %d more remaining piece(s) but has no moves left!\n\n", game->beta.count);
                Divider ();
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case ALPHA_WINS_PAWNS_OUT:
                puts ("\nPlayer Beta has no piece left!\n");
                Divider ();
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case BETA_WINS_ON_BASE:
                printf ("\nPlayer Alpha has %d more remaining piece(s) but has no moves left!\n", game->alpha.count);
                Divider ();
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case BETA_WINS_PAWNS_OUT:
                puts ("\nPlayer Alpha has no piece left!");
                Divider ();
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case STALEMATE:
                 puts ("\nBoth players have no moves left!\n");
                 Divider ();
                 printf ("%s\t\tSTALEMATE!\n\n%s", GREEN, RESET);
        break;
        }
}

void Add (Coordinate tile, Set *current)
{
        current->coordinate[current->count].x = tile.x;
        current->coordinate[current->count].y = tile.y;
        current->count++; // current->coordinate[current->count++] = tile;
}

void Remove (Coordinate tile, Set *current)
{
        int i, key;

        for (i = 0; i < current->count; i++)
        {
                 if (tile.x == current->coordinate[i].x && tile.y == current->coordinate[i].y) key = i;
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

void Move (Coordinate tile, Set *current, Set *destination) // to do : add modification to board or just transfer stuff idk
{
        Remove (tile, current);
        Add (tile, destination);
}

void EliminationProcess (Game *game, Coordinate prev, Coordinate next, int aTurn)
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

int main ()
{
        Game engine, *game;
        game = &engine;
        game->over = FALSE;
        game->ok = FALSE;
        game->aTurn = TRUE;

        InitializeBoard (game);

        while (game->over == FALSE)
        {
                if (game->over == FALSE) game->ok = TRUE;
                while (game->ok)
                        NextPlayerMove (game->prev, game);   
                game->over = GameOver (game);
        }
        DeclareWinner (game->over, game);
        return 0;
}
