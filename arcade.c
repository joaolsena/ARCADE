//g++ ARCADE.C snake.c utils.c Flappy_bird.c space_invaders.c -o ARCADE \-I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -lSDL2_mixer

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
#include "arcade.h" 

#define MAX_tela_X 35
#define MAX_tela_y 25
#define MAX_margem 85
#define MAX_PLAYERS 10

#define forma_escolha '>'

typedef struct {
    char nickname[10];
    int score;
} Player;

Player ranking[MAX_PLAYERS];
int num_players = 0;

typedef enum {
    TELA_MENU_PRINCIPAL,
    TELA_SELECAO_JOGOS,
    SAIR
} Estado;

Estado estado_atual = TELA_MENU_PRINCIPAL;

const int opcao_y[] = {10, 15, 20};
const int opcao_selecao_jogos_y[] = {5, 10, 15, 20};
int opcao_atual = 0;

Player jogador_atual;

void ordenarRanking() {
    for (int i = 0; i < num_players - 1; i++) {
        for (int j = i + 1; j < num_players; j++) {
            if (ranking[i].score < ranking[j].score) {
                Player temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }
}

// Adiciona o jogador ao ranking
void adicionarAoRanking(Player player) {
    // Verifica se há espaço no ranking
    if (num_players < MAX_PLAYERS) {
        ranking[num_players++] = player; // Adiciona o jogador
    } else if (player.score > ranking[MAX_PLAYERS - 1].score) {
        ranking[MAX_PLAYERS - 1] = player; // Substitui o último lugar
    }
    ordenarRanking(); // Ordena o ranking
}


// Exibe o ranking
void exibirRanking() {
    limpar();
    printf("\n\033[33m--- RANKING ---\033[0m\n\n");
    if (num_players == 0) {
        printf("Nenhum jogador no ranking ainda.\n");
    } else {
        for (int i = 0; i < num_players; i++) {
            printf("%d. %s - %d pontos\n", i + 1, ranking[i].nickname, ranking[i].score);
        }
    }
    printf("\nPressione qualquer tecla para voltar ao menu...");
    getchar(); // Aguarda entrada do usuário
}


// Solicita o nickname do jogador
void solicitarNickname() {
    limpar();
    printf("\033[33mInsira seu nickname (até 10 caracteres):\033[0m ");
    fgets(jogador_atual.nickname, 10, stdin);
    jogador_atual.nickname[strcspn(jogador_atual.nickname, "\n")] = '\0'; // Remove o '\n'
    jogador_atual.score = 20;
}

// Atualiza a pontuação após cada jogo
void atualizarPontuacao(int pontos) {
    jogador_atual.score += pontos;
}


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
                if (j == 12) printf(" \033[33mARCADE\033[0m ");
            } else if (i == 10 && j >= 12 && j <= 22) {
                if (j == 12) printf("\033[34mVer Ranking\033[0m");
            } else if (i == 15 && j >= 10 && j <= 25) {
                if (j == 10) printf("\033[32mSelecionar Jogos\033[0m");
            } else if (i == 20 && j >= 15 && j <= 18) {
                if (j == 15) printf("\033[35mSair\033[0m");
            } else if (j == 9 && i == opcao_y[opcao_atual]) {
                printf("\033[36m%c\033[0m", forma_escolha);
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
                if (j == 10) printf("\033[34mSELECIONE UM JOGO\033[0m");
            } else if (i == 5 && j >= 12 && j <= 16) {
                if (j == 12) printf("\033[32mSnake\033[0m");
            } else if (i == 10 && j >= 12 && j <= 22) {
                if (j == 12) printf("\033[31mFlappy Bird\033[0m");
            } else if (i == 15 && j >= 12 && j <= 25) {
                if (j == 12) printf("\033[33mSpace Invaders\033[0m");
            } else if (i == 20 && j >= 12 && j <= 17) {
                if (j == 12) printf("\033[35mVoltar\033[0m");
            } else if (j == 9 && i == opcao_selecao_jogos_y[opcao_atual]) {
                printf("\033[36m%c\033[0m", forma_escolha);
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
                    exibirRanking();
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
                     limpar();
                     reniciar_snake();
                     iniciarSnake();
                        break;
                    case 1:
                     limpar();
                     reniciar_bird();
                        iniciarflappy_bird();
                        break;
                    case 2:
                     limpar();
                     reinicia_jogo();
                        inicia_space();
                        break;
                    case 3:
                     limpar();
                     adicionarAoRanking(jogador_atual);
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

 void inicia_arcade() {
    while (1) {
        limpar();
        switch (estado_atual) {
            case TELA_MENU_PRINCIPAL:
            limpar();
                desenharMenuPrincipal();
                break;
            case TELA_SELECAO_JOGOS:
            limpar();
                desenharSelecaoJogos();
                break;
            case SAIR:
                exit(0);
        }
        mover();
    }

}
int main(){
    solicitarNickname();
    inicia_arcade();
}
