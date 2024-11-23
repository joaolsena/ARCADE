#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_tela_X 35
#define MAX_tela_y 25
#define MAX_margem 40

#define forma_escolha '>'

typedef struct {
    char nickname[10];
    int score;
} Player;

typedef struct {
    int x;
    int y;
} Movimento;


const int opcao_y[] = {10, 15, 20};
int opcao_atual = 0; 

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int getch(void) {
    struct termios oldt, newt;
    int move;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    move = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return move;
}


void desenharTela() {
    system("clear"); 
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


void mover() {
    char opcao = getch();

    if (opcao == 'w' && opcao_atual > 0) {
        opcao_atual--; 
    } else if (opcao == 's' && opcao_atual < 2) {
        opcao_atual++; 
    } else if (opcao == 'q') {
        exit(0); 
    } else if (opcao == '\n') {
        
        switch (opcao_atual) {
            case 0:
                printf("Abrindo Ranking...\n");
                break;
            case 1:
                printf("Abrindo Seleção de Jogos...\n");
                break;
            case 2:
                printf("Saindo...\n");
                exit(0);
        }
    }
}

int main() {
    while (1) {
        desenharTela();
        mover();
    }
    return 0;
}
