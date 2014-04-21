#ifndef __LOG_H_INCLUDED_
#define __LOG_H_INCLUDED_

#include <stdio.h>
#include <pthread.h>
#include <stdarg.h>

#define BUFFSIZE 1024

typedef enum LogLevel {
	DEBUG = 1,
	TRACE,
	NOTICE,
	WARNING,
	ERROR
}LogLevel;


struct log_st {
	FILE *fp;
	LogLevel level;
	char path[256];
	char buffer[BUFFSIZE];
	char proc_name[256];

};

int log_init(LogLevel level, char *path);


#define log_debug(fmt, ...)		Log(DEBUG, __FILE__, __FUNCTION__, __LINE__, \
																	fmt, ##_VA_ARGS__);

#define log_trace(fmt, ...)		Log(TRACE, __FILE__, __FUNCTION__, __LINE__, \
																	fmt, ##_VA_ARGS__);

#define log_notice(fmt, ...)	Log(NOTICE, __FILE__, __FUNCTION__, __LINE__, \
																	fmt, ##_VA_ARGS__);

#define log_warning(fmt, ...)	Log(WARNING, __FILE__, __FUNCTION__, __LINE__, \
																	fmt, ##_VA_ARGS__);

#define log_error(fmt, ...)		Log(ERROR, __FILE__, __FUNCTION__, __LINE__, \
																	fmt, ##_VA_ARGS__);
#endif
