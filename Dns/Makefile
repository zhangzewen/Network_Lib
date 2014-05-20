DIR=$(shell pwd)
BIN_DIR=$(DIR)/bin
LIB_DIR=$(DIR)/lib
SRC_DIR=$(DIR)/src
INCLUDE_DIR=$(DIR)/include
OBJ_DIR=$(DIR)/obj
LIBNAME=EasyDns
LIB_NAME=lib$(LIBNAME).a
LIB=$(LIB_DIR)/lib$(LIBNAME).a
TEST_DIR=$(DIR)/test


EXTENSION=c
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.$(EXTENSION)))

INCLUDE=-I$(INCLUDE_DIR)

CC=gcc
CONFIGURE=
CFLAGS=-Wall -W -g3 -Werror 

.PHONY: all clean test


all: init $(OBJS)
	ar rcs $(LIB) $(OBJS)

init:
	@if [ -d ${BIN_DIR} ];\
	then rm -fr ${BIN_DIR}/*;\
	else mkdir ${BIN_DIR};\
	fi;\
	if [ -d ${LIB_DIR} ];\
	then rm -fr ${LIB_DIR}/*;\
	else mkdir ${LIB_DIR};\
	fi;\
	if [ -d ${OBJ_DIR} ];\
	then rm -fr ${OBJ_DIR}/*;\
	else mkdir ${OBJ_DIR};\
	fi;

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.$(EXTENSION) 
	$(CC) $< -o $@ -c $(CFLAGS) $(CONFIGURE) $(INCLUDE) 


clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

