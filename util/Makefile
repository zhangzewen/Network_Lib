OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc
CFLAGS+=-g3 -W -Werror
LDFLAGS+= -pthread
LIBNAME=util
LIB_NAME=lib$(LIBNAME).a



all: $(OBJECTS)
	ar rcs $(LIB_NAME) $(OBJECTS)

$(objects): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $(LDFLAGS) $@

clean:
	rm -fr *.o
	rm -fr *.a
