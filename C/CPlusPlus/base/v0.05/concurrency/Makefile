main: main.o Thread.o
	g++ main.o Thread.o -o main

main.o: main.cc Thread.h
	g++ -c main.cc -o main.o

Thread.o: Thread.h Thread.cc
	g++ -c Thread.cc -o Thread.o

clean:
	rm -fr main
	rm -fr core.*
	rm -fr *.o
