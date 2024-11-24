#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "arcade.h" 
#include "snake.h" 



#define MAX_tela_X 50
#define MAX_tela_y 30
#define MAX_margem 35

#define forma_cabeca '0'
#define forma_corpo 'o'
#define forma_comida '@'

#define ATRASO_TIQUE 200000
#define MAX_CORPO 100  

int ponto_snake = 0;
int tamanho_corpo = 0; 
int direcao_x = 1;  
int direcao_y = 0;
int pontucao_maxima_snake=0;
const char *ARQUIVO_PONTUACAO_snake = "pontucao_maxima_snake.txt";

char imagem_snake[MAX_tela_y][MAX_tela_X] = {0};

typedef struct {
    int x;
    int y;
} cabecas;

cabecas cabeca;

typedef struct {
    int x;
    int y;
} corpos;

corpos corpo[MAX_CORPO];

typedef struct {
    int x;
    int y;
    int ativo;
} comidas;

comidas comida;

// Funções auxiliares
void salvar_pontucao_maxima_snake_snake() {
    FILE *fp = fopen(ARQUIVO_PONTUACAO_snake, "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", pontucao_maxima_snake);
        fclose(fp);
    } else {
        perror("Erro ao salvar a pontuação máxima");
    }
}

void carregar_pontucao_maxima_snake() {
    FILE *fp = fopen(ARQUIVO_PONTUACAO_snake, "r");
    if (fp != NULL) {
        fscanf(fp, "%d", &pontucao_maxima_snake);
        fclose(fp);
    } else {
        pontucao_maxima_snake = 0; // Arquivo ainda não existe
    }
}

void tocar_som_snake(char *arquivo_som) {
    char comando[150];
    snprintf(comando, sizeof(comando), "afplay '%s' &", arquivo_som); 
    system(comando);
}


void tela_snake() {
    printf("%*sPonto_snakes: %d\n", MAX_margem + 50, "", ponto_snake);
    for (int i = 0; i < MAX_tela_y; i++) {
        for (int k = 0; k < MAX_margem; k++) {
            printf("  ");
        }
        for (int j = 0; j < MAX_tela_X; j++) {
            if (j == 0 || j == MAX_tela_X - 1 || i == MAX_tela_y - 1 || i == 0) {
                printf("\033[35m#\033[0m");
            } else if (imagem_snake[i][j] == forma_cabeca) {
                printf("\033[32m%c\033[0m", forma_cabeca);
            } else if (imagem_snake[i][j] == forma_corpo) {
                printf("\033[32m%c\033[0m", forma_corpo);
            } else if (imagem_snake[i][j] == forma_comida) {
                printf("\033[31m%c\033[0m", forma_comida);
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
}

void inicia_cobra() {
    cabeca.x = MAX_tela_X / 2;
    cabeca.y = MAX_tela_y / 2;
    imagem_snake[cabeca.y][cabeca.x] = forma_cabeca;
}

void inicia_comida() {
    int posicao_valida = 0;
    
    while (!posicao_valida) {
      
        comida.x = rand() % (MAX_tela_X - 2) + 1;
        comida.y = rand() % (MAX_tela_y - 2) + 1;
        
       
        posicao_valida = 1;

       
        if (comida.x == cabeca.x && comida.y == cabeca.y) {
            posicao_valida = 0;
        }

      
        for (int i = 0; i < tamanho_corpo; i++) {
            if (comida.x == corpo[i].x && comida.y == corpo[i].y) {
                posicao_valida = 0;
                break;
            }
        }
    }

    comida.ativo = 1;
    imagem_snake[comida.y][comida.x] = forma_comida;
}

void  configuracoes_iniciais_snake(){
    inicia_cobra();
    inicia_comida();
}

void tela_inicial_snake() {
    do {
        limpar();
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s                  jogo da cobrinha                    \n", MAX_margem + 45, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s  Pontuação máxima: %d\n", MAX_margem + 45, "", pontucao_maxima_snake);
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s  Pressione 's' para iniciar o jogo\n", MAX_margem + 45, "");
        printf("%*s  Pressione 'i' para ver as instruções\n", MAX_margem + 45, "");
        printf("%*s  Pressione 'q' para sair\n", MAX_margem + 45, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        char opcao = getch();
        if (opcao == 's') {
            configuracoes_iniciais_snake();
            break; 
        } else if (opcao == 'i') {
             limpar();
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s           Instruções do Jogo da Cobrinha\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Use as setas para mover a cobra ou w,s,a,d.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Coma as @ para crescer e ganhar ponto_snakes.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Evite bater nas paredes ou no corpo da cobra.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  A cada comida, a cobra cresce e a pontuação aumenta.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s   Pressione qualquer tecla para voltar ao menu.\n", MAX_margem + 45, "");
            getch(); 
        } else if (opcao == 'q') {
            inicia_arcade();
        }
    } while (1);
}

void reniciar_snake(){

    for (int i = 0; i < tamanho_corpo; i++) {
        imagem_snake[corpo[i].y][corpo[i].x] = ' ';
    }
    ponto_snake=0;
    tamanho_corpo=0;
    comida.ativo=0;
    imagem_snake[comida.y][comida.x] = ' '; 
}

void tela_game_over_snake() {
     tocar_som_snake("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/death-sound.mp3");
     salvar_pontucao_maxima_snake_snake();
    do
    {
    limpar();
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*s                   GAME OVER                     \n", MAX_margem + 35, "");
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
     printf("%*s        Pontuação maxima: %d\n", MAX_margem + 35, "", pontucao_maxima_snake);
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*s        Pontuação Final: %d\n", MAX_margem + 35, "", ponto_snake);
    printf("%*s-------------------------------------------------\n", MAX_margem + 35, "");
    printf("%*sPressione f para voltar ao jogo ou q para sair...\n", MAX_margem + 35, "");
   char fim = getch();
    if (fim == 'f')
    {   
        reniciar_snake();
        limpar();
        configuracoes_iniciais_snake();
        limpar();
        break;
    }else if (fim == 'q')
    {
       inicia_arcade();
    } 
    } while (1);
}

void colisa_comida() {
    if (comida.ativo && cabeca.x == comida.x && cabeca.y == comida.y) {
        ponto_snake += 10;
        comida.ativo = 0;
        imagem_snake[comida.y][comida.x] = ' ';
        tocar_som_snake("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/food_G1U6tlb.mp3");
        inicia_comida();
        
       
        if (tamanho_corpo < MAX_CORPO) {
            tamanho_corpo++;
        }
    }
    if(pontucao_maxima_snake<ponto_snake){
        pontucao_maxima_snake=ponto_snake;
    }
}

void colisao_tela_snake() {
    if (cabeca.x == MAX_tela_X - 1 || cabeca.x == 0 || cabeca.y == MAX_tela_y - 1 || cabeca.y == 0) {
         tocar_som_snake("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/hit-by-a-wood-230542.mp3");
       tela_game_over_snake();
    }
}

void colisao_corpo() {
    for (int i = 0; i < tamanho_corpo; i++) {
        if (cabeca.x == corpo[i].x && cabeca.y == corpo[i].y) {
              tocar_som_snake("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/hit-by-a-wood-230542.mp3");
           tela_game_over_snake();
        }
    }
}

int seta() {
    int ch = getchar();
    if (ch == 27) { 
        if (getchar() == '[') { 
            ch = getchar();
            switch (ch) {
                case 'A': return 'w'; 
                case 'B': return 's'; 
                case 'C': return 'd'; 
                case 'D': return 'a';
            }
        }
    }
    return ch;
}

void mover_cobra() {
    
    if (tamanho_corpo > 0) {
        imagem_snake[corpo[tamanho_corpo - 1].y][corpo[tamanho_corpo - 1].x] = ' ';
    }

   
    for (int i = tamanho_corpo - 1; i > 0; i--) {
        corpo[i] = corpo[i - 1];
    }

   
    if (tamanho_corpo > 0) {
        corpo[0].x = cabeca.x;
        corpo[0].y = cabeca.y;
    }

    imagem_snake[cabeca.y][cabeca.x] = ' ';

    if (kbhit()) {
        char move = seta();
        if ((move == 'a' || move == 'A')&& direcao_x == 0) { direcao_x = -1; direcao_y = 0; }
        if ((move == 'd' || move == 'D') && direcao_x == 0) { direcao_x = 1; direcao_y = 0; }
        if ((move == 'w' || move == 'W') && direcao_y == 0) { direcao_x = 0; direcao_y = -1; }
        if ((move == 's' || move == 'S') && direcao_y == 0) { direcao_x = 0; direcao_y = 1; }
    }

    cabeca.x += direcao_x;
    cabeca.y += direcao_y;
    imagem_snake[cabeca.y][cabeca.x] = forma_cabeca;

    
    for (int i = 0; i < tamanho_corpo; i++) {
        imagem_snake[corpo[i].y][corpo[i].x] = forma_corpo;
    }
}

void iniciarSnake() {
    srand(time(NULL));
    carregar_pontucao_maxima_snake();
  tela_inicial_snake();
    while (1) {
        limpar();
        tela_snake();
        colisa_comida();
        mover_cobra();
        colisao_tela_snake();
        colisao_corpo();
        usleep(ATRASO_TIQUE-(ponto_snake*100));
    }
}
