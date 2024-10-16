#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>

typedef struct {
	int linhas, colunas;
	char** celulas;
} labirinto;

void gera_labirinto();
void printa_labirinto(labirinto L);
int posicao_valida(labirinto* L, int i, int j);
int posicao_aleatoria(labirinto* L, int tipo);
void algoritmo_binary_tree(labirinto* L);
void algoritmo_sidewinder(labirinto* L);
void algoritmo_aldous_border(labirinto* L);
void algoritmo_hunt_and_kill(labirinto* L);
void algoritmo_backtracking(labirinto* L);
void backtracking(int** visitado, labirinto* L, int linha, int coluna); 
void pacmaniza(labirinto* L);

#endif
