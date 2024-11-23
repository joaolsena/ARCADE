#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_tela_X 35
#define MAX_tela_y 25
#define MAX_margem 40

typedef struct {
    char nickname[10];
    int score;
} Player;

void tela(){
     for (int i = 0; i < MAX_tela_y; i++) {
        for (int k = 0; k < MAX_margem; k++) {
            printf("  ");
        }
        for (int j = 0; j < MAX_tela_X; j++) {
            if (i == 0) {
                printf("\033[37m_\033[0m");
            } else if (j == 0 || j == MAX_tela_X - 1) {
                printf("|");
            } else if (i == MAX_tela_y - 1) {
                printf("-");
            } else if (i == 2 && j==15) {
                printf("ARCADE");
            } else if (i == 10 && j==15) {
                printf("Ver Ranking");
            } else if (i == 15 && j==15) {
                printf("selecionar jogos");
            } else if (i == 20 && j==15) {
                printf("Sair");
            } else {
                printf(" ");
            }
        }
         putchar('\n');
     }
}

int main(){
    tela();
}