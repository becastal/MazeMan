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

    // Spawnar mazeman
    // spawnar fantasmas

    system("cls");
    printa_labirinto(*L);
    // Jogo acaba quando move_acao é 2 (mazeman bate no fantasma)
    while(!fim_de_jogo){
        // Pegar input

        // Obter direcao para andar
        // Obter movimento válido
        mazeman.direcao_olhando = direcao;

        // Atualizar coisas no mapa

        // Checar fim de jogo

    #ifdef _WIN32
        Sleep(100);  // Windows
    #else
        usleep(100000);  // Linux
    #endif
    }
    printf("Fim de jogo, voce morreu!\n");
    return pontos_jogador;
}

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
