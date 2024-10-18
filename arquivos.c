#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "labirintos.h"

void salva_labirinto(labirinto* L) {
    char nome_arquivo[100];
    char nome_completo[120];
    FILE* arquivo;
    int ok_nome = 0;

    while (!ok_nome) {
        printf("[?] informe o nome do arquivo: ");
        scanf("%99s", nome_arquivo);

        int tamanho = snprintf(nome_completo, sizeof(nome_completo), "./salvos/%s.txt", nome_arquivo);

        arquivo = fopen(nome_completo, "w");
		ok_nome = arquivo != NULL;

		if (!ok_nome) puts("[e] informe um nome valido!");
    }

	fprintf(arquivo, "%d %d\n", L->linhas, L->colunas);
	for (int i = 0; i < L->linhas; i++) {
		for (int j = 0; j < L->colunas; j++) {
			fprintf(arquivo, "%c%s", L->celulas[i][j], (j != L->colunas - 1 ? "" : "\n"));
		}
	}

    fclose(arquivo);
    printf("[i] labirinto salvo em: %s\n", nome_completo);
}
