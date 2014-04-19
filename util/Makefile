OBJECTS= timer.o buf.o file.o hash.o log.o vector.o cJSON.o RBTree.o hashtable.o
CC=gcc
CFLAGS+=-g3 -W -Werror
LDFLAGS+= -pthread


all: $(OBJECTS)

$(objects): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $(LDFLAGS) $@

clean:
	rm -fr *.o
