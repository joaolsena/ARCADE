#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAX_tela_X 50
#define MAX_tela_y 30
#define MAX_margem 35

#define forma_cabeca '0'
#define forma_corpo 'o'
#define forma_comida '@'

#define ATRASO_TIQUE 200000
#define MAX_CORPO 100  

int ponto = 0;
int tamanho_corpo = 0; 
int direcao_x = 1;  
int direcao_y = 0;
int pontuacao_maxima=0;
const char *ARQUIVO_PONTUACAO = "pontuacao_maxima_snake.txt";

char imagem[MAX_tela_y][MAX_tela_X] = {0};

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

void tocar_som(char *arquivo_som) {
    char comando[150];
    snprintf(comando, sizeof(comando), "afplay '%s' &", arquivo_som); 
    system(comando);
}

void limpar() {
    system("clear");
}

void tela() {
    printf("%*sPontos: %d\n", MAX_margem + 50, "", ponto);
    for (int i = 0; i < MAX_tela_y; i++) {
        for (int k = 0; k < MAX_margem; k++) {
            printf("  ");
        }
        for (int j = 0; j < MAX_tela_X; j++) {
            if (j == 0 || j == MAX_tela_X - 1 || i == MAX_tela_y - 1 || i == 0) {
                printf("\033[35m#\033[0m");
            } else if (imagem[i][j] == forma_cabeca) {
                printf("\033[32m%c\033[0m", forma_cabeca);
            } else if (imagem[i][j] == forma_corpo) {
                printf("\033[32m%c\033[0m", forma_corpo);
            } else if (imagem[i][j] == forma_comida) {
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
    imagem[cabeca.y][cabeca.x] = forma_cabeca;
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
    imagem[comida.y][comida.x] = forma_comida;
}

void  configuracoes_iniciais(){
    inicia_cobra();
    inicia_comida();
}

void tela_inicial() {
    do {
        limpar();
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s                  jogo da cobrinha                    \n", MAX_margem + 45, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s  Pontuação máxima: %d\n", MAX_margem + 45, "", pontuacao_maxima);
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        printf("%*s  Pressione 's' para iniciar o jogo\n", MAX_margem + 45, "");
        printf("%*s  Pressione 'i' para ver as instruções\n", MAX_margem + 45, "");
        printf("%*s  Pressione 'q' para sair\n", MAX_margem + 45, "");
        printf("%*s-------------------------------------------------\n", MAX_margem + 45, "");
        char opcao = getch();
        if (opcao == 's') {
            configuracoes_iniciais();
            break; 
        } else if (opcao == 'i') {
             limpar();
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s           Instruções do Jogo da Cobrinha\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Use as setas para mover a cobra ou w,s,a,d.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Coma as @ para crescer e ganhar pontos.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  Evite bater nas paredes ou no corpo da cobra.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s  A cada comida, a cobra cresce e a pontuação aumenta.\n", MAX_margem + 45, "");
            printf("%*s------------------------------------------------------\n", MAX_margem + 45, "");
            printf("%*s   Pressione qualquer tecla para voltar ao menu.\n", MAX_margem + 45, "");
            getch(); 
        } else if (opcao == 'q') {
            exit(0);
        }
    } while (1);
}

void reniciar(){

    for (int i = 0; i < tamanho_corpo; i++) {
        imagem[corpo[i].y][corpo[i].x] = ' ';
    }
    ponto=0;
    tamanho_corpo=0;
    comida.ativo=0;
    imagem[comida.y][comida.x] = ' '; 
}

void tela_game_over() {
     tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/death-sound.mp3");
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

void colisa_comida() {
    if (comida.ativo && cabeca.x == comida.x && cabeca.y == comida.y) {
        ponto += 10;
        comida.ativo = 0;
        imagem[comida.y][comida.x] = ' ';
        tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/food_G1U6tlb.mp3");
        inicia_comida();
        
       
        if (tamanho_corpo < MAX_CORPO) {
            tamanho_corpo++;
        }
    }
    if(pontuacao_maxima<ponto){
        pontuacao_maxima=ponto;
    }
}

void colisao_tela() {
    if (cabeca.x == MAX_tela_X - 1 || cabeca.x == 0 || cabeca.y == MAX_tela_y - 1 || cabeca.y == 0) {
         tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/hit-by-a-wood-230542.mp3");
       tela_game_over();
    }
}

void colisao_corpo() {
    for (int i = 0; i < tamanho_corpo; i++) {
        if (cabeca.x == corpo[i].x && cabeca.y == corpo[i].y) {
              tocar_som("/Users/user01/Documents/GitHub/programacao1/jogo/snake/sons/hit-by-a-wood-230542.mp3");
           tela_game_over();
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
        imagem[corpo[tamanho_corpo - 1].y][corpo[tamanho_corpo - 1].x] = ' ';
    }

   
    for (int i = tamanho_corpo - 1; i > 0; i--) {
        corpo[i] = corpo[i - 1];
    }

   
    if (tamanho_corpo > 0) {
        corpo[0].x = cabeca.x;
        corpo[0].y = cabeca.y;
    }

    imagem[cabeca.y][cabeca.x] = ' ';

    if (kbhit()) {
        char move = seta();
        if ((move == 'a' || move == 'A')&& direcao_x == 0) { direcao_x = -1; direcao_y = 0; }
        if ((move == 'd' || move == 'D') && direcao_x == 0) { direcao_x = 1; direcao_y = 0; }
        if ((move == 'w' || move == 'W') && direcao_y == 0) { direcao_x = 0; direcao_y = -1; }
        if ((move == 's' || move == 'S') && direcao_y == 0) { direcao_x = 0; direcao_y = 1; }
    }

    cabeca.x += direcao_x;
    cabeca.y += direcao_y;
    imagem[cabeca.y][cabeca.x] = forma_cabeca;

    
    for (int i = 0; i < tamanho_corpo; i++) {
        imagem[corpo[i].y][corpo[i].x] = forma_corpo;
    }
}

int main() {
    srand(time(NULL));
    carregar_pontuacao_maxima();
  tela_inicial();
    while (1) {
        limpar();
        tela();
        colisa_comida();
        mover_cobra();
        colisao_tela();
        colisao_corpo();
        usleep(ATRASO_TIQUE-(ponto*100));
    }
}