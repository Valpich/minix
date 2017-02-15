/** 
 * \file
 * \brief Simulatore di un semplice modello di andamento della popolazione acquatica,
 *        usando come unità di elaborazione del mondo il thread.
 *
 * \autor Casenove Matteo 408853
 *
 */

/**
 * SCELTE IMPLEMENTATIVE
 *
 * Il mondo wator viene rappresentato con una matrice di strutture.
 * Una struttura rappresenta lo stato della casella(Acqua o Animale). L'animale è
 * una struttura che rappresenta o lo squalo o il pesce con le loro caratteristiche.
 * Ogni animale ha un indicatore del chronon presente che permette di controllare se
 * ha già effettuato una mossa, evitando così che faccia più movimenti in uno stesso
 * chronon.
 *
 * SCELTE DI SINCRONIZZAZIONE
 *
 * La sincronizzazione fra i vari thread( i worker e il visualizzatore) viene fatta 
 * con un mutex che blocca tutta la matrice e con 3 condition. Quest'ultime sono, una
 * per i worker che lavorano sugli squali, una per i worker che lavorano sui pesci 
 * e l'ultima per il visualizzatore.
 * I worker degli squali attendono che tutti finiscano di lavorare finchè l'ultimo 
 * ne sveglia uno che a catena sveglia gli altri.
 * I thread che lavorano sui pesci si comportano allo stesso modo, solo che l'ultimo 
 * che termina di lavorare,invece di svegliare un thread worker va a svegliare il 
 * visualizzatore che solo alla fine del suo lavoro manda un segnale riattivando i thread
 * in attesa.
 *
 * */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "watorst.h"
//#include <getopt.h>
#include "pthread.h"


int rows,cols,Sb,Sd,Fb,NF,NS;
int N=4; /* Numero di thread paralleli. 4 default */
int* display; /* visualizzatore*/
world matrix; /* matrice di gestione */
int kron=0; /* unita' di tempo */

int thfrees,thfreef; /* numero di thread non bloccati */

int npesci=0,nsquali=0; /* Numero di squali e di pesci presenti nella matrice */

/* Mutex su tutta la matrice */
pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
/* Condition sui thread che stanno lavorando */
pthread_cond_t workerf=PTHREAD_COND_INITIALIZER;
/* Condition sui thread che stanno lavorando */
pthread_cond_t workers=PTHREAD_COND_INITIALIZER;
/* Condition sul thread visualizzatore? */
pthread_cond_t screen=PTHREAD_COND_INITIALIZER;



/** 
 *Inizializza gli squali nella matrice.
 * */
static void initShark(){
	int nsh; /* numero progressivo di squali aggiunti */
	int iC,iR;
	nsh=0;
	nsquali=NS;
	while(nsh<NS){
		/* Posiziono lo squalo in una cella casuale */
		iC=MOD(((int)rand()),cols);
		iR=MOD(((int)rand()),rows);
		if(matrix[iR][iC].type==WATER){
			matrix[iR][iC].type=SHARK;
			matrix[iR][iC].animale.shark.sb=Sb;
			matrix[iR][iC].animale.shark.sd=Sd;
			matrix[iR][iC].klife=kron;
			nsh++;
		}
	}
}

/** 
 *Inizializza i pesci nella matrice.
 * */
static void initFish(){
	int np; /* numero progressivo di pesci aggiunti */
	int iC,iR; /* indice riga e colonna */
	np=0;
	npesci=NF;
	while(np<NF){
		/* Posiziono il pesce in una cella casuale */
		iC=MOD(((int)rand()),cols);
		iR=MOD(((int)rand()),rows);
		if(matrix[iR][iC].type==WATER){
			matrix[iR][iC].type=FISH;
			matrix[iR][iC].animale.fb=Fb;
			matrix[iR][iC].klife=kron;
			np++;
		}
	}

}

/* 
 * Controlla se nelle celle adiacenti ce n'è una libera.
 *
 * \param r indice della riga.
 * \param c indice della colonna.
 *
 * \returns TRUE se c'è una cella libera altrimenti FALSE.
 *  
 * */
static int thereis_water(int r, int c){
	if(matrix[MOD(r-1,rows)][c].type==WATER || matrix[MOD(r+1,rows)][c].type==WATER \
			|| matrix[r][MOD(c-1,cols)].type==WATER || \
			matrix[r][MOD(c+1,cols)].type==WATER )
		return TRUE;
	else
		return FALSE;
}


/* 
 * Funzione che muove l'animale nel mondo.
 *
 * \param r indice della riga.
 * \param c indice della colonna.
 *
 * \returns TRUE se l'animale si è mosso altrimenti FALSE.
 *  
 * */
static int move(int r,int c){
	printf("OOOOOOOOOOOOOOOOOOOO\n");
        int mossa=TRUE;
	int rnd;
	/* Controllo che intorno c'è un posto libero dove spostarsi */
	printf("ciullo sia conte\n");  
        if(!thereis_water(r,c))
		return FALSE;
	printf("quante risate\n");
    while(mossa){
        rnd=MOD((int)rand(),4);
		switch(rnd){
			case 0:
				if(matrix[MOD(r-1,rows)][c].type==WATER){
					MOVE(MOD(r-1,rows),c,r,c)
					mossa=FALSE;
				}
				break;
			case 1:
				if(matrix[MOD(r+1,rows)][c].type==WATER){
					MOVE(MOD(r+1,rows),c,r,c)
					mossa=FALSE;
				}
				break;
			case 2:
				if(matrix[r][MOD(c-1,cols)].type==WATER){
					MOVE(r,MOD(c-1,cols),r,c)
					mossa=FALSE;
				}
				break;
			case 3:
				if(matrix[r][MOD(c+1,cols)].type==WATER){
				       	MOVE(r,MOD(c+1,cols),r,c)
					mossa=FALSE;
				}
				break;
		}

	}
    printf("io passo anche qui:%i",mossa);
    return TRUE;
}

/*
 * Funzione che fa riprodurre l'animale.
 *
 * \param r indice della riga.
 * \param c indice della colonna.
 *
 * \returns TRUE se l'animale si è riprodotto altrimenti FALSE.
 *  
 * */
static int newborn(int r, int c){
	int mossa=TRUE;
	int rnd,tipo;
	tipo=matrix[r][c].type;
	/* Controlla se c'è un posto libero dove spostarsi */
	if(!thereis_water(r,c)){
		return FALSE;
	}
	while(mossa){
		rnd=MOD((int)rand(),4);
		switch(rnd){
			case 0:
				if(matrix[MOD(r-1,rows)][c].type==WATER){
					BORN(MOD(r-1,rows),c,tipo)
					mossa=FALSE;
				}
				break;
			case 1:
				if(matrix[MOD(r+1,rows)][c].type==WATER){
					BORN(MOD(r+1,rows),c,tipo)
					mossa=FALSE;
				}
				break;
			case 2:
				if(matrix[r][MOD(c-1,cols)].type==WATER){
					BORN(r,MOD(c-1,cols),tipo)
					mossa=FALSE;
				}
				break;
			case 3:
				if(matrix[r][MOD(c+1,cols)].type==WATER){
				       	BORN(r,MOD(c+1,cols),tipo)
					mossa=FALSE;
				}
				break;
		}

	}
	if(tipo==1)
		npesci++;
	else
		nsquali++;
	return TRUE;
}

/*
 * Funzione che permette di mangiare allo squalo.
 *
 * \param r indice della riga.
 * \param c indice della colonna.
 *
 * \returns TRUE se lo squalo a mangiato altrimenti FALSE.
 *  
 * */
static int gnamgnam(int r,int c){
	/* Controlla se intorno c'è un pesce da mangiare */
	if(matrix[MOD(r-1,rows)][c].type==FISH){
		MOVE(MOD(r-1,rows),c,r,c)
		matrix[MOD(r-1,rows)][c].animale.shark.sd=Sd;
		return TRUE;
	}
	if(matrix[r][MOD(c+1,cols)].type==FISH){
		MOVE(r,MOD(c+1,cols),r,c)	
		matrix[r][MOD(c+1,cols)].animale.shark.sd=Sd;	                
		return TRUE;
	}
	if(matrix[MOD(r+1,rows)][c].type==FISH){
		MOVE(MOD(r+1,rows),c,r,c)
		matrix[MOD(r+1,rows)][c].animale.shark.sd=Sd;
		return TRUE;
 	}
	if(matrix[r][MOD(c-1,cols)].type==FISH){
		MOVE(r,MOD(c-1,cols),r,c)
		matrix[r][MOD(c-1,cols)].animale.shark.sd=Sd;
		return TRUE;
	}
	return FALSE;
}

/*
 * Funzione di gestionde degli squali.
 *
 * \param sx indice della colonna da dove deve iniziare a gestire
 *		gli squali.
 * \param dx indice della colonna dove deve arrivare a gestire
 *		gli squali.
 *  
 * */
static void shark_manager(int sx,int dx){
	int i,j;
    printf("muoviti muoviti\n");    
    for(i=sx;i<=dx;i++){
        printf("esterno %i \n",i);
            for(j=0;j<rows;j++){
                printf("interno %i", j);
                        if(matrix[j][i].type==SHARK){
                            printf("\nuno l'ho trovato:%i,,,,,,,%i\n",matrix[j][i].klife,kron);
                     
                                if(matrix[j][i].klife==kron){
                                    printf("kron\n");
                                	if(matrix[j][i].animale.shark.sd==0){
                                		/* Lo squalo muore */
                                	    printf("pippo\n");    
                                        matrix[j][i].type=WATER;
						nsquali--;
                                	}else{
                                        printf("mangiamuovi\n");
						if(!gnamgnam(j,i)){
							/* Se non può mangiare */
                                	        	if(matrix[j][i].animale.shark.sb<=0){
								newborn(j,i);
								matrix[j][i].animale.shark.sb=Sb+1;
							}
							if(!move(j,i)){
							/* Lo squalo non si puo' muovere */
								matrix[j][i].klife++;
								matrix[j][i].animale.shark.sb--;
								matrix[j][i].animale.shark.sd--;
							}
						}else{
							npesci--;
						}
					}
                        	}
			}
	 	}
	}
}

/*
 * Funzione di gestione dei pesci
 *
 * \param sx indice della colonna da dove deve iniziare a gestire
 *		i pesci.
 * \param dx indice della colonna dove deve arrivare a gestire
 *		i pesci.
 *  
 * */
static void fish_manager(int sx,int dx){
	int i,j;
	for(i=sx;i<=dx;i++){
		for(j=0;j<rows;j++){
			if(matrix[j][i].type==FISH){
				/* Se non l'ho già spostato */
				if(matrix[j][i].klife==kron){
					if(matrix[j][i].animale.fb<=0){
						newborn(j,i);
						matrix[j][i].animale.fb=Fb+1;
					}
					if(!move(j,i)){
						matrix[j][i].klife++;
						matrix[j][i].animale.fb--;
					}						
				}
			}	

		}
	}

}

/*
 * Funzione di visualizzazione dello stato del mondo.
 *  
 * */
static void * init_screen(void* arg){
	int i,j,ret;
	while(TRUE){
		/* Blocco la matrice */
		pthread_mutex_lock(&mtx);
		/* Attendo che i lavoratori finiscano il loro compito */
		int err=pthread_cond_wait(&screen, &mtx);
        printf("asdsddasdasddasddadssadsadsadssporco:%i\n",err);
		//system("clear");
		fflush(stdout);
		/* Stampo il mondo wator e il numero di squali e di pesci presenti */
		printf(" NPESCI = %d NSQUALI = %d\n",npesci,nsquali);
		for(j=0;j<cols;j++)
			printf("---");
		printf("\n");
		for(i=0;i<rows;i++){
			printf("|");
			for(j=0;j<cols;j++){
				printf("%d|",matrix[i][j].type);
			}
			printf("|\n");
		}
		for(j=0;j<cols;j++)
			printf("---");
		printf("\n");
		/* Incremento il tempo di wator */
        printf("Porcone ladro\n\n\nneco");
        kron++;
		sleep(2);
		/* Sblocco i thread lavoratori che sono in attesa */
		pthread_cond_signal(&workerf);
		/* Sblocco la matrice */
		pthread_mutex_unlock(&mtx);

	}
}


/*
 * Funzione di gestione della matrice da parte dei thread.
 * */
static void * init_thread(void* args){
	int limit,i;
	limit=(int)args; /* Leggo la colonna da dove devo iniziare a gestire */
	while(TRUE){
		/* Blocco la matrice e gestisco gli squali */
		pthread_mutex_lock(&mtx);
        printf("Ora va il n:%d\n",limit);
        shark_manager(limit,LSUP(limit));
		if(thfrees>1){
			/* Attendo che tutti i thread che gestiscono gli squli finiscano */
			thfrees--;
			pthread_cond_wait(&workers, &mtx);
			if(thfrees!=N)
				/* Se ci sono sblocco thread in attesa */
                printf("io ci sono\n");
                pthread_cond_signal(&workers);
			thfrees++;
		}else{
			/* 'sblocco i thread in attesa */
			pthread_cond_signal(&workers);
		}
		/* Inizio a gestire i pesci */
		fish_manager(limit,LSUP(limit));
		if(thfreef>1){
			thfreef--;
			/* Attendo che tutti i thread che gestiscono i pesci finiscano */
			pthread_cond_wait(&workerf, &mtx);
		}else{
			thfreef--;
			/* mando il segnale al visualizzatore e attendo che finisca*/
			pthread_cond_signal(&screen);
			pthread_cond_wait(&workerf, &mtx);
		}
		/* Finchè non sono l'ultimo thread sbocco gli altri */
		if(thfreef!=N)
		  	pthread_cond_signal(&workerf);	  
		thfreef++;
		/* Sblocco la matrice */
		pthread_mutex_unlock(&mtx);
	}
}

/*
 * Funzione di inizializzazione della matrice.
 *  
 * */
static world init_world(){
	int i,j;
	world w;
	w=(world)calloc(rows,(sizeof(struct type*)));
	if(!w){
		fprintf(stderr,"Errore nella creazione della matrice!! Exit! \n");
		return (world)FALSE;
	}
	for(i=0;i<rows;i++){
		w[i]=(struct type*)calloc(cols,(sizeof(struct type)));
		if(!w[i]){
			for(j=i-1;j>=0;j--)
				free(w[j]);
			free(w);
			fprintf(stderr,"Errore nella creazione della matrice!! Exit \n");
			return (world)FALSE;
		}

	}
	return (world)w;
}

/*
 * Funzione per il controllo di correttezza dei parametri d'ingresso
 *
 * \param argc numero di parametri passati in ingresso.
 * \param argv lista dei parametri passati in ingresso.
 *  
 * \returns TRUE se i parametri sono corretti altrimenti FALSE.
 *
 * */
static int test_args(int argc, char* argv[]){
	char c;
	
	if((argc!=8) && (argc!=10)){
		fprintf(stderr,"Usage: wator rows cols Sb Sd Fb NF NS [-t N] \n");
		return FALSE;
	}
	/* Inizializzo i dati del programma con le 
	 * costanti passate come parametri */
	 
	rows=strtol(argv[1],(char **)NULL,10);
	cols=strtol(argv[2],(char **)NULL,10);
	Sb=strtol(argv[3],(char **)NULL,10);
	Sd=strtol(argv[4],(char **)NULL,10);
	Fb=strtol(argv[5],(char **)NULL,10);
	NF=strtol(argv[6],(char **)NULL,10);
	NS=strtol(argv[7],(char **)NULL,10);
	if(rows<=0 || cols<=0 || Sb<=0 || Sd<=0 || Fb<=0 || NF<=0 || NS<=0)
		return FALSE;
	/* Controllo del parametro opzionale */
//	if((c=getopt(argc,argv,"t:"))!=-1){
//		if(c=='t')
//			N=strtol(argv[9],(char **)NULL,10);
//		else{
//			fprintf(stderr,"Usage: wator rows cols Sb Sd Fb NF NS [-t N] \n");
//			return FALSE;
//		}	
//	}
    N=strtol(argv[9],(char **)NULL,10);
	/* Controllo che il numero di thread divide il numero 
	 * di colonne in modo corretto */
	if((cols%N)!=0){
		fprintf(stderr,"Errore nel numero di colonne o di thread!!\n");
		return FALSE;
	}
	/**
	 * Controllo che il numero di pesci e degli squali
	 * non saturino la matrice */
	if((NF+NS)>(cols*rows)){
		fprintf(stderr,"Errore nel numero di squali o di pesci!!\n");
		return FALSE;
	}
	
	fprintf(stderr,"= rows:%d cols:%d Sb:%d Sd:%d Fb:%d NF:%d NS:%d N:%d=\n",rows,cols,Sb,Sd,Fb,NF,NS,N);
    return TRUE;
}

int main(int argc,char* argv[]){
	int err,i=0;
	pthread_t* lthread; /* lista dei thread da creare */
	/* Controllo i parametri passati all'ingresso */
	if(!(test_args(argc,argv))){
        printf("Merda\n");
		return TRUE;
    }
    printf("E invece qui!!\n");    
	/* Creo la matrice */
	if(!(matrix=init_world()))
		return TRUE;
    printf("Qui passa!!\n");
	srand(getpid());
	initFish(); /* Inizializza la tabella con i pesci e gli squali */
	initShark();
	/* inizializzo la sista dei thread */
	lthread=(pthread_t *)calloc(N+1,sizeof(pthread_t));	
	if(!lthread){
		printf(" Errore nella creazione della lista dei thread!!\n ");
		return TRUE;
	}
	thfrees=N; /* Inizializzo il numero di thread che gestiranno i pesci e gli squali */
	thfreef=N;
	/* Creo il thread per la visualizzazione */
	if(err=pthread_create(&lthread[i],NULL, &init_screen,NULL)!=0){
		printf("Errore nella 22 creazione del thread!!%i\n ",err);
		return TRUE;
	}
	/* Creo gli N thread per la gestione della matrice */
	for(i=1;i<=N;i++){
		if(err=pthread_create(&lthread[i],NULL, &init_thread,(void*)((i-1)*(cols/N)))!=0){
			printf("Errore nella creazione del 11 thread!!\n ");
			return TRUE;
		}
	}
	/* Attendo la conclusione di un thread qualsiasi */
	pthread_join(lthread[0],NULL);	
	return FALSE;
}


