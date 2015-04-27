FLAGS=-g3 -W -Werror -I/usr/local/libevent/include
LDFLAGS=-L/usr/local/libevent/lib -L./thirdparty/ -levent -lhttp_parser

main: main.o connection.o listener.o
	g++ main.o connection.o listener.o $(LDFLAGS) -o main
connection.o: connection.cc
	g++ $(FLAGS) -c connection.cc $(LDFLAGS) -o connection.o
listener.o: listener.cc
	g++ $(FLAGS) -c listener.cc $(LDFLAGS) -o listener.o

main.o: main.cc
	g++ $(FLAGS) -c main.cc $(LDFLAGS) -o main.o
clean:
	rm -fr core.*
	rm -fr main
	rm -fr a.out
