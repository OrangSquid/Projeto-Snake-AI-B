#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "core.h"

char opções_pausa[4][29] = {
    "1 [Retomar]",
    "2 [Reiniciar]",
    "3 [Opções]",
    "4 [Voltar ao Menu Principal]"
};

char opções_game_over[3][29] = {
    "1 [Tentar de novo]",
    "2 [Voltar ao Menu Principal]",
    "3 [Sair do Jogo]"
};

char opções_opções[2][29] = {
    "[Velocidade Cobra]",
    "[Modo Ativo]",
};

char opções_ganhar[3][29] = {
    "1 [Continuar]",
    "2 [Voltar ao Menu Principal]",
    "3 [Sair do Jogo]"
};

// Visto que o cputsxy não segue o locale do cmd
// Esta é uma implementação rápida para poder usar acentuação
// Também retira a necessidade de definir a color do texto antes
void printfxy(int x, int y, char* text, int text_color, int text_background) {
    textbackground(text_background);
    textcolor(text_color);
    gotoxy(x, y);
    printf("%s", text);
    textbackground(BLACK);
    textcolor(WHITE);
}

void definir_fruta(int mapa[][MAX_JANELA_Y], int n_frutas) {
    // Define a nova posição da(s) fruta(s)
    // do/while serve para correr a inicialização das variáveis pelo menos
    // uma vez independentemente da condição do while
    int fruta_x, fruta_y;
    for(int x = 0; x < n_frutas; x++) {
        do {
            fruta_x = rand()%MAX_JANELA_X+1;
            fruta_y = rand()%MAX_JANELA_Y+1;
        } while(mapa[fruta_x-1][fruta_y-1] != LIVRE);
        mapa[fruta_x-1][fruta_y-1] = FRUTA;
        // Desenhar fruta
        textbackground(GREEN);
        putchxy(fruta_x, fruta_y, ' ');
        textbackground(BLACK);
    }
}

void desenhar_mapa(int mapa[][MAX_JANELA_Y]) {
    for(int x = 0; x < MAX_JANELA_X; x++) {
        for(int y = 0; y < MAX_JANELA_Y; y++) {
            switch(mapa[x][y]) {
            case COBRA:
                textbackground(BLUE);
                break;
            case FRUTA:
                textbackground(GREEN);
                break;
            case CURSOR_CURSOR:
            case PAREDE:
                textbackground(RED);
                break;
            case CURSOR_DESENHAR:
                textbackground(LIGHTRED);
                break;
            case CURSOR_BORRACHA:
                textbackground(MAGENTA);
                break;
            }
            putchxy(x+1, y+1, ' ');
            textbackground(BLACK);
        }
    }
}

void menu_opções(int modo_de_jogo, Opções* opções) {
    clrscr();
    // Modo de jogo
    // 0: Menu Principal (Labirinto)
    // 1: Menu Principal (Clássico)
    // 2: Labirinto
    // 3: Clássico
    printfxy(3, MAX_JANELA_Y-1, "Trabalho realizado por: Rafael Matos", WHITE, BLACK);
    char string_jogo[40] = "";
    switch(modo_de_jogo) {
        case 0: {
            strcat(string_jogo, " Padrão (Labirinto)");
            break;
        }
        case 1: {
            strcat(string_jogo, " Padrão (Snake Clásssico)");
            break;
        }
        case 2: {
            strcat(string_jogo, " Labirinto");
            break;
        }
        case 3: {
            strcat(string_jogo, " Snake Clássico");
            break;
        }
    }
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 10, "═", WHITE, BLACK);
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 16, "═", WHITE, BLACK);
    printfxy(50, 11, "OPÇÕES ", WHITE, BLACK);
    printfxy(56, 11, string_jogo, WHITE, BLACK);
    printfxy(50, 13, opções_opções[0], BLACK, WHITE);
    printfxy(50, 15, opções_opções[1], WHITE, BLACK);
    gotoxy(69, 13);
    printf(" %d", opções->velocidade);
    gotoxy(69, 15);
    if(opções->modo_ativo)
        printf(" Sim");
    else
        printf(" Não");
    printfxy(50, 18, "Altera a velocidade da cobra.          ", WHITE, BLACK);
    printfxy(50, 19, "Valores mais altos significam velocidades mais lentas.", WHITE, BLACK);
    int opção = 0, mudar_valor = 0;
    for(;;) {
        switch(getch()) {
        case 224:
            switch(getch()) {
            case CIMA:
                printfxy(50, 13 + opção * 2, opções_opções[opção], WHITE, BLACK);
                opção--;
                break;
            case BAIXO:
                printfxy(50, 13 + opção * 2, opções_opções[opção], WHITE, BLACK);
                opção++;
                break;
            case ESQUERDA:
                mudar_valor = -1;
                break;
            case DIREITA:
                mudar_valor = 1;
                break;
            }
            if(opção > 1)
                opção = 0;
            else if(opção < 0)
                opção = 1;
            printfxy(50, 13 + opção * 2, opções_opções[opção], BLACK, WHITE);
            // Mostrar a tooltip da definição
            switch(opção) {
            case 0:
                printfxy(50, 18, "Altera a velocidade da cobra.          ", WHITE, BLACK);
                printfxy(50, 19, "Valores mais altos significam velocidades mais lentas.", WHITE, BLACK);
                break;
            case 1:
                printfxy(50, 18, "Indica se a cobra se mexe por si mesma.", WHITE, BLACK);
                printfxy(50, 19, "Quando ligado, a pontuação recebida será duplicada.   ", WHITE, BLACK);
                break;
            }
            break;
        // ESC
        case 27:
            clrscr();
            return;
        }
        if(mudar_valor) {
            switch (opção) {
            case 0:
                opções->velocidade += mudar_valor; 
                if(opções->velocidade < 0)
                    opções->velocidade = 0;
                gotoxy(69, 13);
                printf(" %d          ", opções->velocidade);
                break;
            case 1:
                opções->modo_ativo = (mudar_valor == 1) ? 1 : 0;
                gotoxy(69, 15);
                if(opções->modo_ativo)
                    printf(" Sim");
                else
                    printf(" Não");
                break;
            }
            mudar_valor = 0;
        }
    }
}

int menu_pausa(int modo_de_jogo, Opções* opções) {
    clrscr();
    for(;;) {
        char string_jogo[40] = "";
        switch(modo_de_jogo) {
        case 2:
            strcat(string_jogo, " Labirinto");
            break;
        case 3:
            strcat(string_jogo, " Snake Clássico");
            break;
        }
        for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 10, "═", WHITE, BLACK);
        for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 20, "═", WHITE, BLACK);
        printfxy(50, 11, "PAUSA ", WHITE, BLACK);
        printfxy(56, 11, string_jogo, WHITE, BLACK);
        printfxy(50, 13, opções_pausa[0], BLACK, WHITE);
        printfxy(50, 15, opções_pausa[1], WHITE, BLACK);
        printfxy(50, 17, opções_pausa[2], WHITE, BLACK);
        printfxy(50, 19, opções_pausa[3], WHITE, BLACK);
        int opção = 0, selecionar = 1;
        while(selecionar) {
            switch(getch()) {
            case 224:
                // Deseleciona a antiga opção
                printfxy(50, 13 + opção * 2, opções_pausa[opção], WHITE, BLACK);
                switch(getch()) {
                case CIMA:
                    opção--;
                    break;
                case BAIXO:
                    opção++;
                    break;
                }
                if(opção > 3)
                    opção = 0;
                else if(opção < 0)
                    opção = 3;
                // Seleciona a nova opção
                printfxy(50, 13 + opção * 2, opções_pausa[opção], BLACK, WHITE);
                break;
            case '1':
                return 0;
            case '2':
                menu_opções(modo_de_jogo, opções);
                // Tornando selecionar em 0, obriga a que o programa saia deste while loop
                // e desenhe o 
                selecionar = 0;
                break;
            case '3':
                return 2;
            case '4':
                return 3;
            // ESC
            case 13:
                if(opção == 2) {
                    menu_opções(modo_de_jogo, opções);
                    // Tornando selecionar em 0, obriga a que o programa saia deste while loop
                    // e desenhe o 
                    selecionar = 0;
                }
                else
                    return opção;
            }
        }
    }
}

int menu_game_over(int pontuação, bool ganhou) {
    clrscr();
    char *título;
    char (*pointer_opções)[3][29];
    switch(ganhou) {
    case 0:
        título = "GAME OVER Pontuação: ";
        pointer_opções = &opções_game_over;
        break;
    case 1:
        título = "GANHOU Pontuação: ";
        pointer_opções = &opções_ganhar;
        break;
    }
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 11, "═", WHITE, BLACK);
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 19, "═", WHITE, BLACK);
    printfxy(50, 12, título, WHITE, BLACK);
    gotoxy(71, 12);
    printf("%d", pontuação);
    printfxy(50, 14, (*pointer_opções)[0], BLACK, WHITE);
    printfxy(50, 16, (*pointer_opções)[1], WHITE, BLACK);
    printfxy(50, 18, (*pointer_opções)[2], WHITE, BLACK);
    int opção = 0;
    bool confirmar;
    while(1) {
        switch(getch()) {
        // SETAS
        case 224:
            printfxy(50, 14 + opção * 2, (*pointer_opções)[opção], WHITE, BLACK);
            switch(getch()) {
            case CIMA:
                opção--;
                break;
            case BAIXO:
                opção++;
                break;
            }
            if(opção > 2)
                opção = 0;
            else if(opção < 0)
                opção = 2;
            break;
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        // ENTER
        case 13:
            return opção;
        }
        // Seleciona a nova opção
        printfxy(50, 14 + opção * 2, (*pointer_opções)[opção], BLACK, WHITE);
    }
}