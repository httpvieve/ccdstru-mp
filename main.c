/**
 * Description:       This is a terminal-based board game oriented for two players which is similar to the game Checkers but a modified version.
 * 
 * Programmed by:     Genevieve S. Balestramon
 *                    
 * Last modified:     Jun 15, 2022
 * Version:           1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define TRUE 1
#define FALSE 0

#define COL 5
#define ROW 7
#define MAX_PAIR ROW * COL

#define FREE ' '
#define BETA_PIECE 'o'
#define ALPHA_PIECE 'x'

#define ALPHA_WINS_PAWNS_OUT 5
#define BETA_WINS_PAWNS_OUT 6
#define ALPHA_WINS_MOVES_OUT 7
#define BETA_WINS_MOVES_OUT 8
#define STALEMATE 10

#define RED printf("\033[1;31m");
#define PURPLE printf("\033[0;34m");
#define GREEN printf("\033[0;32m");
#define CYAN printf("\033[0;36m");
#define RESET printf("\033[0m");

#define ALPHA_TURN CYAN; \
                  printf ("[Alpha's turn]\n");\
                  RESET;\


#define BETA_TURN PURPLE; \
                  printf ("[Beta's turn]\n");\
                  RESET;\
                  


#define ALPHA_WIN CYAN; \
                  printf ("\tPlayer Alpha wins the game!\n");\
                  RESET;\


#define BETA_WIN PURPLE; \
                  printf ("\tPlayer Beta wins the game!\n");\
                   RESET;\


#define GAME_OVER  RED;\
                   puts ("\t\tGAME OVER");\
                   RESET;\

#define VALID (Set set) for (int i = 0; i < set.count; i++) printf ("\t[%d] (%d, %d)\n", i + 1, set.coordinate[i].x, set.coordinate[i].y); 

    
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
        Coordinate coordinate[MAX_PAIR];
} Set;

typedef struct {
        Set alpha, beta, free, valid, S; 
        Coordinate prev, next;
        Board board;
        int over, ok, aTurn;
} Game;

Coordinate Left (int aTurn, Coordinate prev);
Coordinate Front (int aTurn, Coordinate prev);
Coordinate Right (int aTurn, Coordinate prev);
Coordinate GetMove (Set avail);

Set AvailableMoves (Set current, Game *game);
Set ModifyValid (Game *game, int aTurn, Coordinate prev);

int Contains (Coordinate *tile, Set current);
int GameOver (Game *game);

void Divider();
void NextPlayerMove (Game *game);
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
        next.y = prev.y;
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
        Coordinate tile;
        int i;

        for (i = 0; i < avail.count; i++) 
        {
        printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
        }
        printf ("Enter choice: ");
        scanf ("%d", &i);
        while (i < 1 || i > avail.count)
        {
                RED;
                puts ("Invalid input.");
                RESET;
                printf ("Please enter a valid choice: \n");
                for (i = 0; i < avail.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
                scanf ("%d", &i);
        }
        tile = avail.coordinate[i - 1];
        printf ("You have selected (%d, %d).\n", tile.x, tile.y);
        
        return tile;
}

Set AvailableMoves (Set current, Game *game)
{
        Set temp, holder; 
        int i;
        holder.count = 0;
        for (i = 0; i < current.count; i++)
        {
                temp = ModifyValid (game, game->aTurn, current.coordinate[i]);
                if (temp.count > 0)
                        holder.coordinate[holder.count++] = current.coordinate[i]; 
        }
        return holder;
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
        temp_alpha = AvailableMoves (game->alpha, game);
        temp_beta = AvailableMoves (game->beta, game);
        if (temp_alpha.count == 0 && temp_beta.count == 0) return STALEMATE;
        if (game->valid.count == 0)
        {
                if (game->aTurn == TRUE) return BETA_WINS_MOVES_OUT;
                else return ALPHA_WINS_MOVES_OUT;
        } else if (game->alpha.count == 0 || game->beta.count == 0) {
                if (game->alpha.count > 0 && game->beta.count == 0) return ALPHA_WINS_PAWNS_OUT;
                else return BETA_WINS_PAWNS_OUT;
        } else return FALSE;
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
	RED;
	for (i = 0; i < 42; i++) 
        if (i == 41) printf("\n");
        else printf("=");
	RESET;
	
} 

void NextPlayerMove (Game *game)
{
        Set temp;
        int key = 0;
        char c;
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
                game->prev = GetMove (game->valid);
                printf (" Available moves:\n");
                game->valid = ModifyValid (game, game->aTurn, game->prev);
                game->next = GetMove (game->valid);
                EliminationProcess (game, game->prev, game->next, game->aTurn);
                DisplayBoard(game);
                RED;
                printf ("\nSuccessfully moved to (%d, %d)!\n", game->next.x, game->next.y);
                RESET;
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

void DisplayBoard (Game *game)
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
                if (i < ROW) PARTITION;
                }
         } else {
                for (j = ROW; j >= 1; j--)
                {
                TILE(j, game->board[j]);
                if (j > 1) PARTITION;
                 }
        }
        BOTTOM;
}

void InitializeBoard (Game *game)
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

void DeclareWinner(int over, Game *game)
{
        switch (over)
        {
        case ALPHA_WINS_MOVES_OUT:
                printf ("Player Beta has %d more remaining piece(s) but has no moves left!\n\n", game->beta.count);
                Divider ();
                GAME_OVER;
                Divider ();
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case ALPHA_WINS_PAWNS_OUT:
                puts ("\nPlayer Beta has no piece left!\n");
                Divider ();
                GAME_OVER;
                Divider ();
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case BETA_WINS_MOVES_OUT:
                printf ("\nPlayer Alpha has %d more remaining piece(s) but has no moves left!\n", game->alpha.count);
                Divider ();
                GAME_OVER;
                Divider ();
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case BETA_WINS_PAWNS_OUT:
                puts ("\nPlayer Alpha has no piece left!");
                Divider ();
                GAME_OVER;
                Divider ();
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case STALEMATE:
                puts ("\nBoth players have no moves left!\n");
                Divider ();
                GAME_OVER;
                Divider ();
                GREEN;
                puts ("\t\tSTALEMATE!\n\n");
                RESET;
        break;
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

void Move (Coordinate tile, Set *current, Set *destination) //add modification to board or just transfer stuff
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

int main ()
{
        Game main, *game;
        game = &main;
        game->over = FALSE;
        game->ok = FALSE;
        game->aTurn = TRUE;
        
        InitializeBoard (game);

        while (game->over == FALSE)
        {
                if (game->over == FALSE) 
                        game->ok = TRUE;
                //DisplayBoard (game);
                while (game->ok)
                        NextPlayerMove (game);   
                game->over = GameOver (game);
        }

        DeclareWinner (game->over, game);
        return 0;
}

/*************************************************************************************************
    This is to certify that this project is our own work, based on our personal efforts in studying and
    applying the concepts learned. We have constructed the functions and their respective algorithms
    and corresponding code by ourselves. The program was run, tested, and debugged by our own efforts. We
    further certify that we have not copied in part or whole or otherwise plagiarized the work of other
    students and/or persons.
    
    Genevieve S. Balestramon, DLSU ID# 12108254
    
*************************************************************************************************/