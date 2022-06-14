
#include "engine.c"
int main ()
{
        Game main, *game;
        game = &main;
        game->over = FALSE;
        game->ok = FALSE;
        game->aTurn = TRUE;
        Coordinate prev, next;

        initializeBoard (game);
        // ModifyBoard (&main);
        // displayBoard (&main);

        prev.x = 6;
        prev.y = 2;

        next.x = 5;
        next.y = 3;
        Move (prev, &game->alpha, &game->free);
        Move (next, &game->free, &game->alpha);
        ModifyBoard (game);
        displayBoard (&main);

        //Remove (prev, &game->alpha, &game->free, game->board);
        //Add (next, &game->alpha);
        
        // ModifyValidMoves (game->aTurn, &next, game);
        // printValid (game);

        // ModifyValidMoves (game->aTurn, &next, game);
        // printValid (game);

        

        // for ( int i = 0; i < game->beta.count; i++)
        //          printf ("Beta %d is (%d, %d)\n", i + 1, game->beta.coordinate[i].x, game->beta.coordinate[i].y);
        
        //printf ("%d", Contains (prev, game->alpha));
        // //displayBoard (game);
        // for (int i = 0; i < game->valid.count; i++)
        //          printf ("Valid move #%d is (%d, %d)\n", i + 1, game->valid.coordinate[i].x, game->valid.coordinate[i].y);
        

//        for (int i = 0; i < game->valid.count; i++)
//                  printf ("Valid move #%d is (%d, %d)\n", i + 1, game->valid.coordinate[i].x, game->valid.coordinate[i].y);
        
        
        // for ( int i = 0; i < game->free.count; i++)
        //          printf ("Free %d is (%d, %d)\n", i + 1, game->free.coordinate[i].x, game->free.coordinate[i].y);
        
        // for ( int i = 0; i < game->alpha.count; i++)
        //          printf ("Alpha %d is (%d, %d)\n", i + 1, game->alpha.coordinate[i].x, game->alpha.coordinate[i].y);
        

        // while (game->over == FALSE)
        // {
        //         //clearscreen()
        //         displayBoard (game);
        //         //NextPlayerMove ();
        //         game->over = GameOver (game);
        // }

        return 0;
}