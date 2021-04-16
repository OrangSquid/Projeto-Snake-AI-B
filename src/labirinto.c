#include <conio2.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "core.h"
#include "labirinto.h"

#define COMPRIMENTO_PADRÃO 3
#define FRUTAS 5
#define GET_X(X) (int)(quadrados_cobra[X]-&mapa[0][0])/MAX_JANELA_Y
#define GET_Y(Y) (int)(quadrados_cobra[Y]-&mapa[0][0])%MAX_JANELA_Y

enum modos_de_desenho {
    CURSOR,
    DESENHAR,
    BORRACHA
};

void setup_cobra(struct char_info mapa[], bool limpar_mapa, int quadrados_cobra_x[], int quadrados_cobra_y[]) {
    // "Petrifica" a cobra e a fruta que restou para a próxima jogada
    if(limpar_mapa) {
        for(int x = 0; x < N_QUADRICULAS; x++) {
            if(mapa[x].attr != LIVRE_ATTR) {
                mapa[x].attr = PAREDE_ATTR;
            }
        }
    }
    definir_fruta(mapa, FRUTAS);
    // Definir posição aleatória da cobra
    int cobra_x, cobra_y;
    do {
        cobra_x = rand()%MAX_JANELA_X;
        cobra_y = rand()%MAX_JANELA_Y;
    } while(mapa[GET_POS(cobra_x, cobra_y)].attr != LIVRE_ATTR);

    textattr(COBRA_ATTR);
    for(int x = 0; x < COMPRIMENTO_PADRÃO; x++) {
        quadrados_cobra_x[x] = cobra_x + x;
        quadrados_cobra_y[x] = cobra_y;
        putchxy(cobra_x + x, cobra_y, ' ');
        mapa[GET_POS(cobra_x+x, cobra_y)].attr = COBRA_ATTR;
    }
    textattr(LIVRE_ATTR);
}

void labirinto(Opções opções) {
    for(;;) {
        clrscr();
        bool começar_cobra = false, game_over = false;
        short int acel_x = 1, acel_y = 0;
        int index_cabeça = 0, index_cauda, pontuação = 0, modo_labirinto = CURSOR;

        int quadrados_cobra_x[N_QUADRICULAS + 1];
        int quadrados_cobra_y[N_QUADRICULAS + 1];
        struct char_info mapa[N_QUADRICULAS];

        // Para o modo labirinto o primeiro elemento de quadrados_cobra para
        // poder saber a posição do cursor
        quadrados_cobra_x[index_cabeça] = MAX_JANELA_X/2;
        quadrados_cobra_y[index_cabeça] = MAX_JANELA_Y/2;
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 3, "Pressione Q para desenhar o labirinto", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X - 45)/2, MAX_JANELA_Y/2 + 4, "Pressione E para usar a borracha o labirinto", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 5, "Pressione S para terminar o labirinto", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 7, "Pressione qualquer tecla para começar", MENU_N_SELECIONADO);
        getch();
        textattr(LIVRE_ATTR);
        clrscr();
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');

        // Loop do labirinto
        while(!começar_cobra) {
            switch(getch()) {
            case 224:
                switch(getch()) {
                case CIMA:
                    acel_x = 0, acel_y = -1;
                    break;
                case BAIXO:
                    acel_x = 0, acel_y = 1;
                    break;
                case ESQUERDA:
                    acel_x = -1, acel_y = 0;
                    break;
                case DIREITA:
                    acel_x = 1, acel_y = 0;
                    break;
                }
                break;
            case 's':
            case 'S':
                switch(modo_labirinto) {
                case DESENHAR:
                    textattr(PAREDE_ATTR);
                    break;
                default:
                    textattr(LIVRE_ATTR);
                    break;
                }
                putchxy(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça], ' ');
                começar_cobra = true;
                continue;
            case 'q':
            case 'Q':
                modo_labirinto = (modo_labirinto != DESENHAR) ? DESENHAR : CURSOR;
                acel_x = 0, acel_y = 0;
                break;
            case 'e':
            case 'E':
                modo_labirinto = (modo_labirinto != BORRACHA) ? BORRACHA : CURSOR;
                acel_x = 0, acel_y = 0;
                break;
            // ESCAPE
            case 27:
                _conio_gettext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                textattr(LIVRE_ATTR);
                switch(menu_pausa(2, &opções)) {
                case 0: // Retomar
                    puttext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                    break;
                case 1: // Reiniciar
                    começar_cobra = true;
                    game_over = true;
                    break;
                case 3:
                    return;
                }
                switch(modo_labirinto) {
                case DESENHAR:
                    mapa[GET_POS(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça])].attr = PAREDE_ATTR;
                default:
                    mapa[GET_POS(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça])].attr = LIVRE_ATTR;
                }
                continue;
            default:
                continue;
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

            switch(modo_labirinto) {
            case DESENHAR:
                mapa[GET_POS(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça])].attr = PAREDE_ATTR;
                break;
            case BORRACHA:
                mapa[GET_POS(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça])].attr = LIVRE_ATTR;
                break;
            }
            
            textattr(mapa[GET_POS(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça])].attr);
            putchxy(quadrados_cobra_x[index_cabeça], quadrados_cobra_y[index_cabeça], ' ');
            
            // Verificar se index_cabeça ultrapassa o limite de N_QUADRICULAS
            index_cabeça = (index_cabeça+1 > N_QUADRICULAS) ? 0 : index_cabeça + 1;
            quadrados_cobra_x[index_cabeça] = temp_x;
            quadrados_cobra_y[index_cabeça] = temp_y;

            switch(modo_labirinto) {
            case CURSOR:
                textattr(PAREDE_ATTR);
                break;
            case DESENHAR:
                textbackground(LIGHTRED);
                break;
            case BORRACHA:
                textbackground(MAGENTA);
                break;
            }
            putchxy(temp_x, temp_y, ' ');
        }
        // * Fim Loop Labirinto
        _conio_gettext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);

        setup_cobra(mapa, false, quadrados_cobra_x, quadrados_cobra_y);

        // Loop da cobra
        int frutas = FRUTAS;
        index_cabeça = COMPRIMENTO_PADRÃO-1;
        index_cauda = -1;
        while(!game_over) {
            TICK(cobra_time);
            if(kbhit() || !opções.modo_ativo) {
                switch(getch()) {
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
                    _conio_gettext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                    switch(menu_pausa(2, &opções)) {
                    case 0: // Retomar
                        puttext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                        continue;
                    case 1: // Reiniciar
                        game_over = true;
                        break;
                    case 3: // Menu Principal
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

            // Verificar o mesmo para index_cauda
            index_cauda = (index_cauda+1 > N_QUADRICULAS) ? 0 : index_cauda + 1;
            textattr(7);
            putchxy(quadrados_cobra_x[index_cauda], quadrados_cobra_y[index_cauda], ' ');
            mapa[GET_POS(quadrados_cobra_x[index_cauda], quadrados_cobra_y[index_cauda])].attr = LIVRE_ATTR;

            // Verificar a colisão
            switch(mapa[GET_POS(temp_x, temp_y)].attr) {
            case PAREDE_ATTR:
            case COBRA_ATTR:
                textattr(7);
                switch(menu_game_over(pontuação, false)) {
                case 0: // Tentar de Novo
                    // Limpar mapa
                    textattr(7);
                    clrscr();
                    _conio_gettext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                    game_over = true;
                    continue;
                case 1: // Menu Principal
                    return;
                case 2: // Sair
                    exit(0);
                }
                break;
            case FRUTA_ATTR:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                frutas--;
                // Condição para ganhar
                if(frutas == 0) {
                    textattr(7);
                    // Esperar pelo input de menu_game_over
                    switch(menu_game_over(pontuação, 1)) {
                    case 0: // Continuar
                        clrscr();
                        setup_cobra(mapa, true, quadrados_cobra_x, quadrados_cobra_y);
                        puttext(1, 1, MAX_JANELA_X, MAX_JANELA_Y, mapa);
                        frutas = FRUTAS;
                        index_cabeça = COMPRIMENTO_PADRÃO-1;
                        index_cauda = -1;
                        acel_x = 1;
                        acel_y = 0;
                        continue;
                    case 1: // Menu Principal
                        return;
                    case 2: // Sair
                        exit(0);
                    }
                }
                break;
            }
            mapa[GET_POS(temp_x, temp_y)].attr = COBRA_ATTR;
            if(opções.velocidade-TOCK(cobra_time) <= 0) {
                delay(1);
            }
            else {
                delay(opções.velocidade-TOCK(cobra_time));
            }
        }
    }
}