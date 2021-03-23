#include "stdio.h"
#include "conio2.h"
#include "windows.h"
#include "locale.h"
#include "core.h"
#include "snake_classico.h"
#include "labirinto.h"

void print_menu() {
    textbackground(BLACK);
    textcolor(WHITE);
    printfxy(5, 2, "---------------SNAKE---------------");
    textbackground(WHITE);
    textcolor(BLACK);
    printfxy(5, 5, "> Maker de Labirintos");
    textbackground(BLACK);
    textcolor(WHITE);
    printfxy(5, 7, "> Snake Clássico");
    printfxy(5, 9, "> Leaderboards");
    printfxy(5, 11, "> Opções");
    printfxy(5, 13, "> Sair do Programa");
}

int main() {
    // Esconde o cursor
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    // Define o enconding como o correto
    // O set_locale não funciona com UTF-8
    // Obrigado Windows
    system("chcp 65001");

    clrscr();

    int option = 0;
    print_menu();
    for(;;) {
        textbackground(BLACK);
        textcolor(WHITE);
        switch(getch()) {
        case 'W':
        case 'w':
           // Deseleciona a antiga opção
            printfxy(5, 5 + option * 2, string_option(0, option));
            option--;
            break;
        case 'S':
        case 's':
            printfxy(5, 5 + option * 2, string_option(0, option));
            option++;
            break;
        // 13 é o número do enter do getch
        case 13:
            switch(option) {
            case 0:
                labirinto();
                break;
            case 1:
                snake_classico();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                return 0;
            }
            print_menu();
            option = 0;
            break;
        }
        if(option > 4)
            option = 0;
        else if(option < 0)
            option = 4;
        // Seleciona a nova opção
        textbackground(WHITE);
        textcolor(BLACK);
        printfxy(5, 5 + option * 2, string_option(0, option));
    }
}