#include "labirintos.h"
#include "arquivos.h"
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

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
	novo_labirinto.ordem_construcao = (int**) malloc(novo_labirinto.linhas * sizeof(int*));
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		novo_labirinto.celulas[i] = (char*) malloc(novo_labirinto.colunas * sizeof(char));
		novo_labirinto.ordem_construcao[i] = (int*) malloc(novo_labirinto.colunas * sizeof(int));

		for (int j = 0; j < novo_labirinto.colunas; j++) {
			novo_labirinto.ordem_construcao[i][j] = -1;
		}
	}

	for (int i = 0; i < novo_labirinto.linhas; i++) {
		for (int j = 0; j < novo_labirinto.colunas; j++) {
			novo_labirinto.celulas[i][j] = (i & 1 && j & 1 ? ' ' : '#');	
			// pro formato de xadrez apresentado, as coordenadas das linhas e das colunas devem ser ambas
			// impares. x & 1 eh a mesma coisa que x % 2 == 1 mas mais maneiro.
		}
	}

	novo_labirinto.celulas[0][1] = ' ';
	novo_labirinto.celulas[0][novo_labirinto.colunas-2] = ' ';
	novo_labirinto.celulas[novo_labirinto.linhas-1][1] = ' ';
	novo_labirinto.celulas[novo_labirinto.linhas-1][novo_labirinto.colunas-2] = ' ';


	puts("[!] possiveis algoritmos para gerar o labirinto");
	puts("[0] algoritmo da binary tree.");
	puts("[1] algoritmo de sidewinder.");
	puts("[2] algoritmo de aldous-border.");
	puts("[3] algoritmo de hunt-and-kill.");
	puts("[4] algoritmo de backtracking.");
	puts("[5] algoritmo de pacmaniza");

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
			case 4:
				algoritmo_backtracking(&novo_labirinto);
				break;
			case 5:
				// caso em que é criado um labirinto e colocado decente para o mazeman
				algoritmo_backtracking(&novo_labirinto);
				pacmaniza(&novo_labirinto);
				break;
			default:
				puts("[e] selecao invalida!");
				ok_algoritmo = 0;
		}
	}

	ver_construcao(&novo_labirinto);

	char selecao_salvar;
	int ok_salvar = 0;

	while (!ok_salvar) {
		printf("[?] salvar arquivo (s/n)? ");
		scanf(" %c", &selecao_salvar);

		ok_salvar = (selecao_salvar == 's' || selecao_salvar == 'S' ||
					 selecao_salvar == 'n' || selecao_salvar == 'N');
		if (!ok_salvar) puts("[e] informe 's' ou 'n'.");
	}

	if (selecao_salvar == 's' || selecao_salvar == 'S') {
		salva_labirinto(&novo_labirinto);
	}

	printa_labirinto(novo_labirinto);
	
	for (int i = 0; i < novo_labirinto.linhas; i++) {
		free(novo_labirinto.celulas[i]);
		free(novo_labirinto.ordem_construcao[i]);
	}
	free(novo_labirinto.celulas);
	free(novo_labirinto.ordem_construcao);
}

void printa_labirinto(labirinto L) {
	for (int i = 0; i < L.linhas; i++) {
		for (int j = 0; j < L.colunas; j++) {
			printf("%c", L.celulas[i][j]);
		}
		printf("\n");
	}
}

int posicao_valida(labirinto* L, int i, int j) {
	// retorna 1 se i e j estiverem dentro dos limite do labirinto.
    return (i >= 0 && i < L->linhas && j >= 0 && j < L->colunas);
}

int posicao_aleatoria(labirinto* L, int tipo) {
	// retorna uma "posicao impar" aleatoria em um labirinto. tipo deve ser 1 para retornar uma linha impar 
	// e 0 para retornar uma coluna impar aleatoria.
	//
	// "posicao impar" siginifica uma posicao entre as paredes, mas nao incluinto o caminho entre elas
	// ######### 
	// #a# # b #
	// # #   # # nesse exemplo, 'a' eh uma "posicao impar" que essa funcao pode retornar e 'b' nao.
	// #   # # #
	// #########

    int min = 1, max = (tipo ? L->linhas - 2 : L->colunas - 2);
    return min + 2 * (rand() % ((max - min) / 2 + 1));
}

void algoritmo_binary_tree(labirinto* L) {
	// pra cada posicao da matriz, tira ou a parede da direita ou a parede de baixo;
	// complexidade O(nm);
	srand(time(NULL));

	L->contagem_construcao = 0;
	for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
		for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
			L->ordem_construcao[posicao_linha][posicao_coluna] = L->contagem_construcao++;

			int libera_direita = rand() & 1;

			if (posicao_linha == L->linhas - 2) libera_direita = 1;
			if (posicao_coluna == L->colunas - 2) libera_direita = 0;
			if (posicao_linha == L->linhas - 2 && posicao_coluna == L->colunas - 2) break;

			if (libera_direita) {
				L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
				L->ordem_construcao[posicao_linha][posicao_coluna + 1] = L->contagem_construcao++;
			} else {
				L->celulas[posicao_linha + 1][posicao_coluna] = ' ';
				L->ordem_construcao[posicao_linha + 1][posicao_coluna] = L->contagem_construcao++;
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
	
	L->contagem_construcao = 0;
    for (int posicao_linha = 1; posicao_linha < L->linhas; posicao_linha += 2) {
        int inicio_run = 1;
        
        for (int posicao_coluna = 1; posicao_coluna < L->colunas; posicao_coluna += 2) {
            int libera_direita = rand() & 1;

			L->ordem_construcao[posicao_linha][posicao_coluna] = L->contagem_construcao++;
            
            if (posicao_coluna == L->colunas - 2) libera_direita = 0;
            if (posicao_linha == 1) libera_direita = 1;
			if (posicao_linha == 1 && posicao_coluna == L->colunas - 2) break;

            if (libera_direita) {
                L->celulas[posicao_linha][posicao_coluna + 1] = ' ';
                L->ordem_construcao[posicao_linha][posicao_coluna + 1] = L->contagem_construcao++;
            } else {
                int posicao_norte = inicio_run + 2 * (rand() % ((posicao_coluna - inicio_run) / 2 + 1));
                L->celulas[posicao_linha - 1][posicao_norte] = ' ';
                L->ordem_construcao[posicao_linha - 1][posicao_norte] = L->contagem_construcao++;
                inicio_run = posicao_coluna + 2;
            }
        }
    }
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

	L->contagem_construcao = 0;
    while (contador_visitados > 0) {
        int movimento = rand() % 4;
        int nova_posicao_linha = posicao_linha + di_2[movimento];
        int nova_posicao_coluna = posicao_coluna + dj_2[movimento];

        if (!posicao_valida(L, nova_posicao_linha, nova_posicao_coluna)) {
            continue;
        }

		if (L->ordem_construcao[posicao_linha][posicao_coluna] == -1) {
			L->ordem_construcao[posicao_linha][posicao_coluna] = L->contagem_construcao;	
		}
		L->contagem_construcao++;

        if (!visitado[nova_posicao_linha][nova_posicao_coluna]) {
			int paredei = (posicao_linha + nova_posicao_linha) / 2, paredej = (posicao_coluna + nova_posicao_coluna) / 2;
            L->celulas[paredei][paredej] = ' ';
			L->ordem_construcao[paredei][paredej] = L->contagem_construcao++;
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
	L->contagem_construcao = 0;

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
						for (int d = 0; d < 4; d++) { int ni = i + di_2[d]; int nj = j + dj_2[d]; if (posicao_valida(L, ni, nj) && visitado[ni][nj]) {
								posicao_linha = i;
								posicao_coluna = j;
								L->celulas[(i + ni) / 2][(j + nj) / 2] = ' ';
								L->ordem_construcao[(i + ni) / 2][(j + nj) / 2] = L->contagem_construcao++;
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
			int paredei = (posicao_linha + nova_posicao_linha) / 2, paredej = (posicao_coluna + nova_posicao_coluna) / 2;
            L->celulas[paredei][paredej] = ' ';
			L->ordem_construcao[paredei][paredej] = L->contagem_construcao++;
            visitado[nova_posicao_linha][nova_posicao_coluna] = 1;
            contador_visitados--;
        }

        posicao_linha = nova_posicao_linha;
        posicao_coluna = nova_posicao_coluna;
		L->ordem_construcao[posicao_linha][posicao_coluna] = L->contagem_construcao++;
    }

	for (int i = 0; i < L->linhas; i++) {
		free(visitado[i]);
	}
	free(visitado);
}

void algoritmo_backtracking(labirinto* L) {
	// particularmente acho o mais elegante. bem parecido com dfs, mas cortando o caminho que comeco;
	// comeco no topo e sigo em direcoes aleatorias. se nao consigo mais sair, volto pra onde foi originado
	// essa posicao que estou agora (o inicio desse caminho sem volta);
	// complexidade: O(nm);
	srand(time(NULL));

    int** visitado = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        visitado[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            visitado[i][j] = 0;
        }
    }

	L->contagem_construcao = 0;
	backtracking(visitado, L, posicao_aleatoria(L, 1), posicao_aleatoria(L, 0));

	for (int i = 0; i < L->linhas; i++) {
		free(visitado[i]);
	}
	free(visitado);
}

void backtracking(int** visitado, labirinto* L, int linha, int coluna) {
	// funcao recursiva que eh parte de algoritmo_backtracking.
	
	visitado[linha][coluna] = 1;

	int direcao[] = {0, 1, 2, 3}; // randomizando a direcao que ele anda a cada passo
	for (int k = 0; k < 4; k++) {
		int r = rand() % 4;
		
		direcao[k] ^= direcao[r]; // jeito maneiro de trocar os valores no indice [r] e no indice [k]
		direcao[r] ^= direcao[k]; // com triplo xor. obrigado gabriel santana.
		direcao[k] ^= direcao[r];
	}

	L->ordem_construcao[linha][coluna] = L->contagem_construcao++;
	for (int k = 0; k < 4; k++) {
		int movimento = direcao[k];
		int nova_linha = linha + di_2[movimento];
		int nova_coluna = coluna + dj_2[movimento];

		if (posicao_valida(L, nova_linha, nova_coluna) && 
			!visitado[nova_linha][nova_coluna]) {
			int paredei = (linha + nova_linha) / 2, paredej = (coluna + nova_coluna) / 2;
			L->celulas[paredei][paredej] = ' '; 
			L->ordem_construcao[paredei][paredej] = L->contagem_construcao++;
			backtracking(visitado, L, nova_linha, nova_coluna);
		}
	}
} 

void pacmaniza(labirinto* L){
	int quantidade_novas_conexoes = (L->linhas + L->colunas) / 2;
	// TODO: numero padrao de conexões, pra ser mais facil (Arrumar dps). analisar qual a melhor relacao.
	for (int feito = 0; feito < quantidade_novas_conexoes;) {
		int linha = posicao_aleatoria(L, 1);
		int coluna = posicao_aleatoria(L, 0);

		int direcao = rand() % 4;

		int nova_linha = linha + di_1[direcao];
		int nova_coluna = coluna + dj_1[direcao];

		if (posicao_valida(L, nova_linha, nova_coluna) &&
			L->celulas[nova_linha][nova_coluna] == '#' &&
			nova_coluna <= L->colunas) {
			L->celulas[nova_linha][nova_coluna] = ' ';
			feito++;
		}
	}

	// espelhando o labirinto;
	for (int i = 0; i < L->linhas; i++) {
		for (int j = L->colunas-1; j >= L->colunas / 2; j--) {
			L->celulas[i][j] = L->celulas[i][L->colunas - j - 1];
		}
	}
	
	// garantindo que nao exista nenhuma ilha
	for (int i = 1; i < L->linhas - 1; i++) {
		for (int j = 1; j < L->colunas - 1; j++) {
			int eh_ilha = L->celulas[i][j] == '#';

			// as 4 direcoes tem que ter ' '
			for (int k = 0; k < 4 && eh_ilha; k++) {
				eh_ilha &= L->celulas[i + di_1[k]][j + dj_1[k]] == ' ';
			}
		
			if (eh_ilha) {
				L->celulas[i][j] = ' ';
			}
		}
	}
}
void esperar_enter(int sim) {
    struct termios termios_p;
    tcgetattr(STDIN_FILENO, &termios_p);

    if (sim) {
        termios_p.c_lflag |= (ICANON | ECHO);
    } else {
        termios_p.c_lflag &= ~(ICANON | ECHO);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &termios_p);
}

void ver_construcao(labirinto *L) {
	// cada algoritmo gerador de labirinto guarda a ordem com que as celulas foram visitadas e paredes foram quebradas.
    esperar_enter(0);

    int etapa = 0, sair = 0;
    while (!sair) {
        system("clear");
		printf("[i] construcao do labirinto (etapa %d/%d): \n", etapa, L->contagem_construcao);

        for (int i = 0; i < L->linhas; i++) {
            for (int j = 0; j < L->colunas; j++) {
				printf("%s", (etapa + 1 == L->ordem_construcao[i][j] ? "\033[32m" : "\033[0m"));

				if ((L->ordem_construcao[i][j] == etapa + 1 || L->ordem_construcao[i][j] == etapa) 
				 	&& i & 1 && j & 1) {
					printf("\033[32m@\033[0m");
					continue;
				}

                if (L->ordem_construcao[i][j] == -1 || L->ordem_construcao[i][j] > etapa) {
                    printf("%c", (i & 1 && j & 1 ? ' ' : '#'));
                    continue;
                }

				printf("%c", L->celulas[i][j]);
			}
            printf("\n");
        }

		puts("[?] pressione: ");
		puts("[0] para terminar visualizacao.");
		puts("[i] para ir para o inicio.");
		puts("[f] para ir para o fim.");
		puts("[>] (seta p/ direita) para avancar etapa.");
		puts("[<] (seta p/ esquerda) para retornar etapa.");
		printf("...");

        char c = getchar();
        if (c == '\033') {
            c = getchar();
            if (c == '[') {
                c = getchar();
                if (c == 'C' && etapa < L->contagem_construcao) etapa++;
                else if (c == 'D' && etapa > 0) etapa--;
            }
        } else if (c == '0') {
            sair = 1;
        } else if (c == 'i') {
			etapa = 0;
        } else if (c == 'f') {
			etapa = L->contagem_construcao;
		}
    }

    esperar_enter(1);
}

void resolve_labirinto() {
    labirinto L = escolhe_labirinto();

	puts("[i] opcoes para resolucao:");
	puts("[0] de uma ponta a outra.");
	puts("[1] achar uma saida a partir de uma posicao aleatoria.");
	puts("[2] definir inicio e fim do trajeto.");

	int inicio_i, inicio_j;
	int fim_i, fim_j;
	
	int ok_modo = 0, selecao;
	while (!ok_modo) {
		printf("[?] selecione uma das opcoes: ");
		scanf("%d", &selecao);

		ok_modo = 1;
		switch (selecao) {
			case 0:
				inicio_i = 0;
				inicio_j = 1;
				fim_i = L.linhas-1;
				fim_j = L.colunas-2;
				break;
			case 1:
				inicio_i = posicao_aleatoria(&L, 1);
				inicio_j = posicao_aleatoria(&L, 0);
				fim_i = (rand() & 1 ? 0 : L.linhas-1);
				fim_j = (rand() & 1 ? 1 : L.colunas-2);
				break;
			case 2:
				int ok_inicio = 0, ok_fim = 0;
				printa_labirinto(L);
				while (!ok_inicio) {
					printf("[?] indique a linha do inicio: ");
					scanf("%d", &inicio_i);
					printf("[?] indique a coluna do inicio: ");
					scanf("%d", &inicio_j);

					ok_inicio = posicao_valida(&L, inicio_i, inicio_j) && L.celulas[inicio_i][inicio_j] == ' '; 
					if (!ok_inicio) puts("[e] informe uma celula valida para o inicio!");
				}
				while (!ok_fim) {
					printf("[?] indique a linha do fim");
					scanf("%d", &fim_i);
					printf("[?] indique a coluna do fim");
					scanf("%d", &fim_j);

					ok_fim = posicao_valida(&L, fim_i, fim_j) && L.celulas[fim_i][fim_j] == ' '; 
					if (!ok_fim) puts("[e] informe uma celula valida para o fim!");
				}
				break;
			default:
				puts("[e] selecao invalida!");
				ok_modo = 0;
		}
	}
	
    int** distancia = (int**) malloc(L.linhas * sizeof(int*));
    for (int i = 0; i < L.linhas; i++) {
        distancia[i] = (int*) malloc(L.colunas * sizeof(int));
        for (int j = 0; j < L.colunas; j++) {
            distancia[i][j] = -1;
        }
    }

	distancia[inicio_i][inicio_j] = 0;
	resolve_dfs(distancia, &L, inicio_i, inicio_j);
	ver_resolucao(&L, distancia, inicio_i, inicio_j, fim_i, fim_j);

    for (int i = 0; i < L.linhas; i++) {
        free(L.celulas[i]);
		free(distancia[i]);
    }
	free(distancia);
    free(L.celulas);
}

void resolve_dfs(int** distancia, labirinto* L, int i, int j) {
	for (int k = 0; k < 4; k++) {
		int ni = i + di_1[k], nj = j + dj_1[k];
		if (posicao_valida(L, ni, nj) && distancia[ni][nj] == -1 && L->celulas[ni][nj] != '#') {
			distancia[ni][nj] = distancia[i][j] + 1;
			resolve_dfs(distancia, L, ni, nj);
		}
	}
}

void ver_resolucao(labirinto* L, int** distancia, int inicio_i, int inicio_j, int fim_i, int fim_j) {
    esperar_enter(0);

    int** eh_caminho = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        eh_caminho[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
            eh_caminho[i][j] = 0;
        }
    }
	
	int posicao_linha = fim_i, posicao_coluna = fim_j;
	while (distancia[posicao_linha][posicao_coluna] >= 0) {
		eh_caminho[posicao_linha][posicao_coluna] = '1';
		
		for (int i = 0; i < 4; i++) {
			int nova_posicao_linha = posicao_linha + di_1[i], nova_posicao_coluna = posicao_coluna + dj_1[i];
			if (posicao_valida(L, nova_posicao_linha, nova_posicao_coluna) &&
				distancia[nova_posicao_linha][nova_posicao_coluna] + 1 == distancia[posicao_linha][posicao_coluna]) {
				posicao_linha = nova_posicao_linha;
				posicao_coluna = nova_posicao_coluna;
				break;
			}
		}
	}

    int etapa = 0, sair = 0;
    while (!sair) {
        system("clear");
		printf("[i] resolucao do labirinto (etapa %d/%d): \n", etapa, distancia[fim_i][fim_j]);

        for (int i = 0; i < L->linhas; i++) {
            for (int j = 0; j < L->colunas; j++) {
				if (i == inicio_i && j == inicio_j) {
					printf("\033[31mI\033[0m");
					continue;
				}
				if (i == fim_i && j == fim_j) {
					printf("\033[31mF\033[0m");
					continue;
				}

				printf("%s", (etapa == distancia[i][j] ? "\033[32m" : "\033[0m"));

				if (etapa == distancia[fim_i][fim_j] && eh_caminho[i][j]) {
					printf("\033[32m*\033[0m");
					continue;
				}

				if (distancia[i][j] == -1 || distancia[i][j] > etapa) {
					printf("%c", L->celulas[i][j]);
					continue;
				}
				
				if (distancia[i][j] == etapa) {
					printf("@");
					continue;
				}


				printf(".");
			}
            printf("\n");
        }

		puts("[?] pressione: ");
		puts("[0] para terminar visualizacao.");
		puts("[i] para ir para o inicio.");
		puts("[f] para ir para o fim.");
		puts("[>] (seta p/ direita) para avancar etapa.");
		puts("[<] (seta p/ esquerda) para retornar etapa.");
		printf("...");

        char c = getchar();
        if (c == '\033') {
            c = getchar();
            if (c == '[') {
                c = getchar();
                if (c == 'C' && etapa < distancia[fim_i][fim_j]) etapa++;
                else if (c == 'D' && etapa > 0) etapa--;
            }
        } else if (c == '0') {
            sair = 1;
        } else if (c == 'i') {
			etapa = 0;
        } else if (c == 'f') {
			etapa = distancia[fim_i][fim_j];
		}
    }

	for (int i = 0; i < L->linhas; i++) {
		free(eh_caminho[i]);
	}
	free(eh_caminho);
    esperar_enter(1);
}
