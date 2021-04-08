#include <conio2.h>
#include <stdlib.h>
#include "core.h"
#include "snake_classico.h"

// Macros para calcular a posição dos quadrados da cobra dependendo do index
#define GET_X(X) (int)(quadrados_cobra[X]-&mapa[0][0])/MAX_JANELA_Y
#define GET_Y(Y) (int)(quadrados_cobra[Y]-&mapa[0][0])%MAX_JANELA_Y

void snake_classico(struct Opções opções) {
    // Loop externo para facilitar novas tentativas
    for(;;) {
        clrscr();
        int acel_x = 1, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], game_over = 0, index_cabeça = 2, index_cauda = 0, pontuação = 0;
        // Array com pointers para todas as posições da cobra
        // index_cabeça e index_cauda indicam a posição da
        // cabeça e da cauda da cobra
        int *quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y + 1];

        // Inicializa todas as posições
        for(int x = 0; x < MAX_JANELA_X; x++) {
            for(int y = 0; y < MAX_JANELA_Y; y++) {
                mapa[x][y] = LIVRE;
            }
        }

        // * Setup ao jogo
        // Define posições da cobra e desenha-as
        for(int x = MAX_JANELA_X/2-2, z = 0; x <= MAX_JANELA_X/2; x++, z++) {
            mapa[x-1][MAX_JANELA_Y/2-1] = COBRA;
            quadrados_cobra[z] = &mapa[x-1][MAX_JANELA_Y/2-1];
            textbackground(BLUE);
            putchxy(x, MAX_JANELA_Y/2, ' ');
        }

        // Define posição aleatória da fruta
        definir_fruta(mapa, 1);
        // * Fim de setup ao jogo

        // Loop do jogo
        while(!game_over) {
            // Ver se alguma tecla foi pressionada
            if(kbhit() || !opções.modo_ativo) {
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
                // ESCAPE
                case 27:
                    clrscr();
                    switch(menu_pausa(3, &opções)) {
                    case 0:
                        desenhar_mapa(mapa);
                        continue;
                    case 1:
                        game_over = 1;
                        break;
                    case 3:
                        return;
                    }
                    break;
                default:
                    continue;
                }
            }
            // Aplicar o movimento à cabeça
            int temp_x = GET_X(index_cabeça) + acel_x;
            int temp_y = GET_Y(index_cabeça) + acel_y;
            // Verificar se a cabeça antigiu o limite da janela
            // Em X
            if(temp_x > MAX_JANELA_X - 1) temp_x = 0;
            else if(temp_x < 0)           temp_x = MAX_JANELA_X - 1;
            // Em Y
            if(temp_y > MAX_JANELA_Y - 1) temp_y = 0;
            else if(temp_y < 0)           temp_y = MAX_JANELA_Y - 1;

            quadrados_cobra[index_cabeça+1] = &mapa[temp_x][temp_y];

    	    textbackground(BLUE);
            putchxy(temp_x+1, temp_y+1, ' ');
            // Verificar a colisão
            switch(*quadrados_cobra[index_cabeça+1]) {
            case LIVRE: {
                textbackground(BLACK);
                putchxy(GET_X(index_cauda)+1, GET_Y(index_cauda)+1, ' ');
                *quadrados_cobra[index_cauda] = LIVRE;
                index_cabeça++, index_cauda++;
                break;
            }
            case COBRA:
                textbackground(BLACK);
                clrscr();
                switch(menu_game_over(pontuação)) {
                case 0:
                    game_over = 1;
                    continue;
                case 1:
                    return;
                case 2:
                    exit(0);
                }
            case FRUTA:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                definir_fruta(mapa, 1);
                index_cabeça++;
                break;
            }
            if(index_cabeça > MAX_JANELA_X * MAX_JANELA_Y - 1) index_cabeça = 0;
            if(index_cauda > MAX_JANELA_X * MAX_JANELA_Y - 1) index_cauda = 0;
            *quadrados_cobra[index_cabeça] = COBRA;
            delay(opções.velocidade);
        }
    }
}