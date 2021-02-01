typedef struct {
  int* barc; //guarda o bitmap do barco
  int hp;
}ship;

typedef struct {
  ship* barco; //aponta para um barco que esta presente na cell
  int tembarco; //0 = Não tem barco, 1 = Tem barco
  int shoot; //0 = no shot, 1= shot that didn't hit, 2=shot that hit an enemy piece
  int x, y;//x e y para Qtree
} cell;

typedef struct{
  int hptotal;
  cell* mapa;
  ship** b;//array para guardar barcos com o objetivo de libertaçao de memoria
  int nbarco;//numero de barcos
  int nome;
}player_info;

struct quadtree{
  struct quadtree *NE;
  struct quadtree *NW;
  struct quadtree *SE;
  struct quadtree *SW;

  int capacidade, n_cells;
  float h, w, x, y, limite;
  bool dividido;
  cell *cells;
};

typedef struct{
  int hptotal;
  struct quadtree* mapa;
  int nome;
  int nbarco;//numero de barcos
  ship** b;//array para guardar barcos com o objetivo de libertaçao de memoria
}player_infoq;

bool contem_cell(int x, int y, struct quadtree* tree);

cell* retorna_cell(int x, int y, struct quadtree* tree);

void subdivide(struct quadtree* tree, player_infoq* player);

void imprime(cell* celula, int capacidade);

bool inserir_cell(cell celula, struct quadtree *tree, player_infoq* player);

ship barco(int tipo, int angulo);

int testenum(char str[]);

bool testeangulo(int ori);

void cell_ini(cell* mapa, int smapa);

void print_val(int val);

void print_mapa2(player_info* player, int smapa);

void print_mapa(player_info* player, int smapa);

void print_mapap(player_info* player, int smapa);

void print_mapa2q(player_infoq* player, int smapa);

void print_mapaq(player_infoq* player, int smapa);

bool verificar_barcoq(int xi, int yi, player_infoq* player, ship* navio, int smapa);

bool verificar_barco(int xi, int yi, player_info* player, ship* navio, int smapa);

void insert_barcoq(player_infoq* player, int smapa, int tipo, int ori);

void insert_barco(player_info* player, int smapa, int tipo, int ori);

void insert_barcoq_random(player_infoq* player, int smapa, int tipo);

void insert_barco_random(player_info* player, int smapa, int tipo);

bool atirarq(player_infoq* defensor, int x, int y,int smapa);

bool atirar(player_info* defensor, int x, int y,int smapa);

int atirarmm(player_info* defensor, int x, int y,int smapa);

int atirarqm(player_infoq* defensor, int x, int y,int smapa);

void ataqueq(player_infoq* atacante, player_infoq* defensor, int smapa);

void ataque(player_info* atacante, player_info* defensor, int smapa);

player_infoq* inicioq(player_infoq* player, int b1, int b2, int b3, int b4, int b5, int smapa);

player_info* inicio(player_info* player, int b1, int b2, int b3, int b4, int b5, int smapa);

player_infoq* inicioq_random(player_infoq* player, int b1, int b2, int b3, int b4, int b5, int smapa);

player_info* inicio_random(player_info* player, int b1, int b2, int b3, int b4, int b5, int smapa);

void freeallq(player_infoq* player, int smapa);

void freeall(player_info* player, int smapa);

struct quadtree* start_qtree(int smapa);
