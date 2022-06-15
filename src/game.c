
#include "engine.c"

void NextPlayerMove (Game *game)
{
        Set temp;
        int key = 0;
        displayBoard (game);
        
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
                game->prev = GetMove (game->valid);
                printf (" Available moves:\n");
                game->valid = ModifyValid (game, game->aTurn, game->prev);
                game->next = GetMove (game->valid);
                ElimProcess (game, game->prev, game->next, game->aTurn);
                game->aTurn = !game->aTurn;
                game->ok = !game->ok;
        }
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
Coordinate GetMove (Set avail)
{
        Coordinate tile;
        int i;

        for (i = 0; i < avail.count; i++) 
        {
        printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
        }
        puts ("Enter choice: ");
        scanf ("%d", &i);
        while (i < 1 || i > avail.count)
        {
                RED;
                puts ("Invalid input.");
                RESET;
                puts ("Please enter a valid choice: ");
                for (i = 0; i < avail.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
                scanf ("%d", &i);
        }
        tile = avail.coordinate[i - 1];
       // printf ("You have selected (%d, %d).\n", tile.x, tile.y);
        
        return tile;
}

void DeclareWinner(int over, Game *game)
{
        switch (over)
        {
        case ALPHA_WINS_MOVES_OUT:
                printf ("Player Beta has %d more remaining piece(s) but has no moves left!\n\n", game->beta.count);
                DIVIDER;
                GAME_OVER;
                DIVIDER;
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case ALPHA_WINS_PAWNS_OUT:
                puts ("\nPlayer Beta has no piece left!\n");
                DIVIDER;
                GAME_OVER;
                DIVIDER;
                ALPHA_WIN;
                printf ("Player Alpha won with %d remaining piece(s).\n\n", game->alpha.count);

        break;
        case BETA_WINS_MOVES_OUT:
                printf ("\nPlayer Alpha has %d more remaining piece(s) but has no moves left!\n", game->alpha.count);
                DIVIDER;
                GAME_OVER;
                DIVIDER;
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case BETA_WINS_PAWNS_OUT:
                puts ("\nPlayer Alpha has no piece left!");
                DIVIDER;
                GAME_OVER;
                DIVIDER;
                BETA_WIN;
                printf ("Player Beta won with %d remaining piece(s).\n\n", game->beta.count);
        break;
        case STALEMATE:
                puts ("\nBoth players have no moves left!\n");
                DIVIDER;
                GAME_OVER;
                DIVIDER;
                GREEN;
                puts ("\t\tSTALEMATE!\n\n");
                RESET;
        break;
        }
}

int main ()
{
    Game main, *game;
    game = &main;
    game->over = FALSE;
    game->ok = FALSE;
    game->aTurn = TRUE;
    Coordinate prev, next, temp;
    int i;

    initializeBoard (game);

        while (game->over == FALSE)
        {
                game->over = GameOver (game);
                if (game->over == FALSE) 
                                game->ok = TRUE;

                displayBoard (game);
                while (game->ok)
                        NextPlayerMove (game);
                
        }

        DeclareWinner (game->over, game);
        return 0;
}