#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "core.h"
#include "snake_classico.h"

// Macros para calcular a posição dos quadrados da cobra dependendo do index
#define GET_X(X) (int)(quadrados_cobra[X]-&mapa[0][0])/MAX_JANELA_Y
#define GET_Y(Y) (int)(quadrados_cobra[Y]-&mapa[0][0])%MAX_JANELA_Y

void snake_classico(Opções opções) {
    // Loop externo para facilitar novas tentativas
    for(;;) {
        clrscr();
        bool game_over = false;
        // index_cauda é incializada como -1 pois mais tarde isso é compensado
        int acel_x = 1, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], index_cabeça = 2, index_cauda = -1, pontuação = 0;
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
                // SETAS
                case 224:
                    switch(getch()) {
                    case CIMA:
                        acel_x = 0, acel_y = (acel_y != 0) ? acel_y : -1;
                        break;
                    case BAIXO:
                        acel_x = 0, acel_y = (acel_y != 0) ? acel_y : 1;
                        break;
                    case ESQUERDA:
                        acel_x = (acel_x != 0) ? acel_x : -1, acel_y = 0;
                        break;
                    case DIREITA:
                        acel_x = (acel_x != 0) ? acel_x : 1, acel_y = 0;
                        break;
                    }
                    break;
                // ESCAPE
                case 27:
                    switch(menu_pausa(3, &opções)) {
                    case 0:
                        desenhar_mapa(mapa);
                        continue;
                    case 1:
                        game_over = true;
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
            if(temp_x > MAX_JANELA_X - 1) 
                temp_x = 0;
            else if(temp_x < 0)           
                temp_x = MAX_JANELA_X - 1;
            // Em Y
            if(temp_y > MAX_JANELA_Y - 1) 
                temp_y = 0;
            else if(temp_y < 0)           
                temp_y = MAX_JANELA_Y - 1;

            // Desenhar nova posição da cobra
    	    textbackground(BLUE);
            putchxy(temp_x+1, temp_y+1, ' ');

            // Verificar se index_cabeça ultrapassa o limite de MAX_JANELA_X * MAX_JANELA_Y
            index_cabeça = (index_cabeça+1 > MAX_JANELA_X * MAX_JANELA_Y) ? 0 : index_cabeça + 1;
            quadrados_cobra[index_cabeça] = &mapa[temp_x][temp_y];

            // Verificar a colisão
            switch(*quadrados_cobra[index_cabeça]) {
            case LIVRE:
                index_cauda = (index_cauda+1 > MAX_JANELA_X * MAX_JANELA_Y) ? 0 : index_cauda + 1;
                textbackground(BLACK);
                putchxy(GET_X(index_cauda)+1, GET_Y(index_cauda)+1, ' ');
                *quadrados_cobra[index_cauda] = LIVRE;
                break;
            case COBRA:
                textbackground(BLACK);
                switch(menu_game_over(pontuação, false)) {
                case 0:
                    game_over = true;
                    continue;
                case 1:
                    return;
                case 2:
                    exit(0);
                }
            case FRUTA:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                definir_fruta(mapa, 1);
                break;
            }
            // Definir nova posição como cobra
            *quadrados_cobra[index_cabeça] = COBRA;
            delay(opções.velocidade);
        }
    }
}