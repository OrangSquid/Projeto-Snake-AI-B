#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
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

void setup_cobra(int mapa[][MAX_JANELA_Y], int *quadrados_cobra[MAX_JANELA_X * MAX_JANELA_Y + 1], int limpar_mapa) {
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

void labirinto(struct Opções opções) {

    for(;;) {
        clrscr();
        int acel_x = 1, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], começar_jogo = 0, game_over = 0, index_cabeça = COMPRIMENTO_PADRÃO-1, index_cauda = 0, pontuação = 0;
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
        printfxy((MAX_JANELA_X - 42)/2, MAX_JANELA_Y/2 + 5, "Pressione ENTER para terminar o labirinto", WHITE, BLACK);
        printfxy((MAX_JANELA_X - 38)/2, MAX_JANELA_Y/2 + 7, "Pressione qualquer tecla para começar", WHITE, BLACK);
        getch();
        clrscr();
        textbackground(RED);
        putchxy(MAX_JANELA_X/2, MAX_JANELA_Y/2, ' ');

        int modo_labirinto = CURSOR;
        // Loop do labirinto
        while(!começar_jogo) {
            int antigo_x = GET_X(0), antigo_y = GET_Y(0);
            int novo_x = antigo_x, novo_y = antigo_y;
            switch(getch()) {
            case 'w':
                novo_y = antigo_y - 1;
                break;
            case 's':
                novo_y = antigo_y + 1;
                break;
            case 'a':
                novo_x = antigo_x - 1;
                break;
            case 'd':
                novo_x = antigo_x + 1;
                break;
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
            // ENTER
            case 13:
                começar_jogo = 1;
                // Ter a certeza que a última posição é desenhada com a cor certa
                if(mapa[antigo_x][antigo_y] != PAREDE) {
                    textbackground(BLACK);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                }
                else {
                    textbackground(RED);
                    putchxy(antigo_x+1, antigo_y+1, ' ');
                }
                // A troca de começar_jogo para 1 força a que o loop termine após o continue
                continue;
            // ESCAPE
            case 27: {
                int estado_antigo = mapa[antigo_x][antigo_y];
                // Define um estado temporário do cursor para desenhar corretamente
                // o cursor após a pausa
                mapa[antigo_x][antigo_y] = 4 + modo_labirinto;
                switch(menu_pausa(2, &opções)) {
                case 0:
                    desenhar_mapa(mapa);
                    break;
                case 1:
                    começar_jogo = 1;
                    game_over = 1;
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
            if(novo_x > MAX_JANELA_X - 1) novo_x = 0;
            else if(novo_x < 0)           novo_x = MAX_JANELA_X - 1;
            // Em Y
            if(novo_y > MAX_JANELA_Y - 1) novo_y = 0;
            else if(novo_y < 0)           novo_y = MAX_JANELA_Y - 1;

            quadrados_cobra[0] = &mapa[novo_x][novo_y];
            switch(modo_labirinto) {
            case CURSOR:
                textbackground(RED);
                putchxy(novo_x+1, novo_y+1, ' ');
                break;
            case DESENHAR:
                textbackground(LIGHTRED);
                putchxy(novo_x+1, novo_y+1, ' ');
                mapa[novo_x][novo_y] = PAREDE;
                break;
            case BORRACHA:
                textbackground(MAGENTA);
                putchxy(novo_x+1, novo_y+1, ' ');
                mapa[novo_x][novo_y] = LIVRE;
                break;
            }
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

        setup_cobra(mapa, quadrados_cobra, 0);

        // Loop da cobra
        int frutas = FRUTAS;
        while(!game_over) {
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
                    switch(menu_pausa(2, &opções)) {
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
            case PAREDE:
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
                break;
            case FRUTA:
                pontuação = (opções.modo_ativo) ? pontuação + 2 : pontuação + 1;
                frutas--;
                break;
            }
            // Condição para ganhar
            if(frutas == 0) {
                textbackground(BLACK);
                clrscr();
                switch(menu_ganhar(pontuação)) {
                case 0:
                    clrscr();
                    setup_cobra(mapa, quadrados_cobra, 1);
                    desenhar_mapa(mapa);
                    frutas = FRUTAS;
                    index_cabeça = COMPRIMENTO_PADRÃO-1;
                    index_cauda = 0;
                    acel_x = 1;
                    acel_y = 0;
                    continue;
                case 1:
                    return;
                case 2:
                    exit(0);
                }
            }
            // Apgar cauda e mexer o index um para a frente
            textbackground(BLACK);
            putchxy(GET_X(index_cauda)+1, GET_Y(index_cauda)+1, ' ');
            *quadrados_cobra[index_cauda] = LIVRE;
            index_cabeça++, index_cauda++;
            // Meter o index no começo quando ultrapassa COMPIRMENTO_PDRÃO
            if(index_cabeça > MAX_JANELA_X * MAX_JANELA_Y - 1) index_cabeça = 0;
            if(index_cauda > MAX_JANELA_X * MAX_JANELA_Y - 1) index_cauda = 0;
            *quadrados_cobra[index_cabeça] = COBRA;
            delay(opções.velocidade);
        }
    }
}