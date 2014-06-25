#ifndef _HTTP_RESOLVE_H_INCLUDED__
#define _HTTP_RESOLVE_H_INCLUDED__

#include <netinet/in.h>
#include "event_base.h"

#define DEFAULT_HOST_LENGTH 256
#define MAX_DNS_SERVERS 10
#define DEFAULT_PORT 53
#define RESOLVER_CONFIG_FILE  "/etc/resolv.conf"

struct dns_server{
	char host[200];
	int port;
};

struct resolver_result{
	struct event *ev;//let the ev callback to do send query and parse the response
	char *host;
	char **value;
	size_t question_len;
	int query_len;
	void (*handler)(void *arg);
	void *data;
};

struct resolver_st{
	struct dns_server DServer[MAX_DNS_SERVERS];
	int count;
	struct event_base *base;
};

struct resolver_st *resolver_create();
int resolver_init(struct resolver_st *resolver);
void resolve_name(struct resolver_st *resolver, unsigned char *host);
void resolver_distory(struct resolver_st *resolver);

#endif
