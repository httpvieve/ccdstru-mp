#include "game.h"

#ifndef DISPLAY_H
#define DISPLAY_H

#define LABEL_TOP printf("\n     1   2   3   4   5\n");
#define TOP printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 187);
#define TILE(label, p) printf (" %d %c %c %c %c %c %c %c %c %c %c %c\n", label, 186, p[1], 186, p[2], 186, p[3], 186, p[4], 186, p[5], 186);
#define PARTITION printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 204, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 185);
#define BOTTOM printf ("   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 200, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 188);

#define VALID (Set set) for (int i = 0; i < set.count; i++) printf ("\t[%d] (%d, %d)\n", i + 1, set.coordinate[i].x, set.coordinate[i].y); 

#define DIVIDER RED; \
                for (int i = 0; i < 42; i++) \
                        if (i == 41) printf("\n");\
                        else printf("=");\
                        RESET;
           

#define ALPHA_TURN CYAN \
                  printf ("[Alpha's turn]\n");\
                  RESET;\
                   printf (" Please select a piece to move: \n");\

#define BETA_TURN PURPLE \
                  printf ("[Beta's turn]\n");\
                  RESET;\
                  printf (" Please select a piece to move: \n");\

#define ALPHA_WIN CYAN \
                  printf ("\tPlayer Alpha wins the game!\n");\
                  RESET;\

#define BETA_WIN PURPLE \
                  printf ("\tPlayer Beta wins the game!\n");\
                  RESET;\

#define GAME_OVER  RED;\
                   puts ("\t\tGAME OVER");\
                   RESET;\

#define RED printf("\033[1;31m");
#define PURPLE printf("\033[0;34m");
#define GREEN printf("\033[0;32m");
#define CYAN printf("\033[0;36m");
#define RESET printf("\033[0m");

#endif