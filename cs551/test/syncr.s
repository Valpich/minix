/*
 *  syncr.s
 *  Utrix
 *
 *  Created by MinixGroup on 10/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */

.text
.globl _lock
/* void lock(int*val): questa funzione prenderà un puntatore e cambierà il valore in 1 o bloccandosi in caso contrario
@par: val il valore che è un semaforo binario*/
_lock:	movl 4(%esp),%eax/*Acquisisco l'indirizzo del valore*/
	movl (%eax),%ecx/*Acquisisco il contenuto di val*/
	movl %eax,%edx/*Mi salvo l'indirizzo*/
	movl $0,%eax
	movl $1,%ebx
	cmpxchg %ebx,%ecx/*Confronto e cambio i valori*/
	jne cambio
	movl %ecx,(%edx)/*Ripristino il valore in val*/
	movl %edx,4(%esp)
	ret

cambio: jmp _lock	
        ret	 



/* void unlock(int*val): questa funzione prenderà un puntatore e cambierà il valore di val in 0
@par: val il valore che è un semaforo binario*/
.globl _unlock
_unlock: movl 4(%esp),%eax/*Acquisisco l'indirizzo del valore*/
	 movl (%eax),%ebx/*Acquisisco il valore*/
	 movl $0,%ebx/*Lo pongo a 0*/
	 movl %ebx,(%eax)/*Ripristino*/
	 movl %eax,4(%esp)
	 ret


