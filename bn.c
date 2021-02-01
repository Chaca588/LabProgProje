#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "player.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
  char defm[3];//array para ler valores
  char def[5];//array para ler valores
  //Cada array guarda a quantidade de barcos por cada tipo de barco
  char smapac[2], b1c[2], b2c[2], b3c[2], b4c[2], b5c[2];
  //varias para setup do jogo
  int hp, smapa, b1, b2, b3, b4, b5, ran, totalbarcos, max_boat;

int ataquemm(player_info* atacante, int smapa, int fd){//funcao para os ataques da multi-shell matrix
      int val;
      printf("Player %d a atacar!\n", atacante->nome);
    attc:
      printf("Introduz a coordenada X do ataque: ");
      scanf("%s", def);
      int x = testenum(def);
      x--;

      printf("Introduz a coordenada Y do ataque: ");
      scanf("%s", def);
      int y = testenum(def);
      y--;

      if(x < 0 || x >= smapa || y < 0 || y >= smapa){ //Verifica se as coordenadas estao dentro dos limites do mapa
        printf("Coordenadas invalidas\n");
  •••••••••••••••••••      goto attc;
      }

      if(write(fd, &x, sizeof(int)) == -1) return 1;//passar x para bn3
      if(write(fd, &y, sizeof(int)) == -1) return 1;//passar y para bn3
      sleep(1);
      if(read(fd, &val, sizeof(int)) == -1) return 1;//recebe val do bn3
   return val;
  }

int defesamm(player_info* defensor, int smapa, int fd){//funcao para os marcar tiros da multi-shell matrix
    int x, y;
    sleep(1);
    if(read(fd, &x, sizeof(int)) == -1) return 1;//recebe x do bn3
    if(read(fd, &y, sizeof(int)) == -1) return 1;//recebe y do bn3
    sleep(1);
    int val = atirarmm(defensor, x, y, smapa);

    if(defensor->hptotal == 0) val = 7;//val = 7 acaba o jogo
    if(write(fd, &val, sizeof(int)) == -1) return 1;//passar val para bn3

    return 0;
  }

int ataqueqm(player_infoq* atacante, int smapa, int fd){//funcao para os ataques da multi-shell qtree
      int val;
      printf("Player %d a atacar!\n", atacante->nome);
    attc:
      printf("Introduz a coordenada X do ataque: ");
      scanf("%s", def);
      int x = testenum(def);
      x--;

      printf("Introduz a coordenada Y do ataque: ");
      scanf("%s", def);
      int y = testenum(def);
      y--;

      if(x < 0 || x >= smapa || y < 0 || y >= smapa){ //Verifica se as coordenadas estao dentro dos limites do mapa
        printf("Coordenadas invalidas\n");
        goto attc;
      }

      if(write(fd, &x, sizeof(int)) == -1) return 1;//passar x para bn3
      if(write(fd, &y, sizeof(int)) == -1) return 1;//passar y para bn3
      sleep(1);

      if(read(fd, &val, sizeof(int)) == -1) return 1;//recebe val do bn3

     return val;
   }

int defesaqm(player_infoq* defensor, int smapa, int fd){//funcao para os marcar tiros da multi-shell matrix
    int x, y;
    sleep(1);
    if(read(fd, &x, sizeof(int)) == -1) return 1;//recebe x do bn3
    if(read(fd, &y, sizeof(int)) == -1) return 1;//recebe y do bn3
    sleep(1);
    int val = atirarqm(defensor, x, y, smapa);

    if(defensor->hptotal == 0) val = 7;//val = 7 acaba o jogo

    if(write(fd, &val, sizeof(int)) == -1) return 1;//passar val para bn3

    return 0;
}

void startup(){
  hp = 0, smapa = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, ran = 0, totalbarcos = 0;
  lermapa://Caso o tamanho do mapa seja inválido o programa volta a este ponto
  printf("Qual e o tamanho do mapa?(entre 20X20 e 40X40)\n");
  scanf("%s", smapac);
  smapa = atoi(smapac); // Transformação da string com o tamanho do mapa num inteiro
  if((smapa < 20) || (smapa > 40)){
    printf("tamanho do mapa fora do limite\n");
    goto lermapa;
  }

  int max_boat = smapa*smapa/25; // O limite de barcos do mapa

  IniNumBarcos:  //Caso os o numero de barcos seja inválido,
  totalbarcos=0; //o programa volta a este ponto

  barcos1://Caso o numero de barcos seja 0, o programa volta a este ponto
    printf("Quantos barcos de altura 3, base 2(L) a implementar?\n");
    scanf("%s", b1c);
    b1 = atoi(b1c);
    hp += b1*4;
    totalbarcos += b1;
    if(b1 == 0){
      printf("Tem de ter no minimo um barco deste tipo\n");
      goto barcos1;
    }

  barcos2://Caso o numero de barcos seja 0, o programa volta a este ponto
    printf("Quantos barcos de formato + a implementar?\n");
    scanf("%s", b2c);
    b2 = atoi(b2c);
    hp += b2*5;
    totalbarcos += b2;
    if(b2 == 0){
      printf("Tem de ter no minimo um barco deste tipo\n");
      goto barcos2;
    }

  barcos3://Caso o numero de barcos seja 0, o programa volta a este ponto
    printf("Quantos barcos de comprimento 3 a implementar?\n");
    scanf("%s", b3c);
    b3 = atoi(b3c);
    hp += b3*3;
    totalbarcos += b3;
    if(b3 == 0){
      printf("Tem de ter no minimo um barco deste tipo\n");
      goto barcos3;
    }

  barcos4://Caso o numero de barcos seja 0, o programa volta a este ponto
    printf("Quantos barcos formato cubo(3x3) a implementar?\n");
    scanf("%s", b4c);
    b4 = atoi(b4c);
    hp += b4*9;
    totalbarcos += b4;
    if(b4 == 0){
      printf("Tem de ter no minimo um barco deste tipo\n");
      goto barcos4;
    }

  barcos5://Caso o numero de barcos seja 0, o programa volta a este ponto
    printf("Quantos barcos de comprimento 5 a implementar?\n");
    scanf("%s", b5c);
    b5 = atoi(b5c);
    hp += b5*5;
    totalbarcos += b5;
    if(b5 == 0){
      printf("Tem de ter no minimo um barco deste tipo\n");
      goto barcos5;
    }

  if(totalbarcos > max_boat){
     printf("Numero de barcos exede o possivel %d\n", max_boat);
     goto IniNumBarcos;
  }

  modo_de_jogo_erro://Caso o id do modo de jogo seja invalido o programa retorna a este ponto
    printf("Posicões randomizadas(0) ou escolhidas(1)?\n");
    scanf("%s", defm);
    ran = testenum(defm);
    if(ran != 1 && ran != 0){
      printf("Escolha não valida\n");
      goto modo_de_jogo_erro;
    }
}

int MatUni(){

  srand(time(0));//inicia o funcionamento do random

  startup();

  //Alocação da memória de cada player
  player_info* player1 = (player_info*) malloc(sizeof(player_info));
  player_info* player2 = (player_info*) malloc(sizeof(player_info));
  player1->nome = 1;
  player2->nome = 2;
  player1->hptotal = hp;
  player2->hptotal = hp;
  //Alocação da memória dos mapas de cada player
  player1->mapa = (cell*) malloc((smapa*smapa) * sizeof(cell));
  player2->mapa = (cell*) malloc((smapa*smapa) * sizeof(cell));
  player1->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player2->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player1->nbarco = 0;
  player2->nbarco = 0;

  cell_ini(player1->mapa, smapa); //Inicialização do mapa do jogador 1
  cell_ini(player2->mapa, smapa); //Inicialização do mapa do jogador 2

  printf("\n\n");

  if(ran == 0){ //Inicialização aleatoria dos barcos
    player1 = inicio_random(player1, b1, b2, b3, b4, b5, smapa);

    player2 = inicio_random(player2, b1, b2, b3, b4, b5, smapa);
  }

  else{ //Inicialização manual dos barcos
    printf("Setup Player %d:\n", player1->nome);
    player1 = inicio(player1, b1, b2, b3, b4, b5, smapa);

    printf("\nSetup Player %d:\n", player2->nome);
    player2 = inicio(player2, b1, b2, b3, b4, b5, smapa);
  }

//DEBUG
  print_mapa(player1, smapa);//
  printf("\n");
  print_mapa(player2, smapa);

  //Codigo para o inicio dos ataques dos jogadores
  //O código corre enquanto nenhum dos jogadores "morreu"
  cicloAt:
    ataque(player1, player2, smapa);
    if(player2->hptotal > 0){
      ataque(player2, player1, smapa);
    }
  if(player1->hptotal > 0 && player2->hptotal > 0) goto cicloAt;

  if(player2->hptotal == 0) printf("Player 1 ganha!\n");
  if(player1->hptotal == 0) printf("Player 2 ganha!\n");

  //Libertação da memória alocada anteriormente
    freeall(player1, smapa);
    freeall(player2, smapa);

  exit(0);
}

int MatMul(){
  srand(time(0));//inicia o funcionamento do random
  sleep(1);//gera uma nova seed para bn2
  system("mkfifo play");//cria a fifo para transferir dados
  system("gnome-terminal --window -- ./bn2");//abre uma segunda shell
  int fd = open("play", O_RDWR);//inicia fd no modo read e write
  if(fd == -1) return 1;

  startup();

  if(write(fd, &smapa, sizeof(int)) == -1) return 1;//passa o tamanho do mapa para bn2
  //Passa os valores para bn2
  if(write(fd, &b1, sizeof(int)) == -1) return 1;
  if(write(fd, &b2, sizeof(int)) == -1) return 1;
  if(write(fd, &b3, sizeof(int)) == -1) return 1;
  if(write(fd, &b4, sizeof(int)) == -1) return 1;
  if(write(fd, &b5, sizeof(int)) == -1) return 1;
  if(write(fd, &totalbarcos, sizeof(int)) == -1) return 1;
  if(write(fd, &hp, sizeof(int)) == -1) return 1;
  if(write(fd, &ran, sizeof(int)) == -1) return 1;//passa random para bn2
  //Alocação da memória do player1
  player_info* player1 = (player_info*) malloc(sizeof(player_info));
  player1->nome = 1;
  player1->hptotal = hp;
  //Alocação da memória do mapa do player1
  player1->mapa = (cell*) malloc((smapa*smapa) * sizeof(cell));
  player1->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player1->nbarco = 0;

  cell_ini(player1->mapa, smapa); //Inicialização do mapa do jogador 1

  printf("\n\n");

  if(ran == 0){ //Inicialização aleatoria dos barcos
    player1 = inicio_random(player1, b1, b2, b3, b4, b5, smapa);
  }

  else{ //Inicialização manual dos barcos
    printf("Setup Player %d:\n", player1->nome);
    player1 = inicio(player1, b1, b2, b3, b4, b5, smapa);
  }
//DEBUG
  print_mapa(player1, smapa);
  printf("\n");
  int val = 0;
  //Codigo para o inicio dos ataques dos jogadores
  //O código corre enquanto nenhum dos jogadores "morreu"
  while(val != 7){
    val = ataquemm(player1, smapa, fd);//player 1 ataca
    print_val(val);
    if(val == 7){//se val = 7 termina o jogo
        printf("Player 1 ganha!\n");
        goto fim;//salta para o fim do jogo
    }
    val = defesamm(player1, smapa, fd);//player 1 defende
    if(player1->hptotal == 0){
        printf("Player 2 ganha!\n");
        goto fim;//salta para o fim do jogo
    }
  }

  fim://salta para este ponto se o player1 ganhar
  //Libertação da memória alocada anteriormente
  freeall(player1, smapa);
  close(fd);//fecha o fd
  exit(0);
}

int QtUni(){

  srand(time(0));//inicia o funcionamento do random

  startup();

  //Alocação da memória de cada player
  player_infoq* player1 = (player_infoq*) malloc(sizeof(player_info));
  player_infoq* player2 = (player_infoq*) malloc(sizeof(player_info));
  player1->nome = 1;
  player2->nome = 2;
  player1->hptotal = hp;
  player2->hptotal = hp;
  //Alocação da memória dos mapas de cada player
  player1->mapa = start_qtree(smapa);
  player2->mapa = start_qtree(smapa);
  player1->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player2->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player1->nbarco = 0;
  player2->nbarco = 0;

  printf("\n\n");

  if(ran == 0){ //Inicialização aleatoria dos barcos
    player1 = inicioq_random(player1, b1, b2, b3, b4, b5, smapa);
    player2 = inicioq_random(player2, b1, b2, b3, b4, b5, smapa);
  }

  else{ //Inicialização manual dos barcos
    printf("Setup Player %d:\n", player1->nome);
    player1 = inicioq(player1, b1, b2, b3, b4, b5, smapa);

    printf("\nSetup Player %d:\n", player2->nome);
    player2 = inicioq(player2, b1, b2, b3, b4, b5, smapa);
  }
//DEBUG
  print_mapaq(player1, smapa);
  printf("\n");
  print_mapaq(player2, smapa);

  //Codigo para o inicio dos ataques dos jogadores
  //O código corre enquanto nenhum dos jogadores "morreu"
  cicloAt:
    ataqueq(player1, player2, smapa);
    if(player2->hptotal > 0){
      ataqueq(player2, player1, smapa);
    }
  if(player1->hptotal > 0 && player2->hptotal > 0) goto cicloAt;

  if(player2->hptotal == 0) printf("Player 1 ganha!\n");
  if(player1->hptotal == 0) printf("Player 2 ganha!\n");

  //Libertação da memória alocada anteriormente
  freeallq(player1, smapa);
  freeallq(player2, smapa);

  exit(0);
}

int QtMul(){
  srand(time(0));//inicia o funcionamento do random
  sleep(1);
  system("mkfifo play");
  system("gnome-terminal --window -- ./bn3");
  int fd = open("play", O_RDWR);
  if(fd == -1) return 1;

  startup();
  if(write(fd, &smapa, sizeof(int)) == -1) return 1;//passa o tamanho do mapa para bn2
  //Passa os valores para bn2
  if(write(fd, &b1, sizeof(int)) == -1) return 1;
  if(write(fd, &b2, sizeof(int)) == -1) return 1;
  if(write(fd, &b3, sizeof(int)) == -1) return 1;
  if(write(fd, &b4, sizeof(int)) == -1) return 1;
  if(write(fd, &b5, sizeof(int)) == -1) return 1;
  if(write(fd, &totalbarcos, sizeof(int)) == -1) return 1;
  if(write(fd, &hp, sizeof(int)) == -1) return 1;
  if(write(fd, &ran, sizeof(int)) == -1) return 1;//passa random para bn2
  //Alocação da memória do player1
  //Alocação da memória de cada player
  player_infoq* player1 = (player_infoq*) malloc(sizeof(player_info));
  player1->nome = 1;
  player1->hptotal = hp;
  //Alocação da memória dos mapas de cada player
  player1->mapa = start_qtree(smapa);
  player1->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player1->nbarco = 0;

  //cell_ini(player1->mapa, smapa); //Inicialização do mapa do jogador 1
  //cell_ini(player2->mapa, smapa); //Inicialização do mapa do jogador 2

  printf("\n\n");

  if(ran == 0){ //Inicialização aleatoria dos barcos
    player1 = inicioq_random(player1, b1, b2, b3, b4, b5, smapa);
  }

  else{ //Inicialização manual dos barcos
    printf("Setup Player %d:\n", player1->nome);
    player1 = inicioq(player1, b1, b2, b3, b4, b5, smapa);
  }

  print_mapaq(player1, smapa);
  printf("\n");

  //Codigo para o inicio dos ataques dos jogadores
  //O código corre enquanto nenhum dos jogadores "morreu"
  int val = 0;
  while(val != 7){
  val = ataqueqm(player1, smapa, fd);
  print_val(val);
  if(val == 7){//se val = 7 termina o jogo
      printf("Player 1 ganha!\n");
      goto fim;
  }
  val = defesaqm(player1, smapa, fd);
  if(player1->hptotal == 0){
      printf("Player 2 ganha!\n");
      goto fim;//salta para o fim do jogo
  }
}

  fim://salta para este ponto se o player1 ganhar
  freeallq(player1, smapa);  //Libertação da memória alocada anteriormente
  close(fd);//fecha o fd
  exit(0);
}

int main(int argc, char const *argv[]){
  int modo1, modo2;
  modos://Caso o id do modo de jogo seja invalido o programa retorna a este ponto
    printf("Matrix(0) ou QuadTree(1)?\n");
    scanf("%s", defm);
    modo1 = testenum(defm);
    if(modo1 != 1 && modo1 != 0){
      printf("Escolha não valida\n");
      goto modos;
    }

    modos2://Caso o id do modo de jogo seja invalido o programa retorna a este ponto
      printf("Uni-Shell(0) ou Multi-Shell(1)?\n");
      scanf("%s", defm);
      modo2 = testenum(defm);
      if(modo2 != 1 && modo2 != 0){
        printf("Escolha não valida\n");
        goto modos2;
      }

  switch (modo1) {
    case 0:
    switch (modo2) {
      case 0:
      MatUni();
      break;
      case 1:
      MatMul();
      break;
    }
    break;
    case 1:
    switch (modo2) {
      case 0:
      QtUni();
      break;
      case 1:
      QtMul();
      break;
    }
  }
}
