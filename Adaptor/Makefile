FLAGS=-g3 -W -Werror -I/usr/local/libevent144/include -I/usr/local/include/hiredis/
LDFLAGS=-L/usr/local/libevent144/lib -levent -lhiredis

OBJS := $(patsubst %.cc, %.o, $(wildcard *.cc))
#OBJS=$(wildcard %.cc)

#main: connection.o  listener.o  main.o  redisClient.o  util.o
#	g++ connection.o listener.o main.o redisClient.o util.o $(LDFLAGS) -o main
main: $(OBJS)
	g++ $(OBJS) $(LDFLAGS) -o main

%.o : %.cc
	g++ $(FLAGS) -c $< $(LDFLAGS) -o $@
clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
	rm -fr *.o
