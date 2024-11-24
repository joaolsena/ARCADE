#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "flappy_bird.h"
#include "utils.h"

#define MAX_tela_X 30
#define MAX_tela_y 20
#define MAX_margem 40

#define forma_jogador '>'
#define forma_obstaculo '#'

#define gravidade 1
#define pulo -2
#define espaco 2
#define ATRASO_TIQUE 200000
#define max_tubos 4

int ponto=0;
int pontuacao_maxima = 0;
const char *ARQUIVO_PONTUACAO = "pontuacao_maxima_bird.txt";

char imagem[MAX_tela_y][MAX_tela_X] = {0};

typedef struct {
    int x;
    int y;
} passaro;

passaro bird;

typedef struct {
    int x;
    int y;
} obstaculo;

obstaculo obstaculos[max_tubos];  // Dois obstáculos (pares de tubos)


// Funções auxiliares
void salvar_pontuacao_maxima() {
    FILE *fp = fopen(ARQUIVO_PONTUACAO, "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", pontuacao_maxima);
        fclose(fp);
    } else {
        perror("Erro ao salvar a pontuação máxima");
    }
}

void carregar_pontuacao_maxima() {
    FILE *fp = fopen(ARQUIVO_PONTUACAO, "r");
    if (fp != NULL) {
        fscanf(fp, "%d", &pontuacao_maxima);
        fclose(fp);
    } else {
        pontuacao_maxima = 0; // Arquivo ainda não existe
    }
}


void tocar_som(char *arquivo_som) {
    char comando[150];
    snprintf(comando, sizeof(comando), "afplay '%s' &", arquivo_som); 
    system(comando);
}

void tela() {
    printf("%*sPontos: %d\n", MAX_margem + 45, "", ponto);
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
            } else if (imagem[i][j] == forma_jogador) {
                printf("\033[33m%c\033[0m", forma_jogador);
            } else if (imagem[i][j] == forma_obstaculo) {
                printf("\033[32m%c\033[0m", forma_obstaculo);
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
}

void inicia_jogador() {
    bird.x = 3;
    bird.y = (MAX_tela_y / 2) -4;
    imagem[bird.y][bird.x] = forma_jogador;
}

void inicia_obstaculos() {
    for (int i = 0; i < max_tubos; i++) {
        obstaculos[i].x = MAX_tela_X - 1 - (i * (MAX_tela_X / max_tubos)- rand() % 4);  
        obstaculos[i].y = (MAX_tela_y / 2) - (espaco / 2) + (rand() % 6) - 3;;  
    }
}

void  configuracoes_iniciais(){
    inicia_jogador();
    inicia_obstaculos();
}
void tela_inicial() {
    do {
        limpar();
        
        printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
        printf("%*s                  Flappy Bird                    \n", MAX_margem + 35, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
        printf("%*s  Pontuação máxima: %d\n", MAX_margem + 35, "", pontuacao_maxima);
        printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
        printf("%*s  Pressione 's' para iniciar o jogo\n", MAX_margem + 35, "");
        printf("%*s  Pressione 'i' para ver as instruções\n", MAX_margem + 35, "");
        printf("%*s  Pressione 'q' para sair\n", MAX_margem + 35, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
        char opcao = getch();
        if (opcao == 's') {
            tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/swoosh-101soundboards.mp3");
            configuracoes_iniciais();
            break; 
        } else if (opcao == 'i') {
            limpar();
            printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
            printf("%*sInstruções\n", MAX_margem + 53, "");
             printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
            printf("%*s Use a barra de espaço para pular.\n", MAX_margem + 35, "");
             printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
            printf("%*s Evite os obstáculos para aumentar sua pontuação.\n", MAX_margem + 35, "");
             printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
            printf("%*s Pressione qualquer tecla para voltar ao menu.\n", MAX_margem + 35, "");
             printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
            getch(); 
           
        } else if (opcao == 'q') {
            exit(0);
        }
    } while (1);
}

void desenha_obstaculos() {
    for (int i = 0; i < max_tubos; i++) {
        for (int y = 0; y < MAX_tela_y; y++) {
            if (y < obstaculos[i].y || y > obstaculos[i].y + espaco) {
                if (obstaculos[i].x >= 0 && obstaculos[i].x < MAX_tela_X) {
                    imagem[y][obstaculos[i].x] = forma_obstaculo;
                    if (obstaculos[i].x + 1 < MAX_tela_X) {
                        imagem[y][obstaculos[i].x + 1] = forma_obstaculo;
                    }
                }
            }
        }
    }
}

void reniciar(){
    ponto=0;
    for (int i = 0; i < max_tubos; i++) {
        for (int y = 0; y < MAX_tela_y; y++) {
            if (y < obstaculos[i].y || y > obstaculos[i].y + espaco) {
                if (obstaculos[i].x >= 0 && obstaculos[i].x < MAX_tela_X) {
                    imagem[y][obstaculos[i].x] = ' ';
                    if (obstaculos[i].x + 1 < MAX_tela_X) {
                        imagem[y][obstaculos[i].x + 1] = ' ';
                    }
                }
            }
        }
    }
}


void tela_game_over() {
    tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/die-101soundboards.mp3");
     salvar_pontuacao_maxima();
    do
    {
    limpar();
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*s                   GAME OVER                     \n", MAX_margem + 35, "");
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
     printf("%*s        Pontuação maxima: %d\n", MAX_margem + 35, "", pontuacao_maxima);
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*s        Pontuação Final: %d\n", MAX_margem + 35, "", ponto);
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*sPressione f para voltar ao jogo ou q para sair...\n", MAX_margem + 35, "");
   char fim = getch();
    if (fim == 'f')
    {   
        reniciar();
        limpar();
        configuracoes_iniciais();
        limpar();
        break;
    }else if (fim == 'q')
    {
       exit(0); 
    } 
    } while (1);
}

void atualiza_jogador() {
    imagem[bird.y][bird.x] = ' ';
    if (kbhit()) {
        char move=getch();
        if(move == ' '){
            tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/flap-101soundboards.mp3");
        bird.y += pulo;
        }
    } else {
        bird.y += gravidade;
    }

    if (bird.y < 0) {
    bird.y = 0;
} else if (bird.y >= MAX_tela_y - 1) {
    bird.y = MAX_tela_y - 1;
}


    imagem[bird.y][bird.x] = forma_jogador;
}

void atualiza_obstaculos() {
    for (int i = 0; i < max_tubos; i++) {
        for (int y = 0; y < MAX_tela_y; y++) {
            if (obstaculos[i].x >= 0 && obstaculos[i].x < MAX_tela_X) {
                imagem[y][obstaculos[i].x] = ' ';
                if (obstaculos[i].x + 1 < MAX_tela_X) {
                    imagem[y][obstaculos[i].x + 1] = ' ';
                }
            }
        }

        obstaculos[i].x -= 1;

        if (obstaculos[i].x == bird.x - 2) {
            tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/point-101soundboards.mp3");
            ponto += 10;
        }
        if(pontuacao_maxima<ponto){
            pontuacao_maxima=ponto;
        }

        if (obstaculos[i].x < 1) {
            obstaculos[i].x = MAX_tela_X - 1 - rand() % 4;
            obstaculos[i].y = (MAX_tela_y / 2) - (espaco / 2) + (rand() % 6) - 3;;  
        }
    }

    desenha_obstaculos();
}
void colisao() {
    for (int i = 0; i < max_tubos; i++) {
        if ((bird.x == obstaculos[i].x || bird.x == obstaculos[i].x + 1) && (bird.y < obstaculos[i].y|| bird.y > obstaculos[i].y + espaco)) {   
            tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/flappy-bird-hit-sound-101soundboards.mp3");
                tela_game_over();  
        }
    }

    if (bird.y >= MAX_tela_y - 1 || bird.y < 1) {
        tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/flappybird/sons/flappy-bird-hit-sound-101soundboards.mp3");
        tela_game_over(); 
    }
}

void iniciarflappy_bird() {
    srand(time(NULL));
    carregar_pontuacao_maxima();
    tela_inicial();
    while (1) {
        limpar();
        tela();
        atualiza_obstaculos();
         atualiza_jogador();
        colisao();
        usleep(ATRASO_TIQUE - (ponto*100));
    }
    
}