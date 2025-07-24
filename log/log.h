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

void     LOG_SetLevel (LogLevel level);
LogLevel LOG_GetLevel (void);

void     LOG_SetStream (FILE *stream);
FILE    *LOG_GetStream (void);

void     LOG_SetAutoNewline (int autonewl);
int      LOG_GetAutoNewline (void);

void     LOG_Enable (int enable);
int      LOG_Enabled (void);

void     LOG_vprintf (const char *fmt, va_list va);
void     LOG_printf (const char *fmt, ...);

void     LOG_PushFrame (void);
void     LOG_PopFrame (void);

void LOG_Emit (LogLevel level, const char *file, int line, const char *fmt,
               ...);

#define LOG_Trace(...)                                                    \
  LOG_Emit (LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Verbose(...)                                                  \
  LOG_Emit (LOG_VERBOSE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Debug(...)                                                    \
  LOG_Emit (LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Info(...) LOG_Emit (LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Warn(...) LOG_Emit (LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Error(...)                                                    \
  LOG_Emit (LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_Fatal(...)                                                    \
  LOG_Emit (LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif // LOG_H
