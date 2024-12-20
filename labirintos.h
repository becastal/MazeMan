#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
    #include "windows.h"
#else
    #include <termios.h>
#endif

extern int di_2[];
extern int dj_2[];
extern int di_1[];
extern int dj_1[];
extern char* parede[];

typedef struct {
	char nome[100];
	int linhas, colunas;
	char** celulas;
	int** ordem_construcao;
	int contagem_construcao;
} labirinto;

void printa_labirinto(labirinto L);
int posicao_valida(labirinto* L, int i, int j);
int posicao_aleatoria(labirinto* L, int tipo);
char* caracter_parede(labirinto* L, int i, int j);

void gera_labirinto();
void algoritmo_binary_tree(labirinto* L);
void algoritmo_sidewinder(labirinto* L);
void algoritmo_aldous_border(labirinto* L);
void algoritmo_hunt_and_kill(labirinto* L);
void algoritmo_backtracking(labirinto* L);
void backtracking(int** visitado, labirinto* L, int linha, int coluna); 
void pacmaniza(labirinto* L);
void ver_construcao(labirinto* L);

void esperar_enter(int tipo);
void resolve_labirinto();
void resolve_dfs(int** distancia, labirinto* L, int i, int j);
void ver_resolucao(labirinto* L, int** distancia, int inicio_i, int inicio_j, int fim_i, int fim_j);

#endif
