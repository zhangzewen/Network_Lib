main : Server.o Channel.o main.o Acceptor.o TcpTransport.o
	g++  Server.o Channel.o main.o Acceptor.o TcpTransport.o -o main

Server.o : Server.cc Server.h Channel.h ChannelCallBack.h Acceptor.h
	g++ -c Server.cc -o Server.o

Channel.o : Channel.cc Channel.h ChannelCallBack.h
	g++ -c Channel.cc -o Channel.o

Acceptor.o : Acceptor.cc Acceptor.h ChannelCallBack.h
	g++ -c Acceptor.cc -o Acceptor.o

TcpTransport.o : TcpTransport.cc TcpTransport.h
	g++ -c TcpTransport.cc -o TcpTransport.o

main.o : main.cc Server.h Channel.h ChannelCallBack.h
	g++ -c main.cc -o main.o


clean :
	rm -fr *.o
	rm -fr main

