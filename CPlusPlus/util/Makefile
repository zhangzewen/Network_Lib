OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS+=-g -W -Werror
LDFLAGS+= -pthread
LIBNAME=util
LIB_NAME=lib$(LIBNAME).a
PWD=$(shell pwd)
TEST_DIR=$(PWD)/test
INCLUDE=-I. -I../thirdParty/http-parser/



all: $(OBJECTS)
	ar rcs $(LIB_NAME) $(OBJECTS)

%.o: %.cc
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS) 

test: FileTest1


FileTest1: $(TEST_DIR)/FileTest1.cc $(LIB_NAME)
	  $(CC) $< -o $(TEST_DIR)/$@ $(INCLUDE) $(LIB_NAME) $(CFLAGS) -lpthread

clean:
	@-rm -fr *.o
	@-rm -fr *.a
