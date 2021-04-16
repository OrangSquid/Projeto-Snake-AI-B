#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "core.h"
#include "snake_classico.h"

void snake_classico(Opções opções) {
    // Loop externo para facilitar novas tentativas
    for(;;) {
        textattr(7);
        clrscr();
        bool game_over = false;
        // index_cauda é incializada como -1 pois mais tarde isso é compensado
        short int acel_x = 1, acel_y = 0;
        int index_cabeça = 2, index_cauda = -1, pontuação = 0;

        // Array com pointers para todas as posições da cobra
        // index_cabeça e index_cauda indicam a posição da
        // cabeça e da cauda da cobra
        int quadrados_cobra_x[N_QUADRICULAS + 1];
        int quadrados_cobra_y[N_QUADRICULAS + 1];
        struct char_info mapa[N_QUADRICULAS];

        // * Setup ao jogo
        // Define posições da cobra e desenha-as
        textattr(16);
        for(int x = MAX_JANELA_X/2-2, z = 0; x <= MAX_JANELA_X/2; x++, z++) {
            quadrados_cobra_x[z] = x;
            quadrados_cobra_y[z] = MAX_JANELA_Y/2;
            putchxy(x, MAX_JANELA_Y/2, ' ');
        }
        _conio_gettext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);

        // Define posição aleatória da fruta
        definir_fruta(mapa, 1);
        // * Fim de setup ao jogo

        // Loop do jogo
        while(!game_over) {
            TICK(cobra_time);
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
                    case 0: // Continuar
                        puttext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                        continue;
                    case 1: // Tentar de novo
                        game_over = true;
                        break;
                    case 3: // Voltar ao Menu Principal
                        return;
                    }
                    break;
                default:
                    continue;
                }
            }
            // Aplicar o movimento à cabeça
            int temp_x = quadrados_cobra_x[index_cabeça] + acel_x;
            int temp_y = quadrados_cobra_y[index_cabeça] + acel_y;
            // Verificar se a cabeça antigiu o limite da janela
            // Em X
            if(temp_x > MAX_JANELA_X) 
                temp_x = 1;
            else if(temp_x < 1)           
                temp_x = MAX_JANELA_X;
            // Em Y
            if(temp_y > MAX_JANELA_Y) 
                temp_y = 1;
            else if(temp_y < 1)           
                temp_y = MAX_JANELA_Y;

            // Desenhar nova posição da cobra
            textattr(COBRA_ATTR);
            putchxy(temp_x, temp_y, ' ');

            // Verificar se index_cabeça ultrapassa o limite de N_QUADRICULAS
            index_cabeça = (index_cabeça+1 > N_QUADRICULAS) ? 0 : index_cabeça + 1;
            quadrados_cobra_x[index_cabeça] = temp_x;
            quadrados_cobra_y[index_cabeça] = temp_y;

            // Verificar a colisão
            switch(mapa[GET_POS(temp_x, temp_y)].attr) {
            case LIVRE_ATTR:
                index_cauda = (index_cauda+1 > N_QUADRICULAS) ? 0 : index_cauda + 1;
                textattr(7);
                putchxy(quadrados_cobra_x[index_cauda], quadrados_cobra_y[index_cauda], ' ');
                mapa[GET_POS(quadrados_cobra_x[index_cauda], quadrados_cobra_y[index_cauda])].attr = LIVRE_ATTR;
                break;
            case COBRA_ATTR:
                textattr(7);
                switch(menu_game_over(pontuação, false)) {
                case 0:
                    game_over = true;
                    continue;
                case 1:
                    return;
                case 2:
                    exit(0);
                }
                break;
            case FRUTA_ATTR:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                definir_fruta(mapa, 1);
                break;
            }
            // Definir nova posição como cobra
            mapa[GET_POS(temp_x, temp_y)].attr = COBRA_ATTR;
            // Apenas dorme o tempo que resta da velocidade selecionada
            if(opções.velocidade-TOCK(cobra_time) <= 0) {
                delay(1);
            }
            else {
                delay(opções.velocidade-TOCK(cobra_time));
            }
        }
    }
}