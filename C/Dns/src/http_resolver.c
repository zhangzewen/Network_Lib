#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <errno.h>

#include "http_resolver.h"
#include "dns_util.h"
#include "event_base.h"
#include "dns_util.h"
#include "event.h"
#include "RBTree.h"
#include "file.h"

static unsigned int dnserver_index = 0;

int get_ip_port(const char *address, char *host, char *port)
{
  size_t len = 0;
  if (NULL == address) {
    return -1;
  }
  len = strlen(address);
  if (address[0] == '[') { // an ipv6 address
		char *ptr = strstr(address + 1, "]:");
		if (NULL == ptr) {
			return -1;
		}

		strncpy(host, address + 1, ptr - address -1);
		strcpy(port, ptr + 2);
		return 0;
  } else {
    char *ptr = strrchr(address, ':');
    if (NULL == ptr) {
			strncpy(host, address, len);
      return 0;
    }

    strncpy(host, address, ptr - address);
    strcpy(port, ptr + 1);
		return 0;
  }
  return -1;
}





int get_nameservers(struct dns_server *dns, int *count)
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int current = 0;

	fp = fopen(RESOLVER_CONFIG_FILE, "r");
	if (fp == NULL) {
		return -1;
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", line);

		if (line[0] == '#') {
			continue;
		}

		if (strncmp(line, "nameserver", 10) == 0) {
			if (current >= MAX_DNS_SERVERS) {
				break;
			}
			char address[256] = {0};
			char host[DEFAULT_HOST_LENGTH] = {0};
			char port[8] = {0};
			sscanf(line, "%*s%*[ \t]%[^ \t\n#]", address);
			get_ip_port(address, host, port);
			strcpy(dns[current].host, host);
			dns[current].port = port[0] == 0 ? 53 : atoi(port);
			current++;
		}
	}

	if (line) {
		free(line);
	}

	fclose(fp);
	*count = current;
	return 0;
}

struct resolver_result *resolver_result_new()
{
	struct resolver_result *new;
	new = (struct resolver_result *)calloc(1, sizeof(struct resolver_result));
	if (NULL == new) {
		return NULL;
	}

	new->ev = (struct event *)calloc(1, sizeof(struct event));

	if (NULL == new->ev) {
		free(new);
		return NULL;
	}
	return new;
}

struct resolver_st* resolver_create()
{

	struct resolver_st *new;
	new = (struct resolver_st *)malloc(sizeof(struct resolver_st));
	if (NULL == new) {
		fprintf(stderr, "malloc resolver_st error!\n");
		return NULL;
	}

	new->base = event_base_new();

	if (NULL == new->base) {
		perror("malloc event_base error!\n");
		free(new);
		return NULL;
	}

	return new;
}


int resolver_init(struct resolver_st *resolve)
{
//=========int dns server====================
	memset(resolve->DServer, 0, (sizeof(struct dns_server)* MAX_DNS_SERVERS));
	if (get_nameservers(resolve->DServer, &resolve->count) == -1) {
		fprintf(stderr, "get dns servers error!\n");
		return -1;
	}
//=======init  reactor============
	resolve->base = event_base_new();
	return 0;
}

void resolve_name(struct resolver_st *resolver, unsigned char *host)
{
	struct resolver_result *result = NULL;
	struct dns_server *dns = NULL;
	struct sockaddr_in remote;
	ssize_t nwrite = 0;
	int fd = -1;
	int robin = -1;
	unsigned char buf[65536] = {0};

	int sfd = -1;

	result = resolver_result_new();

	if (NULL == result) {
		fprintf(stderr, "calloc error!\n");
		return;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr,"socket error!=\n");
		return;
	}

	if (SetNonblocking(fd) != 0) {
		fprintf(stderr, "set noblocking error!\n");
		return ;
	}

	robin = (dnserver_index + 1) % resolver->count;
	dnserver_index = robin;
	dns = resolver->DServer + robin;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(dns->port);
	inet_pton(AF_INET, dns->host, &remote.sin_addr);

	sfd = connect(fd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));

	if (sfd < 0) {
		free(result);
		return;
	}

	create_dns_query(host, T_A, buf, &result->question_len, &result->query_len);
	nwrite = write(fd, buf, result->query_len);

	if (nwrite < 0) {
		fprintf(stderr, "Can not send dns request!\n");
		free(result);
		return;
	}

	event_set(resolver->base, result->ev, fd, EV_READ | EV_PERSIST, parse_dns, (void *)result, NULL);
	event_add(result->ev, NULL);
	return ;
}

void resolver_distory(struct resolver_st *resolver)
{

}

