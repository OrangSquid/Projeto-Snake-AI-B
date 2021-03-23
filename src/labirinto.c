#include "stdio.h"
#include "conio2.h"
#include "labirinto.h"
#include "stdlib.h"
#include "windows.h"
#include "core.h"

int labirinto() {

    /*for(;;) {
        int acel_x = 0, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], game_over = 0;
        // Array com todas as posições da cobra
        // O primeiro elemento será sempre a cauda
        // O último elemento será sempre a cabeça
        struct Coordenadas quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y];
        struct Coordenadas *snake_cabeça = &quadrados_cobra[2];
        struct Coordenadas *snake_cauda = &quadrados_cobra[0];
        // Definir todos os quadrados como livres
        // 0 -> livre
        // 1 -> cobra
        // 2 -> maçã
        // 3 -> parede
        for(int x = 0; x < MAX_JANELA_X; x++) {
            for(int y = 0; y < MAX_JANELA_Y; y++) {
                mapa[x][y] = 0;
            }
        }

        // * Setup ao jogo
        for(int x = MAX_JANELA_X/2-2, z = 0; x <= MAX_JANELA_X/2; x++, z++) {
            mapa[x-1][MAX_JANELA_Y/2-1] = 1;
            quadrados_cobra[z].mapa_pointer = &mapa[x-1][MAX_JANELA_Y/2-1];
            quadrados_cobra[z].x = x;
            quadrados_cobra[z].y = MAX_JANELA_Y/2;
            textbackground(BLUE);
            putchxy(x, MAX_JANELA_Y/2, ' ');
        }
        mapa[MAX_JANELA_X/2-1][MAX_JANELA_Y/2-1] = 3;
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');
        // * Fim de setup ao jogo

        for(;;) {
            switch(getch()) {
            case 'w':
                acel_x = 0, acel_y = (acel_y != 0) ? acel_y : -1;
                break;
            case 's':
                acel_x = 0, acel_y = (acel_y != 0) ? acel_y : 1;
                break;
            case 'a':
                acel_x = (acel_x != 0) ? acel_x : -1, acel_y = 0;
                break;
            case 'd':
                acel_x = (acel_x != 0) ? acel_x : 1, acel_y = 0;
                break;
            }
        }

        // Loop do jogo
        for(;;) {
            switch(getch()) {
            case 'w':
                acel_x = 0, acel_y = (acel_y != 0) ? acel_y : -1;
                break;
            case 's':
                acel_x = 0, acel_y = (acel_y != 0) ? acel_y : 1;
                break;
            case 'a':
                acel_x = (acel_x != 0) ? acel_x : -1, acel_y = 0;
                break;
            case 'd':
                acel_x = (acel_x != 0) ? acel_x : 1, acel_y = 0;
                break;
            }
            // Aplicar o movimento à cabeça
            struct Coordenadas *nova_cabeça = (snake_cabeça+1);
            nova_cabeça->x = snake_cabeça->x + acel_x;
            nova_cabeça->y = snake_cabeça->y + acel_y;
            snake_cabeça = nova_cabeça;
            // Verificar se a cabeça antigiu o limite da janela
            // Em X
            if(snake_cabeça->x > MAX_JANELA_X) snake_cabeça->x = 1;
            else if(snake_cabeça->x < 1)       snake_cabeça->x = MAX_JANELA_X;
            // Em Y
            if(snake_cabeça->y > MAX_JANELA_Y) snake_cabeça->y = 1;
            else if(snake_cabeça->y < 1)       snake_cabeça->y = MAX_JANELA_Y;

            snake_cabeça->mapa_pointer = &mapa[snake_cabeça->x-1][snake_cabeça->y-1];
            textbackground(BLUE);
            putchxy(snake_cabeça->x, snake_cabeça->y, ' ');
            // Verificar a colisão
            switch(*(snake_cabeça->mapa_pointer)) {
            // Cobra ou parede
            case 3:
            case 1:
                textbackground(BLACK);
                clrscr();
                game_over = 1;
                break;
            default:
                textbackground(BLACK);
                putchxy(snake_cauda->x, snake_cauda->y, ' ');
                *(snake_cauda->mapa_pointer) = 0;
                for(int x = 1; x <= 3; x++) {
                    quadrados_cobra[x-1] = quadrados_cobra[x];
                }
                snake_cabeça = (snake_cabeça-1);
                quadrados_cobra[3].x = 0; 
                quadrados_cobra[3].y = 0; 
                quadrados_cobra[3].mapa_pointer = NULL;
                break;
            }
            *(snake_cabeça->mapa_pointer) = 1;
            if(game_over) {
                game_over = 0;
                break;
            }
            Sleep(20);
        }
    }*/
}