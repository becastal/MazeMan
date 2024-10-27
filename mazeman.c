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
