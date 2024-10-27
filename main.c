#include <stdio.h>
#include "labirintos.h"
#include "arquivos.h"
#include "mazeman.h"

int main() {
	int sair = 0;
	
	while (!sair) {
		puts("[+] menu inicial.");
		puts("[0] sair.");
		puts("[1] criar labiririntos.");
		puts("[2] resolver labirintos.");
		puts("[3] resolver caminhos.");
		puts("[4] jogar MazeMan.");

		printf("\n[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);
		switch (selecao) {
			case 0:
				sair = 1;
				break;
			case 1:
				gera_labirinto();
				break;
			case 2:
				resolve_labirinto();
				break;
			case 4:
				labirinto L;
				L = escolhe_labirinto();
				int pontos = mazeman_game_loop(&L);
				printf("[i] pontos: %d\n", pontos);
				break;
			default:
				puts("[e] selecao invalida!");
		}
	}

	return 0;
}
