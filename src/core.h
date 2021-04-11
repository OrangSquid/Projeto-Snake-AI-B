#pragma once

#define MAX_JANELA_Y 30
#define MAX_JANELA_X 120

#define CIMA 72
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77

enum MapaEstados {
    LIVRE,
    COBRA,
    FRUTA,
    PAREDE,
    CURSOR_CURSOR,
    CURSOR_DESENHAR,
    CURSOR_BORRACHA
};

typedef struct {
    int velocidade, modo_ativo;
} Opções;

void printfxy(int x, int y, char* text, int text_color, int text_background);

void definir_fruta(int mapa[][MAX_JANELA_Y], int n_frutas);

void desenhar_mapa(int mapa[][MAX_JANELA_Y]);

int menu_pausa();

int menu_game_over(int pontuação, bool ganhou);

void menu_opções(int modo_de_jogo, Opções* opções);