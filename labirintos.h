#ifndef labirintos_h
#define labirintos_h
#include <stdio.h>

typedef struct {
	int linhas, colunas;
	char** celulas;
} labirinto;

void gera_labirinto();
void printa_labirinto(labirinto L);
void algoritmo_binary_tree(labirinto* L);

#endif
