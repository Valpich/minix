
#ifndef WATOR_H_
#define WATOR_H_
#define STD_NTREAD 4
#include "pthread.h"
/** tipo booleano */
//typedef enum { TRUE = 1, FALSE = 0 } bool_t;

/*struttura che rappresenta un abitante del mare*/
typedef struct abi {
        unsigned int type: 2 ; /* 0 = non presente, 1 = pesce , 2 = squalo */
        int repTime; /*chronon passati dall'ultima riproduzione*/
        int lastEat; /*tempo passato dall'ultimo pasto*/
} abi_t ;

/*struttura contenente tutti i dati che necessita ogni thread worker*/
typedef struct worker_data {
	int id_th; /*id worker (DEBUG)*/
	int col_bgn; /*indice prima colonna di competenza del worker*/
	int col_end; /*indice ultima colonna di competenza del worker*/
	pthread_mutex_t bef_mtx; /*mutex per mutua esclusione in condivisione con thread sinistro*/
	pthread_mutex_t aft_mtx; /*mutex per mutua esclusione in condivisione con thread destro*/
}worker_data_t;

/*Struttura contenente i tid dei vari thread*/
typedef struct thread_man {
	 pthread_t* tid_worker;
	 pthread_t tid_visualizer;
} thread_man_t ;

thread_man_t* t_man;
 
/*per comodita ho enumerato le posizioni adiacenti per estrarne una random*/
typedef enum { UP = 0 , RIGHT = 1 , DOWN = 2 ,LEFT = 3 } near_pos_t;
typedef enum { T = 1, F = 0 } bool_t;
/*Squali : prima provano a mangiare, se no provano a spostarsi e in caso di impossibilità si fermano. 
   * se possono in entrambi i primi 2 casi si riproducono ad azone effettuata*/
	 

#endif /*WATOR_H_*/
