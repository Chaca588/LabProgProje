### Compilação e execução

Para poder jogar o jogo o programa deve ser compilado com o comando 'make', sendo
depois executado com o './bn'.

### Regras e esclarecimentos
-Deve selecionar o modelo do jogo 0 para usar matrix, 1 para usar quadtree.
-Deve selecionar se vai utilizar uma shell 0 ou duas shells 1.
-O mapa de jogo é um quadrado composto por x colunas e y linhas com dimensões entre 20x20 e 40x40.
-Os jogadores devem decidir quantos barcos de cada tipo esta instacia do jogo ira ter.
-No inicio do jogo deve sempre existir pelo menos um barco de cada tipo.
-O numero total de barcos não deve exeder os tamanho do mapa x tamanho do mapa / 25
-Deve selecionar o modo de jogo intruduzindo 0 para a inserção de barcos aleatoria e 1 para a inserção 
manual.
-O modo random coloca todos os barcos em posições e orientações aleatorias dentro das dimensões desejadas.
-O modo de jogo manual pede uma Orientação(0º, 90º, 180º, 270º), um x(coluna), e um y(linha) para a posição 
central de cada barco.
-Nos mapas mostrados temos que 0 é uma posição ainda não verificada, 1 é um barco danificado e 2 é uma 
posição que já foi vericada mas não tinha barco.
-O jogo acaba quando um jogador não tiver barcos.


### Tipos de barcos

Tipo L
1
1
11

Tipo Mais
 1
111
 1

Tipo 3

111

Tipo Quadrado
111
111
111

Tipo 5

11111


### Como Jogar

O jogo é jogado por turnos, atacando 1º o Player 1 e depois o Player 2, para atacar o jogador deve 
introduzir o x(coluna) e y(linha) que deseja atacar, é depois mostrada uma mensagem sobre a posição 
atacada, "Hit!" para quando um barco foi atinjido, "Falhou" quando nenhum barco foi atinjido, e 
"Tiro repetido" para quando foi atacado uma posição repetida.

