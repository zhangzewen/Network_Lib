#ifndef __LOG_H_INCLUDED_
#define __LOG_H_INCLUDED_

#include <stdio.h>
#include <pthread.h>
#include <stdarg.h>

#define BUFFSIZE 1024*1024*8

typedef enum LogLevel {
	DEBUG = 1,
	TRACE,
	NOTICE,
	WARNING,
	ERROR
}LogLevel;


struct log_st {
	FILE *fp;
	char path[256];
	char buffer[BUFFSIZE];
};

int log_init(LogLevel level, char *path);
int Log(LogLevel level, char *format, ...);

#endif
