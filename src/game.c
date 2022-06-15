
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
                game->prev = GetMove (game->valid);
                        printf (" Please select a coordinate.\n");
                game->valid = ModifyValid (game, game->aTurn, game->prev);
                game->next = GetMove (game->valid);
                        ElimProcess (game, game->prev, game->next, game->aTurn);
         }
        if (!(game->aTurn))
        {
                BETA_TURN;
                game->valid = AvailableMoves (game->beta, game);
                game->prev = GetMove (game->valid);
                        printf (" Please select a coordinate.\n");
                game->valid = ModifyValid (game, game->aTurn, game->prev);
                game->next = GetMove (game->valid);
                        ElimProcess (game, game->prev, game->next, game->aTurn);
        }
        game->ok = !game->ok;
        game->aTurn = !game->aTurn;
}

int GameOver (Game *game) //return game state
{
        if (game->alpha.count == 0 || game->beta.count == 0) {
                if (game->alpha.count > 0 && game->beta.count == 0) return ALPHA;
                else return BETA;
        } else return FALSE;
}


Set AvailableMoves (Set current, Game *game)
{
        Set temp, holder; 

        int i;;
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
void s (Set s)
{
	int i;
	for (i = 0; i < s.count; i++)
	{
		
	 printf ("[%d] (%d, %d)\n",i + 1, s.coordinate[i].x,s.coordinate[i].y);
}
}
void DeclareWinner(int over)
{
        
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
    //displayBoard (game);
    //game->ok = !game->ok;

        while (game->over == FALSE)
        {
                game->over = GameOver (game);
                if (game->over == FALSE) 
                                game->ok = TRUE;

                displayBoard (game);
                while (game->ok)
                        NextPlayerMove (game);
                
        }
        DeclareWinner (game->over);
        return 0;
}