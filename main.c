#include <stdio.h>
#include "labirintos.h"
#include "mazeman.h"

int main() {
	int sair = 0;
	
	while (!sair) {
		system("@cls||clear");
		puts("[+] menu inicial.");
		puts("[0] sair.");
		puts("[1] criar labiririntos.");
		puts("[2] resolver labirintos.");
		puts("[3] jogar mazeman.");

		printf("\n[?] selecione uma das opcoes: ");
		int selecao;
		scanf("%d", &selecao);
		switch (selecao) {
			case 0:
				sair = 1;
				break;
			case 1:
				system("@cls||clear");
				gera_labirinto();
				break;
			case 2:
				system("@cls||clear");
				resolve_labirinto();
				break;
			case 3:
				system("@cls||clear");
				mazeman_inicio();
				break;
			default:
				puts("[e] selecao invalida!");
		}
	}

	return 0;
}
