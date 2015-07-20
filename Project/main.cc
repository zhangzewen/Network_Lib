#include <iostream>
#include "HttpServer.h"

int main(int argc, char** argv)
{
	HttpServer* server = new HttpServer("127.0.0.1", 8080);
	server->start();
}
