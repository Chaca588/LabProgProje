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

char def[5];

int ataquemm(player_info* atacante, int smapa, int fd){//funcao para os ataques da multi-shell Matrix
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
    if(write(fd, &x, sizeof(int)) == -1) return 1;//passar x para bn
    if(write(fd, &y, sizeof(int)) == -1) return 1;//passar y para bn
    sleep(1);
    if(read(fd, &val, sizeof(int)) == -1) return 1;//recebe val do bn
    return val;
}

int defesamm(player_info* defensor, int smapa, int fd){//funcao para os marcar tiros da multi-shell Matrix
  int x, y;
  sleep(1);
  if(read(fd, &x, sizeof(int)) == -1) return 1;//recebe x do bn
  if(read(fd, &y, sizeof(int)) == -1) return 1;//recebe y do bn
  sleep(1);
  int val = atirarmm(defensor, x, y, smapa);

  if(defensor->hptotal == 0) val = 7;//val = 7 acaba o jogo

  if(write(fd, &val, sizeof(int)) == -1) return 1;//passar val para bn

  return 0;
}

int main(int argc, char const *argv[]){
  int fd = open("play", O_RDWR);//inicia fd no modo read e write
  if(fd == -1) return 1;
  srand(time(0));//inicia o funcionamento do random
  //Cada int guarda a quantidade de barcos por cada tipo de barco
  int hp = 0, smapa, b1, b2, b3, b4, b5, random, totalbarcos;
  int val = 0;
  printf("Setup\n");
  if(read(fd, &smapa, sizeof(int)) == -1) return 1;//recebe o tamanho do bn
  printf("Tamanho do mapa: %d\n", smapa);
  int max_boat = smapa*smapa/25; // O limite de barcos do mapa
  //recebe os valores de bn
  if(read(fd, &b1, sizeof(int)) == -1) return 1;
  printf("b1: %d\n", b1);
  if(read(fd, &b2, sizeof(int)) == -1) return 1;
  printf("b2: %d\n", b2);
  if(read(fd, &b3, sizeof(int)) == -1) return 1;
  printf("b3: %d\n", b3);
  if(read(fd, &b4, sizeof(int)) == -1) return 1;
  printf("b4: %d\n", b4);
  if(read(fd, &b5, sizeof(int)) == -1) return 1;
  printf("b5: %d\n", b5);
  if(read(fd, &totalbarcos, sizeof(int)) == -1) return 1;
  printf("total barcos: %d\n", totalbarcos);
  if(read(fd, &hp, sizeof(int)) == -1) return 1;
  printf("Hp total: %d\n", hp);


  if(read(fd, &random, sizeof(int)) == -1) return 1;//recebe o valor de random
  printf("random: %d\n", random);
  //Alocação da memória do player2
  player_info* player2 = (player_info*) malloc(sizeof(player_info));
  player2->nome = 2;
  player2->hptotal = hp;
  //Alocação da memória do mapa do player2
  player2->mapa = (cell*) malloc((smapa*smapa) * sizeof(cell));
  player2->b = (ship**) malloc((max_boat) * sizeof(ship*));
  player2->nbarco = 0;

  cell_ini(player2->mapa, smapa); //Inicialização do mapa do jogador 2

  printf("\n\n");

  if(random == 0){ //Inicialização aleatoria dos barcos
    player2 = inicio_random(player2, b1, b2, b3, b4, b5, smapa);
  }

  else{ //Inicialização manual dos barcos
    printf("\nSetup Player %d:\n", player2->nome);
    player2 = inicio(player2, b1, b2, b3, b4, b5, smapa);
  }

  print_mapa(player2, smapa);
  printf("\n");

  //Codigo para o inicio dos ataques dos jogadores
  //O código corre enquanto nenhum dos jogadores "morreu"
  while(val != 7){
    defesamm(player2, smapa, fd);//player 2 defende
    if(player2->hptotal == 0){
        printf("Player 1 ganha!");
        goto fim;//salta para o fim do jogo
    }
    val = ataquemm(player2, smapa, fd);//player 2 ataca
    print_val(val);
    if(val == 7){//se val = 7 termina o jogo
        printf("Player 2 ganha!\n");
        goto fim;//salta para o fim do jogo
    }
  }

  fim://salta para este ponto se o player1 ganhar
  freeall(player2, smapa);//Libertação da memória alocada anteriormente
  close(fd);//fecha o fd
  exit(0);
}
