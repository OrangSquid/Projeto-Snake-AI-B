#include <conio2.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "core.h"
#include "snake_classico.h"
#include "labirinto.h"

int main() {

    SetConsoleTitle("Projeto Snake");

    _setcursortype(_NOCURSOR);

    Opções opções_labirinto = {27, 0};
    Opções opções_clássico = {27, 1};
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

    // Define a semente para o RNG
    srand((unsigned int)time(NULL));

    // Define o enconding como o correto
    // O set_locale não funciona com Unicode no Windows
    // Obrigado Windows
    system("chcp 65001");

    for(;;) {
        clrscr();
        printfxy((MAX_JANELA_X-62)/2, 7, "______          _      _          _____             _        ", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 8, "| ___ \\        (_)    | |        /  ___|           | |       ", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 9, "| |_/ / __ ___  _  ___| |_ ___   \\ `--. _ __   __ _| | _____ ", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 10, "|  __/ '__/ _ \\| |/ _ \\ __/ _ \\   `--. \\ '_ \\ / _` | |/ / _ \\", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 11, "| |  | | | (_) | |  __/ || (_) | /\\__/ / | | | (_| |   <  __/", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 12, "\\_|  |_|  \\___/| |\\___|\\__\\___/  \\____/|_| |_|\\__,_|_|\\_\\___|", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 13, "              _/ |                                           ", MENU_N_SELECIONADO);
        printfxy((MAX_JANELA_X-62)/2, 14, "             |__/                                            ", MENU_N_SELECIONADO);
        switch(menu(50, 14, 23, 4, opções_menu, NULL, false)) {
        case 0:
            labirinto(opções_labirinto);
            break;
        case 1:
            snake_classico(opções_clássico);
            break;
        case 2:
            switch(menu(67, 19, 23, 2, opções_submenu, NULL, false)) {
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