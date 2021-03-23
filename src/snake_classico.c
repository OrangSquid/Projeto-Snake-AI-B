#include "stdio.h"
#include "conio2.h"
#include "windows.h"
#include "stdlib.h"
#include "snake_classico.h"
#include "core.h"

void snake_classico() {

    clrscr();

    // Loop externo para facilitar novas tentativas
    for(;;) {
        short int acel_x = 1, acel_y = 0, comprimento_cobra = 3, game_over = 0;
        struct Coordenadas mapa[MAX_JANELA_X][MAX_JANELA_Y];
        // Array com pointers para todas as posições da cobra
        // O primeiro elemento será sempre a cauda
        // O último elemento será sempre a cabeça
        struct Coordenadas *quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y];

        // Inicializa todas as posições
        for(int x = 0; x < MAX_JANELA_X; x++) {
            for(int y = 0; y < MAX_JANELA_Y; y++) {
                mapa[x][y].x = x + 1;
                mapa[x][y].y = y + 1;
                mapa[x][y].estado = 0;
            }
        }

        // * Setup ao jogo
        for(int x = MAX_JANELA_X/2-2, z = 0; x <= MAX_JANELA_X/2; x++, z++) {
            mapa[x-1][MAX_JANELA_Y/2-1].estado = 1;
            quadrados_cobra[z] = &mapa[x-1][MAX_JANELA_Y/2-1];
            textbackground(BLUE);
            putchxy(x, MAX_JANELA_Y/2, ' ');
        }

        // Define posição aleatória da fruta
        definir_fruta(mapa, 1);
        // * Fim de setup ao jogo

        // Loop do jogo
        for(;;) {
            // Ver se alguma tecla do foi pressionada
            if(kbhit()) {
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
                // 27 é o número para o esc do getch
                case 27:
                    if(menu_pausa("Snake Clássico")) return;
                    else renderizar_mapa(mapa);
                    break;
                }
            }
            // Aplicar o movimento à cabeça
            int temp_x = quadrados_cobra[comprimento_cobra-1]->x + acel_x;
            int temp_y = quadrados_cobra[comprimento_cobra-1]->y + acel_y;
            // Verificar se a cabeça antigiu o limite da janela
            // Em X
            if(temp_x > MAX_JANELA_X) temp_x = 1;
            else if(temp_x < 1)       temp_x = MAX_JANELA_X;
            // Em Y
            if(temp_y > MAX_JANELA_Y) temp_y = 1;
            else if(temp_y < 1)       temp_y = MAX_JANELA_Y;

            quadrados_cobra[comprimento_cobra] = &mapa[temp_x-1][temp_y-1];

            textbackground(BLUE);
            putchxy(temp_x, temp_y, ' ');
            // Verificar a colisão
            switch(quadrados_cobra[comprimento_cobra]->estado) {
            // Lugar livre
            case 0:
                textbackground(BLACK);
                putchxy(quadrados_cobra[0]->x, quadrados_cobra[0]->y, ' ');
                quadrados_cobra[0]->estado = 0;
                for(int x = 1; x <= comprimento_cobra; x++) {
                    quadrados_cobra[x-1] = quadrados_cobra[x];
                }
                quadrados_cobra[comprimento_cobra] = NULL;
                break;
            // Cobra
            case 1:
                textbackground(BLACK);
                clrscr();
                game_over = 1;
                break;
            // Fruta
            case 2:
                definir_fruta(mapa, 1);
                comprimento_cobra++;
                break;
            }
            quadrados_cobra[comprimento_cobra-1]->estado = 1;
            if(game_over) {
                game_over = 0;
                break;
            }
            Sleep(30);
        }
    }
}