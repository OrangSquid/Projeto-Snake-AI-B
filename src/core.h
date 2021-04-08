#pragma once

#define MAX_JANELA_Y 30
#define MAX_JANELA_X 120

enum MapaEstados {
    LIVRE,
    COBRA,
    FRUTA,
    PAREDE,
    CURSOR_CURSOR,
    CURSOR_DESENHAR,
    CURSOR_BORRACHA
};

struct Opções {
    int velocidade, modo_ativo;
};

void printfxy(int x, int y, char* text, int text_color, int text_background);

void definir_fruta(int mapa[][MAX_JANELA_Y], int n_frutas);

void desenhar_mapa(int mapa[][MAX_JANELA_Y]);

int menu_pausa();

int menu_game_over(int pontuação);

void menu_opções(int modo_de_jogo, struct Opções* opções);

int menu_ganhar(int pontuação);