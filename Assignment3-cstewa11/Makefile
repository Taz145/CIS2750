CFLAGS = -std=c99 -Wall -g
LIBFLAGS = -L./A3 -llistio

a3: A3/main.c A3/liblistio.a
	gcc ${CFLAGS} A3/main.c -o a3 ${LIBFLAGS}

A3/liblistio.a: A3/listio.o
	ar cr A3/liblistio.a A3/listio.o

A3/listio.o: A3/listio.c A3/listio.h
	gcc ${CFLAGS} -c A3/listio.c -o A3/listio.o 	

clean:
	rm A3/*.o

clobber:
	rm A3/*.o A3/liblistio.a a3
