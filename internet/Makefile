FLAGS=-g3 -W -Werror -Wall
INCLUDES=-I/usr/local/libevent1414/include -I/usr/local/include/hiredis/ -I../util/
LDFLAGS=-L/usr/local/libevent1414/lib -levent -lhiredis -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization -lglog -lgtest

OBJS := $(patsubst %.cc, %.o, $(wildcard *.cc))
LIBNAME=Net
LIB_NAME :=lib$(LIBNAME).a

UTIL_OBJS := $(patsubst %.cc, %.o, $(wildcard ../util/*.cc))

#OBJS=$(wildcard %.cc)

#main: connection.o  listener.o  main.o  redisClient.o  util.o
#	g++ connection.o listener.o main.o redisClient.o util.o $(LDFLAGS) -o main
#main: $(OBJS)
#	g++ $(OBJS) $(LDFLAGS) -o main
all: $(OBJS) echoServer
#all: $(OBJS)
#	@echo '${FILES}'
#	    ar rcs $(LIB_NAME) $(OBJS)

%.o : %.cc
	g++ $(FLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

echoServer: $(OBJS) $(UTIL_OBJS)
	g++ $(OBJS) $(UTIL_OBJS) $(LDFLAGS) -o echoServer

clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
	rm -fr *.o
	rm -fr *.a
	rm -fr echoServer
