/*
 *  pthread_errno.h
 *  Utrix
 *
 *  Created by MinixGroup on 11/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

#include<errno.h>
#include<stdio.h>
int pth_errno;

#define CTRL_PRINT(func,str) \
	fprintf(stderr,#func": "#str"\n");

#define CTRL_PRINT_PAR(func,str,par) \
	fprintf(stderr,#func": "#str"\n",par);

#define SETERR(err) errno=err;
#define OK 0




