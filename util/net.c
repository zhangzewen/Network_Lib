#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net.h"
#include "file.h"


listen_t *tcp_listen(const char *addr, const int port, const int backlog, int noblocking)
{
	listen_t *new = NULL;
	new = (listen_t *)calloc(1, sizeof(struct listen_st));

	if (!new) {
		return NULL;
	}

	new->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(new->fd < 0) {
		fprintf(stderr, "create socket error!");
		free(new);
		return NULL;
	}

	new->sockaddr.sin_family = AF_INET;
	new->sockaddr.sin_port = htons(port);
	int s = inet_pton(AF_INET, addr, &new->sockaddr.sin_addr);
	if (s <= 0) {
		close(new->fd);
		free(new);
		return NULL;
	}

	new->socklen = sizeof(new->sockaddr);
	new->addr_text = str_dup(addr, strlen(addr));
	if (!new->addr_text) {
		free(new);
		return NULL;
	}

	new->backlog = backlog;
	new->listening = 0;
	new->nonblocking = noblocking;
	return new;
}

int start_listen(listen_t *ls)
{
  if (!ls) {
    return -1;
  }
	
	if (ls->nonblocking) {
		if (SetNoblock(ls->fd) < 0) {
			fprintf(stderr, "set nonblocking error!\n");
			return -1;
		}
	}

  if(bind(ls->fd, (struct sockaddr *)&ls->sockaddr, ls->socklen) < 0) {
    fprintf(stderr, "bind error!");
    return -1;
  }

  if(listen(ls->fd, ls->backlog) < 0){
    fprintf(stderr, "listen error!\n");
    return -1;
  }

  ls->listening = 1;
  return 0;
}

int close_listen(listen_t *ls)
{
	if (NULL == ls) {
		fprintf(stderr, "listen socket has already free!\n");
		return 0;
	}


}
void free_listen(listen_t **ls);

