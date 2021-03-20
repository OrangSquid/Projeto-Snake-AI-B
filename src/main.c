#include "stdio.h"
#include "conio2.h"
#include "windows.h"
#include "stdlib.h"

const int MAX_JANELA_X = 120;
const int MAX_JANELA_Y = 30;

struct Posição {
    int x, y;
};

int main() {

    // Esconde o cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);


    struct Posição snake_cabeça = {MAX_JANELA_X/2, MAX_JANELA_Y/2};
    struct Posição snake_cauda = {MAX_JANELA_X/2-2, MAX_JANELA_Y/2};
    // Guarda todas as posições dos quadrados da cobra
    struct Posição coordenadas_quadrados_cobra[3600];
    int acel_x = 1, acel_y = 0, mapa[MAX_JANELA_X][MAX_JANELA_Y], *pointers_quadrados_cobra[3600], comprimento_cobra = 3;

    // Definir todos os quadrados como livres
    // 0 -> livre
    // 1 -> cobra
    // 2 -> maçã
    // 3 -> parede 
    for(int x = 0; x < MAX_JANELA_X; x++) {
        for(int y = 0; y < MAX_JANELA_Y; y++) {
            mapa[x][y] = 0;
        }
    }

    // Setup ao jogo
    for(int x = snake_cauda.x, z = 0; x <= snake_cabeça.x; x++, z++) {
        mapa[x-1][snake_cauda.y-1] = 1;
        pointers_quadrados_cobra[z] = &mapa[x-1][snake_cauda.y-1];
        coordenadas_quadrados_cobra[z].x = x;
        coordenadas_quadrados_cobra[z].y = snake_cauda.y;
        textbackground(BLUE);
        putchxy(x, snake_cauda.y, ' ');
    }

    textbackground(BLACK);

    // Loop do jogo
    for(;;) {
        // Ver se alguma tecla do foi pressionada
        if(kbhit()) {
            switch(getch()) {
            case 'w':
                acel_x = 0, acel_y = (acel_y == 1 || acel_y == -1) ? acel_y : -1;
                break;
            case 's':
                acel_x = 0, acel_y = (acel_y == 1 || acel_y == -1) ? acel_y : 1;
                break;
            case 'a':
                acel_x = (acel_x == 1 || acel_x == -1) ? acel_x : -1, acel_y = 0;
                break;
            case 'd':
                acel_x = (acel_x == 1 || acel_x == -1) ? acel_x : 1, acel_y = 0;
                break;
            }
        }
        // Aplicar o movimento à cabeça
        if(acel_x) {
            snake_cabeça.x += acel_x;
            // Verificar se a cabeça antigiu o limite da janela
            if(snake_cabeça.x > MAX_JANELA_X) {
                snake_cabeça.x = 1;
            }
            else if(snake_cabeça.x < 1) {
                snake_cabeça.x = MAX_JANELA_X;
            }
        }
        else if(acel_y) {
            snake_cabeça.y += acel_y;
            // Verificar se a cabeça antigiu o limite da janela
            if(snake_cabeça.y > MAX_JANELA_Y) {
                snake_cabeça.y = 1;
            }
            else if(snake_cabeça.y < 1) {
                snake_cabeça.y = MAX_JANELA_Y;
            }
        }
        // Definir a nova cabeça no mapa e tirar a cauda do array dos pointers
        mapa[snake_cabeça.x-1][snake_cabeça.y-1] = 1;
        *pointers_quadrados_cobra[0] = 0;
        pointers_quadrados_cobra[comprimento_cobra] = &mapa[snake_cabeça.x][snake_cabeça.y];
        coordenadas_quadrados_cobra[comprimento_cobra].x = snake_cabeça.x;
        coordenadas_quadrados_cobra[comprimento_cobra].y = snake_cabeça.y;
        // Trocar a cor da cauda e da cabeça
        textbackground(BLUE);
        putchxy(snake_cabeça.x, snake_cabeça.y, ' ');
        textbackground(BLACK);
        putchxy(snake_cauda.x, snake_cauda.y, ' ');
        // Subsitui snake_cauda pelo primeiro elemento deste array que é a cauda
        snake_cauda = coordenadas_quadrados_cobra[0];
        for(int x = 1; x <= comprimento_cobra; x++) {
            pointers_quadrados_cobra[x-1] = pointers_quadrados_cobra[x];
            coordenadas_quadrados_cobra[x-1] = coordenadas_quadrados_cobra[x];
        }
        Sleep(20);
    }
}