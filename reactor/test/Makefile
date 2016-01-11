OBJECTS=$(patsubst ../%.cc, ../%.o, $(wildcard ../*.cc))
CC=g++
CFLAGS := -g3 -W -Werror -Wall -O0 -std=c++11 -I../util/
#LDFLAGS := -pthread -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization
LDFLAGS := -lglog -lgtest
PWD=$(shell pwd)
INCLUDE=-I../

socket: socket.o ${OBJECTS}
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@

socket.o: socket.cc
	${CC} ${INCLUDE} ${CFLAGS} -c $< ${LDFLAGS} -o $@


clean:
	rm -fr *.o
	rm -fr *.a