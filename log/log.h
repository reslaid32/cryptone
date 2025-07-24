#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

typedef enum log_lvl
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

void     C1_LOG_SetLevel (LogLevel level);
LogLevel C1_LOG_GetLevel (void);

void     C1_LOG_SetStream (FILE *stream);
FILE    *C1_LOG_GetStream (void);

void     C1_LOG_SetAutoNewline (int autonewl);
int      C1_LOG_GetAutoNewline (void);

void     C1_LOG_Enable (int enable);
int      C1_LOG_Enabled (void);

void     C1_LOG_vprintf (const char *fmt, va_list va);
void     C1_LOG_printf (const char *fmt, ...);

void     C1_LOG_PushFrame (void);
void     C1_LOG_PopFrame (void);

void     C1_LOG_Emit (LogLevel level, const char *file, int line,
                      const char *fmt, ...);

#define C1_LOG_Trace(...)                                                 \
  C1_LOG_Emit (LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Verbose(...)                                               \
  C1_LOG_Emit (LOG_VERBOSE, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Debug(...)                                                 \
  C1_LOG_Emit (LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Info(...)                                                  \
  C1_LOG_Emit (LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Warn(...)                                                  \
  C1_LOG_Emit (LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Error(...)                                                 \
  C1_LOG_Emit (LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define C1_LOG_Fatal(...)                                                 \
  C1_LOG_Emit (LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif // LOG_H
