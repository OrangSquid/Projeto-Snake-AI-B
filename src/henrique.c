#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <time.h>

int main()
{
    int x, y, nmacas, maca[120][32];
    for (nmacas = 0; nmacas < 6; nmacas++)
    {
        srand((unsigned int)time(NULL));
        x = rand() % 120 + 1;
        y = rand() % 30 + 1;
        while(maca[x][y] == 0) {
            x = rand() % 120 + 1;
            y = rand() % 30 + 1;
        }
        textbackground(GREEN);
        gotoxy(x, y);
        printf(" ");
        maca[x][y] = 0;
    }
}