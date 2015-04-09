main : Server.o Channel.o main.o Acceptor.o TcpTransport.o
	g++ -std=c++11  Server.o Channel.o main.o Acceptor.o TcpTransport.o -g -o main

Server.o : Server.cc Server.h Channel.h ChannelCallBack.h Acceptor.h
	g++ -std=c++11 -c Server.cc -g -o Server.o

Channel.o : Channel.cc Channel.h ChannelCallBack.h
	g++ -std=c++11 -c Channel.cc -g -o Channel.o

Acceptor.o : Acceptor.cc Acceptor.h ChannelCallBack.h
	g++  -std=c++11 -c Acceptor.cc -g -o Acceptor.o

TcpTransport.o : TcpTransport.cc TcpTransport.h
	g++  -std=c++11 -c TcpTransport.cc -g -o TcpTransport.o

main.o : main.cc Server.h Channel.h ChannelCallBack.h
	g++  -std=c++11 -c main.cc -g -o main.o


clean :
	rm -fr *.o
	rm -fr main
	rm -fr core*

