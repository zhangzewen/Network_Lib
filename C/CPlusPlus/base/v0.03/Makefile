CFLAGS= -g -std=c++11 -I/usr/local/include/boost -L/usr/local/lib -lboost_atomic -lboost_chrono -lboost_container -lboost_context -lboost_exception -lboost_locale -lboost_regex -lboost_serialization


main : Server.o Channel.o main.o Acceptor.o TcpTransport.o
	g++ $(CFLAGS)  Server.o Channel.o main.o Acceptor.o TcpTransport.o -g -o main

Server.o : Server.cc Server.h Channel.h  Acceptor.h
	g++ $(CFLAGS) -c Server.cc -g -o Server.o

Channel.o : Channel.cc Channel.h
	g++ $(CFLAGS) -c Channel.cc -g -o Channel.o

Acceptor.o : Acceptor.cc Acceptor.h
	g++ $(CFLAGS) -c Acceptor.cc -g -o Acceptor.o

TcpTransport.o : TcpTransport.cc TcpTransport.h
	g++ $(CFLAGS) -c TcpTransport.cc -g -o TcpTransport.o

main.o : main.cc Server.h Channel.h
	g++ $(CFLAGS) -c main.cc -g -o main.o


clean :
	rm -fr *.o
	rm -fr main

