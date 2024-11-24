#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "snake.h"
#include "flappy_bird.h"
#include "space_invaders.h" 
#include "utils.h"  

#define MAX_tela_X 35
#define MAX_tela_y 25
#define MAX_margem 40

#define forma_escolha '>'

typedef struct {
    char nickname[10];
    int score;
} Player;

typedef enum {
    TELA_MENU_PRINCIPAL,
    TELA_SELECAO_JOGOS,
    SAIR
} Estado;

Estado estado_atual = TELA_MENU_PRINCIPAL;

const int opcao_y[] = {10, 15, 20};
const int opcao_selecao_jogos_y[] = {5, 10, 15, 20};
int opcao_atual = 0;



void desenharMenuPrincipal() {
    limpar();
    for (int i = 0; i < MAX_tela_y; i++) {
        for (int k = 0; k < MAX_margem; k++) {
            printf(" ");
        }
        for (int j = 0; j < MAX_tela_X; j++) {
            if (i == 0) {
                printf("\033[37m_\033[0m");
            } else if (j == 0 || j == MAX_tela_X - 1) {
                printf("|");
            } else if (i == MAX_tela_y - 1) {
                printf("_");
            } else if (i == 2 && j >= 12 && j <= 19) {
                if (j == 12) printf(" ARCADE ");
            } else if (i == 10 && j >= 12 && j <= 22) {
                if (j == 12) printf("Ver Ranking");
            } else if (i == 15 && j >= 10 && j <= 25) {
                if (j == 10) printf("Selecionar Jogos");
            } else if (i == 20 && j >= 15 && j <= 18) {
                if (j == 15) printf("Sair");
            } else if (j == 9 && i == opcao_y[opcao_atual]) {
                printf("%c", forma_escolha);
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
}

void desenharSelecaoJogos() {
    limpar();
    for (int i = 0; i < MAX_tela_y; i++) {
        for (int k = 0; k < MAX_margem; k++) {
            printf(" ");
        }
        for (int j = 0; j < MAX_tela_X; j++) {
            if (i == 0) {
                printf("\033[37m_\033[0m");
            } else if (j == 0 || j == MAX_tela_X - 1) {
                printf("|");
            } else if (i == MAX_tela_y - 1) {
                printf("_");
            } else if (i == 2 && j >= 10 && j <= 26) {
                if (j == 10) printf("SELECIONE UM JOGO");
            } else if (i == 5 && j >= 12 && j <= 16) {
                if (j == 12) printf("Snake");
            } else if (i == 10 && j >= 12 && j <= 22) {
                if (j == 12) printf("Flappy Bird");
            } else if (i == 15 && j >= 12 && j <= 25) {
                if (j == 12) printf("Space Invaders");
            } else if (i == 20 && j >= 12 && j <= 17) {
                if (j == 12) printf("Voltar");
            } else if (j == 9 && i == opcao_selecao_jogos_y[opcao_atual]) {
                printf("%c", forma_escolha);
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
}

void mover() {
    char opcao = getch();

    switch (estado_atual) {
        case TELA_MENU_PRINCIPAL:
            if (opcao == 'w' && opcao_atual > 0) {
                opcao_atual--;
            } else if (opcao == 's' && opcao_atual < 2) {
                opcao_atual++;
            } else if (opcao == '\n') {
                if (opcao_atual == 0) {
                    printf("Abrindo Ranking...\n");
                } else if (opcao_atual == 1) {
                    estado_atual = TELA_SELECAO_JOGOS;
                    opcao_atual = 0;
                } else if (opcao_atual == 2) {
                    estado_atual = SAIR;
                }
            }
            break;

        case TELA_SELECAO_JOGOS:
            if (opcao == 'w' && opcao_atual > 0) {
                opcao_atual--;
            } else if (opcao == 's' && opcao_atual < 3) {
                opcao_atual++;
            } else if (opcao == '\n') {
                switch (opcao_atual) {
                    case 0:
                        iniciarSnake();
                        break;
                    case 1:
                        iniciarflappy_bird();
                        break;
                    case 2:
                        inicia_space();
                        break;
                    case 3:
                        estado_atual = TELA_MENU_PRINCIPAL;
                        opcao_atual = 0;
                        break;
                }
            }
            break;

        case SAIR:
            exit(0);
    }
}

int main() {
    while (1) {
        switch (estado_atual) {
            case TELA_MENU_PRINCIPAL:
                desenharMenuPrincipal();
                break;
            case TELA_SELECAO_JOGOS:
                desenharSelecaoJogos();
                break;
            case SAIR:
                exit(0);
        }
        mover();
    }

    return 0;
}
