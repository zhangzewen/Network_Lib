CFLAGS=-g -Wall -Werror -I/usr/local/libevent144/include/ -I/usr/local/include/hiredis/
LDFLAGS=-g -L/usr/local/libevent144/lib -levent -lhiredis  -L/usr/lib64/ -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization

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
