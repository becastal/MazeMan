#include "labirintos.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
#include <unistd.h>
	#include <termios.h>
	#include <fcntl.h>
#endif

typedef struct {
    int direcao_olhando;
    int posicao_linha;
    int posicao_coluna;
} Mazeman;

typedef struct {
    int direcao_olhando;
    int posicao_linha;
    int posicao_coluna;
} Fantasma;

int mazeman_game_loop(labirinto *L);
int mazeman_obter_input();
int mazeman_obter_direcao(char input, Mazeman pac);
void mazeman_spawn(labirinto L, Mazeman *pac);
int mazeman_movimento_valido(labirinto L, Mazeman pac, int direcao);
void mazeman_move(Mazeman *pac, int direcao);
void mazeman_fantasma_spawn(labirinto L, Mazeman pac, Fantasma *f);
void mazeman_fantasma_move(labirinto L, Fantasma *fant);
void mazeman_atualizar_print(int i, int j, char conteudo);
void mazeman_atualizar_mapa(labirinto *L, Mazeman *pac, Fantasma *f, int move_acao, int *pontos_jogador);


