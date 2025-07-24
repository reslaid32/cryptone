#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

typedef enum
{
  LOG_TRACE,
  LOG_VERBOSE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  LOG_LEVEL_COUNT
} LogLevel;

extern LogLevel log_level;
extern FILE    *log_stream;

void            log_set_level (LogLevel level);
void            log_set_stream (FILE *stream);
void log_log (LogLevel level, const char *file, int line, const char *fmt,
              ...);

#define log_trace(...) log_log (LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_verbose(...)                                                  \
  log_log (LOG_VERBOSE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log (LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log (LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log (LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log (LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log (LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif // LOG_H
