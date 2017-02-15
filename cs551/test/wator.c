#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
//#include "pthread.h"
#include <limits.h>
#include <unistd.h>
//#include <mcheck.h>

#include "wator.h"
/*MACRO*/
#define PRINT_ERR(arg) fprintf (stderr,arg); fflush(stderr);
#define TESTVAR(var, cmp ,errnoval,strerr) if ( var == cmp ) {\
	fprintf(stderr, strerr);\
	fflush(stderr);\
	errno = errnoval ;\
	exit(EXIT_FAILURE);\
}

/*DEBUG*/
#define SLEEP
#ifdef SLEEP
/*Se si vuole redirigere l'out su file la clear va disabilitata commentando questa riga*/
#define CLEAR
#else
#define DEBUG 
 #define DEBUG_THREAD 
/* #define DEBUG_THREAD_SYN */
/* #define DEBUG_THREAD_MTX */
/* #define DEBUG_ACTION */
/* #define DEBUG_MANGIA */
/* #define DEBUG_RIPRODUCI */
/* #define DEBUG_SPOSTA */
/* #define DEBUG_WORKER */
/* #define DEBUG_POPOLAMAT */
#endif
/*DICHIARAZIONE FUNZIONI*/
static void popolaMat(int Ns, int Nf, int row, int cols, abi_t** mare); /*Popola la matrice con ns squali e nf pesci in modo casuale*/
static abi_t** creaMatrice(int row, int cols);/* alloca in memoria una matrice row*cols di abi_t*/
int ceckInt(char *str);/*controlla se una stringa passata contiene solo cifre*/
void *visualizer();/*funzione main dal thread visualizer*/
static void printmat(abi_t** a);/*stampa la matrice a schermo senza multithreading*/
static int sposta(int curow, int cucols);/*sposta un elemento nella matrice casualmente in una cella adiacente*/
void *worker(void* data);/*funzione main dei thread worker*/
static int riproduci(int row, int cols);/*fa riprodurre l'elemento nella cella (row ,cols)*/
static int mangia(int row, int cols);/*fa mangiare ad uno squalo un pesce adiacente in modo casuale*/
static int rowincr(int curow);/*incrementa curow in modulo con row*/
static int rowdecr(int curow);/*decrementa*/
static int colsdecr(int cucols);/*decrementa cucols in modulo con cols*/
static int colsincr(int cucols);/*incrementa*/
static void clearAbiCell(abi_t** mat, int row, int cols);/* cancella un elemento da mat in posizione (row, cols)*/
static void stop(int curow, int cucols);/*simula la non azione di un elemento durante l'aggiornamento della cella*/
static int muore(int curow, int cucols);/* simula la morte di uno squalo e di un pesce*/

/*VARIABILI GLOBALI*/
/*Contatore crhonon passati da t=1*/
static int chronon_count = 1;
/*Limite massimo chronon simulazione*/
static int max_chron;
/*Parametri*/
static int row;
static int cols;
static int Sd; /* uno squalo non mangia per Sd chronon muore*/
static int Sb; /* n chron, Sb lo squalo si riproduce*/
static int Fb; /* n chron, Fb il pesce si riproduce*/
static int NF; /* numero di pesci presenti all'inizio */
static int NS; /* numero di squali presenti all'inizio */
static int n_thread; /*Numero dei thread che si occupano dell'aggiornamento delle tabelle*/

/*MAtrici della simulazione*/
static abi_t** mat_bef; /*rappresenta la matrice prima dell'aggiornamento al chronon succ*/
static abi_t** mat_aft; /*Matrice successiva all'aggiornamento*/

/*Mutex  per la mutua esclusione per la scrittura sui numeri di pesci e squali*/
static pthread_mutex_t NSmtx= PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t NFmtx= PTHREAD_MUTEX_INITIALIZER;

/*Mutex e cond per la sincronizzazione degli aggiornamenti*/
static pthread_mutex_t waitermtx= PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond= PTHREAD_COND_INITIALIZER;
static int waiter = 0;

/*Mutex e cond per la sincronizzazione tra worker e visualizer*/
static pthread_mutex_t vismtx= PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t viscond_v= PTHREAD_COND_INITIALIZER;
static pthread_cond_t viscond_w= PTHREAD_COND_INITIALIZER;
int visCount = 0;

/*FINE VARIABILI GLOBALI*/

int main(int argc, char* argv[]) {
	void* exstatus;
	pthread_t* tid;
	pthread_mutex_t *arr_mtx_base, *arrmtx;
	int i, cols_rate, cols_r;
	char *tvalue= NULL;
	int c;
	printf("porcone ladro porco\n\n\n");
/*argv[] nome rows cols Sb Sd Fb NF NS [-t N] */
#ifdef DEBUG
	mtrace();
#endif
	if (argc <= 7) {
		PRINT_ERR("Numero di Parametri Insufficente")
		exit(EXIT_FAILURE);
	}
	/* CONTROLLO E INIZIALIZZAZIONE VARIABILI GLOBALI */
	if (!(ceckInt(argv[1]) && ceckInt(argv[2]) && ceckInt(argv[3])
			&& ceckInt(argv[4]) && ceckInt(argv[5]) && ceckInt(argv[6])
			&&ceckInt(argv[7]))) {
		PRINT_ERR("Parametri immessi incorretti")
		exit(EXIT_FAILURE);
	}
	/*Prendo le variabili dai parametri passati almain*/
	row = atoi(argv[1]);
	cols = atoi(argv[2]);
	Sb = atoi(argv[3]); /* n chron, Sb lo squalo si riproduce*/
	Sd = atoi(argv[4]); /* uno squalo non mangia per Sd chronon muore*/
	Fb = atoi(argv[5]); /* n chron, Fb il pesce si riproduce*/
	NF = atoi(argv[6]); /* numero di pesci presenti all'inizio */
	NS = atoi(argv[7]); /* numero di squali presenti all'inizio */

	if (row <= 0 || cols <= 0 || Sd<= 0 || Sb<= 0 || Fb<= 0 || NF<= 0 || NS<= 0
			|| (NS+NF)>(row*cols)) {
		PRINT_ERR("Parametri immessi inconsistenti")
		exit(EXIT_FAILURE);
	}
	/*Controllo variabile opzionale */

	while ((c = getopt(argc, argv, "t:")) != -1)
		switch (c) {
		case 't':
			tvalue = optarg;
			break;
		case '?':
			if (optopt == 'c')
				fprintf(stderr, "Opzione -%c richiede un argomento.\n", optopt);
			else if (isprint (optopt))
				fprintf(stderr, "Opzione Sconosciuta `-%c'.\n", optopt);
			else
				fprintf(stderr,"Carattere sconosciuto `\\x%x'.\n", optopt);
			return EXIT_FAILURE;
		default:
			abort();
		}

	/* numero massimo di chronon eseguibili */
#ifdef DEBUG
	max_chron =100;
#else
	max_chron = INT_MAX;
#endif
printf("%s\n",tvalue);
	if (tvalue!=NULL) {
		if (!ceckInt(tvalue) || !(n_thread=atoi(tvalue))>0) {
			PRINT_ERR("Errore: Thread: paremetro inconsistente \n")
			return EXIT_FAILURE;
		}
		if (n_thread>cols) {
			PRINT_ERR("Errore: Thread : il numero di thread deve essere inferiorea quello delle colonne \n")
			return EXIT_FAILURE;
		}
	} else {
		/*Se il parametro non è specificato applico il default*/
		n_thread= STD_NTREAD ;

    }
n_thread=200;
    fprintf(stderr, "= rows:%d cols:%d Sb:%d Sd:%d Fb:%d NF:%d NS:%d N:%d=\n",row, cols,Sb,Sd,Fb,NF,NS,n_thread);
	fflush(stderr);

#ifdef DEBUG
	printf("n_thread : %d\n",n_thread);
	fflush(stdout);
#endif

	mat_bef = creaMatrice(row, cols);
	mat_aft = creaMatrice(row, cols);
	/* inizializzo rand() */
	srand(time(NULL));
	/*POPOLO LA MATRICE */
	popolaMat(NS, NF, row, cols, mat_bef);
	/*Stampo la matrice*/
	printmat(mat_bef);

	/*Inizializzo struct gestione thread */
	t_man= malloc(sizeof(thread_man_t));
	if ( !t_man) {
		PRINT_ERR("Errore Malloc")
		errno = ENOMEM;
		exit(EXIT_FAILURE);
	}
	

	t_man->tid_worker=malloc(sizeof(pthread_t)*n_thread);
	if ( !t_man) {
		PRINT_ERR("Errore Malloc")
		errno = ENOMEM;
		exit(EXIT_FAILURE);
	}
	tid = t_man->tid_worker; 
	i = 0;

	cols_rate= cols / n_thread;
	cols_r = cols % n_thread;

/*Ogni thread fa riferimento a 2 mutex (bef_mtx , aft_mtx uno a destra e uno a sinistra .
	 * nelle ultime 2 colonne sia a destra che a sinistra il thread deve loccare il 
	 * mutex in comune con il thread che gestisce le colonne 
	 * a destra o a sinistra assicurandosi così l'accesso unico alla risorsa. l'ultimo thread ha il mutex di destra coincidente con quello di 
	 * sinistra del primo thread */

	/*Creo i mutex per il lavoro sulle colonne di bordo*/
	arrmtx = (pthread_mutex_t*)malloc(n_thread*sizeof(pthread_mutex_t));
	arr_mtx_base = arrmtx;
	TESTVAR(arrmtx, NULL , ENOMEM , "Errore Malloc arrmtx. \n")
	/*Creo la struttura dati da passare ad ogni thread*/
	i=0;
	while (i < n_thread) {
		worker_data_t* data = malloc(sizeof(worker_data_t));
		TESTVAR(data, NULL , ENOMEM , "Errore Malloc data. \n")
		/*Imposto le colonne di cui si occuperà l'iesimo thread*/
		data->id_th = i;
		/*Prima colonna di cui si occupa il thread*/
		data->col_bgn = i*cols_rate;
		/*Ultima colonna di cui si occupa il thread piu se il thread è l'ultimo l
		 * eventuale resto se il numero non è multiplo*/
		data->col_end = (i == n_thread -1 ) ? ((i+1)*cols_rate) -1 + cols_r
				: ((i+1)*cols_rate) -1;
		/*Creo mutex per la mutua esclusione tra thread */
		/*Creo i mutex per il lavoro sulle colonne di confine*/
		if (i==0 && i!=n_thread -1) {
			pthread_mutex_init(arrmtx, NULL);
			data->bef_mtx = *arrmtx;
			arrmtx++;
			pthread_mutex_init(arrmtx, NULL);
			data->aft_mtx = *arrmtx;
		} else if ((i==0 && i==n_thread -1)) {
			pthread_mutex_init(arrmtx, NULL);
			data->bef_mtx = *arrmtx;
			data->aft_mtx = *arrmtx;
		} else if (i!=n_thread -1) {
			data->bef_mtx = *arrmtx;
			arrmtx++;
			pthread_mutex_init(arrmtx, NULL);
			data->aft_mtx = *arrmtx;
		} else {
			data->bef_mtx = *arrmtx;
			data->aft_mtx = *arr_mtx_base;
		}



        /*creo il thread worker e gli passo il puntatore alla stuct contenente i dati chegli servono*/
		if ( (pthread_create(tid, NULL, &worker, (void*) data)) != 0) { /* gest errore */
			PRINT_ERR("Errore creazione del thread")
			exit(EXIT_FAILURE);
		} else { /* thread creato */
#ifdef DEBUG_THREAD
			fprintf(stdout,"TREAD workerS OK. tid : %u \n", (unsigned int) *tid);
			fflush(stdout);
#endif
			tid++;
			i++;
		}
	}
	/* Creati i trhead worker creo il visualizer */
	if (pthread_create(&(t_man->tid_visualizer), NULL, &visualizer,
			((void*)mat_aft))) { /* gest errore */
		PRINT_ERR("Errore creazione del thread")
		exit(EXIT_FAILURE);
	} else { /* secondo thread creato */
#ifdef DEBUG_THREAD
		fprintf(stdout,"TREAD visualizer OK. tid : %u \n", (unsigned int)t_man->tid_visualizer);
		fflush(stdout);
#endif
	}
	tid = t_man->tid_worker;
	/*ASPETTO FINE DEL VISUALIZER*/
#ifdef DEBUG
	fprintf(stdout,"Main: aspetto visualizer %u \n",(unsigned int) t_man->tid_visualizer);
	fflush(stdout);
#endif

	pthread_join(t_man->tid_visualizer, &exstatus);

	/*pulizie heap e affini*/
	free(t_man->tid_worker);
	free(t_man);
	for (i=0; i < n_thread; i++) {
		pthread_mutex_destroy((arr_mtx_base+i));
	}
	free(arr_mtx_base);
	pthread_mutex_destroy(&NSmtx);
	pthread_mutex_destroy(&NFmtx);
	pthread_mutex_destroy(&waitermtx);
	pthread_mutex_destroy(&vismtx);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&viscond_v);
	pthread_cond_destroy(&viscond_w);
	for (i=0; i<row; i++) {
		free(mat_bef[i]);
	}
	free(mat_bef);
	for (i=0; i<row; i++) {
		free(mat_aft[i]);
	}
	free(mat_aft);

	printf("fine\n");
	exit(EXIT_SUCCESS);

}

void *worker(void* data) {
	worker_data_t* data_w = (worker_data_t*) data ;
	int ret, cucols, curow;
	while (chronon_count < max_chron) {
#ifdef DEBUG_THREAD
		fprintf(stdout,"Ciao sono il thread Worker %d (id :%u ) ,rangecol [%d , %d] \nAggiorno gli squali. \n\n", data_w->id_th,(unsigned int)pthread_self() , data_w->col_bgn , data_w->col_end);
		fflush(stdout);
#endif

		/*Inizio aggiornamento squali*/
		for (cucols = data_w->col_bgn; cucols <= data_w->col_end; cucols++) {
			/*TREADS esclusione!!!! se la colonna è la iniziale o quella successiva blocco il mutex*/
			if (cucols <= data_w->col_bgn + 1) {
				pthread_mutex_lock(&data_w->bef_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d locco bef_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			/*TREADS esclusione!!!! se la colonna è la finale o l'antecedente blocco il mutex*/
			if (cucols >= data_w->col_end - 1) {
				pthread_mutex_lock(&data_w->aft_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d locco aft_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			for (curow = 0; curow < row; curow++) {

				/*SE SQUALO prova a mangiare e a spostarsi*/
				if (mat_bef[curow][cucols].type==2) {
#ifdef DEBUG_WORKER
					pthread_mutex_lock(&NSmtx);
					pthread_mutex_lock(&NFmtx);
					fprintf(stdout,"Worker : %d Trovato squalo in ( %d , %d )\tN Squali: %d , N Pesci: %d\n", data_w->id_th , cucols ,curow,NS , NF);
					pthread_mutex_unlock(&NFmtx);
					pthread_mutex_unlock(&NSmtx);
					fflush(stdout);
#endif
					/* nella posizione corrente c'è uno squalo*/
					if (mangia(curow, cucols)) {
					} else if (sposta(curow, cucols)) {
					} else {
						stop(curow, cucols);
					}
				}
			}
			/*TREADS esclusione!!!!*/
			if (cucols == data_w->col_bgn || (cucols == data_w->col_bgn + 1)) {
				pthread_mutex_unlock(&data_w->bef_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d sblocco bef_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			if (cucols == data_w->col_end || (cucols == data_w->col_end -1)) {
				pthread_mutex_unlock(&data_w->aft_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d sblocco aft_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
		}
		/*Fine aggiornamento squali*/

		/*Aspetto che tutti i thread aggiornino gli squali*/
		pthread_mutex_lock(&waitermtx);
		waiter++;
		while (waiter < n_thread) {
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d mi blocco su waiter %d\n", data_w->id_th, waiter);
			fflush(stdout);
#endif
			pthread_cond_wait(&cond, &waitermtx);
		}
		/*Se sono l'ultimo thread ad aver aggiornato gli squali sveglio gli altri*/
		if (waiter==n_thread) {
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d sblocco gli altri thread per inizio aggiornamento pesci\n", data_w->id_th);
			fflush(stdout);
#endif
			pthread_mutex_lock(&vismtx);
			visCount=0;
			pthread_mutex_unlock(&vismtx);
			pthread_cond_broadcast(&cond);
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d risveglio thread per inizio aggiornamento pesci\n%d", data_w->id_th,waiter);
#endif			
			/*incremento waiter per non far generare segnali agli altri*/
			waiter++;
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d risveglio thread per inizio aggiornamento pesci\n%d", data_w->id_th,waiter);
#endif
		}
		pthread_mutex_unlock(&waitermtx);

		/*inizio aggiornamento pesci*/

#ifdef DEBUG_THREAD
		fprintf(stdout,"Th_n: %d (id :%u ) ,rangecol [%d , %d] \nAggiorno i pesci. \n\n", data_w->id_th,(unsigned int)pthread_self() , data_w->col_bgn , data_w->col_end);
		fflush(stdout);
#endif

		for (cucols = data_w->col_bgn; cucols <= data_w->col_end; cucols++) {
			/*TREADS esclusione!!!!*/
			if (cucols == data_w->col_bgn || (cucols == data_w->col_bgn + 1)) {
				pthread_mutex_lock(&(data_w->bef_mtx));
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d locco bef_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			if (cucols == data_w->col_end || (cucols == data_w->col_end -1)) {
				pthread_mutex_lock(&(data_w->aft_mtx));
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d locco aft_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			for (curow = 0; curow < row; curow++) {

				if (mat_bef[curow][cucols].type==1) {
#ifdef DEBUG_WORKER
					pthread_mutex_lock(&NSmtx);
					pthread_mutex_lock(&NFmtx);
					fprintf(stdout,"Worker : %d Trovato pesce in ( %d , %d )\tN Squali: %d , N Pesci: %d\n", data_w->id_th , cucols , curow, NS ,NF);
					pthread_mutex_unlock(&NFmtx);
					pthread_mutex_unlock(&NSmtx);
					fflush(stdout);
#endif
					if (sposta(curow, cucols)) {
					} else {
						stop(curow, cucols);
					}
				}

			}
			/*TREADS esclusione!!!!*/
			if (cucols == data_w->col_bgn || (cucols == data_w->col_bgn + 1)) {
				pthread_mutex_unlock(&data_w->bef_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d sblocco bef_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
			if (cucols == data_w->col_end || (cucols == data_w->col_end -1)) {
				pthread_mutex_unlock(&data_w->aft_mtx);
#ifdef DEBUG_THREAD_MTX
				fprintf(stdout,"Worker : %d sblocco aft_mtx ( colonna %d )\n", data_w->id_th , cucols);
				fflush(stdout);
#endif
			}
		}
		/* fine aggiornamento pesci */
#ifdef DEBUG_THREAD
		fprintf(stdout,"Worker : %d Ho finito \n", data_w->id_th);
		fflush(stdout);
#endif

		/*Sincronizzazione con visualizer*/

		pthread_mutex_lock(&vismtx);
		visCount++;
#ifdef DEBUG_THREAD_SYN
		fprintf(stdout,"Worker : %d mi blocco su viscount: %d \n", data_w->id_th, visCount);
		fflush(stdout);
#endif
		while (visCount < n_thread) {
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d mi blocco su viscount: %d \n", data_w->id_th, visCount);
			fflush(stdout);
#endif
			pthread_cond_wait(&viscond_w, &vismtx);
		}
		if (visCount==n_thread) {
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"Worker : %d sblocco il visualizer e attendo. Viscount:%d \n", data_w->id_th, visCount);
			fflush(stdout);
#endif
			/*Sveglio il visualizer che è in attesa*/
			pthread_cond_signal(&viscond_v);
			visCount++;
			/*Aspetta su waiter_w anche lui dopo aver mandato il segnale*/
			pthread_cond_wait(&viscond_w, &vismtx);
		}
		pthread_mutex_unlock(&vismtx);

#ifdef DEBUG_THREAD_SYN
		fprintf(stdout,"Worker : %d mi sblocco su viscount QUI\n", data_w->id_th);
		fflush(stdout);
#endif			

	}
	/*Prima della conclusione libero la memoria dalle strutture dati*/
	pthread_mutex_destroy(&(data_w->aft_mtx));
	pthread_mutex_destroy(&(data_w->bef_mtx));
	free(data_w);
	ret = TRUE;
	pthread_exit(&ret);
}

static int mangia(int curow, int cucols) {
	near_pos_t start;
	int k;
	/*Segno variabili con indici incrementati per non richiamare sempre le funzioni di incremento*/
	int rowde = rowdecr(curow);
	int rowin = rowincr(curow);
	int colsde = colsdecr(cucols);
	int colsin = colsincr(cucols);
	/*Se non squalo ritorna errore*/
	if (mat_bef[curow][cucols].type != 2) {
		PRINT_ERR("Mangia : elemento non squalo!\n")
#ifdef DEBUG_ACTION
		fprintf(stderr," Errore : (%d , %d) Tipe : %d chron : %d \n", cucols, curow,mat_bef[curow][cucols].type, chronon_count);
		fflush(stderr);
#endif
		return FALSE;
	}
	start = (int) (4 * (rand() / (RAND_MAX + 1.0)));
#ifdef DEBUG_MANGIA_RAND
	fprintf(stdout, "mangia rand() %d\n",(start)%4);
	fflush(stdout);
#endif
	k=0;
	while (k<4) {

		switch ((start+k)%4) {
		case UP: /*UP*/
			if (mat_bef[rowde][cucols].type == 1 && mat_aft[rowde][cucols].type
					!= 2) {
				/*se pesce ci copio sopra lo squalo*/
				mat_aft[rowde][cucols].type = mat_bef[curow][cucols].type;
				mat_aft[rowde][cucols].repTime = mat_bef[curow][cucols].repTime
						+ 1;
				mat_aft[rowde][cucols].lastEat = 0;
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) Mangia UP \n", cucols , curow);
				fflush(stdout);
#endif
				/*Elimino dalla matrice di prima il pesce mangiato */
				clearAbiCell(mat_bef, rowde, cucols);
				/*Elimino dalla matrice di prima lo squalo già esaminato */
				clearAbiCell(mat_bef, curow, cucols);
				/*Se possibile faccio riprodurre lo squalo*/
				riproduci(rowde, cucols);
				/*Riduco numero pesci*/
				pthread_mutex_lock(&NFmtx);
				NF--;
				pthread_mutex_unlock(&NFmtx);
				return TRUE;
			}
#ifdef DEBUG_MANGIA
			fprintf(stdout, "UP no, bef_type : %d , aft_type : %d \n",mat_bef[rowde][cucols].type,mat_aft[rowde][cucols].type);
			fflush(stdout);
#endif
			break;
		case RIGHT: /*RIGHT*/
			if (mat_bef[curow][colsin].type == 1 && mat_aft[curow][colsin].type
					!= 2) {
				mat_aft[curow][colsin].type = mat_bef[curow][cucols].type;
				mat_aft[curow][colsin].repTime = mat_bef[curow][cucols].repTime
						+ 1;
				mat_aft[curow][colsin].lastEat = 0;
				/*Elimino dalla matrice di prima il pesce mangiato */
				clearAbiCell(mat_bef, curow, colsin);
				/*Elimino dalla matrice di prima lo squalo già esaminato */
				clearAbiCell(mat_bef, curow, cucols);
				/*Se possibile faccio riprodurre lo squalo*/
				riproduci(curow, colsin);
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) Mangia RIGHT \n", cucols , curow);
				fflush(stdout);
#endif
				/*Riduco numero pesci*/
				pthread_mutex_lock(&NFmtx);
				NF--;
				pthread_mutex_unlock(&NFmtx);
				return TRUE;
			}
#ifdef DEBUG_MANGIA
			fprintf(stdout, "RIGHT no, bef_type : %d , aft_type : %d \n",mat_bef[rowde][cucols].type,mat_aft[rowde][cucols].type);
			fflush(stdout);
#endif
			break;
		case DOWN: /*DOWN*/
			if (mat_bef[rowin][cucols].type == 1 && mat_aft[rowin][cucols].type
					!= 2) {
				mat_aft[rowin][cucols].type = mat_bef[curow][cucols].type;
				mat_aft[rowin][cucols].repTime = mat_bef[curow][cucols].repTime
						+ 1;
				mat_aft[rowin][cucols].lastEat = 0;
				/*Elimino dalla matrice di prima il pesce mangiato */
				clearAbiCell(mat_bef, rowin, cucols);
				/*Elimino dalla matrice di prima lo squalo già esaminato */
				clearAbiCell(mat_bef, curow, cucols);
				/*Se possibile faccio riprodurre lo squalo*/
				riproduci(rowin, cucols);
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) Mangia DOWN \n", cucols , curow);
				fflush(stdout);
#endif
				/*Riduco numero pesci*/
				pthread_mutex_lock(&NFmtx);
				NF--;
				pthread_mutex_unlock(&NFmtx);
				return TRUE;
			}
#ifdef DEBUG_MANGIA
			fprintf(stdout, "DOWN no, bef_type : %d , aft_type : %d \n",mat_bef[rowde][cucols].type,mat_aft[rowde][cucols].type);
			fflush(stdout);
#endif
			break;
		case LEFT: /*LEFT*/
			if (mat_bef[curow][colsde].type == 1 && mat_aft[curow][colsde].type
					!= 2) {
				mat_aft[curow][colsde].type = mat_bef[curow][cucols].type;
				mat_aft[curow][colsde].repTime = mat_bef[curow][cucols].repTime
						+ 1;
				mat_aft[curow][colsde].lastEat = 0;
				/*Elimino dalla matrice di prima il pesce mangiato */
				clearAbiCell(mat_bef, curow, colsde);
				/*Elimino dalla matrice di prima lo squalo già esaminato */
				clearAbiCell(mat_bef, curow, cucols);
				/*Se possibile faccio riprodurre lo squalo*/
				riproduci(curow, colsde);
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) Mangia LEFT \n", cucols , curow);
				fflush(stdout);
#endif
				/*Riduco numero pesci*/
				pthread_mutex_lock(&NFmtx);
				NF--;
				pthread_mutex_unlock(&NFmtx);
				return TRUE;
			}
#ifdef DEBUG_MANGIA
			fprintf(stdout, "LEFT no, bef_type : %d , aft_type : %d \n",mat_bef[rowde][cucols].type,mat_aft[rowde][cucols].type);
			fflush(stdout);
#endif
			break;
		}
#ifdef DEBUG_MANGIA_RAND
		printf("incr k\n");
#endif
		k++;
	}
	return FALSE;
}

static int riproduci(int curow, int cucols) {
	/* devo far riprodurre l'elemento in mat_aft[curow][cucols] quindi:*/
	near_pos_t start;
	int k;
	/*Segno variabili con indici incrementati per non richiamare sempre le funzioni di incremento*/
	int rowde = rowdecr(curow);
	int rowin = rowincr(curow);
	int colsde = colsdecr(cucols);
	int colsin = colsincr(cucols);
	/*controllo se a riprodursi è un pesce o uno squalo*/
	if (mat_aft[curow][cucols].type != 1 && mat_aft[curow][cucols].type != 2) {
#ifdef DEBUG_ACTION
		fprintf(stderr,"Errore ( %d , %d ) type == 0 ! ripoduci fallita!",cucols, curow);
		fflush(stderr);
#endif
		return FALSE;
	}
	/*Controllo se l'animale in questione si può riprodurre*/
	if (mat_aft[curow][cucols].type == 2 && mat_aft[curow][cucols].repTime < Sb) {
		return FALSE;
	}
	if (mat_aft[curow][cucols].type == 1 && mat_aft[curow][cucols].repTime < Fb) {
		return FALSE;
	}
#ifdef DEBUG_RIPRODUCI
	fprintf(stdout,"( %d , %d ) entra in si riproduce Type: %d, RepT : %d < F:%d S:%d\n", cucols , curow , mat_aft[curow][cucols].type, mat_aft[curow][cucols].repTime, Fb , Sb);
	fflush(stdout);
#endif
	/*inizio la scansione delle celle adiacenti*/
	start = (int) (4 * (rand() / (RAND_MAX + 1.0)));
	k=0;
	while (k<4) {
		switch ((start+k)%4) {
		case UP: /*UP*/
			if (mat_bef[rowde][cucols].type == 0 && mat_aft[rowde][cucols].type
					== 0) {
#ifdef DEBUG_RIPRODUCI
				fprintf(stdout,"( %d , %d ) di tipo %d prima di Riprodursi \n", cucols , rowde ,mat_aft[rowde][cucols].type);
				fflush(stdout);
#endif	
				/*Se si riproduce scrivo nuovo esemplare dello stesso tipo*/
				mat_aft[rowde][cucols].type = mat_aft[curow][cucols].type;
				mat_aft[rowde][cucols].repTime = 0;
				mat_aft[rowde][cucols].lastEat = 0;
				/*Azzero il contatore del riproduttore*/
				mat_aft[curow][cucols].repTime = 0;
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) di tipo %d Si Riproduce UP \n", cucols , curow ,mat_aft[curow][cucols].type);
				fflush(stdout);
#endif			
				if (mat_aft[curow][cucols].type==2) {
					pthread_mutex_lock(&NSmtx);
					NS++;
					pthread_mutex_unlock(&NSmtx);
				} else if (mat_aft[curow][cucols].type==1) {
					pthread_mutex_lock(&NFmtx);
					NF++;
					pthread_mutex_unlock(&NFmtx);
				}
				return TRUE;
			}
#ifdef DEBUG_RIPRODUCI
			fprintf(stdout,"( %d , %d ) di tipo %d non Si Riproduce UP { T_bef: %d , T_aft: %d } \n", cucols , curow ,mat_aft[curow][cucols].type,mat_bef[rowde][cucols].type,mat_aft[rowde][cucols].type);
			fflush(stdout);
#endif	
			break;
		case RIGHT: /*RIGHT*/
			if (mat_bef[curow][colsin].type == 0 && mat_aft[curow][colsin].type
					== 0) {
#ifdef DEBUG_RIPRODUCI
				fprintf(stdout,"( %d , %d ) di tipo %d prima di Riprodursi \n", colsin , curow ,mat_aft[curow][colsin].type);
				fflush(stdout);
#endif	
				mat_aft[curow][colsin].type = mat_aft[curow][cucols].type;
				mat_aft[curow][colsin].repTime = 0;
				mat_aft[curow][colsin].lastEat = 0;
				/*Azzero il contatore del riproduttore*/
				mat_aft[curow][cucols].repTime = 0;
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) di tipo %d Si Riproduce RIGHT \n", cucols , curow, mat_aft[curow][cucols].type);
				fflush(stdout);
#endif
				if (mat_aft[curow][cucols].type==2) {
					pthread_mutex_lock(&NSmtx);
					NS++;
					pthread_mutex_unlock(&NSmtx);
				} else if (mat_aft[curow][cucols].type==1) {
					pthread_mutex_lock(&NFmtx);
					NF++;
					pthread_mutex_unlock(&NFmtx);
				}
				return TRUE;
			}
#ifdef DEBUG_RIPRODUCI
			fprintf(stdout,"( %d , %d ) di tipo %d non Si Riproduce RIGHT { T_bef: %d , T: %d } \n", cucols , curow ,mat_aft[curow][cucols].type,mat_bef[curow][colsin].type,mat_aft[curow][colsin].type);
			fflush(stdout);
#endif	
			break;
		case DOWN: /*DOWN*/
			if (mat_bef[rowin][cucols].type == 0 && mat_aft[rowin][cucols].type
					== 0) {
#ifdef DEBUG_RIPRODUCI
				fprintf(stdout,"( %d , %d ) di tipo %d prima di Riprodursi \n", cucols , row ,mat_aft[row][cucols].type);
				fflush(stdout);
#endif
				mat_aft[rowin][cucols].type = mat_aft[curow][cucols].type;
				mat_aft[rowin][cucols].repTime = 0;
				mat_aft[rowin][cucols].lastEat = 0;
				/*Azzero il contatore del riproduttore*/
				mat_aft[curow][cucols].repTime = 0;
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) di tipo %d Si Riproduce DOWN \n", cucols , curow,mat_aft[curow][cucols].type);
				fflush(stdout);
#endif
				if (mat_aft[curow][cucols].type==2) {
					pthread_mutex_lock(&NSmtx);
					NS++;
					pthread_mutex_unlock(&NSmtx);
				} else if (mat_aft[curow][cucols].type==1) {
					pthread_mutex_lock(&NFmtx);
					NF++;
					pthread_mutex_unlock(&NFmtx);
				}
				return TRUE;
			}
#ifdef DEBUG_RIPRODUCI
			fprintf(stdout,"( %d , %d ) di tipo %d non Si Riproduce DOWN { T_bef: %d , T: %d } \n", cucols , curow , mat_aft[curow][cucols].type, mat_bef[rowin][cucols].type, mat_aft[rowin][cucols].type);
			fflush(stdout);
#endif	
			break;
		case LEFT: /*LEFT*/
			if (mat_bef[curow][colsde].type == 0 && mat_aft[curow][colsde].type
					== 0) {
#ifdef DEBUG_RIPRODUCI
				fprintf(stdout,"( %d , %d ) di tipo %d prima di Riprodursi \n", colsde , curow ,mat_aft[curow][colsde].type);
				fflush(stdout);
#endif	
				mat_aft[curow][colsde].type = mat_aft[curow][cucols].type;
				mat_aft[curow][colsde].repTime = 0;
				mat_aft[curow][colsde].lastEat = 0;
				/*Azzero il contatore del riproduttore*/
				mat_aft[curow][cucols].repTime = 0;
#ifdef DEBUG_ACTION
				fprintf(stdout,"( %d , %d ) di tipo %d Si Riproduce LEFT \n", cucols , curow,mat_aft[curow][cucols].type);
				fflush(stdout);
#endif
				if (mat_aft[curow][cucols].type==2) {
					pthread_mutex_lock(&NSmtx);
					NS++;
					pthread_mutex_unlock(&NSmtx);
				} else if (mat_aft[curow][cucols].type==1) {
					pthread_mutex_lock(&NFmtx);
					NF++;
					pthread_mutex_unlock(&NFmtx);
				}
				return TRUE;
			}
#ifdef DEBUG_RIPRODUCI
			fprintf(stdout,"( %d , %d ) di tipo %d non Si Riproduce LEFT { T_bef: %d , T: %d } \n", cucols , curow ,mat_aft[curow][cucols].type,mat_bef[curow][colsde].type,mat_aft[curow][colsde].type);
			fflush(stdout);
#endif	
			break;
		}
		k++;
	}
	return FALSE;
}

static void stop(int curow, int cucols) {
	if (mat_bef[curow][cucols].type!= 1 && mat_bef[curow][cucols].type!= 2) {
		fprintf(stderr,"Errore : Sposta -> ( %d , %d ) Type = %d chronon : %d", cucols, curow,mat_bef[curow][cucols].type,chronon_count);
		fflush(stderr);
		return;
	}
	if (!muore(curow, cucols)) {
		mat_aft[curow][cucols].type = mat_bef[curow][cucols].type;
		/*faccio risultare il soggetto più vecchio di un chronon*/
		mat_aft[curow][cucols].repTime = mat_bef[curow][cucols].repTime +1;
		mat_aft[curow][cucols].lastEat = mat_bef[curow][cucols].lastEat +1;
		riproduci(curow, cucols);
		clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
		fprintf(stdout,"STOP -> ( %d , %d ) \n", cucols , curow);
		fflush(stdout);
#endif
	} else {
		/*Se muore è stato cancellato dalla griglia senza copiarlo*/
	}

}

static int sposta(int curow, int cucols) {
	int k;
	near_pos_t start;
	int rowde = rowdecr(curow);
	int rowin = rowincr(curow);
	int colsde = colsdecr(cucols);
	int colsin = colsincr(cucols);

	if (mat_bef[curow][cucols].type!= 1 && mat_bef[curow][cucols].type!= 2) {
		fprintf(stderr,"Errore : Sposta -> ( %d , %d ) Type = %d chronon : %d", cucols, curow,mat_bef[curow][cucols].type,chronon_count);
		fflush(stderr);
		return FALSE;
	}
	/*ritorna true se lo spostamento è avvenuto false altrimenti*/
	/*calcolo random la partenza e poi faccio una scansione delle possibilità*/
	start = (int) (4 * (rand() / (RAND_MAX + 1.0)));
	k=0;
	while (k<4) {
		switch ((start+k)%4) {
		case UP: /*UP*/

			if (mat_bef[rowde][cucols].type == 0 && mat_aft[rowde][cucols].type
					== 0) {
				if (!muore(curow, cucols)) {
					mat_aft[rowde][cucols].type = mat_bef[curow][cucols].type;
					mat_aft[rowde][cucols].repTime
							= mat_bef[curow][cucols].repTime + 1;
					mat_aft[rowde][cucols].lastEat
							= mat_bef[curow][cucols].lastEat+ 1;
					/*Cancello lo spostato :)*/
					clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
					fprintf(stdout,"( %d , %d ) Si sposta Type: %d in ( %d , %d ) \n" , cucols , curow, mat_bef[curow][cucols].type, cucols ,rowde);
					fflush(stdout);
#endif
					/*Se possibile si riproduce*/
#ifdef DEBUG_ACTION
					fprintf(stdout,"( %d , %d ) Si sposta %d UP in (%d , %d ) \n" , cucols , curow,mat_bef[curow][cucols].type, cucols , rowde);
					fflush(stdout);
#endif
					if (riproduci(rowde, cucols)) {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d ) Si riproduce -sposta- Type: %d  \n" , cucols , rowde,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					} else {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d )NON Si riproduce -sposta- %d \n" , cucols , rowde,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					}
					/**/
				}
				return TRUE;
			}
			break;
		case RIGHT: /*RIGHT*/
			if (mat_bef[curow][colsin].type == 0 && mat_aft[curow][colsin].type
					== 0) {
				if (!muore(curow, cucols)) {
					mat_aft[curow][colsin].type = mat_bef[curow][cucols].type;
					mat_aft[curow][colsin].repTime
							= mat_bef[curow][cucols].repTime + 1;
					mat_aft[curow][colsin].lastEat
							= mat_bef[curow][cucols].lastEat + 1;
					/*Cancello lo spostato :)*/
					clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
					fprintf(stdout,"( %d , %d ) Si sposta Type: %d in ( %d , %d ) \n" , cucols , curow, mat_bef[curow][cucols].type, colsin ,curow);
					fflush(stdout);
#endif
					/*Se possibile si riproduce*/
					if (riproduci(curow, colsin)) {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d ) Si riproduce -sposta- Type: %d  \n" , colsin ,curow,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					} else {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d )NON Si riproduce -sposta- %d \n" , colsin , curow,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					}
					/**/
				}
				return TRUE;
			}
			break;
		case DOWN: /*DOWN*/
			/*Se nella matrice di arrivo e di partenza la casella di arrivo è vuota entro*/
			if (mat_bef[rowin][cucols].type == 0 && mat_aft[rowin][cucols].type
					== 0) {
				/*se l'oggetto dello spostamento è pesce o se è uno squalo in vita*/
				if (!muore(curow, cucols)) {
					mat_aft[rowin][cucols].type = mat_bef[curow][cucols].type;
					mat_aft[rowin][cucols].repTime
							= mat_bef[curow][cucols].repTime + 1;
					mat_aft[rowin][cucols].lastEat
							= mat_bef[curow][cucols].lastEat + 1;
					/*Cancello lo spostato :)*/
					clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
					fprintf(stdout,"( %d , %d ) Si sposta Type: %d in ( %d , %d ) \n" , cucols , curow, mat_bef[curow][cucols].type, cucols ,rowin);
					fflush(stdout);
#endif
					/*Se possibile si riproduce*/
					if (riproduci(rowin, cucols)) {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d ) Si riproduce -sposta- Type: %d  \n" , cucols , rowin,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					} else {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d )NON Si riproduce -sposta- %d \n" , cucols , rowin,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					}
					/**/
				}
				return TRUE;
			}
		case LEFT: /*LEFT*/
			if (mat_bef[curow][colsde].type == 0 && mat_aft[curow][colsde].type
					== 0) {
				if (!muore(curow, cucols)) {
					mat_aft[curow][colsde].type = mat_bef[curow][cucols].type;
					mat_aft[curow][colsde].repTime
							= mat_bef[curow][cucols].repTime + 1;
					mat_aft[curow][colsde].lastEat
							= mat_bef[curow][cucols].lastEat + 1;
					/*Cancello lo spostato :)*/
					clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
					fprintf(stdout,"( %d , %d ) Si sposta Type: %d in ( %d , %d ) \n" , cucols , curow, mat_bef[curow][cucols].type, colsde ,curow);
					fflush(stdout);
#endif
					/*Se possibile si riproduce*/
					if (riproduci(curow, colsde)) {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d ) Si riproduce -sposta- Type: %d  \n" , colsde , curow ,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					} else {
#ifdef DEBUG_SPOSTA
						fprintf(stdout,"( %d , %d ) NON Si riproduce -sposta- %d \n" ,colsde , curow,mat_bef[curow][cucols].type);
						fflush(stdout);
#endif						
					}
					/**/
				}
				return TRUE;
			}
			break;
		}
		k++;
	}
	return FALSE;
}

void *visualizer() {
	abi_t** a;
	int i, j;
	while (chronon_count < max_chron) {

#ifdef DEBUG_THREAD_SYN
		printf("Start Thread visualizer\n");
#endif
		/*Sincronizzazione con Worker*/
		pthread_mutex_lock(&vismtx);
		while (visCount!=n_thread+1) {
#ifdef DEBUG_THREAD_SYN
			fprintf(stdout,"viscount :%d \n", visCount);
			fflush(stdout);
#endif
			pthread_cond_wait(&viscond_v, &vismtx);
		}
		/*Incremento la variabile in modo da ribloccare il visualizzatore alla prossima esecuzione*/
		visCount++;
		pthread_mutex_unlock(&vismtx);

		pthread_mutex_lock(&waitermtx);
		waiter=0;
		pthread_mutex_unlock(&waitermtx);

#ifdef DEBUG_THREAD_SYN
		printf("Visualizer : Aspettati =)\n");
#endif

		/*stampa la matrice a a schermo*/
#ifdef SLEEP
		sleep(2);
#endif
#ifdef CLEAR
		system("clear");
#endif
		for (i = 0; i < cols; i++)
			printf("====");
		printf("\nWATOR\n");
		for (i = 0; i < cols; i++)
			printf("----");
		printf("\n");
		pthread_mutex_lock(&NSmtx);
		pthread_mutex_lock(&NFmtx);
		printf("Chron : %d --> N Squali: %d , N Pesci: %d \n", chronon_count,
				NS, NF);
		pthread_mutex_unlock(&NFmtx);
		pthread_mutex_unlock(&NSmtx);
		for (i = 0; i < cols; i++) {
			printf("====");
		}
		printf("\n");
		for (i = 0; i < cols; i++) {
			printf(" ___");
		}
		printf("\n");;
		for (i = 0; i < row; i++) {
			printf("|");
			for (j = 0; j < cols; j++) {
				if (mat_aft[i][j].type==0) {
					printf("___|");
				} else {
					printf("_%c_|", (mat_aft[i][j].type==1) ? 'F' : 'S');
				}
			}
			printf("\n");
		}
		printf("\n");
		/*Sincronizzazione con Worker*/
		/*metto mat_aft in mat_bef, cancello la mat_bef e creo un nuovo mat_aft per reiniziare l'aggiornamento,
		 * sblocco poi gli worker*/
		chronon_count++;
		a = mat_bef;
		mat_bef = mat_aft;
		for (i=0; i<row; i++) {
			free(a[i]);
		}
		free(a);
		mat_aft= creaMatrice(row, cols);

		pthread_mutex_lock(&vismtx);
		/*Sblocco tutti i thread worker in attesa su viscount_w*/
		pthread_cond_broadcast(&viscond_w);
		pthread_mutex_unlock(&vismtx);
	}
	return NULL;
}

static int muore(int curow, int cucols) {
	/*Cancella da mat_bef un elemento in curow cucols, decrementando i contatori*/
	if (mat_bef[curow][cucols].type != 1 && mat_bef[curow][cucols].type != 2) {
		fprintf(stderr,"Errore: ( %d , %d ) di tipo %d in Muore \n" , cucols , curow, mat_bef[curow][cucols].type);
		fflush(stderr);
		return FALSE;
	}
	if (mat_bef[curow][cucols].type == 1 || (mat_bef[curow][cucols].type == 2
			&& mat_bef[curow][cucols].lastEat < Sd)) {
		return FALSE;
	}
	clearAbiCell(mat_bef, curow, cucols);
#ifdef DEBUG_ACTION
	fprintf(stdout,"MUORE -> ( %d , %d ) \n" , cucols , curow);
	fflush(stdout);
#endif
	pthread_mutex_lock(&NSmtx);
	NS--;
	pthread_mutex_unlock(&NSmtx);
	return TRUE;
}

static void printmat(abi_t** a) {
	int i, j;
#ifdef DEBUG
	printf("STAMPO MATRICE MONOTHREAD:\n");
#endif
#ifdef CLEAR
	system("clear");
#endif
	for (i = 0; i < cols; i++)
		printf("====");
	printf("\nWATOR\n");
	for (i = 0; i < cols; i++)
		printf("----");
	printf("\n");
	printf("Chron : %d --> N Squali: %d , N Pesci: %d \n", chronon_count, NS,
			NF);
	for (i = 0; i < cols; i++) {
		printf("====");
	}
	printf("\n");
	for (i = 0; i < cols; i++) {
		printf(" ___");
	}
	printf("\n");
	for (i = 0; i < row; i++) {
		printf("|");
		for (j = 0; j < cols; j++) {
			if (a[i][j].type==0) {
				printf("___|");
			} else {
				printf("_%c_|", (a[i][j].type==1) ? 'F' : 'S');
			}
		}
		printf("\n");
	}
	printf("\n");

}

static void popolaMat(int Ns, int Nf, int row, int cols, abi_t** mare) {
	int i, j;
	while (Ns > 0 || Nf > 0) {
		/* IMMETTO SQUALI E PESCI NELLA MATRICE IN POSIZIONI CASUALI*/
		i = (int) (row * (rand() / (RAND_MAX + 1.0)));
		j = (int) (cols * (rand() / (RAND_MAX + 1.0)));
		if ((mare[i][j].type == 0) && Ns > 0) {
			mare[i][j].type = 2;
			mare[i][j].lastEat = 0;
			mare[i][j].repTime = 0;
			Ns--;
#ifdef DEBUG_POPOLAMAT
			printf("metto lo squalo in (X:%d , Y:%d). Nf : %d, Ns : %d \n", j, i,
					Nf, Ns);
			fflush(stdout);
#endif
		}
		i = (int) (row * (rand() / (RAND_MAX + 1.0)));
		j = (int) (cols * (rand() / (RAND_MAX + 1.0)));
		if ((mare[i][j].type == 0) && Nf > 0) {
			mare[i][j].type = 1;
			mare[i][j].lastEat = 0;
			mare[i][j].repTime = 0;
			Nf--;
#ifdef DEBUG_POPOLAMAT
			printf("metto il pesce in (X:%d , Y:%d). Nf : %d, Ns : %d \n", j, i,
					Nf, Ns);
			fflush(stdout);
#endif
		}
	}
}

static abi_t** creaMatrice(int row, int cols) {
	abi_t** mat = (abi_t**)calloc(row, sizeof(abi_t*));
	int i;
	if ( !mat) {
		errno = ENOMEM;
		return NULL;
	}
	for (i=0; i<row; i++) {
		mat[i] = (abi_t*)calloc(cols, sizeof(abi_t));
		if ( !mat[i]) {
			errno = ENOMEM;
			return NULL;
		}
	}
	return mat;
}

int ceckInt(char *str) {
	int i = 0;
	while (str[i]!='\0') {
		if (!isdigit(str[i])) {
			return FALSE;
		}
		i++;
	}
	if (i == 0)
		return FALSE;
	return TRUE;
}

static int rowincr(int curow) {
	if (++curow==row) {
		return 0;
	}
	return curow;
}

static int rowdecr(int curow) {
	if (0==curow) {
		return row-1;
	}
	return --curow;
}

static int colsincr(int cucols) {
	if (++cucols==cols) {
		return 0;
	}
	return cucols;
}

static int colsdecr(int cucols) {
	if (0==cucols) {
		return cols-1;
	}
	return --cucols;
}

static void clearAbiCell(abi_t** mat, int row, int cols) {
	mat[row][cols].type = 0;
	mat[row][cols].lastEat = 0;
	mat[row][cols].repTime = 0;
}
