CFLAGS=-g -Wall -Werror -I/usr/lib/libevent2/include/
LDFLAGS=-g -L/usr/lib/libevent2/lib -levent

main: main.o HttpServer.o
	g++ ${LDFLAGS} main.o HttpServer.o -o main

main.o: main.cc HttpServer.h
	g++ ${CFLAGS} -c main.cc ${LDFLAGS} -o main.o

HttpServer.o: HttpServer.cc HttpServer.h
	g++ ${CFLAGS} -c HttpServer.cc ${LDFLAGS} -o HttpServer.o

clean:
	rm -fr *.o
	rm -fr main
	rm -fr core*
