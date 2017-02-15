/* Struttura dello squalo */
struct squalo{
	int sb;
	int sd;
};

/* Struttura generica di un animale presente nel mondo wator */
union animal{
	struct squalo shark; /* squalo */
	int fb; /* pesce */
};

/* Generico punto nel mondo wator */
struct type{
	int klife;
	int type; /* 0 - acqua
		     1 - pesce
		     2 - squalo */
	union animal animale;
};

/* Mondo wator */
typedef struct type** world;

/* Definizione  delle varie macro */

#define TRUE 1
#define FALSE 0

/* Definizione dei vari tipi */
#define WATER 0
#define FISH 1
#define SHARK 2

/* Macro per il calcolo del modulo */
#define MOD(num,mod) (num<0)?(num)+mod:(num)%mod

/* Macro per il calcolo dell'ultima colonna 
 * che il tred deve gestire a partire dalla prima */
#define LSUP(x) x+(cols/N)-1

/* Macro per lo spostamento del generico animale */
#define MOVE(r2,c2,r1,c1) matrix[r2][c2].type=matrix[r1][c1].type;\
			 matrix[r2][c2].klife=kron+1;\
	      if(matrix[r1][c1].type==1){ matrix[r2][c2].animale.fb=matrix[r1][c1].animale.fb-1;\
		}else{\
			    matrix[r2][c2].animale.shark.sb=matrix[r1][c1].animale.shark.sb-1;\
		            matrix[r2][c2].animale.shark.sd=matrix[r1][c1].animale.shark.sd-1;}\
                            matrix[r1][c1].type=0;

/* Macro per la riproduzione del generico animale */
#define BORN(r,c,tipo)  matrix[r][c].type=tipo;\
			matrix[r][c].klife=kron+1;\
			  if(tipo==1)matrix[r][c].animale.fb=Fb;\
			  else{\
		          matrix[r][c].animale.shark.sb=Sb;\
                          matrix[r][c].animale.shark.sd=Sd;}
                          
