#ifndef arquivos_h
#define arquivos_h
#include <stdio.h>
#include "labirintos.h"

void salva_labirinto(labirinto* L);
labirinto escolhe_labirinto();
void le_labirinto(labirinto* L, char* nome);

#endif
