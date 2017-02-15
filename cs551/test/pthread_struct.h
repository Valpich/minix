/*
 *  pthread_struct.h
 *  Utrix
 *
 *  Created by MinixGroup on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pthread.h"
#include "pthread_context.h"
#define ESECUTION_TID thread_exec->tid
#define DETACHABLE 1
#define JOINABLE 0
/*Qui ci saranno solo le dichiarazioni e strutture necessarie per lavorare con i thread*/
typedef union result{
	void** ptr_res;
	void*  res;
} result;
/* Thread Control Block */
typedef struct tcb{
	pthread_t tid_f;
	pthread_t tid;
    char save;
	result thread_res;/*Salvo l'informazione l'utilizzo varia a seconda se faccio prima la exit o la join*/
	struct tcb* thread_join;/*Al massimo solo un thread può fare la join sullo stesso thread, mettendolo nel tcb semplifico di molto il codice*/
	int prior;
	context_t ctx;
	int state;  /* Stato del thread */
	clock_t time;
	int timerInterval;
	sigset_t sig; /* Tabella dei segnali */
} tcb_s;

typedef tcb_s* tcb_t;

/* Thread table field */
typedef struct tbl_field{
    tcb_t tcb;
    struct tbl_field* next; 
} tbl_field_s;

typedef tbl_field_s* tbl_field_t;

/* Tid assegnato all'ultimo tcb creato */
int tcb_n;
/*numero dei thread creati e attivi*/
int  thread_n;
tcb_t thread_exec;/*Thread attualmente in esecuzione*/

/*liste di thread*/

/*Puntatore alla testa della lista dei thread appena creati e non schedulati*/
tbl_field_t thread_new;

/*Puntatore alla coda della lista dei thread appena creati e non schedulati*/
tbl_field_t thread_new_c;
