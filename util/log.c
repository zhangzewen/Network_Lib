#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>

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


int do_get_process_name(char *proc_path, char *proc_name)
{
	FILE *fp;
	char buf[1024] = {0};
	char tmp[64] = {0};
	
	fp = fopen(proc_path, "r");
	if (!fp) {
		perror("fopen error!\n");
		return -1;
	}

	while (fgets(buf, 1024, fp) != NULL) {
		if (strstr(buf, "Name") != NULL) {
			sscanf(buf, "%s %s", tmp, proc_name);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return -1;
}


int get_process_name(char *proc_name)
{
	DIR *dir;
	struct dirent *ptr;
	char tmp[1024];
	pid_t pid;

	if ((dir = opendir("/proc")) == NULL) {
		perror("opendir error!");
		return -1;
	}

	while ((ptr = readdir(dir)) != NULL) {
		if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..")) {
			continue;
		}

		pid = strtoul(ptr->d_name, '\0', 10);
		if (pid == getpid()) {
			snprintf(tmp, sizeof(tmp), "/proc/%s/status", ptr->d_name);
				if (!do_get_process_name(tmp, proc_name)) {
					closedir(dir);
					return 0;
				}
		}
	}

	closedir(dir);
	return -1;
}

int log_check(LogLevel level)
{
	return LOG.level < level;
}

int log_init(LogLevel level, char *path)
{
	if (strlen(path) >= 256 - 1) {
		fprintf(stderr, "log file path too long!\n");
		return -1;
	}
	
	LOG.level = level;
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

	get_process_name(LOG.proc_name);
	
	pthread_mutex_init(&log_lock, NULL);
	return 0;
}


int log_prefix(char *buffer, LogLevel level, char *filename, char *function, int line)
{
	time_t now;
	now = time(&now);
	struct tm v;
	localtime_r(&now, &v);
	return snprintf(buffer, BUFFSIZE, "[%s]%s: %02d-%02d %02d:%02d:%02d [ %s:%s:%d]",
																		LOG.proc_name,
																		level_to_str(level),
																		v.tm_mon + 1,
																		v.tm_mday,
																		v.tm_hour,
																		v.tm_min,
																		v.tm_sec,
																		filename,
																		function,
																		line);
}


int Log(LogLevel level, char *filename, char *function, int line, char *logformat, ...)
{
	int size = 0;
	int len = 0;
	
	char *ptr = LOG.buffer;
	
	if (!log_check(level)) {
		return 0;
	}

	len = log_prefix(ptr, level, filename, function, line);

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

