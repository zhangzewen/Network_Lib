DIR=$(shell pwd)
BIN_DIR=$(DIR)/bin
LIB_DIR=$(DIR)/lib
SRC_DIR=$(DIR)/src
INCLUDE_DIR=$(DIR)/include
OBJ_DIR=$(DIR)/obj
LIBNAME=Reactor
LIB_NAME=lib$(LIBNAME).a
LIB=$(LIB_DIR)/lib$(LIBNAME).a
TEST_DIR=$(DIR)/test


EXTENSION=c
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.$(EXTENSION)))

INCLUDE=-I$(INCLUDE_DIR)

CC=gcc
CONFIGURE=
CFLAGS=-Wall -W -g3 -Werror 

.PHONY: all init clean

all:$(OBJS)
	ar rcs $(LIB) $(OBJS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.$(EXTENSION) 
	$(CC) $< -o $@ -c $(CFLAGS) $(CONFIGURE) $(INCLUDE) 

http_echo: $(TEST_DIR)/http_echo.c $(LIB)
	$(CC) $< -o $(BIN_DIR)/$@ $(INCLUDE) $(LIB) $(CFLAGS) -lpthread
	
timeout: $(TEST_DIR)/timeout.c $(LIB)
	$(CC) $< -o $(BIN_DIR)/$@ $(INCLUDE) $(LIB) $(CFLAGS) -lpthread
clean:
	rm -rf $(OBJ_DIR)/* $(LIB_DIR)/* $(BIN_DIR)/*

