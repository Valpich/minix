/*
 *  pthread.h
 *  Utrix
 *
 *  Created by MinixGroup on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

/*Librerie necessarie*/

#include<stddef.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
#include"pthread_syncr.h"

struct pthread_mutex_s;
struct pthread_cond_s;

typedef int pthread_attr_t;  /* Struttura dichiarata ma non inizializzata. Necessaria nella pthread_create per poter rispettare lo standard POSIX */

extern int       pthread_create(pthread_t* ,  pthread_attr_t* , void* (*)(void *), void *);

extern int       pthread_detach(pthread_t);

extern void       pthread_yield(void);

extern void      pthread_exit(void *);

extern int       pthread_join(pthread_t, void **);

extern int       pthread_kill(pthread_t, int);

extern pthread_t pthread_self(void); 

extern int 	 pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t * attr);

extern int 	 pthread_mutex_destroy(pthread_mutex_t* mutex);

extern int	 pthread_mutex_lock(pthread_mutex_t *mutex);

extern int 	 pthread_mutex_unlock(pthread_mutex_t *mutex);

extern int 	 pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t * attr);

extern int 	 pthread_cond_destroy(pthread_cond_t* cond);

extern int	 pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex);

extern int 	 pthread_cond_signal(pthread_cond_t * cond);

extern int   pthread_cond_broadcast(pthread_cond_t * cond);

/*Aggiungere le chiamate di condition*/

/*int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
*/







