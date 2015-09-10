LIBNAME=SimpleHttp
LIB_NAME=lib$(LIBNAME).a
TEST_DIR=$(DIR)/test

EXTENSION=cc
OBJS=$(patsubst %.$(EXTENSION), %.o,$(wildcard *.$(EXTENSION)))

INCLUDES=-I../internet/ -I./third_party/http-parser/ -I/usr/local/libevent1414/include -I/usr/local/include/hiredis/ -I../util/ -I../buffer/
LDFLAGS=-L/usr/local/libevent1414/lib -levent -lhiredis -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization -lglog -lgtest -L./third_party/http-parser/ -lhttp_parser
#LDFLAGS=-L../internet -lNet -L/usr/local/libevent1414/lib -levent -lhiredis -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization -L./third_party/http-parser/ -lhttp_parser -Wl,-Bdynamic


UTIL_OBJS := $(patsubst %.cc, %.o, $(wildcard ../util/*.cc))
INTERNET_OBJS :=$(patsubst %.cc, %.o, $(wildcard ../internet/*.cc))
BUFFER_OBJS :=$(patsubst %.cc, %.o, $(wildcard ../buffer/*.cc))

CC=g++
CONFIGURE=
CFLAGS= -g3  -W -Werror -Wall

.PHONY: all clean test


all: $(OBJS) httpServer

%.o : %.cc
	g++ $(CFLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

httpServer: $(OBJS) $(UTIL_OBJS) $(INTERNET_OBJS) $(BUFFER_OBJS)
	g++ $(OBJS) $(INTERNET_OBJS) $(UTIL_OBJS) $(BUFFER_OBJS) $(LDFLAGS) -o httpServer

clean:
	rm -rf *.o 
	rm -fr core.*
	rm -fr httpServer

