#include "labirintos.h"
#include <stdlib.h>


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
	puts("[1] algoritmo de sinewinder.");
	puts("[2] algoritmo de aldous-border.");
	puts("[3] algoritmo de hunt-and-kill.");
	puts("[4] algoritmo de backtracking.");

	int ok_algoritmo = 1;
	while (!ok_algoritmo) {
		printf("[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);

		ok_algoritmo = 1;
		switch (selecao) {
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}
	
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		free(novo_labirinto.celulas[i]);
	}
	free(novo_labirinto.celulas);
}
