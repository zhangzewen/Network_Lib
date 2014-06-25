#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <error.h>

#include "http_request.h"
#include "Define_Macro.h"
#include "http_connection.h"
#include "event_base.h"
#include "http_buffer.h"

void http_request_close(http_request_t *r);
void visit(void *data)
{
  http_header_elt_t *v = (http_header_elt_t *)data;
  fprintf(stderr, "[%s]: %s\n", v->key->data, v->value->data);
}

#if 0
unsigned int hash(void *key)
{

  size_t i = 0;
  char *val_1 = (char *)key;
  unsigned int hash_val = 0;
  for(i = 0; i < strlen(val_1); i++){
    hash_val += (unsigned int)(val_1[i] * i);  
  }
  
  fprintf(stderr, "%s hash values is %u\n", val_1, hash_val);
  return hash_val;

}

int key_compare(void *dest_key, void *src_key)
{
  char *val_1 = (char *)dest_key;
  char *val_2 = (char *)src_key;
  return strcmp(val_1, val_2);
}


static HashTableOpt opt = {.hash = hash, .key_compare = key_compare, .key_free = NULL, .value_free = NULL};

#endif

http_request_t *http_request_create()
{
	http_request_t *new;
	new = calloc(1, sizeof(struct http_request_st));
	
	if (NULL == new) {
		return NULL;
	}
	return new;	
}


#if 0
void visit(void *data)
{
  struct header_keyval *v = (struct header_keyval *)data;
  fprintf(stderr, "[%s]: %s\n", v->key->data, v->value->data);
}
#endif

int call_process_begin_cb (http_parser *p)
{
  if (p) {

  }
  fprintf(stderr, "\n\n*** on_request_process_begin() called on paused parser ***\n\n");
  return 0;
}

int call_process_header_key_cb (http_parser *p, const char *buf, size_t len)
{
  assert( p != NULL);
  assert( buf != NULL);
  assert( len != 0);
  http_request_t *r = (http_request_t *)p->data;
  fprintf(stderr, "\n\n*** on_process_header_key() called on paused parser ***\n\n");

  r->kv = (http_header_elt_t *)calloc(1, sizeof(struct http_header_elt_st));
  if (!r->kv) {
    return -1;
  }

  r->kv->key = str_dup(buf, len);

  if (!r->kv->key) {
    return -1;
  }

  return 0;
}

int call_process_header_value_cb (http_parser *p, const char *buf, size_t len)
{
  assert( p != NULL);
  assert( buf != NULL);
  assert( len != 0);
  http_request_t *r = (http_request_t *)p->data;
	http_header_elt_process_t *header;
  fprintf(stderr, "\n\n*** on_process_header_field() called on paused parser ***\n\n");
  if (!r->kv) {
    return -1;
  }
  r->kv->value = str_dup(buf, len);

  if (!r->kv->value) {
    return -1;
  }

	for (header = http_headers_in; header->name.len != 0; header++) {
		if (strncmp(header->name.data, r->kv->key->data, header->name.len) == 0) {
			if ((header->handler(r, r->kv, header->offset)) != 0) {
				return -1;
			}
		}	
	}
	
  r->kv = NULL;

  return 0;
}

int call_process_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } 
  fprintf(stderr, "\n\n*** on_process_request_url() called on paused parser ***\n\n");
  char buffer[1024] = {0};
  sprintf(buffer, buf, len);
  fprintf(stderr, "[%d]:%s\n", len, buffer);
  return 0;
}

int call_process_body_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_process_body_cb() called on paused parser ***\n\n");
  return 0;
}

int call_process_headers_complete_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_process_headers_complete() called on paused "
                  "parser ***\n\n");
  return 0;
}

int call_process_complete_cb (http_parser *p)
{
  fprintf(stderr, "\n\n*** on_process_complete() called on paused "
                  "parser ***\n\n");
  http_request_t *r = (http_request_t *)p->data;
  return 0;
}

int call_process_response_status_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_status() called on paused parser ***\n\n");
  char buffer[1024] = {0};
  sprintf(buffer, buf, len);
  fprintf(stderr, "[%d]:%s\n", len, buffer);
  return 0;
}



int http_process_proxy_connection(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	http_header_elt_t **p;
	p = (http_header_elt_t **)((char *)&r->headers_in + offset);
	if (*p == NULL) {
		*p = elt;
		if (strncasecmp("keep-alive", elt->value->data, strlen("keep-alive")) == 0) {
			r->headers_in.connection_type = 2;
		}
		return 0;
	}	
	return -1;
}
int http_process_content_length(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	http_header_elt_t **p;	
	p = (http_header_elt_t **)((char *)&r->headers_in + offset);
	if (*p == NULL) {
		*p = elt;
		r->headers_in.content_length_n = atoi(elt->value->data);
		return 0;
	}
	return -1;
}
int ngx_http_process_cookies(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	if (!r->headers_in.cookies) {
		r->headers_in.cookies = vector_create();
		if (!r->headers_in.cookies) {
			return -1;
		}
	}
	if (r->headers_in.cookies->push(r->headers_in.cookies, (void*)elt) != 0) {
		return -1;
	}

	return 0;
}

int ngx_http_process_set_cookies(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	if (!r->headers_in.set_cookies) {
		r->headers_in.set_cookies = vector_create();
		if (!r->headers_in.set_cookies) {
			return -1;
		}
	}
	if (r->headers_in.set_cookies->push(r->headers_in.set_cookies, (void*)elt) != 0) {
		return -1;
	}

	return 0;
}

int http_process_connection(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	http_header_elt_t **p;	
	p = (http_header_elt_t **)((char *)&r->headers_in + offset);
	if (*p == NULL) {
		*p = elt;
		if (strncmp("close", elt->value->data, strlen("close")) == 0)  {
			r->headers_in.connection_type = 1;
		} else if (strncmp("keep-alive", elt->value->data, strlen("keep-alive")) == 0) {
			r->headers_in.connection_type = 2;
		}
		return 0;
	}
	return -1;
}

int http_process_header_line(http_request_t *r, http_header_elt_t *elt, size_t offset)
{
	http_header_elt_t  **p;

	p = (http_header_elt_t **) ((char *) &r->headers_in + offset);

	if (*p == NULL) {
		*p = elt;
		return 0;
	}
	return -1;
}

#if 0
int http_process_user_agent(http_request *r, http_header_elt_t *elt, size_t offset)
{
	return 0;
}
#endif
http_header_elt_process_t http_headers_in[] = {
	{ 
		str("Host"),
		offsetof(http_headers_in_t, host),
		http_process_header_line
	},

	{ 
		str("Connection"),
		offsetof(http_headers_in_t, connection),
		http_process_connection
	},

	{ 
		str("If-Modified-Since"),
		offsetof(http_headers_in_t, if_modified_since),
		http_process_header_line 
	},

	{ 
		str("If-Unmodified-Since"),
		offsetof(http_headers_in_t, if_unmodified_since),
		http_process_header_line
	},

	{ 
		str("User-Agent"),
		offsetof(http_headers_in_t, user_agent),
		http_process_header_line
	},

	{ 
		str("Referer"), 
		offsetof(http_headers_in_t, referer),

		http_process_header_line
	},

	{ 
		str("Content-Length"),
		offsetof(http_headers_in_t, content_length),
		http_process_content_length
	},

	{
		str("Content-Type"),
		offsetof(http_headers_in_t, content_type),
		http_process_header_line 
	},

	{ 
		str("Range"),
		offsetof(http_headers_in_t, range),
		http_process_header_line
	},

	{ 
		str("If-Range"),
		offsetof(http_headers_in_t, if_range),
		http_process_header_line 
	},

	{ 
		str("Transfer-Encoding"),
		offsetof(http_headers_in_t, transfer_encoding),
		http_process_header_line 
	},

	{
		str("Expect"),
		offsetof(http_headers_in_t, expect),
		http_process_header_line 
	},

	{ 
		str("Accept-Encoding"),
		offsetof(http_headers_in_t, accept_encoding),
		http_process_header_line 
	},

	{ 
		str("Via"), 
		offsetof(http_headers_in_t, via),
		http_process_header_line
	},

	{ 
		str("Authorization"),
		offsetof(http_headers_in_t, authorization),
		http_process_header_line
	},

	{ 
		str("Keep-Alive"),
		offsetof(http_headers_in_t, keep_alive),
		http_process_header_line
	},

	{ 
		str("X-Forwarded-For"),
		offsetof(http_headers_in_t, x_forwarded_for),
		http_process_header_line
	},

	{
		str("X-Real-IP"),
		offsetof(http_headers_in_t, x_real_ip),
		http_process_header_line 
	},

	{ 
		str("Accept"),
		offsetof(http_headers_in_t, accept),
		http_process_header_line
	},

	{ 
		str("Accept-Language"),
		offsetof(http_headers_in_t, accept_language),
		http_process_header_line
	},


	{ 
		str("Date"), 
		offsetof(http_headers_in_t, date),
		http_process_header_line 
	},

	{ 
		str("Cookie"), 
		0, 
		ngx_http_process_cookies 
	},

	{ 
		str("Set-Cookie"), 
		0, 
		ngx_http_process_set_cookies 
	},
	
	{
		str("Proxy-Connection"),
		offsetof(http_headers_in_t, proxy_connection),
		http_process_proxy_connection,
	},
	
	{
		{0, NULL},
		0,
		NULL
	}	
};

                                                                                    	
void http_init_request(struct event *ev)
{
	http_connection_t *c;
	http_request_t *r;
	struct event *rev;
	struct event *snd;

	c = (http_connection_t *)ev->ev_arg;

	rev = c->read;	
	if (rev->timeout) {//客户端请求接收超时就drop掉连接
		fprintf(stderr, "[%s:%d]client timed out", __func__, __LINE__);
		http_connection_close(c);
		return;
	}

	c->requests++;


	r = c->r;
	if (r) {
		//do something
	} else {
		//means that is a seconde requests
		r = http_request_create();	
		if (!r) {
			http_connection_close(c);
			return ;
		}
	}
	
	c->r = r;

	rev->callback = http_process_request;
	r->read_event_handler = NULL;

	r->settings.on_message_begin = call_process_begin_cb;
	r->settings.on_header_field = call_process_header_key_cb;
	r->settings.on_header_value = call_process_header_value_cb;
	r->settings.on_url = call_process_request_url_cb;
	r->settings.on_status = call_process_response_status_cb;
	r->settings.on_body = call_process_body_cb;
	r->settings.on_headers_complete = call_process_headers_complete_cb;
	r->settings.on_message_complete = call_process_complete_cb;

	r->parser = malloc(sizeof(http_parser));
	r->parser->data = (void *)r;
	r->data = (void*)c;

  rev->callback(rev);
}

void http_process_request(struct event *rev)
{
	http_connection_t *c;
	http_request_t *r;
	ssize_t nread;
	ssize_t nparsed;
	c = (http_connection_t *)rev->ev_arg;

	c = rev->ev_arg;
	r = c->r;

	fprintf(stderr, "[%s:%d]http process request line", __func__, __LINE__);
	if (rev->timeout) {
		fprintf(stderr, "[%s:%d]client timed out", __func__, __LINE__);
		//http_close_request(r);
		return;
	}

	//nread = read(c->fd, c->buffer, 4095);
  nread = buffer_read(c->buf, c->fd, 4095);
	if (nread < 0) {
		if (errno == EAGAIN)
			return;
	} else {
		//do something
	}

  printf("\n----------------------------------------\n");
  fprintf(stderr, "nread = %d\n", nread);
  printf("\n----------------------------------------\n");

	http_parser_init(r->parser, HTTP_REQUEST);
	nparsed = http_parser_execute(r->parser, &r->settings, c->buf->pos, nread);
	if (r->parser->upgrade) {
		//do something
	} else if (nparsed != nread) {
		//Handle error. Usually just close the connection.
	}
		
	if (r->headers_in.connection_type == 2) {
		http_set_keepalive(r);
	}
	
	http_request_close(r);
}

void http_request_close(http_request_t *r)
{

	http_connection_t *c;
	c = (http_connection_t *)r->data;
	free(r->parser);	
	free(r);
	http_connection_close(c);
}

#if 0
http_set_keepalive(http_reqeust_t *r)
{
    ngx_int_t                  i;
    struct event                *rev, *wev;
    http_connection_t          *c;
    http_connection_t     *hc;

    c = (http_connection_t *)r->data;
    rev = c->read;


    fprintf(stderr,  "[%s:%d]set http keepalive handler", __func__, __LINE__);

    c->log->action = "closing request";

    hc = r->http_connection;
    b = r->header_in;

    if (b->pos < b->last) {

        /* the pipelined request */

        if (b != c->buffer) {

            /*
             * If the large header buffers were allocated while the previous
             * request processing then we do not use c->buffer for
             * the pipelined request (see ngx_http_init_request()).
             *
             * Now we would move the large header buffers to the free list.
             */

            cscf = ngx_http_get_module_srv_conf(r, ngx_http_core_module);

            if (hc->free == NULL) {
                hc->free = ngx_palloc(c->pool,
                  cscf->large_client_header_buffers.num * sizeof(ngx_buf_t *));

                if (hc->free == NULL) {
                    ngx_http_close_request(r, 0);
                    return;
                }
            }

            for (i = 0; i < hc->nbusy - 1; i++) {
                f = hc->busy[i];
                hc->free[hc->nfree++] = f;
                f->pos = f->start;
                f->last = f->start;
            }

            hc->busy[0] = b;
            hc->nbusy = 1;
        }
    }

    r->keepalive = 0;

    ngx_http_free_request(r, 0);

    c->data = hc;

    ngx_add_timer(rev, clcf->keepalive_timeout);

    if (ngx_handle_read_event(rev, 0) != NGX_OK) {
        ngx_http_close_connection(c);
        return;
    }

    wev = c->write;
    wev->handler = ngx_http_empty_handler;

    if (b->pos < b->last) {

        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, c->log, 0, "pipelined request");

#if (NGX_STAT_STUB)
        (void) ngx_atomic_fetch_add(ngx_stat_reading, 1);
#endif

        hc->pipeline = 1;
        c->log->action = "reading client pipelined request line";

        rev->handler = ngx_http_init_request;
        ngx_post_event(rev, &ngx_posted_events);
        return;
    }

    hc->pipeline = 0;

    /*
     * To keep a memory footprint as small as possible for an idle
     * keepalive connection we try to free the ngx_http_request_t and
     * c->buffer's memory if they were allocated outside the c->pool.
     * The large header buffers are always allocated outside the c->pool and
     * are freed too.
     */

    if (ngx_pfree(c->pool, r) == NGX_OK) {
        hc->request = NULL;
    }

    b = c->buffer;

    if (ngx_pfree(c->pool, b->start) == NGX_OK) {

        /*
         * the special note for ngx_http_keepalive_handler() that
         * c->buffer's memory was freed
         */

        b->pos = NULL;

    } else {
        b->pos = b->start;
        b->last = b->start;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_HTTP, c->log, 0, "[%s:%d]hc free: %p %d",
                    __func__, __LINE__,
                   hc->free, hc->nfree);

    if (hc->free) {
        for (i = 0; i < hc->nfree; i++) {
            ngx_pfree(c->pool, hc->free[i]->start);
            hc->free[i] = NULL;
        }

        hc->nfree = 0;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_HTTP, c->log, 0, "[%s:%d]hc busy: %p %d",
                  __func__, __LINE__,
                   hc->busy, hc->nbusy);

    if (hc->busy) {
        for (i = 0; i < hc->nbusy; i++) {
            ngx_pfree(c->pool, hc->busy[i]->start);
            hc->busy[i] = NULL;
        }

        hc->nbusy = 0;
    }

#if (NGX_HTTP_SSL)
    if (c->ssl) {
        ngx_ssl_free_buffer(c);
    }
#endif

    rev->handler = ngx_http_keepalive_handler;

    if (wev->active && (ngx_event_flags & NGX_USE_LEVEL_EVENT)) {
        if (ngx_del_event(wev, NGX_WRITE_EVENT, 0) != NGX_OK) {
            ngx_http_close_connection(c);
            return;
        }
    }

    c->log->action = "keepalive";

    if (c->tcp_nopush == NGX_TCP_NOPUSH_SET) {
        if (ngx_tcp_push(c->fd) == -1) {
            ngx_connection_error(c, ngx_socket_errno, ngx_tcp_push_n " failed");
            ngx_http_close_connection(c);
            return;
        }

        c->tcp_nopush = NGX_TCP_NOPUSH_UNSET;
        tcp_nodelay = ngx_tcp_nodelay_and_tcp_nopush ? 1 : 0;

    } else {
        tcp_nodelay = 1;
    }

    if (tcp_nodelay
        && clcf->tcp_nodelay
        && c->tcp_nodelay == NGX_TCP_NODELAY_UNSET)
    {
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, c->log, 0, "[%s:%d]tcp_nodelay", __func__, __LINE__);

        if (setsockopt(c->fd, IPPROTO_TCP, TCP_NODELAY,
                       (const void *) &tcp_nodelay, sizeof(int))
            == -1)
        {
        {
#if (NGX_SOLARIS)
            /* Solaris returns EINVAL if a socket has been shut down */
            c->log_error = NGX_ERROR_IGNORE_EINVAL;
#endif

            ngx_connection_error(c, ngx_socket_errno,
                                 "setsockopt(TCP_NODELAY) failed");

            c->log_error = NGX_ERROR_INFO;
            ngx_http_close_connection(c);
            return;
        }

        c->tcp_nodelay = NGX_TCP_NODELAY_SET;
    }

#if 0
    /* if ngx_http_request_t was freed then we need some other place */
    r->http_state = NGX_HTTP_KEEPALIVE_STATE;
#endif

    c->idle = 1;
    ngx_reusable_connection(c, 1);

    if (rev->ready) {
        ngx_post_event(rev, &ngx_posted_events);
    }
}

}
#endif

