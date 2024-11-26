# ARCADE

https://github.com/user-attachments/assets/b15870da-278a-42f6-8408-42a1ca427ed6



## Descrição

Este projeto implementa um **arcade de jogos em C** que funciona no terminal. O programa permite que o jogador insira um nickname, escolha entre diferentes jogos e veja um ranking dos melhores jogadores com base na pontuação acumulada.

---

## Funcionalidades

- **Interface no Terminal:**
  - Menus com uma interface visual simples e navegação por teclado (`w`, `s` para mover, `Enter` para selecionar).

- **Jogos Disponíveis:**
  - **Snake:** Um clássico onde o jogador guia a cobra para comer alimentos sem bater nas paredes ou no próprio corpo.
  - **Flappy Bird:** Controle um pássaro que precisa passar por obstáculos sem cair ou colidir.
  - **Space Invaders:** Atire em invasores alienígenas e proteja seu território.

- **Ranking de Pontuações:**
  - Armazena até 10 jogadores com as melhores pontuações.
  - Classifica automaticamente o ranking de forma decrescente.
  - Permite visualizar o ranking na tela inicial.

- **Jogabilidade:**
  - Cada jogo contribui para a pontuação acumulada do jogador.
  - Atualização automática de pontuações ao final dos jogos.

- **Controle de Estados:**
  - O programa é baseado em uma máquina de estados, com as seguintes telas principais:
    - **Menu Principal:** Oferece opções para visualizar o ranking, selecionar jogos ou sair.
    - **Seleção de Jogos:** Permite escolher entre os jogos disponíveis.
    - **Jogo Ativo:** Executa o jogo selecionado.
    - **Sair:** Finaliza o arcade.

---

## Como Funciona

### Estrutura do Código

1. **Arquivos de Implementação:**
   - `arcade.c`: Gerencia o menu principal, seleção de jogos e interação do usuário.
   - `snake.c`, `flappy_bird.c`, `space_invaders.c`: Implementam a lógica dos jogos.
   - `utils.c`: Contém funções utilitárias como manipulação de entrada e limpeza de tela.

2. **Player e Ranking:**
   - Um jogador é representado por uma estrutura `Player` com `nickname` (nome) e `score` (pontuação).
   - As pontuações são armazenadas em um array e ordenadas dinamicamente.

3. **Controles:**
   - **`w` e `s`:** Mover o cursor entre as opções.
   - **`Enter`:** Confirmar a seleção.
   - Após o término de um jogo, a pontuação é atualizada e o jogador pode continuar ou retornar ao menu principal.

4. **Menus e Telas:**
   - Menus são desenhados com bordas e opções destacadas para facilitar a navegação.
   - Cada tela é redesenhada com base no estado atual do programa.

---

## Execução

### Passos para Rodar o Programa

1. **Compile o código com o comando:**
   ```bash
   g++ arcade.c snake.c utils.c flappy_bird.c space_invaders.c -o ARCADE \
   -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -lSDL2_mixer
