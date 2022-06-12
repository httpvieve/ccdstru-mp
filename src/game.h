#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifndef GAME_H
#define GAME_H 

#define TRUE 1
#define FALSE 0

#define ROW 5
#define COL 7
#define NO_PIECE ' '
#define BETA_PIECE 'o'
#define ALPHA_PIECE 'x'

#define MAX_PAIR 18

typedef struct {
        int x;
        int y;
} Coordinate;

typedef struct {        
        int count;
        Coordinate coordinate[MAX_PAIR];
} Set;

typedef struct {
        Set Y, E, S;
        char board[ROW + 1][COL + 1];
} Data;
typedef struct {
        Data alpha, beta, free;
        
} Board;


#endif
