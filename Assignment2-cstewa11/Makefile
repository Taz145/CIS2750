CFLAGS = -std=c99 -Wall -g
LIBFLAGS = -L./ -llistio


liblistio.a: listio.o
	gcc ${CFLAGS} listio.c -o listio.o -c;\
	ar cr liblistio.a listio.o

main: main.c liblistio.a
	gcc ${CFLAGS} main.c -o main ${LIBFLAGS}

listio.o: listio.c listio.h
	gcc -c listio.c ${CFLAGS}

clean:
	rm *.o

clobber:
	rm *.o liblistio.a main
