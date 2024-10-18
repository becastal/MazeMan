#include "labirintos.h"
#include <stdlib.h>
#include <time.h>

int di_2[] = {-2, 2, 0, 0};
int dj_2[] = {0, 0, 2, -2};
int di_1[] = {-1, 1, 0, 0};
int dj_1[] = {0, 0, 1, -1};

void gera_labirinto() {
	puts("[+] novo labirinto!");
	labirinto novo_labirinto;

	// recebendo a quantidade de linhas, que deve ser um inteiro >= 1
	int ok_linhas = 0;
	while (!ok_linhas) {
		printf("[?] indique a quantidade de linhas: ");
		scanf("%d", &novo_labirinto.linhas);

		ok_linhas = (novo_labirinto.linhas >= 1);
		if (!ok_linhas) puts("[e] informe um valor inteiro >= 1!");
	}

	// recebendo a quantidade de colunas, que deve ser um inteiro >= 1
	int ok_colunas = 0;
	while (!ok_colunas) {
		printf("[?] indique a quantidade de colunas: ");
		scanf("%d", &novo_labirinto.colunas);

		ok_colunas = (novo_labirinto.colunas >= 1);
		if (!ok_colunas) puts("[e] informe um valor inteiro >= 1!");
	}

	// construindo de fato o labirinto inicial, um pouco de matematica faz o labirinto inicialmente ter
	// o formato: ####### com a quantidade de celunas nas linhas e nas colunas conforme especificado.
	//            # # # #
	//            #######
	//            # # # #
	//            ####### ...
	
	novo_labirinto.linhas = 2 * novo_labirinto.linhas + 1;
	novo_labirinto.colunas = 2 * novo_labirinto.colunas + 1;
	novo_labirinto.celulas = (char**) malloc(novo_labirinto.linhas * sizeof(char*));
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		novo_labirinto.celulas[i] = (char*) malloc(novo_labirinto.colunas * sizeof(char));
	}

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		for (int j = 0; j < novo_labirinto.colunas; j++) {
			novo_labirinto.celulas[i][j] = (i & 1 && j & 1 ? ' ' : '#');	
			// pro formato de xadrez apresentado, as coordenadas das linhas e das colunas devem ser ambas
			// impares. x & 1 eh a mesma coisa que x % 2 == 1 mas mais maneiro.
		}
	}

	puts("[!] possiveis algoritmos para gerar o labirinto");
	puts("[0] algoritmo da binary tree.");
	puts("[1] algoritmo de sidewinder.");
	puts("[2] algoritmo de aldous-border.");
	puts("[3] algoritmo de hunt-and-kill.");
	puts("[4] algoritmo de backtracking.");

	int ok_algoritmo = 0;
	while (!ok_algoritmo) {
		printf("[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);

		ok_algoritmo = 1;
		switch (selecao) {
			case 0:
				algoritmo_binary_tree(&novo_labirinto);
				break;	
			case 1:
				algoritmo_sidewinder(&novo_labirinto);
				break;	
			case 2:
				algoritmo_aldous_border(&novo_labirinto);
				break;	
			case 3:
				algoritmo_hunt_and_kill(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}

	printa_labirinto(novo_labirinto);
	
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		free(novo_labirinto.celulas[i]);
	}
	free(novo_labirinto.celulas);
}

void printa_labirinto(labirinto L) {
	for (int i = 0; i < L.linhas; i++) {
		for (int j = 0; j < L.colunas; j++) {
			printf("%c", L.celulas[i][j]);
		}
		printf("\n");
	}
}

void algoritmo_binary_tree(labirinto* L) {
	// pra cada posicao da matriz, tira ou a parede da direita ou a parede de baixo;
	// complexidade O(nm);
	srand(time(NULL));

	for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
		for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
			int libera_direita = rand() & 1;

			if (posicao_linha == L->linhas - 2) libera_direita = 1;
			if (posicao_coluna == L->colunas - 2) libera_direita = 0;
			if (posicao_linha == L->linhas - 2 && posicao_coluna == L->colunas - 2) break;

			if (libera_direita) {
				L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
			} else {
				L->celulas[posicao_linha + 1][posicao_coluna] = ' ';
			}
		}
	}
}

void algoritmo_sidewinder(labirinto* L) {
	// pra cada posicao da matriz, tira ou a parede de cima ou a parede da direita, mas quando existe uma mudanca
	// na sequencia (por exemplo, lado -> lado -> direita), ele nao corta a direita da posicao que eu to, mas sim
	// uma aleatoria do componente conexo anterior;
	// complexidade O(nm);
	srand(time(NULL));
	
    for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
        int inicio_run = 1;
        
        for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
            int libera_direita = rand() & 1;
            
            if (posicao_coluna == L->colunas - 2) libera_direita = 0;
            if (posicao_linha == 1) libera_direita = 1;
			if (posicao_linha == 1 && posicao_coluna == L->colunas - 2) break;

            if (libera_direita) {
                L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
            } else {
                int posicao_norte = inicio_run + 2 * (rand() % ((posicao_coluna - inicio_run) / 2 + 1));
                L->celulas[posicao_linha - 1][posicao_norte] = ' ';
                inicio_run = posicao_coluna + 2;
            }
        }
    }
}

int posicao_valida(labirinto* L, int i, int j) {
    return i >= 1 && j >= 1 && i < L->linhas - 1 && j < L->colunas - 1;
}

int posicao_aleatoria(labirinto* L, int tipo) {
    int min = 1, max = (tipo ? L->linhas - 2 : L->colunas - 2);
    return min + 2 * (rand() % ((max - min) / 2 + 1));
}

void algoritmo_aldous_border(labirinto* L) {
	// pra uma posicao aleatoria, seguir escolhendo movimentos aleatorios ate ter visitado todos os vertices.
	// se voce entrar em uma posicao nao antes visitada, voce liga as duas celulas cortando a parede, se nao,
	// voce so segue. bizarro tentar achar a complexidade disso. existe um mundo que esse algoritmo nunca acaba;
	// complexidade: O(sei_la * ruim);
	// na real a complexidade media fica ~O(nmlogn);
	srand(time(NULL));

    int contador_visitados = 0;
    
    int** visitado = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        visitado[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            visitado[i][j] = 0;
            if (i % 2 == 1 && j % 2 == 1) {
                contador_visitados++;
            }
        }
    }

    int posicao_linha = posicao_aleatoria(L, 1);
    int posicao_coluna = posicao_aleatoria(L, 0);
    contador_visitados--;
    visitado[posicao_linha][posicao_coluna] = 1;

    while (contador_visitados > 0) {
        int movimento = rand() % 4;
        int nova_posicao_linha = posicao_linha + di_2[movimento];
        int nova_posicao_coluna = posicao_coluna + dj_2[movimento];

        if (!posicao_valida(L, nova_posicao_linha, nova_posicao_coluna)) {
            continue;
        }

        if (!visitado[nova_posicao_linha][nova_posicao_coluna]) {
            L->celulas[(posicao_linha + nova_posicao_linha) / 2][(posicao_coluna + nova_posicao_coluna) / 2] = ' ';
            visitado[nova_posicao_linha][nova_posicao_coluna] = 1;
            contador_visitados--;
        }

        posicao_linha = nova_posicao_linha;
        posicao_coluna = nova_posicao_coluna;
    }

    for (int i = 0; i < L->linhas; i++) {
        free(visitado[i]);
    }
    free(visitado);
}

void algoritmo_hunt_and_kill(labirinto* L) {
	// bem proximo do aldous border, mas ele evita entrar nas que ele ja viu,
	// buscando uma nao vista antes e recomecando a busca.
	// complexidade: O(nm);
	srand(time(NULL));

    int contador_visitados = 0;
    
    int** visitado = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        visitado[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            visitado[i][j] = 0;
            if (i % 2 == 1 && j % 2 == 1) {
                contador_visitados++;
            }
        }
    }

    int posicao_linha = posicao_aleatoria(L, 1);
    int posicao_coluna = posicao_aleatoria(L, 0);
    contador_visitados--;
    visitado[posicao_linha][posicao_coluna] = 1;

	int direcoes_vistas = 0; 
    while (contador_visitados > 0) {
        int movimento = rand() % 4;
        int nova_posicao_linha = posicao_linha + di_2[movimento];
        int nova_posicao_coluna = posicao_coluna + dj_2[movimento];

		int hunt_ok = 0;
		if (direcoes_vistas == 15) {
			// todos os bits de direcoes vistas sao 1. ou seja, ja vi todos os lados e
			// nao consegui sair. esse numero porque 15 == (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3).
			for (int i = 1; i < L->linhas - 1 && !hunt_ok; i += 2) {
				for (int j = 1; j < L->colunas - 1 && !hunt_ok; j += 2) {
					if (!visitado[i][j]) {
						for (int d = 0; d < 4; d++) {
							int ni = i + di_2[d];
							int nj = j + dj_2[d];
							if (posicao_valida(L, ni, nj) && visitado[ni][nj]) {
								posicao_linha = i;
								posicao_coluna = j;
								L->celulas[(i + ni) / 2][(j + nj) / 2] = ' ';
								hunt_ok = 1;
								break;
							}
						}
					}
				}
				if (hunt_ok) break;
			}
		}

		if (hunt_ok) {
			visitado[posicao_linha][posicao_coluna] = 1;
			contador_visitados--;
			hunt_ok = 0;
			direcoes_vistas = 0;
			continue;
		}

        if (!posicao_valida(L, nova_posicao_linha, nova_posicao_coluna) ||
			visitado[nova_posicao_linha][nova_posicao_coluna]) {
			direcoes_vistas |= (1 << movimento); // setando o bit do movimento que vi do movimento que vi
            continue;
        }

        if (!visitado[nova_posicao_linha][nova_posicao_coluna]) {
            L->celulas[(posicao_linha + nova_posicao_linha) / 2][(posicao_coluna + nova_posicao_coluna) / 2] = ' ';
            visitado[nova_posicao_linha][nova_posicao_coluna] = 1;
            contador_visitados--;
        }

        posicao_linha = nova_posicao_linha;
        posicao_coluna = nova_posicao_coluna;
    }

	for (int i = 0; i < L->linhas; i++) {
		free(visitado[i]);
	}
	free(visitado);
}
