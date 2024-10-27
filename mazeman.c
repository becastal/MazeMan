#include "mazeman.h"
#include "labirintos.h"

// Loop do jogo do mazeman
int mazeman_game_loop(labirinto *L){
    Mazeman mazeman;
    Fantasma fantasmas[4];
    char input;
    int direcao;
    int move_acao;
    int fim_de_jogo;
    int pontos_jogador;

    pontos_jogador = 0;
    fim_de_jogo = 0;

    mazeman_spawn(*L, &mazeman);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[0]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[1]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[2]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[3]);

    system("cls");
    printa_labirinto(*L);
    // Jogo acaba quando move_acao é 2 (mazeman bate no fantasma)
    while(!fim_de_jogo){
        input = mazeman_obter_input();

        // Retorna da direcao para andar
        direcao = mazeman_obter_direcao(input, mazeman);
        // Obtem se o movimento é válido (1) ou se vai bater em fantasma (2) Caso inválido retorna 0
        move_acao = mazeman_movimento_valido(*L, mazeman, direcao);
        mazeman.direcao_olhando = direcao;

        mazeman_atualizar_mapa(L, &mazeman, fantasmas, move_acao, &pontos_jogador);
        // Checar colisao com fantasma para ser igual a fim de jogo

    #ifdef _WIN32
        Sleep(100);  // Windows
    #else
        usleep(100000);  // Linux
    #endif
    }
    mazeman_atualizar_print(L->linhas, 0, '\n');
    printf("Fim de jogo, voce morreu!\n");
    return pontos_jogador;
}

void mazeman_atualizar_mapa(labirinto *L, Mazeman *pac, Fantasma *f, int move_acao, int *pontos_jogador);

int mazeman_obter_input() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    int ch = getchar();
        if (ch != EOF) {
            return ch;
        }
#endif
    return -1;  // Nenhum input
}

void mazeman_spawn(labirinto L, Mazeman *maz){
    int maz_pos_i;
    int maz_pos_j;

    // Define uma direcao padrao pro mazeman
    maz->direcao_olhando = 3;

    // Obtem uma posicao valida para spawnar o mazeman
    while(1){
        maz_pos_i = posicao_aleatoria(&L, 1);
        maz_pos_j = posicao_aleatoria(&L, 0);
        if (L.celulas[maz_pos_i][maz_pos_j] == ' '){
            maz->posicao_linha = maz_pos_i;
            maz->posicao_coluna = maz_pos_j;
            break;
        }
    }
}

int mazeman_obter_direcao(char input, Mazeman maz){
    if (input == 'W' || input == 'w'){
        return 0;
    }
    else if (input == 'S' || input == 's'){
        return 1;
    }
    else if (input == 'D' || input == 'd'){
        return 2;
    }
    else if (input == 'A' || input == 'a'){
        return 3;
    }
    else{
        return maz.direcao_olhando;
    }
}

int mazeman_movimento_valido(labirinto L, Mazeman maz, int direcao){
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};

    // Posição atual do mazeman + a posicao do proximo frame movendo na direcao que o jogador escolheu
    int maz_i = maz.posicao_linha + di_1[direcao];
    int maz_j = maz.posicao_coluna + dj_1[direcao];

    // Checa se a posição está dentro do mapa, se não estiver retorna 0
    if (!posicao_valida(&L, maz_i, maz_j)) return 0;

    char posicao_final = L.celulas[maz_i][maz_j];

    // Checa se vai bater em uma parede
    if(posicao_final == '#'){
        return 0;
    }

    else{
        return 1;
    }

}

void mazeman_move(Mazeman *maz, int direcao){
    // NORTE, SUL, LESTE, OESTE EM ORDEM
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};

    maz->posicao_linha += di_1[direcao];
    maz->posicao_coluna += dj_1[direcao];
}

void mazeman_fantasma_spawn(labirinto L, Mazeman maz, Fantasma *f){
    int maz_pos_i;
    int maz_pos_j;
    int f_pos_i;
    int f_pos_j;
    int dif_i;
    int dif_j;

    maz_pos_i = maz.posicao_linha;
    maz_pos_j = maz.posicao_coluna;

    // Define uma direcao padrao pro fantasma
    f->direcao_olhando = 2;

    // Obtem uma posicao valida para spawnar o mazeman
    while(1){
        f_pos_i = posicao_aleatoria(&L, 1);
        f_pos_j = posicao_aleatoria(&L, 0);
        dif_i = abs(maz_pos_i - f_pos_i);
        dif_j = abs(maz_pos_j - f_pos_j);

        // Checar quadrado vazio e diferença de 5x5 do mazeman
        if (L.celulas[f_pos_i][f_pos_j] == ' ' && dif_i > 5 && dif_j > 5){
            f->posicao_linha = f_pos_i;
            f->posicao_coluna = f_pos_j;
            break;
        }
    }
}

void mazeman_fantasma_move(labirinto L, Fantasma *f){
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};
    int fant_i;
    int fant_j;
    char posicao_final;

    // Direção atual do fantasma
    int direcao = f->direcao_olhando;
    int direcao_permitida = 0; // Checar se precisa mudar ou não direcao

    while(!direcao_permitida){
        // Posição atual do mazeman + a posicao do proximo frame movendo na direcao que o jogador escolheu
        fant_i = f->posicao_linha + di_1[direcao];
        fant_j = f->posicao_coluna + dj_1[direcao];

        // Checa se a posição está dentro do mapa, coloca para mudar de direção
        if (!posicao_valida(&L, fant_i, fant_j)) direcao_permitida = 0;

        posicao_final = L.celulas[fant_i][fant_j];

        // Checa se vai bater em uma parede, se for coloca para mudar de direção
        if(posicao_final == '#') direcao_permitida = 0;
        else{
            break;
        }

        // Altera a direcao para uma direção aleatória
        direcao = rand() % 4;
    }

    f->posicao_linha = fant_i;
    f->posicao_coluna = fant_j;
    f->direcao_olhando = direcao;

    // printf("%d, %d\n", fant->posicao_linha, fant->posicao_coluna);
}

void mazeman_atualizar_print(int i, int j, char conteudo){
    printf("\033[%d;%dH%c", i+1, j+1, conteudo);
}

int mazeman_checar_colisao_fantasma(labirinto L, Fantasma *f, Mazeman maz){
    int i;

    for(i = 0; i < 4; i++){
        // Checa se o mazeman colidiu com um fantasma
        if (L.celulas[f[i].posicao_linha][f[i].posicao_coluna] == '#'){
            return 0;
        }
        else if(f[i].posicao_linha == maz.posicao_linha && f[i].posicao_coluna == maz.posicao_coluna){
            return 1;
        }
        else if(f[i].posicao_linha+1 == maz.posicao_linha && f[i].posicao_coluna == maz.posicao_coluna){
            return 1;
        }
        else if(f[i].posicao_linha == maz.posicao_linha && f[i].posicao_coluna+1 == maz.posicao_coluna){
            return 1;
        }
        else if(f[i+1].posicao_linha+1 == maz.posicao_linha && f[i].posicao_coluna+1 == maz.posicao_coluna){
            return 1;
        }
    }
    return 0;
}
