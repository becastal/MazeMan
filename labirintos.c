#include "labirintos.h"
#include <stdlib.h>
#include <time.h>

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
