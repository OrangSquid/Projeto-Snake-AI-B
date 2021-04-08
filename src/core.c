#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core.h"

char opções_pausa[][29] = {
    "1 [Retomar]",
    "2 [Reiniciar]",
    "3 [Opções]",
    "4 [Voltar ao Menu Principal]"
};

char opções_game_over[][29] = {
    "1 [Tentar de novo]",
    "2 [Voltar ao Menu Principal]",
    "3 [Sair do Jogo]"
};

char opções_opções[][29] = {
    "[Velocidade Cobra]",
    "[Modo Ativo]"
};

char opções_ganhar[][29] = {
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

void menu_opções(int modo_de_jogo, struct Opções* opções) {
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
        case 'W':
        case 'w':
            // Deseleciona a antiga opção
            printfxy(50, 13 + opção * 2, opções_opções[opção], WHITE, BLACK);
            opção--;
            break;
        case 'S':
        case 's':
            printfxy(50, 13 + opção * 2, opções_opções[opção], WHITE, BLACK);
            opção++;
            break;
        case 'D':
        case 'd':
            mudar_valor = 1;
            break;
        case 'A':
        case 'a':
            mudar_valor = -1;
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
                break;
            case 1:
                opções->modo_ativo = (mudar_valor == 1) ? 1 : 0;
                break;
            }
            mudar_valor = 0;
        }
        if(opção > 1)
            opção = 0;
        else if(opção < 0)
            opção = 1;
        // Seleciona a nova opção
        printfxy(50, 13 + opção * 2, opções_opções[opção], BLACK, WHITE);
        gotoxy(69, 13);
        // Os espaços a mais servem para evitar 
        printf(" %d          ", opções->velocidade);
        gotoxy(69, 15);
        if(opções->modo_ativo)
            printf(" Sim");
        else
            printf(" Não");
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
    }
}

int menu_pausa(int modo_de_jogo, struct Opções* opções) {
    for(;;) {
        char string_jogo[40] = "";
        switch(modo_de_jogo) {
        case 0:
            strcat(string_jogo, " Padrão (Labirinto)");
            break;
        case 1:
            strcat(string_jogo, " Padrão (Snake Clásssico)");
            break;
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
            case '1':
                return 0;
            case '2':
                clrscr();
                menu_opções(modo_de_jogo, opções);
                // Tornando selecionar em 0, obriga a que o programa saia deste while loop
                // e desenhe o 
                selecionar = 0;
                break;
            case '3':
                return 2;
            case '4':
                return 3;
            case 'W':
            case 'w':
                // Deseleciona a antiga opção
                printfxy(50, 13 + opção * 2, opções_pausa[opção], WHITE, BLACK);
                opção--;
                break;
            case 'S':
            case 's':
                printfxy(50, 13 + opção * 2, opções_pausa[opção], WHITE, BLACK);
                opção++;
                break;
            // 13 é o número do enter do getch
            case 13:
                if(opção == 2) {
                    clrscr();
                    menu_opções(modo_de_jogo, opções);
                    // Tornando selecionar em 0, obriga a que o programa saia deste while loop
                    // e desenhe o 
                    selecionar = 0;
                }
                else
                    return opção;
            }
            if(opção > 3)
                opção = 0;
            else if(opção < 0)
                opção = 3;
            // Seleciona a nova opção
            printfxy(50, 13 + opção * 2, opções_pausa[opção], BLACK, WHITE);
        }
    }
}

int menu_game_over(int pontuação) {
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 11, "═", WHITE, BLACK);
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 19, "═", WHITE, BLACK);
    printfxy(50, 12, "GAME OVER Pontuação: ", WHITE, BLACK);
    gotoxy(71, 12);
    printf("%d", pontuação);
    printfxy(50, 14, opções_game_over[0], BLACK, WHITE);
    printfxy(50, 16, opções_game_over[1], WHITE, BLACK);
    printfxy(50, 18, opções_game_over[2], WHITE, BLACK);
    int opção = 0;
    while(1) {
        switch(getch()) {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        case 'W':
        case 'w':
            // Deseleciona a antiga opção
            printfxy(50, 14 + opção * 2, opções_game_over[opção], WHITE, BLACK);
            opção--;
            break;
        case 'S':
        case 's':
            printfxy(50, 14 + opção * 2, opções_game_over[opção], WHITE, BLACK);
            opção++;
            break;
        // 13 é o número do enter do getch
        case 13:
            return opção;
        }
        if(opção > 2)
            opção = 0;
        else if(opção < 0)
            opção = 2;
        // Seleciona a nova opção
        printfxy(50, 14 + opção * 2, opções_game_over[opção], BLACK, WHITE);
    }
}

int menu_ganhar(int pontuação) {
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 11, "═", WHITE, BLACK);
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 19, "═", WHITE, BLACK);
    printfxy(50, 12, "GANHOU Pontuação: ", WHITE, BLACK);
    gotoxy(71, 12);
    printf("%d", pontuação);
    printfxy(50, 14, opções_ganhar[0], BLACK, WHITE);
    printfxy(50, 16, opções_ganhar[1], WHITE, BLACK);
    printfxy(50, 18, opções_ganhar[2], WHITE, BLACK);
    int opção = 0;
    while(1) {
        switch(getch()) {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        case 'W':
        case 'w':
            // Deseleciona a antiga opção
            printfxy(50, 14 + opção * 2, opções_ganhar[opção], WHITE, BLACK);
            opção--;
            break;
        case 'S':
        case 's':
            printfxy(50, 14 + opção * 2, opções_ganhar[opção], WHITE, BLACK);
            opção++;
            break;
        // 13 é o número do enter do getch
        case 13:
            return opção;
        }
        if(opção > 2)
            opção = 0;
        else if(opção < 0)
            opção = 2;
        // Seleciona a nova opção
        printfxy(50, 14 + opção * 2, opções_ganhar[opção], BLACK, WHITE);
    }
}