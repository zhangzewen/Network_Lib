#FLAGS=-g3 -W -Werror -Wall -O0 -std=c++11
FLAGS=-g  -O0 -std=c++11
INCLUDES= -I../util/ -I../reactor/
LDFLAGS=-lglog -lgtest -std=c++11

OBJS := $(patsubst %.cc, %.o, $(wildcard *.cc))
LIBNAME=Net
LIB_NAME :=lib$(LIBNAME).a

UTIL_OBJS := $(patsubst %.cc, %.o, $(wildcard ../util/*.cc))

OBJECTS :=$(patsubst ../reactor/%.cc, ../reactor/%.o, $(wildcard ../reactor/*.cc))

#OBJS=$(wildcard %.cc)

#main: connection.o  listener.o  main.o  redisClient.o  util.o
#	g++ connection.o listener.o main.o redisClient.o util.o $(LDFLAGS) -o main
#main: $(OBJS)
#	g++ $(OBJS) $(LDFLAGS) -o main
all: $(OBJS)
#all: $(OBJS) echoServer
#all: $(OBJS)
#	@echo '${FILES}'
#	    ar rcs $(LIB_NAME) $(OBJS)

%.o : %.cc
	g++ $(FLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

echoServer: $(OBJS) $(UTIL_OBJS) $(OBJECTS)
	g++ $(OBJS) $(UTIL_OBJS) $(OBJECTS) $(LDFLAGS) -o echoServer

clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
	rm -fr *.o
	rm -fr *.a
	rm -fr echoServer
