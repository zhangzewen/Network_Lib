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
UTIL_INCLUDE=../util
UTIL_LIBPATH=../util
REACTOR_INCLUDE=../Reactor/include
REACTOR_LIBPATH=../Reactor/lib

EXTERN_LIB=-lutil -lReactor
EXTERN_LIBPATH=-L$(UTIL_LIBPATH) -L$(REACTOR_LIBPATH)


EXTENSION=c
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.$(EXTENSION)))

INCLUDE=-I$(INCLUDE_DIR) -I$(UTIL_INCLUDE) -I$(REACTOR_INCLUDE)

CC=gcc
CONFIGURE=
CFLAGS= -g3  -W -Werror

.PHONY: all clean test


all: init $(OBJS)
	ar rcs $(LIB) $(OBJS) $(UTIL_LIBPATH)/libutil.a $(REACTOR_LIBPATH)/libReactor.a

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
	$(CC) $< $(EXTERN_LIBPATH) $(EXTERN_LIB) -o $@ -c $(CFLAGS) $(CONFIGURE) $(INCLUDE) 

test: test_dns 

test_dns: $(TEST_DIR)/test_dns.c $(LIB)
	$(CC) $< -o $(BIN_DIR)/$@ $(INCLUDE) $(LIB) $(CFLAGS) -lpthread
  


clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

