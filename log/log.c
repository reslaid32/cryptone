#include "log.h"
#include "../error/error.h"
#include "../stack/stack.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>
#include <time.h>

typedef struct
{
  LogLevel level;
  FILE    *stream;
  int      autonewl;
  int      enabled;
} LogFrame;

LogLevel               g_log_lvl      = LOG_DEBUG;
FILE                  *g_log_stream   = NULL;
int                    g_log_autonewl = 0;
int                    g_log_enabled  = 1;

static pthread_mutex_t log_mutex      = PTHREAD_MUTEX_INITIALIZER;
Stack                  log_stack;

static const char     *level_names[]
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
#define COLOR_VERBOSE "\x1b[34m"
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
LOG_SetLevel (LogLevel level)
{
  pthread_mutex_lock (&log_mutex);
  g_log_lvl = level;
  pthread_mutex_unlock (&log_mutex);
}

LogLevel
LOG_GetLevel (void)
{
  pthread_mutex_lock (&log_mutex);
  LogLevel lvl = g_log_lvl;
  pthread_mutex_unlock (&log_mutex);
  return lvl;
}

void
LOG_SetStream (FILE *stream)
{
  pthread_mutex_lock (&log_mutex);
  g_log_stream = stream;
  pthread_mutex_unlock (&log_mutex);
}

FILE *
LOG_GetStream (void)
{
  pthread_mutex_lock (&log_mutex);
  FILE *s = g_log_stream;
  pthread_mutex_unlock (&log_mutex);
  return s;
}

void
LOG_SetAutoNewline (int autonewl)
{
  pthread_mutex_lock (&log_mutex);
  g_log_autonewl = autonewl;
  pthread_mutex_unlock (&log_mutex);
}

int
LOG_GetAutoNewline (void)
{
  pthread_mutex_lock (&log_mutex);
  int newl = g_log_autonewl;
  pthread_mutex_unlock (&log_mutex);
  return newl;
}

void
LOG_Enable (int enable)
{
  pthread_mutex_lock (&log_mutex);
  g_log_enabled = enable;
  pthread_mutex_unlock (&log_mutex);
}

int
LOG_Enabled (void)
{
  pthread_mutex_lock (&log_mutex);
  int enabled = g_log_enabled;
  pthread_mutex_unlock (&log_mutex);
  return enabled;
}

void
LOG_vprintf (const char *fmt, va_list va)
{
  pthread_mutex_lock (&log_mutex);

  if (!g_log_enabled)
  {
    pthread_mutex_unlock (&log_mutex);
    return;
  }

  vfprintf (g_log_stream, fmt, va);

  pthread_mutex_unlock (&log_mutex);
}

void
LOG_printf (const char *fmt, ...)
{
  va_list va;
  va_start (va, fmt);
  LOG_vprintf (fmt, va);
  va_end (va);
}

__attribute__ ((constructor (102))) static void
LOG_Init ()
{
  Stack_Init (&log_stack, sizeof (LogFrame), sizeof (LogFrame) * 2);
}

__attribute__ ((destructor)) static void
LOG_Shutdown ()
{
  Stack_Destroy (&log_stack);
}

void
LOG_PushFrame (void)
{
  pthread_mutex_lock (&log_mutex);

  LogFrame frame = {
    .level    = g_log_lvl,
    .stream   = g_log_stream,
    .autonewl = g_log_autonewl,
    .enabled  = g_log_enabled,
  };

  if (!Stack_Push (&log_stack, &frame))
    ferroratf (stderr, "log/frame", "failed to push frame");

  pthread_mutex_unlock (&log_mutex);
}

void
LOG_PopFrame (void)
{
  pthread_mutex_lock (&log_mutex);

  LogFrame frame;
  if (!Stack_Pop (&log_stack, &frame))
    ferroratf (stderr, "log/frame", "failed to pop frame: underflow");

  g_log_lvl      = frame.level;
  g_log_stream   = frame.stream;
  g_log_autonewl = frame.autonewl;
  g_log_enabled  = frame.enabled;

  pthread_mutex_unlock (&log_mutex);
}

void
LOG_Emit (LogLevel level, const char *file, int line, const char *fmt, ...)
{
  pthread_mutex_lock (&log_mutex);

  if (!g_log_enabled)
  {
    pthread_mutex_unlock (&log_mutex);
    return;
  }

  if (level < g_log_lvl)
  {
    pthread_mutex_unlock (&log_mutex);
    return;
  }

  if (!g_log_stream)
    g_log_stream = stderr;

  time_t     t  = time (NULL);
  struct tm *lt = localtime (&t);
  char       timebuf[20];
  strftime (timebuf, sizeof (timebuf), "%Y-%m-%d %H:%M:%S", lt);

  fprintf (g_log_stream, "%s%s %-7s %s:%d: ", level_colors[level], timebuf,
           level_names[level], file, line);

  va_list args;
  va_start (args, fmt);
  vfprintf (g_log_stream, fmt, args);
  va_end (args);

  fprintf (g_log_stream, "%s", COLOR_RESET);

  if (g_log_autonewl)
    fputc ('\n', g_log_stream);

  fflush (g_log_stream);

  pthread_mutex_unlock (&log_mutex);

  if (level == LOG_FATAL)
    abort ();
}
