# Define required macros here
SHELL = /bin/sh

OBJS =  main.o
CFLAG = 
CC = clang++
INCLUDE =
LIBS = 

main:${OBJ}
   ${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
   -rm -f *.o core *.core

.cpp.o:
   ${CC} ${CFLAGS} ${INCLUDES} -c $<