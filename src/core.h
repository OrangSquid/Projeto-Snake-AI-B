#pragma once

#define MAX_JANELA_Y 30
#define MAX_JANELA_X 120
#define N_QUADRICULAS MAX_JANELA_X * MAX_JANELA_Y

#define CIMA 72
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77

#define MENU_SELECIONADO 240
#define MENU_N_SELECIONADO 7

#define COBRA_ATTR 16
#define FRUTA_ATTR 32
#define LIVRE_ATTR 7
#define PAREDE_ATTR 64

// Calcular o index do mapa através do x e do y
#define GET_POS(X, Y) MAX_JANELA_X * ((Y) - 1) + (X) - 1

// Macros para calcular o tempo de execução do prorgama em ms
#define TICK(X) clock_t X = clock()
#define TOCK(X) (int)(clock() - (X))

typedef struct {
    int velocidade;
    bool modo_ativo;
} Opções;

void printfxy(int x, int y, char* text, int attribute);

void definir_fruta(struct char_info mapa[], int n_frutas);

int menu(int alinhar_x, int topo, int fundo, int n_opções, char lista_opções[][30], char título[], bool desenhar_limites);

int menu_pausa();

int menu_game_over(int pontuação, bool ganhou);

void menu_opções(int modo_de_jogo, Opções* opções);