#include "conio2.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "core.h"

// Visto que o cputsxy não segue o locale do cmd
// Esta é uma implementação rápida para seguir a 
void printfxy(int x, int y, char* text) {
    gotoxy(x, y);
    printf("%s", text);
}

// Devolve o string da opção selecionada dependendo do modo
// O modo pode ser:
// 0: menu principal
// 1: menu pausa
char* string_option(int modo, int option) {
    switch(modo) {
    case 0:
        switch(option) {
        case 0:
            return "> Maker de Labirintos";
        case 1:
            return "> Snake Clássico";
        case 2:
            return "> Leaderboards";
        case 3:
            return "> Opções";
        case 4:
            return "> Sair do Programa";
        }
    case 1:
        switch(option) {
        case 0:
            return "[Retomar]";
        case 1:
            return "[Voltar ao Menu Principal]";
        }
    }
}

void definir_fruta(struct Coordenadas mapa[][MAX_JANELA_Y], short int n_frutas) {
    
    // Define a semente para a função random
    srand((unsigned int)time(NULL));

    // Define a nova posição da(s) fruta(s)
    // do/while serve para correr a inicialização das variáveis pelo menos
    // uma vez independentemente da condição do while
    int fruta_x, fruta_y;
    for(int x = 0; x < n_frutas; x++) {
        do {
            fruta_x = rand()%MAX_JANELA_X+1;
            fruta_y = rand()%MAX_JANELA_Y+1;
        } while(mapa[fruta_x-1][fruta_y-1].estado);
        mapa[fruta_x-1][fruta_y-1].estado = 2;
        // Desenhar fruta
        textbackground(GREEN);
        putchxy(fruta_x, fruta_y, ' ');
        textbackground(BLACK);
    }
}

void renderizar_mapa(struct Coordenadas mapa[][MAX_JANELA_Y]) {
    for(int x = 0; x < MAX_JANELA_X; x++) {
        for(int y = 0; y < MAX_JANELA_Y; y++) {
            switch(mapa[x][y].estado) {
            case 1:
                textbackground(BLUE);
                break;
            case 2:
                textbackground(GREEN);
                break;
            case 3:
                textbackground(RED);
                break;
            }
            putchxy(x+1, y+1, ' ');
            textbackground(BLACK);
        }
    }
}

int menu_pausa(char* modo_de_jogo) {
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 11, "͞");
    for(int x = 1; x <= MAX_JANELA_X; x++) printfxy(x, 17, "͟");
    printfxy(50, 12, "PAUSA ");
    printfxy(56, 12, modo_de_jogo);
    textbackground(WHITE);
    textcolor(BLACK);
    printfxy(50, 14, "[Retomar]");
    textbackground(BLACK);
    textcolor(WHITE);
    printfxy(50, 16, "[Voltar ao Menu Principal]");
    int option = 0;
    for(;;) {
        switch(getch()) {
        case 'W':
        case 'w':
            // Deseleciona a antiga opção
            printfxy(50, 14 + option * 2, string_option(1, option));
            option--;
            break;
        case 'S':
        case 's':
            printfxy(50, 14 + option * 2, string_option(1, option));
            option++;
            break;
        // 13 é o número do enter do getch
        case 13:
            clrscr();
            return option;
        }
        if(option > 1)
            option = 0;
        else if(option < 0)
            option = 1;
        // Seleciona a nova opção
        textbackground(WHITE);
        textcolor(BLACK);
        printfxy(50, 14 + option * 2, string_option(1, option));
        textbackground(BLACK);
        textcolor(WHITE);
    }
}