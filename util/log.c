#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "log.h"

static struct log_st LOG;
static pthread_mutex_t log_lock;

char* level_to_str(LogLevel level)
{
	switch(level) {
		case DEBUG:
			return "DEBUG";
		case TRACE:
			return "TRACE";
		case NOTICE:
			return "NOTICE";
		case WARNING:
			return "WARNING";
		case ERROR:
			return "ERROR";
		default:
			return "UNKNOWN";
	}
}



int log_init(LogLevel level, char *path)
{
	if (strlen(path) >= 256 - 1) {
		fprintf(stderr, "log file path too long!\n");
	}
	
	memset(LOG.path, 0, BUFFSIZE);
	strncpy(LOG.path, path, strlen(path));

	if ('\0' == LOG.path[0]) {
		LOG.fp = stdout;
		return 0;
	}
	LOG.fp = fopen(LOG.path, "a");
	if (NULL == LOG.fp) {
		fprintf(stderr, "can not access log file\n");
		return -1;
	}
	setvbuf(LOG.fp, (char *)NULL, _IOLBF, 0);
	
	pthread_mutex_init(&log_lock, NULL);
	return 0;
}


int Log_lock()
{
	if (pthread_mutex_trylock(&log_lock) == 0) {
		return 0;
	}
	return -1;
}


int Log_unlock()
{
	pthread_mutex_unlock(&log_lock);
	return 0;
}


int timer_prefix(char *buffer, LogLevel level)
{
	time_t now;
	now = time(&now);
	struct tm v;
	localtime_r(&now, &v);
	return snprintf(buffer, BUFFSIZE, "%s: %02d-%02d %02d:%02d%02d", level_to_str(level), v.tm_mon + 1, v.tm_mday, v.tm_hour, v.tm_min, v.tm_sec);
}


int Log(LogLevel level, char *logformat, ...)
{
	int size = 0;
	int len = 0;
	
	char *ptr = LOG.buffer;

	len = timer_prefix(ptr, level);

	ptr += len;
	strncpy(ptr, logformat, BUFFSIZE - len);
	
	va_list args;
	va_start(args, logformat);

	
	if (Log_lock() != 0) {
		vfprintf(stderr, LOG.buffer, args);
		fflush(stderr);
		va_end(args);
		return 0;
	}
	
	size = vfprintf(LOG.fp, LOG.buffer, args);
	fflush(LOG.fp);	
	va_end(args);
	Log_unlock();

	return 0;
}

int logclose()
{
	if (LOG.fp == NULL) {
		return 0;
	}	

	fflush(LOG.fp);
	fclose(LOG.fp);
	LOG.fp = NULL;
	pthread_mutex_destroy(&log_lock);
	return 0;
}

