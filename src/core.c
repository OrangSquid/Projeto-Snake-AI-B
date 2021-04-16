#include <conio2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "core.h"

// Visto que o cputsxy não segue o locale do cmd
// Esta é uma implementação rápida para poder usar acentuação
// Também retira a necessidade de definir a color do texto antes
void printfxy(int x, int y, char* text, int attribute) {
    textattr(attribute);
    gotoxy(x, y);
    printf("%s", text);
    textattr(15);
}

void definir_fruta(struct char_info mapa[], int n_frutas) {
    // Define a nova posição da(s) fruta(s)
    // do/while serve para correr a inicialização das variáveis pelo menos
    // uma vez independentemente da condição do while
    int fruta_x, fruta_y;
    textattr(FRUTA_ATTR);
    for(int x = 0; x < n_frutas; x++) {
        do {
            fruta_x = rand()%MAX_JANELA_X+1;
            fruta_y = rand()%MAX_JANELA_Y+1;
        } while(mapa[GET_POS(fruta_x, fruta_y)].attr != LIVRE_ATTR);
        mapa[GET_POS(fruta_x, fruta_y)].attr = FRUTA_ATTR;
        // Desenhar fruta
        putchxy(fruta_x, fruta_y, ' ');
    }
}

int menu(int alinhar_x, int topo, int fundo, int n_opções, char lista_opções[][30], char título[], bool desenhar_limites) {
    if(desenhar_limites) {
        for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, topo, "═", MENU_N_SELECIONADO);
        for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, fundo, "═", MENU_N_SELECIONADO);
    }
    if(título != NULL) {
        printfxy(alinhar_x, topo+1, título, MENU_N_SELECIONADO);
    }
    // Escrever as opções 
    printfxy(alinhar_x, topo+3, lista_opções[0], MENU_SELECIONADO);
    for(int x = 1; x < n_opções; x++) {
        printfxy(alinhar_x, topo+3 + 2*x, lista_opções[x], MENU_N_SELECIONADO);
    }
    int opção = 0;
    while(1) {
        switch(getch()) {
        case 224: // SETAS
            // Deseleciona a opção antiga
            printfxy(alinhar_x, topo+3 + opção * 2, lista_opções[opção], MENU_N_SELECIONADO);
            switch(getch()) {
            case CIMA:
                opção--;
                break;
            case BAIXO:
                opção++;
                break;
            }
            if(opção > n_opções-1)
                opção = 0;
            else if(opção < 0)
                opção = n_opções-1;
            break;
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            if(n_opções >= 3)
                return 2;
        case '4':
            if(n_opções >= 4)
                return 3;
        case 13: // ENTER
            return opção;
        }
        // Seleciona a nova opção
        printfxy(alinhar_x, topo+3 + opção * 2, lista_opções[opção], MENU_SELECIONADO);
    }
}

void menu_opções(int modo_de_jogo, Opções* opções) {
    textattr(7);
    clrscr();
    printfxy(3, MAX_JANELA_Y-1, "Trabalho realizado por: Rafael Matos", MENU_N_SELECIONADO);
    char opções_opções[2][30] = {
        "[Velocidade Cobra]",
        "[Modo Ativo]",
    };
    char título[40] = "OPÇÕES ";
    switch(modo_de_jogo) {
    case 0:
        strcat(título, "Padrão (Labirinto)");
        break;
    case 1:
        strcat(título, "Padrão (Snake Clássico)");
        break;
    case 2:
        strcat(título, "Labirinto");
        break;
    case 3:
        strcat(título, "Snake Clássico");
        break;
    }
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 10, "═", MENU_N_SELECIONADO);
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 16, "═", MENU_N_SELECIONADO);
    printfxy(50, 11, título, MENU_N_SELECIONADO);
    printfxy(50, 13, opções_opções[0], MENU_SELECIONADO);
    printfxy(50, 15, opções_opções[1], MENU_N_SELECIONADO);
    gotoxy(69, 13);
    printf(" %d", opções->velocidade);
    gotoxy(69, 15);
    if(opções->modo_ativo)
        printf(" Sim");
    else
        printf(" Não");
    printfxy(50, 18, "Altera a velocidade da cobra.          ", MENU_N_SELECIONADO);
    printfxy(50, 19, "Valores mais altos significam velocidades mais lentas.", MENU_N_SELECIONADO);
    int opção = 0, mudar_valor = 0;
    for(;;) {
        switch(getch()) {
        case 224:
            switch(getch()) {
            case CIMA:
                printfxy(50, 13 + opção * 2, opções_opções[opção], MENU_N_SELECIONADO);
                opção--;
                break;
            case BAIXO:
                printfxy(50, 13 + opção * 2, opções_opções[opção], MENU_N_SELECIONADO);
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
            printfxy(50, 13 + opção * 2, opções_opções[opção], MENU_SELECIONADO);
            // Mostrar a tooltip da definição
            switch(opção) {
            case 0:
                printfxy(50, 18, "Altera a velocidade da cobra.          ", MENU_N_SELECIONADO);
                printfxy(50, 19, "Valores mais altos significam velocidades mais lentas.", MENU_N_SELECIONADO);
                break;
            case 1:
                printfxy(50, 18, "Indica se a cobra se mexe por si mesma.", MENU_N_SELECIONADO);
                printfxy(50, 19, "Quando ligado, a pontuação recebida será duplicada.   ", MENU_N_SELECIONADO);
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

int menu_pausa(int modo_de_jogo, Opções *opções) {
    char opções_pausa[4][30] = {
        "1 [Retomar]",
        "2 [Reiniciar]",
        "3 [Opções]",
        "4 [Voltar ao Menu Principal]"
    };
    char título[40] = "PAUSA ";
    switch(modo_de_jogo) {
    case 2:
        strcat(título, "Labirinto");
        break;
    case 3:
        strcat(título, "Snake Clássico");
        break;
    }
    textattr(7);
    clrscr();
    for(;;) {
        int opção = menu(50, 10, 20, 4, opções_pausa, título, true);
        switch(opção) {
        case 2:
            menu_opções(modo_de_jogo, opções);
            break;
        default:
            textattr(7);
            clrscr();
            return opção;
        }
    }
}

int menu_game_over(int pontuação, bool ganhou) {
    textattr(7);
    clrscr();
    gotoxy(71, 12);
    printf("%d", pontuação);
    switch(ganhou) {
        case false: {
            char opções_game_over[3][30] = {
                "1 [Tentar de novo]",
                "2 [Voltar ao Menu Principal]",
                "3 [Sair do Jogo]"
            };
            return menu(50, 11, 19, 3, opções_game_over, "GAME OVER Pontuação: ", true);
        }
        case true: {
            char opções_ganhar[3][30] = {
                "1 [Continuar]",
                "2 [Voltar ao Menu Principal]",
                "3 [Sair do Jogo]"
            };
            return menu(50, 11, 19, 3, opções_ganhar, "GAME OVER Pontuação: ", true);
        }
    }
}