#include <conio2.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "core.h"
#include "snake_classico.h"
#include "labirinto.h"

char opções_menu[5][30] = {
    "1> Maker de Labirintos",
    "2> Snake Clássico",
    "3> Opções",
    "4> Sair do Jogo"
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

    struct Opções opções_labirinto = {27, 0};
    struct Opções opções_clássico = {27, 1};

    // Define a semente para o RNG
    srand((unsigned int)time(NULL));

    // Define o enconding como o correto
    // O set_locale não funciona com Unicode no Windows
    // Obrigado Windows
    system("chcp 65001");

    clrscr();

    int opção = 0, última_opção = 0;
    print_menu();
    for(;;) {
        textbackground(BLACK);
        textcolor(WHITE);
        switch(getch()) {
        case 'W':
        case 'w':
           // Deseleciona a antiga opção
            última_opção = opção;
            opção--;
            break;
        case 'S':
        case 's':
            última_opção = opção;
            opção++;
            break;
        case '1':
            labirinto(opções_labirinto);
            opção = 0;
            break;
        case '2':
            snake_classico(opções_clássico);
            opção = 0;
            break;
        case '3':
            clrscr();
            menu_opções(0, &opções_labirinto);
            opção = 0;
            break;
        case '4':
            return 0;
        // 13 é o número do enter do getch
        case 13:
            switch(opção) {
            case 0:
                labirinto(opções_labirinto);
                break;
            case 1:
                snake_classico(opções_clássico);
                break;
            case 2:
                clrscr();
                menu_opções(0, &opções_labirinto);
                break;
            case 3:
                return 0;
            }
            clrscr();
            print_menu();
            opção = 0;
            break;
        }
        if(opção > 3)
            opção = 0;
        else if(opção < 0)
            opção = 3;
        // Deseleciona a antiga opção
        printfxy(50, 16 + última_opção * 2, opções_menu[última_opção], WHITE, BLACK);
        // Seleciona a nova opção
        printfxy(50, 16 + opção * 2, opções_menu[opção], BLACK, WHITE);
    }
}