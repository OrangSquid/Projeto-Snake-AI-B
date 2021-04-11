#include <conio2.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "core.h"
#include "snake_classico.h"
#include "labirinto.h"

char opções_menu[4][30] = {
    "1> Maker de Labirintos",
    "2> Snake Clássico",
    "3> Opções",
    "4> Sair do Jogo"
};

char opções_submenu[2][30] = {
    "1> Labirinto",
    "2> Snake Clássico"
};

void print_menu() {
    printfxy((MAX_JANELA_X-62)/2, 7, "______          _      _          _____             _        ", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 8, "| ___ \\        (_)    | |        /  ___|           | |       ", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 9, "| |_/ / __ ___  _  ___| |_ ___   \\ `--. _ __   __ _| | _____ ", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 10, "|  __/ '__/ _ \\| |/ _ \\ __/ _ \\   `--. \\ '_ \\ / _` | |/ / _ \\", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 11, "| |  | | | (_) | |  __/ || (_) | /\\__/ / | | | (_| |   <  __/", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 12, "\\_|  |_|  \\___/| |\\___|\\__\\___/  \\____/|_| |_|\\__,_|_|\\_\\___|", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 13, "              _/ |                                           ", WHITE, BLACK);
    printfxy((MAX_JANELA_X-62)/2, 14, "             |__/                                            ", WHITE, BLACK);
    printfxy(50, 16, opções_menu[0], BLACK, WHITE);
    printfxy(50, 18, opções_menu[1], WHITE, BLACK);
    printfxy(50, 20, opções_menu[2], WHITE, BLACK);
    printfxy(50, 22, opções_menu[3], WHITE, BLACK);
}

int menu_input() {
    // Função responsável pela interatividade do menu
    int opção = 0;
    for(;;) {
        switch(getch()) {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        case '4':
            exit(0);
        // ENTER
        case 13:
            return opção;
        // SETAS
        case 224:
            // Deseleciona a antiga opção
            printfxy(50, 16 + opção * 2, opções_menu[opção], WHITE, BLACK);
            switch(getch()) {
            case CIMA:
                opção--;
                break;
            case BAIXO:
                opção++;
                break;
            }
        }
        if(opção > 3)
            opção = 0;
        else if(opção < 0)
            opção = 3;
        // Seleciona a nova opção
        printfxy(50, 16 + opção * 2, opções_menu[opção], BLACK, WHITE);
    }
}

int submenu_input() {
    // Função responsável pela interatividade do menu
    int opcao = 0;
    for(;;) {
        switch(getch()) {
        case '1':
            return 0;
        case '2':
            return 1;
        // ENTER
        case 13:
            return opcao;
        // ESC
        case 27:
            return 2;
        // SETAS
        case 224:
            // Deseleciona a antiga opção
            printfxy(67, 20 + opcao, opções_submenu[opcao], WHITE, BLACK);
            switch(getch()) {
            case CIMA:
                opcao--;
                break;
            case BAIXO:
                opcao++;
                break;
            }
        }
        if(opcao > 1)
            opcao = 0;
        else if(opcao < 0)
            opcao = 1;
        // Seleciona a nova opção
        printfxy(67, 20 + opcao, opções_submenu[opcao], BLACK, WHITE);
    }
}

int main() {

    SetConsoleTitle("Projeto Snake");

    // Esconde o cursor
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    Opções opções_labirinto = {27, 0};
    Opções opções_clássico = {27, 1};

    // Define a semente para o RNG
    srand((unsigned int)time(NULL));

    // Define o enconding como o correto
    // O set_locale não funciona com Unicode no Windows
    // Obrigado Windows
    system("chcp 65001");

    for(;;) {
        clrscr();
        print_menu();
        switch(menu_input()) {
        case 0:
            labirinto(opções_labirinto);
            break;
        case 1:
            snake_classico(opções_clássico);
            break;
        case 2:
            printfxy(67, 20, "1> Labirinto", BLACK, WHITE);
            printfxy(67, 21, "2> Snake Clássico", WHITE, BLACK);
            switch(submenu_input()) {
            case 0:
                menu_opções(0, &opções_labirinto);
                break;
            case 1:
                menu_opções(1, &opções_clássico);
                break;
            case 2:
                break;
            }
            break;
        case 3:
            return 0;
        }
    }
}