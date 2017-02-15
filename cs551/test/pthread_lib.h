/*
 *  pthread_lib.h
 *  Utrix-1.0
 *
 *  Created by MinixGroup on 05/11/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */
/* Default attribute */
#define DEFAULT_ATTR NULL

#define FALSE 0
#define TRUE !FALSE
#define pthread_initialize() \
			do{ \
				if (pthread_initialized == FALSE) { \
					pthread_initialized = TRUE; \
					init(); \
				} \
			}while(0)


int init();

extern int pthread_initialized;
typedef unsigned int pthread_t;
