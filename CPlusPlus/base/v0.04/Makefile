main : Server.o Channel.o main.o Acceptor.o TcpTransport.o Dispatcher.o
	g++   Server.o Channel.o main.o Acceptor.o TcpTransport.o Dispatcher.o -g -o main

Server.o : Server.cc Server.h Channel.h EventCallBack.h Acceptor.h
	g++ -W -Werror -c Server.cc -g -o Server.o

Channel.o : Channel.cc Channel.h EventCallBack.h
	g++ -W -Werror -c Channel.cc -g -o Channel.o

Acceptor.o : Acceptor.cc Acceptor.h EventCallBack.h
	g++ -W -Werror -c Acceptor.cc -g -o Acceptor.o

TcpTransport.o : TcpTransport.cc TcpTransport.h
	g++ -W -Werror -c TcpTransport.cc -g -o TcpTransport.o

Dispatcher.o : Dispatcher.h Dispatcher.cc Channel.h
	g++ -W -Werror -c Dispatcher.cc -g -o Dispatcher.o

main.o : main.cc Server.h Channel.h EventCallBack.h
	g++ -W -Werror -c main.cc -g -o main.o


clean :
	rm -fr *.o
	rm -fr main
	rm -fr core.*

