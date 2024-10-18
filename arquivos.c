#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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
labirinto escolhe_labirinto() {
    struct dirent *entrada;
    DIR *pasta;
    int quantidade_arquivos = 0;

    pasta = opendir("./salvos/");
    while ((entrada = readdir(pasta)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            quantidade_arquivos++;
        }
    }
    closedir(pasta);

    if (quantidade_arquivos == 0) {
        printf("[i] Nenhum arquivo encontrado.\n");
        exit(1);
    }

    char **nomes_arquivos = malloc(sizeof(char*) * quantidade_arquivos);

    pasta = opendir("./salvos/");
    int i = 0;
    while ((entrada = readdir(pasta)) != NULL) {
        if (strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
            nomes_arquivos[i] = malloc(strlen(entrada->d_name) + 1);
            strcpy(nomes_arquivos[i], entrada->d_name);
            i++;
        }
    }
    closedir(pasta);

    printf("[i] arquivos encontrados:\n");
    for (int i = 0; i < quantidade_arquivos; i++) {
        printf("[%02d] %s\n", i + 1, nomes_arquivos[i]);
    }

    int escolha = -1;
    char arquivo_escolhido[100];
    int ok_nome = 0;
    while (!ok_nome) {
        printf("[?] informe o indice do arquivo escolhido: ");
        scanf("%d", &escolha);

		ok_nome = escolha >= 1 && escolha <= quantidade_arquivos;
		if (!ok_nome) puts("[e] escolhe invalida!");
    }
	strcpy(arquivo_escolhido, nomes_arquivos[escolha - 1]);

    for (int i = 0; i < quantidade_arquivos; i++) {
        free(nomes_arquivos[i]);
    }
    free(nomes_arquivos);

	printf("[i] arquivo escolhido: %s\n", arquivo_escolhido);

	labirinto L;
	le_labirinto(&L, arquivo_escolhido);
	printa_labirinto(L);

    return L;
}
void le_labirinto(labirinto* L, char* nome) {
    char caminho[100];
    snprintf(caminho, sizeof(caminho), "./salvos/%s", nome);

    FILE* arquivo = fopen(caminho, "r");
    fscanf(arquivo, "%d %d\n", &L->linhas, &L->colunas);
    L->celulas = (char**) malloc(L->linhas * sizeof(char*));

    for (int i = 0; i < L->linhas; i++) {
        L->celulas[i] = (char*) malloc(L->colunas * sizeof(char));
        for (int j = 0; j < L->colunas; j++) {
            fscanf(arquivo, "%c", &L->celulas[i][j]);
        }
		fgetc(arquivo);
    }

    fclose(arquivo);
}
/*
void salvar_bin(labirinto* L){
    FILE* arq = fopen("usuarios/usuario.bin", "wb");
    fwrite(nome[], sizeof(nome),1,arq);
    fwrite(pontuacao, sizeof(int),1,arq);
}

void ler_bin(labirinto* L){
    FILE* arq = fopen("usuarios/usuario.bin", "rb");
    fread(nome[], sizeof(nome),1,arq);
    fread(pontuacao, sizeof(int),1,arq);
}   
*/
