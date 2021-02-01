#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "player.h"
#include <ctype.h>
#include <string.h>

char def[5];

bool contem_cell(int x, int y, struct quadtree* tree){
  float tree_x = tree->x;
  float tree_y = tree->y;
  float w = tree->w;
  float h = tree->h;
  return(x >= tree_x - w &&
    x < tree_x + w &&
    y >= tree_y - h &&
    y < tree_y + h &&
    x >= 0 && y >= 0);
}//completo

cell* retorna_cell(int x, int y, struct quadtree* tree){
  if((contem_cell(x, y, tree))){
    if(!tree->dividido){
      for(int i = 0; i < tree->n_cells; i++){
        if(((tree->cells)+i)->x == x && ((tree->cells)+i)->y == y)
          return ((tree->cells)+i);
      }
      cell *c = (cell*) malloc(sizeof(cell));
      c->x = -1;
      c->y = -1;
      c->tembarco = 0;
      c->shoot = 0;
      return c;
    }
  }

  if(contem_cell(x, y, tree->NW))
    return retorna_cell(x, y, tree->NW);
  if(contem_cell(x, y, tree->NE))
    return retorna_cell(x, y, tree->NE);
  if(contem_cell(x, y, tree->SW))
    return retorna_cell(x, y, tree->SW);
  if(contem_cell(x, y, tree->SE))
    return retorna_cell(x, y, tree->SE);

  cell *c = (cell*) malloc(sizeof(cell));
  c->x = -1;
  c->y = -1;
  c->tembarco = 0;
  c->shoot = 0;
  return c;
}//completo

void subdivide(struct quadtree* tree, player_infoq* player){
  int capacidade = tree->capacidade;
  float limite = tree->limite;
  float x = tree->x;
  float y = tree->y;

  // São Criados todos os quadrantes "filhos" com metade do tamanho
  // E com um novo ponto central associado
  struct quadtree *northeast = (struct quadtree*) malloc(sizeof(struct quadtree));
    northeast->capacidade = capacidade;
    northeast->limite = limite/2;
    northeast->n_cells = 0;
    northeast->x = x + (northeast->limite)/2; // Em relacao ao quadrante "Pai" o ponto central encontra-se a nordeste (x +, y +)
    northeast->y = y + (northeast->limite)/2;
    northeast->w = (northeast->limite)/2;
    northeast->h = (northeast->limite)/2;

    cell* ne_array_p = (cell*) malloc(sizeof(cell)*capacidade); // Array para guardar as células que se encontram no quadrante
    northeast->cells = ne_array_p;

    northeast->dividido = false;

  struct quadtree *northwest = (struct quadtree*) malloc(sizeof(struct quadtree));
    northwest->capacidade = capacidade;
    northwest->limite = limite/2;
    northwest->n_cells = 0;
    northwest->x = x - (northwest->limite)/2; // Em relacao ao quadrante "Pai" o ponto central encontra-se a noroeste (x -, y +)
    northwest->y = y + (northwest->limite)/2;
    northwest->w = (northwest->limite)/2;
    northwest->h = (northwest->limite)/2;

    cell* nw_array_p = (cell*) malloc(sizeof(cell)*capacidade); // Array para guardar as células que se encontram no quadrante
    northwest->cells = nw_array_p;

    northwest->dividido = false;

  struct quadtree *southeast = (struct quadtree*) malloc(sizeof(struct quadtree));
    southeast->capacidade = capacidade;
    southeast->limite = limite/2;
    southeast->n_cells = 0;
    southeast->x = x + (southeast->limite)/2; // Em relacao ao quadrante "Pai" o ponto central encontra-se a sudeste (x +, y -)
    southeast->y = y - (southeast->limite)/2;
    southeast->w = (southeast->limite)/2;
    southeast->h = (southeast->limite)/2;

    cell* se_array_p = (cell*) malloc(sizeof(cell)*capacidade); // Array para guardar as células que se encontram no quadrante
    southeast->cells = se_array_p;

    southeast->dividido = false;

  struct quadtree *southwest = (struct quadtree*) malloc(sizeof(struct quadtree));
    southwest->capacidade = capacidade;
    southwest->limite = limite/2;
    southwest->n_cells = 0;
    southwest->x = x - (southwest->limite)/2; // Em relacao ao quadrante "Pai" o ponto central encontra-se a sudoeste (x -, y -)
    southwest->y = y - (southwest->limite)/2;
    southwest->w = (southwest->limite)/2;
    southwest->h = (southwest->limite)/2;

    cell* sw_array_p = (cell*) malloc(sizeof(cell)*capacidade); // Array para guardar as células que se encontram no quadrante
    southwest->cells = sw_array_p;

    southwest->dividido = false;

  tree->NE = northeast;
  tree->NW = northwest;
  tree->SE = southeast;
  tree->SW = southwest;

  tree->dividido = true;

}//completo

void imprime(cell* celula, int capacidade){ // Função auxiliar para imprimir todos os pontos guardados no quadrante
  for(int i = 0; i < capacidade; i++){
    printf("%d,%d\n", celula->x, celula->y);
  }

}//completo

bool inserir_cell(cell celula, struct quadtree *tree, player_infoq* player){ // Função para inserir uma dada célula numa QTree

  if(!(contem_cell(celula.x, celula.y, tree))){ // Verifica se o uma dada célula se encontra dentro dos limites do quadrante
    return false;
  }

  if(!tree->dividido) // Verifica se a celula ja foi previamente guardado da Quadtree
    for(int i = 0; i < tree->n_cells; i++){
      if(((tree->cells)+i)->x == celula.x && ((tree->cells)+i)->y == celula.y)
        return false;
    }

  if(tree->n_cells < tree->capacidade){ // A celula é guardada no quadrante caso haja espaco dentro dele
      *((tree->cells)+(tree->n_cells)) = celula; // A celula é guarda no array da quadtree correta
      (tree->n_cells)++; // Incrementa o numero total de celulas dentro do quadrante
      return true;
  }

  else{ // O quadrante ja estava dividido. Devemos então procurar o quad "filho" onde irá ficar a celula
    if(!(tree->dividido)){
      subdivide(tree, player); //Criamos os quadrantes "filhos"
      for(int i = 0; i < tree->n_cells; i++){ // Guardamos os pontos que estavam no quadrante "pai" nos respetios "filhos"
        inserir_cell(*((tree->cells)+i), tree, player);
      }
      free(tree->cells); // Libertamos a memória do array que guardava as celulas do nó "pai"
    }

    if(inserir_cell(celula, tree->NE, player)){ // Tentamos inserir a celula no filho NE
      //printf("(%d, %d) Inserido na NE\n", celula.x, celula.y);
      return true;
    }
    else if(inserir_cell(celula, tree->NW, player)){ // Tentamos inserir a celula no filho NW
      //printf("(%d, %d) Inserido na NW\n", celula.x, celula.y);
      return true;
    }
    else if(inserir_cell(celula, tree->SE, player)){ // Tentamos inserir a celula no filho SE
      //printf("(%d, %d) Inserido na SE\n", celula.x, celula.y);
      return true;
    }
    else if(inserir_cell(celula, tree->SW, player)){ // Tentamos inserir a celula no filho SW
      //printf("(%d, %d) Inserido na SW\n", celula.x, celula.y);
      return true;
    }
  }
  return true;
}//COmpleto

void cell_ini(cell* mapa, int smapa){ //Inicia as celulas do mapa
    smapa = smapa*smapa;
    for(int i = 0; i < smapa; i++){
      (mapa + i)->barco = NULL;
      (mapa + i)->shoot = 0; // 0 -> ainda n testado, 2 -> falhou, 1-> acertou
    }
}//completo

ship barco(int tipo, int angulo){ //Da return a um barco com o bitmap do tipo e angulo desejados
    ship b;
    b.hp = 0;
    b.barc = (int*) malloc(25*sizeof(int));
    /*
    | 0  | 1  | 2  |  3 |  4 |
    | 5  | 6  | 7  | 8  |  9 |
    | 10 | 11 | 12 | 13 | 14 |
    | 15 | 16 | 17 | 18 | 19 |
    | 20 | 21 | 22 | 23 | 24 |
    */
    for(int i = 0; i < 25; i++)
        *(b.barc) = 0;

    switch(tipo){
        case 1: // Barco: L
          if(angulo == 0){
            *(b.barc+7) = 1;
            *(b.barc+12) = 1;
            *(b.barc+17) = 1;
            *(b.barc+18) = 1;
          }
          if(angulo == 90){
            *(b.barc+11) = 1;
            *(b.barc+12) = 1;
            *(b.barc+13) = 1;
            *(b.barc+8) = 1;
          }
          if(angulo == 180){
            *(b.barc+7) = 1;
            *(b.barc+12) = 1;
            *(b.barc+17) = 1;
            *(b.barc+6) = 1;
          }
          if(angulo == 270){
            *(b.barc+11) = 1;
            *(b.barc+12) = 1;
            *(b.barc+13) = 1;
            *(b.barc+16) = 1;
          }
            b.hp +=4;
        break;

        case 2: // Barco: +
                *(b.barc+7) = 1;
                *(b.barc+11) = 1;
                *(b.barc+12) = 1;
                *(b.barc+13) = 1;
                *(b.barc+17) = 1;
                b.hp +=5;
        break;

        case 3: // Barco: Linha 3
            if(angulo == 0 || angulo == 180){
                *(b.barc+11) = 1;
                *(b.barc+12) = 1;
                *(b.barc+13) = 1;
            }
            else{
                *(b.barc+17) = 1;
                *(b.barc+12) = 1;
                *(b.barc+7) = 1;
            }
            b.hp +=3;
        break;

        case 4: // Barco: Quadrado
              for(int i = 1; i < 4; i++)
              for(int j = 1; j < 4; j++)
                  *(b.barc+i+(j*5)) = 1;
                b.hp +=9;
        break;

        case 5: // Barco: Linha 5
            if(angulo == 0 || angulo == 180){
                *(b.barc+10) = 1;
                *(b.barc+11) = 1;
                *(b.barc+12) = 1;
                *(b.barc+13) = 1;
                *(b.barc+14) = 1;
            }
            else{
                *(b.barc+22) = 1;
                *(b.barc+17) = 1;
                *(b.barc+12) = 1;
                *(b.barc+7) = 1;
                *(b.barc+2) = 1;
            }
            b.hp +=5;
        break;
    }

/*    for(int i = 0; i < 5; i++){
      for(int j = 0; j < 5; j++){
        printf("%d", *(b.barc+(i*5)+j));
      }
      printf("\n");
    }
*/
    return b;
}//completo

bool testeangulo(int ori){ //Verifica se o angulo e valido
  if(ori != 0 && ori != 90 && ori != 180 && ori != 270) return false;
  else return true;
}//completo

int testenum(char str[]){ //Verifica se todos os elementos da string sao digitos e retorna o valor desta como um int
    int value;
    if (atoi(str)!=0) value=atoi(str);
    else{
        if(isdigit(str[0])) value=0;
        else value=-1;
    }
    memset(def, '\0', 5);
    return value;
}//completo

void print_val(int val){//impreme o estado do disparo para a funçoes multi-shell
  switch (val) {
    case -2:
    printf("Tiro repetido\n");
    break;
    case 1:
    printf("Tiro Falhou\n");
    break;
    case 2:
    printf("Hit!\n");
    break;
    case 3:
    printf("Hit!\nBarco ao Fundo\n");
    break;
  }
}

void print_mapa2(player_info* player, int smapa){ //Imprime o mapa com o estado dos disparos
  int aux = 1;

  //AUXILIO VISUAL COM AS COORDENADAS X, Y
  printf("     ");
   for (int i = 0; i < smapa; ++i){ //Imprime a linha do topo do mapa
       if(aux<9){
           printf("%d  ", aux);
           aux++;
       }
       else if(aux==9){
           printf("%d ", aux);
           aux++;
       }
       else{
           printf("%d ", aux);
           aux++;
       }

   }
   printf("\n");

   printf("     ");
   for (int i = 0; i < smapa; ++i){
       printf("|  ");
   }
   printf("\n");

  for(int i = 0; i < smapa; i++){
    if (i<9) printf(" %d - ",i+1); //imprime os valores do Y
    else printf("%d - ",i+1);

    for(int j = 0; j < smapa; j++){
        printf("%d  ", ((player->mapa + i*(smapa) + j)->shoot)); //Imprime a variavel "shoot" da celula do mapa
    }
    printf("\n");
  }
}//completo

void print_mapa(player_info* player, int smapa){//DEBUG - Imprime se uma posicao tem barco
  int aux = 1;

  //AUXILIO VISUAL COM AS COORDENADAS X, Y
  printf("     ");
   for (int i = 0; i < smapa; ++i){ //Imprime a linha do topo do mapa
       if(aux<9){
           printf("%d  ", aux);
           aux++;
       }
       else if(aux==9){
           printf("%d ", aux);
           aux++;
       }
       else{
           printf("%d ", aux);
           aux++;
       }

   }
   printf("\n");

   printf("     ");
   for (int i = 0; i < smapa; ++i){
       printf("|  ");
   }
   printf("\n");

  for(int i = 0; i < smapa; i++){
    if (i<9) printf(" %d - ",i+1); //imprime os valores do Y
    else printf("%d - ",i+1);

    for(int j = 0; j < smapa; j++){
        printf("%d  ", ((player->mapa + i*(smapa) + j)->tembarco)); //Imprime a variavel "shoot" da celula do mapa
    }
    printf("\n");
  }
}//completo

void print_mapap(player_info* player, int smapa){//DEBUG - Imprime os pointers de cada celula do mapa
  for(int i = 0; i < smapa; i++){
    for(int j = 0; j < smapa; j++){
      printf("%p",((player->mapa) + i*(smapa) + j)->barco);
    }
    printf("\n");
  }
}//completo

void print_mapa2q(player_infoq* player, int smapa){ //Imprime o mapa com o estado dos disparos para Qtree
  int aux = 1;

  //AUXILIO VISUAL COM AS COORDENADAS X, Y
  printf("     ");
   for (int i = 0; i < smapa; ++i){ //Imprime a linha do topo do mapa
       if(aux<9){
           printf("%d  ", aux);
           aux++;
       }
       else if(aux==9){
           printf("%d ", aux);
           aux++;
       }
       else{
           printf("%d ", aux);
           aux++;
       }

   }
   printf("\n");

   printf("     ");
   for (int i = 0; i < smapa; ++i){
       printf("|  ");
   }
   printf("\n");

  for(int i = 0; i < smapa; i++){
    if (i<9) printf(" %d - ",i+1); //imprime os valores do Y
    else printf("%d - ",i+1);

    for(int j = 0; j < smapa; j++){
        cell* c = retorna_cell(j, i, player->mapa);
        printf("%d  ", c->shoot); //Imprime a variavel "shoot" da celula do mapa
    }
    printf("\n");
  }
}//completo

void print_mapaq(player_infoq* player, int smapa){//DEBUG - Imprime se uma posicao tem barco para Qtree
  int aux = 1;

  //AUXILIO VISUAL COM AS COORDENADAS X, Y
  printf("     ");
   for (int i = 0; i < smapa; ++i){ //Imprime a linha do topo do mapa
       if(aux<9){
           printf("%d  ", aux);
           aux++;
       }
       else if(aux==9){
           printf("%d ", aux);
           aux++;
       }
       else{
           printf("%d ", aux);
           aux++;
       }

   }
   printf("\n");

   printf("     ");
   for (int i = 0; i < smapa; ++i){
       printf("|  ");
   }
   printf("\n");

  for(int i = 0; i < smapa; i++){
    if (i<9) printf(" %d - ",i+1); //imprime os valores do Y
    else printf("%d - ",i+1);

    for(int j = 0; j < smapa; j++){
        cell* c = retorna_cell(j, i, player->mapa);
        printf("%d  ", c->tembarco); //Imprime a variavel "shoot" da celula do mapa
    }
    printf("\n");
  }
}//completo

bool verificar_barcoq(int xi, int yi, player_infoq* player, ship* navio, int smapa){ //A funcao verifica se as posiçoes no mapa estao disponiveis para Qtree
    for(int i = -2; i < 3; i++){
      for(int j = -2; j < 3; j++){
        if(*((navio->barc)+(i+2)*5 + (j+2)) == 1){ //Caso a posicao no bitmap esteja ocupada o programa verifica a disponiblidade daquela posicao relativa no mapa do jogador
          if((xi+j) < 0 || (xi + j) >= smapa || (yi + i) < 0 || (yi +i) >= smapa){
            return true; //Verifica se as posiçoes estao dentro dos limites do mapa
          }
          cell* c = retorna_cell(xi+j, yi+i, player->mapa);
          if(c->tembarco == 1){
            //printf("Erro em x: %d, y: %d\n", xi+j+1, yi+i+1);
            return true;//verifica se a posicao ja tem um barco
          }
        }
      }
    }
    return false;
}//completo

void insert_barcoq(player_infoq* player, int smapa, int tipo, int ori){ //Insere barcos numa determinada posicao para Qtree
  ship* navio = (ship*) malloc(sizeof(ship*)); //Aloca espaço na memoria para o barco
  *navio = barco(tipo, ori); //usa a funçao barco para receber o bitmap do barco
  player->b[player->nbarco] = navio;
  player->nbarco++;

  lercoor: //caso as coordenadas sejam invalidas
  printf("Qual é o X central?");
  scanf("%s", def);
  int xi = testenum(def);
  xi--;

  printf("Qual é o Y central?");
  scanf("%s", def);
  int yi = testenum(def);
  yi--;

  if(verificar_barcoq(xi, yi, player, navio, smapa) == true) { //Verifica se as posiçoes no mapa estao disponiveis
    printf("Existe sobreposição de barcos/coordenadas invalidas.\nEscolha outra posição\n");
    goto lercoor; //O programa volta a pedir as coordenadas
  }

  for(int i = -2; i < 3; i++){
    for(int j = -2; j < 3; j++){
      if(*((navio->barc)+ (i+2)*5 + (j+2)) == 1){ //caso a posicao no bitmap esteja ocupada, o programa procegue
        cell c;
        c.x = xi + j;
        c.y = yi + i;
        c.shoot = 0;
        (c.barco) = navio; //aponta a celula do mapa para o barco
        (c.tembarco) = 1;//altera o estado da celula para 1, ou seja "existe um barco nesta celula"
        inserir_cell(c, player->mapa, player);
      }
    }
  }
  print_mapaq(player, smapa);

  //DEBUG
  //print_pointers(player, smapa);
  //print_mapaq(player, smapa);
}//completo

void insert_barcoq_random(player_infoq* player, int smapa, int tipo){ //Insere um barco aleatoriamente para Qtree
  int xi, yi, ori;
  bool teste = false;

  ship* navio = (ship*) malloc(sizeof(ship*)); //aloca o espaco necessraio para a criacao do barco
  do{
    ori = rand() % 4; //Gera uma orientacao aleatoria

    switch (ori) {
      case 1:
        ori = 0;
        break;
      case 2:
        ori = 90;
        break;
      case 3:
        ori = 180;
        break;
      case 4:
        ori = 270;
        break;
      }
    *navio = barco(tipo, ori); //usa a funçao barco para receber o bitmap do barco
    xi = rand() % (smapa + 1); //Gera a coordenada x para posicionar o barco
    yi = rand() % (smapa + 1); //Gera a coordenada y para posicionar o barco
    teste = verificar_barcoq(xi, yi, player, navio, smapa);

  }while(teste == true);

  for(int i = -2; i < 3; i++){
    for(int j = -2; j < 3; j++){
      if(*((navio->barc)+ (i+2)*5 + (j+2)) == 1){ //caso a posicao no bitmap esteja ocupada, o programa procegue
        cell c;
        c.x = xi + j;
        c.y = yi + i;
        c.shoot = 0;
        (c.barco) = navio; //aponta a celula do mapa para o barco
        (c.tembarco) = 1;//altera o estado da celula para 1, ou seja "existe um barco nesta celula"
        inserir_cell(c, player->mapa, player);

      }
    }
  }
  //DEBUG
  //print_mapaq(player, smapa);

    player->b[player->nbarco] = navio;
    player->nbarco++;
}//completo

bool atirarq(player_infoq* defensor, int x, int y,int smapa){ //Se o retrun for true = ataque concluido com sucesso para Qtree

  if(x < 0 || x >= smapa || y < 0 || y >= smapa){ //Verifica se as coordenadas estao dentro dos limites do mapa
    printf("Coordenadas invalidas\n");
    return false;
  }
  cell* c = retorna_cell(x, y, defensor->mapa);
  if(c->tembarco == 1){ //Verifica se existe um barco nesta posicao

    defensor->hptotal--;
    c->tembarco = 0; //"Retira" o barco para que o mesmo nao seja detetado numa proxime iteraçao
    c->shoot = 1; //Guarda o valor 1 (Acertou no inimigo)
    printf("\nHit!\n");

    if(--(c->barco->hp) <= 0){ //Caso o hp do barco seja 0, o barco afunda
      printf("Barco ao fundo!\n");
    }
  }
  else if(c->shoot != 0){ //Disparo repetido
    printf("\nTiro repetido\n");
    return true;
  }
  else{
    c->shoot = 2; //Guarda o valor 2 (Falhou o inimigo)
    printf("\nFalhou\n");
  }
  return true;
}

int atirarqm(player_infoq* defensor, int x, int y,int smapa){ //retorna o estado do dispara para Qtree multi-shell
  // -2 -> Tiro repetido
  // 0 -> erro
  // 1 -> falhou
  // 2 -> hit
  // 3 -> hit + fundo

  cell* c = retorna_cell(x, y, defensor->mapa);//vai buscar os dados da posicao desejada
  if(c->tembarco == 1){ //Verifica se existe um barco nesta posicao

    defensor->hptotal--;
    c->tembarco = 0; //"Retira" o barco para que o mesmo nao seja detetado numa proxime iteraçao
    c->shoot = 1; //Guarda o valor 1 (Acertou no inimigo)

    if(--(c->barco->hp) <= 0){ //Caso o hp do barco seja 0, o barco afunda
      return 3;//hit + fundo
    }
    return 2;//hit
  }
  else if(c->shoot != 0){ //Disparo repetido
    return -2;//Tiro repetido
  }
  else{
    c->shoot = 2; //Guarda o valor 2 (Falhou o inimigo)
    return 1;//falhou
  }
  return 0;//nunca ocorre
}

void ataqueq(player_infoq* atacante, player_infoq* defensor, int smapa){//leitura de coordenadas para o ataque para Qtree

    printf("\nMapa do Player %d\n", defensor->nome);
    print_mapa2q(defensor, smapa);
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

    if(atirarq(defensor, x, y, smapa) == false) goto attc;//O Ciclo repete se forem coordenadas invalidas/repetidas
}

player_infoq* inicioq(player_infoq* player, int b1, int b2, int b3, int b4, int b5, int smapa){//le posiçoes iniciais para Qtree
  int ori = 0;

  for(int i = 1; i <= b1; i++){//barco tipo L
      printf("%dº barco de tipo L\n", i);

      ang1: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang1;
      }
      insert_barcoq(player, smapa, 1, ori);
  }

  for(int i = 1; i <= b2; i++){//barco tipo +
      printf("%dº barco de tipo +\n", i);

      insert_barcoq(player, smapa, 2, ori);
  }

  for(int i = 1; i <= b3; i++){//barco tipo linha 3
      printf("%dº barco de tipo 3\n", i);

      ang3: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?\n");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang3;
      }

      insert_barcoq(player, smapa, 3, ori);
  }

  for(int i = 1; i <= b4; i++){//barco tipo quadrado
      printf("%dº barco de tipo quadrado\n", i);

      insert_barcoq(player, smapa, 4, ori);
  }

  for(int i = 1; i <= b5; i++){//barco tipo linha 5
      printf("%dº barco de tipo 5\n", i);

      ang5: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?\n");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang5;
      }

      insert_barcoq(player, smapa, 5, ori);
  }

    return player;
}//completo

player_infoq* inicioq_random(player_infoq* player, int b1, int b2, int b3, int b4, int b5, int smapa){//manda o tipo de barco para o inserir barco para Qtree
  for(int i = 1; i <= b1; i++) insert_barcoq_random(player, smapa, 1);//barco tipo L

  for(int i = 1; i <= b2; i++) insert_barcoq_random(player, smapa, 2);//barco tipo +

  for(int i = 1; i <= b3; i++) insert_barcoq_random(player, smapa, 3);//barco tipo linha 3

  for(int i = 1; i <= b4; i++) insert_barcoq_random(player, smapa, 4);//barco tipo quadrado

  for(int i = 1; i <= b5; i++) insert_barcoq_random(player, smapa, 5);//barco tipo linha 5

  return player;
}//completo

void freeallq(player_infoq* player, int smapa){//free da memoria total para Qtree
  for(int i = (player->nbarco); i>=0; i--){//ciclo para dar free de barcos
    free(player->b[i]);
  }

  free(player->mapa);//free do mapa
  free(player);//free do player
}

struct quadtree* start_qtree(int smapa){
  struct quadtree* Tree = (struct quadtree*) malloc(sizeof(struct quadtree));
  Tree->capacidade = 5;
  Tree->limite = smapa;
  Tree->n_cells = 0;
  Tree->x = (Tree->limite)/2;
  Tree->y = (Tree->limite)/2;
  Tree->h = (Tree->limite)/2;
  Tree->w = (Tree->limite)/2;
  cell* Tree_array_p = (cell*) malloc(sizeof(cell)*Tree->capacidade);
  Tree->cells = Tree_array_p;

  return Tree;
}

bool verificar_barco(int xi, int yi, player_info* player, ship* navio, int smapa){ //A funcao verifica se as posiçoes no mapa estao disponiveis
    for(int i = -2; i < 3; i++){
      for(int j = -2; j < 3; j++){
        if(*((navio->barc)+(i+2)*5 + (j+2)) == 1){ //Caso a posicao no bitmap esteja ocupada o programa verifica a disponiblidade daquela posicao relativa no mapa do jogador
          if((xi+j) < 0 || (xi + j) >= smapa || (yi + i) < 0 || (yi +i) >= smapa) return true; //Verifica se as posiçoes estao dentro dos limites do mapa
          if(((player->mapa + (i+yi)*(smapa) + (j+xi))->tembarco) == 1) return true;//verifica se a posicao ja tem um barco
        }
      }
    }
    return false;
}//completo

void insert_barco(player_info* player, int smapa, int tipo, int ori){ //Insere barcos numa determinada posicao
  ship* navio = (ship*) malloc(sizeof(ship*)); //Aloca espaço na memoria para o barco
  *navio = barco(tipo, ori); //usa a funçao barco para receber o bitmap do barco
  player->b[player->nbarco] = navio;
  player->nbarco++;
  lercoor: //caso as coordenadas sejam invalidas
  printf("Qual é o X central?");
  scanf("%s", def);
  int xi = testenum(def);
  xi--;

  printf("Qual é o Y central?");
  scanf("%s", def);
  int yi = testenum(def);
  yi--;

  if(verificar_barco(xi, yi, player, navio, smapa) == true) { //Verifica se as posiçoes no mapa estao disponiveis
    printf("Existe sobreposição de barcos/coordenadas invalidas.\nEscolha outra posição\n");
    goto lercoor; //O programa volta a pedir as coordenadas
  }

  for(int i = -2; i < 3; i++){
    for(int j = -2; j < 3; j++){
      if(*((navio->barc)+ (i+2)*5 + (j+2)) == 1){ //caso a posicao no bitmap esteja ocupada, o programa procegue
        ((player->mapa + (i+yi)*(smapa) + j+xi)->barco) = navio; //aponta a celula do mapa para o barco
        ((player->mapa + (i+yi)*(smapa) + j+xi)->tembarco) = 1; //altera o estado da celula para 1, ou seja "existe um barco nesta celula"
      }
    }
  }
  //DEBUG
  //print_pointers(player, smapa);
  //print_mapa(player, smapa);
}//completo

void insert_barco_random(player_info* player, int smapa, int tipo){ //Insere um barco aleatoriamente
  int xi, yi, ori;
  bool teste = false;

  ship* navio = (ship*) malloc(sizeof(ship*)); //aloca o espaco necessraio para a criacao do barco
  player->b[player->nbarco] = navio;
  player->nbarco++;
  do{
    ori = rand() % 4; //Gera uma orientacao aleatoria

    switch (ori) {
      case 1:
        ori = 0;
        break;
      case 2:
        ori = 90;
        break;
      case 3:
        ori = 180;
        break;
      case 4:
        ori = 270;
        break;
      }
    *navio = barco(tipo, ori); //usa a funçao barco para receber o bitmap do barco

    xi = rand() % (smapa + 1); //Gera a coordenada x para posicionar o barco
    yi = rand() % (smapa + 1); //Gera a coordenada y para posicionar o barco
    teste = verificar_barco(xi, yi, player, navio, smapa);

  }while(teste == true);

  for(int i = -2; i < 3; i++){
    for(int j = -2; j < 3; j++){
      if(*((navio->barc)+ (i+2)*5 + (j+2)) == 1){ //caso a posicao no bitmap esteja ocupada, o programa procegue
        ((player->mapa + (i+yi)*(smapa) + j+xi)->barco) = navio; //aponta a celula do mapa para o barco
        ((player->mapa + (i+yi)*(smapa) + j+xi)->tembarco) = 1; //aletra o estado da celula para 1, ou seja "existe um barco nesta celula"
      }
    }
  }
  //DEBUG
  //print_mapa(player, smapa);

}//completo

bool atirar(player_info* defensor, int x, int y,int smapa){ //Se o retrun for true = ataque concluido com sucesso

  if(x < 0 || x >= smapa || y < 0 || y >= smapa){ //Verifica se as coordenadas estao dentro dos limites do mapa
    printf("Coordenadas invalidas\n");
    return false;
  }

  if(((defensor->mapa)+ (y*smapa) + x)->tembarco == 1){ //Verifica se existe um barco nesta posicao

    defensor->hptotal--;
    ((defensor->mapa)+ (y*smapa) + x)->tembarco = 0; //"Retira" o barco para que o mesmo nao seja detetado numa proxime iteraçao
    ((defensor->mapa)+ (y*smapa) + x)->shoot = 1; //Guarda o valor 1 (Acertou no inimigo)
    printf("\nHit!\n");

    if(--(((defensor->mapa)+ (y*smapa) + x)->barco->hp) <= 0){ //Caso o hp do barco seja 0, o barco afunda
      printf("Barco ao fundo!\n");
    }
  }
  else if(((defensor->mapa)+ (y*smapa) + x)->shoot != 0){ //Disparo repetido
    printf("\nTiro repetido\n");
    return true;
  }
  else{
    ((defensor->mapa)+ (y*smapa) + x)->shoot = 2; //Guarda o valor 2 (Falhou o inimigo)
    printf("\nFalhou\n");
  }
  return true;
}

int atirarmm(player_info* defensor, int x, int y,int smapa){ //da return a um valor de acordo com o disparo
// -2 -> Tiro repetido
// 0 -> erro
// 1 -> falhou
// 2 -> hit
// 3 -> hit + fundo

  if(((defensor->mapa)+ (y*smapa) + x)->tembarco == 1){ //Verifica se existe um barco nesta posicao

    defensor->hptotal--;
    ((defensor->mapa)+ (y*smapa) + x)->tembarco = 0; //"Retira" o barco para que o mesmo nao seja detetado numa proxime iteraçao
    ((defensor->mapa)+ (y*smapa) + x)->shoot = 1; //Guarda o valor 1 (Acertou no inimigo)

    if(--(((defensor->mapa)+ (y*smapa) + x)->barco->hp) <= 0){ //Caso o hp do barco seja 0, o barco afunda
      return 3;//hit + fundo
    }
    return 2;//hit
  }
  else if(((defensor->mapa)+ (y*smapa) + x)->shoot != 0){ //Disparo repetido
    return -2;//Tiro repetido
  }
  else{
    ((defensor->mapa)+ (y*smapa) + x)->shoot = 2; //Guarda o valor 2 (Falhou o inimigo)
    return 1;//falhou
  }
  return 0;//nunca ocorre
}

void ataque(player_info* atacante, player_info* defensor, int smapa){//leitura de coordenadas para o ataque

    printf("\nMapa do Player %d\n", defensor->nome);
    print_mapa2(defensor, smapa);
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

    if(atirar(defensor, x, y, smapa) == false) goto attc;//O Ciclo repete se forem coordenadas invalidas/repetidas
}

player_info* inicio(player_info* player, int b1, int b2, int b3, int b4, int b5, int smapa){//inicia a posicao dos barcos
  int ori = 0;
  for(int i = 1; i <= b1; i++){//barco tipo L
      printf("%dº barco de tipo L\n", i);

      ang1: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang1;
      }
      insert_barco(player, smapa, 1, ori);
  }

  for(int i = 1; i <= b2; i++){//barco tipo +
      printf("%dº barco de tipo +\n", i);

      insert_barco(player, smapa, 2, ori);
  }

  for(int i = 1; i <= b3; i++){//barco tipo linha 3
      printf("%dº barco de tipo 3\n", i);

      ang3: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?\n");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang3;
      }

      insert_barco(player, smapa, 3, ori);
  }

  for(int i = 1; i <= b4; i++){//barco tipo quadrado
      printf("%dº barco de tipo quadrado\n", i);

      insert_barco(player, smapa, 4, ori);
  }

  for(int i = 1; i <= b5; i++){//barco tipo linha 5
      printf("%dº barco de tipo 5\n", i);

      ang5: //caso o angulo seja invalido o programa regressa a este ponto
      printf("Qual é o angulo do barco?\n");
      scanf("%s", def);
      int ori = testenum(def); //Verifica se o valor e um inteiro

      if(testeangulo(ori) == false){
        printf("Angulo não é valido\n");
        goto ang5;
      }

      insert_barco(player, smapa, 5, ori);
  }

    return player;
}//completo

player_info* inicio_random(player_info* player, int b1, int b2, int b3, int b4, int b5, int smapa){//manda o tipo de barco para o inserir barco
  for(int i = 1; i <= b1; i++) insert_barco_random(player, smapa, 1);//barco tipo L

  for(int i = 1; i <= b2; i++) insert_barco_random(player, smapa, 2);//barco tipo +

  for(int i = 1; i <= b3; i++) insert_barco_random(player, smapa, 3);//barco tipo linha 3

  for(int i = 1; i <= b4; i++) insert_barco_random(player, smapa, 4);//barco tipo quadrado

  for(int i = 1; i <= b5; i++) insert_barco_random(player, smapa, 5);//barco tipo linha 5

  return player;
}//completo

void freeall(player_info* player, int smapa){
  for(int i = (player->nbarco); i>=0; i--){//Liberta a memoria alocada para os barcos
    free(player->b[i]);
  }
  free(player->mapa); //Liberta a memoria alocada para o mapa
  free(player); //Liberta a memoria alocada para a variavel "player"

}
