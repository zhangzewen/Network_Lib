FLAGS=-g3 -W -Werror
INCLUDES=-I/usr/local/libevent144/include -I/usr/local/include/hiredis/
LDFLAGS=-L/usr/local/libevent144/lib -levent -lhiredis -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization

OBJS := $(patsubst %.cc, %.o, $(wildcard *.cc))
LIBNAME=Net
LIB_NAME :=lib$(LIBNAME).a
#OBJS=$(wildcard %.cc)

#main: connection.o  listener.o  main.o  redisClient.o  util.o
#	g++ connection.o listener.o main.o redisClient.o util.o $(LDFLAGS) -o main
#main: $(OBJS)
#	g++ $(OBJS) $(LDFLAGS) -o main
#all: $(OBJS)
#	    ar rcs $(LIB_NAME) $(OBJS)
#
all: main

main: $(OBJS) http_parser.o
	g++ $(OBJS) http_parser.o  $(LDFLAGS) -o main

%.o : %.cc
	g++ $(FLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

%.o : %.c
	g++ $(FLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
	rm -fr *.o
	rm -fr *.a
