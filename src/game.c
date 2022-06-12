
#include "engine.c"
int main()
{
        Data data, *temp;
        temp = &data;
        init_board (temp);
        display_board (temp);
        //        for ( int i = 0; i < MAX_PAIR; i++)
        // {
        //         printf ("Coordinate %d is (%d, %d)\n", i + 1, data.S.coordinate[i].x, data.S.coordinate[i].y);
        // printf ("---%c", BORDER);
        // }
        // for (int i = 1; i < 6; i++)
        //         for ( int j = 1; j < 8; j++)
        //                 if (i % 2 == j % 2)
        //                         printf ("(%d, %d)\n", i, j);


        return 0;
}