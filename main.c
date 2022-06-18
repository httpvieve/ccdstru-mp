/**
 * Programmed by:     Genevieve S. Balestramon
 * Last modified:     Jun 18, 2022
 * Version:           1.0
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// macro declaration for boolean values
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
#define GAME_OVER printf ("\t\tGAME OVER\n");
  
#define LABEL_TOP printf("\n     1   2   3   4   5\n");
#define TOP printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 187);
#define TILE(label, p) printf (" %d %c %c %c %c %c %c %c %c %c %c %c\n", label, 186, p[1], 186, p[2], 186, p[3], 186, p[4], 186, p[5], 186);
#define PARTITION printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 204, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 185);
#define BOTTOM printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 200, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 188);

#define IS_VALID (a, b) ((a == b)? TRUE : FALSE)	

typedef char Board[ROW + 1][COL + 1]; //  2D array of characters which stores the pieces of the board

/**
 @brief Structure for a tile coordinate
         @param x x-coordinate
         @param y y-coordinate
 */
typedef struct Coordinate{
        int x;
        int y;
} Coordinate;

/**
  @brief Structure for a set of coordinates
       @param coordinate array of coordinates
       @param count number of coordinates in the set
 */
typedef struct Set {        
        int count;
        Coordinate coordinate[ROW * COL];
} Set;

/**
 @brief  Structure for the game
       @param game The game to initialize.
       @param board The board to use.
       @param alpha The set that holds the coordinates of player alpha's present pieces.
       @param beta The set that holds the coordinates of player beta's present pieces
       @param free The set of all coordinates.
       @param valid The set that stores valid coordinates for every round.
       @param S The set of all coordinates with the same parity
       @param Y The set of S with x coordinates less than 3, a.k.a Beta's base.
       @param E The set of S with x coordinates greater than 5, a.k.a Alpha's base.
       @param prev Stores the previous move.
       @param next Stores the next move.
       @param over Determines whether the game is over.
       @param ok Determines if the move is valid for each round.
       @param aTurn Determines the player's turn. 
 */
typedef struct Game {
        Set alpha, beta, free, valid, S, Y, E; 
        Coordinate prev, next;
        Board board;
        int over, ok, aTurn;
} Game;

enum GameState { 
        ALPHA_WINS_PAWNS_OUT = 5, 
        ALPHA_WINS_ON_BASE = 7,
        BETA_WINS_PAWNS_OUT = 6, 
        BETA_WINS_ON_BASE = 8,
        STALEMATE = 10,
        LEFT = 0, FRONT = 1, RIGHT = 2
};

Coordinate Left (int aTurn, Coordinate prev);
Coordinate Front (int aTurn, Coordinate prev);
Coordinate Right (int aTurn, Coordinate prev);
Coordinate GetPiece (Set avail);
Coordinate GetMove (Set avail, int aTurn, Coordinate prev);

Set AvailablePieces (Set current, Game *game);
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

/**
  @brief This is a structure function that returns the coordinate of the top left tile
         @param prev: contains the x and y coordinate of the current tile
         @param aTurn: TRUE if the player is alpha, otherwise beta
         @return: the coordinate of the (1/3) next possible move (left)
 */
Coordinate Left (int aTurn, Coordinate prev) 
{
        Coordinate next;
        if (aTurn)
        {
                next.x = prev.x - 1;
                next.y = prev.y - 1;
        } else {
                next.x = prev.x + 1;
                next.y = prev.y - 1;
        }
        return next;
}

/**
   @brief This is a structure function that returns the coordinate of the top tile
         @param prev: contains the x and y coordinate of the current tile
         @param aTurn: TRUE if the player is alpha, otherwise beta
         @return: the coordinate of the (1/3) next possible move (forward)
 */
Coordinate Front (int aTurn, Coordinate prev)
{
        Coordinate next;
        next.y = prev.y; //y coordinate stays the same since piece only moves forward
        
        if (aTurn) next.x = prev.x - 1; 
        else next.x = prev.x + 1; 
	return next;
}

/**
   @brief This is a structure function that returns the coordinate of the top right tile
         @param prev: contains the x and y coordinate of the current tile
         @param aTurn: TRUE if the player is alpha, otherwise beta
         @return: the coordinate of the (1/3) next possible move (right)
 */
Coordinate Right (int aTurn, Coordinate prev)
{
        Coordinate next;
        if (aTurn)
        {
                next.x = prev.x - 1;
                next.y = prev.y + 1;
        } else {
                next.x = prev.x + 1;
                next.y = prev.y + 1;
        }
        return next;
}

/**  
  @brief This structure function returns the player's next move
         @param avail modified set of available pieces
         @return the coordinate of choice which will be moved
 */
Coordinate GetPiece (Set avail)
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
        printf ("You have selected %s(%d, %d)%s.\n", GREEN, tile.x, tile.y, RESET); //displays selected move
        return tile;
}

Coordinate GetMove (Set avail, int aTurn, Coordinate prev) // huhu do i even need this
{
        Coordinate tile;
        Coordinate left = Left (aTurn, prev);
        Coordinate front = Front (aTurn, prev);
        Coordinate right = Right (aTurn, prev);
        int i;
        for (i = 0; i < avail.count; i++)
        {
                if (avail.coordinate[i].x == left.x && avail.coordinate[i].y == left.y)
                         printf ("\t[%d] (%d, %d) - %sLeft (<--)%s\n",i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
                if (avail.coordinate[i].x == front.x && avail.coordinate[i].y == front.y)
                         printf ("\t[%d] (%d, %d) - %sForward ( ^ )%s\n",i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
                if (avail.coordinate[i].x == right.x && avail.coordinate[i].y == right.y)
                        printf ("\t[%d] (%d, %d) - %sRight (-->)%s\n",i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
        } 
	printf ("Enter choice: ");
        scanf ("%d", &i); //asks user to prompt for input 
	while (i < 1 || i > avail.count)
        {
                printf ("%sInvalid input.%s", RED, RESET);
                printf ("\nPlease enter a valid choice: \n");
                for (i = 0; i < avail.count; i++)
                        {
                        if (avail.coordinate[i].x == left.x && avail.coordinate[i].y == left.y)
                                printf ("\t[%d] (%d, %d) - %sLeft (<--)%s\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
                        if (avail.coordinate[i].x == front.x && avail.coordinate[i].y == front.y)
                                printf ("\t[%d] (%d, %d) - %sForward ( ^ )%s\n",i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
                        if (avail.coordinate[i].x == right.x && avail.coordinate[i].y == right.y)
                                printf ("\t[%d] (%d, %d) - %sRight (-->)%s\n",i + 1, avail.coordinate[i].x, avail.coordinate[i].y, GREEN, RESET);
                        } 
                scanf ("%d", &i);
        } // loops until prompted choice contains a valid move 
        tile = avail.coordinate[i - 1]; // decrements since array indices usually starts at zero
        printf ("You have selected (%d, %d).\n", tile.x, tile.y); //displays selected move
        return tile;
}

/**  
  @brief This is used to store coordinates of the current player's pieces with available moves
         @param current the set of the current player
         @param game the current game state
         @return the set of coordinates of the valid pieces
 */
Set AvailablePieces (Set current, Game *game) 
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

/**  
  @brief Modifies the valid set to include only the available moves    
         @param current the current set of valid moves
         @param game the current game state
         @return the modified set of valid moves
 */
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
		if (Contains (&all.coordinate[i], game->free) == TRUE) temp.coordinate[temp.count++] = all.coordinate[i]; // if the coordinate is in the free set, then it is added to the temp set
		switch (aTurn)
		{
			case TRUE: // the current player is alpha
				if (Contains (&all.coordinate[i], game->beta) == TRUE 
                                    && Contains (&all.coordinate[i], game->S) == TRUE)
					temp.coordinate[temp.count++] = all.coordinate[i]; // if the current tile is occupied by beta and in the S set, then it can 'eaten' by alpha
			break;
			case FALSE: // otherwise beta
				if (Contains (&all.coordinate[i], game->alpha) == TRUE 
                                     && Contains (&all.coordinate[i], game->S) == TRUE)
					temp.coordinate[temp.count++] = all.coordinate[i]; // if the coordinate is in the alpha set and in the S set, then it is added to the temp set
			break;
		}
	}
	return temp;
}

/**
 * Function Description 
  @brief This function is used to determine if the game is over.
         @param game - The game to be checked.
         @return - The game state. 
 */
int GameOver (Game *game) //return game state
{
        Set temp_alpha, temp_beta; // temporary sets to store the current player's pieces
        int in_setY =  0, in_setE = 0, i; // counters to determine if the current player's pieces are in a specific set

        // stores the current player's pieces with available moves
        temp_alpha = AvailablePieces (game->alpha, game); 
        temp_beta = AvailablePieces (game->beta, game);
        for (i = 0; i < game->alpha.count; i++) if (Contains (&game->alpha.coordinate[i], game->Y) == TRUE) in_setE++; // checks if the current player's pieces are in the Y set (base of beta)
        for (i = 0; i < game->beta.count; i++) if (Contains (&game->beta.coordinate[i], game->E) == TRUE) in_setY++; // checks if the current player's pieces are in the E set (base of alpha)

        if (game->alpha.count - in_setE == 0) 
                return ALPHA_WINS_ON_BASE; // if alpha's present pieces are all in beta's base, then alpha wins
        else if (game->beta.count - in_setY == 0) 
                return BETA_WINS_ON_BASE; // if all of beta's pieces occupy alpha's base then beta wins
        else if ((temp_alpha.count == 0 && game->beta.count > in_setY ) || (temp_beta.count == 0  && game->alpha.count > in_setE )) 
                return STALEMATE; // if the current player has no available moves but not all pawns of the other player are in their base, then it is a stalemate
        else if (game->alpha.count > 0 && game->beta.count == 0) 
                return ALPHA_WINS_PAWNS_OUT; // if beta has no pieces left, then alpha wins
        else if (game->beta.count > 0 && game->alpha.count == 0) 
                return BETA_WINS_PAWNS_OUT; // if alpha has no pieces left, then beta wins
        else return FALSE; // otherwise the game is not over
}

/**
 @brief This  function will check if a tile is in the set.
         @param tile - The tile to check if it is in the set.
         @param current - The set to check if the tile is in.
         @return - Returns TRUE if the tile is in the set, FALSE otherwise. 
 */
int Contains (Coordinate *tile, Set current)
{
        int i;
        for (i = 0; i < current.count; i++) // loops through all the tiles in the set
                if (tile->x == current.coordinate[i].x 
                 && tile->y == current.coordinate[i].y)
                        return TRUE;  
        return FALSE;
}

/*
        @brief  Prints the game over message.
 */

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
        // stores the current player's pieces with available moves
        if (game->aTurn)
        {
                ALPHA_TURN;
                game->valid = AvailablePieces (game->alpha, game); 
        } else {
                BETA_TURN;
                game->valid = AvailablePieces (game->beta, game);
        }
        if (game->valid.count == 0)
                game->ok = !game->ok; // if the current player has no available moves, then the game is over
        else {
                printf (" Please select a piece to move: \n");
                prev = GetPiece (game->valid); // gets the coordinates of the chosen piece
                printf (" Available moves:\n");
                game->valid = ModifyValid (game, game->aTurn, prev); // modifies the valid set to include only the available moves
                game->next = GetMove (game->valid, game->aTurn, prev); // determines where the piece will be moved to
                EliminationProcess (game, prev, game->next, game->aTurn);
                DisplayBoard(game);
                printf ("%s \nSuccessfully moved to (%d, %d)! \n%s", RED, game->next.x, game->next.y, RESET);
                printf ("Press any key to continue...");
                getch();
                game->aTurn = !game->aTurn; // switches the turn
                game->ok = !game->ok; // ends loop to continue for the next player
        }
}

/*
        This function updates the pieces on the board.
 */
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
                                game->board[temp.x][temp.y] = BETA_PIECE; // the tile is occupied by one of beta's pawns
                        else if (Contains (&temp, game->alpha) == TRUE)
                                game->board[temp.x][temp.y] = ALPHA_PIECE; // the tile is occupied by one of alpha's pawns
                        else game->board[temp.x][temp.y] = FREE; // otherwise the tile is free
                }
        }
}

/*
        This function prints the board with the current state of the game.
 */

void DisplayBoard (Game *game)
{
        int i, j;
        system("cls"); // clears the screen
	LABEL_TOP;
	TOP;
        if (game->aTurn == TRUE) { 
                for (i = 1; i <= ROW ; i++) { 
                TILE(i, game->board[i]);
                if (i < ROW) PARTITION;
                }
        } else {
                for (j = ROW; j >= 1; j--) { //flips the board 
                TILE(j, game->board[j]);
                if (j > 1) PARTITION;
                }
        } BOTTOM;
}

/*
        This function is used to initialize the values of the game.
 */

void InitializeBoard (Game *game)
{
        Coordinate position;
        //initialize all the counters of sets to 0
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
                        game->board[position.x][position.y] = FREE;//initialize all the tiles to free
                        if (position.x % 2 == position.y % 2) //if the value of the row and column has the same parity
                        {       
                                game->S.coordinate[game->S.count++] = position; //add the tile to the S set
                                if (position.x <= 2) //if the tile is in the first two rows
                                {
                                        game->Y.coordinate[game->Y.count++] = position; //initialize beta's base
                                        game->beta.coordinate[game->beta.count++] = position; // initliaze beta's pawns
                                        game->board[position.x][position.y] = BETA_PIECE;
                                }
                                else if (position.x >= 6)
                                {       
                                        game->E.coordinate[game->E.count++] = position; //initialize alpha's base
                                        game->alpha.coordinate[game->alpha.count++] = position; // initialize alpha's pawns
                                        game->board[position.x][position.y] = ALPHA_PIECE;
                                }
                                else {
                                        game->free.coordinate[game->free.count++] = position; //remaining tiles are free
                                        game->board[position.x][position.y] = FREE;
                                }
                        } else {
                                game->free.coordinate[game->free.count++] = position;
                                game->board[position.x][position.y] = FREE;
                        }
                }
        }
}

/*
        This function displays a statement which indicates the winner of the game.
 */
void DeclareWinner(int over, Game *game)
{
        switch (over)
        {
        case ALPHA_WINS_ON_BASE:
                printf ("\nAll of alpha's current pieces are in beta's base!\n");
                Divider ();
                ALPHA_WIN;
                printf ("Player Beta lost with %d remaining piece(s).\n\n", game->beta.count);

        break;
        case ALPHA_WINS_PAWNS_OUT:
                puts ("\nPlayer Beta has no piece left!\n");
                Divider ();
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case BETA_WINS_ON_BASE:
                printf ("\nAll of beta's current pieces are in alpha's base!\n");
                Divider ();
                BETA_WIN;
               printf ("Player Alpha lost with %d remaining piece(s).\n\n", game->alpha.count);
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

/**
 @brief Adds a tile to the set.
         @param tile - The tile to add to the set.
         @param current - The set to add the tile to.
         @return - The set with the tile added. 
*/
void Add (Coordinate tile, Set *current)
{
        current->coordinate[current->count].x = tile.x;
        current->coordinate[current->count].y = tile.y;
        current->count++; // current->coordinate[current->count++] = tile;
}

/**
 @brief - Remove a tile from a set.
         @param tile - The tile to be removed.
         @param current - The set to be modified.
         @return - The set with the tile removed. 
*/
void Remove (Coordinate tile, Set *current)
{
        int i, key;
        for (i = 0; i < current->count; i++) if (tile.x == current->coordinate[i].x && tile.y == current->coordinate[i].y) key = i; // find the index of the tile to be removed
        while (key < current->count) // shift the remaining tiles to the left
        {
                current->coordinate[key].x = current->coordinate[key + 1].x;
                current->coordinate[key].y = current->coordinate[key + 1].y;
                key++;
        }
        current->coordinate[key].x = '\0';  // set the last tile to null
        current->coordinate[key].y = '\0';
        current->count--; 
}

/**
 @brief Moves a tile from one set to another.
       @param tile - the tcoordinates to be moved.
       @param current - the set where the tile is currently in.
       @param destination - the set where the tile will be moved to. 
*/
void Move (Coordinate tile, Set *current, Set *destination) // to do : add modification to board or just transfer stuff idk
{
        Remove (tile, current); // remove the tile from the current set
        Add (tile, destination); // add the tile to the destination set
}

void EliminationProcess (Game *game, Coordinate prev, Coordinate next, int aTurn)
{
        switch (aTurn) 
        {
	case TRUE: // if it is alpha's turn
        if (Contains (&next, game->free)) // if the next tile is free
        {           
                Move (prev, &game->alpha, &game->free); 
                Move (next, &game->free, &game->alpha);
        }
	if (Contains (&next, game->beta) && Contains (&next, game->S))  // if the next tile is beta and is in the S set
        { 
                Move (next, &game->beta, &game->free); //eating/ elimination process
                Move (prev, &game->alpha, &game->free);
                Move (next, &game->free, &game->alpha);
        } 
	break;
	case FALSE:
        if (Contains (&next, game->free)) 
        {
                Move (prev, &game->beta, &game->free);
                Move (next, &game->free, &game->beta);
        }
        if (Contains (&next, game->alpha) && Contains (&next, game->S)) // if the next tile is alpha and is in the S set
        {
                Move (next, &game->alpha,&game->free);
                Move (prev, &game->beta, &game->free);
                Move (next, &game->free, &game->beta);
        }
	break;
	}
        ModifyBoard (game); // update the board
        aTurn = !aTurn; // switch turns
}

int main ()
{
        Game engine, *game;
        game = &engine; 
        game->over = FALSE; 
        game->ok = FALSE; 
        game->aTurn = TRUE; // initiliaze to alpha's turn

        InitializeBoard (game); // initialize the whole game

        while (game->over == FALSE)
        {
                if (game->over == FALSE) game->ok = TRUE; // reset the ok flag
                while (game->ok) 
                        NextPlayerMove (game->prev, game);   // get the next move
                game->over = GameOver (game); // check if the game is over
        }
        DeclareWinner (game->over, game); 
        return 0;
}
