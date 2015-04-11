main : Server.o Channel.o main.o Acceptor.o TcpTransport.o Dispatcher.o
	g++   Server.o Channel.o main.o Acceptor.o TcpTransport.o Dispatcher.o -g -o main

Server.o : Server.cc Server.h Channel.h EventCallBack.h Acceptor.h
	g++ -c Server.cc -g -o Server.o

Channel.o : Channel.cc Channel.h EventCallBack.h
	g++ -c Channel.cc -g -o Channel.o

Acceptor.o : Acceptor.cc Acceptor.h EventCallBack.h
	g++ -c Acceptor.cc -g -o Acceptor.o

TcpTransport.o : TcpTransport.cc TcpTransport.h
	g++ -c TcpTransport.cc -g -o TcpTransport.o

Dispatcher.o : Dispatcher.h Dispatcher.cc Channel.h
	g++ -c Dispatcher.cc -o Dispatcher.o

main.o : main.cc Server.h Channel.h EventCallBack.h
	g++ -c main.cc -g -o main.o


clean :
	rm -fr *.o
	rm -fr main

