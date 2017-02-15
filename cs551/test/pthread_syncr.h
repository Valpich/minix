/*
 *  pthread_syncr.h
 *  Utrix
 *
 *  Created by MinixGroup on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pthread_lib.h"

#define INIT 10
#define LOCK 11
#define NO_ACTIVE 0
#define ACTIVE 1

/*Permette di inizializzare un mutex
 @param pth_mutex_t* mutex la struttura viene inizializzata a NULL 
 @param int active il valore viene posto ad ACTIVE perchè comunque il mutex viene considerato inizializzato
 
 */
#define PTHREAD_MUTEX_INITIALIZER {NULL,ACTIVE}


/*Permette di inizializzare una condition 
 @param pth_cond_t* condition la struttura viene inizializzata a NULL 
 @param int active il valore viene posto ad ACTIVE perchè comunque la condition viene considerata inizializzato
 
 */
#define PTHREAD_COND_INITIALIZER {NULL,ACTIVE}

/*Struttura necessaria per capire chi sta in attesa*/
typedef struct mutexWait{
	pthread_t own;
	struct mutexWait* next;
} mutexWait;

/*Struttura del mutex vero e proprio*/
typedef struct mutex{
	unsigned char state;
	unsigned int val;/*Valore del mutex*/
	pthread_t own;/*Tid di chi possiede il mutex*/
	mutexWait* list_head;
	mutexWait* list_tail;
	struct mutex* next;
	struct mutex* prev;
} pth_mutex_t;

/*Struttura più esterna del mutex, è un semplice contenitore*/
struct pthread_mutex_s{
	pth_mutex_t* mux;
	int active;
};

typedef struct pthread_mutex_s pthread_mutex_t;

/*Struttura necessaria per memorizzare una lista di condition*/
typedef struct el_cond{
	pthread_mutex_t* mux;
	pthread_t own;
	struct el_cond* next;
} el_cond_t;

/*Struttura effettiva di una condition*/
typedef struct cond{
	unsigned int state;
	el_cond_t* list_head;
	el_cond_t* list_tail;
	struct cond* next;
	struct cond* prev;
} pth_cond_t;

/*Struttura più esterna della condition, è un semplice contenitore*/
struct pthread_cond_s{
	pth_cond_t* condition;
	int active;
};

typedef struct pthread_cond_s pthread_cond_t ;
typedef int pthread_mutexattr_t;
typedef int pthread_condattr_t;

/*Lista dei mutex attivi*/

extern pth_mutex_t* list_mux;
extern pth_cond_t* list_cond;



