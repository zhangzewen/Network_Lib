#ifndef INTERNET_ECHO_H
#define INTERNET_ECHO_H

class EchoServer
{
public:
	EchoServer(const std::string&, int port);
	void start();
	bool makeNewConnection(int fd, struct event_base* base);
	connection::CONN_STATE onMessage(connection*, char*, int);
	connection::CONN_STATE processRequest(connection*);
private:
	std::string host_;
	int port_;
	struct event_base* base_;
	listener* listener_;
};
#endif //INTERNET_ECHO_H
