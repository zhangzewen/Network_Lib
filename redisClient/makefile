CFLAGS=-g -Wall -Werror -I/usr/lib/libevent2/include/ -I/usr/local/include/hiredis/
LDFLAGS=-g -L/usr/lib/libevent2/lib -levent -lhiredis

main: main.o redisClient.o
	g++ ${LDFLAGS} main.o redisClient.o -o main

main.o: main.cc redisClient.h
	g++ ${CFLAGS} -c main.cc ${LDFLAGS} -o main.o

redisClient.o: redisClient.cc redisClient.h
	g++ ${CFLAGS} -c redisClient.cc ${LDFLAGS} -o redisClient.o

clean:
	rm -fr *.o
	rm -fr main
	rm -fr core*
