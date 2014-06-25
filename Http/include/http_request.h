#ifndef _HTTP_REQUEST_H_INCLUDED_
#define _HTTP_REQUEST_H_INCLUDED_

#include "http_buffer.h"
#include "http_parser.h"
#include "event_base.h"
#include "vector.h"
#include "str.h"

typedef struct http_request_st http_request_t;

typedef struct http_header_elt_st {
	str_t *key;
	str_t *value;
}http_header_elt_t;

typedef int (*http_handler)(http_request_t *r);
typedef int (*http_header_handler_pt)(http_request_t *r, http_header_elt_t *elt, size_t offset);

typedef struct http_header_elt_process_st {
	str_t name;
	size_t offset;                        
	http_header_handler_pt        handler;
}http_header_elt_process_t;      


typedef struct http_headers_in_st {
	http_header_elt_t *host;
	http_header_elt_t *connection;
	http_header_elt_t *if_modified_since;
	http_header_elt_t *if_unmodified_since;
	http_header_elt_t *user_agent;
	http_header_elt_t *referer;
	http_header_elt_t *content_length;
	http_header_elt_t *content_type;
	http_header_elt_t *range;
	http_header_elt_t *if_range;
	http_header_elt_t *transfer_encoding;
	http_header_elt_t *expect;
	http_header_elt_t *accept_encoding;
	http_header_elt_t *via;
	http_header_elt_t *authorization;
	http_header_elt_t *keep_alive;
	http_header_elt_t *x_forwarded_for;
	http_header_elt_t *x_real_ip;
	http_header_elt_t *accept;
	http_header_elt_t *accept_language;
	http_header_elt_t *depth;
	http_header_elt_t *destination;
	http_header_elt_t *overwrite;
	http_header_elt_t *date;
	http_header_elt_t *proxy_connection;
	str_t user;
	str_t passwd;
	vector *cookies;
	vector *set_cookies;
	str_t server;
	off_t content_length_n;
	time_t keep_alive_n;
	unsigned int keepalive:1;
	unsigned int connection_type:2;
}http_headers_in_t;

typedef struct http_headers_out_st {

    unsigned int status;
    str_t status_line;
    http_header_elt_t *server;
    http_header_elt_t *date;
    http_header_elt_t *content_length;
    http_header_elt_t *content_encoding;
    http_header_elt_t *location;
    http_header_elt_t *refresh;
    http_header_elt_t *last_modified;
    http_header_elt_t *content_range;
    http_header_elt_t *accept_ranges;
    http_header_elt_t *www_authenticate;
    http_header_elt_t *expires;
    http_header_elt_t *etag;
    off_t content_length_n;
    time_t date_time;
    time_t last_modified_time;
}http_headers_out_t;

struct http_request_st{
	http_header_elt_t *kv;
	void *data;
	http_handler read_event_handler;
	http_handler write_event_handler;
	http_headers_in_t headers_in;
	http_headers_out_t headers_out;
	http_parser *parser;
	http_parser_settings settings;
};

http_request_t *http_request_create();
void http_init_request(struct event *ev);
void http_process_request(struct event *ev);
extern http_header_elt_process_t http_headers_in[];


#if 0
ngx_http_free_request(r, rc);  
#endif
void http_request_close(http_request_t *r);  

#endif
