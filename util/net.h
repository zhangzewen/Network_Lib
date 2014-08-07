#ifndef _UTIL_NET_H_INCLUDED__
#define _UTIL_NET_H_INCLUDED__

#ifdef __cplusplus
extern "c" {
#endif

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "str.h"
typedef struct listen_st listen_t;
struct listen_st {
	int fd;
	struct sockaddr_in sockaddr;
	socklen_t socklen;
	str_t *addr_text;
	int backlog;
	unsigned int listening:1;
	unsigned int nonblocking:1;
	unsigned int keepalive:1;
};


listen_t *tcp_listen(const char *addr, const int port, const int backlog, int nonblocking);
int start_listen(listen_t *ls);
int close_listen(listen_t *ls);
void free_listen(listen_t **ls);

#ifdef __cplusplus
}
#endif

#endif
