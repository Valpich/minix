# *  Makefile
# *  
# *  Created by MinixGroup on 24/07/08.
# *  Copyright 2008 Utrix. All rights reserved.
# *

CC = gcc
CFLAGS =  -Wall -pedantic 
objects = pthread_context.o pthread_sched.o pthread.o pthread_struct.o pthread_syncr.o
included = pthread.h pthread_syncr.h pthread_lib.h pthread_struct.h
exe = libpthread.a

$(exe): $(objects)
	/usr/gnu/bin/gar ruv $(exe) $(objects)

pthread_context.o : pthread_context.h pthread_errno.h config.h	

pthread_sched.o : pthread_errno.h config.h  pthread_sched.h  $(included) 

pthread.o : pthread_errno.h pthread_sched.c	

pthread_struct.o : $(included)

pthread_syncr.o: pthread_errno.h pthread_sched.h $(included)


delete:
	 -rm $(exe) $(objects)

