LIBNAME=SimpleHttp
LIB_NAME=lib$(LIBNAME).a
TEST_DIR=$(DIR)/test

EXTENSION=cc
OBJS=$(patsubst %.$(EXTENSION), %.o,$(wildcard *.$(EXTENSION)))

INCLUDES=-I../internet/
LDFLAGS=-L../internet -lNet


CC=g++
CONFIGURE=
CFLAGS= -g3  -W -Werror -Wall

.PHONY: all clean test


all: $(OBJS)

%.o : %.cc
	g++ $(CFLAGS) $(INCLUDES) -c $< $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
	rm -fr core.*

