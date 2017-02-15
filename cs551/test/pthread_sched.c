/*
 *  pthread_sched.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */



#include "config.h"
#include "pthread_sched.h"
#include "sys/jmp_buf.h"
#include "pthread_errno.h"
#include <stdlib.h>
#include <time.h>
/*#define DEBUG*/

tbl_field_t  thread_priortail[NUM_PRIOR];
tbl_field_t  thread_priorhead[NUM_PRIOR];
tbl_field_t  thread_blocked[NUM_WHY];
tbl_field_t  thread_garbage;

int scheduledthr_n;
clock_t pth_time;

context_t sched;

tbl_field_t selectthr();
void longtermsched();

/*macro che elimina elem da list parent deve essere il predecessore di elem*/
#define ELIM(elem,parent,list) if (!parent)list=list->next;\
else parent->next=elem->next;\
elem->next=NULL;\

/*aggiunge un elemento in coda*/
#define ADDELEM(elem,tail,head) if (!head){ head=elem;\
tail=head;\
}	\
else{tail->next=elem;\
tail=tail->next;\
}\
elem->next=NULL;
/*aggiunge un elemento in testa*/
#define ADDELEMHEAD(elem,head,tail) if (!head){ head=elem;\
tail=head;\
}else{\
elem->next=head;\
head=elem;\
}


#ifdef DEBUG 
/*stampa la lista i-esima trà quelle delle priorità*/
void stampalista(int i)
{ 	
	tbl_field_t ap=thread_priorhead[i];
	while (ap) 
	{
		printf("elemento:%i-",ap->tcb->tid);
		ap=ap->next;
	}
	printf("elemento:*\n");
}
#endif


void recalcprior(tbl_field_t thr);
void empty(void* arg){
#ifdef DEBUG
	abort();
#endif
	return;
}

/*cerca nella lista list il thread con il tid passato torna il thread ed il predecessore*/
int searchonlist(int tid, tbl_field_t list, tbl_field_t* serc , tbl_field_t* parent)
{
	/*imposto i parametri per scorrere la lista*/
	(*parent)=NULL;
	(*serc)=list;
	/*scorro la lista*/
	while((*serc))
	{
		/*se lo trovo esco e torno TRUE*/
		if ((*serc)->tcb->tid==tid) return TRUE;
		(*parent)=(*serc);
		(*serc)=(*serc)->next;
	} 
	/*non ho trovato il tid cercato*/
	return FALSE;
}
/*cerca il tid in tutte le liste dello scheduler*/
int searchonall(int tid,tbl_field_t* serc,tbl_field_t* parent, tbl_field_t** lista)
{
	int i=0;
	while (i<NUM_PRIOR)
	{
		(*lista)=&thread_priorhead[i];
		if (searchonlist(tid,thread_priorhead[i],serc,parent)) return TRUE;
		i++;
	}
	i=0;
	while (i<NUM_WHY)
	{
		(*lista)=&thread_blocked[i];
		if (searchonlist(tid,thread_blocked[i],serc,parent)) return TRUE;
		i++; 
	}
	return FALSE;
}

void gc(){
	while(thread_garbage){
#ifdef DEBUG 
		CTRL_PRINT_PAR(gc,Elimino:%i\n,thread_garbage->tcb->tid);
#endif
		tbl_field_t paus=thread_garbage;
		thread_garbage=thread_garbage->next;
		free(paus->tcb->ctx);
		free(paus->tcb);
		free(paus);
	}
}

void scheduler(void* arg)
{
	/*thread schedulato*/
	tbl_field_t  selectedthr;
	void * (*f)(void*);
	
#ifdef DEBUG 
	CTRL_PRINT(scheduler, );
#endif
	/*inizializzo a 0 il nomero dei thread schedulati*/
	scheduledthr_n=0;
	
	while(TRUE)
	{
		/*controllo che tutti i nuovi thread siano inseriti*/
		if(scheduledthr_n<thread_n)longtermsched();
		if(scheduledthr_n>thread_n)gc();
		if (!(selectedthr=selectthr())){
			empty(NULL);
		}else{
			
#ifdef DEBUG 
			CTRL_PRINT_PAR(scheduler,selected:%i,selectedthr->tcb->tid);
#endif
			
			/*imposto lo stato corretto*/
			selectedthr->tcb->state=EXEC;
			thread_exec=selectedthr->tcb;
			
#ifdef DEBUG 
			CTRL_PRINT_PAR(scheduler,parto%p,selectedthr);
#endif
			
			
			if(!selectedthr->tcb->ctx->eseguito)
			{
#ifdef DEBUG 
				CTRL_PRINT_PAR(scheduler,Prima esecuzione: %d,selectedthr->tcb->tid);
#endif
				selectedthr->tcb->ctx->eseguito=TRUE;
				f =selectedthr->tcb->ctx->f;
				selectedthr->tcb->timerInterval=1;
				/*inizializzo la variabile per il calcolo del tempo di cpu utilizzato*/
				pth_time=clock();
				__asm__("movl %0,%%esp"::"r"(bpcalc(selectedthr->tcb->ctx)));
				f(selectedthr->tcb->ctx->arg);
			}else
			{
				/*inizializzo la variabile per il calcolo del tempo di cpu utilizzato*/
				pth_time=clock();
				/*passo al thread che ho selezionarto*/
				pth_switch(sched,selectedthr->tcb->ctx);
			}
			
#ifdef DEBUG 
			CTRL_PRINT_PAR(scheduler,ritorno %p,selectedthr);
#endif
			/*calcolo il tempo che ha utilizzato*/
			pth_time=clock()-pth_time;
			
			/*aggiorno il suo tempo di cpu globale*/
			selectedthr->tcb->time=+pth_time;
			/*se non è stato bloccato ricalcolo la sua priorità*/
			recalcprior(selectedthr);
		}
	}
}



tbl_field_t selectthr()
{  
	int i=-1;
	
#ifdef DEBUG 
	CTRL_PRINT(selector, );
#endif
	/*scorro i thread schedulabili*/
	while(i<NUM_PRIOR-1)
	{
		
#ifdef DEBUG
		printf("lista:%i\n",i);
		stampalista(PRIOR(i));
#endif
		/*il primo della lista più bassa viene selezionato*/
		if(thread_priorhead[PRIOR(i)]) return thread_priorhead[PRIOR(i)];
		i++;
	}
	return NULL;
}

/*consente di caricare tutti i thread nuovi nello scheduler*/
void longtermsched()
{ 
	/*puntatore fittizio per il campo prior di elementi in testa ad una lista*/
	tbl_field_t null;
	tbl_field_t  new;
	
#ifdef DEBUG
	CTRL_PRINT_PAR(longterm, thread schedulati %i,scheduledthr_n);
#endif
	
	/*campo della tabella fittizio*/
	null=NULL;
	/*imposto la variabile di scorrimento della lista nuovi*/
	new=thread_new;
	/*scorro la lista e carico i thread*/
	while(new && scheduledthr_n<thread_n)
	{
		/*elimino il thread dalla lista nuovi*/
		ELIM(new,null,thread_new);
		/*inizializzo ciò che riguarda lo scheduler*/
		new->tcb->prior=DEFAULT_PRIOR;
		new->tcb->state=PRONTO;
		new->tcb->time=0;
		/*aggiungo l'elemento alla lista di default*/
		ADDELEM(new,thread_priortail[PRIOR(DEFAULT_PRIOR)],thread_priorhead[PRIOR(DEFAULT_PRIOR)]);
		/*incremento il numero di thread schedulati*/
		scheduledthr_n++;
		new=thread_new;
	}
}

/*setta la priorità di thr a prior*/
void setprior(tbl_field_t thr,int prior)
{
	tbl_field_t tcb;
	tbl_field_t  parent;
#ifdef DEBUG 
	CTRL_PRINT_PAR(setprior,prior:%i,prior);
#endif
	if(!thr) 
	{ 
		SETERR(EINVAL);
		return;
	}
	/*cerco l'elemento*/
	if ( searchonlist(thr->tcb->tid,thread_priorhead[PRIOR(thr->tcb->prior)],&tcb,&parent))
	{
		/*se lo trovo lo sposto*
		 */
		ELIM(tcb, parent,thread_priorhead[PRIOR( thr->tcb->prior)]);
		thr->tcb->prior=prior; 
		ADDELEM(thr,thread_priortail[PRIOR(prior)],thread_priorhead[PRIOR(prior)]);
	} else thr->tcb->prior=prior;/*è bloccato calcolo la priorità ma lo lascio stare*/
	thr->tcb->timerInterval++;
}
/*ricalcola la priorità del thread*/
void recalcprior(tbl_field_t thr)
{
	if(!thr) SETERR(EINVAL);
	if (thr->tcb->state!=EXEC) return;
	if (thr->tcb->time<=(BONUSTIME*thr->tcb->timerInterval) && thr->tcb->prior>-1) setprior(thr,thr->tcb->prior-1);
	if (thr->tcb->time>=(MALUSTIME*thr->tcb->timerInterval) && thr->tcb->prior<1) setprior(thr,thr->tcb->prior+1); 
	thr->tcb->state=PRONTO;
}


void schedthrkill(int tid)
{
	tbl_field_t kill,parent;
	tbl_field_t* list;
	tbl_field_t* null;
	null=NULL;
	/*cerco il thread da uccidere*/
	if (!searchonall(tid,&kill,&parent,&list)) {
		SETERR(ESRCH);
		return;
	}
	/*lo elimino dallo scheduler*/ 
	ELIM(kill,parent,(*list));
	if(kill->tcb->tid!=thread_exec->tid) {
		free(kill->tcb->ctx);
		scheduledthr_n--;
	}else{
		ADDELEMHEAD(kill,thread_garbage,null);
	}
}



void pth_sleep(int tid,int why)
{
	tbl_field_t select_tcb,parent;
	tbl_field_t * list;
	tbl_field_t * null;
	null=NULL;
	/*controllo i dati*/
	if (why<NUM_WHY && why>=0) 
	{
		if (searchonall(tid,&select_tcb,&parent,&list) )
		{
#ifdef DEBUG 
			CTRL_PRINT_PAR(pth_sleep,sleep:%i\n,select_tcb->tcb->tid);
#endif
			/*metto il thread nella lista dedicata*/
			ELIM(select_tcb,parent,(*list));
			ADDELEMHEAD(select_tcb,thread_blocked[why],null);
			/*imposto lo stato*/
			if(why==ZOMBIESLEEP)
				select_tcb->tcb->state=ZOMBIE;
			else
				select_tcb->tcb->state=BLOCCATO;
			return;
		}
		SETERR(ESRCH);
	}
	SETERR(EINVAL);
}

void pth_unsleep(int tid,int why){
	tbl_field_t selected_tcb,parent;
	/*controllo i dati*/
	if (why<NUM_WHY && why>=0) 
	{
		if ( searchonlist(tid,thread_blocked[why],&selected_tcb,&parent))
		{ 
			/*rimetto il thread nella lista corrispondente alla sua priorità*/
			ELIM(selected_tcb,parent,thread_blocked[why]);
			ADDELEM(selected_tcb,thread_priortail[PRIOR(selected_tcb->tcb->prior)],thread_priorhead[PRIOR(selected_tcb->tcb->prior)]);
			selected_tcb->tcb->state=PRONTO;
		}
		SETERR(ESRCH);
	}
	SETERR(EINVAL);
}



tcb_t gettcb(int tid){
	int i=0;
	tbl_field_t serc;
	tbl_field_t par;
	/*cerco nelle liste di schedulazione*/
	
	while (i<NUM_PRIOR)
	{
		if (searchonlist(tid,thread_priorhead[i],&serc,&par)) return serc->tcb;
		i++;
	}
	/*nelle liste di boloccaggio*/
	i=0;
	while (i<NUM_WHY)
	{
		if (searchonlist(tid,thread_blocked[i],&serc,&par))return serc->tcb;
		i++; 
	}
	SETERR(ESRCH);
	return NULL;
}

void jumanji()
{
	int i;
	tbl_field_t elim;
	for(i=0;i<NUM_PRIOR;i++)
	{
		while(thread_priortail[i])
		{
			elim=thread_priortail[i];
			thread_priortail[i]=thread_priortail[i]->next; 
			free(elim->tcb->ctx->part);
			free(elim->tcb->ctx);
			free(elim->tcb);
			free(elim);
		}
	}
	
	for(i=0;i<NUM_WHY;i++)
	{
		while(thread_blocked[i])
		{   elim=thread_blocked[i];
			thread_blocked[i]=thread_blocked[i]->next;
			free(elim->tcb->ctx->part);
			free(elim->tcb->ctx);
			free(elim->tcb);
			free(elim);
		}
	}
	
	free(sched);
	
}








