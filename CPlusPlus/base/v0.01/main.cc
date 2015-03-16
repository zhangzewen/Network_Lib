#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int createSocketAndListen()
{
	struct sockaddr_in srvaddr;
	int listenfd;
	socklen_t len = sizeof(srvaddr);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		fprintf(stderr, "socket error!\n");
		exit(-1);
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(8080);
	len = sizeof(srvaddr);
	if (bind(listenfd, (struct sockaddr*)&srvaddr, len) < 0) {
		fprintf(stderr, "socket bind error!\n");
		exit(-1);
	}
	int ret = listen(listenfd, 100);
	if (listenfd < 0) {
		fprintf(stderr, "listen error\n");
		exit(-1);
	}
	return listenfd;
}


int main(int argc, char** argv)
{
	int connfd = 0;
	int listenfd = 0;
	socklen_t len;
	struct sockaddr_in cliaddr; // cli addr
	listenfd = createSocketAndListen();
	len = sizeof(cliaddr);

	while(1) {
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
		if (connfd < 0) {
			continue;
		}
		char buf[1024] = {0};
		int ret = read(connfd, buf, 1024);
		fprintf(stderr, "read: %s\n", buf);
		ret = write(connfd, buf, strlen(buf));
		close(connfd);
	}
	return 0;
}
