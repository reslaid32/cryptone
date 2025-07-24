#include "log.h"
#include <stdlib.h>
#include <string.h>

LogLevel           log_level  = LOG_DEBUG;
FILE              *log_stream = NULL;

static const char *level_names[]
    = { "TRACE", "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

#ifdef _WIN32
#define COLOR_RESET   ""
#define COLOR_TRACE   ""
#define COLOR_VERBOSE ""
#define COLOR_DEBUG   ""
#define COLOR_INFO    ""
#define COLOR_WARN    ""
#define COLOR_ERROR   ""
#define COLOR_FATAL   ""
#else
#define COLOR_RESET   "\x1b[0m"
#define COLOR_TRACE   "\x1b[90m"
#define COLOR_VERBOSE "\x1b[37m"
#define COLOR_DEBUG   "\x1b[36m"
#define COLOR_INFO    "\x1b[32m"
#define COLOR_WARN    "\x1b[33m"
#define COLOR_ERROR   "\x1b[31m"
#define COLOR_FATAL   "\x1b[35;1m"
#endif

static const char *level_colors[]
    = { COLOR_TRACE, COLOR_VERBOSE, COLOR_DEBUG, COLOR_INFO,
        COLOR_WARN,  COLOR_ERROR,   COLOR_FATAL };

void
log_set_level (LogLevel level)
{
  log_level = level;
}

void
log_set_stream (FILE *stream)
{
  log_stream = stream;
}

void
log_log (LogLevel level, const char *file, int line, const char *fmt, ...)
{
  if (level < log_level)
    return;

  if (!log_stream)
    log_stream = stderr;

  time_t     t  = time (NULL);
  struct tm *lt = localtime (&t);
  char       timebuf[20];
  strftime (timebuf, sizeof (timebuf), "%Y-%m-%d %H:%M:%S", lt);

  fprintf (log_stream, "%s%s %-7s %s:%d: ", level_colors[level], timebuf,
           level_names[level], file, line);

  va_list args;
  va_start (args, fmt);
  vfprintf (log_stream, fmt, args);
  va_end (args);

  fprintf (log_stream, "%s\n", COLOR_RESET);
  fflush (log_stream);

  if (level == LOG_FATAL)
    exit (EXIT_FAILURE);
}
