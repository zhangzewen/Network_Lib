LIBNAME=SimpleHttp
LIB_NAME=lib$(LIBNAME).a
TEST_DIR=$(DIR)/test

EXTENSION=cc
OBJS=$(patsubst %.$(EXTENSION), %.o,$(wildcard *.$(EXTENSION)))


CC=g++
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
#	if [ ! -f $(REACTOR_LIBPATH)/libReactor.a ];\
#	then make -C $(REACTOR_PATH)/;\
#	fi;\
#	if [ ! -f $(UTIL_LIBPATH)/libutil.a ];\
#	then make -C $(UTIL_PATH)/;\
#	fi;


$(OBJ_DIR)/%.o:$(SRC_DIR)/%.$(EXTENSION) 
	$(CC) $< $(EXTERN_LIBPATH) $(EXTERN_LIB) -o $@ -c $(CFLAGS) $(CONFIGURE) $(INCLUDE) 

test: test_parse 

test_parse: $(TEST_DIR)/test_parse.c $(LIB)
	$(CC) $< -o $(BIN_DIR)/$@ $(INCLUDE) $(LIB) $(CFLAGS) $(EXTERN_LIBPATH) $(EXTERN_LIB) -lpthread
  


clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
	rm -fr core.*

