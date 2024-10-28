# ᱝ···ᗣ···ᗣ··Bem·Vindo(a)·ao·MazeMan!···ᗣ···ᗣ··

<img src= "img/image (15).png" width = 600>

## | 🚀 Objetivo do Projeto | 
-  Nosso objetivo com o <b>MazeMan</b> como um Projeto livre da disciplina de Desenvolvimento de Algoritmos (CC2632) é o desenvolvimento pessoal e criativo em algoritmos de forma gamificada a partir de tópicos que achamos interessantes. O programa apresenta um resolvedor de labirintos com <b>4 algoritmos</b> distintos e um extra que seria uma releitura do clássico "PacMan" <b>100% jogavel</b> com mapas distintos e criados pelo usuário. 


## | 💻 Compilação e Execução do MazeMan | 
-  Para Compilar nosso projeto é necessario abrir e rodar nosso makefile que apresenta o seguinte codigo:
  ```
  all:
	gcc main.c labirintos.c arquivos.c mazeman.c -o exe
  ```
**2.Terminal**
-  Abrir o Terminal e digitar o seguinte codigo
  ```
	gcc main.c labirintos.c arquivos.c mazeman.c -o exe
  ```
- Logo em seguida digite
  ```
	./executavel
  ```

## | ☕ Usando o MazeMan |
-  Ao iniciar nosso projeto é apresentado o *"Menu inicial"* com algumas opções:
  ### 1. Menu Inicial
-	**1.0 Criar labirinto**
	-	O usuário deve definir uma quantidade de linhas e colunas do labirinto
	-	Aqui você tem a opção de escolher entre 5 algoritmos de resolução e criação de labirintos diferentes. (**binary tree** | **sidewinder** | **aldous-border** | **hunt-and-kill** | **backtracking**). Com um extra do algoritmo para jogar **MazeMan**.
 	-	Depois de escolher o algoritmo desejado, aparecerá uma preview do labirinto sendo criado, onde o usuário pode manipular (**"->"** para ver o passo a passo | **"<-"** retroceder | **"i"** voltar ao início | **"f"** final do labirinto | **"0"** fechar ). Ao fechar, uma opção de salvar ou não o arquivo aparecera.
-	**1.1 Resolver labirinto**
	-	Nesta opção, é mostrado todos os labirintos salvos pelo usuário, o qual, deve escolher um deles para ser resolvido
 	-	Ao escolher o labirinto, é dado ao usuário 3 opções de resolução cada uma com suas peculiaridades
  	-	Após escolher uma das opções é mostrado o labirinto e o passo a passo de sua resolução (Menu igual ao de *"Criar labirinto"*, em que é possivel ver etapa por etapa).	
-	**1.2 Jogar MazeMan**
	-	Ao selecionar essa opção, é mostrado ao usuário o Menu inicial do MazeMan.
  ### 2. Menu MazeMan
-	**1.0 Jogar mapa aleatorio novo**
	-	Aqui um mapa aleatório com um tamanho predefinido é gerado, onde o usuário poderá jogar uma partida rápida. Apos a partida, é questionado se o mesmo deseja que salve sua pontuação.
-	**1.1 Jogar em mapa ja gerado**
	-	Nesta opção, é mostrado ao usuário todos os labirintos salvos por ele (labirintos gerados com o algoritmo do MazeMan Recomendados!)
 	-	Após escolher o labirinto, o usuário poderá jogar a vontade. Ao fim do jogo, é perguntado se deseja salvar sua pontuação.
-	**1.2 Ver pontuações**
	-	Nesta opção, é mostrado ao usuário todas as suas pontuações adquiridas em cada mapa jogado.
  ### 3. Como Jogar? 
-	MazeMan é um jogo que se passa em um labirinto e o jogador **"<"** tem como objetivo, passar por todo labirinto coletando comida **"•"** que esta espalhada por todo labirinto, porem, o jogador é perseguido por fantasmas. **"&"** O qual são perigosos e não podem encontrar o jogador enquanto ele vaga por todo labirinto.
-	**1.0 Controles**
	-	O jogo apresenta controles de direção básicos como (**W**(Norte) | **A**(Oeste) | **S**(Sul) | **D** (Leste))	 	

## | 🤝 Participantes do Grupo |
<table>
  <tr>
    <td align="center">
        <img src="img/mano bernas.PNG" width="120px;" alt="Foto do Bernardo"/><br>
        <sub>
          <b>Bernardo</b>
          <br>
          <b>R.A.: 24.124.007-6</b>
          <br>
            <cite>- Projeto maneiro </cite>
        </sub>
    </td>
    <td align="center">
        <img src="img/eu.PNG" width="120px;" alt="Foto do Gabriel"/><br>
        <sub>
          <b>Gabriel</b>
          <br>
          <b>R.A.: 24.024.124.0</b>
          <br>
          <cite>- Legal </cite>
        </sub>
    </td>
    <td align="center">
        <img src="img/kaio.PNG" width="120px;" alt="Foto do Kaio"/><br>
        <sub>
          <b>Kaio</b>
          <br>
          <b>R.A.: 24.124.058-9</b>
          <br>
          <cite>- Hype </cite>
        </sub>
    </td>
  </tr>
</table>
