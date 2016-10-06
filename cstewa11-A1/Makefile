CFLAGS = -std=c99 -Wall
LIBFLAGS = -L ./ -llistio

lib: listio.o
	gcc ${CFLAGS} listio.c -o listio.o -c;\
	ar cr liblistio.a listio.o

listio.o: listio.c listio.h
	gcc -c listio.c ${CFLAGS}

clean:
	rm *.o
	
clobber:
	rm *.o liblistio.a
