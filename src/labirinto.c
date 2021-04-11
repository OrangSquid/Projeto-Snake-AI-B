#include <conio2.h>
#include <stdlib.h>
#include <stdbool.h>
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

void setup_cobra(int mapa[][MAX_JANELA_Y], int *quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y + 1], bool limpar_mapa) {
    // "Petrifica" a cobra e a fruta que restou para a próxima jogada
    if(limpar_mapa) {
        for(int x = 0; x < MAX_JANELA_X; x++) {
            for(int y = 0; y < MAX_JANELA_Y; y++) {
                switch(mapa[x][y]) {
                case COBRA:
                    mapa[x][y] = PAREDE;
                case FRUTA:
                    mapa[x][y] = PAREDE;
                }
            }
        }
    }
    definir_fruta(mapa, FRUTAS);
    // Definir posição aleatória da cobra
    int cobra_x, cobra_y;
    do {
        cobra_x = rand()%MAX_JANELA_X+1;
        cobra_y = rand()%MAX_JANELA_Y+1;
    } while(mapa[cobra_x-1][cobra_y-1] != LIVRE);
    mapa[cobra_x-1][cobra_y-1] = COBRA;
    quadrados_cobra[0] = &mapa[cobra_x-1][cobra_y-1];
    textbackground(BLUE);
    putchxy(cobra_x, cobra_y, ' ');

    // Extender em mais COMPRIMENTO_PADRÃO - 1 a cobra
    for(int x = 1; x < COMPRIMENTO_PADRÃO; x++) {
        cobra_x++;
        mapa[cobra_x-1][cobra_y-1] = COBRA;
        quadrados_cobra[x] = &mapa[cobra_x-1][cobra_y-1];
        textbackground(BLUE);
        putchxy(cobra_x, cobra_y, ' ');
    }
    textbackground(BLACK);
}

void labirinto(Opções opções) {
    for(;;) {
        clrscr();
        bool começar_cobra = false, game_over = false;
        int acel_x = 1, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], index_cabeça = COMPRIMENTO_PADRÃO-1, index_cauda = -1, pontuação = 0;
        int *quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y + 1];

        // Define todos os quadrados como livres
        for(int x = 0; x < MAX_JANELA_X; x++) {
            for(int y = 0; y < MAX_JANELA_Y; y++) {
                mapa[x][y] = LIVRE;
            }
        }

        // Para o modo labirinto o primeiro elemento de quadrados_cobra para
        // poder saber a posição do cursor
        quadrados_cobra[0] = &mapa[MAX_JANELA_X/2-1][MAX_JANELA_Y/2-1];
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 3, "Pressione Q para desenhar o labirinto", WHITE, BLACK);
        printfxy((MAX_JANELA_X - 45)/2, MAX_JANELA_Y/2 + 4, "Pressione E para usar a borracha o labirinto", WHITE, BLACK);
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 5, "Pressione S para terminar o labirinto", WHITE, BLACK);
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 7, "Pressione qualquer tecla para começar", WHITE, BLACK);
        getch();
        clrscr();
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');

        int modo_labirinto = CURSOR;
        // Loop do labirinto
        while(!começar_cobra) {
            int antigo_x = GET_X(0), antigo_y = GET_Y(0);
            int novo_x = antigo_x, novo_y = antigo_y;
            switch(getch()) {
            case 224:
                switch(getch()) {
                case CIMA:
                    novo_y = antigo_y - 1;
                    break;
                case BAIXO:
                    novo_y = antigo_y + 1;
                    break;
                case ESQUERDA:
                    novo_x = antigo_x - 1;
                    break;
                case DIREITA:
                    novo_x = antigo_x + 1;
                    break;
                }
                break;
            case 'Q':
            case 'q':
                if(modo_labirinto != DESENHAR) {
                    textbackground(LIGHTRED);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                    modo_labirinto = DESENHAR;
                    mapa[antigo_x][antigo_y] = PAREDE;
                }
                else {
                    textbackground(RED);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                    modo_labirinto = CURSOR;
                }
                continue;
            case 'E':
            case 'e':
                if(modo_labirinto != BORRACHA) {
                    textbackground(MAGENTA);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                    modo_labirinto = BORRACHA;
                    mapa[antigo_x][antigo_y] = LIVRE;
                }
                else {
                    textbackground(RED);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                    modo_labirinto = CURSOR;
                }
                continue;
            case 'S':
            case 's':
                começar_cobra = true;
                // Ter a certeza que a última posição é desenhada com a cor certa
                if(mapa[antigo_x][antigo_y] != PAREDE) {
                    textbackground(BLACK);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                }
                else {
                    textbackground(RED);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                }
                // A troca de começar_cobra para true força a que o loop termine após o continue
                continue;
            // ESCAPE
            case 27: {
                int estado_antigo = mapa[antigo_x][antigo_y];
                // Define um estado temporário do cursor para desenhar corretamente
                // o cursor após a pausa
                mapa[antigo_x][antigo_y] = 4 + modo_labirinto;
                textbackground(BLACK);
                switch(menu_pausa(2, &opções)) {
                case 0: // Retomar
                    desenhar_mapa(mapa);
                    break;
                case 1: // Reiniciar
                    começar_cobra = true;
                    game_over = true;
                    break;
                case 3:
                    return;
                }
                mapa[antigo_x][antigo_y] = estado_antigo;
                continue;
            }
            default:
                continue;
            }
            // Verificar se a cabeça antigiu o limite da janela
            // Em X
            if(novo_x > MAX_JANELA_X - 1)
                novo_x = 0;
            else if(novo_x < 0)
                novo_x = MAX_JANELA_X - 1;
            // Em Y
            if(novo_y > MAX_JANELA_Y - 1)
                novo_y = 0;
            else if(novo_y < 0)
                novo_y = MAX_JANELA_Y - 1;

            quadrados_cobra[0] = &mapa[novo_x][novo_y];
            switch(modo_labirinto) {
            case CURSOR:
                textbackground(RED);
                break;
            case DESENHAR:
                textbackground(LIGHTRED);
                mapa[novo_x][novo_y] = PAREDE;
                break;
            case BORRACHA:
                textbackground(MAGENTA);
                mapa[novo_x][novo_y] = LIVRE;
                break;
            }
            putchxy(novo_x+1, novo_y+1, ' ');

            switch(mapa[antigo_x][antigo_y]) {
            case LIVRE:
                textbackground(BLACK);
                break;
            case PAREDE:
                textbackground(RED);
                break;
            }
            putchxy(antigo_x+1, antigo_y+1, ' ');
        }
        // * Fim Loop Labirinto

        setup_cobra(mapa, quadrados_cobra, false);

        // Loop da cobra
        int frutas = FRUTAS;
        while(!game_over) {
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
                    switch(menu_pausa(2, &opções)) {
                    case 0: // Retomar
                        desenhar_mapa(mapa);
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

            // Verificar o mesmo para index_cauda
            index_cauda = (index_cauda+1 > MAX_JANELA_X * MAX_JANELA_Y) ? 0 : index_cauda + 1;
            textbackground(BLACK);
            putchxy(GET_X(index_cauda)+1, GET_Y(index_cauda)+1, ' ');
            *quadrados_cobra[index_cauda] = LIVRE;

            // Verificar a colisão
            switch(*quadrados_cobra[index_cabeça]) {
            case PAREDE:
            case COBRA:
                textbackground(BLACK);
                switch(menu_game_over(pontuação, false)) {
                case 0: // Tentar de Novo
                    game_over = true;
                    continue;
                case 1: // Menu Principal
                    return;
                case 2: // Sair
                    exit(0);
                }
                break;
            case FRUTA:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                frutas--;
                // Condição para ganhar
                if(frutas == 0) {
                    textbackground(BLACK);
                    // Esperar pelo input de menu_game_over
                    switch(menu_game_over(pontuação, 1)) {
                    case 0: // Continuar
                        clrscr();
                        setup_cobra(mapa, quadrados_cobra, true);
                        desenhar_mapa(mapa);
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
            *quadrados_cobra[index_cabeça] = COBRA;
            delay(opções.velocidade);
        }
    }
}