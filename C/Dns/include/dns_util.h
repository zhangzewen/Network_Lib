#ifndef _HTTP_DNS_UTIL_H_INCLUDED__
#define _HTTP_DNS_UTIL_H_INCLUDED__

#include "event_base.h"
#define T_A 1
#define T_NS 2
#define T_CNAME 3
#define T_AAAA 4
#define T_SOA 6
#define T_PTR 12
#define T_MX 15


typedef char *rr_data_parser(const uint8_t*, uint32_t, uint32_t, uint16_t, uint32_t);

typedef struct{
	uint16_t cls;
	uint16_t rtype;
	rr_data_parser *parser;
	const char *name;
	const char *doc;
	unsigned long long count;
}rr_parser_container;

rr_data_parser opts;
rr_data_parser escape;

extern rr_parser_container rr_parsers[];
rr_parser_container default_rr_parse;


typedef struct dns_question{
    char * name;
    uint16_t type;
    uint16_t cls;
    struct dns_question * next;
} dns_question;

typedef struct dns_rr {
    char * name;
    uint16_t type;
    uint16_t cls;
    const char * rr_name;
    uint16_t ttl;
    uint16_t rdlength;
    uint16_t data_len;
    char * data;
    struct dns_rr * next;
} dns_rr;

typedef struct {
    uint16_t id;
    char qr;
    char AA;
    char TC;
    uint8_t rcode;
    uint8_t opcode;
    uint16_t qdcount;
    dns_question * queries;
    uint16_t ancount;
    dns_rr * answers;
    uint16_t nscount;
    dns_rr * name_servers;
    uint16_t arcount;
    dns_rr * additional;
} dns_info;

void parse_dns(struct event *ev);
void create_dns_query(unsigned char *host, int query_type,
	unsigned char *buf, size_t *question_len, int *query_len);



#endif
