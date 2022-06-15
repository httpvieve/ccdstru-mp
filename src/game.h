#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifndef GAME_H
#define GAME_H 

#define TRUE 1
#define FALSE 0
#define ALPHA 3
#define BETA 4

#define ALPHA_WINS 3
#define BETA_WINS 4

#define ROW 7
#define COL 5
#define FREE ' '
#define BETA_PIECE 'o'
#define ALPHA_PIECE 'x'

#define MAX_PAIR ROW * COL
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

int IsValid (int aTurn, Coordinate *prev, Coordinate *next, Game *game);
Coordinate GetMove (Set avail);
Set AvailableMoves (Set current, Game *game);
#endif
