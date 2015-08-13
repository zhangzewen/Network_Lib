#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include "logger.h"

/* ------------------------------------------------------------------------- */

#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_USER      1
#define LOG_LEVEL_INFO      2
#define LOG_LEVEL_WARNING   3
#define LOG_LEVEL_ERROR     4
#define LOG_LEVEL_FATAL     5
#define LOG_LEVEL_MAX       6

static const char* log_level_str[] = {
    "debug", "user", "info", "warning", "error", "fatal",
};

/* fields of logging timestamp */
struct log_tm {
    unsigned short tm_year;
    unsigned short tm_mon;
    unsigned short tm_mday;
    unsigned short tm_hour;
} __attribute__((packed));

/* log filename may be one of the following forms:
 *     - <prefix>YYYY-MM-DD.<level>
 *     - <prefix>YYYY-MM-DD_hh.<level>
 *     - <prefix>YYYY-MM-DD_hh:mm:ss.<level>
 *
 * strlen("YYYY-MM-DD_hh:mm:ss.") == 20
 * max(strlen(log_level_str[])) == strlen("warning") == 7
 *
 * PATH_BUFLEN should be >= 28 (including trailing '\0') */

#define PATH_BUFLEN 1024

#define PATH_PREFIX_BUFLEN  (PATH_BUFLEN \
                             - sizeof(unsigned long) \
                             - sizeof(void*) - sizeof(void*))

/* variables that will be initialized in logger_init() */
struct logger_var {
    unsigned long max_file_size;

    int (*rotate_trigger)(const struct tm* current, const struct log_tm* old,
                          unsigned long max_file_size, unsigned long filesize);
    void (*get_filename)(char* buf, int level, const struct tm* ts);

    char path_prefix[PATH_PREFIX_BUFLEN];
} __attribute__((packed));

#define MAX_LOG_LEN (4096 \
                     - sizeof(FILE*) \
                     - sizeof(struct log_tm) \
                     - sizeof(unsigned int) \
                     - sizeof(unsigned long) \
                     - sizeof(pthread_mutex_t))

/* sizeof(logger_info) == 4096 */
struct logger_info {
    FILE* fp;
    struct log_tm ts; /* timestamp of last write */
    unsigned int level;
    unsigned long filesize;
    pthread_mutex_t lock;
    char buf[MAX_LOG_LEN];
} __attribute__((packed));

struct logger_impl {
    struct logger_var var;
    struct logger_info o_o[LOG_LEVEL_MAX];
};

/* ------------------------------------------------------------------------- */

static void __new_log_level_file(struct logger_info* logger,
                                 struct logger_var* var,
                                 const struct tm* ts)
{
    int len;
    char path[PATH_BUFLEN];

    logger->filesize = 0;

    if (logger->fp && logger->fp != stdout && logger->fp != stderr)
        fclose(logger->fp);

    len = sprintf(path, "%s", var->path_prefix);
    var->get_filename(path + len, logger->level, ts);

    logger->fp = fopen(path, "a");
    if (!logger->fp) {
        fprintf(stderr, "cannot create/open file `%s', "
                "log content of level [%s] is redirected to %s.\n",
                path, log_level_str[logger->level],
                (logger->level <= LOG_LEVEL_INFO) ? "stdout" : "stderr");
        logger->fp = (logger->level <= LOG_LEVEL_INFO) ? stdout : stderr;
    }
}

/* time format: YYYY-MM-DD hh:mm:ss.uuuuuu
 * size >= 27 */
static inline void current_datetime(char *buf, int size, struct tm* tp)
{
    int len;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, tp);
    len = strftime(buf, size, "%F %T", tp);
    sprintf(buf + len, ".%06ld", tv.tv_usec);
}

#include <unistd.h>
#include <sys/syscall.h>

static void generic_logger(struct logger_info* logger,
                           struct logger_var* var,
                           const char* extra_info,
                           const char* fmt, va_list args)
{
    int len;
    struct tm tm;
    char timestr[32];

    pthread_mutex_lock(&logger->lock);

    current_datetime(timestr, 32, &tm);

    if (var->rotate_trigger(&tm, &logger->ts, var->max_file_size,
                            logger->filesize)) {
        __new_log_level_file(logger, var, &tm);

        logger->ts.tm_hour = tm.tm_hour;
        logger->ts.tm_mday = tm.tm_mday;
        logger->ts.tm_mon = tm.tm_mon;
        logger->ts.tm_year = tm.tm_year;
    }

    len = sprintf(logger->buf, "%s %ld %s\t", timestr,
                  syscall(__NR_gettid), extra_info);
    vsnprintf(logger->buf + len, MAX_LOG_LEN - len, fmt, args);

    len = fprintf(logger->fp, "%s\n", logger->buf);
    fflush(logger->fp); /* flush cache to disk */

    logger->filesize += len;

    pthread_mutex_unlock(&logger->lock);
}

void __logger_user(struct logger* l, const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    generic_logger(&handler->o_o[LOG_LEVEL_USER], &handler->var,
                   "", fmt, args);
    va_end(args);
}

void __logger_info(struct logger* l, const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    generic_logger(&handler->o_o[LOG_LEVEL_INFO], &handler->var,
                   "", fmt, args);
    va_end(args);
}

#define MAX_POS_INFO_LEN 128

static inline void log_with_pos_info(struct logger_info* logger,
                                     struct logger_var* var,
                                     const char* filename, int line,
                                     const char* fmt, va_list args)
{
    char pos_info[MAX_POS_INFO_LEN];

    snprintf(pos_info, MAX_POS_INFO_LEN, "%s:%d", filename, line);
    generic_logger(logger, var, pos_info, fmt, args);
}

#ifndef NDEBUG
void __logger_debug(struct logger* l, const char* filename, int line,
                    const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    log_with_pos_info(&handler->o_o[LOG_LEVEL_DEBUG], &handler->var,
                      filename, line, fmt, args);
    va_end(args);
}
#endif

void __logger_warning(struct logger* l, const char* filename, int line,
                      const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    log_with_pos_info(&handler->o_o[LOG_LEVEL_WARNING], &handler->var,
                      filename, line, fmt, args);
    va_end(args);
}

void __logger_error(struct logger* l, const char* filename, int line,
                    const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    log_with_pos_info(&handler->o_o[LOG_LEVEL_ERROR], &handler->var,
                      filename, line, fmt, args);
    va_end(args);
}

void __logger_fatal(struct logger* l, const char* filename, int line,
                    const char* fmt, ...)
{
    va_list args;
    struct logger_impl* handler = l->handler;

    va_start(args, fmt);
    log_with_pos_info(&handler->o_o[LOG_LEVEL_FATAL], &handler->var,
                      filename, line, fmt, args);
    va_end(args);
}

/* ------------------------------------------------------------------------- */

/* all possible combinations of rotating conditions */

static inline int trigger_size(const struct tm* current,
                               const struct log_tm* old,
                               unsigned long max_file_size,
                               unsigned long filesize)
{
    return (filesize >= max_file_size);
}

static inline void filename_size(char* buf, int level, const struct tm* ts)
{
    sprintf(buf, "%04d-%02d-%02d_%02d:%02d:%02d.%s",
            ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday,
            ts->tm_hour, ts->tm_min, ts->tm_sec,
            log_level_str[level]);
}

static inline int trigger_hour(const struct tm* current,
                               const struct log_tm* old,
                               unsigned long max_file_size,
                               unsigned long filesize)
{
    return (! ((old->tm_hour == current->tm_hour) &&
               (old->tm_mday == current->tm_mday) &&
               (old->tm_mon == current->tm_mon) &&
               (old->tm_year == current->tm_year)));
}

static inline void filename_hour(char* buf, int level, const struct tm* ts)
{
    sprintf(buf, "%04d-%02d-%02d_%02d.%s",
            ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour,
            log_level_str[level]);
}

static inline int trigger_day(const struct tm* current,
                              const struct log_tm* old,
                              unsigned long max_file_size,
                              unsigned long filesize)
{
    return (! ((old->tm_mday == current->tm_mday) &&
               (old->tm_mon == current->tm_mon) &&
               (old->tm_year == current->tm_year)));
}

static inline void filename_day(char* buf, int level, const struct tm* ts)
{
    sprintf(buf, "%04d-%02d-%02d.%s",
            ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday,
            log_level_str[level]);
}

static inline int trigger_size_hour(const struct tm* current,
                                    const struct log_tm* old,
                                    unsigned long max_file_size,
                                    unsigned long filesize)
{
    return (trigger_size(current, old, max_file_size, filesize) ||
            trigger_hour(current, old, max_file_size, filesize));
}

static inline void filename_size_hour(char* buf, int level, const struct tm* ts)
{
    filename_size(buf, level, ts);
}

static inline int trigger_size_day(const struct tm* current,
                                   const struct log_tm* old,
                                   unsigned long max_file_size,
                                   unsigned long filesize)
{
    return (trigger_size(current, old, max_file_size, filesize) ||
            trigger_day(current, old, max_file_size, filesize));
}

static inline void filename_size_day(char* buf, int level, const struct tm* ts)
{
    filename_size(buf, level, ts);
}

static void logger_var_set_func(struct logger_var* var, unsigned flags)
{
    switch (flags & LOGGER_ROTATE_FLAG_MASK) {

        case LOGGER_ROTATE_BY_SIZE:
            var->rotate_trigger = trigger_size;
            var->get_filename = filename_size;
        break;

        case LOGGER_ROTATE_PER_HOUR:
        case LOGGER_ROTATE_PER_HOUR | LOGGER_ROTATE_PER_DAY:
            var->rotate_trigger = trigger_hour;
            var->get_filename = filename_hour;
        break;

        case LOGGER_ROTATE_PER_DAY:
            var->rotate_trigger = trigger_day;
            var->get_filename = filename_day;
        break;

        case LOGGER_ROTATE_BY_SIZE | LOGGER_ROTATE_PER_HOUR:
        case LOGGER_ROTATE_BY_SIZE | LOGGER_ROTATE_PER_HOUR | LOGGER_ROTATE_PER_DAY:
            var->rotate_trigger = trigger_size_hour;
            var->get_filename = filename_size_hour;
        break;

        case LOGGER_ROTATE_BY_SIZE | LOGGER_ROTATE_PER_DAY:
            var->rotate_trigger = trigger_size_day;
            var->get_filename = filename_size_day;
        break;

        default:
            var->rotate_trigger = trigger_day;
            var->get_filename = filename_day;
    }
}

static inline void logger_var_set_path_prefix(struct logger_var* var,
                                              const char* prefix,
                                              unsigned int max_prefix_len)
{
    unsigned int path_prefix_len = strlen(prefix);

    if (path_prefix_len > max_prefix_len) {
        fprintf(stderr, "prefix len is greater than %d, truncated.\n",
                max_prefix_len);
        path_prefix_len = max_prefix_len;
    }

    memcpy(var->path_prefix, prefix, path_prefix_len);
    var->path_prefix[path_prefix_len] = '\0';
}

static inline void logger_var_init(struct logger_var* var, const char* prefix,
                                   unsigned int flags,
                                   unsigned int max_megabytes)
{
    var->max_file_size = max_megabytes << 20;
    logger_var_set_func(var, flags);
    logger_var_set_path_prefix(var, prefix,
                               PATH_PREFIX_BUFLEN > PATH_BUFLEN - 27 ?
                               PATH_BUFLEN - 27 - 1 : PATH_PREFIX_BUFLEN - 1);
}

/* ------------------------------------------------------------------------- */

int logger_init(struct logger* l, const char* prefix,
                unsigned int flags, unsigned int max_megabytes)
{
    unsigned int i;
    struct logger_impl* handler;

    handler = malloc(sizeof(struct logger_impl));
    if (!handler)
        return -1;

    memset(handler, 0, sizeof(struct logger_impl));

    for (i = 0; i < LOG_LEVEL_MAX; ++i) {
        struct logger_info* logger = &handler->o_o[i];

        logger->level = i;
        pthread_mutex_init(&logger->lock, NULL);
    }

    logger_var_init(&handler->var, prefix, flags, max_megabytes);

    l->handler = handler;
    return 0;
}

void logger_destroy(struct logger* l)
{
    unsigned int i;
    struct logger_impl* handler = l->handler;

    if (!handler)
        return;

    for (i = 0; i < LOG_LEVEL_MAX; ++i) {
        struct logger_info* logger = &handler->o_o[i];

        if (logger->fp && logger->fp != stdout && logger->fp != stderr)
            fclose(logger->fp);

        pthread_mutex_destroy(&logger->lock);
    }

    free(handler);
    l->handler = NULL;
}
