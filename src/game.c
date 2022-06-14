
#include "engine.c"


// void NextPlayerMove (Coordinate prev, Coordinate next, Game *game)
// {
//         Set temp;
//         int key = 0;
//         while (!(game->over))
//         {
//                 displayBoard (game);
//                 if (game->aTurn)
//                 {
//                         if (game->alpha.count > 0)
//                                 game->ok = TRUE;
//                         while (game->ok)
//                         {
//                                 ALPHA_TURN;
//                                 game->valid = AvailableMoves (game->alpha, game);
//                                 game->prev = GetMove (game->valid);

//                                 game->valid = ModifyValid (game, game->aTurn, game->prev);
//                                 game->next = GetMove (game->valid);

                                
//                         }
//                         if (game->ok)
//                                 game->aTurn = FALSE;
//                 }
//                 if (!(game->aTurn))
//                 {
//                         if (game->beta.count > 0)
//                                 game->ok = TRUE;
//                         while (game->ok)
//                         {

//                         }
//                         if (!(game->ok))
//                                 game->aTurn =TRUE;
//                 }
//                 game->over = GameOver (game);
//         }
        
// }

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
        printf (" Please select a piece to move: \n");
        for (i = 0; i < avail.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
        puts ("Enter x coordinate: ");
        scanf ("%d", &tile.x);
        puts ("Enter y coordinate: ");
        scanf ("%d", &tile.y);

        while (Contains (&tile, avail) == FALSE)
        {
                printf (" Please select a piece to move: \n");
                RED;
                puts ("Invalid move. \n");
                RESET;
                for (i = 0; i < avail.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, avail.coordinate[i].x, avail.coordinate[i].y);
                puts ("Enter x coordinate: ");
                scanf ("%d", &tile.x);
                puts ("Enter y coordinate: ");
                scanf ("%d", &tile.y);
        }
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
    displayBoard (game);
    ALPHA_TURN;
                                game->valid = AvailableMoves (game->alpha, game);
                                game->prev = GetMove (game->valid);

                                game->valid = ModifyValid (game, game->aTurn, game->prev);
                                game->next = GetMove (game->valid);
//     game->valid = AvailableMoves (game->alpha, game);
    for (i = 0; i < game->valid.count; i++)  printf ("\t[%d] (%d, %d)\n", i + 1, game->valid.coordinate[i].x, game->valid.coordinate[i].y);
//    s(game->free);
//    s(main.alpha);
//    s(main.beta);
    prev.x = 2;
    prev.y = 2;
    next.x = 5;
    next.y = 3;
    
    Move (prev, &game->beta, &game->free);
    Move (next, &game->free, &game->beta);
    ModifyBoard (game);
    game->aTurn = FALSE;
    displayBoard (game);
    
    prev.x = 6;
    prev.y = 2;
    next.x = 6;
    next.y = 2;
//    Move (prev, &game->alpha, &game->free);
//    Move (next, &game->free, &game->alpha);
game->aTurn = TRUE;
//     ModifyBoard (game);
//     displayBoard (game);
    
    game->valid = ModifyValid (game, game->aTurn, prev);

   ALPHA_TURN;
    
        RESET;

        return 0;
}