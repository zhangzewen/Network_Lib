OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc
CFLAGS+=-g3 -W -Werror
LDFLAGS+= -pthread
LIBNAME=util
LIB_NAME=lib$(LIBNAME).a
PWD=$(shell pwd)
TEST_DIR=$(PWD)/test
INCLUDE=-I$(PWD)



all: $(OBJECTS)
	ar rcs $(LIB_NAME) $(OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS) 

test:	test_thread 
 
test_thread: $(TEST_DIR)/test_thread.c $(LIB_NAME)
	$(CC) $< -o $@ $(INCLUDE) $(PWD)/$(LIB_NAME) $(CFLAGS) $(LDFLAGS) 
   

clean:
	rm -fr *.o
	rm -fr *.a
	ls -I *.c $(TEST_DIR)
