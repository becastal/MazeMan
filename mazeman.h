#include "labirintos.h"

#include <unistd.h>
#include <string.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
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

void mazeman_inicio();
int mazeman_game_loop(labirinto *L);
int mazeman_obter_input();
int mazeman_obter_direcao(char input, Mazeman pac);
void mazeman_spawn(labirinto L, Mazeman *pac);
int mazeman_movimento_valido(labirinto L, Mazeman pac, int direcao);
void mazeman_move(Mazeman *pac, int direcao);
void mazeman_fantasma_spawn(labirinto L, Mazeman pac, Fantasma *f);
void mazeman_fantasma_move(labirinto L, Fantasma *fant);
void mazeman_atualizar_print(int i, int j, char conteudo);
void mazeman_atualizar_mapa(labirinto *L, Mazeman *pac, Fantasma *f, int move_acao, int *pontos_jogador, int** tem_ponto);
int mazeman_checar_colisao_fantasma(labirinto L, Fantasma *f, Mazeman pac);
void mazeman_pontuacoes();
void mazeman_salvar_pontuacao(int pontos, labirinto* L);
void non_blocking_input_linux();
void restore_input_linux();
