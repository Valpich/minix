/*
 *  pthread_context.c
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
#include "pthread_context.h"
#include "pthread_errno.h"
#include "config.h"
#include <stdlib.h>
/*#define DEBUG*/
partition_t partitionhead;
partition_t partitiontail;
int thread_n;
char* globalsp;




partition_t findfree();
int addpar(partition_t new);

char* bpcalc(context_t ctx)
{
    partition_t part=NULL;
	/*cerco partizioni libere*/
	if (!(part=findfree())) 
	{
		/*se non le trovo ne creo una*/
#ifdef DEBUG
		CTRL_PRINT(bpcalc,nuova partizione);
#endif
		part=malloc(sizeof(partition_s));
		if (addpar(part)) return NULL;
	}
	
#ifdef DEBUG 
	CTRL_PRINT_PAR(bpcalc,sp partizione:%p,part->bp);
#endif
	
	part->present=TRUE;
	ctx->part=part;
	/*ritorno la partizione da usare*/
	return part->bp;
}

/*torna 1 se non esistono partizioni*/ 
int isempty(){
	return !partitionhead;
}

/*se è possibile aggiunge una partizione valida alla lista*/
int addpar(partition_t new)
{
	/*controla la correttezza dei dati*/
	if (!new || !globalsp) return EINVAL;
    /*se non sono presenti altre partizioni*/
	if (isempty()){
		partitiontail=new;
		new->next=NULL;
		partitionhead=partitiontail;
    }else{
        new->next=partitionhead;
        partitionhead=new;
#ifdef DEBUG 
		CTRL_PRINT_PAR(bpcalc, Numero part %d,++val);
#endif
		
		
    }
	/*inizializza i dati delle partizioni*/
	new->bp=globalsp-STACKWIDTH;
	new->present=TRUE;
#ifdef DEBUG 
	CTRL_PRINT_PAR(bpcalc,sp:%p,new->bp);
#endif
	globalsp=new->bp;
	return OK;
}

/*cerca una partizione libera*/
partition_t findfree()
{
	partition_t res=partitionhead;
	/*cicla fino a trovare una partizione libera*/
	while (res)
	{
#ifdef DEBUG 
		CTRL_PRINT_PAR(findfree,cerca ancora:%p,res->bp);
#endif
		if(!res->present)return res;
		res=res->next;
	}
	return NULL;
}

/*libera la partizione passata come argomento*/
int releasepart(partition_t part)
{
#ifdef DEBUG 
	CTRL_PRINT_PAR(findfree,relasepart %p,part->bp);
#endif
	if (part) part->present=FALSE;
	else return SETERR(EINVAL);
	return OK;
}
