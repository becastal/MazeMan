#include "mazeman.h"
#include "labirintos.h"
#include "arquivos.h"

void mazeman_inicio() {
	system("@cls||clear");
	labirinto L;

	puts("[+] seja bem-vindo a MazeMan!");
	puts("[0] sair.");
	puts("[1] jogar mapa aleatorio novo.");
	puts("[2] jogar em mapa ja gerado.");
	puts("[3] ver pontuacoes.");

	int ok_escolha = 0, selecao;
	while (!ok_escolha) {
		printf("\n[?] selecione uma das opcoes: ");
		scanf("%d", &selecao);

		ok_escolha = 1;
		switch (selecao) {
			case 0:
				return;
			case 1:
				system("@cls||clear");
				strcpy(L.nome, "mapa aleatorio");
				L.linhas = 2 * 10 + 1, L.colunas = 2 * 30 + 1;
				L.celulas = (char**) malloc(L.linhas * sizeof(char*));
				L.ordem_construcao = (int**) malloc(L.linhas * sizeof(int*));
				for (int i = 0; i < L.linhas; i++) {
					L.celulas[i] = (char*) malloc(L.colunas * sizeof(char));
					L.ordem_construcao[i] = (int*) malloc(L.colunas * sizeof(int));

					for (int j = 0; j < L.colunas; j++) {
						L.ordem_construcao[i][j] = -1;
						L.celulas[i][j] = (i & 1 && j & 1 ? ' ' : '#');	
					}
				}
				algoritmo_backtracking(&L);
				pacmaniza(&L);
				break;
			case 2:
				system("@cls||clear");
				L = escolhe_labirinto();
				break;
			case 3:
				system("@cls||clear");
				mazeman_pontuacoes();
				return;
			default:
				ok_escolha = 0;
				puts("[e] selecao invalida!");
		}
	}

	for (int i = 0; i < L.linhas; i++) {
		for (int j = 0; j < L.colunas; j++) {
			if (L.celulas[i][j] == ' ') {
				L.celulas[i][j] = '.';
			}
		}
	}
	int pontos = mazeman_game_loop(&L);

	char selecao_salvar;
	int ok_salvar = 0;

	while (!ok_salvar) {
		printf("[?] salvar pontuacao (s/n)? ");
		scanf(" %c", &selecao_salvar);

		ok_salvar = (selecao_salvar == 's' || selecao_salvar == 'S' ||
					 selecao_salvar == 'n' || selecao_salvar == 'N');
		if (!ok_salvar) puts("[e] informe 's' ou 'n'.");
	}

	if (selecao_salvar == 's' || selecao_salvar == 'S') {
		mazeman_salvar_pontuacao(pontos, &L);
		return;
	}

	for (int i = 0; i < L.linhas; i++) {
		free(L.celulas[i]);
		free(L.ordem_construcao[i]);
	}
	free(L.ordem_construcao);
	free(L.celulas);
}

// Loop do jogo do mazeman
int mazeman_game_loop(labirinto *L){
    Mazeman mazeman;
    Fantasma fantasmas[4];
    char input;
    int direcao;
    int move_acao;
    int fim_de_jogo;
    int pontos_jogador;

    pontos_jogador = 0;
    fim_de_jogo = 0;

    #ifndef _WIN32
		non_blocking_input_linux();
    #endif

    mazeman_spawn(*L, &mazeman);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[0]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[1]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[2]);
    mazeman_fantasma_spawn(*L, mazeman, &fantasmas[3]);

    int** tem_ponto = (int**) malloc(L->linhas * sizeof(int*));
    for (int i = 0; i < L->linhas; i++) {
        tem_ponto[i] = (int*) malloc(L->colunas * sizeof(int));
        for (int j = 0; j < L->colunas; j++) {
			tem_ponto[i][j] = L->celulas[i][j] == '.';
        }
    }

    system("@cls||clear");
    printa_labirinto(*L);
    // Jogo acaba quando move_acao é 2 (mazeman bate no fantasma)
    while(!fim_de_jogo){
        input = mazeman_obter_input();

        // Retorna da direcao para andar
        direcao = mazeman_obter_direcao(input, mazeman);
        // Obtem se o movimento é válido (1) ou se vai bater em fantasma (2) Caso inválido retorna 0
        move_acao = mazeman_movimento_valido(*L, mazeman, direcao);
        mazeman.direcao_olhando = direcao;

        mazeman_atualizar_mapa(L, &mazeman, fantasmas, move_acao, &pontos_jogador, tem_ponto);
        fim_de_jogo = mazeman_checar_colisao_fantasma(*L, fantasmas, mazeman);

    #ifdef _WIN32
        Sleep(100);  // Windows
    #else
        usleep(100000);  // Linux
    #endif
    }
    #ifndef _WIN32
		restore_input_linux();
    #endif
    mazeman_atualizar_print(L->linhas, 0, '\n');
    printf("Fim de jogo, voce morreu!\n");

	for (int i = 0; i < L->linhas; i++) {
		free(L->celulas[i]);
	}
	free(L->celulas);

    return pontos_jogador;
}

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

struct termios original_term;

void non_blocking_input_linux() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &original_term);
    term = original_term;

    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}

void restore_input_linux() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);
}
#endif

void mazeman_atualizar_mapa(labirinto *L, Mazeman *maz, Fantasma *f, int move_acao, int *pontos_jogador, int** tem_ponto){
    int fant_i;
    int fant_j;
    int maz_i = maz->posicao_linha;
    int maz_j = maz->posicao_coluna;
    char mazeman_icone_direcao[4] = {'^', 'v', '>', '<'};
    int i;

    // Limpa o quadrado antigo do mazeman
    L->celulas[maz_i][maz_j] = ' ';
	tem_ponto[maz_i][maz_j] = 0;

    // Atualizar o print
    mazeman_atualizar_print(maz_i, maz_j, ' ');

    // Checa se o movimento é permitido
    if (move_acao){
        mazeman_move(maz, maz->direcao_olhando);
    }

    // Cordenadas do mazeman atualizadas
    maz_i = maz->posicao_linha;
    maz_j = maz->posicao_coluna;

    // Checa se o quadrado que o mazeman está é um ponto
    if(L->celulas[maz_i][maz_j] == '.'){
        (*pontos_jogador)++;
    }

    // Quadrado que o mazeman vai se mover para
    L->celulas[maz_i][maz_j] = mazeman_icone_direcao[maz->direcao_olhando];
    mazeman_atualizar_print(maz_i, maz_j, mazeman_icone_direcao[maz->direcao_olhando]);

    // Atualiza posicao dos fantasmas ou move eles
    for (i=0; i < 4; i++){
		char c = (tem_ponto[f[i].posicao_linha][f[i].posicao_coluna] ? '.' : ' ');
        L->celulas[f[i].posicao_linha][f[i].posicao_coluna] = c; // Limpa o quadrado antigo dos fantasmas
        mazeman_atualizar_print(f[i].posicao_linha, f[i].posicao_coluna, c);
    }
    for (i = 0; i < 4; i++){
        mazeman_fantasma_move(*L, &f[i]);
        fant_i = f[i].posicao_linha;
        fant_j = f[i].posicao_coluna;
        // Quadrado que o fantasma vai se mover para
        L->celulas[fant_i][fant_j] = '&';
        mazeman_atualizar_print(fant_i, fant_j, '&');
    }
}

int mazeman_obter_input() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    int ch = getchar();
        if (ch != EOF) {
            return ch;
        }
#endif
    return -1;  // Nenhum input
}

void mazeman_spawn(labirinto L, Mazeman *maz){
    int maz_pos_i;
    int maz_pos_j;

    // Define uma direcao padrao pro mazeman
    maz->direcao_olhando = 3;

    // Obtem uma posicao valida para spawnar o mazeman
    while(1){
        maz_pos_i = posicao_aleatoria(&L, 1);
        maz_pos_j = posicao_aleatoria(&L, 0);
        if (L.celulas[maz_pos_i][maz_pos_j] != '#'){
            maz->posicao_linha = maz_pos_i;
            maz->posicao_coluna = maz_pos_j;
            break;
        }
    }
}

int mazeman_obter_direcao(char input, Mazeman maz){
    if (input == 'W' || input == 'w'){
        return 0;
    }
    else if (input == 'S' || input == 's'){
        return 1;
    }
    else if (input == 'D' || input == 'd'){
        return 2;
    }
    else if (input == 'A' || input == 'a'){
        return 3;
    }
    else{
        return maz.direcao_olhando;
    }
}

int mazeman_movimento_valido(labirinto L, Mazeman maz, int direcao){
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};

    // Posição atual do mazeman + a posicao do proximo frame movendo na direcao que o jogador escolheu
    int maz_i = maz.posicao_linha + di_1[direcao];
    int maz_j = maz.posicao_coluna + dj_1[direcao];

    // Checa se a posição está dentro do mapa, se não estiver retorna 0
    if (!posicao_valida(&L, maz_i, maz_j)) return 0;

    char posicao_final = L.celulas[maz_i][maz_j];

    // Checa se vai bater em uma parede
    if(posicao_final == '#'){
        return 0;
    }

    else{
        return 1;
    }

}

void mazeman_move(Mazeman *maz, int direcao){
    // NORTE, SUL, LESTE, OESTE EM ORDEM
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};

    maz->posicao_linha += di_1[direcao];
    maz->posicao_coluna += dj_1[direcao];
}

void mazeman_fantasma_spawn(labirinto L, Mazeman maz, Fantasma *f){
    int maz_pos_i;
    int maz_pos_j;
    int f_pos_i;
    int f_pos_j;
    int dif_i;
    int dif_j;

    maz_pos_i = maz.posicao_linha;
    maz_pos_j = maz.posicao_coluna;

    // Define uma direcao padrao pro fantasma
    f->direcao_olhando = 2;

    // Obtem uma posicao valida para spawnar o mazeman
    while(1){
        f_pos_i = posicao_aleatoria(&L, 1);
        f_pos_j = posicao_aleatoria(&L, 0);
        dif_i = abs(maz_pos_i - f_pos_i);
        dif_j = abs(maz_pos_j - f_pos_j);

        // Checar quadrado vazio e diferença de 5x5 do mazeman
        if (L.celulas[f_pos_i][f_pos_j] != '#' && dif_i > 5 && dif_j > 5){
            f->posicao_linha = f_pos_i;
            f->posicao_coluna = f_pos_j;
            break;
        }
    }
}

void mazeman_fantasma_move(labirinto L, Fantasma *f){
    int di_1[] = {-1, 1, 0, 0};
    int dj_1[] = {0, 0, 1, -1};
    int fant_i;
    int fant_j;
    char posicao_final;
    int direcao;
    int direcao_permitida = 0; // Checar se precisa mudar ou não direcao

    while(!direcao_permitida){
        direcao = rand() % 4;
        // Posição atual do fantasma e a direcao que ele está configurado
        fant_i = f->posicao_linha + di_1[direcao];
        fant_j = f->posicao_coluna + dj_1[direcao];

        // Checa se a posição está dentro do mapa, coloca para mudar de direção
        if (!posicao_valida(&L, fant_i, fant_j)) direcao_permitida = 0;

        posicao_final = L.celulas[fant_i][fant_j];

        // Checa se vai bater em uma parede, se for coloca para mudar de direção
        if(posicao_final == '#') direcao_permitida = 0;
        else{
            break;
        }

        // Altera a direcao para uma direção aleatória
    }

    f->posicao_linha = fant_i;
    f->posicao_coluna = fant_j;
    f->direcao_olhando = direcao;

}

void mazeman_atualizar_print(int i, int j, char conteudo){
    printf("\033[%d;%dH%c", i+1, j+1, conteudo);
}

int mazeman_checar_colisao_fantasma(labirinto L, Fantasma *f, Mazeman maz){
    int i;

    for(i = 0; i < 4; i++){
        // Checa se o mazeman colidiu com um fantasma
        if (L.celulas[f[i].posicao_linha][f[i].posicao_coluna] == '#'){
            return 0;
        }
        else if(f[i].posicao_linha == maz.posicao_linha && f[i].posicao_coluna == maz.posicao_coluna){
            return 1;
        }
        else if(f[i].posicao_linha+1 == maz.posicao_linha && f[i].posicao_coluna == maz.posicao_coluna){
            return 1;
        }
        else if(f[i].posicao_linha == maz.posicao_linha && f[i].posicao_coluna+1 == maz.posicao_coluna){
            return 1;
        }
        else if(f[i].posicao_linha+1 == maz.posicao_linha && f[i].posicao_coluna+1 == maz.posicao_coluna){
            return 1;
        }
    }
    return 0;
}

void mazeman_pontuacoes() {
    int quantidade_pontuacoes;
    pontuacao* P = le_pontuacoes(&quantidade_pontuacoes);

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    printf("\n%10s ║ %30s ║ %30s\n", "pontos", "nome_usuario", "nome_mapa");
    printf("═══════════╬════════════════════════════════╬═══════════════════════════════\n");

    for (int i = 0; i < quantidade_pontuacoes; i++) {
        printf("%10d ║ %30s ║ %30s\n", P[i].pontos, P[i].nome_usuario, P[i].nome_mapa);
    }
    printf("═══════════╩════════════════════════════════╩═══════════════════════════════\n");
    SetConsoleOutputCP(GetOEMCP());
#else
    printf("\n%10s ║ %30s ║ %30s\n", "pontos", "nome_usuario", "nome_mapa");
    printf("═══════════╬════════════════════════════════╬═══════════════════════════════\n");

    for (int i = 0; i < quantidade_pontuacoes; i++) {
        printf("%10d ║ %30s ║ %30s\n", P[i].pontos, P[i].nome_usuario, P[i].nome_mapa);
    }
    printf("═══════════╩════════════════════════════════╩═══════════════════════════════\n");
#endif

    printf("\ntotal de pontuacoes: %d\n", quantidade_pontuacoes);
	getchar();
	getchar();
    salva_pontuacoes(P, quantidade_pontuacoes);
}
void mazeman_salvar_pontuacao(int pontos, labirinto* L) {
    int quantidade_pontuacoes;
    pontuacao* pontuacoes = le_pontuacoes(&quantidade_pontuacoes);

    pontuacoes = realloc(pontuacoes, (quantidade_pontuacoes + 1) * sizeof(pontuacao));

    char nome_usuario[100];
    printf("[i] informe seu nome: ");
	getchar();
    fgets(nome_usuario, sizeof(nome_usuario), stdin);
    nome_usuario[strcspn(nome_usuario, "\n")] = '\0';

    if (pontuacoes[quantidade_pontuacoes].nome_usuario) {
        strncpy(pontuacoes[quantidade_pontuacoes].nome_usuario, nome_usuario, sizeof(pontuacoes[quantidade_pontuacoes].nome_usuario) - 1);
        pontuacoes[quantidade_pontuacoes].nome_usuario[sizeof(pontuacoes[quantidade_pontuacoes].nome_usuario) - 1] = '\0';
    }

    strncpy(pontuacoes[quantidade_pontuacoes].nome_mapa, L->nome, sizeof(pontuacoes[quantidade_pontuacoes].nome_mapa) - 1);
    pontuacoes[quantidade_pontuacoes].nome_mapa[sizeof(pontuacoes[quantidade_pontuacoes].nome_mapa) - 1] = '\0';
    pontuacoes[quantidade_pontuacoes].pontos = pontos;

    salva_pontuacoes(pontuacoes, quantidade_pontuacoes + 1);
}

