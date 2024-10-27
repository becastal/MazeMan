#ifndef arquivos_h
#define arquivos_h
#include <stdio.h>
#include "labirintos.h"

typedef struct {
	char nome_usuario[100];
	char nome_mapa[100];
	int pontos;
} pontuacao;

void salva_labirinto(labirinto* L);
labirinto escolhe_labirinto();
void le_labirinto(labirinto* L, char* nome);
pontuacao* le_pontuacoes(int* quantidade_pontuacoes);
void salva_pontuacoes(pontuacao* P, int quantidade_pontuacoes);

#endif
