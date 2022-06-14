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


#define ROW 7
#define COL 5
#define FREE ' '
#define BETA_PIECE 'o'
#define ALPHA_PIECE 'x'

#define MAX_PAIR 18
typedef char Board[ + 1][COL + 1];
typedef struct {
        int x;
        int y;
} Coordinate;

typedef struct {        
        int count;
        Coordinate coordinate[MAX_PAIR];
} Set;


// typedef struct {
//         Set Y, E, S;
//         char board[ROW + 1][COL + 1];
// } Data;

typedef struct {
        Set alpha, beta, free, valid, S; 
        Board board;
        int over, ok, aTurn;
} Game;

int IsValid (int aTurn, Coordinate *prev, Coordinate *next, Game *game);

#endif
