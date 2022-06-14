
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
         ModifyBoard (game);
        displayBoard (game);
        // ModifyBoard (&main);

        prev.x = 6;
        prev.y = 2;

        next.x = 5;
        next.y = 3;

        Move (prev, &game->alpha, &game->free);
        Move (next, &game->free, &game->alpha);
        ModifyBoard (game);
        displayBoard (&main);

        prev.x = 5;
        prev.y = 3;

        next.x = 4;
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

        // while (game->over == FALSE)
        // {
        //         //clearscreen()
        //         displayBoard (game);
        //         //NextPlayerMove ();
        //         game->over = GameOver (game);
        // }

        return 0;
}