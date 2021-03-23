#pragma once

#define MAX_JANELA_Y 30
#define MAX_JANELA_X 120

struct Coordenadas {
    // Estado indica o que a posição tem
    short int x, y, estado;
};

void definir_fruta(struct Coordenadas mapa[][MAX_JANELA_Y], short int n_frutas);

void renderizar_mapa(struct Coordenadas mapa[][MAX_JANELA_Y]);

int menu_pausa();

char* string_option(int modo, int menu);

void printfxy(int x, int y, char* text);