FLAGS=-g3 -W -Werror -I/usr/local/libevent/include
LDFLAGS=-L/usr/local/libevent/lib -levent

main: main.o connection.o listener.o http_parser.o util.o
	g++ main.o connection.o listener.o http_parser.o util.o  $(LDFLAGS) -o main
connection.o: connection.cc
	g++ $(FLAGS) -c connection.cc $(LDFLAGS) -o connection.o
listener.o: listener.cc
	g++ $(FLAGS)  -c listener.cc $(LDFLAGS) -o listener.o

http_parser.o: http_parser.c
	g++ $(FLAGS) -c http_parser.c -o http_parser.o

util.o: util.cc
	g++ $(FLAGS) -c util.cc -o util.o

main.o: main.cc http_parser.h util.h
	g++ $(FLAGS)  -c main.cc $(LDFLAGS) -o main.o
clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
	rm -fr *.o
