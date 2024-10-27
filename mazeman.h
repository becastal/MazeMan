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