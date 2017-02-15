/*
 *  pthread.c
 *  Utrix-1.0
 *
 *  Created by MinixGroup on 05/11/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include "pthread_sched.h"
#include "pthread_errno.h"

#define TID_MAIN 0	/* Definisce il tid da assegnare al main */ 
#define TID_INIT -1 /* Definisce il TID del padre del main */
#define ESEGUITO 1	/* Indica che il thread e' stato gia' eseguito */
#define FIRST_THR 1 /* Indica il numero iniziale di thread considerando come numero 1 il main */


int pthread_initialized= FALSE; /* Definisce che la libreria non e' stata ancora inizializzata */

/*
 * init: Crea il thread che gestira' il main e inizializza lo scheduler come un thread.
 *
 * @return FALSE se e' accorso un errore duranete la creazione del thread
 * @return TRUE se l'inizializzazione e' avvenuta con successo 
 */
int init(){
	
	tbl_field_t tbl;
	tcb_t tcb;
	
	pth_globalsp_init; /* Inizializzo lo stac alla posizione attuale */
	
	/* Definisco la tabella dei thread */
	tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
	
	if(!tbl)
		return FALSE;
	
	/* Inizializza il tcb del main */
	tcb=(tcb_t)calloc(1,sizeof(tcb_s));
	if(!tcb)
		return FALSE;
	
	tcb->tid_f=TID_INIT;
	tcb->tid=tcb_n=TID_MAIN;
	tcb->prior=DEFAULT_PRIOR;
	tcb->state=NUOVO;
	
	tcb->ctx=(context_t)calloc(1,sizeof(context_s));
	tcb->ctx->eseguito=ESEGUITO;	
	if(!tcb->ctx){
		free(tbl);
		free(tcb);
		return FALSE;
	}
	
	
	/* Inizializzo la tabella dei thread */ 
	tbl->tcb=tcb;
	tbl->next=NULL;
	
	thread_n=FIRST_THR;/* Inizializzo il numero di thread ad 1 avendo inserito il main */
	
	
	thread_new=thread_new_c=tbl; /* Metto nella code dei thread nuovi il main in attesa di essere schedulato */
	
	sched=(context_t)calloc(1,sizeof(context_s));
	/* Inizializzo lo scheduler come un thread */
	sched_init();
	/*Fare funzione nello scheduler*/
	
	thread_exec=tcb; /* Devinisco come il thread in esecuzione il main */
	
	return TRUE;
	
	
}



/*
 * pthread_create: Crea un nuovo thread. Una volta creato, il thread esegue la funzione passata per 
 *                 argomento con i parametri passati.
 * @param	pth conterra' il tid del thread creato
 * @param	att definisce gli attributi del thread
 * @param	fun rappresenta la procedura di esecuzione del thread
 * @param	param rappresenta i parametri della procedura passata
 *
 * @return EAGAIN se e' accorso un errore duranete la creazione del thread
 * @return EINVAL se e' stato specificato un attributo non valido
 * @return OK se' la creazione ha avuto successo
 */

int pthread_create(pthread_t *pth, pthread_attr_t * att, void *(*fun)(void *) , void * param){
	tcb_t tcb;
	tbl_field_t tbl;
	
	if( pth == NULL || att != NULL || fun == NULL )
		return SETERR(EAGAIN);
	
	/* Controlla se la libreria e' stata inizializata */
	pthread_initialize();
	
	
	tcb=(tcb_t)calloc(1,sizeof(tcb_s));
	if(!tcb)
		return SETERR(EAGAIN);
	
	tbl=(tbl_field_t)calloc(1,sizeof(tbl_field_s));
	if(!tbl){
		free(tcb);
		return SETERR(EAGAIN);
	}
	
	tcb->tid_f=thread_exec->tid;
	tcb->tid=(*pth)=++tcb_n;
	tcb->save=JOINABLE;
	tcb->prior=DEFAULT_PRIOR;
	tcb->ctx=(context_t)calloc(1,sizeof(context_s));
	if(!tcb->ctx){
		free(tcb);
		free(tbl);
		return SETERR(EAGAIN);
	}
	tcb->state=NUOVO;
	
	/* Inizializzo il contesto del thread */
	tcb->ctx->f=fun;
	tcb->ctx->arg=param;
	tcb->ctx->eseguito=FALSE;
	
	
	tbl->tcb=tcb;
	
	/* Metto in coda dei thread nuovi il thread appena creato */
	if(thread_new==NULL)
		thread_new=thread_new_c=tbl;
	else
		thread_new_c=thread_new_c->next=tbl;
	
	thread_n++;
	
	
	/* Chiamo lo scheduler */
	pth_switch(thread_exec->ctx,sched);
	
	return SETERR(OK);
	
}	




/*pthread_self:Questa funzione restituisce il tid del thread in esecuzione(STANDARD)
 @return un valore di tipo pthread_t uguale al tid*/

pthread_t pthread_self(void){
	pthread_initialize();
	return ESECUTION_TID;	
}

/*pthread_yield: Questa funzione permette di rilasciare il processore*/

void pthread_yield() 
{   
	pthread_initialize();
	pth_switch(thread_exec->ctx,sched);
}






/*pthread_join:Questa funzione aspetta la terminazione del thread e restituisce il valore di ritorno del thread in value_ptr(STANDARD)
 @param thread è il tid del thread che devo attendere
 @param value_ptr è la zona di memoria che contiene il valore di ritorno del thread che aspetto.
 @return OK in caso di successo altrimenti uno degli errori sottostanti
 
 @error	EINVAL se il thread non è di tipo joinable
 @error ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
 @error EDEADLK se il tid passato come parametro è uguale al tid del thread chiamante
 */
int pthread_join(pthread_t thread, void ** value_ptr){
	tcb_t thread_search;
    pthread_initialize();
	if(thread==ESECUTION_TID)
		return SETERR(EDEADLK);
	
	thread_search=gettcb(thread);
#ifdef DEBUG
	CTRL_PRINT_PAR(pthread_join,Thread trovato %d,thread);
#endif
	/*Il thread cercato non è attivo*/
	if(!thread_search){
#ifdef DEBUG
		CTRL_PRINT_PAR(pthread_join,Thread  Non trovato col tid %d,thread);
#endif
		return SETERR(ESRCH);
	}
	/*Il thread cercato  è detachable*/
	if(thread_search->save==DETACHABLE)
		return SETERR(EINVAL);
	/*Il thread cercato è morto*/
	if(thread_search->state==ZOMBIE){
#ifdef DEBUG
		CTRL_PRINT_PAR(pthread_join,Thread  Zombie col tid %d,thread);
#endif
		
		/*Controllo se qualcuno ha gia fatto la join*/
		if(thread_search->thread_join){
#ifdef DEBUG
			CTRL_PRINT_PAR(pthread_join,Thread ha qualcuno in join col tid %d,thread);
#endif
			return SETERR(EINVAL);
		} 
		
		else{
			/*Nessuno ha mai fatto la join, quindi prendo i valori del thread morto e lo elimino del tutto*/
#ifdef DEBUG
			CTRL_PRINT_PAR(pthread_join,Thread %d: nessuno ha fatto la join,thread_exec->tid);
#endif
			if(value_ptr){
#ifdef DEBUG
				CTRL_PRINT_PAR(pthread_join,Thread %d: esiste un valore di ritorno salvato e ho spazio in memoria per memorizzarlo,thread_exec->tid);
#endif
				*value_ptr=(thread_search->thread_res.res);  
			}      
			schedthrkill(thread_search->tid);
			thread_n--;
			return SETERR(OK);
		}
		
		
	}
	else{
		/*Non è ancora morto*/
#ifdef DEBUG
		CTRL_PRINT_PAR(pthread_join,Thread %d non è morto,thread);
#endif
        /*Devo mettere in attesa il thread dopo aver salvato tutte le informazioni necessarie*/
        thread_search->thread_join=thread_exec;
        thread_search->thread_res.ptr_res=value_ptr;
        pth_sleep(ESECUTION_TID,JOIN);
        pth_switch(thread_exec->ctx,sched);		
        return SETERR(OK);
	}
}


/*pthread_detach Questa funzione rende un thread detach cioè quando terminerà potrà essere deallocato senza salvare nessun valore di ritorno.(STANDARD)
 @param thread è il tid del thread che devo rendere detach
 
 @return OK in caso di successo altrimenti uno degli errori sottostanti
 
 @error	EINVAL se il thread non è di tipo joinable
 @error ESRCH se non esiste un thread con il tid uguale a quello passato per parametro
 
 */
int  pthread_detach(pthread_t thread){
	tcb_t thread_search;
    pthread_initialize();
	
	thread_search=gettcb(thread);
	/*Cerco sulla lista morti*/
	if(!thread_search)
		return SETERR(ESRCH);/*Come da standard*/
	
	if(thread_search->state==ZOMBIE)/*Se è in stato di zombie allora lo cancello*/
	{	if(thread_search->thread_join)/*Qualcuno ha gia fatto la join*/
		return SETERR(EINVAL);
#ifdef DEBUG
		CTRL_PRINT_PAR(pthread_detach,Thread %d Zombie,thread);
#endif
		/*Uccido senza salvarmi le informazioni*/
		schedthrkill(thread_search->tid);
		thread_n--;
		return SETERR(OK);
	}
	else{
		/*Non è zombie quindi lo pongo di tipo detach*/
		
		if(thread_search->save!=JOINABLE)/*Non è join*/
			return SETERR(EINVAL);	
		/*Diventa detachable*/
		thread_search->save=DETACHABLE;
		return SETERR(OK);
    }
    /*Se aggiungiamo le opzioni devo fare un controllo in più vedi EINVAL sul manuale*/
}


/*pthread_exit: Questa funzione permette la terminazione del thread e value_ptr sarà il valore di ritorno(STANDARD)
 @param value_ptr è il valore di ritorno del thread.
 Non dovrebbe mai ritornare
 */


void pthread_exit(void* value_ptr){
	pthread_initialize();
    if(ESECUTION_TID==TID_MAIN)
	{
        /*Ripulisco la libreria*/
        /*Esco*/
		exit((int)((long)value_ptr));
	}
	
	if(thread_exec->save==JOINABLE){/*Era di tipo joinable*/
		if(thread_exec->thread_join){
			/*Qualcuno mi aspetta*/
			if(thread_exec->thread_res.ptr_res){
#ifdef DEBUG
				CTRL_PRINT_PAR(pthread_exit,Thread %d:il thread in attesa sulla join ha fornito una zona di memoria per memorizzare il risultato,thread_exec->tid);
#endif
				*(thread_exec->thread_res.ptr_res)=value_ptr;
            }
#ifdef DEBUG
			CTRL_PRINT_PAR(pthread_exit,Risveglio chi mi aspetta %d, thread_exec->thread_join->tid);
#endif
			
			schedthrkill(ESECUTION_TID);
			thread_n--;
			pth_unsleep(thread_exec->thread_join->tid,JOIN);
		}
		else{
#ifdef DEBUG
			CTRL_PRINT_PAR(pthread_exit,Salvo indirizzo %d,thread_exec->tid);
#endif
			/*Salvo il valore del thread e mi addormento per sempre*/
			(thread_exec->thread_res.res)=value_ptr;
			pth_sleep(ESECUTION_TID,ZOMBIESLEEP);
		}
	}
	else{/*Detachable possiamo togliere il thread senza salvare niente*/
		
		schedthrkill(ESECUTION_TID);
		thread_n--;
	}
	releasepart(thread_exec->ctx->part);
#ifdef DEBUG
	CTRL_PRINT_PAR(pthread_exit,Rilascio la parte %d,thread_exec->tid);
#endif
	pth_switch(thread_exec->ctx,sched);	
}


