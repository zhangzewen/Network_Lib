OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc
CFLAGS+=-g3 -W -Werror
LDFLAGS+= -pthread


all: $(OBJECTS)

$(objects): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $(LDFLAGS) $@

clean:
	rm -fr *.o
