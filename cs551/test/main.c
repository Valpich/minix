/*
 *  main.c
 *  
 *
 *  Created by lorenzo galeotti on 25/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "pth_errno.h"
#include "pthread_sched.h"


context_t cf;
context_t cw;
context_t mctx;
void f(void* arg);
void w(void* arg);
/*
void init()
{
  getPrisp(&globalSp);
  printf("appena prelevato %p\n",globalSp);
}

*/
void f(void* arg)
{
  printf("f:\n");
 /* pth_init(cw,w,NULL);*/
  pth_switch(cf,sched);
  cw=malloc(sizeof(context_s));
  
   pth_init(cw,w,NULL);
  printf("fine f\n");
  thread_n--;
  pth_sleep(1,2);
  pth_switch(cf,cw);
}

void w(void* arg)
{
  printf("w:%p\n",sched);
  pth_switch(cw,sched);
  printf("fine w\n");
  thread_n--;
  pth_sleep(2,2);
  pth_switch(cw,sched);
  
}

      
int main(void)
{
    __asm__("movl %%ebp,%0":"=r"(globalsp));
   printf("1\n");
   cf=malloc(sizeof (context_s));
   mctx=malloc(sizeof(context_s));
   sched=malloc(sizeof(context_s));
   pth_init(cf,f,NULL);
   pth_init(sched,scheduler,NULL);
  /* printf("2\n");
   pth_switch(mctx,cf);
   printf("Nel main\n");
   printf("switch\n");
   pth_switch(mctx,cw);
   printf("3\n");
   pth_switch(mctx,cf);
   //pth_switch(mctx,cw);
   printf("fine\n");*/
   thread_new=malloc(sizeof(tbl_field_s));
   thread_new->next=malloc(sizeof(tbl_field_s));
   thread_new->next->next=malloc(sizeof(tbl_field_s));
   
   thread_new->tcb=malloc(sizeof(tcb_s));
   thread_new->next->tcb=malloc(sizeof(tcb_s));
   thread_new->next->next->tcb=malloc(sizeof(tcb_s));

   
   thread_new->tcb->ctx=cf;
   thread_new->next->tcb->ctx=cw;
   thread_new->next->next->tcb->ctx=mctx;

   thread_new->tcb->tid=1;
   thread_new->next->tcb->tid=2;
   thread_new->next->next->tcb->tid=3;

   thread_n=3;
   pth_switch(mctx,sched);
   //int i;
   /*int v;
   char j;
   char w;
   char u;
   char k;
   printf("%i",sizeof(int));
   for(i=0;i<1000;i++) printf("%i-",i);*/
   pth_switch(mctx,sched);
   printf("fine\n");
}
